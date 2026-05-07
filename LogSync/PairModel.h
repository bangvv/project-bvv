#pragma once
#include <QAbstractListModel>

struct Pair {
    QString source;
    QString dest;
};

class PairModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
            SourceRole = Qt::UserRole + 1,
            DestRole
        };
        Q_ENUM(Roles)
    PairModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Qt::ItemFlags flags(const QModelIndex&) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVector<Pair> items() const;

    Q_INVOKABLE int addPair();
    Q_INVOKABLE void removePair(int row);

private:
    QVector<Pair> m_data;
};
