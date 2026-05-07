#pragma once
#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

enum class SyncMode {
    Fixed,
    LastModifiedMonth
};

class SyncWorker : public QObject {
    Q_OBJECT
public:
    SyncWorker(const QString& source, const QString& dest);
    void enqueue(const QString& file);
    QString resolveDestPath(const QString& src);
    void setMode(SyncMode m);
public slots:
    void process();

private:
    SyncMode mode = SyncMode::Fixed;
    QQueue<QString> queue;
    QMutex mutex;
    QWaitCondition cond;
    QString destFolder;
    QString sourceFolder;

    void copyFileSafe(const QString& src);
};
