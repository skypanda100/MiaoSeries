#include "inputwidget.h"
#include <QDebug>

InputWidget::InputWidget(QWidget *parent)
    :QWidget(parent)
{
    this->initUI();
    this->initConnect();
}

InputWidget::~InputWidget(){
    delete m_styleComboBox;
    delete m_fDateEdit;
    delete m_tDateEdit;
    delete m_checkBox_01;
    delete m_checkBox_02;
    delete m_checkBox_03;
    delete m_checkBox_04;
    delete m_checkBox_05;
    delete m_checkBox_06;
    delete m_checkBox_07;
    delete m_checkBox_08;
    delete m_checkBox_09;
    delete m_checkBox_boll;
    delete m_goButton;
    delete m_db;
}

void InputWidget::initUI(){
    m_db = new ExchangeServiceDb;

    QFont labelFont;
    labelFont.setBold(true);

    QLabel *styleLabel = new QLabel;
    styleLabel->setFont(labelFont);
    styleLabel->setText("STYLE");

    m_styleComboBox = new QComboBox;
    m_styleComboBox->addItem("LIGHT");
    m_styleComboBox->addItem("DARK");
    m_styleComboBox->setCurrentIndex(1);

    QLabel *fromLabel = new QLabel;
    fromLabel->setFont(labelFont);
    fromLabel->setText("FROM");

    m_fDateEdit = new QDateEdit;
    m_fDateEdit->setFixedWidth(100);
    m_fDateEdit->setDate(QDateTime::currentDateTime().addDays(-90).date());
    m_fDateEdit->setCalendarPopup(true);

    QLabel *toLabel = new QLabel;
    toLabel->setFont(labelFont);
    toLabel->setText("TO");

    m_tDateEdit = new QDateEdit;
    m_tDateEdit->setFixedWidth(100);
    m_tDateEdit->setDate(QDateTime::currentDateTime().date());
    m_tDateEdit->setCalendarPopup(true);

    QLabel *maLabel = new QLabel;
    maLabel->setFont(labelFont);
    maLabel->setText("MA");

    m_checkBox_08 = new QCheckBox("5");
    m_checkBox_01 = new QCheckBox("14");
    m_checkBox_09 = new QCheckBox("20");
    m_checkBox_02 = new QCheckBox("30");
    m_checkBox_03 = new QCheckBox("60");
    m_checkBox_04 = new QCheckBox("99");
    m_checkBox_05 = new QCheckBox("100");
    m_checkBox_06 = new QCheckBox("250");
    m_checkBox_07 = new QCheckBox("888");

    QLabel *bollLabel = new QLabel;
    bollLabel->setFont(labelFont);
    bollLabel->setText("BOLL");

    m_checkBox_boll = new QCheckBox;

    m_goButton = new QPushButton;
    m_goButton->setText("GO");

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(2, 2, 2, 2);
    mainLayout->setSpacing(5);
    mainLayout->addWidget(styleLabel);
    mainLayout->addWidget(m_styleComboBox);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(fromLabel);
    mainLayout->addWidget(m_fDateEdit);
    mainLayout->addWidget(toLabel);
    mainLayout->addWidget(m_tDateEdit);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(maLabel);
    mainLayout->addWidget(m_checkBox_08);
    mainLayout->addWidget(m_checkBox_01);
    mainLayout->addWidget(m_checkBox_09);
    mainLayout->addWidget(m_checkBox_02);
    mainLayout->addWidget(m_checkBox_03);
    mainLayout->addWidget(m_checkBox_04);
    mainLayout->addWidget(m_checkBox_05);
    mainLayout->addWidget(m_checkBox_06);
    mainLayout->addWidget(m_checkBox_07);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(bollLabel);
    mainLayout->addWidget(m_checkBox_boll);

    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_goButton);
    mainLayout->addStretch(1);

    this->setLayout(mainLayout);
}

void InputWidget::initConnect(){
    connect(m_goButton, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
    connect(m_styleComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxChanged(int)));
}

bool InputWidget::validate(){
    QDate fDate = m_fDateEdit->date();
    QDate tDate = m_tDateEdit->date();
    if(fDate.daysTo(tDate) < 0){
        QMessageBox::critical(0, QObject::tr("错误提示"), "日期FROM大于日期TO!");
        return false;
    }
    return true;
}

void InputWidget::onButtonClicked(){
    if(validate()){
        QList<int> maLst;
        if(m_checkBox_08->isChecked()){
            maLst.append(5);
        }
        if(m_checkBox_01->isChecked()){
            maLst.append(14);
        }
        if(m_checkBox_09->isChecked()){
            maLst.append(20);
        }
        if(m_checkBox_02->isChecked()){
            maLst.append(30);
        }
        if(m_checkBox_03->isChecked()){
            maLst.append(60);
        }
        if(m_checkBox_04->isChecked()){
            maLst.append(99);
        }
        if(m_checkBox_05->isChecked()){
            maLst.append(100);
        }
        if(m_checkBox_06->isChecked()){
            maLst.append(250);
        }
        if(m_checkBox_07->isChecked()){
            maLst.append(888);
        }

        int extra = 0;
        if(maLst.count() > 0){
            extra = maLst[maLst.count() - 1];
        }

        bool isBoll = false;
        if(isBoll = m_checkBox_boll->isChecked()){
            if(extra < 20){
                extra = 20;
            }
        }

        if(extra == 0){
            extra = 30;
        }

        QDate fDate = m_fDateEdit->date().addDays(-1 * extra * 2);
        QDate tDate = m_tDateEdit->date();
        QString queryStr = QString("select * from gbpusd_k where date >= '%1' and date <= '%2' order by date asc")
                .arg(fDate.toString("yyyy-MM-dd 00:00:00"))
                .arg(tDate.toString("yyyy-MM-dd 00:00:00"));

        QList<ExchangeRateResult *> eaResults = m_db->query(queryStr);

        extra = 0;
        for(ExchangeRateResult *eaResult : eaResults){
            QDate date = QDateTime::fromString(eaResult->date(), "yyyy-MM-ddThh:mm:ss").date();
            if(m_fDateEdit->date().daysTo(date) >= 0){
                break;
            }else{
                extra++;
            }
        }
        emit search(eaResults, maLst, extra, isBoll);
    }
}

void InputWidget::onComboBoxChanged(int index){
    SET_STYLE(index);
    emit styleChanged();
}
