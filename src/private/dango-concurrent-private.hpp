#ifndef DANGO_PRIVATE_CONCURRENT_HPP_INCLUDED
#define DANGO_PRIVATE_CONCURRENT_HPP_INCLUDED

#include "dango-private.hpp"

namespace
dango::detail
{
  using thread_func_type = void(*)(void*)noexcept(false);

  auto create_thread(dango::detail::thread_func_type, void*)noexcept->bool;
}

#ifdef DANGO_PLATFORM_LINUX_OR_APPLE

namespace
dango::detail
{
  auto tick_count_monotonic()noexcept->dango::tick_count_type;
  auto tick_count_boottime()noexcept->dango::tick_count_type;

  void pthread_mutex_init(dango::detail::mutex_storage&)noexcept;
  void pthread_mutex_destroy(dango::detail::mutex_storage&)noexcept;
  void pthread_mutex_lock(dango::detail::mutex_storage&)noexcept;
  auto pthread_mutex_try_lock(dango::detail::mutex_storage&)noexcept->bool;
  void pthread_mutex_unlock(dango::detail::mutex_storage&)noexcept;

  void pthread_cond_init(dango::detail::cond_var_storage&)noexcept;
  void pthread_cond_destroy(dango::detail::cond_var_storage&)noexcept;
  void pthread_cond_signal(dango::detail::cond_var_storage&)noexcept;
  void pthread_cond_broadcast(dango::detail::cond_var_storage&)noexcept;
  void pthread_cond_wait(dango::detail::cond_var_storage&, dango::detail::mutex_storage&)noexcept;
  void pthread_cond_wait(dango::detail::cond_var_storage&, dango::detail::mutex_storage&, dango::tick_count_type)noexcept;
}

#endif // DANGO_PLATFORM_LINUX_OR_APPLE

#ifdef DANGO_PLATFORM_WINDOWS

namespace
dango::detail
{
  auto perf_freq()noexcept->dango::tick_count_type;
  auto perf_count_suspend_bias()noexcept->dango::tick_count_pair;

  void srw_lock_init(dango::detail::mutex_storage&)noexcept;
  void srw_lock_destroy(dango::detail::mutex_storage&)noexcept;
  void srw_lock_acquire(dango::detail::mutex_storage&)noexcept;
  auto srw_lock_try_acquire(dango::detail::mutex_storage&)noexcept->bool;
  void srw_lock_release(dango::detail::mutex_storage&)noexcept;

  void condition_variable_init(dango::detail::cond_var_storage&)noexcept;
  void condition_variable_destroy(dango::detail::cond_var_storage&)noexcept;
  void condition_variable_wake(dango::detail::cond_var_storage&)noexcept;
  void condition_variable_wake_all(dango::detail::cond_var_storage&)noexcept;
  void condition_variable_wait(dango::detail::cond_var_storage&, dango::detail::mutex_storage&)noexcept;
  void condition_variable_wait(dango::detail::cond_var_storage&, dango::detail::mutex_storage&, dango::tick_count_type)noexcept;

  void time_begin_period()noexcept;
  void time_end_period()noexcept;
}

#endif // DANGO_PLATFORM_WINDOWS

#endif // DANGO_SHARED_CONCURRENT_HPP_INCLUDED
