#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "AppController.h"
#include "DataReceiver.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Meiko");
    QCoreApplication::setOrganizationDomain("Meiko-Elec");
    QCoreApplication::setApplicationName("Barcode");

    QQmlApplicationEngine engine;
    qmlRegisterType<AppController>("App",1,0,"AppController");
    qmlRegisterType<DataReceiver>("App",1,0,"DataReceiver");

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
