#include "ea_db.h"
#include <QDateTime>
#include <QDebug>

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
        qDebug() << "database error";
    }else{
        qDebug() << "database open";
    }
}

void ExchangeServiceDb::closeDb(){
    db.close();
    qDebug() << "database close";
}

bool ExchangeServiceDb::insert(int tableType, QString table, QString timet, int interval){
    double ma14 = query(tableType, table, timet, interval, 14);
    double ma60 = query(tableType, table, timet, interval, 60);
    double ma99 = query(tableType, table, timet, interval, 99);
    double ma888 = query(tableType, table, timet, interval, 888);

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

double ExchangeServiceDb::query(int tableType, QString table, QString timet, int interval, int count){
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
        if(tableType == 0){
            queryStr = QString("select ma1 from %1 "
                               "where timet <= '%2' "
                               "and ("
                               "extract(Hours from timet) = 10 and extract(Minutes from timet) = 30 "
                               "or extract(Hours from timet) = 10 and extract(Minutes from timet) = 30 "
                               "or extract(Hours from timet) = 11 and extract(Minutes from timet) = 30 "
                               "or extract(Hours from timet) = 14 and extract(Minutes from timet) = 0 "
                               "or extract(Hours from timet) = 15 and extract(Minutes from timet) = 0 "
                               ") "
                               "order by timet desc "
                               "limit %3").arg(table).arg(dateTimeTo.toString("yyyy-MM-dd hh:mm:ss")).arg(count);
        }else{
            queryStr = QString("select ma1 from %1 "
                               "where timet <= '%2' and extract(Minutes from timet) = 55 "
                               "order by timet desc "
                               "limit %3").arg(table).arg(dateTimeTo.toString("yyyy-MM-dd hh:mm:ss")).arg(count);
        }
        break;
    case 240:
        if(tableType == 0){
            queryStr = QString("select ma1 from %1 "
                               "where timet <= '%2' "
                               "and extract(Hours from timet) = 15 "
                               "and extract(Minutes from timet) = 0 "
                               "order by timet desc "
                               "limit %3").arg(table).arg(dateTimeTo.toString("yyyy-MM-dd hh:mm:ss")).arg(count);
        }else{
            queryStr = QString("select ma1 from %1 "
                               "where timet <= '%2' "
                               "and mod(cast(extract(Hours from timet) as int) + 1 - 6, 4) = 0 "
                               "and extract(Minutes from timet) = 55 "
                               "order by timet desc "
                               "limit %3").arg(table).arg(dateTimeTo.toString("yyyy-MM-dd hh:mm:ss")).arg(count);
        }
        break;
    case 1440:
        if(tableType == 0){
            queryStr = QString("select ma1 from %1 "
                               "where timet <= '%2' "
                               "and extract(Hours from timet) = 15 "
                               "and extract(Minutes from timet) = 0 "
                               "order by timet desc "
                               "limit %3").arg(table).arg(dateTimeTo.toString("yyyy-MM-dd hh:mm:ss")).arg(count);
        }else{
            queryStr = QString("select ma1 from %1 "
                               "where timet <= '%2' "
                               "and extract(Hours from timet) = 5 "
                               "and extract(Minutes from timet) = 55 "
                               "order by timet desc "
                               "limit %3").arg(table).arg(dateTimeTo.toString("yyyy-MM-dd hh:mm:ss")).arg(count);
        }
        break;
    case 10080:
        if(tableType == 0){
            queryStr = QString("select ma1 from %1 "
                               "where timet <= '%2' "
                               "and extract(DOW from timet) = 5 "
                               "and extract(Hours from timet) = 15 "
                               "and extract(Minutes from timet) = 0 "
                               "order by timet desc "
                               "limit %3").arg(table).arg(dateTimeTo.toString("yyyy-MM-dd hh:mm:ss")).arg(count);
        }else{
            queryStr = QString("select ma1 from %1 "
                               "where timet <= '%2' "
                               "and extract(DOW from timet) = 6 "
                               "and extract(Hours from timet) = 5 "
                               "and extract(Minutes from timet) = 55 "
                               "order by timet desc "
                               "limit %3").arg(table).arg(dateTimeTo.toString("yyyy-MM-dd hh:mm:ss")).arg(count);
        }
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

void ExchangeServiceDb::execute(QString table){
    QString queryMstStr = QString("select tabletype from tablemst where tablename = '%1'").arg(table.toUpper());
    QSqlQuery queryMst(queryMstStr);
    queryMst.next();
    int tableType = queryMst.value(0).toInt();
    qDebug() << "start : calculate [" << table << "]" << "[" << (tableType == 0?"stock":"rate") <<"]";
    QString queryStr = QString("select timet from %1 order by timet asc").arg(table);
    QSqlQuery query(queryStr);
    while (query.next()){
        QDateTime dateTime = QDateTime::fromString(query.value(0).toString(), "yyyy-MM-ddThh:mm:ss");
        //求分钟数
        int minutes = dateTime.toString("mm").toInt();
        //求小时数
        int hours = dateTime.toString("hh").toInt();
        //求星期几
        int weekday = dateTime.date().dayOfWeek();

        if(minutes % 5 == 0){
            insert(tableType, table, dateTime.toString("yyyy-MM-dd hh:mm:ss"), 5);
        }

        if(minutes % 15 == 0){
            insert(tableType, table, dateTime.toString("yyyy-MM-dd hh:mm:ss"), 15);
        }

        if(minutes % 30 == 0){
            insert(tableType, table, dateTime.toString("yyyy-MM-dd hh:mm:ss"), 30);
        }

        if(tableType == 0){
            if((hours == 10 && minutes == 30)
                    || (hours == 11 && minutes == 30)
                    || (hours == 14 && minutes == 0)
                    || (hours == 15 && minutes == 0)){
                insert(tableType, table, dateTime.toString("yyyy-MM-dd hh:mm:ss"), 60);
            }
        }else{
            if(minutes == 55){
                insert(tableType, table, dateTime.toString("yyyy-MM-dd hh:mm:ss"), 60);
            }

        }

        if(tableType == 0){
            if(hours == 15 && minutes == 0){
                insert(tableType, table, dateTime.toString("yyyy-MM-dd hh:mm:ss"), 240);
            }
        }else{
            if((hours + 1 - 6) % 4 == 0 && minutes == 55){
                insert(tableType, table, dateTime.toString("yyyy-MM-dd hh:mm:ss"), 240);
            }
        }

        if(tableType == 0){
            if(hours == 15 && minutes == 0){
                insert(tableType, table, dateTime.toString("yyyy-MM-dd hh:mm:ss"), 1440);
            }
        }else{
            if(hours == 5 && minutes == 55){
                insert(tableType, table, dateTime.toString("yyyy-MM-dd hh:mm:ss"), 1440);
            }
        }

        if(tableType == 0){
            if(weekday == 5 && hours == 15 && minutes == 0){
                insert(tableType, table, dateTime.toString("yyyy-MM-dd hh:mm:ss"), 10080);
            }
        }else{
            if(weekday == 6 && hours == 5 && minutes == 55){
                insert(tableType, table, dateTime.toString("yyyy-MM-dd hh:mm:ss"), 10080);
            }
        }

    }
    qDebug() << "end : calculate [" << table << "]" << "[" << (tableType == 0?"stock":"rate") <<"]";
}
