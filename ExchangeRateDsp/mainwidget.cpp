#include "mainwidget.h"
#include "ea_cons.h"
#include "ea_cons.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();

    searchWidget->onSearchClicked();
}

MainWidget::~MainWidget(){
    delete searchWidget;
    delete detailWidget;
    delete resultMainWidget;
}

void MainWidget::initData(){

}

void MainWidget::initUI(){
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect clientRect = desktopWidget->availableGeometry();
    int MAINWIDTH = clientRect.width() - 15;
    int MAINHEIGHT = clientRect.height() - 35;
    //设置长宽
    this->setFixedSize(MAINWIDTH, MAINHEIGHT);

    //左侧查询条件控制区
    QSplitter *leftSplitter = new QSplitter(Qt::Vertical, 0);
    leftSplitter->setHandleWidth(0);
    leftSplitter->setFixedWidth(250);
    searchWidget = new SearchWidget(leftSplitter);

    //左侧详细信息
    detailWidget = new DetailWidget(leftSplitter);
    leftSplitter->setStretchFactor(1, 1);

    //右侧查询结果
    resultMainWidget = new ResultMainWidget(QSize(MAINWIDTH - leftSplitter->width(), MAINHEIGHT));
    resultMainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(1, 1, 1, 1);
    mainLayout->setSpacing(3);

    mainLayout->addWidget(leftSplitter);
    mainLayout->addWidget(resultMainWidget, 1);

    //设置布局
    this->setLayout(mainLayout);
}


void MainWidget::initConnect(){
    connect(searchWidget, SIGNAL(searchResult(QString, QList<ExchangeRateResult *>)), resultMainWidget, SIGNAL(searchResult(QString, QList<ExchangeRateResult *>)));
    connect(resultMainWidget, SIGNAL(sendDetail(QStringList,QStringList,QList<int>,int)), detailWidget, SLOT(setData(QStringList,QStringList,QList<int>,int)));
}
