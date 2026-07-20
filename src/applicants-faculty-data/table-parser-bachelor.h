#ifndef TABLEPARSERBACHELOR_H
#define TABLEPARSERBACHELOR_H

#include <QDebug>
#include <QFile>
#include <QHash>
#include <QList>
#include <QMap>
#include <QTextStream>
#include <memory>

#include "../xlsx.h"
#include "applicant.h"
#include "namespaces.h"

class TableParserBachelor {

public:
    void setTablePath(const QString& path);

    void parseTable();
    std::shared_ptr<QList<Applicant>> getApplicants(ApplicantsFilterFlags flag,
                                                    StudyType priorityToDelete) const;

    void printStatsToConsole() const;
    void readColumnNamesFromDB();
    void setEntryCommisionInfo(const QString& entryCommisionName, bool isBachelor, int year);

    QString extractCode(const QString& str);
    QString extractName(const QString& str);
    StudyForm extractStudyForm(const QString& str);
    StudyType extractStudyType(const QString& str);

    QString m_tablePath;
    QString m_columnsNamesFilePath;
    QString m_entryCommisionName;
    bool m_isBachelor;
    int m_year;

    QMap<QString, int> m_columnsNames;
    std::unique_ptr<QXlsx::Document> m_applicantsTable;
    std::shared_ptr<QList<Applicant>> m_applicantsList;
};

#endif  // TABLEPARSERBACHELOR_H
