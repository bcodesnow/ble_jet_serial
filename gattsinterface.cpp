#include "gattsinterface.h"

GATTSInterface::GATTSInterface(QObject *parent) : QObject(parent)
{

    m_aliveTimer = new QTimer();
    m_aliveTimer->setInterval(2500);

    m_gattsController = new GATTSController();
    m_gattsController->moveToThread(&m_thread_controller);
    connect(this, &GATTSInterface::invokeInitializeGattServer, m_gattsController, &GATTSController::initializeGattServer, Qt::QueuedConnection);

    connect(m_gattsController, &GATTSController::bleUartRx, this, &GATTSInterface::onBleUartRx, Qt::QueuedConnection);
    connect(this, &GATTSInterface::invokeBleUartTx, m_gattsController, &GATTSController::bleUartTx, Qt::QueuedConnection);
    QObject::connect (m_aliveTimer, &QTimer::timeout, this, &GATTSInterface::onAliveTimerExpired);

    m_thread_controller.start();
    emit this->invokeInitializeGattServer(this);
}

void GATTSInterface::onBleUartRx(QByteArray value)
{
    const quint8 *data = reinterpret_cast<const quint8 *>(value.constData());

    switch (data[0])
    {
    case CMD_START:
        bleUartSendCmdOk();
        break;
    case CMD_STOP:
        bleUartSendCmdOk();
        break;
    case CMD_CONNECTION_VALID:
        m_aliveTimer->start();
        bleUartSendCmdOk();
        break;
    default:
        break;
    }

}

void GATTSInterface::onAliveTimerExpired()
{
    QByteArray tba;
    tba.resize(2);
    tba[0] = 0xFF;
    tba[1] = 0xAA;
    invokeBleUartTx(tba);
}

void GATTSInterface::onConnected()
{
    m_aliveTimer->start(); // todo: remove, its debug only
}

void GATTSInterface::onDisconnected()
{
    m_aliveTimer->stop();
}

void GATTSInterface::bleUartSendCmdOk()
{
#if (VERBOSITY_LEVEL >= 1)
    qDebug()<<"Sending CMD OK!";
#endif
    QByteArray tba;
    tba.resize(2);
    tba[0] = CMD_OK;
    tba[1] = 0xFF;
    invokeBleUartTx(tba);
}
