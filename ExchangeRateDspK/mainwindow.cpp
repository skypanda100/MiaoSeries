#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
{
    resultWidget = new ResultWidget;
    this->setCentralWidget(resultWidget);
}

MainWindow::~MainWindow()
{
    delete resultWidget;
}
