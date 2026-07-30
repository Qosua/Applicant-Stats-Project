#ifndef APPLICANTSTATSPROJECT_APPPATHSMANAGER_H
#define APPLICANTSTATSPROJECT_APPPATHSMANAGER_H

#include <QString>

class AppPathsManager {

public:
    AppPathsManager();

    static void init();

signals:
    void initCacheManager();

public:
    static const QString appDbPath;
    static const QString appRootDataPath;
    static const QString appDataPath;
    static const QString appCachePath;
    static const QString downloadPath;
};

#endif  // APPLICANTSTATSPROJECT_APPPATHSMANAGER_H
