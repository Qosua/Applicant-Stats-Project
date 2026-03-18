#include "src/magichat.h"

MagicHat::MagicHat() { m_facultyCells = new QList<FacultyCell>; }

QList<Applicant> MagicHat::applicantsList() const { return m_applicantsList; }

void MagicHat::setApplicantsList(QList<Applicant>* newApplicantsList) {

    m_applicantsList = *newApplicantsList;
    m_applicantsListCopy = *newApplicantsList;
}

void MagicHat::startPriorityRoundSimulation() {

    QList<Applicant> applicantsList;

    for (auto& elem : m_applicantsList) {

	Applicant tempApplicant = elem;
	tempApplicant.priorities().clear();

	for (const auto& priority : std::as_const(elem.priorities())) {

	    if (priority.studyType() != StudyType::Budget or priority.isBVI()) {

		tempApplicant.addPriority(priority);
	    }
	}

	applicantsList.append(tempApplicant);
    }

    for (int i = -1; applicantsList.size() > 0;) {

	if (applicantsList.size() == 0)  //???
	    break;

	i = (i + 1) % applicantsList.size();

	Applicant* applicant = &applicantsList[i];
	PriorityInfo priority;

	if (applicant->priorities().size() > 0)
	    priority = applicant->priorities().first();
	else {
	    applicantsList.removeOne(*applicant);
	    continue;
	}

	bool deletePriority = true;
	for (int j = 0; j < m_facultyCells->size(); ++j) {

	    FacultyCell* facultyCell = &(*m_facultyCells)[j];

	    if (facultyCell->isAbleToAdd(priority)) {

		deletePriority = false;

		applicant->priorities().removeFirst();
		facultyCell->addToPool(priority, *applicant);
		applicantsList.removeOne(*applicant);

		QPair<PriorityInfo, Applicant*> unsuitable = facultyCell->getUnsuitableApplicant();

		if (unsuitable.second != nullptr) {

		    unsuitable.second->addPriority(unsuitable.first);
		    applicantsList.append(*unsuitable.second);

		    delete unsuitable.second;
		    unsuitable.second = nullptr;
		}
	    }
	}

	if (deletePriority)
	    applicant->priorities().removeFirst();
    }

    // TODO: make less then O(10 * m * n)
    for (auto& elem : (*m_facultyCells)) {

	const auto& applicants = elem.pool();
	for (auto& applicant : applicants) {

	    for (int i = 0; i < m_applicantsList.size(); ++i) {

		if (applicant.second.id() == m_applicantsList[i].id()) {
		    m_applicantsList.remove(i);
		    --i;
		}
	    }
	}
    }

    qDebug() << "Priority simulation completed";
}

void MagicHat::startGeneralRoundSimulation() {

    QList<Applicant> generalList;

    for (auto& applicant : m_applicantsList) {

	Applicant tempApplicant = applicant;
	tempApplicant.priorities().clear();

	for (const auto& priority : std::as_const(applicant.priorities())) {

	    if (priority.studyType() == StudyType::Budget)
		tempApplicant.addPriority(priority);
	}

	generalList.append(tempApplicant);
    }

    for (int i = -1; generalList.size() > 0;) {

	if (generalList.size() == 0)
	    break;

	i = (i + 1) % generalList.size();

	Applicant* applicant = &generalList[i];
	PriorityInfo tempPriority;

	if (applicant->priorities().size() > 0)
	    tempPriority = applicant->priorities().first();
	else {
	    m_uncountedApplicants.append(*applicant);
	    generalList.removeOne(*applicant);
	    continue;
	}

	bool deletePriority = true;
	for (int j = 0; j < m_facultyCells->size(); ++j) {

	    FacultyCell* facultyCell = &(*m_facultyCells)[j];

	    if (facultyCell->isAbleToAdd(tempPriority)) {

		deletePriority = false;

		applicant->priorities().removeFirst();
		facultyCell->addToPool(tempPriority, *applicant);
		generalList.removeOne(*applicant);

		QPair<PriorityInfo, Applicant*> unsuitable = facultyCell->getUnsuitableApplicant();

		if (unsuitable.second != nullptr) {

		    unsuitable.second->addPriority(unsuitable.first);
		    generalList.append(*unsuitable.second);

		    // delete unsuitable.second;
		    // unsuitable.second = nullptr;
		}
	    }
	}

	if (deletePriority)
	    applicant->priorities().removeFirst();
    }

    qDebug() << "General simulation completed";
}

void MagicHat::rebalanceBudgetaryPlaces() {

    for (auto& elem : (*m_facultyCells)) {

	if (elem.studyType() != StudyType::Budget)
	    continue;

	// Skip Physics faculty for now
	if (elem.code().mid(0, 2) == "03" or elem.code().mid(0, 2) == "11")
	    continue;

	for (auto& elem1 : (*m_facultyCells)) {

	    if (elem.name() != elem1.name())
		continue;
	    if (elem.studyType() == elem1.studyType())
		continue;

	    if (elem.studyForm() == elem1.studyForm() and elem.code() == elem1.code() and
	        elem1.studyType() != StudyType::Budget) {

		elem.setCapacity(elem.capacity() + (elem1.capacity() - elem1.getPoolSize()));
	    }
	}
    }

    // Fix Physics rebalancing
    for (auto& elem : (*m_facultyCells)) {

	if (elem.studyType() != StudyType::Budget)
	    continue;
	if (elem.code().mid(0, 2) != "03" and elem.code().mid(0, 2) != "11")
	    continue;

	for (auto& elem1 : (*m_facultyCells)) {

	    if (elem.studyType() == elem1.studyType())
		continue;
	    if (elem1.code().mid(0, 2) != elem.code().mid(0, 2))
		continue;

	    if (elem.studyForm() == elem1.studyForm())
		elem.setCapacity(elem.capacity() + (elem1.capacity() - elem1.getPoolSize()));
	}
    }
}

void MagicHat::printStatsToConsole() {

    int globalCounter = 0;

    QString type;
    QString form;

    for (int i = 0; i < m_facultyCells->size(); ++i) {

	switch ((*m_facultyCells)[i].studyType()) {
	    case StudyType::Budget:
		type = "Бюджет";
		break;
	    case StudyType::NonBudget:
		type = "Внебюджет";
		break;
	    case StudyType::Kvot:
		type = "Отдельная квота";
		break;
	    case StudyType::CompanySponsor:
		type = "Целевое";
		break;
	    case StudyType::SpecialRight:
		type = "Особое право";
		break;
	}

	switch ((*m_facultyCells)[i].studyForm()) {
	    case StudyForm::Personal:
		form = "Очная";
		break;
	    case StudyForm::NotPersonal:
		form = "Заочная";
		break;
	    case StudyForm::PersonalNotPersonal:
		form = "Очно-заочная";
		break;
	}

	qDebug() << "\n>----" << (*m_facultyCells)[i].division() << "|"
	         << (*m_facultyCells)[i].code() << "|" << (*m_facultyCells)[i].name() << "|"
	         << QString::number((*m_facultyCells)[i].pool().size()) + "/" +
	                QString::number((*m_facultyCells)[i].capacity())
	         << "|" << type << "|" << form << "----<";

	if ((*m_facultyCells)[i].pool().size() == 0) {
	    qDebug() << "НЕТ АБИТУРИЕНТОВ\n";
	    continue;
	}

	globalCounter += (*m_facultyCells)[i].pool().size();
	int counter = (*m_facultyCells)[i].pool().size();

	for (const auto& elem : (*m_facultyCells)[i].pool()) {

	    qDebug() << "" << counter << "-" << elem.second.id() << "-" << elem.second.FIO() << "-"
	             << elem.first.egeScore() << elem.first.egeAdditionalScore()
	             << (elem.first.isBVI() ? "БВИ" : "") << "Проритет -"
	             << elem.first.priorityNumber();
	    counter -= 1;
	}

	type.clear();
    }

    qDebug() << "Зачислено -" << globalCounter << "\n";
}

QList<Applicant> MagicHat::uncountedApplicants() const { return m_uncountedApplicants; }

void MagicHat::printUncountedApplicants() {

    for (auto& elem : m_uncountedApplicants) {

	for (const auto elem1 : m_applicantsListCopy) {

	    if (elem.id() == elem1.id())
		elem = elem1;
	}
    }

    std::sort(m_uncountedApplicants.begin(), m_uncountedApplicants.end(),
              [](Applicant& first, Applicant& second) {
	          std::sort(first.priorities().begin(), first.priorities().end(),
	                    [](const PriorityInfo& first1, const PriorityInfo& second2) {
	                        return first1.egeScore() < second2.egeScore();
	                    });
	          std::sort(second.priorities().begin(), second.priorities().end(),
	                    [](const PriorityInfo& first1, const PriorityInfo& second2) {
	                        return first1.egeScore() < second2.egeScore();
	                    });

	          if (first.priorities().size() == 0)
	              return false;

	          if (second.priorities().size() == 0)
	              return true;

	          return first.priorities()[0] > second.priorities()[0];
              });

    for (auto& elem : m_uncountedApplicants) {

	QString temp;
	for (const auto& scores : elem.priorities())
	    temp += QString::number(scores.egeScore()) + " ";

	qDebug() << elem.id() << elem.FIO() << "{" << temp << "}" << elem.priorities()[0].name();
    }

    qDebug() << m_uncountedApplicants.size();
}

void MagicHat::printToExcel() {

    QXlsx::Document newDoc;
    newDoc.addSheet("ПМИ");
    newDoc.addSheet("МКН");
    newDoc.addSheet("ИБ");
    newDoc.addSheet("КБ");

    int globalCounter = 0;

    newDoc.selectSheet("ПМИ");
    for (int i = 0; i < m_facultyCells->size(); ++i) {

	if ((*m_facultyCells)[i].name() ==
	        "Прикладное программирование и информационные технологии" and
	    (*m_facultyCells)[i].studyType() == StudyType::Budget) {

	    globalCounter += (*m_facultyCells)[i].pool().size();
	    int counter = (*m_facultyCells)[i].pool().size();
	    for (int j = 0; j < (*m_facultyCells)[i].pool().size(); ++j) {

		newDoc.write(j + 1, 1, (*m_facultyCells)[i].pool()[j].second.id());
		newDoc.write(j + 1, 2, (*m_facultyCells)[i].pool()[j].second.FIO());
		newDoc.write(j + 1, 3, (*m_facultyCells)[i].pool()[j].first.egeScore());
		newDoc.write(j + 1, 4, (*m_facultyCells)[i].pool()[j].first.egeAdditionalScore());
		newDoc.write(j + 1, 5, (*m_facultyCells)[i].pool()[j].first.name());
	    }
	}
    }

    newDoc.selectSheet("МКН");
    for (int i = 0; i < m_facultyCells->size(); ++i) {

	if ((*m_facultyCells)[i].name() == "Программирование, алгоритмы и анализ данных" and
	    (*m_facultyCells)[i].studyType() == StudyType::Budget) {

	    globalCounter += (*m_facultyCells)[i].pool().size();
	    int counter = (*m_facultyCells)[i].pool().size();
	    for (int j = 0; j < (*m_facultyCells)[i].pool().size(); ++j) {

		newDoc.write(j + 1, 1, (*m_facultyCells)[i].pool()[j].second.id());
		newDoc.write(j + 1, 2, (*m_facultyCells)[i].pool()[j].second.FIO());
		newDoc.write(j + 1, 3, (*m_facultyCells)[i].pool()[j].first.egeScore());
		newDoc.write(j + 1, 4, (*m_facultyCells)[i].pool()[j].first.egeAdditionalScore());
		newDoc.write(j + 1, 5, (*m_facultyCells)[i].pool()[j].first.name());
	    }
	}
    }

    newDoc.selectSheet("ИБ");
    for (int i = 0; i < m_facultyCells->size(); ++i) {

	if ((*m_facultyCells)[i].name() == "Безопасность компьютерных систем" and
	    (*m_facultyCells)[i].studyType() == StudyType::Budget) {

	    globalCounter += (*m_facultyCells)[i].pool().size();
	    int counter = (*m_facultyCells)[i].pool().size();
	    for (int j = 0; j < (*m_facultyCells)[i].pool().size(); ++j) {

		newDoc.write(j + 1, 1, (*m_facultyCells)[i].pool()[j].second.id());
		newDoc.write(j + 1, 2, (*m_facultyCells)[i].pool()[j].second.FIO());
		newDoc.write(j + 1, 3, (*m_facultyCells)[i].pool()[j].first.egeScore());
		newDoc.write(j + 1, 4, (*m_facultyCells)[i].pool()[j].first.egeAdditionalScore());
		newDoc.write(j + 1, 5, (*m_facultyCells)[i].pool()[j].first.name());
	    }
	}
    }

    newDoc.selectSheet("КБ");
    for (int i = 0; i < m_facultyCells->size(); ++i) {

	if ((*m_facultyCells)[i].name() == "Математические методы защиты информации" and
	    (*m_facultyCells)[i].studyType() == StudyType::Budget) {

	    globalCounter += (*m_facultyCells)[i].pool().size();
	    int counter = (*m_facultyCells)[i].pool().size();
	    for (int j = 0; j < (*m_facultyCells)[i].pool().size(); ++j) {

		newDoc.write(j + 1, 1, (*m_facultyCells)[i].pool()[j].second.id());
		newDoc.write(j + 1, 2, (*m_facultyCells)[i].pool()[j].second.FIO());
		newDoc.write(j + 1, 3, (*m_facultyCells)[i].pool()[j].first.egeScore());
		newDoc.write(j + 1, 4, (*m_facultyCells)[i].pool()[j].first.egeAdditionalScore());
		newDoc.write(j + 1, 5, (*m_facultyCells)[i].pool()[j].first.name());
	    }
	}
    }

    if (newDoc.saveAs("newDoc1.xlsx")) {
	qDebug() << "OK";
    } else {
	qDebug() << "NOT OK";
    }
}

void MagicHat::setKCP(const QString& path, const QString& sheet) {

    QFile input(path);

    if (!input.exists()) {
	qDebug() << "Не удалось найти файл " << path << "\n\t Ошибка: " << input.errorString();
    }

    QXlsx::Document doc(path);
    doc.selectSheet(sheet);

    // skipping title
    for (int i = 2; doc.read(i, 1).isValid(); ++i) {

	QString code = doc.read(i, 1).toString();
	if (code.contains("/"))
	    continue;

	QString division = doc.read(i, 2).toString();
	QString name = doc.read(i, 3).toString();
	QString studyForm = doc.read(i, 4).toString();
	QString type = doc.read(i, 5).toString();
	QString kcp = doc.read(i, 6).toString();

	m_facultyCells->append(FacultyCell());
	m_facultyCells->last().setName(name);
	m_facultyCells->last().setDivision(division);
	m_facultyCells->last().setCode(code);
	m_facultyCells->last().setCapacity(kcp.toInt());

	if (studyForm.toLower() == "очная" or studyForm.toLower() == "очное")
	    m_facultyCells->last().setStudyForm(StudyForm::Personal);

	if (studyForm.toLower() == "заочная" or studyForm.toLower() == "заочное")
	    m_facultyCells->last().setStudyForm(StudyForm::NotPersonal);

	if (studyForm.toLower() == "очно-заочная" or studyForm.toLower() == "очно-заочное")
	    m_facultyCells->last().setStudyForm(StudyForm::PersonalNotPersonal);

	if (type.toLower() == "бюджет")
	    m_facultyCells->last().setStudyType(StudyType::Budget);

	if (type.toLower() == "внебюджет")
	    m_facultyCells->last().setStudyType(StudyType::NonBudget);

	if (type.toLower() == "отдельная квота")
	    m_facultyCells->last().setStudyType(StudyType::Kvot);

	if (type.toLower() == "особое право")
	    m_facultyCells->last().setStudyType(StudyType::SpecialRight);

	if (type.toLower() == "целевое" or type.toLower() == "целевая")
	    m_facultyCells->last().setStudyType(StudyType::CompanySponsor);
    }
}

QList<FacultyCell>* MagicHat::faculties() const { return m_facultyCells; }

void MagicHat::printFaculties() {

    qDebug() << "\n";

    QString type;
    QString form;

    QString tempCode = (*m_facultyCells)[0].code();
    for (const auto& elem : (*m_facultyCells)) {

	if (tempCode != elem.code()) {
	    qDebug() << "\n";
	    tempCode = elem.code();
	}

	switch (elem.studyType()) {
	    case StudyType::Budget:
		type = "Бюджет";
		break;
	    case StudyType::NonBudget:
		type = "Внебюджет";
		break;
	    case StudyType::Kvot:
		type = "Отдельная квота";
		break;
	    case StudyType::CompanySponsor:
		type = "Целевое";
		break;
	    case StudyType::SpecialRight:
		type = "Особое право";
		break;
	}

	switch (elem.studyForm()) {
	    case StudyForm::Personal:
		form = "Очная";
		break;
	    case StudyForm::NotPersonal:
		form = "Заочная";
		break;
	    case StudyForm::PersonalNotPersonal:
		form = "Очно-заочная";
		break;
	}

	qDebug() << elem.code() + " " + elem.name() + "-" + form + "-" + type + "-" +
	                QString::number(elem.capacity()) + "-" +
	                QString::number(elem.pool().size()) + "-" +
	                QString::number(
	                    (elem.pool().size() != 0 ? elem.pool()[0].first.egeScore() : 0));
    }
}
