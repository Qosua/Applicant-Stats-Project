#ifndef SUPPORTSYSTEM_H
#define SUPPORTSYSTEM_H

#include <QDir>
#include <QStandardPaths>
#include <QString>

class SupportSystem {

public:
    SupportSystem();

    static void init() {

	bool result = true;

	if (!QDir().exists(SupportSystem::appRootDataPath))
	    result *= QDir().mkdir(SupportSystem::appRootDataPath);

	if (!QDir().exists(SupportSystem::appCachePath))
	    result *= QDir().mkdir(SupportSystem::appCachePath);

	if (!QDir().exists(SupportSystem::appDataPath))
	    result *= QDir().mkdir(appDataPath);

	if (!QDir().exists(SupportSystem::appConfigPath))
	    result *= QDir().mkdir(appConfigPath);

	if (!result)
	    qDebug() << "SupportSystem INIT ERROR";
    }

signals:
    void initCacheManager();

public:
    inline const static QString appDbPath
        = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
          + "/Applicants-Stats-Project/database.db";

    inline const static QString appRootDataPath
        = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
          + "/Applicants-Stats-Project";

    inline const static QString appDataPath
        = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
          + "/Applicants-Stats-Project/data";

    inline const static QString appConfigPath
        = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
          + "/Applicants-Stats-Project/config";

    inline const static QString appCachePath
        = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
          + "/Applicants-Stats-Project/cache";

    inline const static QString downloadPath
        = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
};

#endif  // SUPPORTSYSTEM_H
