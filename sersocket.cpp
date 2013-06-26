#include "sersocket.h"

SerSocket::SerSocket(QObject *parent) :
    QObject(parent)
{
    m_tcpPort = 5331;
    m_serialPort = QString("/dev/ttyUSB0");
    m_baudRate = BAUD115200;
    m_parity = PAR_NONE;
    m_stopBits = STOP_1;
}

void SerSocket::setTcpPort(unsigned short port)
{
    m_tcpPort = port;
}

void SerSocket::setMaxClients(int max_clients)
{
    m_maxTcpClients = max_clients;
}

void SerSocket::setSerialPort(const QString &serialPort)
{
    m_serialPort = serialPort;
}

void SerSocket::setBaudRate(int baudRate)
{
    if (baudRate == 110) {
        m_baudRate = BAUD110;
    } else if (baudRate == 300) {
        m_baudRate = BAUD300;
    } else if (baudRate == 600) {
        m_baudRate = BAUD600;
    } else if (baudRate == 1200) {
        m_baudRate = BAUD1200;
    } else if (baudRate == 2400) {
        m_baudRate = BAUD2400;
    } else if (baudRate == 4800) {
        m_baudRate = BAUD4800;
    } else if (baudRate == 9600) {
        m_baudRate = BAUD9600;
    } else if (baudRate == 19200) {
        m_baudRate = BAUD19200;
    } else if (baudRate == 38400) {
        m_baudRate = BAUD38400;
    } else if (baudRate == 57600) {
        m_baudRate = BAUD57600;
    } else {
        m_baudRate = BAUD115200;
    }
}

void SerSocket::setParity(QChar parity)
{
    if (parity == 'N') {
        m_parity = PAR_NONE;
    } else if (parity == 'E') {
        m_parity = PAR_EVEN;
    } else if (parity == 'O') {
        m_parity = PAR_ODD;
    }
}

void SerSocket::setStopBits(QChar stopBits)
{
    if (stopBits == '1') {
        m_stopBits = STOP_1;
    } else if (stopBits == '2') {
        m_stopBits = STOP_2;
    }
}

void SerSocket::start()
{
    m_server = new QTcpServer(this);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newTcpClient()));
    Logger::info(QString("Starting Tcp Server on port %1").arg(m_tcpPort));
    if (m_server->listen(QHostAddress::Any, m_tcpPort)) {
        Logger::debug("Server Listening");
    } else {
        Logger::error("Server could not be started");
    }

    m_serial = new QextSerialPort(m_serialPort, QextSerialPort::EventDriven);
    m_serial->setBaudRate(m_baudRate);
    m_serial->setParity(m_parity);
    m_serial->setStopBits(m_stopBits);
    Logger::info(QString("Opening Serial Port %1").arg(m_serialPort));
    if (m_serial->open(QIODevice::ReadWrite)) {
        Logger::debug("Device opened, listening for data");
        connect(m_serial, SIGNAL(readyRead()), this, SLOT(serialReadyRead()));
        connect(m_serial, SIGNAL(dsrChanged(bool)), this, SLOT(serialDsrChanged(bool)));
    } else {
        Logger::debug(QString("Device failed to open: %1").arg(m_serial->errorString()));
    }
}

void SerSocket::end()
{
}

void SerSocket::newTcpClient()
{
    QTcpSocket * client = m_server->nextPendingConnection();
    if ((m_maxTcpClients == 0) || (m_clients.size() < m_maxTcpClients)) {
        connect(client, SIGNAL(readyRead()), this, SLOT(onNewData()));
        connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
        m_clients.append(client);
        Logger::debug(QString("New client attached, %1 client(s) now connected").arg(m_clients.size()));
    } else {
        client->close();
        Logger::warn(QString("New client attached, but max clients reached. Closing Socket. %1 client(s) already").arg(m_clients.size()));
    }
}

void SerSocket::onNewData()
{
    Logger::debug("New data received on Tcp Socket");
    QTcpSocket *client = (QTcpSocket*)QObject::sender();
    QByteArray data = client->readAll();
    Logger::debug(QString("Read data %1").arg(QString(data)));
    m_serial->write(data);
//    foreach (QTcpSocket *c, m_clients) {
//        c->write(data);
//    }
}


void SerSocket::clientDisconnected()
{
    QTcpSocket *client = (QTcpSocket*)QObject::sender();
    m_clients.removeAll(client);
    Logger::debug(QString("Client Disconnected, %1 client(s) still connected").arg(m_clients.size()));
}

void SerSocket::serialReadyRead()
{
    QByteArray bytes;
    int a = m_serial->bytesAvailable();
    bytes.resize(a);
    m_serial->read(bytes.data(), bytes.size());

    Logger::debug(QString("Read %1 bytes.").arg(bytes.size()));

    foreach (QTcpSocket *c, m_clients) {
        c->write(bytes);
    }
}

void SerSocket::serialDsrChanged(bool status)
{
    if (status) {
        Logger::info(QString("Device on %1 has been turned on").arg(m_serialPort));
    } else {
        Logger::info(QString("Device on %1 has been turned off").arg(m_serialPort));
    }
}
