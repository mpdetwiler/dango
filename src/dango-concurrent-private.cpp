#include "dango-concurrent-private.hpp"

#ifdef DANGO_PLATFORM_LINUX_OR_APPLE

#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#ifdef DANGO_PLATFORM_APPLE
#include <mach/mach_time.h>
#endif

static_assert(sizeof(dango::detail::mutex_storage) >= sizeof(::pthread_mutex_t));
static_assert(alignof(dango::detail::mutex_storage) >= alignof(::pthread_mutex_t));
static_assert(sizeof(dango::detail::cond_var_storage) >= sizeof(::pthread_cond_t));
static_assert(alignof(dango::detail::cond_var_storage) >= alignof(::pthread_cond_t));

namespace
{
  template
  <typename tp_func>
  auto thread_start_address(void*)noexcept->void*;

  auto tick_count_help(bool)noexcept->dango::tick_count_type;
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
detail::
thread_sleep
(dango::uint const a_ms)noexcept
{
  if(a_ms == dango::uint(0))
  {
    ::sched_yield();

    return;
  }

  auto const a_div = a_ms / dango::uint(1'000);
  auto const a_mod = a_ms % dango::uint(1'000);

  ::timespec a_req;

  a_req.tv_sec = a_div;
  a_req.tv_nsec = a_mod * dango::uint(1'000'000);

  ::timespec a_rem;

  auto a_req_ptr = &a_req;
  auto a_rem_ptr = &a_rem;

#ifdef DANGO_PLATFORM_LINUX
  while(::clock_nanosleep(CLOCK_MONOTONIC, 0, a_req_ptr, a_rem_ptr))
#else
  while(::nanosleep(a_req_ptr, a_rem_ptr))
#endif
  {
    dango_assert(errno == EINTR);

    dango::swap(a_req_ptr, a_rem_ptr);
  }
}

auto
dango::
detail::
tick_count_monotonic
()noexcept->dango::tick_count_type
{
  return tick_count_help(false);
}

auto
dango::
detail::
tick_count_boottime
()noexcept->dango::tick_count_type
{
  return tick_count_help(true);
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
#ifdef DANGO_PLATFORM_LINUX
  constexpr auto const c_cond_clock = ::clockid_t(CLOCK_MONOTONIC);
#endif
}

void
dango::
detail::
pthread_cond_init
(dango::detail::cond_var_storage& a_storage)noexcept
{
  using type = ::pthread_cond_t;

  auto const a_temp = dango_placement_new(a_storage.get(), type);

#ifdef DANGO_PLATFORM_LINUX
  ::pthread_condattr_t a_spec;

  auto a_result = ::pthread_condattr_init(&a_spec);

  dango_assert(a_result == 0);

  a_result = ::pthread_condattr_setclock(&a_spec, c_cond_clock);

  dango_assert(a_result == 0);

  a_result = ::pthread_cond_init(a_temp, &a_spec);

  dango_assert(a_result == 0);

  a_result = ::pthread_condattr_destroy(&a_spec);

  dango_assert(a_result == 0);
#else
  auto const a_result = ::pthread_cond_init(a_temp, dango::null);

  dango_assert(a_result == 0);
#endif
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
(dango::detail::cond_var_storage& a_storage, dango::detail::mutex_storage& a_lock_storage, dango::ulong const a_ms)noexcept
{
  using type = ::pthread_cond_t;
  using lock_type = ::pthread_mutex_t;
  using u64 = dango::ulong;

  if(a_ms == u64(0))
  {
    return;
  }

  ::timespec a_spec;

#ifdef DANGO_PLATFORM_LINUX
  ::clock_gettime(c_cond_clock, &a_spec);
#endif

  {
    auto const a_sec = a_ms / u64(1'000L);
    auto const a_nsec = (a_ms % u64(1'000L)) * u64(1'000'000L);
#ifdef DANGO_PLATFORM_LINUX
    auto const a_sum = u64(a_spec.tv_nsec) + a_nsec;
    dango_assert(a_sum < u64(2'000'000'000L));
    a_spec.tv_sec += (a_sec + (a_sum / u64(1'000'000'000L)));
    a_spec.tv_nsec = (a_sum % u64(1'000'000'000L));
#else
    a_spec.tv_sec = a_sec;
    a_spec.tv_nsec = a_nsec;
#endif
  }

  auto const a_result =
#ifdef DANGO_PLATFORM_LINUX
    ::pthread_cond_timedwait(a_storage.launder_get<type>(), a_lock_storage.launder_get<lock_type>(), &a_spec);
#else
    ::pthread_cond_timedwait_relative_np(a_storage.launder_get<type>(), a_lock_storage.launder_get<lock_type>(), &a_spec);
#endif

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
  (bool const a_biased)noexcept->dango::tick_count_type
#ifdef DANGO_PLATFORM_LINUX
  {
    ::clockid_t const a_clock = a_biased ? CLOCK_BOOTTIME : CLOCK_MONOTONIC;

    ::timespec a_spec;

    auto const a_result =
      ::clock_gettime(a_clock, &a_spec);

    dango_assert(a_result == 0);

    using i64 = dango::slong;

    auto const a_sec = i64(a_spec.tv_sec);
    auto const a_nsec = i64(a_spec.tv_nsec);

    return dango::tick_count_type{ a_sec * i64(1'000'000'000L) + a_nsec };
  }
#else
  {
    static constexpr auto const c_get_info =
      []()noexcept->auto
      {
        ::mach_timebase_info_data_t a_result;

        auto const a_ret = ::mach_timebase_info(&a_result);

        dango_assert(a_ret == KERN_SUCCESS);

        return dango::make_pair(a_result.numer, a_result.denom);
      };

    static auto const s_timebase_info = c_get_info();

    using u64 = dango::ulong;

    u64 a_result;

    if(a_biased)
    {
      a_result = ::mach_continuous_time();
    }
    else
    {
      a_result = ::mach_absolute_time();
    }

    a_result *= u64(s_timebase_info.first());
    a_result /= u64(s_timebase_info.second());

    return dango::tick_count_type{ dango::slong(a_result) };
  }
#endif
}

#endif // DANGO_PLATFORM_LINUX_OR_APPLE

#ifdef DANGO_PLATFORM_WINDOWS

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
  auto __stdcall thread_start_address(void*)noexcept->dango::integer::u_int;

  auto perf_freq()noexcept->dango::ulong;
  auto perf_count()noexcept->dango::ulong;
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
detail::
thread_sleep
(dango::uint const a_ms)noexcept
{
  ::Sleep(DWORD(a_ms));
}

auto
dango::
detail::
perf_count_suspend_bias
()noexcept->dango::tick_count_pair
{
  using u64 = dango::ulong;

  static auto& s_bias_ref =
    *reinterpret_cast<ULONGLONG const volatile*>(dango::uptr(0x7F'FE'00'00u + 0x00'00'03'B0u));

  u64 a_tick;
  u64 a_bias;

  {
    u64 a_temp;

    do
    {
      a_bias = s_bias_ref;

      a_tick = perf_count();

      a_temp = s_bias_ref;
    }
    while(a_bias != a_temp);
  }

  auto const a_freq = perf_freq();
  auto const a_sec = a_tick / a_freq;
  auto const a_rem = a_tick % a_freq;

  constexpr auto const c_bil = u64(1'000'000'000L);

  a_tick = a_sec * c_bil + (a_rem * c_bil) / a_freq;
  a_bias *= u64(100L);

  using dango::tick::from_nano;

  return dango::tick_count_pair{ from_nano(a_tick), from_nano(a_bias) };
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
(dango::detail::cond_var_storage& a_storage, dango::detail::mutex_storage& a_lock_storage, dango::ulong const a_ms)noexcept
{
  using type = ::CONDITION_VARIABLE;
  using lock_type = ::SRWLOCK;
  using u64 = dango::ulong;

  if(a_ms == u64(0))
  {
    return;
  }

  static constexpr auto const c_max_interval = u64(DWORD(INFINITE) - DWORD(1));

  auto const a_spec = dango::min(a_ms, c_max_interval);

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
  (void* const a_data)noexcept->dango::integer::u_int
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

    return dango::integer::u_int(0);
  }

  auto
  perf_freq
  ()noexcept->dango::ulong
  {
    static constexpr auto const c_impl =
      []()noexcept->dango::ulong
      {
        LARGE_INTEGER a_result;

        ::QueryPerformanceFrequency(&a_result);

        return dango::ulong(a_result.QuadPart);
      };

    static auto const s_result = c_impl();

    return s_result;
  }

  auto
  perf_count
  ()noexcept->dango::ulong
  {
    LARGE_INTEGER a_result;

    ::QueryPerformanceCounter(&a_result);

    return dango::ulong(a_result.QuadPart);
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

#endif // DANGO_PLATFORM_WINDOWS
