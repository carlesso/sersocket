#include <QCoreApplication>
#include <QSettings>
#include <QStringList>
#include <QString>
#include <QDebug>
#include <QFile>
#include "sersocket.h"
#include "logger.h"

void usage() {
    printf("usage: sersocket [-f configfile.ini]\n");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();
    QString settingsFile("/etc/sersocket.ini");
    if (args.size() > 1) {
        if (args.at(1) == "-f") {
        }
    }
    if (args.size() != 1) {
        if ((args.size() == 2) && (args.at(1) == "-h")) {
            usage();
            exit(0);
        }
        if (args.size() != 3) {
            printf("Error parsing command line arguments\n");
            usage();
            exit(1);
        } else if (args.at(1) == "-f") {
            settingsFile = QString(args.at(2));
        }
    }

    if (!QFile::exists(settingsFile)) {
        fprintf(stderr, QString("Unable to open config file: %1\n").arg(settingsFile).toLatin1());
        usage();
        exit(128);
    }

    QList<SerSocket *> ser_sockets;
    QSettings settings(settingsFile, QSettings::IniFormat);
    QString log_file = settings.value("log_file", "STDOUT").toString();
    int log_level = settings.value("log_level", 0).toInt();
    Logger::init(log_file, log_level);

    QStringList groups = settings.childGroups();
    foreach (const QString &group, groups) {
        Logger::debug(QString("Initializing SerSocket instance %1").arg(group));
        SerSocket * ss = new SerSocket();
        unsigned short tcpPort = settings.value(QString("%1/tcp_port").arg(group), 5331).toInt();
        ss->setTcpPort(tcpPort);

        int baudRate = settings.value(QString("%1/baud_rate").arg(group), 115200).toInt();
        ss->setBaudRate(baudRate);
        QChar parity = settings.value(QString("%1/parity").arg(group), 'N').toChar();
        ss->setParity(parity);
        QChar stopBits = settings.value(QString("%1/stop_bits").arg(group), '1').toChar();
        ss->setStopBits(stopBits);
        ss->start();
        ser_sockets.append(ss);
    }

    return a.exec();
}
