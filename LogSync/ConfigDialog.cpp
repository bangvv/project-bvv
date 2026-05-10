#include "ConfigDialog.h"
#include <QSettings>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QDir>
#include <QUrl>

ConfigDialog::ConfigDialog(QObject* parent)
    : QObject(parent) {
    load();
}

PairModel* ConfigDialog::model() {
    return &m_model;
}

void ConfigDialog::load()
{
    QFile file("config.json");
    if (!file.open(QIODevice::ReadOnly))
        return;

    auto doc = QJsonDocument::fromJson(file.readAll());
    auto arr = doc["pairs"].toArray();

    for (auto v : arr) {
        auto obj = v.toObject();

        int row = m_model.addPair();

        QModelIndex idx = m_model.index(row, 0);

        m_model.setData(idx, obj["source"].toString(), PairModel::SourceRole);
        m_model.setData(idx, obj["dest"].toString(), PairModel::DestRole);
    }
}

void ConfigDialog::save()
{
    qDebug() << "Saving to:";
    QJsonArray arr;

    auto items = m_model.items();
    for (const auto &p : items) {
        QJsonObject obj;
        obj["source"] = p.source;
        obj["dest"] = p.dest;
        arr.append(obj);
    }

    QJsonObject root;
    root["pairs"] = arr;

    QFile file("config.json");
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open config.json";
        return;
    }

    file.write(QJsonDocument(root).toJson());
    file.close();

    emit saved();
}

bool ConfigDialog::isMonthMode() const
{
    QSettings s("LogSync", "LogSync");
    return s.value("mode/month", false).toBool();
}

void ConfigDialog::setMonthMode(bool enable)
{
    QSettings s("LogSync", "LogSync");
    s.setValue("mode/month", enable);
}

QString ConfigDialog::normalizePath(const QString& input)
{
    QString p = input.trimmed();

    if (p.startsWith("file://", Qt::CaseInsensitive)) {
        QUrl url(p);
        p = url.toLocalFile();
    }
    p = QDir::fromNativeSeparators(p);
    p = QDir::cleanPath(p);
    if (p.endsWith('/'))
        p.chop(1);

    return p;
}
