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

    QSqlDatabase& db() { return m_db; }

private:
    DataBaseManager() = default;

    bool createDatabase() {

	QSqlQuery query(m_db);

	const QString strQuery1 = R"(
            CREATE TABLE IF NOT EXISTS "columns_mapping" (
	        "id" INTEGER NOT NULL,
	        "entry_commision_id" INTEGER NOT NULL,
	        "unique_code" TEXT NOT NULL,
	        "score" TEXT NOT NULL,
	        "achivments_score" TEXT NOT NULL,
	        "subject_one" TEXT NOT NULL,
	        "subject_two" TEXT NOT NULL,
	        "subject_three" TEXT NOT NULL,
	        "priority_number" TEXT NOT NULL,
	        "consent" TEXT NOT NULL,
	        "competition_group" TEXT NOT NULL,
	        "phone_number" TEXT,
	        "email" TEXT,
	        "FIO" TEXT,
	        "BVI" TEXT NOT NULL,
	        "faculty_name" TEXT NOT NULL,
	        PRIMARY KEY("id"),
	        FOREIGN KEY ("entry_commision_id") REFERENCES "entry_commisions"("id")
	        ON UPDATE CASCADE ON DELETE CASCADE
            );
        )";
        const QString strQuery2 = R"(
            CREATE TABLE IF NOT EXISTS "entry_commisions" (
	            "id" INTEGER NOT NULL,
	            "uni_name" TEXT NOT NULL,
	            "year" INTEGER NOT NULL,
	            PRIMARY KEY("id"),
	            CONSTRAINT "entry_commisions_unique_0" UNIQUE ("uni_name", "year")
            );
        )";
        const QString strQuery3 = R"(
            CREATE TABLE IF NOT EXISTS "kcp" (
	            "id" INTEGER NOT NULL,
	            "entry_commision_id" INTEGER NOT NULL,
	            "code" TEXT NOT NULL,
	            "department" TEXT NOT NULL,
	            "profile_name" TEXT NOT NULL,
	            "study_form" INTEGER NOT NULL,
	            "study_type" INTEGER NOT NULL,
	            "kcp_count" INTEGER NOT NULL,
	            PRIMARY KEY("id"),
                    CONSTRAINT "kcp_unique_0" UNIQUE ("entry_commision_id", "code", "department", "profile_name", "study_form", "study_type")
	            FOREIGN KEY ("entry_commision_id") REFERENCES "entry_commisions"("id")
	            ON UPDATE CASCADE ON DELETE CASCADE
            );
        )";
        const QString strQuery4 = R"(
            CREATE INDEX IF NOT EXISTS idx_kcp_commision ON kcp(entry_commision_id)
        )";
        const QString strQuery5 = R"(
            CREATE INDEX IF NOT EXISTS idx_columns_mapping_commision ON columns_mapping(entry_commision_id)
        )";

	if (query.exec(strQuery1) and query.exec(strQuery2) and
	    query.exec(strQuery3) and query.exec(strQuery4) and
	    query.exec(strQuery5)) {
	    return true;
	}

	return false;
    }

    QSqlDatabase m_db;
};

#endif  // APPLICANTSTATSPROJECT_DATABASE_MANAGER_H
