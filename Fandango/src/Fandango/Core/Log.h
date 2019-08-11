#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Fandango {
	class FNDG_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
} 

// Engine log macros
#define FNDG_ENGINE_TRACE(...)    ::Fandango::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define FNDG_ENGINE_INFO(...)     ::Fandango::Log::GetEngineLogger()->info(__VA_ARGS__)
#define FNDG_ENGINE_WARN(...)     ::Fandango::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define FNDG_ENGINE_ERROR(...)    ::Fandango::Log::GetEngineLogger()->error(__VA_ARGS__)
#define FNDG_ENGINE_CRITICAL(...) ::Fandango::Log::GetEngineLogger()->critical(__VA_ARGS__)

// Client log macros
#define FNDG_TRACE(...)        ::Fandango::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FNDG_INFO(...)         ::Fandango::Log::GetClientLogger()->info(__VA_ARGS__)
#define FNDG_WARN(...)         ::Fandango::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FNDG_ERROR(...)        ::Fandango::Log::GetClientLogger()->error(__VA_ARGS__)
#define FNDG_CRITICAL(...)     ::Fandango::Log::GetClientLogger()->critical(__VA_ARGS__)