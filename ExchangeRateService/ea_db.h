#ifndef EA_DB
#define EA_DB

#include <QObject>
#include <QtSql>

class ExchangeServiceDb : public QObject{
    Q_OBJECT

public:
    ExchangeServiceDb();
    ~ExchangeServiceDb();

    bool insert(QString, QString, QString);
    bool insert(QString, QString, int);
    double query(QString, QString, int, int);
    int query(QString, QString);

private:
    void openDb();
    void closeDb();

private:
    QSqlDatabase db;
};

#endif // EA_DB

