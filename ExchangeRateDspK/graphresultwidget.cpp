#include "graphresultwidget.h"

using namespace std;

GraphResultWidget::GraphResultWidget(QWidget *parent)
    :QWidget(parent)
    ,m_tack(true)
    ,m_zoomFactor(0.1)
    ,m_zoomXValue(0)
{
    this->initUI();
    this->initConnect();
}

GraphResultWidget::~GraphResultWidget(){
    delete m_ChartViewer;
    clearData();
}

void GraphResultWidget::clearData(){
    m_lastMaList.clear();
    m_zoomLeftCountVec.clear();
    m_zoomRightCountVec.clear();
    for(Result *eaResult : m_zoomLeftVec){
        delete eaResult;
    }
    m_zoomLeftVec.clear();
    for(Result *eaResult : m_zoomRightVec){
        delete eaResult;
    }
    m_zoomRightVec.clear();
    for(Result *eaResult : m_lastResults){
        delete eaResult;
    }
    m_lastResults.clear();
}

void GraphResultWidget::onSearch(QList<Result *> eaResults, QList<int> maList, int extra, bool isBoll){
    makeChart(eaResults, maList, extra, isBoll);
    if(this->sender() != NULL){
        clearData();
        FinanceChart *c = (FinanceChart *)(m_ChartViewer->getChart());
        trackFinance(c, ((XYChart *)c->getChart(0))->getPlotArea()->getRightX());
        m_ChartViewer->updateDisplay();
    }else{
        FinanceChart *c = (FinanceChart *)(m_ChartViewer->getChart());
        trackFinance(c, m_zoomXValue, false);
        m_ChartViewer->updateDisplay();
    }
    m_lastResults = eaResults;
    m_lastMaList = maList;
    m_lastExtra = extra;
    m_lastBoll = isBoll;
}

void GraphResultWidget::onStyleChanged(){
    makeChart(m_lastResults, m_lastMaList, m_lastExtra, m_lastBoll);
    FinanceChart *c = (FinanceChart *)(m_ChartViewer->getChart());
    trackFinance(c, m_zoomXValue, false);
    m_ChartViewer->updateDisplay();
}

void GraphResultWidget::onBuy(){

}

void GraphResultWidget::onSell(){

}

void GraphResultWidget::wheelEvent(QWheelEvent *wheelEvent){
    if(m_lastResults.count() == 0){
        wheelEvent->ignore();
        return;
    }
    if(wheelEvent->orientation() == Qt::Vertical){
        wheelEvent->accept();
        QPoint numDegrees = wheelEvent->angleDelta() / 8;
        if(numDegrees.y() > 0){
            zoomIn();
        }else if(numDegrees.y() < 0){
            zoomOut();
        }
    }else{
        wheelEvent->ignore();
    }
}

void GraphResultWidget::initUI(){
    m_ChartViewer = new QChartViewer(this);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(2, 2, 2, 2);
    mainLayout->addWidget(m_ChartViewer);

    this->setLayout(mainLayout);
}

void GraphResultWidget::initConnect(){
    connect(m_ChartViewer, SIGNAL(mouseMovePlotArea(QMouseEvent*)), SLOT(onMouseMovePlotArea(QMouseEvent*)));
    connect(m_ChartViewer, SIGNAL(clicked(QMouseEvent*)), SLOT(onChartClicked(QMouseEvent*)));
}

void GraphResultWidget::makeChart(const QList<Result *> &eaResults, const QList<int> &maList, int extra, bool isBoll){
    if(m_ChartViewer->getChart() != NULL){
        delete m_ChartViewer->getChart();
    }
    m_ChartViewer->setChart(finance(eaResults, maList, extra, isBoll));
}

BaseChart *GraphResultWidget::finance(const QList<Result *> &eaResults, const QList<int> &maList, int extra, bool isBoll){
    int extraDays = extra;

    int count = eaResults.count();
    double *timeStamps_p = new double[count];
    double *highData_p = new double[count];
    double *lowData_p = new double[count];
    double *openData_p = new double[count];
    double *closeData_p = new double[count];

    for(int i = 0;i < count;i++){
        Result *eaResult = eaResults[i];

        timeStamps_p[i] = Chart::chartTime2(QDateTime::fromString(eaResult->date(), "yyyy-MM-ddThh:mm:ss").toTime_t());
        highData_p[i] = eaResult->high();
        lowData_p[i] = eaResult->low();
        openData_p[i] = eaResult->open();
        closeData_p[i] = eaResult->close();
    }

    DoubleArray timeStamps = DoubleArray(timeStamps_p, count);
    DoubleArray highData = DoubleArray(highData_p, count);
    DoubleArray lowData = DoubleArray(lowData_p, count);
    DoubleArray openData = DoubleArray(openData_p, count);
    DoubleArray closeData = DoubleArray(closeData_p, count);
    DoubleArray volData;

    FinanceChart *c = new FinanceChart(this->width());

    // set style
    c->setBackground(GET_STYLE().main_bg_color);
    c->setPlotAreaStyle(GET_STYLE().plot_bg_color, GET_STYLE().grid_color, GET_STYLE().grid_color, GET_STYLE().grid_color, GET_STYLE().grid_color);
    c->setPlotAreaBorder(Chart::Transparent, 2);
    c->setXAxisStyle("normal", 8, GET_STYLE().font_color, 0);
    c->setYAxisStyle("normal", 8, GET_STYLE().font_color, 14);

    // set margins
    c->setMargins(10, 0, 40, 35);

//    c->addTitle("Finance Chart Demonstration");

    c->setLegendStyle("normal", 8, Chart::Transparent, Chart::Transparent);

    c->setData(timeStamps, highData, lowData, openData, closeData, volData, extraDays);

    c->addMainChart(this->height() - 480);

    for(int ma : maList){
        int color = MA5_COLOR;
        switch(ma){
        case 5:
            color = MA5_COLOR;
            break;
        case 14:
            color = MA14_COLOR;
            break;
        case 20:
            color = MA20_COLOR;
            break;
        case 30:
            color = MA30_COLOR;
            break;
        case 60:
            color = MA60_COLOR;
            break;
        case 99:
            color = MA99_COLOR;
            break;
        case 100:
            color = MA100_COLOR;
            break;
        case 250:
            color = MA250_COLOR;
            break;
        case 888:
            color = MA888_COLOR;
            break;
        }

        c->addSimpleMovingAvg(ma, color);
    }

    c->addCandleStick(UP_COLOR, DW_COLOR, GET_STYLE().edge_color);

    if(isBoll){
        c->addBollingerBand(20, 2, BOLL_LINE_COLOR, BOLL_FILL_COLOR);
        c->addSimpleMovingAvg(20, BOLL_MA_COLOR);
    }

//    c->addVolBars(150, 0x99ff99, 0xff9999, 0x808080);

    c->addKDJ(150, 9, 3, 3, KDJ_K_COLOR, KDJ_D_COLOR, KDJ_J_COLOR);

    c->addMACD(150, 26, 12, 9, MACD_COLOR, EXP_COLOR, UP_COLOR, DW_COLOR);

    c->addRSI(150, 6, 12, 24, RSI_R_COLOR, RSI_S_COLOR, RSI_I_COlOR, 30, UP_COLOR, DW_COLOR);

    c->makeChart();

    delete timeStamps_p;
    delete highData_p;
    delete lowData_p;
    delete openData_p;
    delete closeData_p;

    return c;
}

void GraphResultWidget::trackFinance(MultiChart *m, int mouseX, bool isRelateToTrackVar){
    if(isRelateToTrackVar){
        if(!m_tack){
            return;
        }
    }
    // Clear the current dynamic layer and get the DrawArea object to draw on it.
    DrawArea *d = m->initDynamicLayer();

    // It is possible for a FinanceChart to be empty, so we need to check for it.
    if (m->getChartCount() == 0)
        return ;

    // Get the data x-value that is nearest to the mouse
    int xValue = isRelateToTrackVar ? (int)(((XYChart *)m->getChart(0))->getNearestXValue(mouseX)) : mouseX;

    // set zoom factor
    m_zoomXValue = xValue;

    // Iterate the XY charts (main price chart and indicator charts) in the FinanceChart
    XYChart *c = 0;
    for(int i = 0; i < m->getChartCount(); ++i) {
        c = (XYChart *)m->getChart(i);

        // Variables to hold the legend entries
        ostringstream ohlcLegend;
        vector<string> legendEntries;

        // Iterate through all layers to find the highest data point
        for(int j = 0; j < c->getLayerCount(); ++j) {
            Layer *layer = c->getLayerByZ(j);
            int xIndex = layer->getXIndexOf(xValue);
            int dataSetCount = layer->getDataSetCount();

            // In a FinanceChart, only layers showing OHLC data can have 4 data sets
            if (dataSetCount == 4) {
                double highValue = layer->getDataSet(0)->getValue(xIndex);
                double lowValue = layer->getDataSet(1)->getValue(xIndex);
                double openValue = layer->getDataSet(2)->getValue(xIndex);
                double closeValue = layer->getDataSet(3)->getValue(xIndex);

                if (closeValue != Chart::NoValue) {
                    // Build the OHLC legend
                    ohlcLegend << "      <*block*>";
                    ohlcLegend << "Open: " << c->formatValue(openValue, "{value|P4}");
                    ohlcLegend << ", High: " << c->formatValue(highValue, "{value|P4}");
                    ohlcLegend << ", Low: " << c->formatValue(lowValue, "{value|P4}");
                    ohlcLegend << ", Close: " << c->formatValue(closeValue, "{value|P4}");

                    // We also draw an upward or downward triangle for up and down days and the %
                    // change
                    double lastCloseValue = layer->getDataSet(3)->getValue(xIndex - 1);
                    if (lastCloseValue != Chart::NoValue) {
                        double change = closeValue - lastCloseValue;
                        double percent = change * 100 / closeValue;
                        string symbol = (change >= 0) ?
                            "<*font,color="+string(UP_COLOR_C)+"*><*img=@triangle,width=8,color="+string(UP_COLOR_C)+"*>" :
                            "<*font,color="+string(DW_COLOR_C)+"*><*img=@invertedtriangle,width=8,color="+string(DW_COLOR_C)+"*>" ;

                        ohlcLegend << "  " << symbol << " " << c->formatValue(change, "{value|P4}");
                        ohlcLegend << " (" << c->formatValue(percent, "{value|2}") << "%)<*/font*>";
                    }

                    ohlcLegend << "<*/*>";
                }
            } else {
                // Iterate through all the data sets in the layer
                for(int k = 0; k < layer->getDataSetCount(); ++k) {
                    DataSet *dataSet = layer->getDataSetByZ(k);

                    string name = dataSet->getDataName();
                    double value = dataSet->getValue(xIndex);
                    if ((0 != name.size()) && (value != Chart::NoValue)) {

                        // In a FinanceChart, the data set name consists of the indicator name and its
                        // latest value. It is like "Vol: 123M" or "RSI (14): 55.34". As we are
                        // generating the values dynamically, we need to extract the indictor name
                        // out, and also the volume unit (if any).

                        // The volume unit
                        string unitChar;

                        // The indicator name is the part of the name up to the colon character.
                        int delimiterPosition = (int)name.find(':');
                        if ((int)name.npos != delimiterPosition) {

                            // The unit, if any, is the trailing non-digit character(s).
                            int lastDigitPos = (int)name.find_last_of("0123456789");
                            if (((int)name.npos != lastDigitPos) && (lastDigitPos + 1 < (int)name.size())
                                && (lastDigitPos > delimiterPosition))
                                unitChar = name.substr(lastDigitPos + 1);

                            name.resize(delimiterPosition);
                        }

                        // In a FinanceChart, if there are two data sets, it must be representing a
                        // range.
                        if (dataSetCount == 2) {
                            // We show both values in the range in a single legend entry
                            value = layer->getDataSet(0)->getValue(xIndex);
                            double value2 = layer->getDataSet(1)->getValue(xIndex);
                            name = name + ": " + c->formatValue(min(value, value2), "{value|P3}");
                            name = name + " - " + c->formatValue(max(value, value2), "{value|P3}");
                        } else {
                            // In a FinanceChart, only the layer for volume bars has 3 data sets for
                            // up/down/flat days
                            if (dataSetCount == 3) {
                                // The actual volume is the sum of the 3 data sets.
                                value = layer->getDataSet(0)->getValue(xIndex) + layer->getDataSet(1
                                    )->getValue(xIndex) + layer->getDataSet(2)->getValue(xIndex);
                            }

                            // Create the legend entry
                            name = name + ": " + c->formatValue(value, "{value|P3}") + unitChar;
                        }

                        // Build the legend entry, consist of a colored square box and the name (with
                        // the data value in it).
                        ostringstream legendEntry;
                        legendEntry << "<*block*><*img=@square,width=8,edgeColor=000000,color="
                            << hex << dataSet->getDataColor() << "*> " << name << "<*/*>";
                        legendEntries.push_back(legendEntry.str());
                    }
                }
            }
        }

        // Get the plot area position relative to the entire FinanceChart
        PlotArea *plotArea = c->getPlotArea();
        int plotAreaLeftX = plotArea->getLeftX() + c->getAbsOffsetX();
        int plotAreaTopY = plotArea->getTopY() + c->getAbsOffsetY();

        // The legend begins with the date label, then the ohlcLegend (if any), and then the
        // entries for the indicators.
        ostringstream legendText;
        legendText << "<*block,valign=top,maxWidth=" << (plotArea->getWidth() - 5)
            << "*><*font=arialbd.ttf*>[" << c->xAxis()->getFormattedLabel(xValue, "mmm dd, yyyy")
            << "]<*/font*>" << ohlcLegend.str();
        for (int i = ((int)legendEntries.size()) - 1; i >= 0; --i) {
            legendText << "      " << legendEntries[i];
        }
        legendText << "<*/*>";

        // Draw a vertical track line at the x-position
        d->vline(plotAreaTopY, plotAreaTopY + plotArea->getHeight(), c->getXCoor(xValue) +
            c->getAbsOffsetX(), d->dashLineColor(GET_STYLE().edge_color, 0x0101));

        // Display the legend on the top of the plot area
        TTFText *t = d->text(legendText.str().c_str(), "arial.ttf", 8);
        t->draw(plotAreaLeftX + 5, plotAreaTopY + 3, GET_STYLE().font_color, Chart::TopLeft);
        t->destroy();
    }
}

void GraphResultWidget::zoomIn(){
    int zoomCount = m_lastResults.count();
    int zoomLeftCount = m_zoomXValue * m_zoomFactor;
    int zoomRightCount = (zoomCount - m_lastExtra - m_zoomXValue - 1) * m_zoomFactor;

    if(zoomLeftCount == 0 && zoomRightCount == 0){
        return;
    }

    m_zoomXValue -= zoomLeftCount;
    for(int i = 0;i < zoomLeftCount;i++){
        m_zoomLeftVec.push_back(m_lastResults.takeFirst());
    }
    m_zoomLeftCountVec.push_back(zoomLeftCount);

    for(int i = 0;i < zoomRightCount;i++){
        m_zoomRightVec.push_back(m_lastResults.takeLast());
    }
    m_zoomRightCountVec.push_back(zoomRightCount);

    onSearch(m_lastResults, m_lastMaList, m_lastExtra, m_lastBoll);
}

void GraphResultWidget::zoomOut(){
    int zoomLeftCount = m_zoomLeftCountVec.count() == 0 ? 0 : m_zoomLeftCountVec.takeLast();
    int zoomRightCount = m_zoomRightCountVec.count() == 0 ? 0 : m_zoomRightCountVec.takeLast();

    if(zoomLeftCount == 0 && zoomRightCount == 0){
        return;
    }

    m_zoomXValue += zoomLeftCount;

    for(int i = 0;i < zoomLeftCount;i++){
        m_lastResults.push_front(m_zoomLeftVec.takeLast());
    }

    for(int i = 0;i < zoomRightCount;i++){
        m_lastResults.push_back(m_zoomRightVec.takeLast());
    }

    onSearch(m_lastResults, m_lastMaList, m_lastExtra, m_lastBoll);
}

void GraphResultWidget::onMouseMovePlotArea(QMouseEvent *){
    trackFinance((MultiChart *)m_ChartViewer->getChart(), m_ChartViewer->getPlotAreaMouseX());
    m_ChartViewer->updateDisplay();
}

void GraphResultWidget::onChartClicked(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        m_tack = !m_tack;
    }
}
