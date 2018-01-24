#include <QMessageBox>
#include <QFileDialog>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "STLParser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowTitle("STLViewer");
    ui->setupUi(this);

    QGridLayout *layout3Dview = new QGridLayout(ui->tab3D);
    glView = new AppGLWidget(ui->tab3D);

    layout3Dview->addWidget(glView);
    ui->tab3D->setLayout(layout3Dview);

    statusLabel = new QLabel(ui->statusbar);
    statusLabel->setText("Default model");
    ui->statusbar->addWidget(statusLabel);

    connectSignalsSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSignalsSlots()
{
    QObject::connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::LoadFile);
}

void MainWindow::LoadFile()
{
    STLParser parser;
    QString filename = QFileDialog::getOpenFileName(this, "Load ASCII .stl model", QString(), "STL Files (*.stl)");
    if (!filename.isNull())
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
        {
            qDebug() << "Cannot open file";
            return;
        }
        Model m = parser.parse(file);
        if (m.isInitialized()) {
            glView->SetModel(m);
            glView->show();
            // FIXME: what if filename is veery long?
            statusLabel->setText(filename + ": " + m.GetName());
        } else {
            qDebug() << "Cannot read  ASCII .stl file";
        }
    } else {
        qDebug() << "file does not exists";
        return;
    }
}
