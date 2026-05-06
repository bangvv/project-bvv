#pragma once
#include <QObject>
#include <QFileSystemWatcher>

class FileWatcher : public QObject {
    Q_OBJECT
public:
    FileWatcher(const QString& path);

signals:
    void fileChanged(const QString& path);

private:
    QFileSystemWatcher watcher;
};