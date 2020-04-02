#pragma once

#include "teslyn/core/tensor.hpp"
// #include "teslyn/core/neural_network.hpp"

// #include "teslyn/linalg/inversion.hpp"

#include "teslyn/utils/instrumentation.hpp"
#include "teslyn/utils/log.hpp"

#ifdef DEBUG
    // Client Logging Macros
    #define TESLYN_TRACE(...) ::Teslyn::Log::get_client_logger()->trace(__VA_ARGS__)
    #define TESLYN_INFO(...) ::Teslyn::Log::get_client_logger()->info(__VA_ARGS__)
    #define TESLYN_WARN(...) ::Teslyn::Log::get_client_logger()->warn(__VA_ARGS__)
    #define TESLYN_ERROR(...) ::Teslyn::Log::get_client_logger()->error(__VA_ARGS__)
    #define TESLYN_CRITICAL(...) ::Teslyn::Log::get_client_logger()->critical(__VA_ARGS__)
#else
    // Client Logging Macros
    #define TESLYN_TRACE(...)
    #define TESLYN_INFO(...)
    #define TESLYN_WARN(...)
    #define TESLYN_ERROR(...)
    #define TESLYN_CRITICAL(...)
#endif // DEBUG