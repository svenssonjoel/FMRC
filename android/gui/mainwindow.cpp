#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString uart_service = QString("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const QString uart_tx      = QString("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const QString uart_rx      = QString("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSize s = this->size();

    ui->graphicsView->resize(s.width(),s.height()-10);
    ui->widget->resize(s.width(),s.height()-10);

    mDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    ui->statusLabel->setText("Discovering Devices");

    connect(mDiscoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(addDevice(QBluetoothDeviceInfo)));
    connect(mDiscoveryAgent, SIGNAL(finished()),
            this, SLOT(deviceDiscoveryFinished()));

    mDiscoveryAgent->start();

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()),
            this, SLOT(timeout()));

    mTimer->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addDevice(QBluetoothDeviceInfo info)
{
    //mDevices.append(info);

    QString name = QString("%1 - %2").arg(info.name()).arg(info.address().toString());

    for (int i = 0; i < ui->devicesListWidget->count(); i ++) {
        if (ui->devicesListWidget->item(i)->text() == name) {
            return;
        }
    }

    QListWidgetItem *item = new QListWidgetItem(ui->devicesListWidget);

    item->setData(Qt::UserRole, QVariant::fromValue(info));
    item->setText(name);

    ui->devicesListWidget->addItem(item);



    //mDevices.append(QPair<QBluetoothDeviceInfo,QLowEnergyController*>(info,ctrl));

    //mBLEControl = QLowEnergyController::createCentral(info, this);

    //connect(mBLEControl, &QLowEnergyController::serviceDiscovered,
    //        this, [this](QBluetoothUuid uuid)
    //{
    //    if (uuid == QBluetoothUuid::fromString(uart_service)) {
    //        ui->statusLabel->setText("UART FOUND!");
    //    }
    //});

    //mBLEControl->connectToDevice();

    /*
    connect(mBLEControl, &QLowEnergyController::discoveryFinished,
            this, &MainWindow::bleServiceDiscoveryFinished);
    connect(mBLEControl, &QLowEnergyController::connected, this, [this]() {
        (void)this; qDebug() << "connected to BLE device!";
        mBLEControl->discoverServices();
    });
    */
}

void MainWindow::deviceDiscoveryFinished()  {
    //ui->statusLabel->setText("Discovery finished");
}



void MainWindow::timeout()
{

    double ang;
    double mag;

    ui->widget->getCTRL(&ang, &mag);

    QString sang = QString::number(ang,'f',4);
    QString smag = QString::number(mag,'f',4);

    //QString("%1;%2\n").arg(ang,8,'g',-1,'0').arg(mag,8,'g',-1,'0');
    QString str;

    str.append(sang).append(" ; ").append(smag).append("\n");
    ui->angMagLabel->setText(str);
    //ui->statusLabel->setText(str);

    if (mUartFound) {
        QLowEnergyCharacteristic tx;

        tx = mUartService->characteristic(QBluetoothUuid(uart_tx));

        if (tx.isValid()) {
            QByteArray ba = str.toLocal8Bit();
            while (ba.size() > 0) {
                //qDebug() << "ba = " << QString(ba);
                mUartService->writeCharacteristic(tx, ba.mid(0,20), QLowEnergyService::WriteWithoutResponse);
                ba = ba.mid(20,-1);
            }

        } else {
            mUartFound = false;
        }
    } else {
        connectToBLEUart();

        QString str = QString("Reconnecting: %1").arg(mAttempts);

        ui->statusLabel->setText(str);
    }

    //ui->statusLabel->setText(str);
    //QPoint p = QCursor::pos();
    //QPoint q = ui->widget->mapFromGlobal(p);
    //qDebug() << "TIMEOUT" << q.x() << " " << q.y();

}


void MainWindow::on_discoverPushButton_clicked()
{
       mDiscoveryAgent->start();
}

void MainWindow::on_connectUartPushButton_clicked()
{

    ui->connectUartPushButton->setEnabled(false);

    QListWidgetItem *item = ui->devicesListWidget->currentItem();

    if (!item) {
        ui->statusLabel->setText("No device selected");
        return;
    }

    ui->statusLabel->setText("Checking for UART");

    QBluetoothDeviceInfo info = item->data(Qt::UserRole).value<QBluetoothDeviceInfo>();

    if (mControl) {
        if (mControl->state() == QLowEnergyController::ConnectedState) {
            mControl->disconnectFromDevice();
        }
        mUartFound = false;
        mUartDevice = info;
        delete mControl;
        mControl = nullptr;
    }

    mControl = QLowEnergyController::createCentral(info,this);

    connectToBLEUart();

}

void MainWindow::connectToBLEUart()
{
    if (mUartConnecting || !mControl) {
        return;
    }
    mAttempts ++;
    mUartConnecting = true;



    if (mControl->state() == QLowEnergyController::ConnectedState) {
        mControl->disconnectFromDevice();
        mControl->connectToDevice();
    }

    connect(mControl, &QLowEnergyController::serviceDiscovered,
            this, [this](QBluetoothUuid uuid)
    {
        //qDebug() << mControl->remoteName() << " - " << uuid.toString();
        if (uuid == QBluetoothUuid::fromString(uart_service)) {
            mUartService = mControl->createServiceObject(uuid,this);

            connect(mUartService, &QLowEnergyService::stateChanged,
                    this, [this](QLowEnergyService::ServiceState state)
            {
                switch(state) {
                case QLowEnergyService::InvalidService:
                    mUartConnecting = false;
                    ui->statusLabel->setText("Invalid Service");
                break;
                case QLowEnergyService::DiscoveryRequired:
                    break;
                case QLowEnergyService::DiscoveringServices:
                    break;
                case QLowEnergyService::ServiceDiscovered:
                    ui->statusLabel->setText("UART Discovered!");
                    mUartFound = true;
                    mUartConnecting = false;
                    mAttempts = 0;
                    break;
                case QLowEnergyService::LocalService:
                    mUartConnecting = false;
                    break;
                }
            });
            mUartService->discoverDetails();
        }
    });

    connect(mControl, &QLowEnergyController::connected, this, [this]() {
        (void)this; //qDebug() << "connected to BLE device!";
        mControl->discoverServices();
    });

    connect(mControl, QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error),
          [=](QLowEnergyController::Error newError){
        (void) this;
        mUartConnecting = false;
        mUartFound = false;
        QString str = QString("Error %1").arg(newError);
        ui->statusLabel->setText(str);
    });

    mControl->connectToDevice();

}
