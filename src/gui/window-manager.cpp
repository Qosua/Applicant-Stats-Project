#include "ui_mainwidget.h"
#include "window-manager.h"

WindowManager::WindowManager() {
    
    mainWidget.setAcceptDrops(true);

    stackedLayout.addWidget(&mainWidget);

    centralWidget.setLayout(&stackedLayout);
    this->setCentralWidget(&centralWidget);
    
    QObject::connect(&mainWidget,   &MainWidget::sendTable,
                     &tableManager, &TableManager::processTable);
    
    QObject::connect(&tableManager, &TableManager::sendTableList,
                     &mainWidget,   &MainWidget::loadTablesToUi);
    
    QObject::connect(&tableManager, &TableManager::waitForFinish,
                     &mainWidget,   &MainWidget::wait);
    
    QObject::connect(&tableManager, &TableManager::finished,
                     &mainWidget,   &MainWidget::stopWaiting);
    
    QObject::connect(this,          &WindowManager::initTableManager,
                     &tableManager, &TableManager::init, Qt::SingleShotConnection);
    
    tableManager.moveToThread(&thread);
    thread.start();
    
    supportSystem.init();
    emit initTableManager();
    
}

WindowManager::~WindowManager() {
    
    
    
}
