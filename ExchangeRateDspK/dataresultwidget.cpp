#include "dataresultwidget.h"

DataResultWidget::DataResultWidget(QWidget *parent)
    :QWidget(parent)
{
    this->initUI();
    this->initConnect();
}

DataResultWidget::~DataResultWidget(){
    delete m_dataWidget;
}

void DataResultWidget::onOperate(Simulate simulate){
    int lastRowCount = m_dataWidget->rowCount();
    m_dataWidget->insertRow(lastRowCount);

    QTableWidgetItem *operateItem = new QTableWidgetItem(simulate.operation());
    m_dataWidget->setItem(lastRowCount, 0, operateItem);

    QTableWidgetItem *dateItem = new QTableWidgetItem(simulate.date());
    m_dataWidget->setItem(lastRowCount, 1, dateItem);

    QTableWidgetItem *openItem = new QTableWidgetItem(QString::number(simulate.open()));
    m_dataWidget->setItem(lastRowCount, 2, openItem);

    qDebug() << "table" << simulate.volume();
    QTableWidgetItem *volumeItem = new QTableWidgetItem(QString::number(simulate.volume()));
    m_dataWidget->setItem(lastRowCount, 3, volumeItem);

    QTableWidgetItem *moneyItem = new QTableWidgetItem(QString::number(simulate.money()));
    m_dataWidget->setItem(lastRowCount, 4, moneyItem);
}

void DataResultWidget::onOperateEnd(){
    m_dataWidget->setRowCount(0);
    m_dataWidget->clearContents();
}

void DataResultWidget::initUI(){
    this->setFixedWidth(320);
    m_dataWidget = new QTableWidget(this);
    QStringList titleList;
    titleList << "B/S" << "Date" << "Open" << "Volume" << "￥";

    QHeaderView* headerView = m_dataWidget->verticalHeader();
    headerView->setHidden(true);
    m_dataWidget->verticalHeader()->setDefaultSectionSize(24);
    m_dataWidget->horizontalHeader()->setStretchLastSection(true);
    m_dataWidget->setColumnCount(titleList.count());
    m_dataWidget->setColumnWidth(0, 35);
    m_dataWidget->setColumnWidth(1, 75);
    m_dataWidget->setColumnWidth(2, 60);
    m_dataWidget->setColumnWidth(3, 60);
    m_dataWidget->setColumnWidth(4, 60);

    m_dataWidget->setHorizontalHeaderLabels(titleList);
    m_dataWidget->setSortingEnabled(false);

    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->setContentsMargins(2, 2, 2, 2);
    verticalLayout->addWidget(m_dataWidget);
}

void DataResultWidget::initConnect(){

}
