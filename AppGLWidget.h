#ifndef AppGLWidget_H
#define AppGLWidget_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "Model.h"

class AppGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit AppGLWidget(QOpenGLWidget *parent = 0);
    ~AppGLWidget();

    void SetModel(Model model);

protected:
    // opengl stuff
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    // user event handlers
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void Draw();
    void DrawModel();

    Model m;

    QPoint mouseLastPos;
    int xRot;
    int yRot;
    float scale, s0;
};

#endif // AppGLWidget_H
