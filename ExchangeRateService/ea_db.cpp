#include "ea_db.h"
#include <QMessageBox>
#include <QDateTime>

ExchangeServiceDb::ExchangeServiceDb(){
    this->openDb();
}

ExchangeServiceDb::~ExchangeServiceDb(){
    this->closeDb();
}

void ExchangeServiceDb::openDb(){
    db = QSqlDatabase::addDatabase("QODBC");
    QString dsn = QString::fromLocal8Bit("Driver={PostgreSQL ODBC Driver(ANSI)};server=127.0.0.1;port=5432;uid=posgres;pwd=123456;database=postgres");
    db.setDatabaseName(dsn);
    db.setUserName("postgres");
    db.setPassword("123456");
    if(!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              db.lastError().text());
    }
}

void ExchangeServiceDb::closeDb(){
    db.close();
}

bool ExchangeServiceDb::insert(QString table, QString ma1, QString timet){
    QString insertStr = QString("insert into %1(ma1, timet) values (?,?)").arg(table);
    QSqlQuery query;
    query.prepare(insertStr);
    query.addBindValue(ma1);
    query.addBindValue(timet);
    return query.exec();
}

bool ExchangeServiceDb::insert(QString table, QString timet, int interval){
    double ma14 = query(table, timet, interval, 14);
    double ma60 = query(table, timet, interval, 60);
    double ma99 = query(table, timet, interval, 99);
    double ma888 = query(table, timet, interval, 888);

    QString insertStr = QString("insert into %1_%2(ma14, ma60, ma99, ma888, timet) values (?,?,?,?,?)")
            .arg(table)
            .arg(interval);
    QSqlQuery query;
    query.prepare(insertStr);
    query.addBindValue(ma14 == 0.0?QVariant(QVariant::Double):ma14);
    query.addBindValue(ma60 == 0.0?QVariant(QVariant::Double):ma60);
    query.addBindValue(ma99 == 0.0?QVariant(QVariant::Double):ma99);
    query.addBindValue(ma888 == 0.0?QVariant(QVariant::Double):ma888);
    query.addBindValue(timet);

    return query.exec();
}

double ExchangeServiceDb::query(QString table, QString timet, int interval, int count){
    QDateTime dateTimeTo = QDateTime::fromString(timet, "yyyy-MM-dd hh:mm:ss");
    QString queryStr;
    switch(interval){
    case 5:
    case 15:
    case 30:
        queryStr = QString("select ma1 from %1 "
                           "where timet <= '%2' and mod(cast(extract(Minutes from timet) as int), %3) = 0 "
                           "order by timet desc "
                           "limit %4").arg(table).arg(dateTimeTo.toString("yyyy-MM-dd hh:mm:ss")).arg(interval).arg(count);
        break;
    case 60:
        queryStr = QString("select ma1 from %1 "
                           "where timet <= '%2' and extract(Minutes from timet) = 55 "
                           "order by timet desc "
                           "limit %3").arg(table).arg(dateTimeTo.toString("yyyy-MM-dd hh:mm:ss")).arg(count);
        break;
    case 240:
        queryStr = QString("select ma1 from %1 "
                           "where timet <= '%2' "
                           "and mod(cast(extract(Hours from timet) as int) + 1 - 6, 4) = 0 "
                           "and extract(Minutes from timet) = 55 "
                           "order by timet desc "
                           "limit %3").arg(table).arg(dateTimeTo.toString("yyyy-MM-dd hh:mm:ss")).arg(count);
        break;
    case 1440:
        queryStr = QString("select ma1 from %1 "
                           "where timet <= '%2' "
                           "and extract(Hours from timet) = 5 "
                           "and extract(Minutes from timet) = 55 "
                           "order by timet desc "
                           "limit %3").arg(table).arg(dateTimeTo.toString("yyyy-MM-dd hh:mm:ss")).arg(count);
        break;
    case 10080:
        queryStr = QString("select ma1 from %1 "
                           "where timet <= '%2' "
                           "and extract(DOW from timet) = 6 "
                           "and extract(Hours from timet) = 5 "
                           "and extract(Minutes from timet) = 55 "
                           "order by timet desc "
                           "limit %3").arg(table).arg(dateTimeTo.toString("yyyy-MM-dd hh:mm:ss")).arg(count);
        break;
    }

    QSqlQuery query(queryStr);
    double money = 0.0;
    int rsCount = 0;
    while (query.next()){
        money += query.value(0).toDouble();
        rsCount++;
    }
    if(count == rsCount){
        money = money / count;
    }else{
        money = 0.0;
    }
    return money;
}

int ExchangeServiceDb::query(QString table, QString timet){
    QString queryStr = QString("select count(1) from %1 where timet = '%2'")
            .arg(table)
            .arg(timet);

    QSqlQuery query(queryStr);
    int rsCount = 0;
    while (query.next()){
        rsCount = query.value(0).toInt();
    }
    return rsCount;
}
