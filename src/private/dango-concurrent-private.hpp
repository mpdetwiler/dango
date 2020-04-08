#ifndef DANGO_PRIVATE_CONCURRENT_HPP_INCLUDED
#define DANGO_PRIVATE_CONCURRENT_HPP_INCLUDED

#include "dango-private.hpp"

namespace
dango::detail
{
  using thread_func_type = void(*)(void*)noexcept(false);

  auto create_thread(dango::detail::thread_func_type, void*)noexcept->bool;
}

#ifdef __linux__

namespace
dango::detail
{
  auto tick_count_monotonic()noexcept->dango::tick_count_type;
  auto tick_count_boottime()noexcept->dango::tick_count_type;

  using futex_type = dango::builtin::sint;

  void futex_wait(dango::detail::futex_type*, dango::detail::futex_type)noexcept;
  void futex_wait(dango::detail::futex_type*, dango::detail::futex_type, dango::tick_count_type)noexcept;
  void futex_wake(dango::detail::futex_type*)noexcept;
  void futex_wake_requeue(dango::detail::futex_type*, dango::detail::futex_type*)noexcept;
}

#endif // __linux__

#ifdef _WIN32

namespace
dango::detail
{
  auto perf_freq()noexcept->dango::tick_count_type;
  auto perf_count_suspend_bias(dango::tick_count_type&)noexcept->dango::tick_count_type;

  using srw_lock_storage = dango::aligned_storage<sizeof(void*), alignof(void*)>;

  void srw_lock_init(void*)noexcept;
  void srw_lock_acquire(void*)noexcept;
  auto srw_lock_try_acquire(void*)noexcept->bool;
  void srw_lock_release(void*)noexcept;

  using condition_variable_storage = dango::aligned_storage<sizeof(void*), alignof(void*)>;

  void condition_variable_init(void*)noexcept;
  void condition_variable_wake(void*)noexcept;
  void condition_variable_wake_all(void*)noexcept;
  void condition_variable_wait(void*, void*)noexcept;
  void condition_variable_wait(void*, void*, dango::tick_count_type)noexcept;

  void time_begin_period()noexcept;
  void time_end_period()noexcept;
}

#endif // _WIN32

#endif // DANGO_SHARED_CONCURRENT_HPP_INCLUDED

