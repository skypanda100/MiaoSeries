#include "mainwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //加载QSS样式表
//    QFile qss(":/style/dracula.css");
//    qss.open(QFile::ReadOnly);
//    qApp->setStyleSheet(qss.readAll());
//    qss.close();

    MainWidget mainWidget;
    mainWidget.setWindowTitle("涨停喵");
    mainWidget.show();
    return app.exec();
}
