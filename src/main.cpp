#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QSortFilterProxyModel>
#include <QThread>

#include "data-processing/cache-manager.h"
#include "models/entry-commisions-list-model.h"
#include "models/stats-page-model.h"
#include "models/tables-list-model.h"
#include "models/tree-view-model.h"
#include "utils/qml-helper.h"
#include "utils/database-manager.h"
#include "utils/support-system.h"
#include "utils/window-manager.h"

void connectSignals(QmlHelper& qmlHelper, CacheManager& cacheManager, TreeViewModel& treeViewModel,
                    StatsPageModel& statsPageModel) {

    QObject::connect(&qmlHelper, &QmlHelper::sendSignalToProceedTable, &cacheManager,
                     &CacheManager::processTable);

    QObject::connect(&cacheManager, &CacheManager::sendProceededData, &treeViewModel,
                     &TreeViewModel::setFaculties);

    QObject::connect(&cacheManager, &CacheManager::sendProceededData, &statsPageModel,
                     &StatsPageModel::setFaculties);
}

void moveToThread(QThread& thread, CacheManager& cacheManager) {
    cacheManager.moveToThread(&thread);
    QObject::connect(&thread, &QThread::started, &cacheManager, &CacheManager::init);
    thread.start();
}

int main(int argc, char* argv[]) {

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
    EntryCommisionsListModel entryModel;

    qmlHelper.setAppVersion("0.9.0");

    connectSignals(qmlHelper, cacheManager, treeViewModel, statsPageModel);
    moveToThread(cacheThread, cacheManager);

    proxyModel.setSourceModel(&tablesListModel);
    proxyModel.setSortRole(TablesListModel::TablesListRoles::tableNameRole);

    QQmlApplicationEngine engine;
    WindowManager::init(&engine);

    engine.rootContext()->setContextProperty("sortFilterProxyModel", &proxyModel);
    engine.rootContext()->setContextProperty("tablesListModel", &tablesListModel);
    engine.rootContext()->setContextProperty("qmlHelper", &qmlHelper);
    engine.rootContext()->setContextProperty("treeViewModel", &treeViewModel);
    engine.rootContext()->setContextProperty("statsPageModel", &statsPageModel);
    engine.rootContext()->setContextProperty("entryCommisionsListModel", &entryModel);
    engine.rootContext()->setContextProperty("applicantsListModel", statsPageModel.getApplicantsListModel().get());
    engine.loadFromModule("ApplicantStatsProjectModule", "Main");

    int ret = QApplication::exec();

    cacheThread.quit();
    cacheThread.wait();

    return ret;
}
