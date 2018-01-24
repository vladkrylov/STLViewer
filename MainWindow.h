#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AppGLWidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:


private:
    Ui::MainWindow *ui;

    void connectSignalsSlots();

    AppGLWidget* glView;

private slots:
    void LoadFile();
};

#endif // MAINWINDOW_H
