#include "dango-shared-concurrent.hpp"

#ifdef __linux__

#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <linux/futex.h>

namespace
{
  template
  <typename tp_func>
  auto thread_start_address(void*)noexcept->void*;

  auto tick_count_help(clockid_t)noexcept->dango::uint64;

  auto
  sys_futex
  (
    dango::s_int*,
    dango::s_int,
    dango::s_int,
    timespec const*,
    dango::s_int*,
    dango::s_int
  )
  noexcept->dango::s_int;
}

auto
dango::
shared::
create_thread
(
  shared::thread_func_type const a_thread_func,
  void* const a_thread_data
)
noexcept->bool
{
  constexpr auto const acquire = dango::mem_order::acquire;
  constexpr auto const release = dango::mem_order::release;

  dango::atomic<bool> a_starting{ true };

  auto a_func =
  [
    a_thread_func,
    a_thread_data,
    &a_starting
  ]
  ()noexcept(false)->void
  {
    ::pthread_detach(::pthread_self());

    a_starting.store<release>(false);

    a_thread_func(a_thread_data);
  };

  {
    ::pthread_t a_thread_ID;

    auto const a_result =
    ::pthread_create
    (
      &a_thread_ID,
      nullptr,
      thread_start_address<decltype(a_func)>,
      &a_func
    );

    if(a_result == EAGAIN)
    {
      return false;
    }

    dango_assert(a_result == 0);
  }

  auto a_count = detail::c_spin_count_init;

  while(a_starting.load<acquire>())
  {
    detail::spin_yield(a_count);
  }

  return true;
}

void
dango::
shared::
yield_thread
()noexcept
{
  ::sched_yield();
}

auto
dango::
shared::
tick_count_monotonic
()noexcept->dango::uint64
{
  return tick_count_help(CLOCK_MONOTONIC);
}

auto
dango::
shared::
tick_count_boottime
()noexcept->dango::uint64
{
  return tick_count_help(CLOCK_BOOTTIME);
}

void
dango::
shared::
futex_wait
(shared::futex_type* const a_futex, shared::futex_type const a_expected)noexcept
{
  sys_futex
  (
    a_futex,
    FUTEX_WAIT_PRIVATE,
    a_expected,
    nullptr,
    nullptr,
    dango::s_int(0)
  );
}

void
dango::
shared::
futex_wait
(
  shared::futex_type* const a_futex,
  shared::futex_type const a_expected,
  dango::uint64 const a_interval
)
noexcept
{
  using u64 = dango::uint64;

  ::timespec a_spec;

  {
    auto const a_sec = a_interval / u64(1'000);
    auto const a_mod = a_interval % u64(1'000);
    auto const a_nsec = a_mod * u64(1'000'000);

    a_spec.tv_sec = a_sec;
    a_spec.tv_nsec = a_nsec;
  }

  sys_futex
  (
    a_futex,
    FUTEX_WAIT_PRIVATE,
    a_expected,
    &a_spec,
    nullptr,
    dango::s_int(0)
  );
}

void
dango::
shared::
futex_wake
(shared::futex_type* const a_futex)noexcept
{
  sys_futex
  (
    a_futex,
    FUTEX_WAKE_PRIVATE,
    dango::s_int(1),
    nullptr,
    nullptr,
    dango::s_int(0)
  );
}

void
dango::
shared::
futex_wake_requeue
(shared::futex_type* const a_futex, shared::futex_type* const a_dest)noexcept
{
  constexpr auto const c_all = dango::uintptr(dango::integer::MAX_VAL<dango::s_int>);

  sys_futex
  (
    a_futex,
    FUTEX_REQUEUE_PRIVATE,
    dango::s_int(1),
    reinterpret_cast<timespec const*>(c_all),
    a_dest,
    dango::s_int(0)
  );
}

namespace
{
  template
  <typename tp_func>
  auto
  thread_start_address
  (void* const a_data)noexcept->void*
  {
    static_assert(!dango::is_ref<tp_func>);
    static_assert(!dango::is_const<tp_func> && !dango::is_volatile<tp_func>);
    static_assert(dango::is_noexcept_move_constructible<tp_func>);
    static_assert(dango::is_callable_ret<void, tp_func const&>);
    static_assert(dango::is_noexcept_destructible<tp_func>);

    auto const a_func_ptr = static_cast<tp_func*>(a_data);

    tp_func const a_func = dango::move(*a_func_ptr);

    try
    {
      a_func();
    }
    catch(...)
    {
#ifndef DANGO_NO_DEBUG
      dango_unreachable_msg("uncaught exception in thread");
#else
      dango::terminate();
#endif
    }

    return nullptr;
  }

  auto
  tick_count_help
  (clockid_t const a_clock)noexcept->dango::uint64
  {
    using u64 = dango::uint64;

    constexpr auto const c_mul = u64(1'000);
    constexpr auto const c_div = u64(1'000'000);

    ::timespec a_spec;

    auto const a_result =
      ::clock_gettime(a_clock, &a_spec);

    dango_assert(a_result == 0);

    auto const a_sec = u64(a_spec.tv_sec);
    auto const a_nsec = u64(a_spec.tv_nsec);

    return (a_sec * c_mul) + (a_nsec / c_div);
  }

  auto
  sys_futex
  (
    dango::s_int* const a_addr1,
    dango::s_int const a_futex_op,
    dango::s_int const a_val1,
    timespec const* const a_timeout,
    dango::s_int* const a_addr2,
    dango::s_int const a_val3
  )
  noexcept->dango::s_int
  {
    auto const a_result =
    ::syscall
    (
      SYS_futex,
      a_addr1,
      a_futex_op,
      a_val1,
      a_timeout,
      a_addr2,
      a_val3
    );

    return a_result;
  }
}

#endif /* __linux__ */

#ifdef _WIN32

#include <windows.h>

namespace
{
  template
  <typename tp_func>
  auto WINAPI thread_start_address(LPVOID)noexcept->DWORD;

  auto perf_count()noexcept->dango::uint64
}

auto
dango::
shared::
create_thread
(
  shared::thread_func_type const a_thread_func,
  void* const a_thread_data
)
noexcept->bool
{
  constexpr auto const acquire = dango::mem_order::acquire;
  constexpr auto const release = dango::mem_order::release;

  dango::atomic<bool> a_starting{ true };

  auto a_func =
  [
    a_thread_func,
    a_thread_data,
    &a_starting
  ]
  ()noexcept(false)->void
  {
    a_starting.store<release>(false);

    a_thread_func(a_thread_data);
  };

  {
    auto const a_handle =
    ::CreateThread
    (
      nullptr,
      SIZE_T(0),
      thread_start_address<decltype(a_func)>,
      &a_func,
      DWORD(0),
      nullptr
    );

    if(a_handle == NULL)
    {
      return false;
    }

    auto const a_result = ::CloseHandle(a_handle);

    dango_assert(a_result != 0);
  }

  auto a_count = detail::c_spin_count_init;

  while(a_starting.load<acquire>())
  {
    detail::spin_yield(a_count);
  }

  return true;
}

void
dango::
shared::
yield_thread
()noexcept
{
  ::Sleep(DWORD(0));
}

auto
dango::
shared::
perf_freq
()noexcept->dango::uint64
{
  static constexpr auto const c_impl =
  []()noexcept->dango::uint64
  {
    LARGE_INTEGER a_result;

    ::QueryPerformanceFrequency(&a_result);

    return dango::uint64(a_result.QuadPart);
  };

  static auto const s_result = c_impl();

  return s_result;
}

auto
dango::
shared::
perf_count_suspend_bias
(dango::uint64& a_suspend_bias)noexcept->dango::uint64
{
  using u64 = dango::uint64;

  static auto& s_bias_ref =
    *reinterpret_cast<ULONGLONG const volatile*>(dango::uintptr(0x7FFE0000 + 0x3B0));

  u64 a_bias;
  u64 a_count;

  do
  {
    a_bias = u64(s_bias_ref);
    a_count = perf_count();
  }
  while(a_bias != u64(s_bias_ref));

  a_suspend_bias = a_bias;

  return a_count;
}

namespace
{
  template
  <typename tp_func>
  auto WINAPI
  thread_start_address
  (LPVOID const a_data)noexcept->DWORD
  {
    static_assert(!dango::is_ref<tp_func>);
    static_assert(!dango::is_const<tp_func> && !dango::is_volatile<tp_func>);
    static_assert(dango::is_noexcept_move_constructible<tp_func>);
    static_assert(dango::is_callable_ret<void, tp_func const&>);
    static_assert(dango::is_noexcept_destructible<tp_func>);

    auto const a_func_ptr = static_cast<tp_func*>(a_data);

    tp_func const a_func = dango::move(*a_func_ptr);

    try
    {
      a_func();
    }
    catch(...)
    {
#ifndef DANGO_NO_DEBUG
      dango_unreachable_msg("uncaught exception in thread");
#else
      dango::terminate();
#endif
    }

    return DWORD(0);
  }

  auto
  perf_count
  ()noexcept->dango::uint64
  {
    LARGE_INTEGER a_result;

    ::QueryPerformanceCounter(&a_result);

    return dango::uint64(a_result.QuadPart);
  }
}

#endif /* _WIN32 */

