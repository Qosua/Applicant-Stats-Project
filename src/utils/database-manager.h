#ifndef APPLICANTSTATSPROJECT_DATABASE_MANAGER_H
#define APPLICANTSTATSPROJECT_DATABASE_MANAGER_H

#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantList>

class DataBaseManager {

public:
    static DataBaseManager &instance();

    bool init();

    QSqlDatabase &db();

    // Read only
    QList<QVariantList> getEntryCommisionsList() const;
    int getEntryCommissionsIndexForTable(const QString& tableName) const;
    bool isTableExist(const QString& tableName) const;
    QList<QString> getEntryCommisionColumnsNamesListFromDb(const QString& name, bool isBachelor, int year) const;

    // Read/Write
    bool changeEntryCommissionsIndexForTable(const QString& tableName, int index);
    void addNewTablePreset(const QString& tableName, int commisionIndex);

private:
    DataBaseManager() = default;

    bool createDatabase();

    QSqlDatabase m_db;
};

#endif  // APPLICANTSTATSPROJECT_DATABASE_MANAGER_H
