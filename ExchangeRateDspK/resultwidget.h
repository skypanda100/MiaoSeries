#ifndef RESULTWIDGET
#define RESULTWIDGET

#include <QtWidgets>
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

private:
    QChartViewer *m_ChartViewer;
};

#endif // RESULTWIDGET

