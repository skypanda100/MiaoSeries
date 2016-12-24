#ifndef RESULTWIDGET
#define RESULTWIDGET

#include <QtWidgets>
#include <vector>
#include <sstream>
#include <algorithm>
#include "qchartviewer.h"
#include "FinanceChart.h"

class ResultWidget : public QWidget{
    Q_OBJECT

public:
    ResultWidget(QWidget *parent = NULL);
    ~ResultWidget();

private:
    void initUI();
    void initConnect();
    void makeChart();
    BaseChart *finance();
    void trackFinance(MultiChart *, int);

private slots:
    void onMouseMovePlotArea(QMouseEvent *);

private:
    QChartViewer *m_ChartViewer;
};

#endif // RESULTWIDGET

