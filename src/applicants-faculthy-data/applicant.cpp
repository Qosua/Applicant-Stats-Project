#include "src/applicants-faculthy-data/applicant.h"

#include "qdebug.h"

Applicant::Applicant() {}

Applicant::Applicant(const Applicant &copy) { *this = copy; }

int Applicant::id() const { return m_id; }

void Applicant::setId(int newId) {
    if (m_id == newId)
	return;
    m_id = newId;
    emit idChanged();
}

QList<PriorityInfo> &Applicant::priorities() { return m_priorities; }

void Applicant::setPriorities(const QList<PriorityInfo> &newPriorities) {
    m_priorities = newPriorities;
    emit prioritiesChanged();
}

void Applicant::addPriority(const PriorityInfo &info) {

    m_priorities.append(info);

    std::sort(m_priorities.begin(), m_priorities.end(),
              [](const PriorityInfo &info1, const PriorityInfo &info2) {
	          // old system

	          /*
	          if(info1.type() == "Целевое" and info2.type() != "Целевое")
	              return !(false);
	          else
	              if(info1.type() == "Целевое" and info2.type() == "Целевое")
	                  return (info1.priorityNumber() < info2.priorityNumber());
	              else
	                  if(info1.type() != "Целевое" and info2.type() == "Целевое")
	                      return !(true);
	                  else
	                      if((info1.type() == "Бюджет" or info1.type() == "Внебюджет") and
	                          info2.type() != "Бюджет" and info2.type() != "Внебюджет")
	                          return !(true);
	                      else
	                          if((info1.type() == "Бюджет" or info1.type() == "Внебюджет") and
	                              (info2.type() == "Бюджет" or info2.type() == "Внебюджет"))
	                              return (info1.priorityNumber() < info2.priorityNumber());
	                          else
	                              if((info1.type() != "Бюджет" and info1.type() != "Внебюджет")
	          and (info2.type() == "Бюджет" or info2.type() == "Внебюджет")) return !(false);
	                              else
	                                  return (info1.priorityNumber() < info2.priorityNumber());

	          qDebug() << "ERROR IN COMPARATOR -"  << __FILE__  << ":" << __LINE__;
	          return false;
	          */

	          if ((info1.studyType() == StudyType::Budget or
	               info1.studyType() == StudyType::NonBudget) and
	              info2.studyType() != StudyType::Budget and
	              info2.studyType() != StudyType::NonBudget)
	              return !(true);
	          else if ((info1.studyType() == StudyType::Budget or
	                    info1.studyType() == StudyType::NonBudget) and
	                   (info2.studyType() == StudyType::Budget or
	                    info2.studyType() == StudyType::NonBudget))
	              return (info1.priorityNumber() < info2.priorityNumber());
	          else if ((info1.studyType() != StudyType::Budget and
	                    info1.studyType() != StudyType::NonBudget) and
	                   (info2.studyType() == StudyType::Budget or
	                    info2.studyType() == StudyType::NonBudget))
	              return !(false);
	          else
	              return (info1.priorityNumber() < info2.priorityNumber());

	          qDebug() << "ERROR IN COMPARATOR -" << __FILE__ << ":" << __LINE__;
	          return false;
              });
}

void Applicant::deletePriority(StudyType flag) {

    for (int i = 0; i < m_priorities.size(); ++i) {

	if (m_priorities[i].studyType() == flag) {
	    m_priorities.removeOne(m_priorities[i]);
	    --i;
	}
    }

    // old system

    /*
    switch(flag){
        case StudyType::Budget: {
            for(int i = 0; i < m_priorities.size(); ++i)
                if(m_priorities[i].studyType() == "Бюджет") {
                    m_priorities.removeOne(m_priorities[i]);
                    --i;
                }
        }break;

        case StudyType::NonBudget: {
            for(int i = 0; i < m_priorities.size(); ++i)
                if(m_priorities[i].studyType() == "Внебюджет") {
                    m_priorities.removeOne(m_priorities[i]);
                    --i;
                }
        }break;

        case StudyType::SpecialRight: {
            for(int i = 0; i < m_priorities.size(); ++i)
                if(m_priorities[i].studyType() == "Особое право") {
                    m_priorities.removeOne(m_priorities[i]);
                    --i;
                }
        }break;

        case StudyType::Kvot: {
            for(int i = 0; i < m_priorities.size(); ++i)
                if(m_priorities[i].studyType() == "Отдельная квота") {
                    m_priorities.removeOne(m_priorities[i]);
                    --i;
                }
        }break;

        case StudyType::CompanySponsor: {
            for(int i = 0; i < m_priorities.size(); ++i)
                if(m_priorities[i].studyType() == "Целевое") {
                    m_priorities.removeOne(m_priorities[i]);
                    --i;
                }
        }break;
    }
    */
}

QString Applicant::phoneNumber() const { return m_phoneNumber; }

void Applicant::setPhoneNumber(const QString &newPhoneNumber) {
    if (m_phoneNumber == newPhoneNumber)
	return;
    m_phoneNumber = newPhoneNumber;
    emit phoneNumberChanged();
}

QString Applicant::email() const { return m_email; }

void Applicant::setEmail(const QString &newEmail) {
    if (m_email == newEmail)
	return;
    m_email = newEmail;
    emit emailChanged();
}

QString Applicant::FIO() const { return m_FIO; }

void Applicant::setFIO(const QString &newFIO) {
    if (m_FIO == newFIO)
	return;
    m_FIO = newFIO;
    emit FIOChanged();
}

void Applicant::operator=(const Applicant &copy) {
    m_id = copy.m_id;
    m_priorities = copy.m_priorities;
    m_FIO = copy.m_FIO;
    m_email = copy.m_email;
    m_phoneNumber = copy.m_phoneNumber;
}

bool Applicant::operator==(const Applicant &copy) const {

    if ((m_id == copy.m_id) and (m_priorities == copy.m_priorities) and (m_FIO == copy.m_FIO) and
        (m_email == copy.m_email) and (m_phoneNumber == copy.m_phoneNumber))
	return true;
    else
	return false;
}

//////////////////////////////////////////////////////////////////////

int PriorityInfo::egeAdditionalScore() const { return m_egeAdditionalScore; }

void PriorityInfo::setEgeAdditionalScore(int newEgeAdditionalScore) {
    if (m_egeAdditionalScore == newEgeAdditionalScore)
	return;
    m_egeAdditionalScore = newEgeAdditionalScore;
    emit egeAdditionalScoreChanged();
}

int PriorityInfo::priorityNumber() const { return m_priorityNumber; }

void PriorityInfo::setPriorityNumber(int newPriorityNumber) {
    if (m_priorityNumber == newPriorityNumber)
	return;
    m_priorityNumber = newPriorityNumber;
    emit priorityNumberChanged();
}

QString PriorityInfo::code() const { return m_code; }

void PriorityInfo::setCode(const QString &newCode) {
    if (m_code == newCode)
	return;
    m_code = newCode;
    emit codeChanged();
}

QString PriorityInfo::name() const { return m_name; }

void PriorityInfo::setName(const QString &newName) {
    if (m_name == newName)
	return;
    m_name = newName;
    emit nameChanged();
}

StudyForm PriorityInfo::studyForm() const { return m_studyForm; }

void PriorityInfo::setStudyForm(const StudyForm &newStudyForm) {
    if (m_studyForm == newStudyForm)
	return;
    m_studyForm = newStudyForm;
    emit studyFormChanged();
}

StudyType PriorityInfo::studyType() const { return m_studyType; }

void PriorityInfo::setStudyType(const StudyType &newType) {
    if (m_studyType == newType)
	return;
    m_studyType = newType;
    emit typeChanged();
}

QList<int> PriorityInfo::subjectScores() const { return m_subjectScores; }

void PriorityInfo::setSubjectScores(const QList<int> &newSubjectScores) {
    if (m_subjectScores == newSubjectScores)
	return;
    m_subjectScores = newSubjectScores;
    emit subjectScoresChanged();
}

void PriorityInfo::addSubject(int score) {
    m_subjectScores.append(score);
    if (m_subjectScores.size() > 3)
	qDebug() << "WARNING SUBJECTS COUNT MOEW THAN 3" << __FILE__ << ":" << __LINE__;
}

int PriorityInfo::id() const { return m_id; }

void PriorityInfo::setId(int newId) {
    if (m_id == newId)
	return;
    m_id = newId;
    emit idChanged();
}

bool PriorityInfo::admissionFlag() const { return m_admissionFlag; }

void PriorityInfo::setAdmissionFlag(bool newAdmissionsFlag) {
    if (m_admissionFlag == newAdmissionsFlag)
	return;
    m_admissionFlag = newAdmissionsFlag;
    emit admissionFlagChanged();
}

bool PriorityInfo::isBVI() const { return m_isBVI; }

void PriorityInfo::setIsBVI(bool newIsBVI) {
    if (m_isBVI == newIsBVI)
	return;
    m_isBVI = newIsBVI;
    emit isBVIChanged();
}

QString PriorityInfo::division() const { return m_division; }

void PriorityInfo::setDivision(const QString &newDivision) {
    if (m_division == newDivision)
	return;
    m_division = newDivision;
    emit divisionChanged();
}

PriorityInfo::PriorityInfo() {}

PriorityInfo::PriorityInfo(const PriorityInfo &copy) { *this = copy; }

void PriorityInfo::operator=(const PriorityInfo &copy) {
    m_egeScore = copy.m_egeScore;
    m_egeAdditionalScore = copy.m_egeAdditionalScore;
    m_priorityNumber = copy.m_priorityNumber;
    m_code = copy.m_code;
    m_name = copy.m_name;
    m_studyForm = copy.m_studyForm;
    m_studyType = copy.m_studyType;
    m_subjectScores = copy.m_subjectScores;
    m_id = copy.m_id;
    m_admissionFlag = copy.m_admissionFlag;
    m_isBVI = copy.m_isBVI;
    m_division = copy.m_division;
}

bool PriorityInfo::operator==(const PriorityInfo &copy) const {
    return ((m_egeScore == copy.m_egeScore) and
            (m_egeAdditionalScore == copy.m_egeAdditionalScore) and
            (m_priorityNumber == copy.m_priorityNumber) and (m_code == copy.m_code) and
            (m_name == copy.m_name) and (m_studyForm == copy.m_studyForm) and
            (m_studyType == copy.m_studyType) and (m_subjectScores == copy.m_subjectScores) and
            (m_id == copy.m_id) and (m_admissionFlag == copy.m_admissionFlag) and
            (m_isBVI == copy.m_isBVI) and (m_division == copy.m_division));
}

bool PriorityInfo::operator!=(const PriorityInfo &copy) const { return !(*this == copy); }

bool PriorityInfo::operator<(const PriorityInfo &copy) const {

    if (m_isBVI == false and copy.m_isBVI == true)
	return true;
    else if (m_isBVI == true and copy.m_isBVI == false)
	return false;

    if (m_egeScore < copy.m_egeScore)
	return true;
    else if (m_egeScore == copy.m_egeScore) {

	if (m_egeAdditionalScore > copy.m_egeAdditionalScore)
	    return true;
	else if (m_egeAdditionalScore == copy.m_egeAdditionalScore) {

	    if (m_subjectScores[0] < copy.m_subjectScores[0])
		return true;
	    else if (m_subjectScores[0] == copy.m_subjectScores[0]) {

		if (m_subjectScores[1] < copy.m_subjectScores[1])
		    return true;
		else if (m_subjectScores[1] == copy.m_subjectScores[1]) {

		    if (m_subjectScores[2] < copy.m_subjectScores[2])
			return true;
		    else if (m_subjectScores[2] == copy.m_subjectScores[2]) {

			// qDebug() << "Why???" << __FILE__ << ":" << __LINE__;
			return m_id >= copy.m_id;
		    } else
			return false;
		} else
		    return false;
	    } else
		return false;
	} else
	    return false;
    } else
	return false;
}

bool PriorityInfo::operator>(const PriorityInfo &copy) const {
    return (!(*this < copy) and (*this != copy));
}

bool PriorityInfo::operator<=(const PriorityInfo &copy) const { return !(*this > copy); }

bool PriorityInfo::operator>=(const PriorityInfo &copy) const { return !(*this < copy); }

int PriorityInfo::egeScore() const { return m_egeScore; }

void PriorityInfo::setEgeScore(int newEgeScore) {
    if (m_egeScore == newEgeScore)
	return;
    m_egeScore = newEgeScore;
    emit egeScoreChanged();
}
