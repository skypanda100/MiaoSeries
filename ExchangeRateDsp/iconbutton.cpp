#include "iconbutton.h"
#include <QDebug>

IconButton::IconButton(QWidget *parent)
    :QToolButton(parent)
{
    status = NORMAL;
    setCursor(Qt::PointingHandCursor);
    isPress = false;
    isSinkable = false;
    backgroundColor = QColor(150, 150, 150);
    setContentsMargins(0, 0, 0, 0);
}

IconButton::~IconButton()
{

}

void IconButton::setSource(QString src)
{
    this->iconW = QPixmap(src).width();
    this->iconH = QPixmap(src).height();
    this->rectW = this->iconW;
    this->rectH = this->iconH;
    this->setMinimumSize(this->rectW, this->rectH);
    this->setMaximumSize(this->rectW, this->rectH);
    setPixmap(src);
    update();
}

void IconButton::setSource(QString src, int iconW, int iconH, int rectW, int rectH)
{
    this->iconW = iconW;
    this->iconH = iconH;
    this->rectW = rectW;
    this->rectH = rectH;
    this->setMinimumSize(this->rectW, this->rectH);
    this->setMaximumSize(this->rectW, this->rectH);
    setPixmap(src);
    update();
}

void IconButton::setPixmap(QString src){
    pixmap.load(src);
}

void IconButton::setSinkable(bool isSinkable){
    this->isSinkable = isSinkable;
}

void IconButton::setBackgroundColor(QColor color){
    backgroundColor = color;
}

void IconButton::enterEvent(QEvent *)
{
    if(!(isSinkable && isPress)){
        status = ENTER;
    }
    update();
}

void IconButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(isSinkable){
            if(isPress){
                isPress = false;
                status = ENTER;
            }else{
                isPress = true;
                status = PRESS;
            }
        }else{
            isPress = true;
            status = PRESS;
        }
        update();
        QToolButton::mousePressEvent(event);
    }
}

void IconButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(!isSinkable && isPress && this->rect().contains(event->pos()))
    {
        isPress = false;
        status = ENTER;
        update();
        emit clicked(true);
    }else if(isSinkable && isPress){
        emit clicked(true);
    }else if(isSinkable && !isPress){
        emit clicked(false);
    }
}

void IconButton::leaveEvent(QEvent *)
{
    if(!(isPress && isSinkable)){
        status = NORMAL;
    }
    update();
}

void IconButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    qreal iconX = rectW / 2.0 - iconW / 2.0;
    qreal iconY = rectH / 2.0 - iconH / 2.0;

    QRectF rect(0.0, 0.0, iconW, iconH);

    if(isEnabled()){
        switch(status)
        {
        case NORMAL:
            {
                paintBackground(painter, 0, 0, 0);
                break;
            }
        case ENTER:
            {
                paintBackground(painter, 150, 100, 150);
                break;
            }
        case PRESS:
            {
                paintBackground(painter, 200, 150, 200);
                break;
            }
        default:
            paintBackground(painter, 0, 0, 0);
        }

        painter.translate(iconX, iconY);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.drawPixmap(rect, pixmap, QRectF(0.0, 0.0, pixmap.width(), pixmap.height()));
    }else{
        QPixmap temp(pixmap.size());
        temp.fill(Qt::transparent);
        QPainter tempPainter(&temp);
        tempPainter.drawPixmap(0, 0, pixmap);
        tempPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        tempPainter.fillRect(temp.rect(), QColor(35, 35, 35));

        painter.translate(iconX, iconY);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.drawPixmap(rect, temp, QRectF(0.0, 0.0, temp.width(), temp.height()));
    }
}

void IconButton::paintBackground(QPainter &painter, int tcolor, int mcolor, int bcolor){
    if(backgroundColor.alpha() == 0) return;
    painter.save();
    QPen pen(Qt::NoBrush, 1);
    painter.setPen(pen);
    QLinearGradient linear(QPointF(0.0, 0.0), QPointF(0.0, rectH));
    linear.setColorAt(0, QColor(backgroundColor.red(), backgroundColor.green(), backgroundColor.blue(), tcolor));
    linear.setColorAt(0.5, QColor(backgroundColor.red(), backgroundColor.green(), backgroundColor.blue(), mcolor));
    linear.setColorAt(1, QColor(backgroundColor.red(), backgroundColor.green(), backgroundColor.blue(), bcolor));
    painter.setBrush(linear);
    painter.drawRoundRect(QRectF(0.0, 0.0, rectW, rectH), 0.0, 0.0);
    painter.restore();
}

void IconButton::setCheckStatus(bool isChecked){
    if(isChecked){
        isPress = true;
        status = PRESS;
    }else{
        isPress = false;
        status = NORMAL;
    }
    update();
}
