#ifndef __DANGO_SHARED_CONCURRENT_HPP__
#define __DANGO_SHARED_CONCURRENT_HPP__

#include "dango-shared.hpp"

namespace
dango::shared
{
  using thread_func_type = void(*)(void*)noexcept(false);

  auto DANGO_SHARED_API create_thread(shared::thread_func_type, void*)noexcept->bool;
  void DANGO_SHARED_API yield_thread()noexcept;
}

#ifdef __linux__

namespace
dango::shared
{
  auto DANGO_SHARED_API tick_count_monotonic()noexcept->dango::uint64;
  auto DANGO_SHARED_API tick_count_boottime()noexcept->dango::uint64;

  using futex_type = dango::s_int;

  void DANGO_SHARED_API futex_wait(shared::futex_type*, shared::futex_type)noexcept;
  void DANGO_SHARED_API futex_wait(shared::futex_type*, shared::futex_type, dango::uint64)noexcept;
  void DANGO_SHARED_API futex_wake(shared::futex_type*)noexcept;
  void DANGO_SHARED_API futex_wake_requeue(shared::futex_type*, shared::futex_type*)noexcept;
}

#endif /* __linux__ */

#ifdef _WIN32

namespace
dango::shared
{
  auto DANGO_SHARED_API perf_freq()noexcept->dango::uint64;
  auto DANGO_SHARED_API perf_count_suspend_bias(dango::uint64&)noexcept->dango::uint64;

  using srw_lock_storage = dango::aligned_storage<sizeof(void*), alignof(void*)>;

  void DANGO_SHARED_API srw_lock_init(void*)noexcept;
  void DANGO_SHARED_API srw_lock_acquire(void*)noexcept;
  auto DANGO_SHARED_API srw_lock_try_acquire(void*)noexcept->bool;
  void DANGO_SHARED_API srw_lock_release(void*)noexcept;

  using condition_variable_storage = dango::aligned_storage<dango::usize(2) * sizeof(void*), alignof(void*)>;

  void DANGO_SHARED_API condition_variable_init(void*)noexcept;
  void DANGO_SHARED_API condition_variable_wake(void*)noexcept;
  void DANGO_SHARED_API condition_variable_wake_all(void*)noexcept;
  void DANGO_SHARED_API condition_variable_wait(void*, void*)noexcept;
  void DANGO_SHARED_API condition_variable_wait(void*, void*, dango::uint64)noexcept;

  void DANGO_SHARED_API time_begin_period()noexcept;
  void DANGO_SHARED_API time_end_period()noexcept;
}

#endif /* _WIN32 */

#endif

