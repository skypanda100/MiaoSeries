#ifndef EA_DB
#define EA_DB

#include <QObject>
#include <QtSql>
#include <QDateTime>
#include "memblock.h"
#include "ea_result.h"

class ExchangeServiceDb : public QObject{
    Q_OBJECT

public:
    ExchangeServiceDb();
    ~ExchangeServiceDb();

    QList<ExchangeRateResult *> query(QString, QString, int);

private:
    void openDb();
    void closeDb();

private:
    QSqlDatabase db;
};

#endif // EA_DB

