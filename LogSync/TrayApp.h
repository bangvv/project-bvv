#pragma once
#include <QObject>
#include <QSystemTrayIcon>
#include "SyncEngine.h"
#include "SyncEngine.h"
#include <memory>

class TrayApp : public QObject {
    Q_OBJECT
public:
    TrayApp();
    void reloadEngine();

private:
    QSystemTrayIcon tray;
    std::unique_ptr<SyncEngine> engine;
};
