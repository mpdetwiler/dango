#ifndef DANGO_SHARED_CONCURRENT_HPP_INCLUDED
#define DANGO_SHARED_CONCURRENT_HPP_INCLUDED

#include "dango-private.hpp"

namespace
dango::shared
{
  using thread_func_type = void(*)(void*)noexcept(false);

  DANGO_EXPORT auto create_thread(shared::thread_func_type, void*)noexcept->bool;
}

#ifdef __linux__

namespace
dango::shared
{
  DANGO_EXPORT auto tick_count_monotonic()noexcept->dango::tick_count_type;
  DANGO_EXPORT auto tick_count_boottime()noexcept->dango::tick_count_type;

  using futex_type = dango::builtin::sint;

  DANGO_EXPORT void futex_wait(shared::futex_type*, shared::futex_type)noexcept;
  DANGO_EXPORT void futex_wait(shared::futex_type*, shared::futex_type, dango::tick_count_type)noexcept;
  DANGO_EXPORT void futex_wake(shared::futex_type*)noexcept;
  DANGO_EXPORT void futex_wake_requeue(shared::futex_type*, shared::futex_type*)noexcept;
}

#endif // __linux__

#ifdef _WIN32

namespace
dango::shared
{
  DANGO_EXPORT auto perf_freq()noexcept->dango::tick_count_type;
  DANGO_EXPORT auto perf_count_suspend_bias(dango::tick_count_type&)noexcept->dango::tick_count_type;

  using srw_lock_storage = dango::aligned_storage<sizeof(void*), alignof(void*)>;

  DANGO_EXPORT void srw_lock_init(void*)noexcept;
  DANGO_EXPORT void srw_lock_acquire(void*)noexcept;
  DANGO_EXPORT auto srw_lock_try_acquire(void*)noexcept->bool;
  DANGO_EXPORT void srw_lock_release(void*)noexcept;

  using condition_variable_storage = dango::aligned_storage<sizeof(void*), alignof(void*)>;

  DANGO_EXPORT void condition_variable_init(void*)noexcept;
  DANGO_EXPORT void condition_variable_wake(void*)noexcept;
  DANGO_EXPORT void condition_variable_wake_all(void*)noexcept;
  DANGO_EXPORT void condition_variable_wait(void*, void*)noexcept;
  DANGO_EXPORT void condition_variable_wait(void*, void*, dango::tick_count_type)noexcept;

  DANGO_EXPORT void time_begin_period()noexcept;
  DANGO_EXPORT void time_end_period()noexcept;
}

#endif // _WIN32

#endif // DANGO_SHARED_CONCURRENT_HPP_INCLUDED

