#ifndef DB
#define DB

#include <QObject>
#include <QtSql>
#include <QDateTime>
#include "result.h"

class Db : public QObject{
    Q_OBJECT

public:
    Db();
    ~Db();

    QList<Result *> query(QString);

private:
    void openDb();
    void closeDb();

private:
    QSqlDatabase db;
};

#endif // DB

