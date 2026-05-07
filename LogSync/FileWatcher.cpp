#include "FileWatcher.h"

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>

FileWatcher::FileWatcher(const QString& path, QObject* parent)
    : QObject(parent),
      m_sourcePath(QDir(path).absolutePath())
{
    m_watcher.addPath(m_sourcePath);

    connect(&m_watcher, &QFileSystemWatcher::directoryChanged,
            this, &FileWatcher::onDirectoryChanged);

    connect(&m_watcher, &QFileSystemWatcher::fileChanged,
            this, &FileWatcher::onFileChanged);

    addRecursive(m_sourcePath);
}

void FileWatcher::onDirectoryChanged(const QString& path)
{
    if (!QFileInfo::exists(path)) {
        m_watcher.removePath(path);
        return;
    }

    addRecursive(path);

    if (!m_watcher.directories().contains(path))
        m_watcher.addPath(path);
}

void FileWatcher::onFileChanged(const QString& path)
{
    if (!QFileInfo::exists(path)) {
        m_watcher.removePath(path);
        return;
    }

    emit fileChanged(path);
}

void FileWatcher::addRecursive(const QString& dir)
{
    QDirIterator it(dir,
                    QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString path = it.next();

        if (QFileInfo(path).isDir()) {
            if (!m_watcher.directories().contains(path))
                m_watcher.addPath(path);
        } else {
            if (!m_watcher.files().contains(path))
                m_watcher.addPath(path);
        }
    }
}
