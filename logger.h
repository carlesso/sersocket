#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QDateTime>
#include <QDebug>

#define LOG_DEBUG 0
#define LOG_INFO  1
#define LOG_WARN  2
#define LOG_ERROR 3
#define LOG_FATAL 4

class Logger
{
public:
    static void debug(const QString &message);
    static void  info(const QString &message);
    static void  warn(const QString &message);
    static void error(const QString &message);
    static void fatal(const QString &message);

    static void close();

    static void setLogLevel(int i);
    static void init(const QString &log_file);
    static void init(const QString &log_file, int log_level);
    int logLevel;

private:
    Logger(const QString &log_file);
    ~Logger();
    static Logger * s_instance;
    void write(const QString &message, int level);
    static Logger* getInstance();
    QFile logfile;
    bool m_useStdOut;

};

#endif // LOGGER_H
