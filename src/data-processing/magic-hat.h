#ifndef MAGICHAT_H
#define MAGICHAT_H

#include <QFile>
#include <memory>

#include "../applicants-faculty-data/applicant.h"
#include "../applicants-faculty-data/faculty-direction.h"
#include "../xlsx.h"

class MagicHat {
public:
    MagicHat();

    QList<Applicant> applicantsList() const;
    QList<Applicant> uncountedApplicants() const;
    std::shared_ptr<QList<FacultyDirection>> faculties() const;

    void setApplicantsList(std::shared_ptr<QList<Applicant>> newApplicantsList);
    void setKCPFromDB(const QString& entryCommisionName, bool isBachelor, int year);

    void startPriorityRoundSimulation();
    void startGeneralRoundSimulation();
    void rebalanceBudgetaryPlaces();

    void printFaculties();
    void printUncountedApplicants();
    void printStatsToConsole();
    void printToExcel();

private:
    std::shared_ptr<QList<Applicant>> m_applicantsListCopy;
    std::shared_ptr<QList<Applicant>> m_applicantsList;
    QList<Applicant> m_uncountedApplicants;
    std::shared_ptr<QList<FacultyDirection>> m_facultyCells;
};
#endif  // MAGICHAT_H
