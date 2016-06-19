#include <QCoreApplication>
#include "ea_db.h"

QTextStream cin(stdin, QIODevice::ReadOnly);
QTextStream cout(stdout, QIODevice::WriteOnly);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ExchangeServiceDb *eaDb = new ExchangeServiceDb;

    QString argStr;
    while(true){
        qDebug() << "please input arguments:tableName,fromTime(yyyy-MM-dd),toTime(yyyy-MM-dd)";
        cin >> argStr;
        QStringList argList = argStr.split(",");
        int count = argList.size();
        if(count == 3){
            eaDb->execute(argList[0], argList[1], argList[2]);
        }else{
            qDebug() << "arguments error!";
        }
    }
    return a.exec();
}

