#ifndef EA_NET
#define EA_NET

#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QThread>
#include <QList>
#include "ea_db.h"

#define CRAWLER_INTERVAL 5

class ExchangeRateWorker : public QObject{
    Q_OBJECT

public:
    ExchangeRateWorker();
    ~ExchangeRateWorker();
    static QString currentTimet;

    enum RATE {
        GBPUSD, //英镑美元
        USDJPY, //美元日元
        CNHJPY  //人民币日元
    };

public slots:
    void work(QString);

private:
    void execute(QString, RATE, int, int, int);

private:
    ExchangeServiceDb *eaDb;

    QList<QString> tables;
    QList<QString> keys;
};

class ExchangeRateNet : public QObject
{
    Q_OBJECT

public:
    ExchangeRateNet();
    ~ExchangeRateNet();

signals:
    void doWork(QString);

public slots:
    void query(QNetworkReply *);
    void crawler();
    void startService();
    void stopService();

private:
    void init();

private:
    QString url;
    QTimer *timer;
    QThread thread;
    ExchangeRateWorker *eaWorker;
};

#endif // EA_NET

