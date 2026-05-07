#pragma once
#include <QObject>
#include <QSystemTrayIcon>
#include "SyncEngine.h"
#include "SyncEngine.h"
#include <memory>
#include <QQmlApplicationEngine>
#include "ConfigDialog.h"

class TrayApp : public QObject {
    Q_OBJECT
public:
    TrayApp();
    void reloadEngine();

private:
    QSystemTrayIcon tray;
    std::unique_ptr<SyncEngine> engine;
    std::unique_ptr<QQmlApplicationEngine> qmlEngine;
    std::unique_ptr<ConfigDialog> config;
};
