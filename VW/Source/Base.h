#pragma once

#define VW_PLATFORM_WINDOWS 1

#ifdef _WIN32
#define VW_PLATFORM VW_PLATFORM_WINDOWS
#define VW_API __declspec(dllexport)
#else
#error "Unsupported platform! Currently only Windows is supported."
#endif
