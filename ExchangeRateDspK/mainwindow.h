#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "inputwidget.h"
#include "graphresultwidget.h"
#include "dataresultwidget.h"

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
    InputWidget *m_inputWidget;
    QDockWidget *m_inputDockWidget;

    DataResultWidget *m_dataResultWidget;
    QDockWidget *m_dataResultDockWidget;

    GraphResultWidget *m_graphResultWidget;
};

#endif // MAINWINDOW_H
