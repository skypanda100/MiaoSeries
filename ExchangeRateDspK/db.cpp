#include "db.h"
#include <QMessageBox>
#include "chartdir.h"

Db::Db(){
    this->openDb();
}

Db::~Db(){
    this->closeDb();
}

void Db::openDb(){
    db = QSqlDatabase::addDatabase("QODBC");
    QString dsn = QString::fromLocal8Bit("Driver={PostgreSQL ODBC Driver(ANSI)};server=47.94.165.17;port=5432;uid=posgres;pwd=123456;database=postgres");
    db.setDatabaseName(dsn);
    db.setUserName("postgres");
    db.setPassword("123456");
    if(!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              db.lastError().text());
    }
}

void Db::closeDb(){
    db.close();
}

QList<Result *> Db::query(QString queryStr){
    QList<Result *> ea_results;

    QSqlQuery query(queryStr);
    while (query.next()){
        Result *ea_result = new Result;
        ea_result->setDate(query.value("date").toString());
        ea_result->setOpen(query.value("open").toDouble());
        ea_result->setClose(query.value("close").toDouble());
        ea_result->setHigh(query.value("high").toDouble());
        ea_result->setLow(query.value("low").toDouble());
        ea_results.append(ea_result);
    }

    return ea_results;
}
