#ifndef SERSOCKET_H
#define SERSOCKET_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <qextserialport/qextserialport.h>
#include "logger.h"

class SerSocket : public QObject
{
    Q_OBJECT
public:
    explicit SerSocket(QObject *parent = 0);
    void setTcpPort(unsigned short port);
    void setSerialPort(const QString &serialPort);
    void setBaudRate(int baudRate);
    void setParity(QChar parity);
    void setStopBits(QChar stopBits);
    void start();
    void end();

signals:
    
public slots:
    void newTcpClient();
    void onNewData();
    void clientDisconnected();
    void serialReadyRead();
    void serialDrsChanged(bool status);
    
private:
    unsigned short m_tcpPort;
    QString m_serialPort;
    BaudRateType m_baudRate;
    ParityType m_parity;
    StopBitsType m_stopBits;

    QList<QTcpSocket *> m_clients;
    QTcpServer * m_server;
    QextSerialPort * m_serial;
};

#endif // SERSOCKET_H
