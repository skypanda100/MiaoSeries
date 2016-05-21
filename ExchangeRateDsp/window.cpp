#include "window.h"
#include <QHBoxLayout>
#include <windows.h>
#include <windowsx.h>
#include <QDebug>

/*** 窗口标题 ***/
WindowTitle::WindowTitle(QWidget *parent)
    : QToolBar(parent)
{
    this->initUI();
    this->initConnect();
}

WindowTitle::~WindowTitle(){
    delete minButton;
    delete maxButton;
    delete closeButton;
    delete title_label;
}

void WindowTitle::initUI(){
    //设置字体颜色及背景色

    this->setFloatable(false);
    this->setMovable(false);

    isMaxSize = false;
    dragging = -1;

    QFont font;
    font.setFamily(QStringLiteral("微软雅黑"));
    font.setPixelSize(15);

    title_label = new QLabel;
    font.setPixelSize(12);
    title_label->setFont(font);
    QLabel *title_icon_label = new QLabel;
    QPixmap title_pixmap(":/images/review.png");
    title_icon_label->setPixmap(title_pixmap);
    title_icon_label->setFixedSize(20, 20);
    title_icon_label->setScaledContents(true);
    minButton = new IconButton;
    minButton->setSource(QString(":/images/min.png"), 10, 10, 30, 20);
    minButton->setContentsMargins(0, 0, 0, 0);
    minButton->setCursor(Qt::ArrowCursor);

    maxButton = new IconButton;
    if(isMaxSize){
        maxButton->setSource(QString(":/images/max_normal.png"), 10, 10, 30, 20);
    }else{
        maxButton->setSource(QString(":/images/max_max.png"), 10, 10, 30, 20);
    }
    maxButton->setContentsMargins(0, 0, 0, 0);
    maxButton->setCursor(Qt::ArrowCursor);

    closeButton = new IconButton;
    closeButton->setSource(QString(":/images/close.png"), 10, 10, 30, 20);
    closeButton->setBackgroundColor(QColor(255, 0, 0));
    closeButton->setContentsMargins(0, 0, 0, 0);
    closeButton->setCursor(Qt::ArrowCursor);

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->addWidget(title_icon_label, 0, Qt::AlignVCenter);
    titleLayout->addSpacing(5);
    titleLayout->addWidget(title_label, 0, Qt::AlignVCenter);
    titleLayout->addStretch();
    titleLayout->addWidget(minButton, 0, Qt::AlignVCenter);
    titleLayout->addWidget(maxButton, 0, Qt::AlignVCenter);
    titleLayout->addWidget(closeButton, 0, Qt::AlignVCenter);

    QWidget *titleWidget = new QWidget;
    titleWidget->setLayout(titleLayout);
    this->addWidget(titleWidget);
}

void WindowTitle::initConnect(){
    connect(minButton, SIGNAL(clicked()), this, SLOT(_showMinimized()));
    connect(maxButton, SIGNAL(clicked()), this, SLOT(_resizeWindow()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(_closeWindow()));
}

QWidget *WindowTitle::findParent(){
    QWidget *temp = this;
    QWidget *parent = temp;

    while((temp = temp->parentWidget()) != 0){
        parent = temp;
    }

    temp = 0;

    return parent;
}

void WindowTitle::_resizeWindow(){
        if(isMaxSize){
            isMaxSize = false;
            maxButton->setSource(QString(":/images/max_max.png"), 10, 10, 30, 20);
        _showNormal();
        }else{
            isMaxSize = true;
            maxButton->setSource(QString(":/images/max_normal.png"), 10, 10, 30, 20);
        _showMaximized();
        }
}

void WindowTitle::mouseDoubleClickEvent(QMouseEvent *){
    _resizeWindow();
}

void WindowTitle::mousePressEvent(QMouseEvent *event){
    if(Qt::LeftButton == event->button()){
        if (ReleaseCapture()){
            QWidget *parent = findParent();
            SendMessage(HWND(parent->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
            dragging = 0;
        }
    }
}

void WindowTitle::mouseMoveEvent(QMouseEvent *){
    if(dragging == 0){
        dragging += 1;
    }
}

void WindowTitle::mouseReleaseEvent(QMouseEvent *){
    if(dragging == 1){
        isMaxSize = false;
        maxButton->setSource(QString(":/images/max_max.png"), 10, 10, 30, 20);
        dragging = -1;
    }
}

void WindowTitle::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setPen(QColor(75, 75, 75));
    painter.drawLine(QPointF(0, 0), QPointF(this->width(), 0));
}

void WindowTitle::_showMinimized(){
    findParent()->setWindowState(Qt::WindowMinimized);
}

void WindowTitle::_showMaximized(){
    findParent()->setWindowState(Qt::WindowMaximized);
    }

void WindowTitle::_showNormal(){
    findParent()->setWindowState(Qt::WindowNoState);
}

void WindowTitle::_closeWindow(){
    findParent()->close();
}

void WindowTitle::setTitle(const QString &title){
    this->title_label->setText(title);
}

/*** 主窗口 ***/
Window::Window(QWidget *parent) :
    QMainWindow(parent)
{
    this->initData();
    this->initUI();
    this->initConnect();
}

Window::~Window()
{
    delete windowTitle;
}

void Window::initData()
{
}

void Window::initUI(){
    //设置背景色
    setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(50, 50, 50));
    setPalette(palette);
    this->setContentsMargins(2, 1, 2, 1);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    windowTitle = new WindowTitle;
    windowTitle->setFixedHeight(30);
    this->addToolBar(Qt::TopToolBarArea, windowTitle);
//    this->statusBar();
}

void Window::initConnect(){
}

void Window::setTitle(const QString &title){
    this->windowTitle->setTitle(title);
}
