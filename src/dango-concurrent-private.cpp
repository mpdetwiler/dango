#include "dango-concurrent-private.hpp"

#ifdef __linux__

#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

static_assert(sizeof(dango::detail::mutex_storage) >= sizeof(::pthread_mutex_t));
static_assert(alignof(dango::detail::mutex_storage) >= alignof(::pthread_mutex_t));
static_assert(sizeof(dango::detail::cond_var_storage) >= sizeof(::pthread_cond_t));
static_assert(alignof(dango::detail::cond_var_storage) >= alignof(::pthread_cond_t));

namespace
{
  template
  <typename tp_func>
  auto thread_start_address(void*)noexcept->void*;

  auto tick_count_help(clockid_t)noexcept->dango::tick_count_type;
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

  dango::busy_wait_while([&a_starting]()noexcept->bool{ return a_starting.load<acquire>(); }, dango::uint(128));

  return true;
}

void
dango::
thread_yield
(dango::uint const a_ms)noexcept
{
  if(a_ms == dango::uint(0))
  {
    ::sched_yield();

    return;
  }

  auto const a_div = a_ms / dango::uint(1'000);
  auto const a_mod = a_ms % dango::uint(1'000);

  ::timespec a_spec;

  a_spec.tv_sec = a_div;
  a_spec.tv_nsec = a_mod * dango::uint(1'000'000);

  ::nanosleep(&a_spec, dango::null);
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

/*****************************/

void
dango::
detail::
pthread_mutex_init
(dango::detail::mutex_storage& a_storage)noexcept
{
  using type = ::pthread_mutex_t;

  auto const a_temp = dango_placement_new(a_storage.get(), type);

  auto const a_result = ::pthread_mutex_init(a_temp, dango::null);

  dango_assert(a_result == 0);
}

void
dango::
detail::
pthread_mutex_destroy
(dango::detail::mutex_storage& a_storage)noexcept
{
  using type = ::pthread_mutex_t;

  auto const a_temp = a_storage.launder_get<type>();

  auto const a_result = ::pthread_mutex_destroy(a_temp);

  dango_assert(a_result == 0);

  dango::destructor(a_temp);
}

void
dango::
detail::
pthread_mutex_lock
(dango::detail::mutex_storage& a_storage)noexcept
{
  using type = ::pthread_mutex_t;

  auto const a_result = ::pthread_mutex_lock(a_storage.launder_get<type>());

  dango_assert(a_result == 0);
}

auto
dango::
detail::
pthread_mutex_try_lock
(dango::detail::mutex_storage& a_storage)noexcept->bool
{
  using type = ::pthread_mutex_t;

  auto const a_result = ::pthread_mutex_trylock(a_storage.launder_get<type>());

  return a_result == 0;
}

void
dango::
detail::
pthread_mutex_unlock
(dango::detail::mutex_storage& a_storage)noexcept
{
  using type = ::pthread_mutex_t;

  auto const a_result = ::pthread_mutex_unlock(a_storage.launder_get<type>());

  dango_assert(a_result == 0);
}

/*****************************/

namespace
{
  constexpr auto const c_cond_clock = ::clockid_t(CLOCK_MONOTONIC);
}

void
dango::
detail::
pthread_cond_init
(dango::detail::cond_var_storage& a_storage)noexcept
{
  using type = ::pthread_cond_t;

  auto const a_temp = dango_placement_new(a_storage.get(), type);

  ::pthread_condattr_t a_spec;

  auto a_result = ::pthread_condattr_init(&a_spec);

  dango_assert(a_result == 0);

  a_result = ::pthread_condattr_setclock(&a_spec, c_cond_clock);

  dango_assert(a_result == 0);

  a_result = ::pthread_cond_init(a_temp, &a_spec);

  dango_assert(a_result == 0);

  a_result = ::pthread_condattr_destroy(&a_spec);

  dango_assert(a_result == 0);
}

void
dango::
detail::
pthread_cond_destroy
(dango::detail::cond_var_storage& a_storage)noexcept
{
  using type = ::pthread_cond_t;

  auto const a_temp = a_storage.launder_get<type>();

  auto const a_result = ::pthread_cond_destroy(a_temp);

  dango_assert(a_result == 0);

  dango::destructor(a_temp);
}

void
dango::
detail::
pthread_cond_signal
(dango::detail::cond_var_storage& a_storage)noexcept
{
  using type = ::pthread_cond_t;

  auto const a_result = ::pthread_cond_signal(a_storage.launder_get<type>());

  dango_assert(a_result == 0);
}

void
dango::
detail::
pthread_cond_broadcast
(dango::detail::cond_var_storage& a_storage)noexcept
{
  using type = ::pthread_cond_t;

  auto const a_result = ::pthread_cond_broadcast(a_storage.launder_get<type>());

  dango_assert(a_result == 0);
}

void
dango::
detail::
pthread_cond_wait
(dango::detail::cond_var_storage& a_storage, dango::detail::mutex_storage& a_lock_storage)noexcept
{
  using type = ::pthread_cond_t;
  using lock_type = ::pthread_mutex_t;

  auto const a_result =
    ::pthread_cond_wait(a_storage.launder_get<type>(), a_lock_storage.launder_get<lock_type>());

  dango_assert(a_result == 0);
}

void
dango::
detail::
pthread_cond_wait
(dango::detail::cond_var_storage& a_storage, dango::detail::mutex_storage& a_lock_storage, dango::tick_count_type const a_interval)noexcept
{
  using type = ::pthread_cond_t;
  using lock_type = ::pthread_mutex_t;

  using tc64 = dango::tick_count_type;

  dango_assert(a_interval >= tc64(0));

  ::timespec a_spec;

  ::clock_gettime(c_cond_clock, &a_spec);

  {
    auto const a_sec = a_interval / tc64(1'000);
    auto const a_nsec = (a_interval % tc64(1'000)) * tc64(1'000'000);
    auto const a_sum = tc64(a_spec.tv_nsec) + a_nsec;

    a_spec.tv_sec += a_sec + (a_sum / tc64(1'000'000'000));
    a_spec.tv_nsec = a_sum % tc64(1'000'000'000);
  }

  auto const a_result =
    ::pthread_cond_timedwait(a_storage.launder_get<type>(), a_lock_storage.launder_get<lock_type>(), &a_spec);

  dango_assert(a_result == 0 || a_result == ETIMEDOUT);
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
}

#endif // __linux__

#ifdef _WIN32

#include <windows.h>
#include <process.h>

static_assert(sizeof(dango::detail::mutex_storage) >= sizeof(::SRWLOCK));
static_assert(alignof(dango::detail::mutex_storage) >= alignof(::SRWLOCK));
static_assert(sizeof(dango::detail::cond_var_storage) >= sizeof(::CONDITION_VARIABLE));
static_assert(alignof(dango::detail::cond_var_storage) >= alignof(::CONDITION_VARIABLE));

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

  dango::busy_wait_while([&a_starting]()noexcept->bool{ return a_starting.load<acquire>(); }, dango::uint(128));

  return true;
}

void
dango::
thread_yield
(dango::uint const a_ms)noexcept
{
  ::Sleep(DWORD(a_ms));
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
()noexcept->dango::tick_count_pair
{
  using tc64 = dango::tick_count_type;

  static auto& s_bias_ref =
    *reinterpret_cast<ULONGLONG const volatile*>(dango::uptr(0x7F'FE'00'00u + 0x00'00'03'B0u));

  tc64 a_count;
  tc64 a_bias;

  {
    tc64 a_temp;

    do
    {
      a_bias = tc64(s_bias_ref);

      a_count = perf_count();

      a_temp = tc64(s_bias_ref);
    }
    while(a_bias != a_temp);
  }

  return dango::tick_count_pair{ a_count, a_bias };
}

/*****************************/

void
dango::
detail::
srw_lock_init
(dango::detail::mutex_storage& a_storage)noexcept
{
  using type = ::SRWLOCK;

  auto const a_temp = dango_placement_new(a_storage.get(), type);

  ::InitializeSRWLock(a_temp);
}

void
dango::
detail::
srw_lock_destroy
(dango::detail::mutex_storage& a_storage)noexcept
{
  using type = ::SRWLOCK;

  dango::destructor(a_storage.launder_get<type>());
}

void
dango::
detail::
srw_lock_acquire
(dango::detail::mutex_storage& a_storage)noexcept
{
  using type = ::SRWLOCK;

  ::AcquireSRWLockExclusive(a_storage.launder_get<type>());
}

auto
dango::
detail::
srw_lock_try_acquire
(dango::detail::mutex_storage& a_storage)noexcept->bool
{
  using type = ::SRWLOCK;

  auto const a_result = ::TryAcquireSRWLockExclusive(a_storage.launder_get<type>());

  return bool(a_result);
}

void
dango::
detail::
srw_lock_release
(dango::detail::mutex_storage& a_storage)noexcept
{
  using type = ::SRWLOCK;

  ::ReleaseSRWLockExclusive(a_storage.launder_get<type>());
}

/*****************************/

void
dango::
detail::
condition_variable_init
(dango::detail::cond_var_storage& a_storage)noexcept
{
  using type = ::CONDITION_VARIABLE;

  auto const a_temp = dango_placement_new(a_storage.get(), type);

  ::InitializeConditionVariable(a_temp);
}

void
dango::
detail::
condition_variable_destroy
(dango::detail::cond_var_storage& a_storage)noexcept
{
  using type = ::CONDITION_VARIABLE;

  dango::destructor(a_storage.launder_get<type>());
}

void
dango::
detail::
condition_variable_wake
(dango::detail::cond_var_storage& a_storage)noexcept
{
  using type = ::CONDITION_VARIABLE;

  ::WakeConditionVariable(a_storage.launder_get<type>());
}

void
dango::
detail::
condition_variable_wake_all
(dango::detail::cond_var_storage& a_storage)noexcept
{
  using type = ::CONDITION_VARIABLE;

  ::WakeAllConditionVariable(a_storage.launder_get<type>());
}

void
dango::
detail::
condition_variable_wait
(dango::detail::cond_var_storage& a_storage, dango::detail::mutex_storage& a_lock_storage)noexcept
{
  using type = ::CONDITION_VARIABLE;
  using lock_type = ::SRWLOCK;

  ::SleepConditionVariableSRW
  (
    a_storage.launder_get<type>(),
    a_lock_storage.launder_get<lock_type>(),
    DWORD(INFINITE),
    ULONG(0)
  );
}

void
dango::
detail::
condition_variable_wait
(dango::detail::cond_var_storage& a_storage, dango::detail::mutex_storage& a_lock_storage, dango::tick_count_type const a_interval)noexcept
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
    a_storage.launder_get<type>(),
    a_lock_storage.launder_get<lock_type>(),
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

