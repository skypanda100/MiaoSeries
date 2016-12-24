#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
{
    this->initUI();
    this->initConnect();
}

MainWindow::~MainWindow()
{
    delete m_inputWidget;
    delete m_resultWidget;
}

void MainWindow::initUI(){
    m_inputWidget = new InputWidget;

    m_resultWidget = new ResultWidget;

    m_mainWidget = new QWidget;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_inputWidget);
    mainLayout->addWidget(m_resultWidget);
    m_mainWidget->setLayout(mainLayout);

    this->setCentralWidget(m_mainWidget);
}

void MainWindow::initConnect(){
    connect(m_inputWidget, SIGNAL(search(QList<ExchangeRateResult*>, QList<int>, int)), m_resultWidget, SLOT(onSearch(QList<ExchangeRateResult*>, QList<int>, int)));
}
