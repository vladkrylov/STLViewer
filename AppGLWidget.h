#ifndef AppGLWidget_H
#define AppGLWidget_H

#include <QOpenGLWidget>

class AppGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit AppGLWidget(QOpenGLWidget *parent = 0);
    ~AppGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    void draw();

    int xRot;
    int yRot;
    int zRot;
};

#endif // AppGLWidget_H
