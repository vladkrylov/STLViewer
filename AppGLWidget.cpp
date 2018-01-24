#include <QtWidgets>
#include <QtOpenGL>
#include "AppGLWidget.h"

AppGLWidget::AppGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    xRot = 40;
    yRot = 70;
    scale = 1.0f;
    s0 = 1.001f;
}

AppGLWidget::~AppGLWidget()
{
}

void AppGLWidget::SetModel(Model model)
{
    m = model;
    resize(width(), height());
    update();
}

void AppGLWidget::initializeGL()
{
    // Set up the rendering context, define display lists etc.:
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    static GLfloat lightPosition[4] = { 0.f, 0.f, 1.f, 0.f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

}

void AppGLWidget::resizeGL(int w, int h)
{
    // setup viewport, projection etc.:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int l = qMin(w, h);
    glViewport(0, 0, l, l);
    glMatrixMode(GL_MODELVIEW);
}

void AppGLWidget::paintGL()
{
    // draw the scene:
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (m.isInitialized()) {
        float wf = m.xMax() - m.xMin();
        float hf = m.yMax() - m.yMin();
        float df = m.zMax() - m.zMin();
        float diag = sqrt(wf*wf + hf*hf + df*df);
        glOrtho(-0.5*diag, +0.5*diag, -0.5*diag, +0.5*diag, 2.0, 2.0+2*diag);
        glTranslatef(0.0f, 0.0f, -2.0f-diag);
    } else {
        glOrtho(-1.5, +1.5, -1.5, +1.5, 2.0, 9.0);
        glTranslatef(0.0f, 0.0f, -5.5f);
    }

    glMatrixMode(GL_MODELVIEW);
    glScalef(scale, scale, scale);
    glRotatef(xRot / 2.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot / 2.0f, 0.0f, 1.0f, 0.0f);
    Draw();
}

void AppGLWidget::Draw()
{
    // check if model has been initialized
    if (m.isInitialized()) {
        DrawModel();
    } else {
        glColor3b(1, 0, 0);
        glBegin(GL_QUADS);
            glNormal3f(0, 0, -1);
            glVertex3f(-1, -1, 0);
            glVertex3f(-1, 1, 0);
            glVertex3f(1, 1, 0);
            glVertex3f(1, -1, 0);

        glEnd();
        glBegin(GL_TRIANGLES);
            glNormal3f(0, -1, 0.707);
            glVertex3f(-1, -1, 0);
            glVertex3f(1, -1, 0);
            glVertex3f(0, 0, 1.2);
        glEnd();
        glBegin(GL_TRIANGLES);
            glNormal3f(1, 0, 0.707);
            glVertex3f(1, -1, 0);
            glVertex3f(1, 1, 0);
            glVertex3f(0, 0, 1.2);
        glEnd();
        glBegin(GL_TRIANGLES);
            glNormal3f(0,1,0.707);
            glVertex3f(1,1,0);
            glVertex3f(-1,1,0);
            glVertex3f(0,0,1.2);
        glEnd();
        glBegin(GL_TRIANGLES);
            glNormal3f(-1, 0, 0.707);
            glVertex3f(-1, 1, 0);
            glVertex3f(-1, -1, 0);
            glVertex3f(0, 0, 1.2);
        glEnd();
    }
}

void AppGLWidget::DrawModel()
{
    glColor3b(1, 0, 0);
    QVector3D n;
    QVector3D v;
    STLTriangle t;
    QVector3D center = m.GetCenter();
    for(int i = 0; i < m.GetNTriangles(); ++i) {
        t = m.GetTriangle(i);
        glBegin(GL_TRIANGLES);
            n = t.GetNormal();
            glNormal3f(n.x(), n.y(), n.z());
            for(size_t j=0; j<3; ++j) {
                v = t.GetVertex(j);
                glVertex3f(v.x()-center.x(),
                           v.y()-center.y(),
                           v.z()-center.z());
            }
        glEnd();
    }
}

void AppGLWidget::mousePressEvent(QMouseEvent *event)
{
    mouseLastPos = event->pos();
}

void AppGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - mouseLastPos.x();
    int dy = event->y() - mouseLastPos.y();
    mouseLastPos = event->pos();

    xRot += dy;
    yRot += dx;
    update();
}

void AppGLWidget::wheelEvent(QWheelEvent *event)
{
    scale *= 1 + event->delta() * (s0 - 1);
    update();
}

