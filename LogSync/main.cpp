#include <QApplication>
#include "TrayApp.h"
#include "SyncEngine.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    if (argc > 1 && QString(argv[1]) == "--service") {
        SyncEngine engine;
        return app.exec();
    }

    TrayApp tray;
    return app.exec();
}
