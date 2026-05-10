#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <atomic>

#include <QString>
#include <QVector>
#include <QDateTime>
#include <QFileInfo>

#include "SyncMode.h"

struct SyncPair {
    QString source;   // config only
    QString dest;     // config only
    SyncMode mode;    // config only
};

// Runtime state cho mỗi pair (QUAN TRỌNG)
struct PairRuntime {
    SyncPair pair;

    QVector<QString> files;  // toàn bộ relative path đã index
    int cursor = 0;          // đang chạy tới file nào
    bool indexed = false;    // đã index xong chưa
};

class SyncWorker
{
public:
    SyncWorker();
    ~SyncWorker();

    void setPairs(const std::vector<SyncPair>& pairs);
    void stop();

private:
    void run();

    void processPair(PairRuntime& runtime);

    // index toàn bộ file 1 lần duy nhất
    void buildIndex(PairRuntime& runtime);

    QString resolveDest(const SyncPair& pair,
                        const QString& relativePath,
                        const QFileInfo& sInfo);

    bool copyChunk(const QString& srcFile,
                   const QString& destFile,
                   qint64 offset,
                   qint64 size);

private:
    std::thread m_thread;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    // KHÔNG lưu SyncPair trực tiếp nữa
    std::vector<PairRuntime> m_runtimes;

    std::atomic<bool> m_running{true};
    std::atomic<bool> m_reloadRequested{false};
};
