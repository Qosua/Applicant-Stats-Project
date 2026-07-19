#ifndef APPLICANTSTATSPROJECT_TABLESLISTMODEL_H
#define APPLICANTSTATSPROJECT_TABLESLISTMODEL_H

#include <QAbstractListModel>
#include <QDir>
#include <QFileSystemWatcher>
#include <QList>
#include <QString>

#include "../utils/support-system.h"

class TablesListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum TablesListRoles {
	tableNameRole = Qt::UserRole + 1,
	tableFullPathRole,
	tableIconPathRole,
	tableLastChangeDateRole,
	entryCommisionIdRole,
	indexInArrayRole,
    };
    struct TableData {
	TableData(const QString &tableName, const QString &filePath, const QString &iconPath,
	          const QString &lastChangeDate, const int& entryCommisionId, const int& indexInArray)
	    : m_tableName(tableName),
	      m_tableFullPath(filePath),
	      m_tableIconPath(iconPath),
	      m_tableLastChangeDate(lastChangeDate),
	      m_entryCommisionId(entryCommisionId),
	      m_indexInArray(indexInArray) {}

	QString m_tableName;
	QString m_tableFullPath;
	QString m_tableIconPath;
	QString m_tableLastChangeDate;
        int m_entryCommisionId;
        int m_indexInArray;
    };

    TablesListModel(QObject *parent = nullptr);
    ~TablesListModel();

    Q_INVOKABLE void setNewCurrentIndex(const int& indexInArray, const int &entryCommisionIndex);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void updateWatcher(const QString &changedPath);
    static QString tableLastChangeDate(const QString &tableName);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<TableData> m_tables;
    QFileSystemWatcher *m_watcher;
};

#endif  // APPLICANTSTATSPROJECT_TABLESLISTMODEL_H
