#include "table-parser-bachelor.h"

#include "src/utils/database-manager.h"

void TableParserBachelor::setTablePath(const QString& path) {

    if (!QFile::exists(path)) {
	qDebug() << "CAN NOT FIND TABLE FILE" << path << __FILE__ << ":" << __LINE__;
	return;
    }
    m_tablePath = path;
}

void TableParserBachelor::parseTable() {

    m_applicantsTable.reset(new QXlsx::Document(m_tablePath));
    m_applicantsList.reset(new QList<Applicant>);

    readColumnNamesFromDB();

    QHash<int, Applicant> tempHash;
    int applicantId = 0;

    for (int i = 2; m_applicantsTable->read(i, 1).isValid(); ++i) {

	if (std::expected<QVariant, TableParserError> res = getValueInTable(i, "Допущен до конкурса");
	    res.has_value() and res.value().toString().toLower() == "нет")
	    continue;

	if (std::expected<QVariant, TableParserError> res = getValueInTable(i, "Уникальный код");
	    res.has_value())
	    applicantId = res.value().toInt();
        else {
            qCritical() << __FUNCTION__ << "READING ID ISSUE" << "row index: " << i;
            continue;
        }

	if (!tempHash.contains(applicantId)) {

	    Applicant applicant;
	    applicant.setId(applicantId);

	    if (std::expected<QVariant, TableParserError> res = getValueInTable(i, "ФИО");
                res.has_value())
	        applicant.setFIO(res.value().toString());

	    if (std::expected<QVariant, TableParserError> res = getValueInTable(i, "E-mail");
                res.has_value())
	        applicant.setEmail(res.value().toString());

	    if (std::expected<QVariant, TableParserError> res = getValueInTable(i, "Телефон");
                res.has_value())
	        applicant.setPhoneNumber(res.value().toString());


	    tempHash[applicantId] = applicant;
	}

	PriorityInfo info;

        if (std::expected<QVariant, TableParserError> res = getValueInTable(i, "Сумма баллов");
            res.has_value())
            info.setEgeScore(res.value().toInt());
        else {
            qCritical() << __FUNCTION__ << "READING PRIORITY ISSUE" << "row index: " << i;
            continue;
        }

        if (std::expected<QVariant, TableParserError> res = getValueInTable(i, "Сумма баллов за инд.дост.(конкурсные)");
            res.has_value())
            info.setEgeAdditionalScore(res.value().toInt());
        else {
            qCritical() << __FUNCTION__ << "READING PRIORITY ISSUE" << "row index: " << i;
            continue;
        }

        if (std::expected<QVariant, TableParserError> res = getValueInTable(i, "Приоритет");
            res.has_value())
            info.setPriorityNumber(res.value().toInt());
        else {
            qCritical() << __FUNCTION__ << "READING PRIORITY ISSUE" << "row index: " << i;
            continue;
        }

        if (std::expected<QVariant, TableParserError> res = getValueInTable(i, "Предмет 1");
            res.has_value())
            info.addSubject(res.value().toInt());
        else {
            info.addSubject(0);
        }

        if (std::expected<QVariant, TableParserError> res = getValueInTable(i, "Предмет 2");
            res.has_value())
            info.addSubject(res.value().toInt());
        else {
            info.addSubject(0);
        }

        if (std::expected<QVariant, TableParserError> res = getValueInTable(i, "Предмет 3");
            res.has_value())
            info.addSubject(res.value().toInt());
        else {
            info.addSubject(0);
        }

        QString priorityFullName;
        if (std::expected<QVariant, TableParserError> res = getValueInTable(i, "Конкурсная группа");
            res.has_value())
            priorityFullName = res.value().toString();
        else {
            qCritical() << __FUNCTION__ << "READING PRIORITY ISSUE" << "row index: " << i;
            continue;
        }

	info.setCode(extractCode(priorityFullName));
	info.setName(extractName(priorityFullName));
	info.setStudyForm(extractStudyForm(priorityFullName));
	info.setStudyType(extractStudyType(priorityFullName));
	info.setId(applicantId);

        if (std::expected<QVariant, TableParserError> res = getValueInTable(i, "Согласие на зачисление");
            res.has_value())
            info.setAdmissionFlag((res.value().toString().toLower() == "да" ? true : false));
        else {
            qCritical() << __FUNCTION__ << "READING PRIORITY ISSUE" << "row index: " << i;
            continue;
        }

        if (std::expected<QVariant, TableParserError> res = getValueInTable(i, "Без вступительных испытаний");
            res.has_value())
            info.setIsBVI((res.value().toString().toLower() == "да" ? true : false));
        else {
            qCritical() << __FUNCTION__ << "READING PRIORITY ISSUE" << "row index: " << i;
            continue;
        }

        if (std::expected<QVariant, TableParserError> res = getValueInTable(i, "Имя факультета");
            res.has_value())
            info.setDivision(res.value().toString());
        else {
            qCritical() << __FUNCTION__ << "READING PRIORITY ISSUE" << "row index: " << i;
            continue;
        }

	tempHash[applicantId].addPriority(info);
    }

    *m_applicantsList = tempHash.values();
}

std::shared_ptr<QList<Applicant>> TableParserBachelor::getApplicants(
    ApplicantsFilterFlags flag, StudyType priorityToDelete) const {

    std::shared_ptr<QList<Applicant>> newList = std::make_shared<QList<Applicant>>();

    if (flag == ApplicantsFilterFlags::All) {
	*newList = *m_applicantsList;

	for (int i = 0; i < newList->size(); ++i) (*newList)[i].deletePriority(priorityToDelete);

	return newList;
    }
    if (flag == ApplicantsFilterFlags::AdmissionsTrue) {
	for (auto& elem : *m_applicantsList) {

	    Applicant applicant = elem;
	    applicant.priorities().clear();

	    for (const auto& priority : elem.priorities())
		if (priority.admissionFlag())
		    applicant.addPriority(priority);

	    if (applicant.priorities().size() != 0)
		newList->append(applicant);
	}

	for (int i = 0; i < newList->size(); ++i) (*newList)[i].deletePriority(priorityToDelete);

	return newList;
    }
    if (flag == ApplicantsFilterFlags::AdmissionsFalse) {
	for (auto& elem : *m_applicantsList) {

	    Applicant applicant = elem;
	    applicant.priorities().clear();

	    for (const auto& priority : elem.priorities())
		if (!priority.admissionFlag())
		    applicant.addPriority(priority);

	    if (applicant.priorities().size() != 0)
		newList->append(applicant);
	}

	for (int i = 0; i < newList->size(); ++i) (*newList)[i].deletePriority(priorityToDelete);

	return newList;
    }

    return nullptr;
}

void TableParserBachelor::readColumnNamesFromDB() {

    QList<QString> columnNames
        = DataBaseManager::instance().getEntryCommisionColumnsNamesListFromDb(m_entryCommisionName,
                                                                              m_isBachelor, m_year);

    m_columnsNames.clear();

    QList<QPair<QString, QString>> namesList(15);
    {
	int i = 0;
	namesList[i].first = "Уникальный код";
	namesList[i].second = columnNames[i];

	i += 1;
	namesList[i].first = "Сумма баллов";
	namesList[i].second = columnNames[i];

	i += 1;
	namesList[i].first = "Сумма баллов за инд.дост.(конкурсные)";
	namesList[i].second = columnNames[i];

	i += 1;
	namesList[i].first = "Предмет 1";
	namesList[i].second = columnNames[i];

	i += 1;
	namesList[i].first = "Предмет 2";
	namesList[i].second = columnNames[i];

	i += 1;
	namesList[i].first = "Предмет 3";
	namesList[i].second = columnNames[i];

	i += 1;
	namesList[i].first = "Приоритет";
	namesList[i].second = columnNames[i];

	i += 1;
	namesList[i].first = "Согласие на зачисление";
	namesList[i].second = columnNames[i];

	i += 1;
	namesList[i].first = "Конкурсная группа";
	namesList[i].second = columnNames[i];

	i += 1;
	namesList[i].first = "Телефон";
	namesList[i].second = columnNames[i];

	i += 1;
	namesList[i].first = "E-mail";
	namesList[i].second = columnNames[i];

	i += 1;
	namesList[i].first = "ФИО";
	namesList[i].second = columnNames[i];

	i += 1;
	namesList[i].first = "Без вступительных испытаний";
	namesList[i].second = columnNames[i];

	i += 1;
	namesList[i].first = "Имя факультета";
	namesList[i].second = columnNames[i];

	i += 1;
	namesList[i].first = "Допущен до конкурса";
	namesList[i].second = columnNames[i];
    }

    for (const auto& pair : namesList)
        m_columnsNames[pair.first] = -1;

    for (auto const pair : namesList) {

	for (int i = 1; m_applicantsTable->read(1, i).isValid(); ++i) {

	    QString columnNameInProgram = pair.first;
	    QString columnNameInTable = pair.second;

	    if (m_applicantsTable->read(1, i).toString() == columnNameInTable) {

		m_columnsNames[columnNameInProgram] = i;
		break;
	    }
	}
    }
}

void TableParserBachelor::setEntryCommisionInfo(const QString& entryCommisionName, bool isBachelor,
                                                int year) {
    if (entryCommisionName.isEmpty()) {
        qCritical() << Q_FUNC_INFO << ": entryCommisionName is empty";
        return;
    }

    m_entryCommisionName = entryCommisionName;
    m_isBachelor = isBachelor;
    m_year = year;
}

std::expected<QVariant, TableParserError> TableParserBachelor::getValueInTable(const int rowIndex, const QString& columnName) const {

    if (m_columnsNames[columnName] == -1)
        return std::unexpected<TableParserError>(TableParserError::noSuchColumn);

    auto value = m_applicantsTable->read(rowIndex, m_columnsNames[columnName]);
    if (value.isValid() and !value.isNull())
        return value;

    return std::unexpected<TableParserError>(TableParserError::valueError);
}

QString TableParserBachelor::extractCode(const QString& str) {

    if (str.isEmpty() or str.size() < 8) {
        qCritical() << "CODE EXTRACTION ERROR";
        return "CODE ERROR";
    }

    return str.mid(0, 8);
}

QString TableParserBachelor::extractName(const QString& str) {

    if (str.isEmpty()) {
        qCritical() << "NAME EXTRACTION ERROR";
        return "NAME ERROR";
    }

    QString name;

    for (int i = 9;
        (str.mid(i + 2, 4).toLower() != "заоч" and
         str.mid(i + 2, 4).toLower() != "очно" and
         str.mid(i + 2, 4).toLower() != "очна");
         ++i) {

        if (i >= str.size()) {
            qCritical() << "NAME EXTRACTION ERROR";
            return "NAME ERROR";
        }

	if (str[i - 1] == str[i] and str[i] == " ")
	    continue;

	name += str[i];
    }

    return name;
}

StudyForm TableParserBachelor::extractStudyForm(const QString& str) {

    if (str.isEmpty() or str.size() < 5) {
        qCritical() << "STUDY FORM EXTRACTION ERROR";
        return StudyForm::Error;
    }

    if (str.contains("Очное") or str.contains("Очная")) {
	return StudyForm::Personal;
    }
    if (str.contains("Заочная") or str.contains("Заочное")) {
	return StudyForm::NotPersonal;
    }
    if (str.contains("Очно-заочная") or str.contains("Очно-заочное")) {
	return StudyForm::PersonalNotPersonal;
    }
    return StudyForm::Error;
}

StudyType TableParserBachelor::extractStudyType(const QString& str) {

    if (str.isEmpty() or str.size() < 6) {
        qCritical() << "STUDY TYPE EXTRACTION ERROR";
        return StudyType::Error;
    }

    if (str.contains("Бюджет")) {
	return StudyType::Budget;
    }
    if (str.contains("Отдельная квота") or str.contains("Отдельное квота")) {
	return StudyType::Kvot;
    }
    if (str.contains("Особое право") or str.contains("Особая право")) {
	return StudyType::SpecialRight;
    }
    if (str.contains("Внебюджет")) {
	return StudyType::NonBudget;
    }
    if (str.contains("Целевое") or str.contains("Целевая")) {
	return StudyType::CompanySponsor;
    }
    return StudyType::Error;
}
