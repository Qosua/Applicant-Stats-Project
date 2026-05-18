#ifndef APPLICANTSTATSPROJECT_QMLHELPER_H
#define APPLICANTSTATSPROJECT_QMLHELPER_H

#include <QDesktopServices>
#include <QFileDialog>
#include <QFileInfo>
#include <QObject>
#include <QUrl>

#include "support-system.h"

class QmlHelper : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString currentTablePath READ currentTablePath WRITE setCurrentTablePath)
    Q_PROPERTY(QString fileSizeString READ fileSizeString WRITE setCurrentTablePath)
    Q_PROPERTY(QString appVersion READ appVersion WRITE setAppVersion)
    Q_PROPERTY(bool statusBarVisible READ statusBarVisible WRITE setStatusBarVisible)
    Q_PROPERTY(double currentLoadBarPart READ currentLoadBarPart WRITE setCurrentLoadBarPart)
public:
    explicit QmlHelper(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void openFolder(const QString &path) {
	QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    }
    Q_INVOKABLE void openAppDataFolder() {
	QDesktopServices::openUrl(QUrl(SupportSystem::appDataPath));
    }
    Q_INVOKABLE void addTableFromExploler() {

	QFileDialog fileDialog;
	fileDialog.setDirectory(SupportSystem::downloadPath);

	QString src = fileDialog.getOpenFileName(nullptr, "", SupportSystem::downloadPath,
	                                         "Выберите таблицу (*.xlsx)");
	if (!src.isEmpty())
	    copyFileToAppdata(src);
    }
    Q_INVOKABLE void copyFileToAppdata(QString src) {
	QString dest = SupportSystem::appDataPath + "/" + src.split('/').last();

	if (QFile::exists(dest))
	    QFile::remove(dest);
	QFile::copy(src, dest);
    }

    QString currentTablePath() const { return m_currentTablePath; }
    QString fileSizeString() const { return QString::number(QFileInfo(m_fileSizeString).size()); }
    QString appVersion() const { return m_version; }
    bool statusBarVisible() const { return m_statusBarVisible; }
    double currentLoadBarPart() const { return m_currentLoadBarPart; }

    void setCurrentTablePath(const QString &path) { m_currentTablePath = path; }
    void setFileSizeString(const QString &sizeString) { m_fileSizeString = sizeString; }
    void setAppVersion(const QString &version) { m_version = version; }
    void setStatusBarVisible(const bool &flag) { m_statusBarVisible = flag; }
    void setCurrentLoadBarPart(const double &part) { m_currentLoadBarPart = part; }

private:
    QString m_currentTablePath;
    QString m_fileSizeString;
    QString m_version;
    bool m_statusBarVisible;
    double m_currentLoadBarPart = 0.8;

signals:
    void sendSignalToProceedTable(const QString &tableName);
};

#endif  // APPLICANTSTATSPROJECT_QMLHELPER_H
