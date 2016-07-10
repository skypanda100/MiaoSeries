#include "searchwidget.h"

SearchWidget::SearchWidget(QWidget *parent)
    : QWidget(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();

    onSearchClicked();
}

SearchWidget::~SearchWidget(){
    delete eaDb;
    delete typeLabel;
    delete changeButton;
    delete rateComboBox;
    delete fdateEdit;
    delete tdateEdit;
    delete colorButton;
    delete periodCheckBox1_1;
    delete periodCheckBox1_2;
    delete periodCheckBox1_3;
    delete periodCheckBox1_4;
    delete periodCheckBox2_1;
    delete periodCheckBox2_2;
    delete periodCheckBox2_3;
    delete periodCheckBox2_4;
    delete periodCheckBox3_1;
    delete periodCheckBox3_2;
    delete periodCheckBox3_3;
    delete periodCheckBox3_4;
    delete periodCheckBox4_1;
    delete periodCheckBox4_2;
    delete periodCheckBox4_3;
    delete periodCheckBox4_4;
    delete periodCheckBox5_1;
    delete periodCheckBox5_2;
    delete periodCheckBox5_3;
    delete periodCheckBox5_4;
    delete periodCheckBox6_1;
    delete periodCheckBox6_2;
    delete periodCheckBox6_3;
    delete periodCheckBox6_4;
    delete periodCheckBox7_1;
    delete periodCheckBox7_2;
    delete periodCheckBox7_3;
    delete periodCheckBox7_4;
    delete searchButton;
    delete rateColorDialog;
}

void SearchWidget::initData(){
    //初始化数据库
    eaDb = new ExchangeServiceDb;

    //数据库表
    e_tables << "GBPUSD"
             << "USDJPY"
             << "CNHJPY";

    s_tables << "S002724"
             << "S002001"
             << "S000839"
             << "S002253"
             << "S000889"
             << "S002431"
             << "S002230"
             << "S600036";
    //下拉列表
    rates << "GBPUSD(英镑/美元)"
          << "USDJPY(美元/日元)"
          << "CNHJPY(人民币/日元)";

    stocks << "002724(海洋王)"
           << "002001(新和成)"
           << "000839(中信国安)"
           << "002253(川大智胜)"
           << "000889(茂业通信)"
           << "002431(棕梠园林)"
           << "002230(科大讯飞)"
           << "600036(招商银行)";
}

void SearchWidget::initUI(){
    //设置字体
    QFont font;
    font.setFamily("微软雅黑");

//    //设置背景色
//    this->setAutoFillBackground(true);
//    this->setPalette(QPalette(QColor(224, 220, 216)));

    //代码币种
    typeLabel = new QLabel("货币");
    font.setPixelSize(12);
    font.setBold(true);
    typeLabel->setFont(font);

    //交换
    font.setPixelSize(11);
    font.setBold(false);
    changeButton = new QPushButton;
    changeButton->setIcon(QIcon(":/images/exchange.png"));
    changeButton->setFlat(true);

    QHBoxLayout *typeHLayout = new QHBoxLayout;
    typeHLayout->setContentsMargins(0, 0, 0, 0);
    typeHLayout->setSpacing(0);
    typeHLayout->addWidget(typeLabel);
    typeHLayout->addStretch();
    typeHLayout->addWidget(changeButton);

    rateComboBox = new QComboBox;
    font.setPixelSize(11);
    font.setBold(false);
    rateComboBox->setFont(font);
    rateComboBox->setContentsMargins(5, 5, 5, 5);
    rateComboBox->setFixedHeight(25);
    rateComboBox->insertItems(0, rates);

    //时间段
    QLabel *dateLabel = new QLabel("时间");
    font.setPixelSize(12);
    font.setBold(true);
    dateLabel->setFont(font);

    QDateTime currentDateTime = QDateTime::currentDateTime();

    font.setPixelSize(11);
    font.setBold(false);

    fdateEdit = new QLineEdit;
    fdateEdit->setFont(font);
    fdateEdit->setFixedHeight(25);
    fdateEdit->setText(currentDateTime.addDays(-9).toString("yyyy-MM-dd"));

    tdateEdit = new QLineEdit;
    tdateEdit->setFont(font);
    tdateEdit->setFixedHeight(25);
    tdateEdit->setText(currentDateTime.toString("yyyy-MM-dd"));

    QHBoxLayout *dateLayout = new QHBoxLayout;
    dateLayout->setContentsMargins(0, 0, 0, 0);
    dateLayout->setSpacing(1);
    dateLayout->addWidget(fdateEdit);
    dateLayout->addWidget(tdateEdit);
    dateLayout->addStretch();

    //周期
    QLabel *periodLabel = new QLabel("周期");
    font.setPixelSize(12);
    font.setBold(true);
    periodLabel->setFont(font);

    font.setPixelSize(11);
    font.setBold(false);

    //颜色
    colorButton = new QPushButton;
    colorButton->setFont(font);
    colorButton->setText("颜色设置");
    colorButton->setFlat(true);

    QHBoxLayout *periodHLayout = new QHBoxLayout;
    periodHLayout->setContentsMargins(0, 0, 0, 0);
    periodHLayout->setSpacing(0);
    periodHLayout->addWidget(periodLabel);
    periodHLayout->addStretch();
    periodHLayout->addWidget(colorButton);

    //5分钟
    QGroupBox *periodGroup1 = new QGroupBox("5分钟");
    periodGroup1->setCheckable(true);
    periodGroup1->setChecked(false);
    periodGroup1->setFont(font);
    periodGroup1->setContentsMargins(0, 10, 0, 0);
    QHBoxLayout *hlayout1 = new QHBoxLayout;
    periodCheckBox1_1 = new QCheckBox("14");
    periodCheckBox1_1->setFont(font);
    periodCheckBox1_2 = new QCheckBox("60");
    periodCheckBox1_2->setFont(font);
    periodCheckBox1_3 = new QCheckBox("99");
    periodCheckBox1_3->setFont(font);
    periodCheckBox1_4 = new QCheckBox("888");
    periodCheckBox1_4->setFont(font);
    hlayout1->addWidget(periodCheckBox1_1, 0, Qt::AlignVCenter);
    hlayout1->addWidget(periodCheckBox1_2, 0, Qt::AlignVCenter);
    hlayout1->addWidget(periodCheckBox1_3, 0, Qt::AlignVCenter);
    hlayout1->addWidget(periodCheckBox1_4, 0, Qt::AlignVCenter);
    periodGroup1->setLayout(hlayout1);
    connect(periodGroup1, SIGNAL(clicked(bool)), this, SLOT(checkGroup1(bool)));

    //15分钟
    QGroupBox *periodGroup2 = new QGroupBox("15分钟");
    periodGroup2->setCheckable(true);
    periodGroup2->setChecked(false);
    periodGroup2->setFont(font);
    periodGroup2->setContentsMargins(0, 10, 0, 0);
    QHBoxLayout *hlayout2 = new QHBoxLayout;
    periodCheckBox2_1 = new QCheckBox("14");
    periodCheckBox2_1->setFont(font);
    periodCheckBox2_2 = new QCheckBox("60");
    periodCheckBox2_2->setFont(font);
    periodCheckBox2_3 = new QCheckBox("99");
    periodCheckBox2_3->setFont(font);
    periodCheckBox2_4 = new QCheckBox("888");
    periodCheckBox2_4->setFont(font);
    hlayout2->addWidget(periodCheckBox2_1, 0, Qt::AlignVCenter);
    hlayout2->addWidget(periodCheckBox2_2, 0, Qt::AlignVCenter);
    hlayout2->addWidget(periodCheckBox2_3, 0, Qt::AlignVCenter);
    hlayout2->addWidget(periodCheckBox2_4, 0, Qt::AlignVCenter);
    periodGroup2->setLayout(hlayout2);
    connect(periodGroup2, SIGNAL(clicked(bool)), this, SLOT(checkGroup2(bool)));

    //30分钟
    QGroupBox *periodGroup3 = new QGroupBox("30分钟");
    periodGroup3->setCheckable(true);
    periodGroup3->setChecked(false);
    periodGroup3->setFont(font);
    periodGroup3->setContentsMargins(0, 10, 0, 0);
    QHBoxLayout *hlayout3 = new QHBoxLayout;
    periodCheckBox3_1 = new QCheckBox("14");
    periodCheckBox3_1->setFont(font);
    periodCheckBox3_2 = new QCheckBox("60");
    periodCheckBox3_2->setFont(font);
    periodCheckBox3_3 = new QCheckBox("99");
    periodCheckBox3_3->setFont(font);
    periodCheckBox3_4 = new QCheckBox("888");
    periodCheckBox3_4->setFont(font);
    hlayout3->addWidget(periodCheckBox3_1, 0, Qt::AlignVCenter);
    hlayout3->addWidget(periodCheckBox3_2, 0, Qt::AlignVCenter);
    hlayout3->addWidget(periodCheckBox3_3, 0, Qt::AlignVCenter);
    hlayout3->addWidget(periodCheckBox3_4, 0, Qt::AlignVCenter);
    periodGroup3->setLayout(hlayout3);
    connect(periodGroup3, SIGNAL(clicked(bool)), this, SLOT(checkGroup3(bool)));

    //1小时
    QGroupBox *periodGroup4 = new QGroupBox("60分钟");
    periodGroup4->setCheckable(true);
    periodGroup4->setChecked(false);
    periodGroup4->setFont(font);
    periodGroup4->setContentsMargins(0, 10, 0, 0);
    QHBoxLayout *hlayout4 = new QHBoxLayout;
    periodCheckBox4_1 = new QCheckBox("14");
    periodCheckBox4_1->setFont(font);
    periodCheckBox4_2 = new QCheckBox("60");
    periodCheckBox4_2->setFont(font);
    periodCheckBox4_3 = new QCheckBox("99");
    periodCheckBox4_3->setFont(font);
    periodCheckBox4_4 = new QCheckBox("888");
    periodCheckBox4_4->setFont(font);
    hlayout4->addWidget(periodCheckBox4_1, 0, Qt::AlignVCenter);
    hlayout4->addWidget(periodCheckBox4_2, 0, Qt::AlignVCenter);
    hlayout4->addWidget(periodCheckBox4_3, 0, Qt::AlignVCenter);
    hlayout4->addWidget(periodCheckBox4_4, 0, Qt::AlignVCenter);
    periodGroup4->setLayout(hlayout4);
    connect(periodGroup4, SIGNAL(clicked(bool)), this, SLOT(checkGroup4(bool)));

    //4小时
    QGroupBox *periodGroup5 = new QGroupBox("4小时");
    periodGroup5->setCheckable(true);
    periodGroup5->setChecked(false);
    periodGroup5->setFont(font);
    periodGroup5->setContentsMargins(0, 10, 0, 0);
    QHBoxLayout *hlayout5 = new QHBoxLayout;
    periodCheckBox5_1 = new QCheckBox("14");
    periodCheckBox5_1->setFont(font);
    periodCheckBox5_2 = new QCheckBox("60");
    periodCheckBox5_2->setFont(font);
    periodCheckBox5_3 = new QCheckBox("99");
    periodCheckBox5_3->setFont(font);
    periodCheckBox5_4 = new QCheckBox("888");
    periodCheckBox5_4->setFont(font);
    hlayout5->addWidget(periodCheckBox5_1, 0, Qt::AlignVCenter);
    hlayout5->addWidget(periodCheckBox5_2, 0, Qt::AlignVCenter);
    hlayout5->addWidget(periodCheckBox5_3, 0, Qt::AlignVCenter);
    hlayout5->addWidget(periodCheckBox5_4, 0, Qt::AlignVCenter);
    periodGroup5->setLayout(hlayout5);
    connect(periodGroup5, SIGNAL(clicked(bool)), this, SLOT(checkGroup5(bool)));

    //日
    QGroupBox *periodGroup6 = new QGroupBox("日");
    periodGroup6->setCheckable(true);
    periodGroup6->setChecked(false);
    periodGroup6->setFont(font);
    periodGroup6->setContentsMargins(0, 10, 0, 0);
    QHBoxLayout *hlayout6 = new QHBoxLayout;
    periodCheckBox6_1 = new QCheckBox("14");
    periodCheckBox6_1->setFont(font);
    periodCheckBox6_2 = new QCheckBox("60");
    periodCheckBox6_2->setFont(font);
    periodCheckBox6_3 = new QCheckBox("99");
    periodCheckBox6_3->setFont(font);
    periodCheckBox6_4 = new QCheckBox("888");
    periodCheckBox6_4->setFont(font);
    hlayout6->addWidget(periodCheckBox6_1, 0, Qt::AlignVCenter);
    hlayout6->addWidget(periodCheckBox6_2, 0, Qt::AlignVCenter);
    hlayout6->addWidget(periodCheckBox6_3, 0, Qt::AlignVCenter);
    hlayout6->addWidget(periodCheckBox6_4, 0, Qt::AlignVCenter);
    periodGroup6->setLayout(hlayout6);
    connect(periodGroup6, SIGNAL(clicked(bool)), this, SLOT(checkGroup6(bool)));

    //周
    QGroupBox *periodGroup7 = new QGroupBox("周");
    periodGroup7->setCheckable(true);
    periodGroup7->setChecked(false);
    periodGroup7->setFont(font);
    periodGroup7->setContentsMargins(0, 10, 0, 0);
    QHBoxLayout *hlayout7 = new QHBoxLayout;
    periodCheckBox7_1 = new QCheckBox("14");
    periodCheckBox7_1->setFont(font);
    periodCheckBox7_2 = new QCheckBox("60");
    periodCheckBox7_2->setFont(font);
    periodCheckBox7_3 = new QCheckBox("99");
    periodCheckBox7_3->setFont(font);
    periodCheckBox7_4 = new QCheckBox("888");
    periodCheckBox7_4->setFont(font);
    hlayout7->addWidget(periodCheckBox7_1, 0, Qt::AlignVCenter);
    hlayout7->addWidget(periodCheckBox7_2, 0, Qt::AlignVCenter);
    hlayout7->addWidget(periodCheckBox7_3, 0, Qt::AlignVCenter);
    hlayout7->addWidget(periodCheckBox7_4, 0, Qt::AlignVCenter);
    periodGroup7->setLayout(hlayout7);
    connect(periodGroup7, SIGNAL(clicked(bool)), this, SLOT(checkGroup7(bool)));

    QVBoxLayout *periodVLayout = new QVBoxLayout;
    periodVLayout->setContentsMargins(0, 0, 0, 0);
    periodVLayout->setSpacing(3);
    periodVLayout->addWidget(periodGroup1);
    periodVLayout->addWidget(periodGroup2);
    periodVLayout->addWidget(periodGroup3);
    periodVLayout->addWidget(periodGroup4);
    periodVLayout->addWidget(periodGroup5);
    periodVLayout->addWidget(periodGroup6);
    periodVLayout->addWidget(periodGroup7);

    //查询按钮
    searchButton = new QPushButton;
    font.setPixelSize(11);
    font.setBold(false);
    searchButton->setText("查询");
    searchButton->setFont(font);
    searchButton->setFixedHeight(25);
    //布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(2);
    mainLayout->addLayout(typeHLayout);
    mainLayout->addWidget(rateComboBox);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(dateLabel, 0, Qt::AlignLeft);
    mainLayout->addLayout(dateLayout);
    mainLayout->addSpacing(5);
    mainLayout->addLayout(periodHLayout);
    mainLayout->addLayout(periodVLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(searchButton);

    //设置布局
    this->setLayout(mainLayout);

    //初始化颜色设置dialog
    rateColorDialog = new RateColorDialog;

    //初始化输入项
    rateComboBox->setCurrentIndex(1);
    periodGroup4->setChecked(true);
    checkGroup4(true);
    periodGroup5->setChecked(true);
    checkGroup5(true);
    periodGroup6->setChecked(true);
    checkGroup6(true);
    periodGroup7->setChecked(true);
    checkGroup7(true);
}

void SearchWidget::initConnect(){
    connect(searchButton, SIGNAL(clicked()), this, SLOT(onSearchClicked()));
    connect(colorButton, SIGNAL(clicked()), this, SLOT(showColorDialog()));
    connect(changeButton, SIGNAL(clicked()), this, SLOT(changeType()));
}

void SearchWidget::checkGroup1(bool checked){
    periodCheckBox1_1->setChecked(checked);
    periodCheckBox1_2->setChecked(checked);
    periodCheckBox1_3->setChecked(checked);
    periodCheckBox1_4->setChecked(checked);
}

void SearchWidget::checkGroup2(bool checked){
    periodCheckBox2_1->setChecked(checked);
    periodCheckBox2_2->setChecked(checked);
    periodCheckBox2_3->setChecked(checked);
    periodCheckBox2_4->setChecked(checked);
}

void SearchWidget::checkGroup3(bool checked){
    periodCheckBox3_1->setChecked(checked);
    periodCheckBox3_2->setChecked(checked);
    periodCheckBox3_3->setChecked(checked);
    periodCheckBox3_4->setChecked(checked);
}

void SearchWidget::checkGroup4(bool checked){
    periodCheckBox4_1->setChecked(checked);
    periodCheckBox4_2->setChecked(checked);
    periodCheckBox4_3->setChecked(checked);
    periodCheckBox4_4->setChecked(checked);
}

void SearchWidget::checkGroup5(bool checked){
    periodCheckBox5_1->setChecked(checked);
    periodCheckBox5_2->setChecked(checked);
    periodCheckBox5_3->setChecked(checked);
    periodCheckBox5_4->setChecked(checked);
}

void SearchWidget::checkGroup6(bool checked){
    periodCheckBox6_1->setChecked(checked);
    periodCheckBox6_2->setChecked(checked);
    periodCheckBox6_3->setChecked(checked);
    periodCheckBox6_4->setChecked(checked);
}

void SearchWidget::checkGroup7(bool checked){
    periodCheckBox7_1->setChecked(checked);
    periodCheckBox7_2->setChecked(checked);
    periodCheckBox7_3->setChecked(checked);
    periodCheckBox7_4->setChecked(checked);
}

void SearchWidget::changeType(){
    if(typeLabel->text().compare("货币") == 0){
        typeLabel->setText("股票");
        rateComboBox->clear();
        rateComboBox->insertItems(0, stocks);
    }else{
        typeLabel->setText("货币");
        rateComboBox->clear();
        rateComboBox->insertItems(0, rates);
    }
}

void SearchWidget::onSearchClicked(){
    //组织查询语句
    QString tableStr = typeLabel->text().compare("货币") == 0?e_tables[rateComboBox->currentIndex()]:s_tables[rateComboBox->currentIndex()];
    QString fdateStr = fdateEdit->text();
    QString tdateStr = tdateEdit->text();

    QString queryStr_1 = QString("select ma1,timet from %1 where timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
            .arg(tableStr)
            .arg(fdateStr)
            .arg(tdateStr);
    //查询实时表
    QList<ExchangeRateResult *> ea_results_1 = eaDb->query(queryStr_1, "实时价格", 0xff0000);

    //查询周期表
    if(periodCheckBox1_1->isEnabled() && periodCheckBox1_1->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma14,timet from %1_5 where ma14 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "5m_ma14", rateColorDialog->getColor("5", 0));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox1_2->isEnabled() && periodCheckBox1_2->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma60,timet from %1_5 where ma60 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "5m_ma60", rateColorDialog->getColor("5", 1));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox1_3->isEnabled() && periodCheckBox1_3->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma99,timet from %1_5 where ma99 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "5m_ma99", rateColorDialog->getColor("5", 2));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox1_4->isEnabled() && periodCheckBox1_4->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma888,timet from %1_5 where ma888 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "5m_ma888", rateColorDialog->getColor("5", 3));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }

    if(periodCheckBox2_1->isEnabled() && periodCheckBox2_1->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma14,timet from %1_15 where ma14 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "15m_ma14", rateColorDialog->getColor("15", 0));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox2_2->isEnabled() && periodCheckBox2_2->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma60,timet from %1_15 where ma60 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "15m_ma60", rateColorDialog->getColor("15", 1));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox2_3->isEnabled() && periodCheckBox2_3->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma99,timet from %1_15 where ma99 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "15m_ma99", rateColorDialog->getColor("15", 2));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox2_4->isEnabled() && periodCheckBox2_4->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma888,timet from %1_15 where ma888 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "15m_ma888", rateColorDialog->getColor("15", 3));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }

    if(periodCheckBox3_1->isEnabled() && periodCheckBox3_1->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma14,timet from %1_30 where ma14 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "30m_ma14", rateColorDialog->getColor("30", 0));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox3_2->isEnabled() && periodCheckBox3_2->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma60,timet from %1_30 where ma60 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "30m_ma60", rateColorDialog->getColor("30", 1));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox3_3->isEnabled() && periodCheckBox3_3->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma99,timet from %1_30 where ma99 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "30m_ma99", rateColorDialog->getColor("30", 2));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox3_4->isEnabled() && periodCheckBox3_4->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma888,timet from %1_30 where ma888 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "30m_ma888", rateColorDialog->getColor("30", 3));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }

    if(periodCheckBox4_1->isEnabled() && periodCheckBox4_1->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma14,timet from %1_60 where ma14 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "1h_ma14", rateColorDialog->getColor("60", 0));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox4_2->isEnabled() && periodCheckBox4_2->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma60,timet from %1_60 where ma60 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "1h_ma60", rateColorDialog->getColor("60", 1));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox4_3->isEnabled() && periodCheckBox4_3->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma99,timet from %1_60 where ma99 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "1h_ma99", rateColorDialog->getColor("60", 2));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox4_4->isEnabled() && periodCheckBox4_4->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma888,timet from %1_60 where ma888 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "1h_ma888", rateColorDialog->getColor("60", 3));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox5_1->isEnabled() && periodCheckBox5_1->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma14,timet from %1_240 where ma14 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "4h_ma14", rateColorDialog->getColor("240", 0));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox5_2->isEnabled() && periodCheckBox5_2->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma60,timet from %1_240 where ma60 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "4h_ma60", rateColorDialog->getColor("240", 1));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox5_3->isEnabled() && periodCheckBox5_3->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma99,timet from %1_240 where ma99 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "4h_ma99", rateColorDialog->getColor("240", 2));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox5_4->isEnabled() && periodCheckBox5_4->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma888,timet from %1_240 where ma888 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "4h_ma888", rateColorDialog->getColor("240", 3));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }

    if(periodCheckBox6_1->isEnabled() && periodCheckBox6_1->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma14,timet from %1_1440 where ma14 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "1d_ma14", rateColorDialog->getColor("1440", 0));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox6_2->isEnabled() && periodCheckBox6_2->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma60,timet from %1_1440 where ma60 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "1d_ma60", rateColorDialog->getColor("1440", 1));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox6_3->isEnabled() && periodCheckBox6_3->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma99,timet from %1_1440 where ma99 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "1d_ma99", rateColorDialog->getColor("1440", 2));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox6_4->isEnabled() && periodCheckBox6_4->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma888,timet from %1_1440 where ma888 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "1d_ma888", rateColorDialog->getColor("1440", 3));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }

    if(periodCheckBox7_1->isEnabled() && periodCheckBox7_1->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma14,timet from %1_10080 where ma14 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "1w_ma14", rateColorDialog->getColor("10080", 0));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox7_2->isEnabled() && periodCheckBox7_2->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma60,timet from %1_10080 where ma60 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "1w_ma60", rateColorDialog->getColor("10080", 1));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox7_3->isEnabled() && periodCheckBox7_3->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma99,timet from %1_10080 where ma99 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "1w_ma99", rateColorDialog->getColor("10080", 2));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }
    if(periodCheckBox7_4->isEnabled() && periodCheckBox7_4->checkState() == Qt::Checked){
        QString queryStr_2 = QString("select ma888,timet from %1_10080 where ma888 is not null and timet >= '%2 00:00:00' and timet <= '%3 23:59:59' order by timet asc")
                .arg(tableStr)
                .arg(fdateStr)
                .arg(tdateStr);
        QList<ExchangeRateResult *> ea_results_2 = eaDb->query(queryStr_2, "1w_ma888", rateColorDialog->getColor("10080", 3));
        if(ea_results_2.size() > 0)
        ea_results_1.append(ea_results_2);
    }

    //将查询结果发送出去
    if(ea_results_1.size() > 0)
        emit searchResult(rateComboBox->currentText() ,ea_results_1);
}

void SearchWidget::showColorDialog(){
//    QColor newColor = QColorDialog::getColor(Qt::white);
    rateColorDialog->exec();
}
