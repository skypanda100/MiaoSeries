#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "inputwidget.h"
#include "resultwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initUI();
    void initConnect();

private:
    InputWidget *inputWidget;
    ResultWidget *resultWidget;
    QWidget *mainWidget;
};

#endif // MAINWINDOW_H
