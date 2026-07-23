#include "magic-hat.h"
#include "src/utils/database-manager.h"

MagicHat::MagicHat() { m_facultyCells = std::make_shared<QList<FacultyDirection>>(); }

QList<Applicant> MagicHat::applicantsList() const { return *m_applicantsList; }

void MagicHat::setApplicantsList(std::shared_ptr<QList<Applicant>> newApplicantsList) {

    m_applicantsList = newApplicantsList;
    m_applicantsListCopy = std::make_shared<QList<Applicant>>(*newApplicantsList);
}

void MagicHat::startPriorityRoundSimulation() {

    QList<Applicant> applicantsList;

    for (auto& elem : *m_applicantsList) {

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

	    FacultyDirection* facultyCell = &(*m_facultyCells)[j];

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

	    for (int i = 0; i < m_applicantsList->size(); ++i) {

		if (applicant.second.id() == (*m_applicantsList)[i].id()) {
		    m_applicantsList->remove(i);
		    --i;
		}
	    }
	}
    }

    qDebug() << "Priority simulation completed";
}

void MagicHat::startGeneralRoundSimulation() {

    QList<Applicant> generalList;

    for (auto& applicant : *m_applicantsList) {

	Applicant tempApplicant = applicant;
	tempApplicant.priorities().clear();

	for (const auto& priority : std::as_const(applicant.priorities())) {

	    if (priority.studyType() == StudyType::Budget)
		tempApplicant.addPriority(priority);
	}

	generalList.append(tempApplicant);
    }

    for (int i = -1; generalList.size() > 0;) {

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

	    FacultyDirection* facultyCell = &(*m_facultyCells)[j];

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

	    if (elem.studyForm() == elem1.studyForm() and elem.code() == elem1.code()
	        and elem1.studyType() != StudyType::Budget) {

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

QList<Applicant> MagicHat::uncountedApplicants() const { return m_uncountedApplicants; }

void MagicHat::setKCPFromDB(const QString& entryCommisionName, bool isBachelor, int year) const {

    QSqlQuery* result = DataBaseManager::instance().getKCP(entryCommisionName, isBachelor, year);

    if (!result)
        return;

    while (result->next()) {

	QString code = result->value("code").toString();
	if (code.contains("/"))
	    continue;

	QString division = result->value("department").toString();
	QString name = result->value("profile_name").toString();
	StudyForm studyForm = static_cast<StudyForm>(result->value("study_form").toInt());
	StudyType studyType = static_cast<StudyType>(result->value("study_type").toInt());
	int kcp = result->value("kcp_count").toInt();

	m_facultyCells->append(FacultyDirection());
	m_facultyCells->last().setName(name);
	m_facultyCells->last().setDivision(division);
	m_facultyCells->last().setCode(code);
	m_facultyCells->last().setCapacity(kcp);
        m_facultyCells->last().setStudyForm(studyForm);
        m_facultyCells->last().setStudyType(studyType);
    }
}

std::shared_ptr<QList<FacultyDirection>> MagicHat::faculties() const { return m_facultyCells; }
