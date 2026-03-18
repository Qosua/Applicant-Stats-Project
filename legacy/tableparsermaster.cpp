#include "../legacy/tableparsermaster.h"

TableParserMaster::TableParserMaster(const QString& tablePath, const QString& columnsNamesFilePath) {
    
    if(!QFile::exists(tablePath)) {
        qDebug() << "CAN NOT FIND TABLE FILE" << tablePath << __FILE__ << ":" << __LINE__;
        return;
    }
    
    if(!QFile::exists(columnsNamesFilePath)) {
        qDebug() << "CAN NOT FIND COLUMNS NAMES FILE" << columnsNamesFilePath << __FILE__ << ":" << __LINE__;
        return;
    }
    
    m_tablePath = tablePath;
    m_columnsNamesFilePath = columnsNamesFilePath;
    
}

TableParserMaster::~TableParserMaster() {
    delete m_doc;
    m_doc = nullptr;
    
    delete m_applicants;
    m_applicants = nullptr;
}

void TableParserMaster::parseTable() {
    
    if(m_doc) {
        delete m_doc;
        m_doc = nullptr;
    }
    if(m_applicants) {
        delete m_applicants;
        m_applicants = nullptr;
    }
    
    m_doc = new QXlsx::Document(m_tablePath);
    m_applicants = new QList<Applicant>;

    qDebug() << m_doc->read(1, 1).toString();
    
    setColumnsNames();
    
    QHash<int, Applicant> tempHash;
    int applicantId = 0;
    
    //The first line is names row so we start from the second
    for(int i = 2; m_doc->read(i, 1).isValid(); ++i){
        
        applicantId = m_doc->read(i, m_columnsNames["Уникальный код"]).toInt();
        
        //Adding new applicant if not exits
        if(!tempHash.contains(applicantId)) {

            Applicant applicant;

            applicant.setId(applicantId);
            applicant.setFIO(m_doc->read(i, m_columnsNames["ФИО"]).toString());
            applicant.setEmail(m_doc->read(i, m_columnsNames["E-mail"]).toString());
            applicant.setPhoneNumber(m_doc->read(i, m_columnsNames["Телефон"]).toString());

            tempHash[applicantId] = applicant;
        }

        PriorityInfo info;
        QString priorityFullName = m_doc->read(i, m_columnsNames["Конкурсная группа"]).toString();

        info.setEgeScore(m_doc->read(i, m_columnsNames["Сумма баллов"]).toInt());
        info.setEgeAdditionalScore(m_doc->read(i, m_columnsNames["Сумма баллов за инд.дост.(конкурсные)"]).toInt());
        info.setPriorityNumber(m_doc->read(i, m_columnsNames["Приоритет"]).toInt());
        info.addSubject(-1);
        info.addSubject(-1);
        info.addSubject(-1);
        
        info.setCode(extractCode(priorityFullName));
        info.setName(extractName(priorityFullName));
        info.setStudyForm(extractStudyForm(priorityFullName));
        info.setType(extractType(priorityFullName));
        
        info.setId(applicantId);
        info.setAdmissionFlag((m_doc->read(i, m_columnsNames["Согласие на зачисление"]).toString() == "Да" ? true : false));
        info.setDivision(m_doc->read(i, m_columnsNames["Имя факультета"]).toString());

        tempHash[applicantId].addPriority(info);

    }
    
    *m_applicants = tempHash.values();
    
    qDebug() << "Парсинг завершён успешно";
    
    //Just for debug
    printStatsToConsole();
    
}

QList<Applicant> TableParserMaster::getApplicants(ApplicantsFilterFlags flag, PriorityType priorityToDelete) {
    
    QList<Applicant> newList;
    
    if(flag == ApplicantsFilterFlags::All) {
        newList = *m_applicants;
        
        for(int i = 0; i < newList.size(); ++i)
            newList[i].deletePriority(priorityToDelete);
        
        return newList;
    }
    if(flag == ApplicantsFilterFlags::AdmissionsTrue) {
        for(auto& elem : *m_applicants) {
            
            Applicant applicant = elem;
            applicant.priorities().clear();
            
            for(const auto& priority : elem.priorities())
                if(priority.admissionFlag())
                    applicant.addPriority(priority);
            
            if(applicant.priorities().size() != 0)
                newList.append(applicant);
        }
        
        for(int i = 0; i < newList.size(); ++i)
            newList[i].deletePriority(priorityToDelete);
        
        return newList;
    }
    if(flag == ApplicantsFilterFlags::AdmissionsFalse) {
        for(auto& elem : *m_applicants) {
            
            Applicant applicant = elem;
            applicant.priorities().clear();
            
            for(const auto& priority : elem.priorities())
                if(!priority.admissionFlag())
                    applicant.addPriority(priority);
            
            if(applicant.priorities().size() != 0)
                newList.append(applicant);
            
        }
        
        for(int i = 0; i < newList.size(); ++i)
            newList[i].deletePriority(priorityToDelete);
        
        return newList;
    }
    
    return QList<Applicant>();
    
}

bool TableParserMaster::setColumnsNames() {
    
    QFile file(m_columnsNamesFilePath);
    
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл " << m_columnsNamesFilePath << "\n\tОшибка: " << file.errorString();
        return false;
    }
    
    QTextStream stream(&file);
    QString line;
    
    while(!stream.atEnd()) {
        
        line = stream.readLine();
        
        if(line.isEmpty() or line[0] == '#')
            continue;
        
        QStringList list = line.split('=');
        
        while(list[0].last(1) == ' ')
            list[0].removeLast();
        
        while(list[0].first(1) == ' ')
            list[0].removeFirst();
        
        while(list[1].last(1) == ' ')
            list[1].removeLast();
        
        while(list[1].first(1) == ' ')
            list[1].removeFirst();
        
        list[1] = list[1].mid(1,list[1].size() - 2);
        
        for(int i = 1; m_doc->read(1, i).isValid(); ++i){
            
            if(m_doc->read(1, i).toString() == list[1]){
                
                m_columnsNames[list[0]] = i;
                break;
            }
        }
    }
    
    return true;
    
}

void TableParserMaster::printStatsToConsole() const {
    
    qDebug() << ">>=====================================================================<<";
        
    int counter = 0;
    for(auto& elem : *m_applicants)
        for(const auto& priority : elem.priorities())
            if(priority.type().contains("Внебюджет")) {
                counter += 1;
                break;
            }
    qDebug() << " applicants count with at least one nonbudget priority -" << counter;
    
    
    counter = 0;
    for(auto& elem : *m_applicants)
        for(const auto& priority : elem.priorities())
            if(priority.type().contains("Бюджет")) {
                counter += 1;
                break;
            }
    qDebug() << " applicants count with at least one budget priority -" << counter;
    
    
    counter = 0;
    for(auto& elem : *m_applicants)
        for(const auto& priority : elem.priorities())
            if(priority.type().contains("Целевое")) {
                counter += 1;
                break;
            }
    qDebug() << " applicants count with at least one goal priority -" << counter;
    
    
    counter = 0;
    for(auto& elem : *m_applicants)
        for(const auto& priority : elem.priorities())
            if(priority.type().contains("Отдельная квота")) {
                counter += 1;
                break;
            }
    qDebug() << " applicants count with at least one kvot priority -" << counter;
    
    
    counter = 0;
    for(auto& elem : *m_applicants)
        for(const auto& priority : elem.priorities())
            if(priority.type().contains("Особое право")) {
                counter += 1;
                break;
            }
    qDebug() << " applicants count with at least one special right -" << counter;
    qDebug() << ">-----------------------------------------------------------------------<";
    
    ////////////////////////////////////////////////////////////////////////////////////////
    counter = 0;
    for(auto& elem : *m_applicants)
        for(const auto& priority : elem.priorities())
            if(priority.type().contains("Внебюджет")) {
                counter += 1;
            }
    qDebug() << " nonbudget priority count -" << counter;
    
    
    counter = 0;
    for(auto& elem : *m_applicants)
        for(const auto& priority : elem.priorities())
            if(priority.type().contains("Бюджет")) {
                counter += 1;
            }
    qDebug() << " bidget priority count -" << counter;
    
    
    counter = 0;
    for(auto& elem : *m_applicants)
        for(const auto& priority : elem.priorities())
            if(priority.type().contains("Целевое")) {
                counter += 1;
            }
    qDebug() << " goal priority count -" << counter;
    
    
    counter = 0;
    for(auto& elem : *m_applicants)
        for(const auto& priority : elem.priorities())
            if(priority.type().contains("Отдельная квота")) {
                counter += 1;
            }
    qDebug() << " kvot priority count -" << counter;
    
    
    counter = 0;
    for(auto& elem : *m_applicants)
        for(const auto& priority : elem.priorities())
            if(priority.type().contains("Особое право")) {
                counter += 1;
            }
    qDebug() << " special right priority count -" << counter;
    qDebug() << ">>=====================================================================<<\n";
    
    
}

QString TableParserMaster::extractCode(const QString& str) {
    return str.mid(0,8);
}

QString TableParserMaster::extractName(const QString& str) {
    
    QString ans;
    
    for(int i = 9; (str.mid(i+2,4) != "Заоч" and str.mid(i+2,4) != "Очно" and str.mid(i+2,4) != "Очна");++i) {
        
        if(str[i - 1] == str[i] and str[i] == " ")
            continue;


        ans += str[i];
        
    }
    
    return ans;
    
}

QString TableParserMaster::extractStudyForm(const QString& str) {
    
    if(str.contains("Очное") or str.contains("Очная")){
        return "Очное";
    }
    if(str.contains("Заочная") or str.contains("Заочное")){
        return "Заочная";
    }
    if(str.contains("Очно-заочная") or str.contains("Очно-заочное")){
        return "Очно-заочная";
    }
    return "ОШИБКА ФОРМЫ ОБУЧЕНИЯ";
}

QString TableParserMaster::extractType(const QString& str) {
    if(str.contains("Бюджет")){
        return "Бюджет";
    }
    if(str.contains("Отдельная квота")){
        return "Отдельная квота";
    }
    if(str.contains("Особое право")){
        return "Особое право";
    }
    if(str.contains("Внебюджет")){
        return "Внебюджет";
    }
    if(str.contains("Целевое")){
        return "Целевое";
    }
    return "ОШИБКА ТИПА";
}



















