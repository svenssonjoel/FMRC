#include "controller.h"
#include <QDebug>
#include <QPoint>
#include <QSize>
#include <QPainter>
#include <QtMath>
#include <QObject>

Controller::Controller(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    mMousePos = QPoint(0,0);

    setAttribute(Qt::WA_AcceptTouchEvents, true);


    QSize s = this->size();

    int m = qMin(s.width(), s.height());

    mOuterRad = m/2;
    mInnerRad = (int) (0.60 * ((float)m/2));

    repaint();
}

void Controller::getCTRL(double *ang, double *mag)
{
    QPoint pos = mMousePos;

    *ang = atan2(pos.x(), pos.y());

    double len = sqrt(pos.x()*pos.x() + pos.y()*pos.y());

    if (mInnerRad > 0) {
        *mag = len / mInnerRad;
    } else {
        *mag = 0; // zero speed
    }
}

void Controller::mouseMoveEvent(QMouseEvent *event)
{
    static int count = 0;
    //qDebug() << "mouse event!" << count ++;

    QPoint p = this->mapFromGlobal(QCursor::pos());
    setMousePos(QPointF(p));
}

void Controller::enterEvent(QEvent *event)
{
    //qDebug() << "ENTERING WIDGET";
    repaint();
}

void Controller::leaveEvent(QEvent *event)
{
    //qDebug() << "LEAVE EVENT";
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

    painter.setBrush(QBrush(Qt::white));
    painter.drawEllipse(QPoint(center_x,center_y), 25, 25);

    painter.setBrush(QBrush(Qt::black));
    painter.drawEllipse(pos, 25, 25);

}

bool Controller::event(QEvent *event)
{

    switch (event->type()) {
    case QEvent::TouchBegin:
        //qDebug() << "BEGIN";
    case QEvent::TouchUpdate: {

        QTouchEvent *te = static_cast<QTouchEvent*>(event);

        for (auto tp : te->touchPoints()) {
            switch(tp.state()) {
            case Qt::TouchPointPressed:
                //fall through
            case Qt::TouchPointMoved: {
                QPointF p = tp.pos();
                setMousePos(p);

            } break;
            case Qt::TouchPointReleased:
                setMousePos(QPointF(0,0));
                break;
            }
        }


        event->accept();
        //return QWidget::event(event);
    }break;
    case QEvent::TouchEnd:{
        setMousePos(QPointF(0,0));
        event->accept();
        //qDebug() << "END";
        QTouchEvent *te = static_cast<QTouchEvent*>(event);
        QList<QTouchEvent::TouchPoint> tps = te->touchPoints();
        for (auto e : tps) {
            switch(e.state()) {
            case Qt::TouchPointReleased:
                continue;
            }

        }
        break;
    }
    default:
        return QWidget::event(event);
    }
    return true;
}

void Controller::setMousePos(QPointF p)
{
    QSize s = this->size();

    int center_x = s.width() / 2;
    int center_y = s.height() / 2;

    int m = qMin(s.width(), s.height());

    mOuterRad = m/2;
    mInnerRad = (int) (0.60 * (float)(m/2));

    double pos_x = p.x() - center_x;
    double pos_y = p.y() - center_y;

    float mag = qSqrt(pos_x*pos_x + pos_y*pos_y);

    if (mag <= mOuterRad) {

        if (mag >= mInnerRad) {
            int s_x = (int)(((double)pos_x / mag) * mInnerRad);
            int s_y = (int)(((double)pos_y / mag) * mInnerRad);

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

    //qDebug() << "RAW" << p.x() << " " << p.y();
    //qDebug() << "PRS" << mMousePos.x() << " " << mMousePos.y();
    repaint();
}
