#pragma once
#include <QObject>
#include "SyncWorker.h"

class SyncEngine : public QObject
{
    Q_OBJECT
public:
    SyncEngine(QObject* parent = nullptr);
    ~SyncEngine();

private:
    void loadConfig();

    SyncWorker m_worker;
};
