#include "FileWatcher.h"
#include <QDir>

FileWatcher::FileWatcher(const QString& path) {
    watcher.addPath(path);

    connect(&watcher, &QFileSystemWatcher::directoryChanged, this, [=](const QString& dir){
        QDir d(dir);
        for (auto& file : d.entryList(QDir::Files))
            emit fileChanged(d.filePath(file));
    });
}