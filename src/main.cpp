#include <Windows.h>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QSortFilterProxyModel>
#include <QThread>
#include <QQuickStyle>

#include "data-processing/cache-manager.h"
#include "database-manager.h"
#include "models/stats-page-model.h"
#include "models/tables-list-model.h"
#include "models/tree-view-model.h"
#include "qml-helper.h"
#include "support-system.h"

void connectSignals(QmlHelper& qmlHelper, CacheManager& cacheManager, TreeViewModel& treeViewModel,
                    StatsPageModel& statsPageModel) {

    QObject::connect(&qmlHelper, &QmlHelper::sendSignalToProceedTable, &cacheManager,
                     &CacheManager::processTable);

    QObject::connect(&cacheManager, &CacheManager::sendProceededData, &treeViewModel,
                     &TreeViewModel::setFaculties);

    QObject::connect(&cacheManager, &CacheManager::sendProceededData, &statsPageModel,
                     &StatsPageModel::setFaculties);

    // QObject::connect(&qmlHelper, &QmlHelper::sendTreeElemName,
    //                  );
}

void moveToThread(QThread& thread, CacheManager& cacheManager) {
    cacheManager.moveToThread(&thread);
    QObject::connect(&thread, &QThread::started, &cacheManager, &CacheManager::init);
    thread.start();
}

void tempScript() {
    QXlsx::Document* doc = new QXlsx::Document(":/config/KCP.xlsx");
    TableParserBachelor parser;

    QSqlQuery query(DataBaseManager::instance().db());
    query.prepare("INSERT INTO kcp (entry_commision_id, code, department, profile_name, study_form, study_type, kcp_count)"
                                      " VALUES (:entry_commision_id, :code, :department, :profile_name, :study_form, :study_type, :kcp_count)");

    for (int i = 2; doc->read(i,1).isValid(); ++i) {

        if (doc->read(i,1).toString() == "/")
            continue;

        query.bindValue(":entry_commision_id", 1);
        query.bindValue(":code", doc->read(i,1).toString());
        query.bindValue(":department", doc->read(i,2).toString());
        query.bindValue(":profile_name", doc->read(i,3).toString());

        QString study_form = doc->read(i,4).toString();
        QString study_type = doc->read(i,5).toString();

        query.bindValue(":study_form", int(parser.extractStudyForm(study_form)));
        query.bindValue(":study_type", int(parser.extractStudyType(study_type)));

        query.bindValue(":kcp_count", doc->read(i,6).toInt());

        if (!query.exec()) {
            qCritical() << query.lastError().text();
            break;
        }
    }

    delete doc;
}

int main(int argc, char* argv[]) {

    SetConsoleOutputCP(CP_UTF8);

    // QQuickWindow::setSceneGraphBackend("software");

    QApplication app(argc, argv);

    SupportSystem::init();
    DataBaseManager::instance().init();
    QQuickStyle::setStyle("Basic");

    QThread cacheThread;
    CacheManager cacheManager;

    TablesListModel tablesListModel;
    QSortFilterProxyModel proxyModel;
    QmlHelper qmlHelper;
    TreeViewModel treeViewModel;
    StatsPageModel statsPageModel;

    qmlHelper.setAppVersion("0.8.0");

    connectSignals(qmlHelper, cacheManager, treeViewModel, statsPageModel);
    moveToThread(cacheThread, cacheManager);

    proxyModel.setSourceModel(&tablesListModel);
    proxyModel.setSortRole(TablesListModel::TablesListRoles::tableNameRole);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("sortFilterProxyModel", &proxyModel);
    engine.rootContext()->setContextProperty("qmlHelper", &qmlHelper);
    engine.rootContext()->setContextProperty("treeViewModel", &treeViewModel);
    engine.rootContext()->setContextProperty("cppStats", &statsPageModel);
    // CHECK THIS IN THE FUTURE
    engine.rootContext()->setContextProperty("applicantsListModel",
                                             statsPageModel.getApplicantsListModel().get());
    //

    engine.loadFromModule("ApplicantStatsProjectModule", "Main");

    int ret = QApplication::exec();

    cacheThread.quit();
    cacheThread.wait();

    return ret;
}
