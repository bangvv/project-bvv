#pragma once
#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

class SyncWorker : public QObject {
    Q_OBJECT
public:
    SyncWorker(const QString& source, const QString& dest);
    void enqueue(const QString& file);

public slots:
    void process();

private:
    QQueue<QString> queue;
    QMutex mutex;
    QWaitCondition cond;
    QString destFolder;
    QString sourceFolder;

    void copyFileSafe(const QString& src);
};
