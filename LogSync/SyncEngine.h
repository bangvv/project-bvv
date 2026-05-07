#pragma once

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QThread>
#include "DebounceManager.h"
#include "FileWatcher.h"
#include "SyncWorker.h"
#include <QFileInfo>

class SyncEngine : public QObject {
    Q_OBJECT

public:
    explicit SyncEngine(QObject* parent = nullptr);
    ~SyncEngine();

    void initialScanAndEnqueue();

    void setMode(SyncMode m);
    SyncMode mode() const { return m_mode; }
    void reconcile();
    bool isStable(const QFileInfo& info);
private:
    struct Pair {
        QString source;
        QString dest;
    };

    struct RuntimePair {
        QString source;
        QString dest;

        FileWatcher* watcher = nullptr;
        SyncWorker*  worker  = nullptr;
        QThread*     thread  = nullptr;
    };

    QVector<Pair> m_pairs;
    QVector<RuntimePair> m_runtime;

    DebounceManager debounce;
    QTimer timer;

    SyncMode m_mode = SyncMode::Fixed;

    void loadConfig();
    void setupRuntime();
};
