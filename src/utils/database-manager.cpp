#include "database-manager.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

#include "support-system.h"

DataBaseManager& DataBaseManager::instance() {
    static DataBaseManager inst;
    return inst;
}

bool DataBaseManager::init() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(SupportSystem::appDbPath);

    if (!m_db.open()) {
	qCritical() << "Database error";
	return false;
    }

    QSqlQuery("PRAGMA foreign_keys = ON;", m_db);
    return createDatabase();
}

QSqlDatabase& DataBaseManager::db() { return m_db; }

QList<QVariantList> DataBaseManager::getEntryCommisionsList() const {

    if (!m_db.isOpen())
	qCritical() << "DatabaseManager could not open connection to database";

    QSqlQuery query(m_db);
    const QString strQuery = R"(SELECT uni_name, year, is_bachelor FROM entry_commisions;)";

    if (!query.exec(strQuery)) {
	qCritical() << query.lastError().text();
	return {};
    }

    QList<QVariantList> list;
    while (query.next()) {
	list << QVariantList{query.value("uni_name").toString(),
	                     query.value("is_bachelor").toBool(), query.value("year").toInt()};
    }

    return list;
}

int DataBaseManager::getEntryCommissionsIndexForTable(const QString& tableName) const {

    if (!m_db.isOpen()) {
	qCritical() << "DatabaseManager could not open connection to database";
	return -1;
    }

    QSqlQuery query(m_db);
    if (!query.prepare(
            R"(SELECT entry_commision_id FROM current_table_presets WHERE tableName = :tName)")) {
	qCritical() << query.lastError().text();
	return -1;
    }
    query.bindValue(":tName", tableName);

    if (!query.exec()) {
	qCritical() << query.lastError().text();
	return -1;
    }

    if (query.next())
	return query.value(0).toInt();

    return -1;
}

bool DataBaseManager::isTableExist(const QString& tableName) const {

    QSqlQuery query(m_db);
    query.prepare(
        R"(SELECT EXISTS(SELECT 1 FROM current_table_presets WHERE tableName = :tableName);)");
    query.bindValue(":tableName", tableName);

    if (!query.exec()) {
	qCritical() << query.lastError().text();
	return false;
    }

    if (query.next() and query.value(0).toInt() == 1)
	return true;

    return false;
}
QList<QString> DataBaseManager::getEntryCommisionColumnsNamesListFromDb(const QString& name,
                                                                        bool isBachelor,
                                                                        int year) const {

    QSqlQuery query(m_db);
    query.prepare(R"(
        select unique_code, score, achivments_score, subject_one, subject_two, subject_three, priority_number, consent, competition_group, phone_number, email, FIO, BVI, faculty_name
        from columns_mapping
        where entry_commision_id = (select id
        from entry_commisions
        where uni_name = :name and
              is_bachelor = :isBachelor and
              year = :year
            );
    )");
    query.bindValue(":name", name);
    query.bindValue(":isBachelor", isBachelor);
    query.bindValue(":year", year);

    if (!query.exec()) {
	qCritical() << query.lastError().text();
	return {};
    }

    QList<QString> strList;
    if (query.next()) {
	strList.append(query.value(0).toString());
	strList.append(query.value(1).toString());
	strList.append(query.value(2).toString());
	strList.append(query.value(3).toString());
	strList.append(query.value(4).toString());
	strList.append(query.value(5).toString());
	strList.append(query.value(6).toString());
	strList.append(query.value(7).toString());
	strList.append(query.value(8).toString());
	strList.append(query.value(9).toString());
	strList.append(query.value(10).toString());
	strList.append(query.value(11).toString());
	strList.append(query.value(12).toString());
	strList.append(query.value(13).toString());
    }

    return strList;
}

bool DataBaseManager::changeEntryCommissionsIndexForTable(const QString& tableName, int index) {
    if (!m_db.isOpen()) {
	qCritical() << "DatabaseManager could not open connection to database";
	return false;
    }

    QSqlQuery query(m_db);
    if (!query.prepare(R"(
            UPDATE current_table_presets
            SET entry_commision_id = :newID
            WHERE tableName = :tName )")) {

	qCritical() << query.lastError().text();
	return false;
    }
    query.bindValue(":tName", tableName);
    query.bindValue(":newID", index);

    if (!query.exec()) {
	qCritical() << query.lastError().text();
	return false;
    }

    return true;
}

void DataBaseManager::addNewTablePreset(const QString& tableName, int commisionIndex) {

    if (isTableExist(tableName)) {
	changeEntryCommissionsIndexForTable(tableName, commisionIndex);
	return;
    }

    QSqlQuery query(m_db);

    query.prepare(R"(
        INSERT INTO current_table_presets (tableName, entry_commision_id)
        VALUES (:tableName, :entry_commision_id) ON CONFLICT(id) DO UPDATE SET
        tableName = excluded.tableName,
        entry_commision_id = :entry_commision_id;
    )");
    query.bindValue(":tableName", tableName);
    query.bindValue(":entry_commision_id", commisionIndex);

    if (!query.exec()) {
	qCritical() << query.lastError().text();
	return;
    }
}

bool DataBaseManager::createDatabase() {

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
            "is_bachelor" boolean NOT NULL,
            "year" INTEGER NOT NULL,
            PRIMARY KEY("id"),
            CONSTRAINT "entry_commisions_unique_0" UNIQUE ("uni_name", "is_bachelor", "year")
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
        CREATE TABLE IF NOT EXISTS "current_table_presets" (
            "id" INTEGER NOT NULL,
            "tableName" TEXT NOT NULL,
            "entry_commision_id" TEXT NULL,
            PRIMARY KEY("id"),
            CONSTRAINT "current_table_presets_unique_0" UNIQUE ("entry_commision_id", "tableName")
            FOREIGN KEY ("entry_commision_id") REFERENCES "entry_commisions"("id")
            ON UPDATE CASCADE ON DELETE CASCADE
        );
    )";
    const QString strQuery5 = R"(
        CREATE INDEX IF NOT EXISTS idx_kcp_commision ON kcp(entry_commision_id)
    )";
    const QString strQuery6 = R"(
        CREATE INDEX IF NOT EXISTS idx_columns_mapping_commision ON columns_mapping(entry_commision_id)
    )";

    if (query.exec(strQuery1) and query.exec(strQuery2) and query.exec(strQuery3)
        and query.exec(strQuery4) and query.exec(strQuery5) and query.exec(strQuery6)) {
	return true;
    }

    return false;
}
