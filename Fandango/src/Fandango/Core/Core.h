#pragma once

#ifdef FNDG_PLATFORM_WINDOWS
#if FNDG_DYNAMIC_LINK
	#ifdef FNDG_BUILD_DLL
		#define FNDG_API __declspec(dllexport)
	#else
		#define	FNDG_API __declspec(dllimport)
	#endif
#else
	#define FNDG_API
#endif
#else
	#error Fandango only supports Windows
#endif

#ifdef FNDG_ENABLE_ASSERTS
	#define FNDG_ASSERT(x, ...) {if(!(x))} { FNDG_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define FNDG_ENGINE_ASSERT(x, ...) {if(!x)} {FNDG_ENGINE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
#define FNDG_ASSERT(x, ...)
#define FNDG_ENGINE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define FNDG_BIND_EVENT_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)