#ifndef INPUTWIDGET
#define INPUTWIDGET

#include <QtWidgets>
#include <QPrinter>
#include <QPrintDialog>
#include "db.h"
#include "style.h"
#include "simulate.h"

class InputWidget : public QWidget{
    Q_OBJECT

public:
    InputWidget(QWidget *parent = NULL);
    ~InputWidget();

signals:
    void search(QList<Result *>, QList<int>, int, bool);
    void styleChanged();
    void operate(Simulate);
    void operateEnd();

private:
    void initUI();
    void initConnect();
    bool validate();

private slots:
    void onGoButtonClicked();
    void onPrintButtonClicked();
    void onSimulateButtonClicked();
    void onOperateButtonClicked();
    void onSkipButtonClicked();
    void onComboBoxChanged(int);

private:
    QComboBox *m_styleComboBox;
    QDateEdit *m_fDateEdit;
    QDateEdit *m_tDateEdit;
    QCheckBox *m_checkBox_01;
    QCheckBox *m_checkBox_02;
    QCheckBox *m_checkBox_03;
    QCheckBox *m_checkBox_04;
    QCheckBox *m_checkBox_05;
    QCheckBox *m_checkBox_06;
    QCheckBox *m_checkBox_07;
    QCheckBox *m_checkBox_08;
    QCheckBox *m_checkBox_09;
    QCheckBox *m_checkBox_boll;
    QLineEdit *m_moneyEdit;
    QToolButton *m_goButton;
    QToolButton *m_printButton;
    QToolButton *m_simulateButton;
    QToolButton *m_operateButton;
    QToolButton *m_skipButton;

    double m_money;
    double m_volume;
    QString m_operateDate;

    bool m_simulateStart;
    bool m_operateBuy;

    Db *m_db;
};

#endif // INPUTWIDGET

