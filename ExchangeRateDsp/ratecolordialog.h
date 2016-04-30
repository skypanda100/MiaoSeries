#ifndef RATECOLORDIALOG
#define RATECOLORDIALOG

#include <QtWidgets>
#include <QtSql>

class RateColorDialog : public QDialog{
    Q_OBJECT

public:
    RateColorDialog(QWidget *parent = 0);
    ~RateColorDialog();

    int getColor(QString, int);

private:
    void initData();
    void initUI();
    void initConnect();
    void setData();

private slots:
    void onColorItemDoubleClicked(QTableWidgetItem *);

private:
    QList<QStringList > color_list;
    QTableWidget *m_ParamList;
};

#endif // RATECOLORDIALOG

