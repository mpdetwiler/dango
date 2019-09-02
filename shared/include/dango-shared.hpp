#ifndef __DANGO_SHARED_HPP__
#define __DANGO_SHARED_HPP__

#include <dango.hpp>

#ifdef _WIN32
#ifdef DANGO_COMPILING_DANGO_SHARED
#define DANGO_SHARED_API __declspec(dllexport)
#else
#define DANGO_SHARED_API __declspec(dllimport)
#endif
#else
#ifdef DANGO_COMPILING_DANGO_SHARED
#define DANGO_SHARED_API __attribute__((visibility("default")))
#else
#define DANGO_SHARED_API __attribute__((visibility("default")))
#endif
#endif

#endif

