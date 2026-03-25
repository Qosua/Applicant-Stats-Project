#include <QApplication>
#include <QStyleFactory>

#include "gui/window-manager.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("windows11"));

    WindowManager winManager;
    winManager.show();

    return QApplication::exec();
}
