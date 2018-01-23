#include <QApplication>
#include "AppGLWidget.h"
#include "STLParser.h"
#include "Model.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    STLParser parser;
    QFile file("box_1x1x1.stl");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "error_missing_file()";
        return 1;
    }
    Model m = parser.parse(file);
    AppGLWidget glwidget;
    glwidget.SetModel(m);
    glwidget.show();

    return app.exec();
}
