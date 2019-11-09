#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <QtBluetooth/qbluetoothdevicediscoveryagent.h>
#include <QtBluetooth/qlowenergycontroller.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void timeout();

protected:

private:



    Ui::MainWindow *ui;

    QBluetoothDeviceDiscoveryAgent *mDiscoveryAgent = nullptr;
    QLowEnergyController *mBLEControl = nullptr;

    QTimer *mTimer = nullptr;
};
#endif // MAINWINDOW_H
