#include "ea_net.h"
#include <QDateTime>
#include <QRegExp>
#include <QDebug>

ExchangeRateWorker::ExchangeRateWorker(){
    eaDb = new ExchangeServiceDb;
    //数据库表
    tables << "GBPUSD"
           << "USDJPY"
           << "CNHJPY";
    //检索key
    keys << "GBP/USD"
         << "USD/JPY"
         << "CNH/JPY";
}

ExchangeRateWorker::~ExchangeRateWorker(){
    delete eaDb;
}

void ExchangeRateWorker::work(QString html){
    QString rateHtml = html.trimmed();
    if(rateHtml.isEmpty()){
        sendMessage();
    }else{
        QRegExp timeExp(QString(".*"
                          "\"updatedAt\": "
                          "\"([0-9]{14})\","
                          ".*"));
        timeExp.setMinimal(true);
        int pos = timeExp.indexIn(rateHtml, 0);
        if(pos != -1){
            QString timeStr = timeExp.cap(1);
            //将日本时间转换为中国时间
            QDateTime time = QDateTime::fromString(timeStr, "yyyyMMddhhmmss");
            time = time.addSecs(-3600);
            //操作英镑美元
            execute(html, GBPUSD, time);
            //操作美元日元
            execute(html, USDJPY, time);
            //操作人民币日元
            execute(html, CNHJPY, time);
        }else{
            sendMessage();
        }
    }
}

void ExchangeRateWorker::execute(QString html, RATE rate, QDateTime time){
    //求时间
    QString currentTimet = time.toString("yyyy-MM-dd hh:mm:00");
    //求分钟数
    int minutes = time.toString("mm").toInt();
    //求小时数
    int hours = time.toString("hh").toInt();
    //求星期几
    int weekday = time.date().dayOfWeek();
    //数据库表名
    QString table = tables[rate];
    //检索key
    QString key = keys[rate];

    //check表中是否有重复数据
    if(eaDb->query(table, currentTimet) > 0){
        return;
    }

    //正则查找汇率
    QRegExp rateExp(QString(".*"
                      "\"cd\": "
                      "\"%1\","
                      ".*"
                      "\"b\": "
                      "\"([0-9]+\.?[0-9]+)\","
                      ".*").arg(key));
    rateExp.setMinimal(true);
    int pos = rateExp.indexIn(html, 0);

    if(pos != -1){
        //向数据库中插入数据
        eaDb->insert(table, rateExp.cap(1), currentTimet);

        //周期5分钟
        if(minutes % 5 == 0){
            eaDb->insert(table, currentTimet, 5);
        }

        //周期15分钟
        if(minutes % 15 == 0){
            eaDb->insert(table, currentTimet, 15);
        }

        //周期30分钟
        if(minutes % 30 == 0){
            eaDb->insert(table, currentTimet, 30);
        }

        //周期60分钟
        if(minutes == 55){
            eaDb->insert(table, currentTimet, 60);
        }

        //周期240分钟
        if((hours + 1 - 6) % 4 == 0 && minutes == 55){
            eaDb->insert(table, currentTimet, 240);
        }

        //周期1440分钟
        if(hours == 5 && minutes == 55){
            eaDb->insert(table, currentTimet, 1440);
        }

        //周期10080分钟
        if(weekday == 6 && hours == 5 && minutes == 55){
            eaDb->insert(table, currentTimet, 10080);
        }
    }else{
        sendMessage();
    }
}

void ExchangeRateWorker::sendMessage(){
    QString message = QString("数据抓取失败!\n%1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    emit showMessage(message);
}

ExchangeRateNet::ExchangeRateNet(){
    this->init();
}

ExchangeRateNet::~ExchangeRateNet(){
    delete timer;
    delete eaWorker;
    if(thread->isRunning()){
        thread->quit();
        thread->wait();
        delete thread;
    }
    delete mgr;
}

void ExchangeRateNet::init(){
//    url = QString("http://quote.hexun.com/forex/forex.aspx?type=3");
    url = QString("http://exquote.yjfx.jp/quote.js");

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(crawler()));

    mgr = new QNetworkAccessManager(this);
    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(query(QNetworkReply*)));

    thread = new QThread;
    eaWorker = new ExchangeRateWorker;
    eaWorker->moveToThread(thread);
    connect(this, SIGNAL(doWork(QString)), eaWorker, SLOT(work(QString)), Qt::QueuedConnection);
    connect(eaWorker, SIGNAL(showMessage(QString)), this, SIGNAL(showMessage(QString)));
    thread->start();
}

void ExchangeRateNet::query(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        QString html = reply->readAll();
        emit doWork(html);
    }else{
        QString message = QString("数据抓取失败!\n%1\n%2")
                .arg(reply->errorString())
                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        qDebug() << reply->errorString();
        emit showMessage(message);
    }

    reply->deleteLater();
}

void ExchangeRateNet::crawler(){
    QDateTime dateTime = QDateTime::currentDateTime();
    int weekday = dateTime.date().dayOfWeek();
    int hours = dateTime.toString("hh").toInt();
    int minutes = dateTime.toString("mm").toInt();

    /*
     * 从星期一早上六点到星期六早上六点执行抓取动作
     */
    if((weekday >= 2 && weekday <= 5)
    || (weekday == 1 && hours >= 6)
    || (weekday == 6 && hours < 6)){
        if(minutes % CRAWLER_INTERVAL == 0){
            mgr->get(QNetworkRequest(QUrl(url)));
        }
    }
}

void ExchangeRateNet::startService(){
    timer->setInterval(60000);
    timer->start();
}

void ExchangeRateNet::stopService(){
    timer->stop();
}
