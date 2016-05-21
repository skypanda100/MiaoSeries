#ifndef DETAILWIDGET
#define DETAILWIDGET

#include <QtWidgets>

class DetailWidget : public QWidget{
    Q_OBJECT
public:
    DetailWidget(QWidget *parent = 0);
    ~DetailWidget();

public slots:
    void setData(QStringList, QStringList, QList<int>, int times = 10000);

private:
    void initUI();

private:
    QTableWidget *m_ParamList;
};

#endif // DETAILWIDGET

