#ifndef INPUTWIDGET
#define INPUTWIDGET

#include <QtWidgets>

class InputWidget : public QWidget{
    Q_OBJECT

public:
    InputWidget(QWidget *parent = NULL);
    ~InputWidget();

private:
    void initUI();
    void initConnect();
    bool validate();

private slots:
    void onButtonClicked();

private:
    QDateEdit *m_fDateEdit;
    QDateEdit *m_tDateEdit;
    QCheckBox *m_checkBox_01;
    QCheckBox *m_checkBox_02;
    QCheckBox *m_checkBox_03;
    QCheckBox *m_checkBox_04;
    QCheckBox *m_checkBox_05;
    QCheckBox *m_checkBox_06;
    QCheckBox *m_checkBox_07;
    QPushButton *m_goButton;
};

#endif // INPUTWIDGET

