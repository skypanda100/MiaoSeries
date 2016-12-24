#include "ea_db.h"
#include <QMessageBox>
#include "chartdir.h"

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

QList<ExchangeRateResult *> ExchangeServiceDb::query(QString queryStr){
    QList<ExchangeRateResult *> ea_results;

    QSqlQuery query(queryStr);
    while (query.next()){
        ExchangeRateResult *ea_result = new ExchangeRateResult;
        ea_result->setDate(query.value("date").toString());
        ea_result->setOpen(query.value("open").toDouble());
        ea_result->setClose(query.value("close").toDouble());
        ea_result->setHigh(query.value("high").toDouble());
        ea_result->setLow(query.value("low").toDouble());
        ea_results.append(ea_result);
    }

    return ea_results;
}
