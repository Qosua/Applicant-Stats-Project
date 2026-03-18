#include "../tablemanager.h"

TableManager::TableManager() {}

void TableManager::init() {
    
    watcher = new QFileSystemWatcher;
    
    QObject::connect(watcher, &QFileSystemWatcher::directoryChanged,
                     this, &TableManager::directoryChanged);
    
    QObject::connect(watcher, &QFileSystemWatcher::fileChanged,
                     this, &TableManager::fileChanged);
    
    updateWatcher();
    
}

void TableManager::processTable(const QString& tableName) {
    
    QFileInfo fileInfo(APP_DATA_PATH + "/" + tableName);
    QDir cacheDir(APP_CACHE_PATH);
    
    QStringList cacheEntr = cacheDir.entryList();
    QString stringDateTime = fileInfo.lastModified().toString("yyyy-MM-dd-hh-mm-ss");
    
    for(const QString& entry : std::as_const(cacheEntr)) {
        
        QStringList parts = entry.split('_');
        
        if(parts[0] != "cache" or parts[2] != tableName)
            continue;
        
        if(parts[1] == stringDateTime) {
            readCache(tableName);
            return;
        }
    }

    makeCache(tableName);
    
}

void TableManager::readCache(const QString& tableName) {
    emit waitForFinish();
    
    UniversityData* data = new UniversityData;
    QFileInfo fileInfo(APP_DATA_PATH + "/" + tableName);
    QString stringDateTime = fileInfo.lastModified().toString("yyyy-MM-dd-hh-mm-ss");

    QXlsx::Document xlsx(APP_CACHE_PATH + "/cache_" + stringDateTime + "_" + tableName);
    QStringList sheetNames = xlsx.sheetNames();

    for(const QString& sheetName : sheetNames) {

        xlsx.selectSheet(sheetName);

        for(int i = 1; xlsx.read(i, 1).isValid(); ++i) {

            int id =                xlsx.read(i, 1).toInt();
            QString FIO =           xlsx.read(i, 2).toString();
            QString directionName = xlsx.read(i, 3).toString();
            QString code =          xlsx.read(i, 4).toString();
            int prirityNumber =     xlsx.read(i, 5).toInt();
            QString studyTypeStr =  xlsx.read(i, 6).toString();
            QString studyFormStr =  xlsx.read(i, 7).toString();
            bool isBVI =            (xlsx.read(i, 8).toString() == "БВИ" ? true : false);
            int egeSum =            xlsx.read(i, 9).toInt();
            int subj1 =             xlsx.read(i, 10).toInt();
            int subj2 =             xlsx.read(i, 11).toInt();
            int subj3 =             xlsx.read(i, 12).toInt();

            StudyForm form = StudyForm::Error;
            StudyType type = StudyType::Error;

            if(studyFormStr == "Очное") form = StudyForm::Personal;
            else if(studyFormStr == "Очно-заочное") form = StudyForm::PersonalNotPersonal;
            else if(studyFormStr == "Заочное") form = StudyForm::NotPersonal;

            if(studyTypeStr == "Общий конкурс") type = StudyType::Budget;
            else if(studyTypeStr == "Отдельная квота") type = StudyType::Kvot;
            else if(studyTypeStr == "Целевое") type = StudyType::CompanySponsor;
            else if(studyTypeStr == "Особое право") type = StudyType::SpecialRight;





        }
    }
    
    
    emit finished(data);
}

void TableManager::makeCache(const QString& tableName) {
    emit waitForFinish();
    
    TableParserBachelor parserBachelor;
    MagicHat magicHatBachelor;
    
    parserBachelor.setTablePath(APP_DATA_PATH + "/" + tableName);
    parserBachelor.setColumnsNamesPath(":/settings/columnsNames.xlsx");
    parserBachelor.parseTable();
    
    QList<Applicant>* applicantsList = parserBachelor.getApplicants(ApplicantsFilterFlags::AdmissionsTrue,
                                                                    StudyType::NonBudget);
    
    magicHatBachelor.setKCP(":/settings/KCP.xlsx", "Бакалавры");
    magicHatBachelor.setApplicantsList(applicantsList);
    
    magicHatBachelor.startPriorityRoundSimulation();
    magicHatBachelor.rebalanceBudgetaryPlaces();
    magicHatBachelor.startGeneralRoundSimulation();
    
    //magicHatBachelor.printStatsToConsole();
    //magicHatBachelor.printUncountedApplicants();
    
    QList<FacultyCell>* faculties = magicHatBachelor.faculties();
    //QList<Applicant>   uncountedApplicants = magicHatBachelor.uncountedApplicants();

    UniversityData* data = makeDataForCaching(faculties);
    makeCacheTable(data, tableName);
    
    emit finished(data);
}

UniversityData* TableManager::makeDataForCaching(QList<FacultyCell> *directions) {

    UniversityData* data = new UniversityData;

    for(const auto& direction : std::as_const(*directions)) {

        auto& currentElem = (*data)[direction.division()]
                                   [direction.name()]
                                   [direction.studyForm()];

        if(direction.studyType() != StudyType::Budget) {

            currentElem.size += direction.pool().size();
            currentElem.studentsCount += direction.pool().size();
        }
        else {

            currentElem.size += direction.pool().size();
            currentElem.studentsCount += direction.capacity();
        }

        currentElem.pool += direction.pool();

        if(direction.pool().isEmpty() or direction.studyType() != StudyType::Budget)
            continue;

        currentElem.maxScore = std::max(direction.pool().last().first.egeScore(), currentElem.maxScore);
        currentElem.minScore = std::min(direction.pool().first().first.egeScore(), currentElem.minScore);
    }

    return data;
}

void TableManager::makeCacheTable(UniversityData *data, const QString& tableName) {

    QFileInfo fileInfo(APP_DATA_PATH + "/" + tableName);
    QString stringDateTime = fileInfo.lastModified().toString("yyyy-MM-dd-hh-mm-ss");

    QString savePath = APP_CACHE_PATH + "/cache_" + stringDateTime + "_" + tableName;
    QFile someFile(savePath);
    someFile.open(QIODevice::WriteOnly);

    QDataStream stream(&someFile);

    //stream << *data;


    someFile.close();
}

void TableManager::fileChanged(const QString &path) {
    
    if(!QFile::exists(path))
        return;
    
    //makeCache(path.split('/').last());
    
}

void TableManager::directoryChanged(const QString &path) {
    
    updateWatcher();
    
}

void TableManager::updateWatcher() {
    
    QDir dataDir(APP_DATA_PATH);
    QStringList currentFiles = dataDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    QStringList watchedFiles = watcher->files();
    
    for (const QString& path : std::as_const(watchedFiles)) {
        
        if (!QFile::exists(path)) 
            watcher->removePath(path);
    }
    for (const QString& file : std::as_const(currentFiles)) {
        
        QString fullPath = dataDir.absoluteFilePath(file);
        if (!watchedFiles.contains(fullPath) and !fullPath.contains('~') and !fullPath.endsWith(".tmp")) 
            watcher->addPath(fullPath);
    }
    
    watcher->addPath(APP_DATA_PATH + "/");
    
    emit sendTableList(currentFiles);
    
}
