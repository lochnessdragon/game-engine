#pragma once

#include <string>

#include <spdlog/spdlog.h>

class Log {
private:
	static std::shared_ptr<spdlog::logger> GameLog;
	static std::shared_ptr<spdlog::logger> RendererLog;
	static spdlog::sinks_init_list sinks;
public:
	static void Init();
	static std::shared_ptr<spdlog::logger> CreateLogger(const std::string& name) { return CreateLogger(name, sinks); }
	static std::shared_ptr<spdlog::logger> CreateLogger(const std::string& name, spdlog::sinks_init_list list);

	static std::shared_ptr<spdlog::logger> GetGameLog() { return GameLog; }
	static std::shared_ptr<spdlog::logger> GetRendererLog() { return RendererLog; }
};