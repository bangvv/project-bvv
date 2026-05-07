#include "SyncEngine.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDirIterator>
#include <QFileInfo>
#include <QDateTime>
#include <QDebug>

#define DEBOUNCE_SECONDS 20

// =======================
// CONSTRUCTOR
// =======================
SyncEngine::SyncEngine(QObject* parent)
    : QObject(parent)
{
    loadConfig();
    setupRuntime();

    connect(&timer, &QTimer::timeout, this, [&]() {
        qDebug() << "5s call";
        auto files = debounce.getStableFiles(DEBOUNCE_SECONDS);

        // chỉ gửi đúng worker của từng pair
        for (const auto &file : files) {
            for (auto &r : m_runtime) {
                if (file.startsWith(r.watcher->sourcePath())) {
                    r.worker->enqueue(file);
                    break;
                }
            }
        }
        reconcile();
    });

    timer.start(5000);

    initialScanAndEnqueue();
}
SyncEngine::~SyncEngine()
{
    for (auto &r : m_runtime) {

        if (r.thread) {
            r.thread->quit();
            r.thread->wait();
            delete r.thread;
        }

        delete r.watcher;
        delete r.worker;
    }

    m_runtime.clear();
}
void SyncEngine::setMode(SyncMode m)
{
    m_mode = m;

    for (auto &r : m_runtime) {
        if (r.worker)
            r.worker->setMode(m);
    }
}
void SyncEngine::setupRuntime()
{
    // cleanup old runtime
    for (auto &r : m_runtime) {

        if (r.thread) {
            r.thread->quit();
            r.thread->wait();
            delete r.thread;
        }

        delete r.watcher;
        delete r.worker;
    }

    m_runtime.clear();

    // create new runtime per pair
    for (const auto &p : m_pairs) {

        RuntimePair r;

        r.watcher = new FileWatcher(p.source, this);
        r.worker  = new SyncWorker(p.source, p.dest);
        r.thread  = new QThread();
        r.source = p.source;
        r.dest   = p.dest;

        // move worker to thread
        r.worker->moveToThread(r.thread);

        connect(r.thread, &QThread::started,
                r.worker, &SyncWorker::process);

        connect(r.thread, &QThread::finished,
                r.worker, &QObject::deleteLater);

        connect(r.thread, &QThread::finished,
                r.thread, &QObject::deleteLater);

        // watcher → ONLY its worker (NO broadcast)
        connect(r.watcher, &FileWatcher::fileChanged,
                this, [this, worker = r.worker](const QString& file) {
                    debounce.markChanged(file);
                    worker->enqueue(file);
                });

        r.thread->start();

        m_runtime.push_back(r);
    }
}
void SyncEngine::initialScanAndEnqueue()
{
    for (const auto &p : m_pairs) {

        QDirIterator it(p.source,
                        QDir::Files,
                        QDirIterator::Subdirectories);

        while (it.hasNext()) {
            QString file = it.next();
            QFileInfo info(file);

            if (info.lastModified().secsTo(QDateTime::currentDateTime())
                > DEBOUNCE_SECONDS)
            {
                for (auto &r : m_runtime) {
                    if (file.startsWith(r.watcher->sourcePath())) {
                        r.worker->enqueue(file);
                        break;
                    }
                }
            }
        }
    }
}
void SyncEngine::loadConfig()
{
    QFile f("config.json");
    if (!f.open(QIODevice::ReadOnly))
        return;

    auto doc = QJsonDocument::fromJson(f.readAll());
    auto arr = doc.object()["pairs"].toArray();

    m_pairs.clear();

    for (const auto &v : arr) {
        auto obj = v.toObject();

        m_pairs.push_back({
            obj["source"].toString(),
            obj["dest"].toString()
        });
    }
}

void SyncEngine::reconcile()
{
    for (auto &r : m_runtime) {

        QDir srcDir(r.source);
        QDir destDir(r.dest);

        QDirIterator it(r.source,
                        QDir::Files,
                        QDirIterator::Subdirectories);

        while (it.hasNext()) {

            QString srcFile = it.next();

            QString relative = srcDir.relativeFilePath(srcFile);
            QString destFile = destDir.filePath(relative);

            QFileInfo srcInfo(srcFile);
            QFileInfo destInfo(destFile);
            if ((!destInfo.exists() && isStable(srcInfo)) ||
                ((destInfo.exists()&&(srcInfo.size()!=destInfo.size()) && isStable(srcInfo) && srcInfo.lastModified() > destInfo.lastModified())))
            {
                r.worker->enqueue(srcFile);
            }
        }
    }
}

bool SyncEngine::isStable(const QFileInfo& info)
{
    return info.lastModified().secsTo(QDateTime::currentDateTime()) > 10*60;
}
