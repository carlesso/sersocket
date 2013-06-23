#include "logger.h"

Logger* Logger::s_instance(0);

const char LOG_SIGNATURE[6] = "DIWEF";

Logger* Logger::getInstance() {
    if (s_instance == 0) {
        s_instance = new Logger("STDOUT");
    }
    return s_instance;
}

Logger::Logger(const QString &log_file)
{
    logLevel = 0;
    if (log_file == "STDOUT") {
        m_useStdOut = true;
    } else {
        m_useStdOut = false;
        logfile.setFileName(log_file);
        if (logfile.exists()){
            if (logfile.size() > (4*1024*1024)){
                logfile.open(QIODevice::WriteOnly);
            }else{
                logfile.open(QIODevice::WriteOnly|QIODevice::Append);
            }
        } else {
            logfile.open(QIODevice::WriteOnly|QIODevice::Append);
        }
    }
    write("=================================", LOG_WARN);
    write("Session Opening", LOG_WARN);
    write("=================================", LOG_WARN);
}

Logger::~Logger()
{
    write("=================================", LOG_WARN);
    write("Session Closing", LOG_WARN);
    write("=================================", LOG_WARN);
    logfile.close();
}

void Logger::write(const QString &message, int level) {
    if (level >= logLevel) {
        QString log_message = QString("%1:%2 --- %3\n").arg(QString(LOG_SIGNATURE[level]),
                                                            QDateTime::currentDateTime().toString(),
                                                            message);
        if (m_useStdOut) {
            qDebug() << log_message.replace("\n", "");
        } else {
            logfile.write(log_message.toLatin1().data());
            logfile.flush();
        }
    }
}

void Logger::debug(const QString &message) {
    return getInstance()->write(message, LOG_DEBUG);
}
void Logger::info(const QString &message) {
    return getInstance()->write(message, LOG_INFO);
}
void Logger::warn(const QString &message) {
    return getInstance()->write(message, LOG_WARN);
}
void Logger::error(const QString &message) {
    return getInstance()->write(message, LOG_ERROR);
}
void Logger::fatal(const QString &message) {
    return getInstance()->write(message, LOG_FATAL);
}

void Logger::close()
{
    delete s_instance;
}

void Logger::setLogLevel(int i) {
    getInstance()->logLevel = i;
}

void Logger::init(const QString &log_file, int log_level)
{
    close();
    s_instance = new Logger(log_file);
    s_instance->setLogLevel(log_level);
}

void Logger::init(const QString &log_file)
{
    close();
    s_instance = new Logger(log_file);
}

