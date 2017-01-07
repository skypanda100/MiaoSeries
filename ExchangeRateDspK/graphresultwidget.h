#ifndef GRAPHRESULTWIDGET
#define GRAPHRESULTWIDGET

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
#define BOLL_LINE_COLOR 0x9999ff
#define BOLL_FILL_COLOR 0xc06666ff
#define BOLL_MA_COLOR   0xff9900
#define MA5_COLOR       0x666666
#define MA14_COLOR      0xaaaaaa
#define MA20_COLOR      0xffe599
#define MA30_COLOR      0xff9900
#define MA60_COLOR      0xffff00
#define MA99_COLOR      0xff00ff
#define MA100_COLOR     0xff00ff
#define MA250_COLOR     0x00ffff
#define MA888_COLOR     0x00ff00
#define KDJ_K_COLOR     0xaa0000
#define KDJ_D_COLOR     0x008000
#define KDJ_J_COLOR     0x000000
#define MACD_COLOR      0xff00ff
#define EXP_COLOR       0xffff00
#define RSI_R_COLOR     0x980000
#define RSI_S_COLOR     0xff9900
#define RSI_I_COlOR     0x4a86e8

class GraphResultWidget : public QWidget{
    Q_OBJECT

public:
    GraphResultWidget(QWidget *parent = NULL);
    ~GraphResultWidget();

public slots:
    void onSearch(QList<ExchangeRateResult *>, QList<int>, int, bool);
    void onStyleChanged();

protected:
    void wheelEvent(QWheelEvent *) Q_DECL_OVERRIDE;

private:
    void clearData();
    void initUI();
    void initConnect();
    void makeChart(const QList<ExchangeRateResult *> &, const QList<int> &, int, bool);
    BaseChart *finance(const QList<ExchangeRateResult *> &, const QList<int> &, int, bool);
    void trackFinance(MultiChart *, int, bool isRelateToTrackVar = true);
    void zoomIn();
    void zoomOut();

private slots:
    void onMouseMovePlotArea(QMouseEvent *);
    void onChartClicked(QMouseEvent*);

private:
    QChartViewer *m_ChartViewer;
    bool m_tack;
    QList<ExchangeRateResult *> m_allResults;
    QList<ExchangeRateResult *> m_lastResults;
    QList<int > m_lastMaList;
    int m_lastExtra;
    bool m_lastBoll;

    float m_zoomFactor;
    int m_zoomXValue;
    QVector<int> m_zoomLeftCountVec;
    QVector<int> m_zoomRightCountVec;
    QVector<ExchangeRateResult *> m_zoomLeftVec;
    QVector<ExchangeRateResult *> m_zoomRightVec;
};

#endif // GRAPHRESULTWIDGET

