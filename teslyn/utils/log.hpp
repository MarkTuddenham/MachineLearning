#pragma once

#include <memory>
#include <unordered_map>

#include "spdlog/spdlog.h"

namespace Teslyn
{

class Log
{

public:
    enum Level
    {
        CRITICAL,
        ERROR,
        WARN,
        INFO,
        TRACE,
        NONE
    };

    static void init();
    static void init(const Log::Level &);

    static std::shared_ptr<spdlog::logger> &get_core_logger() { return s_core_logger; }
    static std::shared_ptr<spdlog::logger> &get_client_logger() { return s_client_logger; }

private:
    static std::shared_ptr<spdlog::logger> s_core_logger;
    static std::shared_ptr<spdlog::logger> s_client_logger;

    static spdlog::level::level_enum teslyn_to_spdlog_level(Log::Level);
};

} // namespace Teslyn

#ifdef DEBUG
// Core Macros
#define TESLYN_CORE_TRACE(...) ::Teslyn::Log::get_core_logger()->trace(__VA_ARGS__)
#define TESLYN_CORE_INFO(...) ::Teslyn::Log::get_core_logger()->info(__VA_ARGS__)
#define TESLYN_CORE_WARN(...) ::Teslyn::Log::get_core_logger()->warn(__VA_ARGS__)
#define TESLYN_CORE_ERROR(...) ::Teslyn::Log::get_core_logger()->error(__VA_ARGS__)
#define TESLYN_CORE_CRITICAL(...) ::Teslyn::Log::get_core_logger()->critical(__VA_ARGS__)
#else
// Core Macros
#define TESLYN_CORE_TRACE(...)
#define TESLYN_CORE_INFO(...)
#define TESLYN_CORE_WARN(...)
#define TESLYN_CORE_ERROR(...)
#define TESLYN_CORE_CRITICAL(...)
#endif // DEBUG
