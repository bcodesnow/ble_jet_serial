#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include <QtBluetooth/qlowenergyadvertisingdata.h>
#include <QtBluetooth/qlowenergyadvertisingparameters.h>
#include <QtBluetooth/qlowenergycharacteristic.h>
#include <QtBluetooth/qlowenergycharacteristicdata.h>
#include <QtBluetooth/qlowenergydescriptordata.h>
#include <QtBluetooth/qlowenergycontroller.h>
#include <QtBluetooth/qlowenergyservice.h>
#include <QtBluetooth/qlowenergyservicedata.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qlist.h>
#include <QtCore/qloggingcategory.h>
#include <QtCore/qscopedpointer.h>
#include <QtCore/qtimer.h>
#include <QObject>
#include <QTimer>
#include "gattsinterface.h"

class GATTSInterface;

class GATTSController : public QObject
{
    Q_OBJECT
public:
    explicit GATTSController(QObject *parent = nullptr);

signals:
    void bleUartRx(QByteArray value);

private:
    const QString BLE_UART_RX_CHAR = "{d773f2e2-b19e-11e2-9e96-0800200c9a66}";
    const QString BLE_UART_TX_CHAR = "{d873f2e1-b19e-11e2-9e96-0800200c9a66}";
    const QString BLE_UART_SERVICE = "{d973f2e0-b19e-11e2-9e96-0800200c9a66}";
    const QStringList BLE_UART_TX_POOL = {
        "{8073f2e2-b19e-11e2-9e96-0800200c9a66}",
        "{8173f2e2-b19e-11e2-9e96-0800200c9a66}",
        "{8273f2e2-b19e-11e2-9e96-0800200c9a66}",
        "{8373f2e2-b19e-11e2-9e96-0800200c9a66}",
        "{8473f2e2-b19e-11e2-9e96-0800200c9a66}",
        "{8573f2e2-b19e-11e2-9e96-0800200c9a66}",
        "{8673f2e2-b19e-11e2-9e96-0800200c9a66}",
        "{8773f2e2-b19e-11e2-9e96-0800200c9a66}",
        "{8873f2e2-b19e-11e2-9e96-0800200c9a66}",
    };

    QLowEnergyAdvertisingData    m_advertisingData;
    QLowEnergyCharacteristicData m_bleUartTxCharData;
    QLowEnergyCharacteristicData m_bleUartRxCharData;
    QLowEnergyServiceData m_serviceData;
    QLowEnergyController* m_controller;
    QScopedPointer<QLowEnergyService> m_bleUartService;
    QLowEnergyCharacteristic m_bleUartTxChar;
    QLowEnergyCharacteristic m_bleUartRxChar;
    GATTSInterface* m_gattsInterface;



public slots:

    void onCharacteristicWritten(const QLowEnergyCharacteristic &c, const QByteArray &value);
    void onDescriptorWritten(const QLowEnergyDescriptor &descriptor, const QByteArray &newValue);
    void onConnected();
    void onDisconnected();
    void bleUartTx(QByteArray value);
    void initializeGattServer(GATTSInterface* refToGattInterface);

};

#endif // SERVERCONTROLLER_H
