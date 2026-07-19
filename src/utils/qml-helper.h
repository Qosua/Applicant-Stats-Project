#ifndef APPLICANTSTATSPROJECT_QMLHELPER_H
#define APPLICANTSTATSPROJECT_QMLHELPER_H

#include <QObject>
#include <QString>
#include <QVariantList>

#include "window-manager.h"
#include "database-manager.h"

class QmlHelper : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString appVersion READ appVersion CONSTANT)
    Q_PROPERTY(QString droppedTableName READ droppedTableName
               WRITE setDroppedTableName NOTIFY droppedTableNameChanged)
    Q_PROPERTY(QString droppedTablePath READ droppedTablePath
               WRITE setDroppedTablePath NOTIFY droppedTablePathChanged)
    Q_PROPERTY(bool isMainWindowShadowed READ isMainWindowShadowed
               WRITE setMainWindowShadow NOTIFY currentMainWindowShadowChanged)
    Q_PROPERTY(QString currentTablePath READ currentTablePath
               WRITE setCurrentTablePath NOTIFY currentTablePathChanged)
    Q_PROPERTY(double currentLoadBarPart READ currentLoadBarPart
               WRITE setCurrentLoadBarPart NOTIFY currentLoadBarPartChanged)

public:
    explicit QmlHelper(QObject *parent = nullptr);

    Q_INVOKABLE void openFolder(const QString &path);
    Q_INVOKABLE void openAppCacheFolder();
    Q_INVOKABLE void addNewTableFromExploler();
    Q_INVOKABLE void addNewTable(const QString &tableFullPath);
    Q_INVOKABLE void addFileToAppDataAndDB(QString tablePath, QString tableName, int currentCommisionIndex);

    QString currentTablePath() const;
    QString appVersion() const;
    double currentLoadBarPart() const;
    bool isMainWindowShadowed() const;
    QString droppedTablePath() const;
    QString droppedTableName() const;

    void setCurrentTablePath(const QString &path);
    void setAppVersion(const QString &version);
    void setCurrentLoadBarPart(int v);
    void setMainWindowShadow(bool flag);
    void setDroppedTablePath(const QString &path);
    void setDroppedTableName(const QString &name);

signals:
    void sendSignalToProceedTable(const QString &tableName, const QVariantList &infoList);
    void currentTablePathChanged();
    void currentLoadBarPartChanged();
    void currentMainWindowShadowChanged();
    void droppedTablePathChanged();
    void droppedTableNameChanged();

private:
    QString m_currentTablePath;
    QString m_version;
    QString m_droppedTablePath;
    QString m_droppedTableName;
    bool m_isMainWindowShadowed = false;
    double m_currentLoadBarPart = 0.8;
};

#endif  // APPLICANTSTATSPROJECT_QMLHELPER_H
