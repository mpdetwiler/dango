#ifndef __DANGO_HPP__
#define __DANGO_HPP__

#include "dango-macro.hpp"
#include "dango-def.hpp"
#include "dango-assert.hpp"
#include "dango-traits.hpp"
#include "dango-atomic.hpp"
#include "dango-cstring.hpp"
#include "dango-int.hpp"
#include "dango-intrusive-list.hpp"
#include "dango-operator.hpp"
#include "dango-util.hpp"
#include "dango-concurrent-base.hpp"
#include "dango-mem.hpp"
#include "dango-global.hpp"
#include "dango-tuple.hpp"
#include "dango-concurrent.hpp"

#ifdef DANGO_SOURCE_FILE

#include <new>
#include <exception>
#include <cstdio>
#include <cerrno>

#ifdef __linux__

#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <linux/futex.h>

#endif /* __linux__ */

#ifdef _WIN32

#define DANGO_WINDOWS_VER 0x0601

#ifdef WINVER
#if WINVER < DANGO_WINDOWS_VER
#undef WINVER
#define WINVER DANGO_WINDOWS_VER
#endif
#else
#define WINVER DANGO_WINDOWS_VER
#endif

#ifdef _WIN32_WINNT
#if _WIN32_WINNT < DANGO_WINDOWS_VER
#undef _WIN32_WINNT
#define _WIN32_WINNT DANGO_WINDOWS_VER
#endif
#else
#define _WIN32_WINNT DANGO_WINDOWS_VER
#endif

#include <windows.h>

#endif /* _WIN32 */

#include "dango-assert-source.hpp"
#include "dango-mem-source.hpp"
#include "dango-concurrent-source.hpp"

#endif /* DANGO_SOURCE_FILE */

#endif

