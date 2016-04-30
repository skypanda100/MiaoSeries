#include <QCoreApplication>
#include "ea_db.h"

QTextStream cin(stdin, QIODevice::ReadOnly);
QTextStream cout(stdout, QIODevice::WriteOnly);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ExchangeServiceDb *eaDb = new ExchangeServiceDb;

    QString tableStr;
    while(true){
        qDebug() << "please input table name(tbl1 or tbl1,tbl2,tbl3):";
        cin >> tableStr;
        QStringList tableList = tableStr.split(",");
        int count = tableList.size();
        for(int i = 0;i < count;i++){
            eaDb->execute(tableList[i]);
        }
    }
    return a.exec();
}

