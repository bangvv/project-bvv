#include "SyncEngine.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QThread>
#include <QDebug>
#include <QDirIterator>

#define DEBOUNCE_SECONDS 20

void SyncEngine::loadConfig() {
    QFile f("config.json");
    if (!f.open(QIODevice::ReadOnly))
        return;

    auto obj = QJsonDocument::fromJson(f.readAll()).object();
    sourceFolder = obj["source"].toString();
    destFolder   = obj["dest"].toString();
}

SyncEngine::SyncEngine()
{
    loadConfig();   // ← đọc config trước

    watcher = new FileWatcher(sourceFolder);
    worker  = new SyncWorker(sourceFolder,destFolder);

    // file thay đổi → debounce
    connect(watcher, &FileWatcher::fileChanged,
            &debounce, &DebounceManager::markChanged);

    // timer check file ổn định
    connect(&timer, &QTimer::timeout, this, [&]() {
        qDebug() <<" QTimer::timeout 5s";
        auto files = debounce.getStableFiles(DEBOUNCE_SECONDS);
        for (auto& f : files)
            worker->enqueue(f);
    });

    timer.start(5000);

    // worker thread
    QThread* t = QThread::create([&](){ worker->process(); });
    t->start();

    initialScanAndEnqueue();
}

void SyncEngine::initialScanAndEnqueue()
{
    QDirIterator it(sourceFolder, QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString file = it.next();
        QFileInfo info(file);

        // Chỉ enqueue nếu file đã "yên" đủ lâu
        if (info.lastModified().secsTo(QDateTime::currentDateTime()) > DEBOUNCE_SECONDS) {
            worker->enqueue(file);
        }
    }
}
