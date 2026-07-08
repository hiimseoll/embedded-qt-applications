#include "mainwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QApplication a(argc, argv);
    mainWidget w;
    w.show();
    return QApplication::exec();
}
