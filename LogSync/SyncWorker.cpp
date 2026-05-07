#include "SyncWorker.h"
#include <QFile>
#include <QFileInfo>
#include <QThread>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QSettings>

bool safeCopy(const QString& src, const QString& dest)
{
    if (!QFile::exists(src))
        return false;

    QDir().mkpath(QFileInfo(dest).path());

    QString tmp = dest + ".tmp";

    QFile::remove(tmp);

    QFile in(src);
    if (!in.open(QIODevice::ReadOnly))
        return false;

    QFile out(tmp);
    if (!out.open(QIODevice::WriteOnly))
        return false;

    char buffer[64 * 1024];
    qint64 len;

    while ((len = in.read(buffer, sizeof(buffer))) > 0) {
        out.write(buffer, len);
    }

    out.flush();
    out.close();
    in.close();

    QFile::remove(dest);
    return QFile::rename(tmp, dest);
}

SyncWorker::SyncWorker(const QString& source, const QString& dest)
    : destFolder(dest), sourceFolder(source)
{
    QSettings s("LogSync", "LogSync");

    bool monthMode = s.value("mode/month", false).toBool();

    mode = monthMode ? SyncMode::LastModifiedMonth
                     : SyncMode::Fixed;
}

void SyncWorker::enqueue(const QString& file) {
    QMutexLocker lock(&mutex);
    queue.enqueue(file);
    cond.wakeOne();
    qDebug() <<" add enqueue to wakeOne thread";
}

void SyncWorker::process() {
    while (true) {
        mutex.lock();

        while (queue.isEmpty()) {
            cond.wait(&mutex);
        }

        QString file = queue.dequeue();
        mutex.unlock();

        copyFileSafe(file);
    }
}

void SyncWorker::copyFileSafe(const QString& src)
{
    QString dest = resolveDestPath(src);

    QFileInfo srcInfo(src);
    QFileInfo destInfo(dest);

    QDir().mkpath(QFileInfo(dest).path());

    if (destInfo.exists()) {
        qDebug() <<" destInfo:"+destInfo.baseName();
        if (destInfo.size() == srcInfo.size() &&
            destInfo.lastModified() >= srcInfo.lastModified()) {
            if (destInfo.baseName() == "abx"){
                qDebug() <<" size:"+QString::number(destInfo.size());
                qDebug() <<" size:"+QString::number(srcInfo.size());
            }
            return;
        }
    }
    qDebug() <<" copy:"+src + " to:"+dest;
    safeCopy(src, dest);
}



QString SyncWorker::resolveDestPath(const QString& src)
{
    QFileInfo srcInfo(src);

    QDir sourceRoot(sourceFolder);
    QString relativePath = sourceRoot.relativeFilePath(src);

    QString base = destFolder;

    if (mode == SyncMode::LastModifiedMonth) {
        QString month = srcInfo.lastModified().toString("yyyy-MM");
        base = QDir(base).filePath(month);
    }

    return QDir(base).filePath(relativePath);
}

void SyncWorker::setMode(SyncMode m)
{
    mode = m;
}
