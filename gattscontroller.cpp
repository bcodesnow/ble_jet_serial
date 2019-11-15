#include "gattscontroller.h"

GATTSController::GATTSController(QObject *parent) : QObject(parent),  m_bleUartTxChar(), m_bleUartRxChar()
{

}

void GATTSController::onCharacteristicWritten(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    qDebug()<<"onCharacteristicWritten()"<<*value;
    emit bleUartTx(value);
}

void GATTSController::onDescriptorWritten(const QLowEnergyDescriptor &descriptor, const QByteArray &newValue)
{
    qDebug()<<"onDescriptorWritten()"<<"Notification enabled!";
}

void GATTSController::onConnected()
{
    qDebug()<<"onConnected()";
}

void GATTSController::onDisconnected()
{
    qDebug()<<"onDisconnected()";

    m_bleUartService.reset(m_controller->addService(m_serviceData));
    if (!m_bleUartService.isNull())
    {
        m_controller->startAdvertising(QLowEnergyAdvertisingParameters(),
                                       m_advertisingData, m_advertisingData);
        QObject::connect (m_bleUartService.data(), &QLowEnergyService::characteristicChanged, this, &GATTSController::onCharacteristicWritten);
        QObject::connect (m_bleUartService.data(), &QLowEnergyService::descriptorWritten, this, &GATTSController::onDescriptorWritten);
    }
}

void GATTSController::bleUartTx(QByteArray value)
{
    m_bleUartService->writeCharacteristic(m_bleUartTxChar, value); // Potentially causes notification.
    qDebug()<<"bleUartTx";
}

void GATTSController::initializeGattServer(GATTSInterface* refToGattInterface)
{
    qInfo()<<"initializeGattServer()";
    m_gattsInterface = refToGattInterface;

    //QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));
    //! [Advertising Data]
    m_advertisingData.setDiscoverability(QLowEnergyAdvertisingData::DiscoverabilityGeneral);
    m_advertisingData.setIncludePowerLevel(true);
    m_advertisingData.setLocalName("Stat?!");
    m_advertisingData.setServices(QList<QBluetoothUuid>() << QBluetoothUuid(BLE_UART_SERVICE));
    //! [Advertising Data]

    //! [Service Data]

    m_bleUartTxCharData.setUuid(QBluetoothUuid(BLE_UART_TX_CHAR));

    //m_bleUartTxCharData.setValue(QByteArray(2, 0));
    m_bleUartTxCharData.setValueLength(0,23);
    m_bleUartTxCharData.setProperties(QLowEnergyCharacteristic::Notify);
    const  QLowEnergyDescriptorData bleUartTxDescData (QBluetoothUuid::ClientCharacteristicConfiguration,
                                                       QByteArray(2, 0));
    m_bleUartTxCharData.addDescriptor(bleUartTxDescData);

    m_bleUartRxCharData.setUuid(QBluetoothUuid(BLE_UART_RX_CHAR));
    m_bleUartRxCharData.setValue(QByteArray(2, 0));
    m_bleUartRxCharData.setProperties(QLowEnergyCharacteristic::WriteNoResponse  | QLowEnergyCharacteristic::Write );


    // i think we need to include some std gap service too

    m_serviceData.setType(QLowEnergyServiceData::ServiceTypePrimary);
    m_serviceData.setUuid(QBluetoothUuid(BLE_UART_SERVICE));
    m_serviceData.addCharacteristic(m_bleUartTxCharData);
    m_serviceData.addCharacteristic(m_bleUartRxCharData);
    //! [Service Data]

    //! [Start Advertising]
    //! m

    m_controller = QLowEnergyController::createPeripheral();
    m_bleUartService.reset( m_controller->addService(m_serviceData) );

    // todo add params
    m_controller->startAdvertising(QLowEnergyAdvertisingParameters(),
                                   m_advertisingData,
                                   m_advertisingData);
    m_bleUartTxChar = m_bleUartService->characteristic(QBluetoothUuid(BLE_UART_TX_CHAR));
    //! [Start Advertising]
    //!

    QObject::connect (m_controller, &QLowEnergyController::disconnected, this, &GATTSController::onDisconnected);
    QObject::connect (m_controller, &QLowEnergyController::connected, this, &GATTSController::onConnected);

    QObject::connect (m_controller, &QLowEnergyController::disconnected, m_gattsInterface, &GATTSInterface::onDisconnected);
    QObject::connect (m_controller, &QLowEnergyController::connected, m_gattsInterface, &GATTSInterface::onConnected);

    // CAN BE LATER A PROBLEM SOURCE -> bleuart service is a qscoped pointer
    QObject::connect (m_bleUartService.data(), &QLowEnergyService::characteristicChanged, this, &GATTSController::onCharacteristicWritten);
    QObject::connect (m_bleUartService.data(), &QLowEnergyService::descriptorWritten, this, &GATTSController::onDescriptorWritten);

}


