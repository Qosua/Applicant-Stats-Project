#ifndef APPLICANTSTATSPROJECT_WINDOWMANAGER_H
#define APPLICANTSTATSPROJECT_WINDOWMANAGER_H

#include <QObject>
#include <QPointer>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickWindow>
#include <QQmlContext>

#include "src/utils/qml-helper.h"

class WindowManager : public QObject{
    Q_OBJECT

    Q_PROPERTY(bool isMainWindowShadowed READ isMainWindowShadowed
               WRITE setMainWindowShadow NOTIFY currentMainWindowShadowChanged)
public:
    static WindowManager& instance();
    static void init(QQmlApplicationEngine* engine, QObject* parent = nullptr);

    Q_INVOKABLE void openNewTableSetterWindow();

    bool isMainWindowShadowed() const;

    void setMainWindowShadow(bool flag);

signals:
    void currentMainWindowShadowChanged();

private:
    WindowManager() = default;
    WindowManager(const WindowManager&) = delete;

    QQmlApplicationEngine* m_engine;
    bool m_isMainWindowShadowed = false;

};

#endif  // APPLICANTSTATSPROJECT_WINDOWMANAGER_H
