#ifndef DETAILWIDGET
#define DETAILWIDGET

#include <QtWidgets>

class DetailWidget : public QWidget{
    Q_OBJECT
public:
    DetailWidget(QWidget *parent = 0);
    ~DetailWidget();

public slots:
    void setData(QStringList, QStringList);

private:
    void initUI();

private:
    QTableWidget *m_ParamList;
};

#endif // DETAILWIDGET

