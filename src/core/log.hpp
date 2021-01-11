#ifndef LOG_H
#define LOG_H

#include <memory>
#include "spdlog/logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace tme {
    namespace core {
        
        using LoggerHandle = std::shared_ptr<spdlog::logger>;

        class Log {
            static LoggerHandle s_logger;

        public:
            static void init();
            static inline LoggerHandle& getInstance() { return s_logger; }
        };

    }
}

#ifdef TME_DEBUG
#   define TME_TRACE(...)     ::tme::core::Log::getInstance()->trace(__VA_ARGS__)
#   define TME_INFO(...)      ::tme::core::Log::getInstance()->info(__VA_ARGS__)
#   define TME_WARN(...)      ::tme::core::Log::getInstance()->warn(__VA_ARGS__)
#   define TME_ERROR(...)     ::tme::core::Log::getInstance()->error(__VA_ARGS__)
#   define TME_CRITICAL(...)  ::tme::core::Log::getInstance()->critical(__VA_ARGS__)
#   include <signal.h>
#   define TME_ASSERT(x, ...) { if (!(x)) { TME_CRITICAL("Assertion failed: {0}", __VA_ARGS__); raise(SIGTRAP); } }
#else
#   define TME_TRACE(...)
#   define TME_INFO(...)
#   define TME_WARN(...)
#   define TME_ERROR(...)  
#   define TME_CRITICAL(...)
#   define TME_ASSERT(x, ...)
#endif // TME_DEBUG

#endif // LOG_H
