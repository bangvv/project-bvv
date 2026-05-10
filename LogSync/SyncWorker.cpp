#include "SyncWorker.h"
#include <QDirIterator>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include "config.h"

static const qint64 CHUNK_SIZE = 1024 * 1024; // 1MB
static const int ACTIVE_DAYS = 15;

SyncWorker::SyncWorker()
{
    m_thread = std::thread(&SyncWorker::run, this);
}

SyncWorker::~SyncWorker()
{
    stop();
}

void SyncWorker::stop()
{
    m_running = false;
    m_cv.notify_all();
    if (m_thread.joinable())
        m_thread.join();
}

void SyncWorker::setPairs(const std::vector<SyncPair>& pairs)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_reloadRequested = true;
    m_runtimes.clear();

    for (const auto& p : pairs) {
        PairRuntime rt;
        rt.pair = p;
        m_runtimes.push_back(rt);
    }

    m_cv.notify_one();
}

void SyncWorker::buildIndex(PairRuntime& runtime)
{
    LOG(QString("Indexing: %1").arg(runtime.pair.source));

    QDirIterator it(runtime.pair.source,
                    QDir::Files,
                    QDirIterator::Subdirectories);

    QDir srcRoot(runtime.pair.source);

    while (it.hasNext()) {
        QString abs = it.next();
        QString rel = srcRoot.relativeFilePath(abs);
        runtime.files.push_back(rel);
    }

    runtime.indexed = true;
    runtime.cursor = 0;

    LOG(QString("Indexed files: %1").arg(runtime.files.size()));
}

void SyncWorker::run()
{
    std::unique_lock<std::mutex> lock(m_mutex);

    while (m_running) {

        m_cv.wait_for(lock, std::chrono::seconds(TIME_WORKLOAD));

        if (!m_running)
            break;

        auto runtimes = m_runtimes;
        m_reloadRequested = false;

        lock.unlock();

        for (auto& rt : runtimes) {
            if (!m_running) break;
            if (m_reloadRequested)
                break;
            processPair(rt);
        }

        lock.lock();
    }
}

QString SyncWorker::resolveDest(const SyncPair& pair,
                                const QString& relativePath,
                                const QFileInfo& sInfo)
{
    QDir destRoot(pair.dest);

    if (pair.mode == SyncMode::Fixed) {
        return QDir::cleanPath(destRoot.filePath(relativePath));
    }

    QString month = sInfo.lastModified().toString("yyyy-MM");

    return QDir::cleanPath(
        destRoot.filePath(month + "/" + relativePath));
}

void SyncWorker::processPair(PairRuntime& runtime)
{
    if (!runtime.indexed)
        buildIndex(runtime);

    const SyncPair& pair = runtime.pair;

    int processed = 0;

    while (processed < 20 && runtime.cursor < runtime.files.size()) {

        if (m_reloadRequested)
            return;

        QString relative = runtime.files[runtime.cursor++];
        QString srcFile = QDir(pair.source).filePath(relative);

        QFileInfo sInfo(srcFile);
        if (!sInfo.exists())
            continue;

        // chỉ sync file mới sửa gần đây
        if (sInfo.lastModified().daysTo(QDateTime::currentDateTime()) > ACTIVE_DAYS)
            continue;

        QString destFile = resolveDest(pair, relative, sInfo);

        QFileInfo dInfo(destFile);

        qint64 already = dInfo.exists() ? dInfo.size() : 0;

        if (already >= sInfo.size())
            continue;

        QDir().mkpath(QFileInfo(destFile).path());

        copyChunk(srcFile, destFile, already,
                  qMin(CHUNK_SIZE, sInfo.size() - already));

        LOG(QString("Copy 1MB: %1").arg(destFile));

        processed++;
    }

    // quay vòng lại từ đầu
    if (runtime.cursor >= runtime.files.size())
        runtime.cursor = 0;
}

bool SyncWorker::copyChunk(const QString& srcFile,
               const QString& destFile,
               qint64 offset,
               qint64 size)
{
    QFile src(srcFile);
    if (!src.open(QIODevice::ReadOnly))
        return false;

    if (!src.seek(offset)) {
        src.close();
        return false;
    }

    QByteArray data = src.read(size);
    src.close();   // đóng NGAY lập tức

    QDir().mkpath(QFileInfo(destFile).path());

    QFile dest(destFile);
    if (!dest.open(QIODevice::Append))
        return false;

    dest.write(data);
    dest.close();  // đóng NGAY

    return true;
}
