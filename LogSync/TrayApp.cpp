#include "TrayApp.h"
#include <QMenu>
#include <QCoreApplication>
#include "ConfigDialog.h"

TrayApp::TrayApp() {
    engine = std::make_unique<SyncEngine>();
    tray.setIcon(QIcon(":/icon.png"));

    QMenu* menu = new QMenu();

    QAction* configAction = menu->addAction("Configuration");

    connect(configAction, &QAction::triggered, [=]() {
        ConfigDialog* dlg = new ConfigDialog();

        connect(dlg, &ConfigDialog::configSaved,
                this, &TrayApp::reloadEngine);

        dlg->exec();
    });

    menu->addSeparator();
    menu->addAction("Exit", qApp, &QCoreApplication::quit);

    tray.setContextMenu(menu);
    tray.show();
}

void TrayApp::reloadEngine() {
    tray.showMessage("LogSync", "Reloading configuration...");

    engine.reset();                 // destroy engine cũ
    engine = std::make_unique<SyncEngine>();  // tạo engine mới

    tray.showMessage("LogSync", "Configuration reloaded!");
}