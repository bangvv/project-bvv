#pragma once
#include <QObject>

class RamCleaner : public QObject
{
    Q_OBJECT
public:
    explicit RamCleaner(QObject *parent = nullptr);

    Q_INVOKABLE void cleanRam();
    Q_INVOKABLE qint64 getUsedRamMB();
};
