#ifndef FACULTYCELL_H
#define FACULTYCELL_H

#include <QDebug>
#include <QObject>
#include <QPair>
#include <algorithm>

#include "applicant.h"
#include "namespaces.h"

class Direction : public QObject {

    Q_OBJECT

public:
    Direction();
    Direction(const Direction &copy);
    void operator=(const Direction &copy);

    QString name() const;
    void setName(const QString &newName);

    QString division() const;
    void setDivision(const QString &newName);

    int capacity() const;
    void setCapacity(int newCapacity);

    QString code() const;
    void setCode(const QString &newCode);

    StudyForm studyForm() const;
    void setStudyForm(const StudyForm &newStudyForm);

    StudyType studyType() const;
    void setStudyType(const StudyType &newType);

    QList<QPair<PriorityInfo, Applicant>> pool() const;
    void setPool(const QList<QPair<PriorityInfo, Applicant>> &newPool);
    void addToPool(const PriorityInfo &priority, const Applicant &applicant);

    bool isAbleToAdd(PriorityInfo priority);

    QPair<PriorityInfo, Applicant *> getUnsuitableApplicant();

    int getPoolSize();

signals:
    void divisionChanged();
    void nameChanged();
    void capacityChanged();
    void codeChanged();
    void studyFormChanged();
    void typeChanged();
    void poolChanged();

private:
    int m_capacity;
    QString m_division;
    QString m_name;
    QString m_code;
    StudyForm m_studyForm;  // Personal, Personal-NotPersonal,...
    StudyType m_studyType;  // Budget, CompanySponsor,...
    QList<QPair<PriorityInfo, Applicant>> m_pool;
};

#endif  // FACULTYCELL_H
