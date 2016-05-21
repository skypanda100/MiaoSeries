#ifndef AD_ICONBUTTON
#define AD_ICONBUTTON

#include <QToolButton>
#include <QPainter>
#include <QMouseEvent>

class IconButton : public QToolButton
{
    Q_OBJECT

public:

    explicit IconButton(QWidget *parent = 0);
    ~IconButton();

    void setSource(QString);
    void setSource(QString, int, int, int, int);
    void setSinkable(bool);
    void setBackgroundColor(QColor);
    void setCheckStatus(bool);

protected:

    void enterEvent(QEvent *) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:

    void paintBackground(QPainter &, int, int, int);
    void setPixmap(QString);

private:

    QColor backgroundColor;

    enum ButtonStatus{NORMAL, ENTER, PRESS};
    ButtonStatus status;
    QPixmap pixmap;

    int iconW;
    int iconH;
    int rectW;
    int rectH;
    bool isPress;

    bool isSinkable;
};

#endif // AD_ICONBUTTON

