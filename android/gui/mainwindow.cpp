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
}

MainWindow::~MainWindow()
{
    delete ui;
}

