#include "src/facultycell.h"

FacultyCell::FacultyCell() {}

FacultyCell::FacultyCell(const FacultyCell &copy) {

    *this = copy;
}

void FacultyCell::operator=(const FacultyCell &copy) {

    m_capacity = copy.m_capacity;
    m_name = copy.m_name;
    m_code = copy.m_code;
    m_studyForm = copy.m_studyForm;
    m_studyType = copy.m_studyType;
    m_pool = copy.m_pool;
    m_division = copy.m_division;
}

QString FacultyCell::name() const {
    return m_name;
}

void FacultyCell::setName(const QString &newName) {
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

QString FacultyCell::division() const {
    return m_division;  
}

void FacultyCell::setDivision(const QString &newName) {
    m_division = newName;
}

int FacultyCell::capacity() const {
    return m_capacity;
}

void FacultyCell::setCapacity(int newCapacity) {
    if (m_capacity == newCapacity)
        return;
    m_capacity = newCapacity;
    emit capacityChanged();
}

QString FacultyCell::code() const {
    return m_code;
}

void FacultyCell::setCode(const QString &newCode) {
    if (m_code == newCode)
        return;
    m_code = newCode;
    emit codeChanged();
}

StudyForm FacultyCell::studyForm() const {
    return m_studyForm;
}

void FacultyCell::setStudyForm(const StudyForm &newStudyForm) {
    if (m_studyForm == newStudyForm)
        return;
    m_studyForm = newStudyForm;
    emit studyFormChanged();
}

StudyType FacultyCell::studyType() const {
    return m_studyType;
}

void FacultyCell::setStudyType(const StudyType &newType) {
    if (m_studyType == newType)
        return;
    m_studyType = newType;
    emit typeChanged();
}

QList<QPair<PriorityInfo, Applicant>> FacultyCell::pool() const {
    return m_pool;
}

void FacultyCell::setPool(const QList<QPair<PriorityInfo, Applicant>> &newPool) {
    m_pool = newPool;
    emit poolChanged();
}

void FacultyCell::addToPool(const PriorityInfo &priority, const Applicant &applicant) {
    
    m_pool.append({priority, applicant});
    
    std::sort(m_pool.begin(), m_pool.end(), [](const QPair<PriorityInfo, Applicant>& priority1, const QPair<PriorityInfo, Applicant>& priority2){
        
        return (priority1.first <= priority2.first);
        
    });
    
}

bool FacultyCell::isAbleToAdd(PriorityInfo priority) {
    
    if(m_capacity <= 0)
        return false;
    
    if(m_name != priority.name() or
       m_code != priority.code() or
       m_studyForm != priority.studyForm() or
       m_studyType != priority.studyType()) {
        //qDebug() << "PRIORITY IN WRONG GROUP" << __FILE__ << ":" << __LINE__;
        return false;
    }
    
    if(m_pool.size() == 0)
        return true;
    
    // if(m_pool.first().first == priority)
    //     return false;
    
    if(m_pool.size() >= m_capacity and m_pool.first().first > priority)
        return false;
    else
        return true;
    
}

QPair<PriorityInfo, Applicant*> FacultyCell::getUnsuitableApplicant() {
    
    if(m_pool.size() > m_capacity){
        
        Applicant* applicant = new Applicant;
        *applicant = m_pool.first().second;
        
        PriorityInfo temp =  m_pool.first().first;
        m_pool.removeFirst();
        
        return {temp, applicant};
    }
    
    return {PriorityInfo(), nullptr};
}

int FacultyCell::getPoolSize() {

    return m_pool.size();
}





























