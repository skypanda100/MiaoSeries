#include "resultwidget.h"

TabBar::TabBar(QWidget *parent)
    : QTabBar(parent)
{
    setAcceptDrops(true);
    setMovable(true);
    setTabsClosable(true);
    connect(this, SIGNAL(tabCloseRequested(int)),
            this, SIGNAL(closeTab(int)));
}

TabBar::~TabBar(){

}

void TabBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_dragStartPos = event->pos();
    QTabBar::mousePressEvent(event);
}

void TabBar::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        int diffX = event->pos().x() - m_dragStartPos.x();
        int diffY = event->pos().y() - m_dragStartPos.y();
        if ((event->pos() - m_dragStartPos).manhattanLength() > QApplication::startDragDistance()
            && diffX < 3 && diffX > -3
            && diffY < -10) {
            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;
            QList<QUrl> urls;
            int index = tabAt(event->pos());
            QUrl url = tabData(index).toUrl();
            urls.append(url);
            mimeData->setUrls(urls);
            mimeData->setText(tabText(index));
            mimeData->setData(QLatin1String("action"), "tab-reordering");
            drag->setMimeData(mimeData);
            drag->exec();
        }
    }
    QTabBar::mouseMoveEvent(event);
}

ResultWidget::ResultWidget(QWidget *parent)
    :QTabWidget(parent)
{
    this->initUI();
    this->initConnect();
}

ResultWidget::~ResultWidget(){
    delete m_tabBar;
}

void ResultWidget::initUI(){
//    //设置背景色
//    this->setAutoFillBackground(true);
//    this->setPalette(QPalette(QColor(224, 220, 216)));

    setElideMode(Qt::ElideRight);

    //标签栏
    m_tabBar = new TabBar(this);
    setTabBar(m_tabBar);
    setDocumentMode(true);
}

void ResultWidget::initConnect(){
    connect(m_tabBar, SIGNAL(closeTab(int)), this, SLOT(closeTab(int)));
}

void ResultWidget::setChildSize(QSize size){
    this->childSize = size;
}

void ResultWidget::addGraphWidget(QString title, QList<ExchangeRateResult *> ea_results){
    GraphWidget *graphWidget = new GraphWidget(childSize);
    graphWidget->onSearchResult(ea_results);
    graphWidget->setTitle(title);
    connect(graphWidget, SIGNAL(sendDetail(QStringList,QStringList,int)), this, SIGNAL(sendDetail(QStringList,QStringList,int)));
    this->addTab(graphWidget, title);
    this->setCurrentWidget(graphWidget);
}

void ResultWidget::closeTab(int index){
    if (index < 0)
        index = currentIndex();
    if (index < 0 || index >= count())
        return;
    //先释放
    GraphWidget *graphWidget = (GraphWidget *)(this->widget(index));
    delete graphWidget;
    //释放后，再从TabWidget删除，才能释放内存
    removeTab(index);
}
