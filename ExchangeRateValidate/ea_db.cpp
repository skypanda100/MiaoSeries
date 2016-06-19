#include "ea_db.h"

ExchangeServiceDb::ExchangeServiceDb(){
    this->openDb();
}

ExchangeServiceDb::~ExchangeServiceDb(){
    this->closeDb();
}

void ExchangeServiceDb::execute(QString tableStr, QString ymdStrFrom, QString ymdStrTo){
    QDateTime dateTimeFrom = QDateTime::fromString(ymdStrFrom, "yyyy-MM-dd");
    QDateTime dateTimeTo = QDateTime::fromString(QString("%1 23:59:59").arg(ymdStrTo), "yyyy-MM-dd hh:mm:ss");
    if(dateTimeFrom.secsTo(dateTimeTo) < 0){
        qDebug() << "end time is earlier than from time!";
        return;
    }
    while(dateTimeFrom.secsTo(dateTimeTo) >= 0){
        QString ymdhmsStrFrom = dateTimeFrom.toString("yyyy-MM-dd hh:mm:00");
        QString ymdhmsStrTo = dateTimeFrom.toString("yyyy-MM-dd hh:59:00");
        QString queryStr = QString("select * from %1 where timet >= '%2' and timet <= '%3' order by timet")
                .arg(tableStr)
                .arg(ymdhmsStrFrom)
                .arg(ymdhmsStrTo);
        QSqlQuery query(queryStr);
        QList<QString> timetList;
        while(query.next()){
            QString timetStr = query.value(2).toString();
            timetList.append(timetStr);
        }
        QList<QString> validateTimeList;
        for(int i = 0;i < 60;i++){
            if(i % 5 == 0){
                if(i < 10){
                    validateTimeList.append(
                                QString("%1:0%2:00")
                                .arg(dateTimeFrom.toString("yyyy-MM-ddThh"))
                                .arg(i));

                }else{
                    validateTimeList.append(
                                QString("%1:%2:00")
                                .arg(dateTimeFrom.toString("yyyy-MM-ddThh"))
                                .arg(i));
                }
            }
        }
        foreach (QString validateTimeStr, validateTimeList) {
            int count = timetList.count(validateTimeStr);
            if(count == 0){
                qDebug() <<  "no data : " << validateTimeStr;
            }else if(count > 1){
                qDebug() <<  "duplicated data(" << count << ") : " << validateTimeStr;
            }
        }

        dateTimeFrom = dateTimeFrom.addSecs(3600);
    }
}

void ExchangeServiceDb::openDb(){
    db = QSqlDatabase::addDatabase("QODBC");
    QString dsn = QString::fromLocal8Bit("Driver={PostgreSQL ODBC Driver(ANSI)};server=127.0.0.1;port=5432;uid=posgres;pwd=123456;database=postgres");
    db.setDatabaseName(dsn);
    db.setUserName("postgres");
    db.setPassword("123456");
    if(!db.open()) {
        qDebug() << "database error";
    }else{
        qDebug() << "database open";
    }
}

void ExchangeServiceDb::closeDb(){
    db.close();
    qDebug() << "database close";
}
