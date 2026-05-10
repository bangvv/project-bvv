#pragma once
#include <QObject>
#include "PairModel.h"
#include <QSettings>

class ConfigDialog : public QObject {
    Q_OBJECT
    Q_PROPERTY(PairModel* model READ model CONSTANT)

public:
    explicit ConfigDialog(QObject* parent = nullptr);

    PairModel* model();

    Q_INVOKABLE void save();
    Q_INVOKABLE bool isMonthMode() const;
    Q_INVOKABLE void setMonthMode(bool enable);
    QString normalizePath(const QString& input);

signals:
    void saved();

private:
    PairModel m_model;
    void load();
};
