#include "SyncWorker.h"
#include <QFile>
#include <QFileInfo>
#include <QThread>
#include <QDir>
#include <QDateTime>
#include <QDebug>

SyncWorker::SyncWorker(const QString& source, const QString& dest)
    : destFolder(dest), sourceFolder(source)
{
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
    QFileInfo srcInfo(src);

    // ✅ Lấy relative path chuẩn
    QDir sourceRoot(sourceFolder);
    QString relativePath = sourceRoot.relativeFilePath(src);

    QString dest = QDir(destFolder).filePath(relativePath);

    QFileInfo destInfo(dest);

    // Tạo thư mục đích nếu chưa có
    QDir().mkpath(QFileInfo(dest).path());

    // Nếu file giống nhau → bỏ qua
    if (destInfo.exists()) {
        if (destInfo.size() == srcInfo.size() &&
            destInfo.lastModified() >= srcInfo.lastModified()) {
            return;
        }
    }

    QFile in(src);
    if (!in.open(QIODevice::ReadOnly))
        return;

    QFile out(dest);
    if (!out.open(QIODevice::WriteOnly))
        return;

    out.write(in.readAll());
}
