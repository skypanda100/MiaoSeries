#include "inputwidget.h"
#include <QDebug>

//#define ZDT_TEST

#ifdef ZDT_TEST
#include "FinanceChart.h"
#endif


InputWidget::InputWidget(QWidget *parent)
    :QWidget(parent)
    ,m_simulateStart(false)
    ,m_operateBuy(false)
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
    delete m_moneyEdit;
    delete m_goButton;
    delete m_printButton;
    delete m_simulateButton;
    delete m_operateButton;
    delete m_skipButton;
    delete m_db;
}

void InputWidget::initUI(){
    this->setFixedHeight(430);
    m_db = new Db;

    QFont labelFont;
    labelFont.setBold(false);

    m_styleComboBox = new QComboBox;
    m_styleComboBox->addItem("LIGHT");
    m_styleComboBox->addItem("DARK");
    m_styleComboBox->setCurrentIndex(1);

    QVBoxLayout *styleLayout = new QVBoxLayout;
    styleLayout->addWidget(m_styleComboBox);
    QGroupBox *styleGroupBox = new QGroupBox;
    styleGroupBox->setFont(labelFont);
    styleGroupBox->setTitle("STYLE");
    styleGroupBox->setLayout(styleLayout);

    m_fDateEdit = new QDateEdit;
    m_fDateEdit->setDate(QDateTime::currentDateTime().addDays(-90).date());
    m_fDateEdit->setCalendarPopup(true);

    m_tDateEdit = new QDateEdit;
    m_tDateEdit->setDate(QDateTime::currentDateTime().date());
    m_tDateEdit->setCalendarPopup(true);

    QVBoxLayout *dateLayout = new QVBoxLayout;
    dateLayout->addWidget(m_fDateEdit);
    dateLayout->addWidget(m_tDateEdit);
    QGroupBox *dateGroupBox = new QGroupBox;
    dateGroupBox->setFont(labelFont);
    dateGroupBox->setTitle("DATE");
    dateGroupBox->setLayout(dateLayout);

    m_checkBox_08 = new QCheckBox("5");
    m_checkBox_01 = new QCheckBox("14");
    m_checkBox_09 = new QCheckBox("20");
    m_checkBox_02 = new QCheckBox("30");
    m_checkBox_03 = new QCheckBox("60");
    m_checkBox_04 = new QCheckBox("99");
    m_checkBox_05 = new QCheckBox("100");
    m_checkBox_06 = new QCheckBox("250");
    m_checkBox_07 = new QCheckBox("888");
    m_checkBox_boll = new QCheckBox("BOLL");

    QRegExp regExp("^(\\d+(\\.\\d{1,2})?)$");
    QRegExpValidator *validator = new QRegExpValidator(regExp, this);
    m_moneyEdit = new QLineEdit;
    m_moneyEdit->setValidator(validator);

    QVBoxLayout *moneyLayout = new QVBoxLayout;
    moneyLayout->addWidget(m_moneyEdit);
    QGroupBox *moneyGroupBox = new QGroupBox;
    moneyGroupBox->setFont(labelFont);
    moneyGroupBox->setTitle("MONEY");
    moneyGroupBox->setLayout(moneyLayout);

    QGridLayout *maLayout = new QGridLayout;
    maLayout->addWidget(m_checkBox_08, 0, 0);
    maLayout->addWidget(m_checkBox_01, 0, 1);
    maLayout->addWidget(m_checkBox_09, 0, 2);
    maLayout->addWidget(m_checkBox_02, 1, 0);
    maLayout->addWidget(m_checkBox_03, 1, 1);
    maLayout->addWidget(m_checkBox_04, 1, 2);
    maLayout->addWidget(m_checkBox_05, 2, 0);
    maLayout->addWidget(m_checkBox_06, 2, 1);
    maLayout->addWidget(m_checkBox_07, 2, 2);
    maLayout->addWidget(m_checkBox_boll, 3, 0);
    QGroupBox *maGroupBox = new QGroupBox;
    maGroupBox->setFont(labelFont);
    maGroupBox->setTitle("MA && BOLL");
    maGroupBox->setLayout(maLayout);

    QSize iconSize(40, 40);

    QIcon goIcon(":/image/go.png");
    m_goButton = new QToolButton;
    m_goButton->setIcon(goIcon);
    m_goButton->setIconSize(iconSize);

    QIcon printIcon(":/image/print.png");
    m_printButton = new QToolButton;
    m_printButton->setIcon(printIcon);
    m_printButton->setIconSize(iconSize);

    QIcon simulateIcon(":/image/simulate_start.png");
    m_simulateButton = new QToolButton;
    m_simulateButton->setIcon(simulateIcon);
    m_simulateButton->setIconSize(iconSize);

    QIcon operateIcon(":/image/buy.png");
    m_operateButton = new QToolButton;
    m_operateButton->setIcon(operateIcon);
    m_operateButton->setIconSize(iconSize);
    m_operateButton->setDisabled(true);

    QIcon skipIcon(":/image/skip.png");
    m_skipButton = new QToolButton;
    m_skipButton->setIcon(skipIcon);
    m_skipButton->setIconSize(iconSize);
    m_skipButton->setDisabled(true);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setContentsMargins(2, 2, 2, 2);
    mainLayout->setSpacing(15);
    mainLayout->addWidget(styleGroupBox, 0, 0, 1, 5);
    mainLayout->addWidget(dateGroupBox, 1, 0, 1, 5);
    mainLayout->addWidget(maGroupBox, 2, 0, 1, 5);
    mainLayout->addWidget(moneyGroupBox, 3, 0, 1, 5);
    mainLayout->addWidget(m_goButton, 4, 0, 1, 1, Qt::AlignCenter);
    mainLayout->addWidget(m_printButton, 4, 1, 1, 1, Qt::AlignCenter);
    mainLayout->addWidget(m_simulateButton, 4, 2, 1, 1, Qt::AlignCenter);
    mainLayout->addWidget(m_operateButton, 4, 3, 1, 1, Qt::AlignCenter);
    mainLayout->addWidget(m_skipButton, 4, 4, 1, 1, Qt::AlignCenter);

    this->setLayout(mainLayout);
}

void InputWidget::initConnect(){
    connect(m_goButton, SIGNAL(clicked()), this, SLOT(onGoButtonClicked()));
    connect(m_printButton, SIGNAL(clicked()), this, SLOT(onPrintButtonClicked()));
    connect(m_simulateButton, SIGNAL(clicked()), this, SLOT(onSimulateButtonClicked()));
    connect(m_operateButton, SIGNAL(clicked()), this, SLOT(onOperateButtonClicked()));
    connect(m_skipButton, SIGNAL(clicked()), this, SLOT(onSkipButtonClicked()));
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

void InputWidget::onGoButtonClicked(){
#ifdef ZDT_TEST

    /*****************************************************/
    int noOfDays = 100;
    int extraDays = 20;
    RanTable rantable(9, 6, noOfDays + extraDays);

    // Set the 1st col to be the timeStamp, starting from Sep 4, 2011, with each row representing one
    // day, and counting week days only (jump over Sat and Sun)
    rantable.setDateCol(0, Chart::chartTime(2016, 3, 1), 86400, false);

    // Set the 2nd, 3rd, 4th and 5th columns to be high, low, open and close data. The open value
    // starts from 100, and the daily change is random from -5 to 5.
    rantable.setHLOCCols(1, 100, -5, 5);

    // Set the 6th column as the vol data from 5 to 25 million
    rantable.setCol(5, 50000000, 250000000);

    // Now we read the data from the table into arrays
    DoubleArray timeStamps = rantable.getCol(0);
    DoubleArray highData = rantable.getCol(1);
    DoubleArray lowData = rantable.getCol(2);
    DoubleArray openData = rantable.getCol(3);
    DoubleArray closeData = rantable.getCol(4);
    DoubleArray volData = rantable.getCol(5);
    QDateTime baseDateTime = QDateTime::fromString("20160301", "yyyyMMdd");
    QList<Result *> eaResults;
    for(int i = 0;i < (noOfDays + extraDays);i++){
        Result *eaResult = new Result;
        eaResult->setDate(baseDateTime.addDays(i).toString("yyyy-MM-ddT00:00:00"));
        eaResult->setHigh(highData[i]);
        eaResult->setLow(lowData[i]);
        eaResult->setOpen(openData[i]);
        eaResult->setClose(closeData[i]);
        eaResults.append(eaResult);
    }
    QList<int> maList;
    maList.append(20);
    emit search(eaResults, maList, extraDays, true);

    return;
    /*****************************************************/
#endif
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
        if((isBoll = m_checkBox_boll->isChecked())){
            if(extra < 20){
                extra = 20;
            }
        }

        if(extra < 20){
            extra = 20;
        }

        QDate fDate = m_fDateEdit->date().addDays(-1 * extra * 2);
        QDate tDate = m_tDateEdit->date();
        QString queryStr = QString("select * from gbpusd_k where date >= '%1' and date <= '%2' order by date asc")
                .arg(fDate.toString("yyyy-MM-dd 00:00:00"))
                .arg(tDate.toString("yyyy-MM-dd 00:00:00"));

        QList<Result *> eaResults = m_db->query(queryStr);

        extra = 0;
        for(Result *eaResult : eaResults){
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

void InputWidget::onPrintButtonClicked(){
    QPixmap currentScreenPixmap;
    if(QApplication::activeWindow()){
        currentScreenPixmap = QPixmap::grabWindow(QApplication::activeWindow()->winId()
                                                  , -2
                                                  , -26
                                                  , QApplication::activeWindow()->width()
                                                  , QApplication::activeWindow()->height() + 25);
    }
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = currentScreenPixmap.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(),
                            size.width(), size.height());
        painter.setWindow(currentScreenPixmap.rect());
        painter.drawPixmap(0, 0, currentScreenPixmap);
    }
}

void InputWidget::onSimulateButtonClicked(){
    if(!m_simulateStart && m_moneyEdit->text().isEmpty()){
        QMessageBox::critical(0, QObject::tr("错误提示"), "金额不能为空!");
        return;
    }
    m_simulateStart = !m_simulateStart;
    m_operateButton->setDisabled(!m_simulateStart);
    m_skipButton->setDisabled(!m_simulateStart);

    QSize iconSize(40, 40);
    QIcon simulateIcon(":/image/simulate_start.png");
    if(m_simulateStart){
        m_money = m_moneyEdit->text().toDouble(0);
        m_volume = 0;
        m_operateDate = QString("");
        simulateIcon = QIcon(":/image/simulate_stop.png");
    }else{
        QSize iconSize(40, 40);
        QIcon operateIcon(":/image/buy.png");
        m_operateBuy = false;
        m_operateButton->setIcon(operateIcon);
        m_operateButton->setIconSize(iconSize);
        emit operateEnd();
    }
    m_simulateButton->setIcon(simulateIcon);
    m_simulateButton->setIconSize(iconSize);
}

void InputWidget::onOperateButtonClicked(){
    m_operateBuy = !m_operateBuy;
    QSize iconSize(40, 40);
    QIcon operateIcon(":/image/buy.png");
    if(m_operateBuy){
        operateIcon = QIcon(":/image/sell.png");
    }
    m_operateButton->setIcon(operateIcon);
    m_operateButton->setIconSize(iconSize);

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
        if((isBoll = m_checkBox_boll->isChecked())){
            if(extra < 20){
                extra = 20;
            }
        }

        if(extra < 20){
            extra = 20;
        }

        QString queryDateStr = QString("select * from gbpusd_k where date > '%1' order by date asc limit 1")
                .arg(m_tDateEdit->date().toString("yyyy-MM-dd 00:00:00"));

        QList<Result *> eaDateResults = m_db->query(queryDateStr);
        if(eaDateResults.count() == 0){
            QMessageBox::critical(0, QObject::tr("错误提示"), "没有下一条数据了!");
            return;
        }else{
            m_fDateEdit->setDate(m_fDateEdit->date().addDays(1));
            m_tDateEdit->setDate(QDateTime::fromString(eaDateResults[0]->date(), "yyyy-MM-ddThh:mm:ss").date());
        }

        QDate fDate = m_fDateEdit->date().addDays(-1 * extra * 2);
        QDate tDate = m_tDateEdit->date();

        QString queryStr = QString("select * from gbpusd_k where date >= '%1' and date <= '%2' order by date asc")
                .arg(fDate.toString("yyyy-MM-dd 00:00:00"))
                .arg(tDate.toString("yyyy-MM-dd 00:00:00"));

        QList<Result *> eaResults = m_db->query(queryStr);

        extra = 0;
        for(Result *eaResult : eaResults){
            QDate date = QDateTime::fromString(eaResult->date(), "yyyy-MM-ddThh:mm:ss").date();
            if(m_fDateEdit->date().daysTo(date) >= 0){
                break;
            }else{
                extra++;
            }
        }
        emit search(eaResults, maLst, extra, isBoll);

        if(eaResults.count() > 0){
            Result *result = eaResults[eaResults.count() - 1];
            if(m_operateDate.compare(result->date()) == 0){
                m_operateBuy = !m_operateBuy;
                QSize iconSize(40, 40);
                QIcon operateIcon(":/image/buy.png");
                if(m_operateBuy){
                    operateIcon = QIcon(":/image/sell.png");
                }
                m_operateButton->setIcon(operateIcon);
                m_operateButton->setIconSize(iconSize);

                return;
            }else{
                m_operateDate = result->date();
            }
            if(m_operateBuy){
                m_volume = (float)m_money / (float)result->open();
                qDebug() << m_volume;
            }
            m_money = m_volume * result->open();
            Simulate simulate;
            simulate.setOperation(m_operateBuy ? "B" : "S");
            simulate.setDate(m_operateDate.split("T")[0]);
            simulate.setOpen(result->open());
            simulate.setVolume(m_volume);
            simulate.setMoney(m_money);
            emit operate(simulate);
        }
    }
}

void InputWidget::onSkipButtonClicked(){
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
        if((isBoll = m_checkBox_boll->isChecked())){
            if(extra < 20){
                extra = 20;
            }
        }

        if(extra < 20){
            extra = 20;
        }

        QString queryDateStr = QString("select * from gbpusd_k where date > '%1' order by date asc limit 1")
                .arg(m_tDateEdit->date().toString("yyyy-MM-dd 00:00:00"));

        QList<Result *> eaDateResults = m_db->query(queryDateStr);
        if(eaDateResults.count() == 0){
            QMessageBox::critical(0, QObject::tr("错误提示"), "没有下一条数据了!");
            return;
        }else{
            m_fDateEdit->setDate(m_fDateEdit->date().addDays(1));
            m_tDateEdit->setDate(QDateTime::fromString(eaDateResults[0]->date(), "yyyy-MM-ddThh:mm:ss").date());
        }

        QDate fDate = m_fDateEdit->date().addDays(-1 * extra * 2);
        QDate tDate = m_tDateEdit->date();
        QString queryStr = QString("select * from gbpusd_k where date >= '%1' and date <= '%2' order by date asc")
                .arg(fDate.toString("yyyy-MM-dd 00:00:00"))
                .arg(tDate.toString("yyyy-MM-dd 00:00:00"));

        QList<Result *> eaResults = m_db->query(queryStr);

        extra = 0;
        for(Result *eaResult : eaResults){
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
