#include <QtWidgets>
#include <QtOpenGL>
#include "AppGLWidget.h"

AppGLWidget::AppGLWidget(QOpenGLWidget *parent)
{
    xRot = 100;
    yRot = 50;
    scale = 1.0f;
    s0 = 1.001f;
}

AppGLWidget::~AppGLWidget()
{
}

void AppGLWidget::initializeGL()
{
    // Set up the rendering context, define display lists etc.:
    initializeOpenGLFunctions();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);

//    glEnable(GL_CULL_FACE);
//    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
//    static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void AppGLWidget::resizeGL(int w, int h)
{
    // setup viewport, projection etc.:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}

void AppGLWidget::paintGL()
{
    // draw the scene:
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glScalef(scale, scale, scale);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    draw();
}

void AppGLWidget::draw()
{
    glColor3b(1, 0, 0);
    glBegin(GL_QUADS);
        glNormal3f(0,0,-1);
        glVertex3f(-1,-1,0);
        glVertex3f(-1,1,0);
        glVertex3f(1,1,0);
        glVertex3f(1,-1,0);

    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,-1,0.707);
        glVertex3f(-1,-1,0);
        glVertex3f(1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(1,0, 0.707);
        glVertex3f(1,-1,0);
        glVertex3f(1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,1,0.707);
        glVertex3f(1,1,0);
        glVertex3f(-1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(-1,0,0.707);
        glVertex3f(-1,1,0);
        glVertex3f(-1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();
}

void AppGLWidget::mousePressEvent(QMouseEvent *event)
{
    mouseLastPos = event->pos();
}

void AppGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int rotationSpeed = 8;  // TODO: set this as a private class member?
    int dx = event->x() - mouseLastPos.x();
    int dy = event->y() - mouseLastPos.y();
    mouseLastPos = event->pos();

    xRot += rotationSpeed*dy;
    yRot += rotationSpeed*dx;
    update();
}

void AppGLWidget::wheelEvent(QWheelEvent *event)
{
    scale *= 1 + event->delta() * (s0 - 1);
    update();
}
