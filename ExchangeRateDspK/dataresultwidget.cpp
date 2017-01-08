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

    QTableWidgetItem *volumeItem = new QTableWidgetItem(QString::number(simulate.volume()));
    m_dataWidget->setItem(lastRowCount, 3, volumeItem);

    QTableWidgetItem *moneyItem = new QTableWidgetItem(QString::number(simulate.money()));
    m_dataWidget->setItem(lastRowCount, 4, moneyItem);
}

void DataResultWidget::onOperateEnd(){
    int rowCount = m_dataWidget->rowCount();
    if(rowCount == 0){
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, tr("数据导出"), qApp->applicationDirPath(),
                                                        tr("Excel Files (*.csv)"));
    if(filePath.isEmpty()){
        return;
    }
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::critical(0, QObject::tr("错误提示"), "无法创建csv文件!");
        return;

    }
    QTextStream out(&file);

    for(QString title : m_titleList){
        out << title << ",";
    }
    out << "\n";
    for(int i = 0;i < rowCount;i++){
        QString operate = m_dataWidget->item(i, 0)->text();
        QString date = m_dataWidget->item(i, 1)->text();
        QString open = m_dataWidget->item(i, 2)->text();
        QString volume = m_dataWidget->item(i, 3)->text();
        QString money = m_dataWidget->item(i, 4)->text();
        out << operate << "," << date << "," << open << "," << volume << "," << money << "\n";
    }
    file.close();
    QMessageBox::information(0, QObject::tr("信息提示"), "数据导出成功!");

    m_dataWidget->setRowCount(0);
    m_dataWidget->clearContents();
}

void DataResultWidget::initUI(){
    this->setFixedWidth(320);
    m_dataWidget = new QTableWidget(this);
    m_titleList << "B/S" << "Date" << "Open" << "Volume" << "￥";

    QHeaderView* headerView = m_dataWidget->verticalHeader();
    headerView->setHidden(true);
    m_dataWidget->verticalHeader()->setDefaultSectionSize(24);
    m_dataWidget->horizontalHeader()->setStretchLastSection(true);
    m_dataWidget->setColumnCount(m_titleList.count());
    m_dataWidget->setColumnWidth(0, 35);
    m_dataWidget->setColumnWidth(1, 75);
    m_dataWidget->setColumnWidth(2, 60);
    m_dataWidget->setColumnWidth(3, 60);
    m_dataWidget->setColumnWidth(4, 60);

    m_dataWidget->setHorizontalHeaderLabels(m_titleList);
    m_dataWidget->setSortingEnabled(false);

    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->setContentsMargins(2, 2, 2, 2);
    verticalLayout->addWidget(m_dataWidget);
}

void DataResultWidget::initConnect(){

}
