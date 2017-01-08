#ifndef DATARESULTWIDGET
#define DATARESULTWIDGET

#include <QtWidgets>
#include "simulate.h"

class DataResultWidget : public QWidget{
    Q_OBJECT
public:
    DataResultWidget(QWidget *parent = NULL);
    ~DataResultWidget();

public slots:
    void onOperate(Simulate);
    void onOperateEnd();

private:
    void initUI();
    void initConnect();

private:
    QStringList m_titleList;
    QTableWidget *m_dataWidget;
};

#endif // DATARESULTWIDGET

