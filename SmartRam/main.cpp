#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "ramcleaner.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    RamCleaner cleaner;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("ramCleaner", &cleaner);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
