#include "ratecolordialog.h"
#include <QDebug>

RateColorDialog::RateColorDialog(QWidget *parent)
    :QDialog(parent)
{
    this->initData();
    this->initUI();
    this->setData();
    this->initConnect();
}

RateColorDialog::~RateColorDialog(){
    delete m_ParamList;
}

void RateColorDialog::initData(){
    color_list.clear();
    QString queryStr("select * from color order by cast(type as int)");
    QSqlQuery query(queryStr);
    while (query.next()){
        QString type = query.value(0).toString();
        int ma14 = query.value(1).toInt();
        int ma60 = query.value(2).toInt();
        int ma99 = query.value(3).toInt();
        int ma888 = query.value(4).toInt();
        QStringList vals ;
        vals << type;
        vals << QString("").setNum(ma14);
        vals << QString("").setNum(ma60);
        vals << QString("").setNum(ma99);
        vals << QString("").setNum(ma888);

        color_list.append(vals);
    }
}

void RateColorDialog::initUI(){
    this->setFixedSize(520, 380);
    m_ParamList = new QTableWidget(this);
    m_ParamList->verticalHeader()->setDefaultSectionSize(50);
    m_ParamList->setColumnCount(5);
    m_ParamList->setHorizontalHeaderLabels(QStringList() << "类别" << "m14" << "ma60" << "ma99" << "ma888");

    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    verticalLayout->addWidget(m_ParamList);
}

void RateColorDialog::setData(){
    m_ParamList->setRowCount(0);
    m_ParamList->clearContents();
    int type_count = color_list.size();
    for(int i = 0;i < type_count;i++){
        m_ParamList->insertRow(i);
        QStringList colorList = color_list[i];

        QString type = colorList[0];
        QTableWidgetItem *typeItem = new QTableWidgetItem(type);
        typeItem->setFlags(Qt::ItemIsSelectable);
        m_ParamList->setItem(i, 0, typeItem);

        int color_count = colorList.size();
        for(int j = 1;j < color_count;j++){
            int color = colorList[j].toInt();
            QTableWidgetItem *colorItem = new QTableWidgetItem;
            colorItem->setBackgroundColor(QColor(color >> 16 & 0x000000ff, color >> 8 & 0x000000ff, color & 0x000000ff));
            m_ParamList->setItem(i, j, colorItem);
        }
    }
}

void RateColorDialog::initConnect(){
    connect(m_ParamList, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(onColorItemDoubleClicked(QTableWidgetItem *)));
}

void RateColorDialog::onColorItemDoubleClicked(QTableWidgetItem *item){
    int row = item->row();
    int column = item->column();
    QString type = color_list[row][0];
    int color = color_list[row][column].toInt();
    QColor newColor = QColorDialog::getColor(QColor(color >> 16 & 0x000000ff, color >> 8 & 0x000000ff, color & 0x000000ff));
    if(newColor.isValid()){
        QString queryStr;
        switch(column){
        case 1:
            queryStr = QString("update color set ma14 = ? where type = ?");
            break;
        case 2:
            queryStr = QString("update color set ma60 = ? where type = ?");
            break;
        case 3:
            queryStr = QString("update color set ma99 = ? where type = ?");
            break;
        case 4:
            queryStr = QString("update color set ma888 = ? where type = ?");
            break;
        }
        QSqlQuery query;
        query.prepare(queryStr);
        query.addBindValue((newColor.red() << 16 & 0x00ff0000) | (newColor.green() << 8 & 0x0000ff00) | (newColor.blue() & 0x000000ff));
        query.addBindValue(type);
        if(query.exec()){
            initData();
            setData();
        }
    }
}

int RateColorDialog::getColor(QString type, int column){
    int count = color_list.size();
    for(int i = 0;i < count;i++){
        QStringList colorList = color_list[i];
        if(type.compare(colorList[0]) == 0){
            return colorList[column + 1].toInt();
        }
    }
    //白色
    return 16777215;
}
