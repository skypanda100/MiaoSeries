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
    delete m_inputDockWidget;
    delete m_dataResultWidget;
    delete m_dataResultDockWidget;

    delete m_graphResultWidget;
}

void MainWindow::initUI(){
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect clientRect = desktopWidget->availableGeometry();
    int MAINWIDTH = clientRect.width();
    int MAINHEIGHT = clientRect.height();
    this->setMinimumSize(MAINWIDTH, MAINHEIGHT);

    m_inputWidget = new InputWidget;
    m_inputDockWidget = new QDockWidget("Input", this);
    m_inputDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_inputDockWidget->setWidget(m_inputWidget);
    m_inputDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    this->addDockWidget(Qt::LeftDockWidgetArea, m_inputDockWidget);

    m_dataResultWidget = new DataResultWidget;
    m_dataResultDockWidget = new QDockWidget("Data", this);
    m_dataResultDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_dataResultDockWidget->setWidget(m_dataResultWidget);
    m_dataResultDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    this->addDockWidget(Qt::LeftDockWidgetArea, m_dataResultDockWidget);

    m_graphResultWidget = new GraphResultWidget;
    this->setCentralWidget(m_graphResultWidget);

    this->setDockOptions(QMainWindow::AnimatedDocks);

    this->move(0, 0);
}

void MainWindow::initConnect(){
    connect(m_inputWidget
            , SIGNAL(search(QList<Result*>, QList<int>, int, bool))
            , m_graphResultWidget
            , SLOT(onSearch(QList<Result*>, QList<int>, int, bool)));

    connect(m_inputWidget
            , SIGNAL(styleChanged())
            , m_graphResultWidget
            , SLOT(onStyleChanged()));

    connect(m_inputWidget
            , SIGNAL(operate(Simulate))
            , m_dataResultWidget
            , SLOT(onOperate(Simulate)));

    connect(m_inputWidget
            , SIGNAL(operateEnd())
            , m_dataResultWidget
            , SLOT(onOperateEnd()));


}
