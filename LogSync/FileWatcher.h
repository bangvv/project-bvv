#pragma once

#include <QObject>
#include <QFileSystemWatcher>
#include <QString>

class FileWatcher : public QObject {
    Q_OBJECT

public:
    explicit FileWatcher(const QString& path, QObject* parent = nullptr);

    QString sourcePath() const { return m_sourcePath; }

signals:
    void fileChanged(const QString& filePath);

private slots:
    void onDirectoryChanged(const QString& path);
    void onFileChanged(const QString& path);

private:
    QFileSystemWatcher m_watcher;
    QString m_sourcePath;

    void addRecursive(const QString& dir);
};
