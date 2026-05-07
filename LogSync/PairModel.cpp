#include "PairModel.h"

PairModel::PairModel(QObject*) {}

int PairModel::rowCount(const QModelIndex&) const
{
    return qMax(0, m_data.size());   // chặn âm
}

QVariant PairModel::data(const QModelIndex& index, int role) const {
    const auto& p = m_data[index.row()];
    if (role == SourceRole) return p.source;
    if (role == DestRole) return p.dest;
    return {};
}

bool PairModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
        return false;

    int row = index.row();
    if (row < 0 || row >= m_data.size())
        return false;

    if (role == SourceRole)
        m_data[row].source = value.toString();
    else if (role == DestRole)
        m_data[row].dest = value.toString();
    else
        return false;

    emit dataChanged(index, index, {role});
    return true;
}
Qt::ItemFlags PairModel::flags(const QModelIndex&) const {
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QHash<int, QByteArray> PairModel::roleNames() const {
    return {
        {SourceRole, "source"},
        {DestRole, "dest"}
    };
}

int PairModel::addPair()
{
    beginInsertRows({}, m_data.size(), m_data.size());
    m_data.push_back({});
    endInsertRows();

    return m_data.size() - 1;
}

void PairModel::removePair(int row) {
    beginRemoveRows({}, row, row);
    m_data.remove(row);
    endRemoveRows();
}

QVector<Pair> PairModel::items() const {
    return m_data;
}
