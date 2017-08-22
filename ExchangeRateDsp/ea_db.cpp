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
    QString dsn = QString::fromLocal8Bit("Driver={PostgreSQL ODBC Driver(ANSI)};server=192.168.1.3;port=15432;uid=posgres;pwd=123456;database=postgres");
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

QList<ExchangeRateResult *> ExchangeServiceDb::query(QString queryStr, QString name, int color){
    QList<ExchangeRateResult *> ea_results;

    QSqlQuery query(queryStr);
    int resultCount = query.numRowsAffected();

    int i = 0;
    double *xVals = new double[resultCount];
    double *yVals = new double[resultCount];
    while (query.next()){
        xVals[i] = Chart::chartTime2(QDateTime::fromString(query.value(1).toString(), "yyyy-MM-ddThh:mm:ss").toTime_t());
        yVals[i] = query.value(0).toDouble();
        i++;
    }

    if(i > 0){
        ExchangeRateResult *ea_result = new ExchangeRateResult;
        ea_result->setName(name);
        ea_result->setColor(color);
        ea_result->setXData(DoubleArray(xVals, resultCount));
        ea_result->setYData(DoubleArray(yVals, resultCount));
        ea_results << ea_result;
    }

    return ea_results;
}
