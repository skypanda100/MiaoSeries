#include "mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->initUI();
    this->initConnect();
}

MainWindow::~MainWindow()
{
    delete startButton;
    delete stopButton;
    delete eaNet;
    delete quitAction;
    delete trayiconMenu;
    delete trayicon;
}

void MainWindow::initUI(){
    setFixedSize(300, 80);

    trayicon = new QSystemTrayIcon(this);
    QIcon icon(":/images/tip.png");
    trayiconMenu = new QMenu(this);
    quitAction = new QAction(this);
    quitAction->setText("退出");
    trayiconMenu->addAction(quitAction);
    trayiconMenu->addSeparator();

    trayicon->setIcon(icon);
    trayicon->show();
    trayicon->setToolTip("汇率抓取");
    trayicon->setContextMenu(trayiconMenu);

    startButton = new QPushButton;
    startButton->setFixedSize(100, 30);
    startButton->setText("启动");

    stopButton = new QPushButton;
    stopButton->setFixedSize(100, 30);
    stopButton->setText("停止");
    stopButton->setDisabled(true);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->addWidget(startButton, 0, Qt::AlignVCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(stopButton, 0, Qt::AlignVCenter);

    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);

    setCentralWidget(mainWidget);

    eaNet = new ExchangeRateNet;
}

void MainWindow::initConnect(){
    connect(startButton, SIGNAL(clicked()), this, SLOT(onStartClicked()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(onStopClicked()));
    connect(this, SIGNAL(startService()), eaNet, SLOT(startService()));
    connect(this, SIGNAL(stopService()), eaNet, SLOT(stopService()));
    connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(onQuitActionTriggered()));
    connect(eaNet, SIGNAL(showMessage(QString)), this, SLOT(showMessage(QString)));
}

void MainWindow::onStartClicked(){
    startButton->setDisabled(true);
    stopButton->setDisabled(false);
    emit startService();
}

void MainWindow::onStopClicked(){
    startButton->setDisabled(false);
    stopButton->setDisabled(true);
    emit stopService();
}

void MainWindow::onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    //单击
    case QSystemTrayIcon::Trigger:
        //双击
    case QSystemTrayIcon::DoubleClick:
        //恢复窗口显示
        this->setWindowState(Qt::WindowActive);
        this->show();
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  if(trayicon->isVisible())
  {
      hide();
      event->ignore();
  }
}

void MainWindow::onQuitActionTriggered(){
    qApp->quit();
}

void MainWindow::showMessage(QString message){
    trayicon->showMessage("汇率抓取服务", message, QSystemTrayIcon::Critical, 10000);
}
