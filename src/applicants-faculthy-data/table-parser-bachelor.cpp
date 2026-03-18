#include "src/tableparserbachelor.h"

TableParserBachelor::TableParserBachelor() {}

TableParserBachelor::~TableParserBachelor() {
    
    if(m_applicantsTable) {
        delete m_applicantsTable;
        m_applicantsTable = nullptr;
    }
    if(m_applicantsList) {
        delete m_applicantsList;
        m_applicantsList = nullptr;
    }
}

void TableParserBachelor::setTablePath(const QString &path) {
    
    if(!QFile::exists(path)) {
        qDebug() << "CAN NOT FIND TABLE FILE" << path << __FILE__ << ":" << __LINE__;
        return;
    }
    m_tablePath = path;
}

void TableParserBachelor::setColumnsNamesPath(const QString &path) {
    
    if(!QFile::exists(path)) {
        qDebug() << "CAN NOT FIND COLUMNS NAMES FILE" << path << __FILE__ << ":" << __LINE__;
        return;
    }
    m_columnsNamesFilePath = path;
}

void TableParserBachelor::parseTable() {
    
    if(m_applicantsTable) {
        delete m_applicantsTable;
        m_applicantsTable = nullptr;
    }
    if(m_applicantsList) {
        delete m_applicantsList;
        m_applicantsList = nullptr;
    }
    
    m_applicantsTable = new QXlsx::Document(m_tablePath);
    m_applicantsList = new QList<Applicant>;
    
    setColumnsNames();
    
    QHash<int, Applicant> tempHash;
    int applicantId = 0;
    
    //The first line is column's names so we start from the second
    for(int i = 2; m_applicantsTable->read(i, 1).isValid(); ++i){
        
        applicantId = m_applicantsTable->read(i, m_columnsNames["Уникальный код"]).toInt();
        
        if(!tempHash.contains(applicantId)) {

            Applicant applicant;
            applicant.setId(applicantId);
            applicant.setFIO(         m_applicantsTable->read(i, m_columnsNames["ФИО"]).toString());
            applicant.setEmail(       m_applicantsTable->read(i, m_columnsNames["E-mail"]).toString());
            applicant.setPhoneNumber( m_applicantsTable->read(i, m_columnsNames["Телефон"]).toString());

            tempHash[applicantId] = applicant;
        }

        PriorityInfo info;
        QString priorityFullName = m_applicantsTable->read(i, m_columnsNames["Конкурсная группа"]).toString();

        info.setEgeScore(           m_applicantsTable->read(i, m_columnsNames["Сумма баллов"]).toInt());
        info.setEgeAdditionalScore( m_applicantsTable->read(i, m_columnsNames["Сумма баллов за инд.дост.(конкурсные)"]).toInt());
        info.setPriorityNumber(     m_applicantsTable->read(i, m_columnsNames["Приоритет"]).toInt());
        info.addSubject(            m_applicantsTable->read(i, m_columnsNames["Предмет 1"]).toInt());
        info.addSubject(            m_applicantsTable->read(i, m_columnsNames["Предмет 2"]).toInt());
        info.addSubject(            m_applicantsTable->read(i, m_columnsNames["Предмет 3"]).toInt());
        
        info.setCode(      extractCode(priorityFullName));
        info.setName(      extractName(priorityFullName));
        info.setStudyForm( extractStudyForm(priorityFullName));
        info.setStudyType( extractType(priorityFullName));
        
        info.setId(applicantId);
        info.setAdmissionFlag( (m_applicantsTable->read(i, m_columnsNames["Согласие на зачисление"]).toString().toLower() == "да" ? true : false));
        info.setIsBVI(         (m_applicantsTable->read(i, m_columnsNames["Без вступительных испытаний"]).toString().toLower() == "да" ? true : false));
        info.setDivision(       m_applicantsTable->read(i, m_columnsNames["Имя факультета"]).toString());

        tempHash[applicantId].addPriority(info);
    }
    
    *m_applicantsList = tempHash.values();

    printStatsToConsole(); 
    
}

QList<Applicant>* TableParserBachelor::getApplicants(ApplicantsFilterFlags flag, StudyType priorityToDelete) {
    
    QList<Applicant>* newList = new QList<Applicant>;
    
    if(flag == ApplicantsFilterFlags::All) {
        *newList = *m_applicantsList;

        for(int i = 0; i < newList->size(); ++i)
            (*newList)[i].deletePriority(priorityToDelete);

        return newList;
    }
    if(flag == ApplicantsFilterFlags::AdmissionsTrue) {
        for(auto& elem : *m_applicantsList) {
            
            Applicant applicant = elem;
            applicant.priorities().clear();
            
            for(const auto& priority : elem.priorities())
                if(priority.admissionFlag())
                    applicant.addPriority(priority);

            if(applicant.priorities().size() != 0)
                newList->append(applicant);
        }

        for(int i = 0; i < newList->size(); ++i)
            (*newList)[i].deletePriority(priorityToDelete);

        return newList;
    }
    if(flag == ApplicantsFilterFlags::AdmissionsFalse) {
        for(auto& elem : *m_applicantsList) {
            
            Applicant applicant = elem;
            applicant.priorities().clear();
            
            for(const auto& priority : elem.priorities())
                if(!priority.admissionFlag())
                    applicant.addPriority(priority);
            
            if(applicant.priorities().size() != 0)
                newList->append(applicant);
            
        }

        for(int i = 0; i < newList->size(); ++i)
            (*newList)[i].deletePriority(priorityToDelete);

        return newList;
    }
    
    return nullptr;
}

bool TableParserBachelor::setColumnsNames() {
    
    QFile file(m_columnsNamesFilePath);
    
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл " << m_columnsNamesFilePath << "\n\tОшибка: " << file.errorString();
        return false;
    }
    
    QXlsx::Document columnsNamesTable(m_columnsNamesFilePath);

    for(int j = 2; columnsNamesTable.read(j, 2).isValid(); ++j) {
        
        for(int i = 1; m_applicantsTable->read(1, i).isValid(); ++i){
            
            QString columnNameInProgram         = columnsNamesTable.read(j, 1).toString();
            QString columnNameInTable           = columnsNamesTable.read(j, 2).toString();
            QString columnNameInApplicantsTable = m_applicantsTable->read(1, i).toString();
            
            if(columnNameInApplicantsTable == columnNameInTable) {
                
                m_columnsNames[columnNameInProgram] = i;
                break;
            }
        }
    }
    
    return true;
}

void TableParserBachelor::printStatsToConsole() const {
    
    qDebug() << ">>=====================================================================<<";
        
    int counter = 0;
    for(auto& elem : *m_applicantsList)
        for(const auto& priority : elem.priorities())
            if(priority.studyType() == StudyType::NonBudget) {
                counter += 1;
                break;
            }
    qDebug() << " applicants count with at least one nonbudget priority -" << counter;
    
    
    counter = 0;
    for(auto& elem : *m_applicantsList)
        for(const auto& priority : elem.priorities())
            if(priority.studyType() == StudyType::Budget) {
                counter += 1;
                break;
            }
    qDebug() << " applicants count with at least one budget priority -" << counter;
    
    
    counter = 0;
    for(auto& elem : *m_applicantsList)
        for(const auto& priority : elem.priorities())
            if(priority.studyType() == StudyType::CompanySponsor) {
                counter += 1;
                break;
            }
    qDebug() << " applicants count with at least one goal priority -" << counter;
    
    
    counter = 0;
    for(auto& elem : *m_applicantsList)
        for(const auto& priority : elem.priorities())
            if(priority.studyType() == StudyType::Kvot) {
                counter += 1;
                break;
            }
    qDebug() << " applicants count with at least one kvot priority -" << counter;
    
    
    counter = 0;
    for(auto& elem : *m_applicantsList)
        for(const auto& priority : elem.priorities())
            if(priority.studyType() == StudyType::SpecialRight) {
                counter += 1;
                break;
            }
    qDebug() << " applicants count with at least one special right -" << counter;
    qDebug() << ">-----------------------------------------------------------------------<";
    
    ////////////////////////////////////////////////////////////////////////////////////////
    counter = 0;
    for(auto& elem : *m_applicantsList)
        for(const auto& priority : elem.priorities())
            if(priority.studyType() == StudyType::NonBudget) {
                counter += 1;
            }
    qDebug() << " nonbudget priority count -" << counter;
    
    
    counter = 0;
    for(auto& elem : *m_applicantsList)
        for(const auto& priority : elem.priorities())
            if(priority.studyType() == StudyType::Budget) {
                counter += 1;
            }
    qDebug() << " bidget priority count -" << counter;
    
    
    counter = 0;
    for(auto& elem : *m_applicantsList)
        for(const auto& priority : elem.priorities())
            if(priority.studyType() == StudyType::CompanySponsor) {
                counter += 1;
            }
    qDebug() << " goal priority count -" << counter;
    
    
    counter = 0;
    for(auto& elem : *m_applicantsList)
        for(const auto& priority : elem.priorities())
            if(priority.studyType() == StudyType::Kvot) {
                counter += 1;
            }
    qDebug() << " kvot priority count -" << counter;
    
    
    counter = 0;
    for(auto& elem : *m_applicantsList)
        for(const auto& priority : elem.priorities())
            if(priority.studyType() == StudyType::SpecialRight) {
                counter += 1;
            }
    qDebug() << " special right priority count -" << counter;
    qDebug() << ">>=====================================================================<<\n";
    
    
}

QString TableParserBachelor::extractCode(const QString& str) {
    return str.mid(0,8);
}

QString TableParserBachelor::extractName(const QString& str) {
    
    QString name;
    
    for(int i = 9; (str.mid(i+2,4).toLower() != "заоч" and
                    str.mid(i+2,4).toLower() != "очно" and
                    str.mid(i+2,4).toLower() != "очна");++i) {
        
        if(str[i - 1] == str[i] and str[i] == " ")
            continue;

        name += str[i];
        
    }
    
    return name;
    
}

StudyForm TableParserBachelor::extractStudyForm(const QString& str) {
    
    if(str.contains("Очное") or str.contains("Очная")){
        return StudyForm::Personal;
    }
    if(str.contains("Заочная") or str.contains("Заочное")){
        return StudyForm::NotPersonal;
    }
    if(str.contains("Очно-заочная") or str.contains("Очно-заочное")){
        return StudyForm::PersonalNotPersonal;
    }
    return StudyForm::Error;
}

StudyType TableParserBachelor::extractType(const QString& str) {
    
    if(str.contains("Бюджет")) {
        return StudyType::Budget;
    }
    if(str.contains("Отдельная квота") or str.contains("Отдельное квота")) {
        return StudyType::Kvot;
    }
    if(str.contains("Особое право") or str.contains("Особая право")) {
        return StudyType::SpecialRight;
    }
    if(str.contains("Внебюджет")) {
        return StudyType::NonBudget;
    }
    if(str.contains("Целевое") or str.contains("Целевая")) {
        return StudyType::CompanySponsor;
    }
    return StudyType::Error;
}



















