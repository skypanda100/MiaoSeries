#ifndef RESULTWIDGET
#define RESULTWIDGET

#include <QtWidgets>
#include "graphwidget.h"
#include "ea_result.h"

class TabBar : public QTabBar{
    Q_OBJECT

public:
    TabBar(QWidget *parent = 0);
    ~TabBar();

signals:
    void closeTab(int);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    friend class ResultWidget;
    QPoint m_dragStartPos;
    int m_dragCurrentIndex;
};

class ResultWidget : public QTabWidget{
    Q_OBJECT

public:
    ResultWidget(QWidget *parent = 0);
    ~ResultWidget();

    void setChildSize(QSize);
signals:
    void sendDetail(QStringList,QStringList,QList<int>,int);

public slots:
    void addGraphWidget(QString, QList<ExchangeRateResult*>);
    void closeTab(int index = -1);

private:
    void initUI();
    void initConnect();

private:
    TabBar *m_tabBar;
    QSize childSize;
};

class ResultMainWidget : public QWidget{
    Q_OBJECT

public:
    ResultMainWidget(QSize childSize, QWidget *parent = 0);
    ~ResultMainWidget();

signals:
    void searchResult(QString, QList<ExchangeRateResult *>);
    void sendDetail(QStringList,QStringList,QList<int>,int);

private:
    void initUI(QSize childSize);
};

#endif // RESULTWIDGET

