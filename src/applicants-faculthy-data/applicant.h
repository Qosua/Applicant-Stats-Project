#ifndef APPLICANT_H
#define APPLICANT_H

#include <QList>
#include <QObject>
#include <QString>
#include <algorithm>

#include "namespaces.h"

class PriorityInfo : public QObject {
    
    Q_OBJECT
    
public:
    PriorityInfo();
    PriorityInfo(const PriorityInfo& copy);
    void operator=(const PriorityInfo& copy);
    
    bool operator==(const PriorityInfo& copy) const;
    bool operator!=(const PriorityInfo& copy) const;
    bool operator <(const PriorityInfo& copy) const;
    bool operator >(const PriorityInfo& copy) const;
    bool operator<=(const PriorityInfo& copy) const;
    bool operator>=(const PriorityInfo& copy) const;
    
    int egeScore() const;
    void setEgeScore(int newEgeScore);
    
    int egeAdditionalScore() const;
    void setEgeAdditionalScore(int newEgeAdditionalScore);
    
    int priorityNumber() const;
    void setPriorityNumber(int newPriorityNumber);
    
    QString code() const;
    void setCode(const QString &newCode);
    
    QString name() const;
    void setName(const QString &newName);
    
    QList<int> subjectScores() const;
    void setSubjectScores(const QList<int> &newSubjectScores);
    void addSubject(int score);
    
    int id() const;
    void setId(int newId);
    
    bool admissionFlag() const;
    void setAdmissionFlag(bool newAdmissionsFlag);
    
    bool isBVI() const;
    void setIsBVI(bool newIsBVI);
    
    QString division() const;
    void setDivision(const QString &newDivision);
    
    StudyForm studyForm() const;
    void setStudyForm(const StudyForm &newStudyForm);
    
    StudyType studyType() const;
    void setStudyType(const StudyType &newType);

private:
    int m_id;   
    int m_egeScore;
    int m_egeAdditionalScore;
    int m_priorityNumber;
    QString m_code;
    QString m_name;
    QString m_division;
    QList<int> m_subjectScores;
    bool m_admissionFlag;
    bool m_isBVI;
    StudyForm m_studyForm; //Personal, Personal-NotPersonal,...
    StudyType m_studyType; //Budget, NonBudget,...
    
signals:
    void egeScoreChanged();
    void egeAdditionalScoreChanged();
    void priorityNumberChanged();
    void priorityNameChanged();
    void codeChanged();
    void nameChanged();
    void studyFormChanged();
    void typeChanged();
    void subjectScoresChanged();
    void idChanged();
    void admissionFlagChanged();
    void isBVIChanged();
    void divisionChanged();
    
};

//////////////////////////////////////////////////////////////////////

class Applicant : public QObject {
    
    Q_OBJECT
    
public:
    Applicant();
    Applicant(const Applicant& copy);
    void operator=(const Applicant& copy);
    
    bool operator==(const Applicant& copy) const;
    
    int id() const;
    void setId(int newId);
    
    QList<PriorityInfo>& priorities();
    void setPriorities(const QList<PriorityInfo> &newPriorities);
    void addPriority(const PriorityInfo& info);
    void deletePriority(StudyType flag);

    QString phoneNumber() const;
    void setPhoneNumber(const QString &newPhoneNumber);

    QString email() const;
    void setEmail(const QString &newEmail);

    QString FIO() const;
    void setFIO(const QString &newFIO);

private:
    int m_id;
    QList<PriorityInfo> m_priorities;
    QString m_phoneNumber;
    QString m_email;
    QString m_FIO;
    
signals:
    void idChanged();
    void prioritiesChanged();
    void phoneNumberChanged();
    void emailChanged();
    void FIOChanged();
    
};
#endif // APPLICANT_H
