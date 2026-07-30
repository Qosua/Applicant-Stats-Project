#include <QDebug>
#include <QDir>
#include <QStandardPaths>

#include "app-paths-manager.h"

const QString AppPathsManager::appDbPath
    = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
      + "/Applicants-Stats-Project/database.db";

const QString AppPathsManager::appRootDataPath
    = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
      + "/Applicants-Stats-Project";

const QString AppPathsManager::appDataPath
    = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
      + "/Applicants-Stats-Project/data";

const QString AppPathsManager::appCachePath
    = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
      + "/Applicants-Stats-Project/cache";

const QString AppPathsManager::downloadPath
    = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

AppPathsManager::AppPathsManager() {}

void AppPathsManager::init() {

    bool result = true;

    if (!QDir().exists(AppPathsManager::appRootDataPath))
        result *= QDir().mkdir(AppPathsManager::appRootDataPath);

    if (!QDir().exists(AppPathsManager::appCachePath))
        result *= QDir().mkdir(AppPathsManager::appCachePath);

    if (!QDir().exists(AppPathsManager::appDataPath))
        result *= QDir().mkdir(appDataPath);

    if (!result)
        qDebug() << "AppPathsManager INIT ERROR";
}
