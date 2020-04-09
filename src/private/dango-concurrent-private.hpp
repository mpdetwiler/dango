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

  class
  srw_lock_storage
  final
  {
  public:
    explicit constexpr srw_lock_storage()noexcept = default;
    constexpr ~srw_lock_storage()noexcept = default;

    constexpr auto get()const noexcept->void*{ return m_storage.get(); }

    template
    <dango::is_object tp_type>
    constexpr auto launder_get()const noexcept->tp_type*;
  private:
    dango::aligned_storage<sizeof(void*), alignof(void*)> m_storage;
  public:
    DANGO_IMMOBILE(srw_lock_storage)
  };

  void srw_lock_init(dango::detail::srw_lock_storage*)noexcept;
  void srw_lock_acquire(dango::detail::srw_lock_storage*)noexcept;
  auto srw_lock_try_acquire(dango::detail::srw_lock_storage*)noexcept->bool;
  void srw_lock_release(dango::detail::srw_lock_storage*)noexcept;

  class
  condition_variable_storage
  final
  {
  public:
    explicit constexpr condition_variable_storage()noexcept = default;
    constexpr ~condition_variable_storage()noexcept = default;

    constexpr auto get()const noexcept->void*{ return m_storage.get(); }

    template
    <dango::is_object tp_type>
    constexpr auto launder_get()const noexcept->tp_type*;
  private:
    dango::aligned_storage<sizeof(void*), alignof(void*)> m_storage;
  public:
    DANGO_IMMOBILE(condition_variable_storage)
  };

  void condition_variable_init(dango::detail::condition_variable_storage*)noexcept;
  void condition_variable_wake(dango::detail::condition_variable_storage*)noexcept;
  void condition_variable_wake_all(dango::detail::condition_variable_storage*)noexcept;
  void condition_variable_wait(dango::detail::condition_variable_storage*, dango::detail::srw_lock_storage*)noexcept;
  void condition_variable_wait(dango::detail::condition_variable_storage*, dango::detail::srw_lock_storage*, dango::tick_count_type)noexcept;

  void time_begin_period()noexcept;
  void time_end_period()noexcept;
}

#endif // _WIN32

#endif // DANGO_SHARED_CONCURRENT_HPP_INCLUDED

