#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSize s = this->size();

    ui->graphicsView->resize(s.width(),s.height()-10);
    ui->widget->resize(s.width(),s.height()-10);

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()),
            this, SLOT(timeout()));

    mTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timeout()
{
    QPoint p = QCursor::pos();
    QPoint q = ui->widget->mapFromGlobal(p);
    qDebug() << "TIMEOUT" << q.x() << " " << q.y();

}



