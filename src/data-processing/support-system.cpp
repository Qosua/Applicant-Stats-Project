#include "support-system.h"

SupportSystem::SupportSystem() {}

void SupportSystem::init() {
    
    if(!QDir().exists(APP_PATH))
        QDir().mkdir(APP_PATH);
    
    if(!QDir().exists(APP_CACHE_PATH))
        QDir().mkdir(APP_CACHE_PATH);
    
    if(!QDir().exists(APP_DATA_PATH))
        QDir().mkdir(APP_DATA_PATH);
}
