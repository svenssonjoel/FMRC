#include "controller.h"
#include <QDebug>
#include <QPoint>
#include <QSize>
#include <QPainter>
#include <QtMath>

Controller::Controller(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_AcceptTouchEvents, true);
    setMouseTracking(true);
    mMousePos = QPoint(0,0);

    QSize s = this->size();

    int m = qMin(s.width(), s.height());

    mOuterRad = m/2;
    mInnerRad = (int) (0.60 * ((float)m/2));

    repaint();
}

void Controller::mouseMoveEvent(QMouseEvent *event)
{
    static int count = 0;
    //qDebug() << "mouse event!" << count ++;

    QPoint p = this->mapFromGlobal(QCursor::pos());

    QSize s = this->size();

    int center_x = s.width() / 2;
    int center_y = s.height() / 2;

    int m = qMin(s.width(), s.height());

    mOuterRad = m/2;
    mInnerRad = (int) (0.60 * (float)(m/2));

    int pos_x = p.x() - center_x;
    int pos_y = p.y() - center_y;

    if ( pos_x <= mOuterRad && pos_x >= -mOuterRad &&
         pos_y <= mOuterRad && pos_y >= -mOuterRad) {

        float mag = qSqrt(pos_x*pos_x + pos_y*pos_y);

        if (mag >= mInnerRad) {
            int s_x = (int)(((float)pos_x / mag) * mInnerRad);
            int s_y = (int)(((float)pos_y / mag) * mInnerRad);

            mMousePos.setX(s_x);
            mMousePos.setY(s_y);
        } else {
            mMousePos.setX(pos_x);
            mMousePos.setY(pos_y);
        }
    } else {
        mMousePos.setX(0);
        mMousePos.setY(0);
    }


    qDebug() << mMousePos.x() << " " << mMousePos.y();
    repaint();

}

void Controller::enterEvent(QEvent *event)
{
    qDebug() << "ENTERING WIDGET";
    repaint();
}

void Controller::leaveEvent(QEvent *event)
{
    qDebug() << "LEAVE EVENT";
    mMousePos = QPoint(0,0);
    repaint();
}

void Controller::paintEvent(QPaintEvent *event)
{
    QSize s = this->size();

    int center_x = s.width() / 2;
    int center_y = s.height() / 2;

    QPainter painter(this);

    QPoint pos = QPoint(center_x + mMousePos.x(),
                        center_y + mMousePos.y());

    painter.setBrush(QBrush(Qt::gray));
    painter.drawEllipse(QPoint(center_x,center_y), mOuterRad, mOuterRad);

    painter.setBrush(QBrush(Qt::darkGray));
    painter.drawEllipse(QPoint(center_x,center_y), mInnerRad, mInnerRad);

    painter.setBrush(QBrush(Qt::black));
    painter.drawEllipse(pos, 25, 25);

}
