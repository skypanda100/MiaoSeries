#include "mainwidget.h"
#include "window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //加载QSS样式表
    QFile qss(":/qdarkstyle/style.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();

    Window window;
    window.setTitle("涨停喵");
    MainWidget mainWidget;
    window.setCentralWidget(&mainWidget);
    window.show();
    window.move(10, 10);
    return app.exec();
}
