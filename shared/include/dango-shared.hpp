#ifndef DANGO_SHARED_HPP_INCLUDED
#define DANGO_SHARED_HPP_INCLUDED

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

DANGO_SHARED_API dango::assert_log_handler dango_shared_set_assert(dango::assert_log_handler)noexcept;
DANGO_SHARED_API dango::assert_log_handler dango_shared_get_assert()noexcept;

#endif // DANGO_SHARED_HPP_INCLUDED

