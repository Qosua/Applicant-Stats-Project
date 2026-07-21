#ifndef APPLICANTSTATSPROJECT_SCRIPTS_H
#define APPLICANTSTATSPROJECT_SCRIPTS_H

#include <QSqlError>
#include <QSqlQuery>
#include <QString>

#include "../xlsx.h"
#include "src/applicants-faculty-data/table-parser-bachelor.h"

namespace Scripts {
void tempScript() {
    QXlsx::Document* doc = new QXlsx::Document(":/config/KCP.xlsx");
    TableParserBachelor parser;

    QSqlQuery query(DataBaseManager::instance().db());
    query.prepare(
        "INSERT INTO kcp (entry_commision_id, code, department, profile_name, study_form, "
        "study_type, kcp_count)"
        " VALUES (:entry_commision_id, :code, :department, :profile_name, :study_form, "
        ":study_type, :kcp_count)");

    for (int i = 2; doc->read(i, 1).isValid(); ++i) {

	if (doc->read(i, 1).toString() == "/")
	    continue;

	query.bindValue(":entry_commision_id", 0);
	query.bindValue(":code", doc->read(i, 1).toString());
	query.bindValue(":department", doc->read(i, 2).toString());
	query.bindValue(":profile_name", doc->read(i, 3).toString());

	QString study_form = doc->read(i, 4).toString();
	QString study_type = doc->read(i, 5).toString();

	query.bindValue(":study_form", int(parser.extractStudyForm(study_form)));
	query.bindValue(":study_type", int(parser.extractStudyType(study_type)));

	query.bindValue(":kcp_count", doc->read(i, 6).toInt());

	if (!query.exec()) {
	    qCritical() << query.lastError().text();
	    break;
	}
    }

    delete doc;
}
}  // namespace Scripts

#endif  // APPLICANTSTATSPROJECT_SCRIPTS_H
