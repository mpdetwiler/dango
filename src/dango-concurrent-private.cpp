#include "dango-concurrent-private.hpp"

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

  auto tick_count_help(clockid_t)noexcept->dango::tick_count_type;

  using linux_int = dango::detail::futex_type;

  auto
  sys_futex
  (
    linux_int*,
    linux_int,
    linux_int,
    timespec const*,
    linux_int*,
    linux_int
  )
  noexcept->linux_int;
}

auto
dango::
detail::
create_thread
(
  dango::detail::thread_func_type const a_thread_func,
  void* const a_thread_data
)
noexcept->bool
{
  using dango::mem_order::acquire;
  using dango::mem_order::release;

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
      dango::null,
      &thread_start_address<decltype(a_func)>,
      &a_func
    );

    if(a_result == EAGAIN)
    {
      return false;
    }

    dango_assert(a_result == 0);
  }

  auto a_count = dango::detail::c_spin_count_init;

  while(a_starting.load<acquire>())
  {
    dango::detail::spin_yield(a_count);
  }

  return true;
}

void
dango::
thread_yield
()noexcept
{
  ::sched_yield();
}

auto
dango::
detail::
tick_count_monotonic
()noexcept->dango::tick_count_type
{
  return tick_count_help(CLOCK_MONOTONIC);
}

auto
dango::
detail::
tick_count_boottime
()noexcept->dango::tick_count_type
{
  return tick_count_help(CLOCK_BOOTTIME);
}

void
dango::
detail::
futex_wait
(dango::detail::futex_type* const a_futex, dango::detail::futex_type const a_expected)noexcept
{
  sys_futex
  (
    a_futex,
    FUTEX_WAIT_PRIVATE,
    a_expected,
    dango::null,
    dango::null,
    linux_int(0)
  );
}

void
dango::
detail::
futex_wait
(
  dango::detail::futex_type* const a_futex,
  dango::detail::futex_type const a_expected,
  dango::tick_count_type const a_interval
)
noexcept
{
  using tc64 = dango::tick_count_type;

  dango_assert(a_interval >= tc64(0));

  ::timespec a_spec;

  {
    auto const a_sec = a_interval / tc64(1'000);
    auto const a_mod = a_interval % tc64(1'000);
    auto const a_nsec = a_mod * tc64(1'000'000);

    a_spec.tv_sec = a_sec;
    a_spec.tv_nsec = a_nsec;
  }

  sys_futex
  (
    a_futex,
    FUTEX_WAIT_PRIVATE,
    a_expected,
    &a_spec,
    dango::null,
    linux_int(0)
  );
}

void
dango::
detail::
futex_wake
(dango::detail::futex_type* const a_futex)noexcept
{
  sys_futex
  (
    a_futex,
    FUTEX_WAKE_PRIVATE,
    linux_int(1),
    dango::null,
    dango::null,
    linux_int(0)
  );
}

void
dango::
detail::
futex_wake_requeue
(dango::detail::futex_type* const a_futex, dango::detail::futex_type* const a_dest)noexcept
{
  static constexpr auto const c_all = dango::uptr(dango::integer::MAX_VAL<linux_int>);

  sys_futex
  (
    a_futex,
    FUTEX_REQUEUE_PRIVATE,
    linux_int(1),
    reinterpret_cast<timespec const*>(c_all),
    a_dest,
    linux_int(0)
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
      dango_unreachable_terminate_msg(u8"uncaught exception in thread");
    }

    return dango::null;
  }

  auto
  tick_count_help
  (clockid_t const a_clock)noexcept->dango::tick_count_type
  {
    using tc64 = dango::tick_count_type;

    static constexpr auto const c_mul = tc64(10'000);
    static constexpr auto const c_div = tc64(100'000);

    ::timespec a_spec;

    auto const a_result =
      ::clock_gettime(a_clock, &a_spec);

    dango_assert(a_result == 0);

    auto const a_sec = tc64(a_spec.tv_sec);
    auto const a_nsec = tc64(a_spec.tv_nsec);

    return (a_sec * c_mul) + (a_nsec / c_div);
  }

  auto
  sys_futex
  (
    linux_int* const a_addr1,
    linux_int const a_futex_op,
    linux_int const a_val1,
    timespec const* const a_timeout,
    linux_int* const a_addr2,
    linux_int const a_val3
  )
  noexcept->linux_int
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

#endif // __linux__

#ifdef _WIN32

#include <windows.h>
#include <process.h>

static_assert(sizeof(dango::detail::srw_lock_storage) == sizeof(::SRWLOCK));
static_assert(alignof(dango::detail::srw_lock_storage) == alignof(::SRWLOCK));
static_assert(sizeof(dango::detail::condition_variable_storage) == sizeof(::CONDITION_VARIABLE));
static_assert(alignof(dango::detail::condition_variable_storage) == alignof(::CONDITION_VARIABLE));

namespace
{
  template
  <typename tp_func>
  auto __stdcall thread_start_address(void*)noexcept->dango::builtin::uint;

  auto perf_count()noexcept->dango::tick_count_type;

  auto time_min_period()noexcept->dango::uint;
}

auto
dango::
detail::
create_thread
(
  dango::detail::thread_func_type const a_thread_func,
  void* const a_thread_data
)
noexcept->bool
{
  using dango::mem_order::acquire;
  using dango::mem_order::release;

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
    auto const a_temp =
    ::_beginthreadex
    (
      dango::null,
      0u,
      &thread_start_address<decltype(a_func)>,
      &a_func,
      0x04u,
      dango::null
    );

    if(a_temp == 0)
    {
      return false;
    }

    auto const a_handle = reinterpret_cast<HANDLE>(a_temp);

    auto const a_resume = ::ResumeThread(a_handle);

    dango_assert(a_resume != DWORD(-1));

    auto const a_close = ::CloseHandle(a_handle);

    dango_assert(a_close);
  }

  auto a_count = dango::detail::c_spin_count_init;

  while(a_starting.load<acquire>())
  {
    dango::detail::spin_yield(a_count);
  }

  return true;
}

void
dango::
thread_yield
()noexcept
{
  ::Sleep(DWORD(0));
}

auto
dango::
detail::
perf_freq
()noexcept->dango::tick_count_type
{
  static constexpr auto const c_impl =
  []()noexcept->dango::tick_count_type
  {
    LARGE_INTEGER a_result;

    ::QueryPerformanceFrequency(&a_result);

    return dango::tick_count_type(a_result.QuadPart);
  };

  static auto const s_result = c_impl();

  return s_result;
}

auto
dango::
detail::
perf_count_suspend_bias
(dango::tick_count_type& a_suspend_bias)noexcept->dango::tick_count_type
{
  using tc64 = dango::tick_count_type;

  static auto& s_bias_ref =
    *reinterpret_cast<ULONGLONG const volatile*>(dango::uptr(0x7FFE0000 + 0x3B0));

  tc64 a_bias;
  tc64 a_count;

  do
  {
    a_bias = tc64(s_bias_ref);
    a_count = perf_count();
  }
  while(a_bias != tc64(s_bias_ref));

  a_suspend_bias = a_bias;

  return a_count;
}

/*****************************/

template
<dango::is_object tp_type>
constexpr auto
dango::
detail::
srw_lock_storage::
launder_get
()const noexcept->tp_type*
{
  using type = ::SRWLOCK;

  static_assert(dango::is_same<tp_type, type>);

  return m_storage.launder_get<type>();
}

void
dango::
detail::
srw_lock_init
(dango::detail::srw_lock_storage* const a_storage)noexcept
{
  using type = ::SRWLOCK;

  auto const a_temp = dango::placement_new_trivial<type>(a_storage->get());

  ::InitializeSRWLock(a_temp);
}

void
dango::
detail::
srw_lock_acquire
(dango::detail::srw_lock_storage* const a_storage)noexcept
{
  using type = ::SRWLOCK;

  ::AcquireSRWLockExclusive(a_storage->launder_get<type>());
}

auto
dango::
detail::
srw_lock_try_acquire
(dango::detail::srw_lock_storage* const a_storage)noexcept->bool
{
  using type = ::SRWLOCK;

  auto const a_result = ::TryAcquireSRWLockExclusive(a_storage->launder_get<type>());

  return bool(a_result);
}

void
dango::
detail::
srw_lock_release
(dango::detail::srw_lock_storage* const a_storage)noexcept
{
  using type = ::SRWLOCK;

  ::ReleaseSRWLockExclusive(a_storage->launder_get<type>());
}

/*****************************/

template
<dango::is_object tp_type>
constexpr auto
dango::
detail::
condition_variable_storage::
launder_get
()const noexcept->tp_type*
{
  using type = ::CONDITION_VARIABLE;

  static_assert(dango::is_same<tp_type, type>);

  return m_storage.launder_get<type>();
}

void
dango::
detail::
condition_variable_init
(dango::detail::condition_variable_storage* const a_storage)noexcept
{
  using type = ::CONDITION_VARIABLE;

  auto const a_temp = dango::placement_new_trivial<type>(a_storage);

  ::InitializeConditionVariable(a_temp);
}

void
dango::
detail::
condition_variable_wake
(dango::detail::condition_variable_storage* const a_storage)noexcept
{
  using type = ::CONDITION_VARIABLE;

  ::WakeConditionVariable(a_storage->launder_get<type>());
}

void
dango::
detail::
condition_variable_wake_all
(dango::detail::condition_variable_storage* const a_storage)noexcept
{
  using type = ::CONDITION_VARIABLE;

  ::WakeAllConditionVariable(a_storage->launder_get<type>());
}

void
dango::
detail::
condition_variable_wait
(dango::detail::condition_variable_storage* const a_storage, dango::detail::srw_lock_storage* const a_lock_storage)noexcept
{
  using type = ::CONDITION_VARIABLE;
  using lock_type = ::SRWLOCK;

  ::SleepConditionVariableSRW
  (
    a_storage->launder_get<type>(),
    a_lock_storage->launder_get<lock_type>(),
    DWORD(INFINITE),
    ULONG(0)
  );
}

void
dango::
detail::
condition_variable_wait
(dango::detail::condition_variable_storage* const a_storage, dango::detail::srw_lock_storage* const a_lock_storage, dango::tick_count_type const a_interval)noexcept
{
  using type = ::CONDITION_VARIABLE;
  using lock_type = ::SRWLOCK;

  using tc64 = dango::tick_count_type;

  dango_assert(a_interval >= tc64(0));

  static constexpr auto const c_max_interval = tc64(DWORD(INFINITE) - DWORD(1));

  auto const a_spec = dango::min(a_interval, c_max_interval);

  dango_assert(DWORD(a_spec) != DWORD(INFINITE));

  ::SleepConditionVariableSRW
  (
    a_storage->launder_get<type>(),
    a_lock_storage->launder_get<lock_type>(),
    DWORD(a_spec),
    ULONG(0)
  );
}

/*****************************/

void
dango::
detail::
time_begin_period
()noexcept
{
  auto const a_period = time_min_period();

  auto const a_error = ::timeBeginPeriod(UINT(a_period));

  if(a_error == TIMERR_NOERROR)
  {
    return;
  }

  dango_unreachable_terminate_msg(u8"timeBeginPeriod failed");
}

void
dango::
detail::
time_end_period
()noexcept
{
  auto const a_period = time_min_period();

  auto const a_error = ::timeEndPeriod(UINT(a_period));

  if(a_error == TIMERR_NOERROR)
  {
    return;
  }

  dango_unreachable_terminate_msg(u8"timeEndPeriod failed");
}

namespace
{
  template
  <typename tp_func>
  auto __stdcall
  thread_start_address
  (void* const a_data)noexcept->dango::builtin::uint
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
      dango_unreachable_terminate_msg(u8"uncaught exception in thread");
    }

    return dango::builtin::uint(0);
  }

  auto
  perf_count
  ()noexcept->dango::tick_count_type
  {
    LARGE_INTEGER a_result;

    ::QueryPerformanceCounter(&a_result);

    return dango::tick_count_type(a_result.QuadPart);
  }

  auto
  time_min_period
  ()noexcept->dango::uint
  {
    constexpr auto c_impl =
    []()noexcept->dango::uint
    {
      ::TIMECAPS a_caps;

      auto const a_error = ::timeGetDevCaps(&a_caps, sizeof(a_caps));

      if(a_error == MMSYSERR_NOERROR)
      {
        return dango::uint(a_caps.wPeriodMin);
      }

      dango_unreachable_terminate_msg(u8"timeGetDevCaps failed");
    };

    static auto const s_period = c_impl();

    return s_period;
  }
}

#endif // _WIN32

