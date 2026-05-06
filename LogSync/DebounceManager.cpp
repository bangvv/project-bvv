#include "DebounceManager.h"
#include <QMutexLocker>

void DebounceManager::markChanged(const QString& path)
{
    QMutexLocker lock(&mutex);
    lastChange[path] = QDateTime::currentDateTime();
}

QStringList DebounceManager::getStableFiles(int seconds)
{
    QStringList stable;
    QDateTime now = QDateTime::currentDateTime();

    QMutexLocker lock(&mutex);

    auto it = lastChange.begin();
    while (it != lastChange.end()) {
        if (it.value().secsTo(now) >= seconds) {
            stable << it.key();
            it = lastChange.erase(it);
        } else {
            ++it;
        }
    }

    return stable;
}
