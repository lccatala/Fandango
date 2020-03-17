#include "fndgpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Fandango {

	std::shared_ptr<spdlog::logger> Log::s_EngineLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		// Color + timestamp + name of the logger + message
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_EngineLogger = spdlog::stdout_color_mt("ENGINE");
		s_EngineLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}