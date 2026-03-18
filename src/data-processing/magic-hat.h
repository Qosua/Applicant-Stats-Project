#ifndef MAGICHAT_H
#define MAGICHAT_H

#include <QFile>

#include "../applicants-faculthy-data/applicant.h"
#include "../applicants-faculthy-data/faculthy-direction.h"
#include "src/xlsx.h"

class MagicHat : public QObject {
    Q_OBJECT
public:
    MagicHat();

    QList<Applicant> applicantsList() const;
    QList<Applicant> uncountedApplicants() const;
    QList<Direction> *faculties() const;

    void setApplicantsList(QList<Applicant> *newApplicantsList);
    void setKCP(const QString &path, const QString &sheet);

    void startPriorityRoundSimulation();
    void startGeneralRoundSimulation();
    void rebalanceBudgetaryPlaces();

    void printFaculties();
    void printUncountedApplicants();
    void printStatsToConsole();
    void printToExcel();

private:
    QList<Applicant> m_applicantsListCopy;
    QList<Applicant> m_applicantsList;
    QList<Applicant> m_uncountedApplicants;
    QList<Direction> *m_facultyCells;
};
#endif  // MAGICHAT_H
