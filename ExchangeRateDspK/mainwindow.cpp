#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
{
    this->initUI();
    this->initConnect();
}

MainWindow::~MainWindow()
{
    delete inputWidget;
    delete resultWidget;
}

void MainWindow::initUI(){
    inputWidget = new InputWidget;

    resultWidget = new ResultWidget;

    mainWidget = new QWidget;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(inputWidget);
    mainLayout->addWidget(resultWidget);
    mainWidget->setLayout(mainLayout);

    this->setCentralWidget(mainWidget);
}

void MainWindow::initConnect(){

}
