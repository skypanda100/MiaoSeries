#ifndef RESULTWIDGET
#define RESULTWIDGET

#include <QtWidgets>
#include <vector>
#include <sstream>
#include <algorithm>
#include "qchartviewer.h"
#include "FinanceChart.h"
#include "ea_result.h"

class ResultWidget : public QWidget{
    Q_OBJECT

public:
    ResultWidget(QWidget *parent = NULL);
    ~ResultWidget();

public slots:
    void onSearch(QList<ExchangeRateResult *>, QList<int>, int);

private:
    void initUI();
    void initConnect();
    void makeChart(const QList<ExchangeRateResult *> &, const QList<int> &, int);
    BaseChart *finance(const QList<ExchangeRateResult *> &, const QList<int> &, int);
    void trackFinance(MultiChart *, int);

private slots:
    void onMouseMovePlotArea(QMouseEvent *);

private:
    QChartViewer *m_ChartViewer;
};

#endif // RESULTWIDGET

