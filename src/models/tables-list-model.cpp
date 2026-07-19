#include "tables-list-model.h"

#include "src/utils/database-manager.h"

TablesListModel::TablesListModel(QObject *parent) : QAbstractListModel(parent) {

    m_watcher = new QFileSystemWatcher(this);

    QObject::connect(m_watcher, &QFileSystemWatcher::directoryChanged, this,
                     &TablesListModel::updateWatcher);

    updateWatcher("");
}

TablesListModel::~TablesListModel() {}

void TablesListModel::setNewCurrentIndex(const int& indexInArray, const int &entryCommisionIndex) {

    if (indexInArray >= m_tables.size() or indexInArray < 0)
        return;

    qDebug() << Q_FUNC_INFO << m_tables[indexInArray].m_tableName << entryCommisionIndex;
    DataBaseManager::instance().changeEntryCommissionsIndexForTable(m_tables[indexInArray].m_tableName, entryCommisionIndex);
}

int TablesListModel::rowCount(const QModelIndex &parent) const {

    Q_UNUSED(parent)
    return m_tables.size();
}

QVariant TablesListModel::data(const QModelIndex &index, int role) const {

    if (index.row() < 0 or index.row() >= m_tables.count())
	return QVariant();

    const TableData &table = m_tables[index.row()];

    switch (role) {
	case TablesListRoles::tableNameRole:
	    return table.m_tableName;
	    break;
	case TablesListRoles::tableFullPathRole:
	    return table.m_tableFullPath;
	    break;
	case TablesListRoles::tableIconPathRole:
	    return table.m_tableIconPath;
	    break;
	case TablesListRoles::tableLastChangeDateRole:
	    return table.m_tableLastChangeDate;
            break;
        case TablesListRoles::entryCommisionIdRole:
            return table.m_entryCommisionId;
            break;
        case TablesListRoles::indexInArrayRole:
            return table.m_indexInArray;
            break;
	default:
	    return QVariant();
	    break;
    }
}

QHash<int, QByteArray> TablesListModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[TablesListRoles::tableNameRole] = "tableName";
    roles[TablesListRoles::tableFullPathRole] = "tableFullPath";
    roles[TablesListRoles::tableIconPathRole] = "tableIconPath";
    roles[TablesListRoles::tableLastChangeDateRole] = "tableLastChangeDate";
    roles[TablesListRoles::entryCommisionIdRole] = "entryCommisionId";
    roles[TablesListRoles::indexInArrayRole] = "indexInArray";
    return roles;
}

void TablesListModel::updateWatcher(const QString &changedPath) {

    Q_UNUSED(changedPath);

    const QDir dataDir(SupportSystem::appDataPath);
    const QList<QString> currentFiles = dataDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    const QList<QString> watchedFiles = m_watcher->files();

    m_watcher->removePaths(watchedFiles);
    m_watcher->removePath(SupportSystem::appDataPath + "/");

    for (const QString &file : currentFiles) {

	QString fullPath = dataDir.absoluteFilePath(file);

	if (!fullPath.contains('~') and
	    !fullPath.endsWith(".tmp"))
	    m_watcher->addPath(fullPath);
    }

    m_watcher->addPath(SupportSystem::appDataPath + "/");

    if (!m_tables.isEmpty()) {
	beginRemoveRows(QModelIndex(), 0, m_tables.size() - 1);
	m_tables.clear();
	endRemoveRows();
    }

    int suitableTables = 0;
    for (const QString &tableName : currentFiles) {
        if (DataBaseManager::instance().getEntryCommissionsIndexForTable(tableName) == -1)
            continue;
        suitableTables += 1;
    }

    beginInsertRows(QModelIndex(), 0, suitableTables - 1);
    for (const QString &tableName : currentFiles) {

        int entryCommisionID = DataBaseManager::instance().getEntryCommissionsIndexForTable(tableName);
        if (entryCommisionID == -1)
            continue;

        m_tables.append(TableData(tableName,
                                    dataDir.absoluteFilePath(tableName),
                                    "qrc:/resources/icons/excel-icon.png",
                                    tableLastChangeDate(tableName),
                                    entryCommisionID,
                                    m_tables.size()
                                ));
    }
    endInsertRows();
}

QString TablesListModel::tableLastChangeDate(const QString &tableName) {

    const QFileInfo fileInfo(SupportSystem::appDataPath + "/" + tableName);
    const QString stringDateTime = fileInfo.lastModified().toString("yyyy.MM.dd hh:mm:ss");
    return stringDateTime;
}