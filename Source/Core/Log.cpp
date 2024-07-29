#include "log.h"

#include <iostream>

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Log::RendererLog;
std::shared_ptr<spdlog::logger> Log::GameLog;
spdlog::sinks_init_list Log::sinks;

void Log::Init() {
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

	console_sink->set_pattern("[%T] [%^%n/%l%$]: %v");
	console_sink->set_level(spdlog::level::trace);
	
	sinks = { console_sink };

	RendererLog = CreateLogger("renderer", sinks);
	GameLog = CreateLogger("game", sinks);
}

std::shared_ptr<spdlog::logger> Log::CreateLogger(const std::string& name, spdlog::sinks_init_list sinks) {
	auto logger = std::make_shared<spdlog::logger>(name, sinks);
	logger->set_level(spdlog::level::trace);
	spdlog::register_logger(logger);

	return logger;
}