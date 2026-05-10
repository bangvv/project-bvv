#pragma once
#include <QString>
#include <SyncLogger.h>

#define TIME_WORKLOAD 10

inline void LOG(const QString& msg)
{
    SyncLogger::instance().log(msg, Q_FUNC_INFO);
}
