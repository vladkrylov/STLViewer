#ifndef HISTWIDGET_H
#define HISTWIDGET_H

#include "qcustomplot.h"

class HistWidget : public QCustomPlot
{
    Q_OBJECT
public:
    explicit HistWidget(QWidget *parent = 0);

    void Reset();
    void SetX(const QVector<double>& xNew);
    void SetY(const QVector<double>& yNew);
    void SetHistData(const QVector<double>& xNew, const QVector<double>& yNew);
    QPointF GetPoint(int index) const;
    int GetNPoints() const;

    void SetNBins(int nbinsNew);
    void SetBinWidth(double binWidthNew);
    void SetUserDispRange(double xmin, double xmax);

    void Plot();

private:
    void CalculateHist();

    QVector<double> x, y;
    QVector<double> xPlot, yPlot;
    int nbins;
    double binsGap;
    double relativeBinWidth;

    bool rangeDefinedByUser;
    double userXMinDisp;
    double userXMaxDisp;

    double min(const QVector<double>& v) const;
    double max(const QVector<double>& v) const;

    enum status {HIST_ERROR = -1};
};

#endif // HISTWIDGET_H
