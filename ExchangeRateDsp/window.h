#ifndef AD_WINDOW
#define AD_WINDOW

#include <QMainWindow>
#include <QtWidgets>
#include "iconbutton.h"

/*** 窗口标题 ***/
class WindowTitle : public QToolBar{
    Q_OBJECT

public:
    explicit WindowTitle(QWidget *parent = 0);
    ~WindowTitle();

    void setTitle(const QString &);

public slots:
    void _resizeWindow();
    void _showMinimized();
    void _showMaximized();
    void _showNormal();
    void _closeWindow();

protected:
    void mouseDoubleClickEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    void initUI();
    void initConnect();
    QWidget *findParent();

private:
    bool isMaxSize;
    int dragging;
    QLabel *title_label;
    IconButton *minButton;
    IconButton *maxButton;
    IconButton *closeButton;
};

/*** 主窗口 ***/
class Window : public QMainWindow{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

    void setTitle(const QString &);

private:
    void initData();
    void initUI();
    void initConnect();

private:
    WindowTitle *windowTitle;
};

#endif // AD_WINDOW

