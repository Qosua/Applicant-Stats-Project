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
    void setColumnsNamesPath(const QString& path);

    void parseTable();
    std::shared_ptr<QList<Applicant>> getApplicants(ApplicantsFilterFlags flag,
                                                    StudyType priorityToDelete) const;

//private:
    bool setColumnsNames();
    void printStatsToConsole() const;

    QString extractCode(const QString& str);
    QString extractName(const QString& str);
    StudyForm extractStudyForm(const QString& str);
    StudyType extractStudyType(const QString& str);

    QString m_tablePath;
    QString m_columnsNamesFilePath;

    QMap<QString, int> m_columnsNames;
    std::unique_ptr<QXlsx::Document> m_applicantsTable;
    std::shared_ptr<QList<Applicant>> m_applicantsList;
};

#endif  // TABLEPARSERBACHELOR_H
