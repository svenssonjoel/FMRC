#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <QtBluetooth/qbluetoothdevicediscoveryagent.h>
#include <QtBluetooth/qlowenergycontroller.h>
#include <qbluetoothaddress.h>
#include <qbluetoothservicediscoveryagent.h>
#include <qbluetoothserviceinfo.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothuuid.h>
#include <qbluetoothsocket.h>
#include <qlowenergycontroller.h>
#include <qlowenergyservice.h>
#include <qlowenergycharacteristic.h>
#include <qlowenergycharacteristicdata.h>

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
    void addDevice(QBluetoothDeviceInfo info);
    void deviceDiscoveryFinished();
    void timeout();
    void on_discoverPushButton_clicked();
    void on_connectUartPushButton_clicked();

protected:

private:
    void connectToBLEUart();

    bool mUartFound = false;
    bool mUartConnecting = false;
    int mAttempts = 0;
    QBluetoothDeviceInfo mUartDevice;
    QLowEnergyService *mUartService;

    Ui::MainWindow *ui;

    QBluetoothDeviceDiscoveryAgent *mDiscoveryAgent = nullptr;
    QLowEnergyController *mControl = nullptr;
    QTimer *mTimer = nullptr;

};
#endif // MAINWINDOW_H
