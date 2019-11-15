#ifndef GATTSINTERFACE_H
#define GATTSINTERFACE_H

#include <QObject>
#include <QThread>
#include "ble_uart.h"
#include "gattscontroller.h"

class GATTSController;

class GATTSInterface : public QObject
{
    Q_OBJECT
public:
    explicit GATTSInterface(QObject *parent = nullptr);

signals:
void invokeBleUartTx (QByteArray value);
void invokeInitializeGattServer(GATTSInterface* refToGattInterface);

private:
    QThread m_thread_controller;
    GATTSController* m_gattsController;
    QTimer* m_aliveTimer;

public slots:
    void onBleUartRx(QByteArray value);
    void onAliveTimerExpired();
    void onConnected();
    void onDisconnected();
    void bleUartSendCmdOk();
};

#endif // GATTSINTERFACE_H
