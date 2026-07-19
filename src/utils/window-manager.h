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

public:
    static WindowManager& instance();
    static void init(QQmlApplicationEngine* engine, QObject* parent = nullptr);

    Q_INVOKABLE void openNewTableSetterWindow();

private:
    WindowManager() = default;

    QQmlApplicationEngine* m_engine;

};

#endif  // APPLICANTSTATSPROJECT_WINDOWMANAGER_H
