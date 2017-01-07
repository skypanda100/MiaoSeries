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

void DataResultWidget::initUI(){
    this->setFixedWidth(300);
    m_dataWidget = new QTableWidget(this);
    QStringList titleList;
    titleList << "B/S" << "Date" << "Open" << "Volume" << "￥";

    QHeaderView* headerView = m_dataWidget->verticalHeader();
    headerView->setHidden(true);
    m_dataWidget->verticalHeader()->setDefaultSectionSize(24);
    m_dataWidget->horizontalHeader()->setStretchLastSection(true);
    m_dataWidget->setColumnCount(titleList.count());
    m_dataWidget->setColumnWidth(0, 35);
    m_dataWidget->setColumnWidth(1, 60);
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
