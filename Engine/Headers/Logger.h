#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"

class Logger
{
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

// Core log macros
#define N3D_CORE_TRACE(...)    Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define N3D_CORE_INFO(...)     Logger::GetCoreLogger()->info(__VA_ARGS__)
#define N3D_CORE_WARN(...)     Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define N3D_CORE_ERROR(...)    Logger::GetCoreLogger()->error(__VA_ARGS__)
#define N3D_CORE_FATAL(...)    Logger::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define N3D_LOG_TRACE(...)	      Logger::GetClientLogger()->trace(__VA_ARGS__)
#define N3D_LOG_INFO(...)	      Logger::GetClientLogger()->info(__VA_ARGS__)
#define N3D_LOG_WARN(...)	      Logger::GetClientLogger()->warn(__VA_ARGS__)
#define N3D_LOG_ERROR(...)	      Logger::GetClientLogger()->error(__VA_ARGS__)
#define N3D_LOG_FATAL(...)	      {Logger::GetClientLogger()->critical(__VA_ARGS__); __debugbreak();}