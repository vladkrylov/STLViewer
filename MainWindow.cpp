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

    // 3D tab widgets
    QGridLayout *layout3DView = new QGridLayout(ui->tab3D);
    glView = new AppGLWidget(ui->tab3D);

    layout3DView->addWidget(glView);
    ui->tab3D->setLayout(layout3DView);

    // Histogram tab widget
    QGridLayout *layoutAnalysisView = new QGridLayout(ui->tabAnalysis);
    analysisWidget = new HistWidget(ui->tabAnalysis);

    layoutAnalysisView->addWidget(analysisWidget);
    ui->tabAnalysis->setLayout(layoutAnalysisView);

    // test plot
    analysisWidget->replot();

    // statusbar
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

            // update the histogram
            int nTriangles = m.GetNTriangles();
            QVector<double> x(nTriangles), y(nTriangles);
            for(int i=0; i<nTriangles; ++i) {
                x[i] = m.GetTriangle(i).GetTheta();
                y[i] = m.GetTriangle(i).GetArea();
            }
            analysisWidget->SetHistData(x, y);
            analysisWidget->SetNBins(100);
            analysisWidget->SetBinWidth(1.);
            analysisWidget->SetUserDispRange(-100., 100);
            analysisWidget->Plot();
        } else {
            qDebug() << "Cannot read  ASCII .stl file";
        }
    } else {
        qDebug() << "file does not exists";
        return;
    }
}
