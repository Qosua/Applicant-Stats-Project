#include "qml-helper.h"

#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QUrl>

#include "app-paths-manager.h"

QmlHelper::QmlHelper(QObject *parent) : QObject(parent) {}

void QmlHelper::openFolder(const QString &path) {
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void QmlHelper::openAppCacheFolder() {
    QDesktopServices::openUrl(QUrl(AppPathsManager::appCachePath));
}

void QmlHelper::addNewTableFromExploler() {

    QFileDialog fileDialog;
    fileDialog.setDirectory(AppPathsManager::downloadPath);

    QString src = fileDialog.getOpenFileName(nullptr, "", AppPathsManager::downloadPath,
                                             "Выберите таблицу (*.xlsx)");
    if (!src.isEmpty())
        addNewTable(src);
}

void QmlHelper::addNewTable(const QString &tableFullPath) {

    QFileInfo src(tableFullPath);

    if (!src.exists(tableFullPath) or !src.isFile()) {
        qWarning() << "File does not exist";
        return;
    }

    if (src.suffix() != "xlsx") {
        qWarning() << "File is not a .xlsx file";
        return;
    }

    setDroppedTablePath(tableFullPath);
    setDroppedTableName(tableFullPath.split("/").last());

    WindowManager::instance().openNewTableSetterWindow();

}

void QmlHelper::addTableToAppDataAndDB(QString tablePath, QString tableName, int currentCommisionIndex) {

    QString dest = AppPathsManager::appDataPath + "/" + tablePath.split('/').last();

    if (QFile::exists(dest))
        QFile::remove(dest);

    dest = AppPathsManager::appDataPath + "/" + tableName;

    QFile::copy(tablePath, dest);

    DataBaseManager::instance().addNewTablePreset(tableName, currentCommisionIndex);

}

QString QmlHelper::currentTablePath() const { return m_currentTablePath; }

QString QmlHelper::appVersion() const { return m_version; }

double QmlHelper::currentLoadBarPart() const { return m_currentLoadBarPart; }

QString QmlHelper::droppedTablePath() const {
    return m_droppedTablePath;
}

QString QmlHelper::droppedTableName() const {
    return m_droppedTableName;
}

void QmlHelper::setCurrentTablePath(const QString &path) {
    if (m_currentTablePath == path)
        return;
    m_currentTablePath = path;
    emit currentTablePathChanged();
}

void QmlHelper::setAppVersion(const QString &version) { m_version = version; }

void QmlHelper::setCurrentLoadBarPart(int v) {
    if (m_currentLoadBarPart == v)
        return;
    m_currentLoadBarPart = v;
    emit currentLoadBarPartChanged();
}

void QmlHelper::setDroppedTablePath(const QString &path) {
    if (m_droppedTablePath == path)
        return;
    m_droppedTablePath = path;
    emit droppedTablePathChanged();
}

void QmlHelper::setDroppedTableName(const QString &name) {
    if (m_droppedTableName == name)
        return;
    m_droppedTableName = name;
    emit droppedTablePathChanged();
}
