#include "entry-commisions-list-model.h"

EntryCommisionsListModel::EntryCommisionsListModel(QObject *parent) : QAbstractListModel(parent) {

    QList<QVariantList> list = DataBaseManager::instance().getEntryCommisionsList();

    for (auto elem : list)
        m_data.append(EntryCommisionData{
            elem.at(0).toString(),
            elem.at(1).toBool(),
            elem.at(2).toInt(),
        });


}

int EntryCommisionsListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_data.size();
}

QHash<int, QByteArray> EntryCommisionsListModel::roleNames() const {
    QHash<int, QByteArray> data;
    data[EntryCommisionsListModelRoles::entryCommisionNameRole] = "entryCommisionName";
    data[EntryCommisionsListModelRoles::entryCommisionYearRole] = "entryCommisionYear";
    data[EntryCommisionsListModelRoles::entryCommisionIsBachelorRole] = "entryCommisionIsBachelor";
    data[EntryCommisionsListModelRoles::entryCommisionNameYearIsBachelorRole] = "entryCommisionNameYearIsBachelor";
    return data;
}

QVariant EntryCommisionsListModel::data(const QModelIndex &index, int role) const {
    switch (role) {
        case EntryCommisionsListModelRoles::entryCommisionNameRole:
            return m_data.at(index.row()).name;
            break;
        case EntryCommisionsListModelRoles::entryCommisionYearRole:
            return m_data.at(index.row()).year;
            break;
        case EntryCommisionsListModelRoles::entryCommisionIsBachelorRole:
            return m_data.at(index.row()).isBachelor;
            break;
        case EntryCommisionsListModelRoles::entryCommisionNameYearIsBachelorRole:
            return QVariantList {
                m_data.at(index.row()).name,
                m_data.at(index.row()).isBachelor,
                m_data.at(index.row()).year
            };
            break;
        default:
            return QVariant();
    }
}