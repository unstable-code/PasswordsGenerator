#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QVector>
#include <QDateTime>

enum class LogLevel {
    Error,
    Warning,
    Info,
    Note
};

struct LogEntry {
    QDateTime timestamp;
    LogLevel level;
    QString message;

    LogEntry(LogLevel lvl, const QString& msg)
        : timestamp(QDateTime::currentDateTime())
        , level(lvl)
        , message(msg)
    {}

    QString levelString() const {
        switch (level) {
            case LogLevel::Error: return "ERR";
            case LogLevel::Warning: return "WRN";
            case LogLevel::Info: return "INFO";
            case LogLevel::Note: return "NOTE";
        }
        return "";
    }

    QString toString() const {
        return QString("%1 [%2]: %3")
            .arg(timestamp.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(levelString())
            .arg(message);
    }
};

class Logger {
public:
    Logger();

    void log(LogLevel level, const QString& message);
    void error(const QString& message);
    void warning(const QString& message);
    void info(const QString& message);
    void note(const QString& message);

    void setDebugMode(bool enabled);
    bool debugMode() const { return m_debugMode; }

    void clear();
    const QVector<LogEntry>& logs() const { return m_logs; }

private:
    QVector<LogEntry> m_logs;
    bool m_debugMode;
};

#endif // LOGGER_H
