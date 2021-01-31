/** @file */
#include "core/log.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/cfg/env.h"

namespace tme {
    namespace core {
        
        LoggerHandle Log::s_logger;

        void Log::init() {
            spdlog::set_pattern("%H:%M:%S:%e (%5t) %^[%7l] : %v%$");
            s_logger = spdlog::stdout_color_mt("logger");
            // load log level from env 
            // (e.g. export SPDLOG_LEVEL=info before execution)
            spdlog::cfg::load_env_levels();
        }

    }
}
