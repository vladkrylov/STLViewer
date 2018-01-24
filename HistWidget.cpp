#include <algorithm>
#include "HistWidget.h"

HistWidget::HistWidget(QWidget *parent)
    : QCustomPlot(parent)
{
    Reset();

    // test example stuff
    int n = 10;
    QVector<double> xt(n), yt(n);
    for (int i=0; i<n; i++) {
        xt[i] = i;
        yt[i] = i*i;
    }
    xt[5] = 9;

    SetHistData(xt, yt);
    SetNBins(10);
    SetBinWidth(0.5);
    Plot();
//    for(int i=0; i<xPlot.size(); ++i) {
//        qDebug() << xPlot[i] << yPlot[i];
//    }
}

void HistWidget::Reset()
{
    x.clear();
    y.clear();
    xPlot.clear();
    yPlot.clear();
    nbins = 0;
    binsGap = 0.;
    relativeBinWidth = 1.;
}

void HistWidget::CalculateHist()
{
    int nPoints = x.size();
    if (nPoints == 0 || nPoints != y.size()) {
        qDebug() << "Cannot plot histogram, data vector dimensions do not agree.";
        return;
    }
    if (nbins <= 0) {
        qDebug() << "Cannot plot histogram, number of bins is not valid.";
        return;
    }
    // with the value of 1 the artefacts appear
    const double MAX_REL_BIN_WIDTH = 0.99999;
    if (relativeBinWidth < 0) {
        relativeBinWidth = 0.;
    } else if (relativeBinWidth > MAX_REL_BIN_WIDTH) {
        relativeBinWidth = MAX_REL_BIN_WIDTH;
    }

    // FIXME: what if nbins == 1?
    if (nbins == 1) {
        return;
    }

    xPlot.clear();
    yPlot.clear();
    xPlot.insert(xPlot.begin(), nbins*4, 0.);
    yPlot.insert(yPlot.begin(), nbins*4, 0.);

    // create vector of indices
    QVector<int> index(x.size(), 0);
    for (int i = 0 ; i != index.size() ; i++) {
        index[i] = i;
    }
    std::sort(index.begin(), index.end(),
        [&](const int& a, const int& b) {
            return (x[a] < x[b]);
        }
    );
    // now index is a vector of indices which correspond to the sorted x vector
    // i.e. x[index[0]] <= x[index[1]] <= ... <= x[index[N]]

    double xMax = max(x);
    double xMin = min(x);
    binsGap = (xMax-xMin) / (nbins-1);
    double binWidth = binsGap * relativeBinWidth;
    for(int i=0; i<nbins; ++i) {
        // build bin representation
        double binCenter = xMin + i*binsGap + binWidth/2.;
        xPlot[4*i] = binCenter - binWidth/2.;
        xPlot[4*i+1] = xPlot[4*i];
        xPlot[4*i+2] = binCenter + binWidth/2.;
        xPlot[4*i+3] = xPlot[4*i+2];
        double yval = 0.;
        // loop through all data points
        for(int j=0; j<nPoints; ++j) {
            double xj = x[index[j]];
            // check if the point.x is within current bin
            if (xj >= (binCenter - binsGap/2.) && xj < (binCenter + binsGap/2.)) {
                // if so, increment the bin height accordingly
                yval += y[index[j]];
            }
        }
        yPlot[4*i] = 0.;
        yPlot[4*i+1] = yval;
        yPlot[4*i+2] = yval;
        yPlot[4*i+3] = 0.;
    }
//    qDebug() << min(xPlot) << max(xPlot);
//    for(int i=0; i<xPlot.size(); ++i) {
//        qDebug() << xPlot[i] << yPlot[i];
//    }
}

void HistWidget::SetX(const QVector<double>& xNew)
{
    x = xNew;
}

void HistWidget::SetY(const QVector<double>& yNew)
{
    y = yNew;
}

void HistWidget::SetHistData(const QVector<double>& xNew, const QVector<double>& yNew)
{
    x = xNew;
    y = yNew;
}

QPointF HistWidget::GetPoint(int index) const
{
    return QPointF(x.at(index), y.at(index));
}

int HistWidget::GetNPoints() const
{
    int nPoints = x.size();
    if (nPoints != y.size()) {
        qDebug() << "Histogram is corrupted! Data vectors dimensions do not agree.";
        return HIST_ERROR;
    }
    return nPoints;
}

void HistWidget::SetNBins(int nbinsNew)
{
    nbins = nbinsNew;
}

void HistWidget::SetBinWidth(double relativeBinWidthNew)
{
    relativeBinWidth = relativeBinWidthNew;
}

void HistWidget::Plot()
{
    CalculateHist();
    if (graphCount() == 0) {
        addGraph();
    }
    graph()->setName("Histogram");
    graph()->setData(xPlot, yPlot);
    graph()->setLineStyle(QCPGraph::lsStepRight);
    xAxis->setRange(min(xPlot) - binsGap/2., max(xPlot) + binsGap/2.);
    yAxis->setRange(0., max(yPlot)*1.1);
    axisRect()->setupFullAxesBox();
    replot();
}

double HistWidget::min(const QVector<double>& v) const
{
    return *std::min_element(v.begin(), v.end());
}
double HistWidget::max(const QVector<double>& v) const
{
    return *std::max_element(v.begin(), v.end());
}
