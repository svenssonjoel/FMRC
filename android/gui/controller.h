#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QPaintEvent>
#include <QWidget>

class Controller : public QWidget
{
    Q_OBJECT
public:
    explicit Controller(QWidget *parent = nullptr);

signals:

public slots:    

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    void paintEvent(QPaintEvent *event);

private:
    QPoint mMousePos;

    int mOuterRad;
    int mInnerRad;
};

#endif // CONTROLLER_H
