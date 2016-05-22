#include "detailwidget.h"

DetailWidget::DetailWidget(QWidget *parent)
    :QWidget(parent)
{
    this->initUI();
}

DetailWidget::~DetailWidget(){
    delete m_ParamList;
}

void DetailWidget::initUI(){
    this->setWindowTitle("涨停喵");

//    //设置背景色
//    this->setAutoFillBackground(true);
//    this->setPalette(QPalette(QColor(255, 255, 255)));

    m_ParamList = new QTableWidget(this);
    QHeaderView* headerView = m_ParamList->verticalHeader();
    headerView->setHidden(true);
    m_ParamList->verticalHeader()->setDefaultSectionSize(24);
    m_ParamList->horizontalHeader()->setStretchLastSection(true);
    m_ParamList->setColumnCount(3);
    m_ParamList->setColumnWidth(0, 60);
    m_ParamList->setColumnWidth(1, 70);
    m_ParamList->setHorizontalHeaderLabels(QStringList() << "类别" << "差值" << "值");
    m_ParamList->setSortingEnabled(true);

    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    verticalLayout->addWidget(m_ParamList);
}

void DetailWidget::setData(QStringList names, QStringList values, QList<int> colors, int times){
    m_ParamList->setRowCount(0);
    m_ParamList->clearContents();

    double standard_price = values[names.indexOf("实时价格")].toDouble();
    int count = names.size();
    for(int i = 0;i < count;i++){
        int color = colors[i];
        QColor bgcolor = QColor((color >> 16) & 0x000000FF, (color >> 8) & 0x000000FF, (color) & 0x000000FF);
        m_ParamList->insertRow(i);

        QTableWidgetItem *nameItem = new QTableWidgetItem(names[i]);
        nameItem->setBackgroundColor(bgcolor);
        nameItem->setForeground(QBrush(QColor(0,0,0)));
        m_ParamList->setItem(i, 0, nameItem);

        if(values[i].compare("N/A") == 0){
            QTableWidgetItem *diffValueItem = new QTableWidgetItem("N/A");
            diffValueItem->setBackgroundColor(bgcolor);
            diffValueItem->setForeground(QBrush(QColor(0,0,0)));
            m_ParamList->setItem(i, 1, diffValueItem);
        }else{
            QTableWidgetItem *diffValueItem = new QTableWidgetItem(QString("%1").arg((values[i].toDouble() - standard_price) * times));
            diffValueItem->setBackgroundColor(bgcolor);
            diffValueItem->setForeground(QBrush(QColor(0,0,0)));
            m_ParamList->setItem(i, 1, diffValueItem);
        }

        QTableWidgetItem *valueItem = new QTableWidgetItem(values[i]);
        valueItem->setBackgroundColor(bgcolor);
        valueItem->setForeground(QBrush(QColor(0,0,0)));
        m_ParamList->setItem(i, 2, valueItem);
    }
    m_ParamList->sortByColumn(2, Qt::DescendingOrder);

    //最后一行为时间
    m_ParamList->insertRow(count);
    m_ParamList->setSpan(count, 0, 1, 3);
    QTableWidgetItem *timeItem = new QTableWidgetItem(values[count]);
    m_ParamList->setItem(count, 0, timeItem);
}
