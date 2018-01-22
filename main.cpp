#include "AppGLWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    AppGLWidget glwidget;
    glwidget.show();

    return app.exec();
}
