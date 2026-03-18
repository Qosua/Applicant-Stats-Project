#ifndef SUPPORTSYSTEM_H
#define SUPPORTSYSTEM_H

#include <QDir>
#include <QFile>
#include <QList>
#include <QString>

#include "../applicants-faculthy-data/applicant.h"
#include "../applicants-faculthy-data/faculthy-direction.h"

const QString APP_PATH = "C:/ProgramData/ApplicantStatsProject";
const QString APP_DATA_PATH = "C:/ProgramData/ApplicantStatsProject/data";
const QString APP_CACHE_PATH = "C:/ProgramData/ApplicantStatsProject/cache";

class SupportSystem {

public:
    SupportSystem();

    void toCache(QList<Direction>& faculties, QString tablePath);
    QList<Direction> fromCache(QString tablePath);
    bool isCacheExist(QString tablePath);

    void init();
};

#endif  // SUPPORTSYSTEM_H
