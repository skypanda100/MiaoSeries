#include "mainwindow.h"
#include <QVBoxLayout>
#include <QRegExp>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    titles[0] = "日期";
    titles[1] = "开盘";
    titles[2] = "收盘";
    titles[3] = "涨跌额";
    titles[4] = "涨跌幅";
    titles[5] = "最低";
    titles[6] = "最高";
    titles[7] = "成交量(手)";
    titles[8] = "成交金额(万)";
    titles[9] = "换手率";

    this->initUI();
    this->initConnect();
}

MainWindow::~MainWindow()
{
    delete addrEdit;
    delete searchButton;
    delete resultEdit;
    delete mgr;
}

void MainWindow::initUI(){
    resize(800, 600);

    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->setSpacing(5);
    searchLayout->setContentsMargins(0, 0, 0, 0);

    addrEdit = new QLineEdit;
    addrEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //搜狐
//    addrEdit->setText("http://q.stock.sohu.com/hisHq?code=cn_002724&start=20160201&end=20160201&stat=1&order=D&period=d&callback=historySearchHandler&rt=jsonp");
    //谷歌
    addrEdit->setText("http://www.google.com/finance/getprices?q=002724&x=SHE&i=60&p=1d&f=d,c,h,l,o,v");
    searchLayout->addWidget(addrEdit, 0, Qt::AlignVCenter);

    searchButton = new QPushButton;
    searchButton->setText(tr("爬一爬"));
    searchButton->setFixedSize(80, 20);
    searchLayout->addWidget(searchButton, 0, Qt::AlignVCenter);

    resultEdit = new QPlainTextEdit;
    resultEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QDateTime dateTime = QDateTime::fromString("2016-03-05 18:30:00", "yyyy-MM-dd hh:mm:ss");
    dateTime = dateTime.addSecs(-60);
    resultEdit->setPlainText(dateTime.toString("yyyy-MM-dd hh:mm:ss"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(5);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(resultEdit);

    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);

    setCentralWidget(mainWidget);
}

void MainWindow::initConnect(){
    mgr = new QNetworkAccessManager(this);
    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(query(QNetworkReply*)));
    connect(searchButton, SIGNAL(clicked()), this, SLOT(onSearchClicked()));
}

//谷歌
void MainWindow::query(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QString buffer = reply->readAll();
        QRegExp exp(".*\n.*\n.*\nINTERVAL=(\\d*)\n.*\n.*\n.*\n.*\n(.*)");
        exp.setMinimal(false);
        int pos = exp.indexIn(buffer, 0);
        if(pos == -1){
            resultEdit->setPlainText("服务器数据格式不符合程序定义:\r\n" + buffer);
            return;
        }
        int interval = exp.cap(1).toInt();
        QString data = exp.cap(2);

        exp = QRegExp("(.*)\n");
        exp.setMinimal(true);
        pos = 0;
        uint firstTime = 0;
        while((pos = exp.indexIn(data, pos)) != -1){
            QString result = exp.cap(1);
            QStringList resLst = result.split(",");
            int resLen = resLst.length();
            QString printText;
            for(int i = 0;i < resLen;i++){
                if(i == 0){
                    QDateTime time;
                    QString timeStr = resLst[i];
                    if(firstTime == 0){
                        timeStr.remove(0, 1);
                        firstTime = timeStr.toInt();
                        time = QDateTime::fromTime_t(firstTime);
                    }else{
                        if(timeStr.indexOf("a") != -1){
                            timeStr.remove(0, 1);
                            firstTime = timeStr.toInt();
                            time = QDateTime::fromTime_t(firstTime);
                        }else{
                            time = QDateTime::fromTime_t(firstTime + interval * timeStr.toInt());
                        }
                    }
//                    printText += "时间" + time.toString("yyyy-MM-dd") + " " + time.toString("hh:mm:ss") + "\t";
                    printText += "\"" + time.toString("yyyy-MM-dd") + " " + time.toString("hh:mm:ss") + "\"";
                }else{
                    switch (i) {
                    case 1:
//                        printText += "收盘价" + resLst[i] + "\t";
                        printText += "," + resLst[i];
                        break;
                    case 2:
//                        printText += "最高价" + resLst[i] + "\t";
                        printText += "," + resLst[i];
                        break;
                    case 3:
//                        printText += "最低价" + resLst[i] + "\t";
                        printText += "," + resLst[i];
                        break;
                    case 4:
//                        printText += "开盘价" + resLst[i] + "\t";
                        printText += "," + resLst[i];
                        break;
                    case 5:
//                        printText += "成交量" + resLst[i] + "\t";
                        printText += "," + resLst[i];
                        break;
                    default:
                        break;
                    }
                }
            }
            resultEdit->appendPlainText(printText);
            pos += exp.matchedLength();
        }

    }else{
        resultEdit->setPlainText("网络请求失败");
    }
}

//搜狐
//void MainWindow::query(QNetworkReply *reply)
//{
//    if(reply->error() == QNetworkReply::NoError)
//    {
//        QString buffer = reply->readAll();

//        QRegExp exp("\"hq\":(.*),\"code");
//        exp.setMinimal(false);
//        int pos = exp.indexIn(buffer, 0);
//        if(pos == -1){
//            resultEdit->setPlainText("服务器数据格式不符合程序定义:" + buffer);
//            return;
//        }

//        QByteArray data = "{\"data\":" + exp.cap(1).toUtf8() + "}";
//        QJsonParseError jsonError;
//        QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);

//        if(jsonError.error == QJsonParseError::NoError)
//        {
//            if(json.isObject())
//            {
//                QJsonObject obj = json.object();
//                QJsonArray mainArray = obj["data"].toArray();
//                int mainLen = mainArray.size();
//                for(int i = 0;i < mainLen;i++){
//                    QJsonArray subArray = mainArray[i].toArray();
//                    int subLen = subArray.size();
//                    QString str;
//                    for(int j = 0;j < subLen;j++){
//                        str += titles[j] + subArray[j].toString() + " ";
//                    }
//                    resultEdit->appendPlainText(str);
//                }

//            }else{
//                resultEdit->setPlainText("数据解析失败");
//            }
//        }else{
//            resultEdit->setPlainText(jsonError.errorString());
//        }
//    }else{
//        resultEdit->setPlainText("网络请求失败");
//    }
//}

void MainWindow::onSearchClicked()
{
    resultEdit->clear();
    qDebug("下面开始爬");
    mgr->get(QNetworkRequest(QUrl(addrEdit->text())));
}
