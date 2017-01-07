#ifndef DATARESULTWIDGET
#define DATARESULTWIDGET

#include <QtWidgets>

class DataResultWidget : public QWidget{
    Q_OBJECT
public:
    DataResultWidget(QWidget *parent = NULL);
    ~DataResultWidget();

private:
    void initUI();
    void initConnect();

private:
    QTableWidget *m_dataWidget;
};

#endif // DATARESULTWIDGET

