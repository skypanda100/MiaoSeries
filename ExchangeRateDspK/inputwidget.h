#ifndef INPUTWIDGET
#define INPUTWIDGET

#include <QtWidgets>
#include <QPrinter>
#include <QPrintDialog>
#include "ea_db.h"
#include "style.h"

class InputWidget : public QWidget{
    Q_OBJECT

public:
    InputWidget(QWidget *parent = NULL);
    ~InputWidget();

signals:
    void search(QList<ExchangeRateResult *>, QList<int>, int, bool);
    void styleChanged();

private:
    void initUI();
    void initConnect();
    bool validate();

private slots:
    void onGoButtonClicked();
    void onPrintButtonClicked();
    void onSimulateButtonClicked();
    void onOperateButtonClicked();
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

    bool m_simulateStart;
    bool m_operateBuy;

    ExchangeServiceDb *m_db;
};

#endif // INPUTWIDGET

