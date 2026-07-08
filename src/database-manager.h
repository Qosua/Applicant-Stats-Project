#ifndef APPLICANTSTATSPROJECT_DATABASE_MANAGER_H
#define APPLICANTSTATSPROJECT_DATABASE_MANAGER_H

#include <QSql>
#include <QSqlError>
#include <QSqlQuery>

#include "support-system.h"

class DataBaseManager {

public:
    static DataBaseManager& instance() {
	static DataBaseManager theData;
	return theData;
    }

    bool init() {
	m_db = QSqlDatabase::addDatabase("QSQLITE");
	m_db.setDatabaseName(SupportSystem::appDbPath);

	if (!m_db.open()) {
	    qCritical() << "Database error";
	    return false;
	}

	QSqlQuery("PRAGMA foreign_keys = ON;", m_db);
	return createDatabase();
    }

private:
    DataBaseManager() = default;

    bool createDatabase() {
	QSqlQuery query(m_db);
	const QString strQuery = R"(
            CREATE TABLE IF NOT EXISTS test (
                id INTEGER PRIMARY KEY,
                someInfo TEXT
            );
        )";

	if (!query.exec(strQuery)) {
	    qCritical() << "Failed to create table `test`";
	    return false;
	}
	return true;
    }

    QSqlDatabase m_db;
};

#endif  // APPLICANTSTATSPROJECT_DATABASE_MANAGER_H
