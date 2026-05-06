#pragma once
#include <QObject>
#include <QMap>
#include <QDateTime>
#include <QMutex>

class DebounceManager : public QObject {
    Q_OBJECT
public:
    void markChanged(const QString& path);
    QStringList getStableFiles(int seconds);

private:
    QMap<QString, QDateTime> lastChange;
    QMutex mutex;
};
