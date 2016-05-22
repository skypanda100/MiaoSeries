#include <QApplication>
#include "graphwidget.h"
#include <vector>
#include <sstream>
#include <QDebug>

using namespace std;

GraphWidget::GraphWidget(QSize size, QWidget *parent) :
    size(size), QWidget(parent), isValueHide(true)
{
    this->initUI();
    this->initConnect();
}

GraphWidget::~GraphWidget()
{
    delete m_ChartViewer->getChart();
    delete m_ViewPortControl->getChart();
    delete m_ChartViewer;
    delete m_ViewPortControl;
    int count = ea_results.size();
    for(int i = 0;i < count;i++){
        ExchangeRateResult *ea_result = ea_results[i];
        delete[] ea_result->getXData().data;
        delete[] ea_result->getYData().data;
        delete ea_result;
    }
}

void GraphWidget::initUI(){
//    //设置背景色
//    this->setAutoFillBackground(true);
//    this->setPalette(QPalette(Qt::transparent));

    m_ChartViewer = new QChartViewer(this);
    m_ChartViewer->updateViewPort(true, true);
    m_ViewPortControl = new QViewPortControl(this);
    m_ViewPortControl->setViewer(m_ChartViewer);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_ChartViewer);
    mainLayout->addWidget(m_ViewPortControl);

    this->setLayout(mainLayout);
}

void GraphWidget::initChartViewer()
{
    int ea_result_count = ea_results.size();
    if(ea_result_count > 0){
        DoubleArray x_data = getXData(0);
        m_ChartViewer->setFullRange("x", x_data[0], x_data[x_data.len - 1]);

        //576代表两天
        double width = 1.0;
        int dspCount = DSP_DAY*24*60/5;
        if(x_data.len > dspCount){
            width = dspCount*1.0 / x_data.len;
        }
        m_ChartViewer->setViewPortWidth(width);
        m_ChartViewer->setViewPortLeft(1 - m_ChartViewer->getViewPortWidth());

        m_ChartViewer->setZoomInWidthLimit(10.0 / x_data.len);
        m_ChartViewer->setMouseWheelZoomRatio(1.1);
    }
}

void GraphWidget::initConnect(){
    connect(m_ChartViewer, SIGNAL(mouseMovePlotArea(QMouseEvent*)), SLOT(onMouseMovePlotArea(QMouseEvent*)));
    connect(m_ChartViewer, SIGNAL(viewPortChanged()), SLOT(onViewPortChanged()));
    connect(m_ChartViewer, SIGNAL(clicked(QMouseEvent*)), SLOT(onChartClicked(QMouseEvent*)));
}

void GraphWidget::drawChart()
{
    XYChart *c = new XYChart(size.width(), size.height() - 100, Chart::Transparent);

    c->setPlotArea(10, 50, c->getWidth() - 65, c->getHeight() - 10, 0x000000, -1, 0xC8C8C8, Chart::Transparent, Chart::Transparent);

    c->setClipping();

    c->getLegend()->setLineStyleKey();
    c->getLegend()->setFontSize(10);

    //遍历查询结果并绘制结果
    int ea_result_count = ea_results.size();
    //设置X轴的label
    for(int i = 0;i < ea_result_count;i++){
        ExchangeRateResult *ea_result = ea_results[i];
        DoubleArray x_data = getXData(i);
        DoubleArray y_data = ea_result->getYData();

        double viewPortStartDate = m_ChartViewer->getValueAtViewPort("x", m_ChartViewer->getViewPortLeft());
        double viewPortEndDate = m_ChartViewer->getValueAtViewPort("x", m_ChartViewer->getViewPortLeft() +
            m_ChartViewer->getViewPortWidth());

        int startIndex = (int)floor(Chart::bSearch(x_data, viewPortStartDate));
        int endIndex = (int)ceil(Chart::bSearch(x_data, viewPortEndDate));
        int noOfPoints = endIndex - startIndex + 1;

        DoubleArray viewPortXData = DoubleArray(x_data.data + startIndex, noOfPoints);
        DoubleArray viewPortYData = DoubleArray(y_data.data + startIndex, noOfPoints);
        //添加图层
        LineLayer *layer = c->addLineLayer();
        layer->setLineWidth(1);
        //设置X轴
        layer->setXData(viewPortXData);
        //设置Y轴
        layer->addDataSet(viewPortYData, ea_result->getColor(), ea_result->getName().toUtf8().data())->setUseYAxis2();
    }
    c->xAxis()->setLabelStyle("arial.ttf", 10);
    c->yAxis2()->setLabelStyle("arial.ttf", 10);
    c->xAxis()->setColors(0xC8C8C8, 0xC8C8C8);
    c->yAxis2()->setColors(0xC8C8C8, 0xC8C8C8);

    m_ChartViewer->syncDateAxisWithViewPort("x", c->xAxis());

    c->yAxis()->setTickDensity(30);
    c->yAxis2()->setTickDensity(30);

    if (!m_ChartViewer->isInMouseMoveEvent())
    {
        trackLineLegend(c, (0 == m_ChartViewer->getChart()) ? c->getPlotArea()->getRightX() :
            m_ChartViewer->getPlotAreaMouseX());
    }
    delete m_ChartViewer->getChart();
    m_ChartViewer->setChart(c);
}

void GraphWidget::drawFullChart(){
    nameList.clear();

    initChartViewer();

    XYChart *c = new XYChart(size.width(), 100, Chart::Transparent);

    c->setPlotArea(10, 5, c->getWidth() - 65, c->getHeight() - 15, 0x000000, -1, 0xC8C8C8,
        Chart::Transparent, 0xffffff);

    c->xAxis()->setColors(Chart::Transparent);
    c->xAxis()->setLabelStyle("arial.ttf", 10);
    c->xAxis()->setLabelGap(-1);
    c->xAxis()->setLabelAlignment(1);

    c->yAxis()->setColors(Chart::Transparent, Chart::Transparent);

    //遍历查询结果并绘制结果
    int ea_result_count = ea_results.size();
    //设置X轴的label
    for(int i = 0;i < ea_result_count;i++){
        ExchangeRateResult *ea_result = ea_results[i];
        DoubleArray y_data = ea_result->getYData();

        //添加图层
        LineLayer *layer = c->addLineLayer();
        layer->setFastLineMode();
        layer->setLineWidth(1);
        //设置X轴
        layer->setXData(getXData(i));
        //设置Y轴
        layer->addDataSet(y_data, ea_result->getColor(), ea_result->getName().toUtf8().data());
    }

    c->xAxis()->setDateScale(m_ChartViewer->getValueAtViewPort("x", 0), m_ChartViewer->getValueAtViewPort("x", 1));

//    c->xAxis()->setTickDensity(75);

    c->yAxis()->setRounding(false, false);

    delete m_ViewPortControl->getChart();
    m_ViewPortControl->setChart(c);
    drawChart();
}

void GraphWidget::onMouseMovePlotArea(QMouseEvent *)
{
    trackLineLegend((XYChart *)m_ChartViewer->getChart(), m_ChartViewer->getPlotAreaMouseX());
    m_ChartViewer->updateDisplay();
}

void GraphWidget::onMouseUsageChanged(int mouseUsage)
{
    m_ChartViewer->setMouseUsage(mouseUsage);
}

void GraphWidget::onViewPortChanged()
{
    if (m_ChartViewer->needUpdateChart()){
        drawChart();
    }
}

void GraphWidget::onChartClicked(QMouseEvent *event){
    if(Qt::RightButton == event->button()){
        isValueHide = !isValueHide;
        onMouseMovePlotArea(event);
    }else{
        if(m_title.startsWith("USDJPY")){
            emit sendDetail(nameList, valueList, colorList, 100);
        }else if(m_title.startsWith("CNHJPY")){
            emit sendDetail(nameList, valueList, colorList, 100);
        }else{
            emit sendDetail(nameList, valueList, colorList, 10000);
        }
    }
}

void GraphWidget::setTitle(const QString &title)
{
    m_title = title;
}

void GraphWidget::trackLineLegend(XYChart *c, int mouseX)
{
    //清空值
    nameList.clear();
    valueList.clear();
    colorList.clear();

    DrawArea *d = c->initDynamicLayer();

    // The plot area object
    PlotArea *plotArea = c->getPlotArea();

    // Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
    double xValue = c->getNearestXValue(mouseX);
    int xCoor = c->getXCoor(xValue);

    // Draw a vertical track line at the x-position
    d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor, d->dashLineColor(0xffffff, 0x0101));

    // Container to hold the legend entries
    vector<string> legendEntries;

    // Iterate through all layers to build the legend array
    for (int i = 0; i < c->getLayerCount(); ++i) {
        Layer *layer = c->getLayerByZ(i);
        // The data array index of the x-value
        int xIndex = layer->getXIndexOf(xValue);

        // Iterate through all the data sets in the layer
        for (int j = 0; j < layer->getDataSetCount(); ++j) {
            DataSet *dataSet = layer->getDataSetByZ(j);

            // We are only interested in visible data sets with names
            const char *dataName = dataSet->getDataName();
            //添加类别信息
            nameList << QString::fromUtf8(dataName);
            //添加颜色信息
            colorList << dataSet->getDataColor();

            int color = dataSet->getDataColor();
            if (dataName && *dataName && (color != (int)Chart::Transparent)) {
                // Build the legend entry, consist of the legend icon, name and data value.
                double dataValue = dataSet->getValue(xIndex);
                //添加值信息
                if(dataValue == Chart::NoValue){
                    bool isFind = false;
                    for(int temp_i = xValue - 1;temp_i >= 0;temp_i--){
                        int index = layer->getXIndexOf(temp_i);
                        if(index != -1){
                            isFind = true;
                            double value = dataSet->getValue(index);
                            valueList << QString::fromUtf8(c->formatValue(value, "{value|P4}"));
                            break;
                        }
                    }
                    if(!isFind){
                        valueList << "N/A";
                    }
                }else{
                    valueList << QString::fromUtf8(c->formatValue(dataValue, "{value|P4}"));
                }
                ostringstream legendEntry;
                legendEntry << "<*block*><*font=msyhbd.ttf*>" << dataSet->getLegendIcon() << " " << dataName << ": " <<
                    ((dataValue == Chart::NoValue) ? "N/A" : c->formatValue(dataValue, "{value|P4}"))
                    << "<*/*>";
                legendEntries.push_back(legendEntry.str());

                // Draw a track dot for data points within the plot area
                int yCoor = c->getYCoor(dataSet->getPosition(xIndex), dataSet->getUseYAxis());
                if ((yCoor >= plotArea->getTopY()) && (yCoor <= plotArea->getBottomY())) {
                    d->circle(xCoor, yCoor, 4, 4, color, color);
                }
            }
        }
    }
    //添加值信息
    valueList << QString::fromUtf8(c->xAxis()->getFormattedLabel(getXValue((int)xValue), "yyyy-mm-dd hh:nn"));
    // Create the legend by joining the legend entries
    ostringstream legendText;
    legendText << "<*block,maxWidth=" << plotArea->getWidth() << "*><*block*><*font=arialbd.ttf*>["
        << c->xAxis()->getFormattedLabel(getXValue((int)xValue), "yyyy-mm-dd hh:nn") << "]<*/*>";
    for (int i = ((int)legendEntries.size()) - 1; i >= 0; --i)
        legendText << "        " << legendEntries[i];

    // Display the legend on the top of the plot area
    TTFText *t = d->text(legendText.str().c_str(), "arial.ttf", 8);
    t->draw(plotArea->getLeftX() + 5, plotArea->getTopY() - 3, 0xC8C8C8, Chart::BottomLeft);
    t->destroy();

    //y轴显示值
    if(!isValueHide){
        trackLineAxis(c, d, mouseX);
    }
}

void GraphWidget::trackLineAxis(XYChart *c, DrawArea *d, int mouseX)
{
    // Clear the current dynamic layer and get the DrawArea object to draw on it.
//    DrawArea *d = c->initDynamicLayer();

    // The plot area object
    PlotArea *plotArea = c->getPlotArea();

    // Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
    double xValue = c->getNearestXValue(mouseX);
    int xCoor = c->getXCoor(xValue);

    // The vertical track line is drawn up to the highest data point (the point with smallest
    // y-coordinate). We need to iterate all datasets in all layers to determine where it is.
    int minY = plotArea->getBottomY();

    // Iterate through all layers to find the highest data point
    for(int i = 0; i < c->getLayerCount(); ++i) {
        Layer *layer = c->getLayerByZ(i);

        // The data array index of the x-value
        int xIndex = layer->getXIndexOf(xValue);

        // Iterate through all the data sets in the layer
        for(int j = 0; j < layer->getDataSetCount(); ++j) {
            DataSet *dataSet = layer->getDataSetByZ(j);

            double dataPoint = dataSet->getPosition(xIndex);
            if ((dataPoint != Chart::NoValue) && (dataSet->getDataColor() != (int)Chart::Transparent))
                minY = min(minY, c->getYCoor(dataPoint, dataSet->getUseYAxis()));
        }
    }

    // Draw a vertical track line at the x-position up to the highest data point.
    d->vline(max(minY, plotArea->getTopY()), plotArea->getBottomY() + 6, xCoor, d->dashLineColor(
        0xffffff, 0x0101));

    // Draw a label on the x-axis to show the track line position
    ostringstream xlabel;
    xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "mmm dd, yyyy")
        << " <*/font*>";
    TTFText *t = d->text(xlabel.str().c_str(), "arialbd.ttf", 8);
    t->draw(xCoor, plotArea->getBottomY() + 6, 0xffffff, Chart::Top);
    t->destroy();

    // Iterate through all layers to build the legend array
    for(int i = 0; i < c->getLayerCount(); ++i) {
        Layer *layer = c->getLayerByZ(i);

        // The data array index of the x-value
        int xIndex = layer->getXIndexOf(xValue);

        // Iterate through all the data sets in the layer
        for(int j = 0; j < layer->getDataSetCount(); ++j) {
            DataSet *dataSet = layer->getDataSetByZ(j);

            // The positional value, axis binding, pixel coordinate and color of the data point.
            double dataPoint = dataSet->getPosition(xIndex);
            Axis *yAxis = dataSet->getUseYAxis();
            int yCoor = c->getYCoor(dataPoint, yAxis);
            int color = dataSet->getDataColor();

            // Draw the axis label only for visible data points of named data sets
            if ((dataPoint != Chart::NoValue) && (color != (int)Chart::Transparent) && (yCoor >=
                plotArea->getTopY()) && (yCoor <= plotArea->getBottomY())) {
                // The axis label consists of 3 parts - a track dot for the data point, an axis label,
                // and a line joining the track dot to the axis label.

                // Draw the line first. The end point of the line at the axis label side depends on
                // whether the label is at the left or right side of the axis (that is, on whether the
                // axis is on the left or right side of the plot area).
                int xPos = yAxis->getX() + ((yAxis->getAlignment() == Chart::Left) ? -4 : 4);
                d->hline(xCoor, xPos, yCoor, d->dashLineColor(color, 0x0101));

                // Draw the track dot
                d->circle(xCoor, yCoor, 4, 4, color, color);

                // Draw the axis label. If the axis is on the left side of the plot area, the labels
                // should right aligned to the axis, and vice versa.
                ostringstream axisLabel;
                axisLabel << "<*font,bgColor=" << hex << color << "*> "
                    << c->formatValue(dataPoint, "{value|P4}")	<< " <*/font*>";
                t = d->text(axisLabel.str().c_str(), "arialbd.ttf", 8);
                t->draw(xPos, yCoor, 0x000000,
                    ((yAxis->getAlignment() == Chart::Left) ? Chart::Right : Chart::Left));
                t->destroy();
            }
        }
    }
}

DoubleArray GraphWidget::getXData(int index){
    int count = ea_results.size();
    if(count == 0){
        return DoubleArray();
    }

    DoubleArray firstDoubleArray = ea_results[0]->getXData();
    double *firstIndexArray = new double[firstDoubleArray.len];
    for(int i = 0;i < firstDoubleArray.len;i++){
        firstIndexArray[i] = i;
    }
    if(index == 0){
        return DoubleArray(firstIndexArray, firstDoubleArray.len);
    }else{
        DoubleArray someDoubleArray = ea_results[index]->getXData();
        double *someIndexArray = new double[someDoubleArray.len];
        int someIndex = 0;
        int startIndex = 0;
        for(int i = 0;i < someDoubleArray.len;i++){
            for(int j = startIndex;j < firstDoubleArray.len;j++){
                if(someDoubleArray.data[i] == firstDoubleArray.data[j]){
                    someIndexArray[someIndex] = j;
                    someIndex++;
                    startIndex = j + 1;
                    break;
                }
            }
        }
        return DoubleArray(someIndexArray, someDoubleArray.len);
    }
}

double GraphWidget::getXValue(int index){
    int count = ea_results.size();
    if(count == 0){
        return 0;
    }
    DoubleArray firstDoubleArray = ea_results[0]->getXData();
    return firstDoubleArray.data[index];
}

void GraphWidget::onSearchResult(QList<ExchangeRateResult *> ea_results){
    this->ea_results = ea_results;
    drawFullChart();
}

