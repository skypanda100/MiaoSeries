#ifndef LEFTWIDGET
#define LEFTWIDGET

#include <QtWidgets>
#include "ea_db.h"
#include "ea_result.h"
#include "ratecolordialog.h"

class SearchWidget : public QWidget
{
    Q_OBJECT

public:
    SearchWidget(QWidget *parent = 0);
    ~SearchWidget();

signals:
    void searchResult(QString, QList<ExchangeRateResult *>);

public slots:
    void onSearchClicked();

private:
    void initData();
    void initUI();
    void initConnect();

private slots:
    void showColorDialog();
    void changeType();
    void checkGroup1(bool);
    void checkGroup2(bool);
    void checkGroup3(bool);
    void checkGroup4(bool);
    void checkGroup5(bool);
    void checkGroup6(bool);
    void checkGroup7(bool);

private:
    ExchangeServiceDb *eaDb;
    QList<QString> e_tables;
    QList<QString> s_tables;
    QList<QString> rates;
    QList<QString> stocks;

    QLabel *typeLabel;
    QPushButton *changeButton;
    QComboBox *rateComboBox;
    QLineEdit *fdateEdit;
    QLineEdit *tdateEdit;
    QPushButton *colorButton;
    QCheckBox *periodCheckBox1_1;
    QCheckBox *periodCheckBox1_2;
    QCheckBox *periodCheckBox1_3;
    QCheckBox *periodCheckBox1_4;
    QCheckBox *periodCheckBox2_1;
    QCheckBox *periodCheckBox2_2;
    QCheckBox *periodCheckBox2_3;
    QCheckBox *periodCheckBox2_4;
    QCheckBox *periodCheckBox3_1;
    QCheckBox *periodCheckBox3_2;
    QCheckBox *periodCheckBox3_3;
    QCheckBox *periodCheckBox3_4;
    QCheckBox *periodCheckBox4_1;
    QCheckBox *periodCheckBox4_2;
    QCheckBox *periodCheckBox4_3;
    QCheckBox *periodCheckBox4_4;
    QCheckBox *periodCheckBox5_1;
    QCheckBox *periodCheckBox5_2;
    QCheckBox *periodCheckBox5_3;
    QCheckBox *periodCheckBox5_4;
    QCheckBox *periodCheckBox6_1;
    QCheckBox *periodCheckBox6_2;
    QCheckBox *periodCheckBox6_3;
    QCheckBox *periodCheckBox6_4;
    QCheckBox *periodCheckBox7_1;
    QCheckBox *periodCheckBox7_2;
    QCheckBox *periodCheckBox7_3;
    QCheckBox *periodCheckBox7_4;
    QPushButton *searchButton;
    RateColorDialog *rateColorDialog;
};

#endif // LEFTWIDGET

