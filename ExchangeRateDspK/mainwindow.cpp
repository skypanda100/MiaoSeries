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
    delete m_historyInputDockWidget;
    delete m_resultWidget;
}

void MainWindow::initUI(){
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect clientRect = desktopWidget->availableGeometry();
    int MAINWIDTH = clientRect.width();
    int MAINHEIGHT = clientRect.height() - 60;
    this->setMinimumSize(MAINWIDTH, MAINHEIGHT);

    m_inputWidget = new InputWidget;
    m_historyInputDockWidget = new QDockWidget("History", this);
    m_historyInputDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_historyInputDockWidget->setWidget(m_inputWidget);
    this->addDockWidget(Qt::LeftDockWidgetArea, m_historyInputDockWidget);

    m_resultWidget = new ResultWidget;
    this->setCentralWidget(m_resultWidget);

    this->move(0, 0);
}

void MainWindow::initConnect(){
    connect(m_inputWidget
            , SIGNAL(search(QList<ExchangeRateResult*>, QList<int>, int, bool))
            , m_resultWidget
            , SLOT(onSearch(QList<ExchangeRateResult*>, QList<int>, int, bool)));

    connect(m_inputWidget
            , SIGNAL(styleChanged())
            , m_resultWidget
            , SLOT(onStyleChanged()));
}
