#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "AppGLWidget.h"
#include "HistWidget.h"

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
    void connectSignalsSlots();

    Ui::MainWindow *ui;
    QLabel* statusLabel;
    AppGLWidget* glView;
    HistWidget* analysisWidget;

private slots:
    void LoadFile();
};

#endif // MAINWINDOW_H
