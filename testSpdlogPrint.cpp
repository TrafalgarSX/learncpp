// #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE // Must: define SPDLOG_ACTIVE_LEVEL before `#include "spdlog/spdlog.h"`
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

int main(int argc, const char** argv) {

    spdlog::set_pattern("[thread %t] [source %s] [function %!] [line %#] %v");
    auto console_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs.txt", true);

    // 创建一个包含控制台和文件接收器的多接收器 logger
    std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
    auto logger = std::make_shared<spdlog::logger>("multi_sink", sinks.begin(), sinks.end());
    // 注册 logger
    spdlog::register_logger(logger);

    spdlog::set_level(spdlog::level::warn); // Set global log level to trace


    SPDLOG_LOGGER_INFO(logger, "log with source info"); // Use console.

    SPDLOG_TRACE("print trace"); 
    SPDLOG_DEBUG("print debug"); 
    SPDLOG_INFO("global log with source info"); // Use spdlog::default_logger()
    SPDLOG_WARN("print warn"); 
    SPDLOG_TRACE("print trace");
    SPDLOG_ERROR("print error");
    SPDLOG_CRITICAL("print critical");

    logger->info("source info is not printed"); // Console: "[source ] [function ] [line ] source info is not printed"
    return 0;
}