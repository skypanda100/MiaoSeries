#ifndef MAINWIDGET
#define MAINWIDGET

#include <QtWidgets>
#include "searchwidget.h"
#include "detailwidget.h"
#include "resultwidget.h"
#include "graphwidget.h"
#include "ea_result.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    void initData();
    void initUI();
    void initConnect();

private:
    SearchWidget *searchWidget;
    DetailWidget *detailWidget;
    ResultWidget *resultWidget;
};

#endif // MAINWIDGET

