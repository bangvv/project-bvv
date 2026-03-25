#include "AppController.h"
#include <QDebug>

AppController::AppController(QObject *parent)
    : QObject(parent), m_appName("My App") {}


//-------------------------------------------------
//                      Getter
//-------------------------------------------------

QString AppController::appName() const { return m_appName; }
int AppController::getCount() const { return m_count; }


//-------------------------------------------------
//                      Setter
//-------------------------------------------------
void AppController::setAppName(const QString &name) {
    if (m_appName == name) return;
    m_appName = name;
    emit appNameChanged();
}

void AppController::setCount(const int &count) {
    if (m_count == count) return;
    m_count = count;
    emit countChanged();
}


//-------------------------------------------------
//                 Hàm gọi từ QML
//-------------------------------------------------

void AppController::changeName(const QString &name) { setAppName(name); }
void AppController::changeCount(const int &count) { setCount(count); }
