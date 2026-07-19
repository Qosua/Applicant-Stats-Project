#ifndef SUPPORTSYSTEM_H
#define SUPPORTSYSTEM_H

#include <QString>

class SupportSystem {

public:
    SupportSystem();

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

#endif  // SUPPORTSYSTEM_H
