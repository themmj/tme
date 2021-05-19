#ifndef _CORE_LOG_H
#define _CORE_LOG_H
/** @file */

#include <memory>
#include "spdlog/logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace tme {
    namespace core {

        /// Smart handle to spd::logger
        using LoggerHandle = std::shared_ptr<spdlog::logger>;

        /**//**
         * \brief Logger
         *
         * Singleton wrapper for splog logger.
         * Initialize with init().
         * Prefer TME_<LEVEL> macro calls for logging
         * but instance can be accessed directly if needed.
         */
        class Log {
            static LoggerHandle s_logger;

        public:
            /**//**
             * Initialize logger instance
             */
            static void init();

            /**//**
             * Get logger instance
             */
            static inline LoggerHandle& getInstance() { return s_logger; }
        };

    }
}

#ifdef TME_DEBUG
/**//**
 * \brief Log on trace level.
 *
 * @param ... Arguments which need to have an ostream& operator<<(ostream&, T) definded.
 * First argument can be an spdlog format string followed by the needed arguments.
 */
#   define TME_TRACE(...)     SPDLOG_LOGGER_TRACE(::tme::core::Log::getInstance(), __VA_ARGS__)
/// Log on info level. \sa #TME_TRACE(...)
#   define TME_INFO(...)      SPDLOG_LOGGER_INFO(::tme::core::Log::getInstance(), __VA_ARGS__)
/// Log on warning level. \sa #TME_TRACE(...)
#   define TME_WARN(...)      SPDLOG_LOGGER_WARN(::tme::core::Log::getInstance(), __VA_ARGS__)
/// Log on error level. \sa #TME_TRACE(...)
#   define TME_ERROR(...)     SPDLOG_LOGGER_ERROR(::tme::core::Log::getInstance(), __VA_ARGS__)
/// Log on critical level. \sa #TME_TRACE(...)
#   define TME_CRITICAL(...)  SPDLOG_LOGGER_CRITICAL(::tme::core::Log::getInstance(), __VA_ARGS__)
#   include <signal.h>
/**//**
 * Log on critical level and raise SIGTRAP if x is false.
 *
 * @param x Statement to be used in an if clause.
 * @param ... \ref #TME_TRACE(...)
 */
#   define TME_ASSERT(x, ...) { if (!(x)) { TME_CRITICAL("Assertion failed!"); TME_CRITICAL(__VA_ARGS__); raise(SIGTRAP); } }
#else
#   define TME_TRACE(...)
#   define TME_INFO(...)
#   define TME_WARN(...)
#   define TME_ERROR(...)  
#   define TME_CRITICAL(...)
#   define TME_ASSERT(x, ...)
#endif // TME_DEBUG

#endif // LOG_H
