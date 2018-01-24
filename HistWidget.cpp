#include <algorithm>
#include "HistWidget.h"

HistWidget::HistWidget(QWidget *parent)
    : QCustomPlot(parent)
{
    Reset();
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

    rangeDefinedByUser = false;
    userXMinDisp = 0.;
    userXMaxDisp = 0.;
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
    xPlot.insert(xPlot.begin(), nbins*2, 0.);
    yPlot.insert(yPlot.begin(), nbins*2, 0.);

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
        xPlot[2*i] = binCenter - binWidth/2.;
        xPlot[2*i+1] = binCenter + binWidth/2.;
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
        yPlot[2*i] = 0.;
        yPlot[2*i+1] = yval;
    }
    // add a point to correctly finish histogram
    xPlot.push_back(xMax+binsGap);
    yPlot.push_back(0.);

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

void HistWidget::SetUserDispRange(double xmin, double xmax)
{
    if (xmin >= xmax) {
        qDebug() << "Wrong user range limits!";
        rangeDefinedByUser = false;
        return;
    }
    rangeDefinedByUser = true;
    userXMinDisp = xmin;
    userXMaxDisp = xmax;
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
    if (rangeDefinedByUser) {
        xAxis->setRange(userXMinDisp, userXMaxDisp);
    } else {
        xAxis->setRange(min(xPlot) - binsGap/2., max(xPlot) + binsGap/2.);
    }
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
