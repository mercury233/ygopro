#ifndef YGOPRO_CONFIG_H
#define YGOPRO_CONFIG_H

#include <cerrno>
#include <cstdio>
#include <string>
#include "bufferio.h"
#include "../ocgcore/ocgapi.h"

#ifdef _WIN32

#define NOMINMAX 1
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#if defined(_MSC_VER) || defined(__MINGW32__)
#define mywcsncasecmp _wcsnicmp
#define mystrncasecmp _strnicmp
#else
#define mywcsncasecmp wcsncasecmp
#define mystrncasecmp strncasecmp
#endif

#else //_WIN32

#define mywcsncasecmp wcsncasecmp
#define mystrncasecmp strncasecmp

#endif // _WIN32

template<size_t N, typename... TR>
inline int myswprintf(wchar_t(&buf)[N], const wchar_t* fmt, TR... args) {
	return std::swprintf(buf, N, fmt, args...);
}
template<size_t N, typename... TR>
inline int mysnprintf(char(&buf)[N], const char* fmt, TR... args) {
	return std::snprintf(buf, N, fmt, args...);
}

#define _IRR_STATIC_LIB_
#include <irrlicht.h>

constexpr uint16_t PRO_VERSION = 0x1362;

#endif // YGOPRO_CONFIG_H
