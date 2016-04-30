#ifndef EA_DB
#define EA_DB

#include <QObject>
#include <QtSql>

class ExchangeServiceDb : public QObject{
    Q_OBJECT

public:
    ExchangeServiceDb();
    ~ExchangeServiceDb();

    void execute(QString);
    bool insert(int, QString, QString, int);
    double query(int, QString, QString, int, int);
private:
    void openDb();
    void closeDb();

private:
    QSqlDatabase db;
};

#endif // EA_DB

