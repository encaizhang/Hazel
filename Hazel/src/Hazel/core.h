#pragma once

#ifdef HZ_PLATFORM_WINDOWS
	#ifdef HZ_BUILD_DLL
		#define HAZEL_API_  __declspec(dllexport)
	#else
		#define HAZEL_API_  __declspec(dllimport)
	#endif
#else
	#error Hazel only support Windows!
#endif
