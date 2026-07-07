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
    Q_PROPERTY(QString appVersion READ appVersion CONSTANT)
    Q_PROPERTY(QString currentTablePath READ currentTablePath WRITE setCurrentTablePath NOTIFY currentTablePathChanged)
    Q_PROPERTY(double currentLoadBarPart READ currentLoadBarPart WRITE setCurrentLoadBarPart NOTIFY currentLoadBarPartChanged)
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
    QString appVersion() const { return m_version; }
    double currentLoadBarPart() const { return m_currentLoadBarPart; }

    void setCurrentTablePath(const QString &path) {
        if (m_currentTablePath == path) return;
        m_currentTablePath = path;
        emit currentTablePathChanged();
    }
    void setAppVersion(const QString &version) { m_version = version; }
    void setCurrentLoadBarPart(int v) {
        if (m_currentLoadBarPart == v) return;
        m_currentLoadBarPart = v;
        emit currentLoadBarPartChanged();
    }

private:
    QString m_currentTablePath;
    QString m_version;
    double m_currentLoadBarPart = 0.8;

signals:
    void sendSignalToProceedTable(const QString &tableName);
    void currentTablePathChanged();
    void currentLoadBarPartChanged();
};

#endif  // APPLICANTSTATSPROJECT_QMLHELPER_H
