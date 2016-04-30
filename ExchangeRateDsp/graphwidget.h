#ifndef GRAPHWIDGET
#define GRAPHWIDGET

#include <QtWidgets>
#include "qchartviewer.h"
#include "ea_result.h"

#define DSP_DAY 2

class GraphWidget : public QWidget {
    Q_OBJECT
public:
    GraphWidget(QSize size = QSize(640, 400), QWidget *parent = 0);
    ~GraphWidget();

signals:
    void sendDetail(QStringList, QStringList);

public slots:
    void onSearchResult(QList<ExchangeRateResult *>);

private:
    void initUI();
    void initChartViewer();
    void initConnect();
    void drawChart();
    void drawFullChart();
    void trackLineLegend(XYChart *c, int mouseX);
    void trackLineAxis(XYChart *c, DrawArea *, int mouseX);
    DoubleArray getXData(int);
    double getXValue(int);

private slots:
    void onViewPortChanged();
    void onMouseMovePlotArea(QMouseEvent *event);
    void onMouseUsageChanged(int mouseUsage);
    void onChartClicked(QMouseEvent*);

private:
    QSize size;
    QChartViewer *m_ChartViewer;
    QViewPortControl *m_ViewPortControl;
    QList<ExchangeRateResult *> ea_results;
    QStringList nameList;
    QStringList valueList;
    bool isValueHide;
};
#endif // GRAPHWIDGET

