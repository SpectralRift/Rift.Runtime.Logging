#pragma once

#include <string>
#include <string_view>
#include <functional>

namespace engine::runtime {
    enum LogLevel {
        LOG_LEVEL_UNKNOWN,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_WARNING,
        LOG_LEVEL_INFO,
        LOG_LEVEL_DEBUG
    };

    extern const char* LogLevel_ToString(LogLevel level);

    struct Logger {
        using LogDelegate = std::function<void(LogLevel, std::string_view, std::string_view)>;

        Logger(std::string_view name);

        void Log(LogLevel level, const char* format, ...);

        static void SubscribeLogger(const LogDelegate& handler);
        static void UnsubscribeLogger(const LogDelegate& handler);
    protected:
        std::string m_LoggerName;
        std::vector<LogDelegate> m_LogHandlers;
    };
}