#include <Engine/Runtime/Logger.hpp>

#include <cstdarg>
#include <cstdio>
#include <vector>
#include <mutex>
#include <algorithm>

namespace engine::runtime {
    static std::vector<Logger::LogDelegate> s_LogHandlers;
    static std::mutex s_LogMutex;

    Logger::Logger(std::string_view name) : m_LoggerName(name) {}

    const char* LogLevel_ToString(LogLevel level) {
        switch (level) {
            case LOG_LEVEL_ERROR: return "ERROR";
            case LOG_LEVEL_WARNING: return "WARNING";
            case LOG_LEVEL_INFO: return "INFO";
            case LOG_LEVEL_DEBUG: return "DEBUG";
            default: return "UNKNOWN";
        }
    }

    void Logger::Log(LogLevel level, const char *format, ...) {
        va_list args;

        // first get size of our data
        va_start(args, format);
        int bufSize = vsnprintf(nullptr, 0, format, args);
        va_end(args);

        if (bufSize > 0) {
            std::string fmtText(bufSize, '\0');

            va_start(args, format);
            vsnprintf(fmtText.data(), bufSize + 1, format, args);
            va_end(args);

            printf("[%s] %s: %s\n", LogLevel_ToString(level), m_LoggerName.c_str(), fmtText.c_str());

            std::lock_guard<std::mutex> lock(s_LogMutex);
            for (const auto &handler: s_LogHandlers) {
                handler(level, m_LoggerName, fmtText);
            }
        }
    }

    void Logger::SubscribeLogger(const LogDelegate& handler) {
        std::lock_guard<std::mutex> lock(s_LogMutex);
        s_LogHandlers.push_back(std::move(handler));
    }

    void Logger::UnsubscribeLogger(const LogDelegate& handler) {
        std::lock_guard<std::mutex> lock(s_LogMutex);

        auto it = std::find_if(s_LogHandlers.begin(), s_LogHandlers.end(),
                               [&handler](const LogDelegate& registeredHandler) {
                                   return registeredHandler.target_type() == handler.target_type() &&
                                          registeredHandler.target<void(LogLevel, std::string_view, std::string_view)>() ==
                                          handler.target<void(LogLevel, std::string_view, std::string_view)>();
                               });

        if (it != s_LogHandlers.end()) {
            s_LogHandlers.erase(it);
        }
    }
}
