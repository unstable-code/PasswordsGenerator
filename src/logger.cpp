#include "logger.h"

Logger::Logger()
    : m_debugMode(false)
{
}

void Logger::log(LogLevel level, const QString& message)
{
    // INFO 레벨은 디버그 모드에서만 표시
    if (level == LogLevel::Info && !m_debugMode) {
        return;
    }

    m_logs.append(LogEntry(level, message));
}

void Logger::error(const QString& message)
{
    log(LogLevel::Error, message);
}

void Logger::warning(const QString& message)
{
    log(LogLevel::Warning, message);
}

void Logger::info(const QString& message)
{
    log(LogLevel::Info, message);
}

void Logger::note(const QString& message)
{
    log(LogLevel::Note, message);
}

void Logger::setDebugMode(bool enabled)
{
    m_debugMode = enabled;
    QString msg = QString("INFO레벨 로그 표시 기능이 %1되었음")
                    .arg(enabled ? "활성화" : "비활성화");
    info(msg);
}

void Logger::clear()
{
    m_logs.clear();
}
