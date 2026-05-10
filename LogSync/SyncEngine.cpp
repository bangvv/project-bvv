#include "SyncEngine.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSettings>
#include <QJsonObject>
#include <QDebug>
#include "config.h"

SyncEngine::SyncEngine(QObject* parent)
    : QObject(parent)
{
    LOG("tao moi SyncEngine");
    loadConfig();
}

SyncEngine::~SyncEngine() {}

void SyncEngine::loadConfig()
{
    QFile f("config.json");
    if (!f.open(QIODevice::ReadOnly))
        return;

    QSettings s("LogSync", "LogSync");
    bool monthMode = s.value("mode/month", false).toBool();

    SyncMode mode = monthMode
            ? SyncMode::LastModifiedMonth
            : SyncMode::Fixed;

    auto arr = QJsonDocument::fromJson(f.readAll())
                   .object()["pairs"].toArray();

    std::vector<SyncPair> pairs;

    for (auto v : arr) {
        auto o = v.toObject();
        pairs.push_back({
            o["source"].toString(),
            o["dest"].toString(),
            mode
        });
    }

    m_worker.setPairs(pairs);
}
