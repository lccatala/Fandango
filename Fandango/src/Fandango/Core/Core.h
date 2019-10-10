#pragma once

#include <memory>

//
//	*** Platform detection*** 
//

#ifdef _WIN32 // Windows x64/x86
	#ifdef _WIN64 // Windows x64
		#define FNDG_PLATFORM_WINDOWS
	#else // Windows x86
		#error "x86 builds are not supported"
	#endif
#elif defined(__APPLE__) || defined(__MACH__) // Apple devices
	#include <TargetConditionals.h>

	// TARGET_OS_MAC exists on all Apple platforms, so we must check
	// all of them (in this order) to ensure we're running on Mac
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "iOS Simulator is not supported"
	#elif TARGET_OS_IPHONE == 1
		#define FNDG_PLATFORM_IOS
		#error "iOS is not supported"
	#elif TARGET_OS_MAC == 1
		#define FNDG_PLATFORM_MACOS
		#error "MacOS is not supported"
	#else
		#error "Unknown Apple platform"
	#endif

// Check Android before Linux since Android is based on the Linux kernel
#elif defined(__ANDROID__)
	#define FNDG_PLATFORM_ANDROID
	#error "Android is not supported"
#elif defined(__linux__)
	#define FNDG_PLATFORM_LINUX´
	#error "Linux is not supported"
#else
	#error "Unknown platform"
#endif // End of platform detection

// DLL support
#ifdef FNDG_PLATFORM_WINDOWS
	#if FNDG_DYNAMIC_LINK
		#ifdef FNDG_BUILD_DLL
			#define FNDG_API __declspec(dllexport)
		#else
			#define FNDG_API __declspec(dllimport)
		#endif
	#else
		#define FNDG_API
	#endif
#else
	#error "Fandango only supports Windows"
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

namespace Fandango 
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}