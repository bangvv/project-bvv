#include "TrayApp.h"
#include <QMenu>
#include <QCoreApplication>
#include "ConfigDialog.h"
#include <QQmlContext>
#include <QTimer>
#include <QDebug>


TrayApp::TrayApp() {
    engine = std::make_unique<SyncEngine>();
    tray.setIcon(QIcon(":/icon.png"));

    QMenu* menu = new QMenu();
    QAction* configAction = menu->addAction("Configuration");

    connect(configAction, &QAction::triggered, [=]() {
        qmlEngine = std::make_unique<QQmlApplicationEngine>();
        config = std::make_unique<ConfigDialog>();

        connect(config.get(), &ConfigDialog::saved,
                this, &TrayApp::reloadEngine);

        connect(config.get(), &ConfigDialog::saved, [this]() {
            QTimer::singleShot(0, this, [this]() {
                qDebug() <<" close";
                qmlEngine.reset();
            });
        });

        qmlEngine->rootContext()->setContextProperty("config", config.get());
        qmlEngine->load(QUrl("qrc:/qml/ConfigDialog.qml"));
    });

    menu->addSeparator();
    menu->addAction("Exit", qApp, &QCoreApplication::quit);

    tray.setContextMenu(menu);
    tray.show();
}

void TrayApp::reloadEngine() {
    tray.showMessage("LogSync", "Reloading configuration...");

    QTimer::singleShot(0, this, [this]() {
        engine.reset();
        engine = std::make_unique<SyncEngine>();

        tray.showMessage("LogSync", "Configuration reloaded!");
    });
}
