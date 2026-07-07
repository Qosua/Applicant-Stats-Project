#include <Windows.h>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QSortFilterProxyModel>
#include <QThread>

#include "data-processing/cache-manager.h"
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

int main(int argc, char* argv[]) {

    SetConsoleOutputCP(CP_UTF8);

    // QQuickWindow::setSceneGraphBackend("software");

    QApplication app(argc, argv);
    SupportSystem::init();
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
    engine.rootContext()->setContextProperty("applicantsListModel", statsPageModel.getApplicantsListModel().get());
    //

    engine.loadFromModule("ApplicantStatsProjectModule", "Main");

    int ret = QApplication::exec();

    cacheThread.quit();
    cacheThread.wait();

    return ret;
}
