#pragma once
#include <QObject>
#include <QTimer>
#include "DebounceManager.h"
#include "FileWatcher.h"
#include "SyncWorker.h"

class SyncEngine : public QObject {
    Q_OBJECT
public:
    SyncEngine();
    void initialScanAndEnqueue();

private:
    QString sourceFolder;
    QString destFolder;

    DebounceManager debounce;
    FileWatcher* watcher;
    SyncWorker* worker;
    QTimer timer;

    void loadConfig();
};
