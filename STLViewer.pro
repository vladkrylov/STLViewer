#-------------------------------------------------
#
# Project created by QtCreator 2018-01-22T12:58:12
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = STLViewer
TEMPLATE = app


SOURCES += main.cpp \
    AppGLWidget.cpp \
    STLParser.cpp \
    Model.cpp \
    MainWindow.cpp \
    qcustomplot.cpp \
    HistWidget.cpp

HEADERS  += \
    AppGLWidget.h \
    STLParser.h \
    Model.h \
    MainWindow.h \
    qcustomplot.h \
    HistWidget.h

FORMS    += \
    MainWindow.ui

DISTFILES += \
    .gitignore \
    readme.md
