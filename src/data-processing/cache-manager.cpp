#include "cache-manager.h"

void CacheManager::init() {

    QObject::connect(this, &CacheManager::processTable, this, &CacheManager::processTableHandle);
}

void CacheManager::processTableHandle(const QString& tableName, const QVariantList& infoList) {

    qDebug() << "processTable called with:" << tableName << ":" << infoList;

    QDir cacheDir(SupportSystem::appCachePath);
    QList<QString> cacheEntry = cacheDir.entryList();

    for (const QString& entry : std::as_const(cacheEntry)) {

	QList<QString> parts = entry.split('_');

	if (parts[0] != "cache" or parts[2] != tableNameInCache(tableName, infoList))
	    continue;

	if (parts[1] == tableLastChangeDate(tableName)) {

	    auto data = loadCache(tableName, infoList);
	    emit sendProceededData(data);
	    return;
	}
    }

    auto data = makeCache(tableName, infoList);
    emit sendProceededData(data);
}

std::shared_ptr<QList<FacultyDirection>> CacheManager::loadCache(const QString& tableName, const QVariantList& infoList) {
    emit waitForFinish();

    auto data = std::make_shared<QList<FacultyDirection>>();

    QFile file(SupportSystem::appCachePath + +"/cache_" + tableLastChangeDate(tableName) + "_"
               + tableNameInCache(tableName, infoList));

    if (!file.open(QIODevice::ReadOnly)) {
	qCritical() << file.errorString();
	return nullptr;
    }

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_6_8);

    stream >> *data;

    emit finished();
    return data;
}

std::shared_ptr<QList<FacultyDirection>> CacheManager::makeCache(const QString& tableName, const QVariantList& infoList) {
    emit waitForFinish();

    TableParserBachelor parserBachelor;
    MagicHat magicHatBachelor;

    parserBachelor.setTablePath(SupportSystem::appDataPath + "/" + tableName);
    parserBachelor.setEntryCommisionInfo(infoList[0].toString(), infoList[1].toBool(), infoList[2].toInt());
    parserBachelor.parseTable();

    std::shared_ptr<QList<Applicant>> applicantsList
        = parserBachelor.getApplicants(ApplicantsFilterFlags::AdmissionsTrue, StudyType::NonBudget);

    magicHatBachelor.setPathToKCP(":/config/KCP.xlsx", "Бакалавры");
    magicHatBachelor.setApplicantsList(applicantsList);

    magicHatBachelor.startPriorityRoundSimulation();
    magicHatBachelor.rebalanceBudgetaryPlaces();
    magicHatBachelor.startGeneralRoundSimulation();

    auto data = magicHatBachelor.faculties();

    saveCache(data, tableName, infoList);

    emit finished();
    return data;
}

void CacheManager::saveCache(const std::shared_ptr<QList<FacultyDirection>>& data,
                             const QString& tableName, const QVariantList& infoList) {

    QFile file(SupportSystem::appCachePath + "/cache_" + tableLastChangeDate(tableName) + "_"
               + tableNameInCache(tableName, infoList));

    if (!file.open(QIODevice::WriteOnly)) {
	qCritical() << file.errorString();
	return;
    }

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_6_8);

    stream << *data;

    file.close();
}

QString CacheManager::tableNameInCache(const QString& tableName, const QVariantList& infoList) {

    QString tempName = tableName;
    tempName.replace(" ", "%");
    tempName.replace("_", "%");
    tempName += "%" + infoList.at(0).toString().replace(" ", "%").replace("_", "%");
    tempName += "%" + infoList.at(1).toString().replace(" ", "%").replace("_", "%");
    tempName += "%" + infoList.at(2).toString().replace(" ", "%").replace("_", "%");
    tempName += ".bin";
    return tempName;
}

QString CacheManager::tableLastChangeDate(const QString& tableName) {

    QFileInfo fileInfo(SupportSystem::appDataPath + "/" + tableName);
    const QString stringDateTime = fileInfo.lastModified().toString("yyyy-MM-dd-hh-mm-ss");
    return stringDateTime;
}
