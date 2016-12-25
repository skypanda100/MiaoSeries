#ifndef RESULTWIDGET
#define RESULTWIDGET

#include <QtWidgets>
#include <vector>
#include <sstream>
#include <algorithm>
#include "qchartviewer.h"
#include "FinanceChart.h"
#include "ea_result.h"
#include "style.h"

#define UP_COLOR        0xaa0000
#define DW_COLOR        0x008000
#define UP_COLOR_C      "aa0000"
#define DW_COLOR_C      "008000"
#define MA14_COLOR      0x663300
#define MA30_COLOR      0x663300
#define MA60_COLOR      0x663300
#define MA99_COLOR      0x663300
#define MA100_COLOR     0x663300
#define MA250_COLOR     0x663300
#define MA888_COLOR     0x663300
#define KDJ_K_COLOR     0x006060
#define KDJ_D_COLOR     0x606000
#define KDJ_J_COLOR     0xff6000
#define MACD_COLOR      0x0000ff
#define EXP_COLOR       0xff00ff
#define RSI_R_COLOR     0x800080
#define RSI_S_COLOR     0x666600
#define RSI_I_COlOR     0x6600ff

class ResultWidget : public QWidget{
    Q_OBJECT

public:
    ResultWidget(QWidget *parent = NULL);
    ~ResultWidget();

public slots:
    void onSearch(QList<ExchangeRateResult *>, QList<int>, int);
    void onStyleChanged();

private:
    void initUI();
    void initConnect();
    void makeChart(const QList<ExchangeRateResult *> &, const QList<int> &, int);
    BaseChart *finance(const QList<ExchangeRateResult *> &, const QList<int> &, int);
    void trackFinance(MultiChart *, int);

private slots:
    void onMouseMovePlotArea(QMouseEvent *);
    void onChartClicked(QMouseEvent*);

private:
    QChartViewer *m_ChartViewer;
    bool m_tack;
    QList<ExchangeRateResult *> m_lastResults;
    QList<int > m_lastMaList;
    int m_lastExtra;
};

#endif // RESULTWIDGET

