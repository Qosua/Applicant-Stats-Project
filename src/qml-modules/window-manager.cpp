#include "window-manager.h"

void WindowManager::init(QQmlApplicationEngine* engine, QObject* parent) {
    WindowManager::instance().m_engine = engine;
}

WindowManager& WindowManager::instance() {
    static WindowManager windowManager;
    return windowManager;
}

void WindowManager::openNewTableSetterWindow() {

    static QPointer<QQuickWindow> m_tableSetterWindow;

    if (m_tableSetterWindow) {
        this->setMainWindowShadow(true);
        m_tableSetterWindow->show();
        m_tableSetterWindow->raise();
        m_tableSetterWindow->requestActivate();
        return;
    }

    QQmlComponent component(m_engine);
    component.loadFromModule("ApplicantStatsProjectModule", "NewTableSetter");

    m_tableSetterWindow = qobject_cast<QQuickWindow*>(component.create(m_engine->rootContext()));

    if (!m_engine->rootObjects().isEmpty()) {
        auto mainWindow = qobject_cast<QQuickWindow*>(m_engine->rootObjects().first());
        if (mainWindow) {

            m_tableSetterWindow->setTransientParent(mainWindow);
            this->setMainWindowShadow(true);
        }
    }

    m_tableSetterWindow->show();

}
bool WindowManager::isMainWindowShadowed() const {
    return m_isMainWindowShadowed;
}

void WindowManager::setMainWindowShadow(bool flag) {
    m_isMainWindowShadowed = flag;
    emit currentMainWindowShadowChanged();
}