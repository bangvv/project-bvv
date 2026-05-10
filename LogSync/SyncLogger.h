#pragma once

#include <QString>
#include <QFile>
#include <QDate>
#include <QDateTime>
#include <QMutex>
#include <QThread>
#include <QDir>

class SyncLogger
{
public:
    static SyncLogger& instance()
    {
        static SyncLogger inst;
        return inst;
    }

    void log(const QString& classFunc,
             const QString& msg)
    {
        QMutexLocker lock(&m_mutex);

        QFile file(currentFile());
        if (!file.open(QIODevice::Append | QIODevice::Text))
            return;

        QString time = QDateTime::currentDateTime()
                           .toString("HH:mm:ss");

        QString threadId = QString::number(
            reinterpret_cast<quintptr>(QThread::currentThreadId()));

        QString line = QString("[%1][%2][T%3] %4\n")
                           .arg(time)
                           .arg(classFunc)
                           .arg(threadId)
                           .arg(msg);

        file.write(line.toUtf8());
    }

private:
    QString currentFile()
    {
        QString dir = "logs";
        QDir().mkpath(dir);

        return dir + "/LogApp/LogSync/" +
               QDate::currentDate().toString("yyyy-MM-dd") +
               ".log";
    }

private:
    QMutex m_mutex;

    SyncLogger() = default;
};
