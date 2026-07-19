#include "support-system.h"

#include <QDebug>
#include <QDir>
#include <QStandardPaths>

const QString SupportSystem::appDbPath
    = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
      + "/Applicants-Stats-Project/database.db";

const QString SupportSystem::appRootDataPath
    = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
      + "/Applicants-Stats-Project";

const QString SupportSystem::appDataPath
    = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
      + "/Applicants-Stats-Project/data";

const QString SupportSystem::appCachePath
    = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
      + "/Applicants-Stats-Project/cache";

const QString SupportSystem::downloadPath
    = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

SupportSystem::SupportSystem() {}

void SupportSystem::init() {

    bool result = true;

    if (!QDir().exists(SupportSystem::appRootDataPath))
        result *= QDir().mkdir(SupportSystem::appRootDataPath);

    if (!QDir().exists(SupportSystem::appCachePath))
        result *= QDir().mkdir(SupportSystem::appCachePath);

    if (!QDir().exists(SupportSystem::appDataPath))
        result *= QDir().mkdir(appDataPath);

    if (!result)
        qDebug() << "SupportSystem INIT ERROR";
}
