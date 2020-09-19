#include "dango.hpp"

/*** tick_count ***/

namespace
{
  auto tick_count_suspend_bias_help()noexcept->dango::tick_count_pair;
}

auto
dango::
current_tick_and_suspend_bias
()noexcept->dango::tick_count_pair
{
  static auto const s_init = tick_count_suspend_bias_help();
  static thread_local auto t_last = s_init;

  auto a_current = tick_count_suspend_bias_help();
  auto& [a_tick, a_bias] = a_current;

  a_tick = dango::max(a_tick, t_last.first());
  a_bias = dango::max(a_bias, t_last.second());
  t_last = a_current;

  a_tick -= s_init.first();
  a_bias -= s_init.second();

  return a_current;
}

/*** thread ***/

constinit
dango::thread::registry
dango::
thread::
s_registry{ };

auto
dango::
thread::
thread_self_init
(bool const a_daemon)noexcept->thread const&
{
  static thread_local thread const t_thread{ construct_tag{ }, a_daemon };

  static thread_local notifier const t_notifier{ t_thread };

  return t_thread;
}

auto
dango::
thread::
thread_self_access_check
(bool const a_clear, void const** const a_ID)noexcept->bool
{
  static thread_local constinit bool t_access = true;

  if(a_clear)
  {
    t_access = false;
  }

  if(a_ID)
  {
    *a_ID = &t_access;
  }

  return t_access;
}

void
dango::
thread::
sleep
(dango::timeout const& a_timeout)noexcept
{
  static constinit dango::static_mutex s_mutex{ };
  static constinit dango::static_cond_var s_cond{ s_mutex };

  if(a_timeout.has_expired())
  {
    return;
  }

  dango_crit_full(s_cond, a_crit)
  {
    while(!a_timeout.has_expired())
    {
      a_crit.wait(a_timeout);
    }
  }
}

/*** bad_thread ***/

auto
dango::
bad_thread::
message()const noexcept->dango::bchar const*
{
  return u8"dango::bad_thread";
}

/*** cond_var_registry ***/

constexpr
dango::
detail::
cond_var_registry::
cond_var_registry
()noexcept:
m_mutex{ },
m_cond{ m_mutex },
m_list{ { }, { } },
m_external_list{ &m_list[0] },
m_internal_list{ &m_list[1] },
m_alive{ true },
m_waiting{ false }
{

}

constinit
dango::detail::cond_var_registry
dango::
detail::
cond_var_registry_access::
s_registry{ };

void
dango::
detail::
cond_var_registry::
regist
(cond_type* const a_cond, dango::timeout const& a_timeout)noexcept
{
  dango_assert(a_cond != dango::null);

  if(!a_timeout.is_suspend_aware())
  {
    return;
  }

  dango_crit_full(m_cond, a_crit)
  {
    if(!a_cond->increment_suspend_aware_count(a_crit))
    {
      return;
    }

    add(a_cond);

    if(m_waiting)
    {
      a_crit.notify();
    }
  }
}

void
dango::
detail::
cond_var_registry::
unregist
(cond_type* const a_cond, dango::timeout const& a_timeout)noexcept
{
  dango_assert(a_cond != dango::null);

  if(!a_timeout.is_suspend_aware())
  {
    return;
  }

  dango_crit_full(m_cond, a_crit)
  {
    if(!a_cond->decrement_suspend_aware_count(a_crit))
    {
      return;
    }

    remove(a_cond);

    if(m_waiting)
    {
      a_crit.notify();
    }
  }
}

void
dango::
detail::
cond_var_registry::
notify_exit
()noexcept
{
  dango_crit_full(m_cond, a_crit)
  {
    m_alive = false;

    if(m_waiting)
    {
      a_crit.notify();
    }
  }
}

void
dango::
detail::
cond_var_registry::
thread_func
()noexcept
{
  do
  {
    if(wait_empty())
    {
      break;
    }

    auto [a_tick, a_bias] = dango::current_tick_and_suspend_bias();

    auto a_timeout = dango::timeout::make(a_tick);

    while(poll_bias(a_bias, a_timeout));
  }
  while(true);
}

void
dango::
detail::
cond_var_registry::
add
(cond_type* const a_cond)noexcept
{
  m_external_list->add_last(a_cond);
}

void
dango::
detail::
cond_var_registry::
remove
(cond_type* const a_cond)noexcept
{
  list_type::remove(a_cond);
}

auto
dango::
detail::
cond_var_registry::
wait_empty
()noexcept->bool
{
  dango_crit_full(m_cond, a_crit)
  {
    do
    {
      if(!m_external_list->is_empty())
      {
        break;
      }

      if(!m_alive)
      {
        return true;
      }

      m_waiting = true;

      a_crit.wait();

      m_waiting = false;
    }
    while(true);
  }

  return false;
}

auto
dango::
detail::
cond_var_registry::
poll_bias
(dango::tick_count_type& a_prev_bias, dango::timeout& a_timeout)noexcept->bool
{
  bool a_alive;

  dango_crit_full(m_cond, a_crit)
  {
    do
    {
      if(m_external_list->is_empty())
      {
        return false;
      }

      a_alive = m_alive;

      if(a_timeout.has_expired())
      {
        a_timeout.add(dango::tick::from_sec(1));

        break;
      }

      if(!a_alive)
      {
        break;
      }

      m_waiting = true;

      a_crit.wait(a_timeout);

      m_waiting = false;
    }
    while(true);

    if(bias_okay(a_prev_bias))
    {
      return a_alive;
    }

    dango::swap(m_external_list, m_internal_list);

    dango_assert(m_external_list->is_empty());
  }

  while(pop_internal());

  return a_alive;
}

auto
dango::
detail::
cond_var_registry::
bias_okay
(dango::tick_count_type& a_prev_bias)noexcept->bool
{
  auto const a_bias = dango::current_suspend_bias();

  auto const a_delta = dango::tick::to_milli(a_bias - a_prev_bias);

  a_prev_bias = a_bias;

  return a_delta <= dango::slong(1);
}

auto
dango::
detail::
cond_var_registry::
pop_internal
()noexcept->bool
{
  auto a_guard = m_mutex.lock();

    if(m_internal_list->is_empty())
    {
      return false;
    }

    auto const a_cond_control = m_internal_list->first();

    remove(a_cond_control);

    add(a_cond_control);

    auto a_cond = a_cond_control->make_reference();

  a_guard.unlock();

  dango_crit_full(a_cond, a_crit)
  {
    a_crit.notify_all();
  }

  return true;
}

/*** cond_var_registry_thread ***/

namespace
dango::detail
{
  DANGO_DEFINE_GLOBAL_EXTERN(s_cond_var_registry_thread, { })
}

auto
dango::
detail::
cond_var_registry_thread::
start_thread
()noexcept->dango::thread
{
  auto& a_registry = dango::detail::cond_var_registry_access::s_registry;

  try
  {
    return dango::thread::create_daemon([]()noexcept->void{ a_registry.thread_func(); });
  }
  catch(...)
  {
    dango_unreachable_terminate_msg(u8"cond_var watcher-thread creation failed");
  }
}

void
dango::
detail::
cond_var_registry_thread::
stop_thread
(dango::thread const& a_thread)noexcept
{
  auto& a_registry = dango::detail::cond_var_registry_access::s_registry;

  a_registry.notify_exit();

  a_thread.join();
}

/*** mutex_control ***/

auto
dango::
detail::
mutex_control::
storage
()noexcept->dango::detail::mutex_storage&
{
  return m_storage;
}

auto
dango::
detail::
mutex_get_storage
(dango::detail::mutex_base const& a_mutex)noexcept->dango::detail::mutex_storage&
{
  return a_mutex.get_control()->storage();
}

/*** cond_var_control ***/

auto
dango::
detail::
cond_var_control::
storage
()noexcept->dango::detail::cond_var_storage&
{
  return m_storage;
}

auto
dango::
detail::
cond_var_control::
increment_suspend_aware_count
(dango::crit_section const&)noexcept->bool
{
  return m_suspend_aware_count++ == count_type(0);
}

auto
dango::
detail::
cond_var_control::
decrement_suspend_aware_count
(dango::crit_section const&)noexcept->bool
{
  dango_assert(m_suspend_aware_count != count_type(0));

  return --m_suspend_aware_count == count_type(0);
}

auto
dango::
detail::
cond_var_control::
make_reference
()noexcept->dango::cond_var
{
  return dango::cond_var{ dango::cond_var::increment_tag{ }, this };
}

#include "dango-concurrent-private.hpp"

/*** thread ***/

void
dango::
thread::
start_thread
(
  thread_start_func const a_thread_func,
  void* const a_thread_data
)
noexcept(false)
{
  if(dango::detail::create_thread(a_thread_func, a_thread_data))
  {
    return;
  }

  throw dango::bad_thread{ };
}

#ifdef DANGO_PLATFORM_LINUX_OR_APPLE

/*** tick_count_suspend_bias_help ***/

namespace
{
  auto
  tick_count_suspend_bias_help
  ()noexcept->dango::tick_count_pair
  {
    using dango::tick::to_milli;
    using dango::tick::from_milli;
    using dango::tick::zero;
    using tc64 = dango::tick_count_type;

    tc64 a_mono;
    tc64 a_boot;

    {
      tc64 a_temp;

      do
      {
        a_temp = dango::detail::tick_count_boottime();

        a_mono = dango::detail::tick_count_monotonic();

        a_boot = dango::detail::tick_count_boottime();
      }
      while(to_milli(a_boot - a_temp) != dango::slong(0));
    }

    auto const a_bias = to_milli(dango::max(zero, a_boot - a_mono));

    return dango::tick_count_pair{ a_mono , from_milli(a_bias) };
  }
}

/*** mutex_control ***/

void
dango::
detail::
mutex_control::
initialize
()noexcept
{
  dango::detail::pthread_mutex_init(storage());
}

void
dango::
detail::
mutex_control::
destroy
()noexcept
{
  dango::detail::pthread_mutex_destroy(storage());
}

auto
dango::
detail::
mutex_control::
acquire
()noexcept->mutex_control*
{
  dango_assert_noassume(m_init.has_executed());

  dango::detail::pthread_mutex_lock(storage());

  return this;
}

auto
dango::
detail::
mutex_control::
try_acquire
()noexcept->mutex_control*
{
  dango_assert_noassume(m_init.has_executed());

  if(dango::detail::pthread_mutex_try_lock(storage()))
  {
    return this;
  }

  return dango::null;
}

void
dango::
detail::
mutex_control::
release
()noexcept
{
  dango_assert_noassume(m_init.has_executed());

  dango::detail::pthread_mutex_unlock(storage());
}

/*** cond_var_control ***/

void
dango::
detail::
cond_var_control::
initialize
()noexcept
{
  dango::detail::pthread_cond_init(storage());
}

void
dango::
detail::
cond_var_control::
destroy
()noexcept
{
  dango::detail::pthread_cond_destroy(storage());
}

void
dango::
detail::
cond_var_control::
wait
()noexcept
{
  dango_assert_noassume(m_init.has_executed());

  dango::detail::pthread_cond_wait(storage(), dango::detail::mutex_get_storage(mutex_ref()));
}

void
dango::
detail::
cond_var_control::
wait
(dango::timeout const& a_timeout)noexcept
{
  dango_assert_noassume(m_init.has_executed());

  if(a_timeout.remaining_ms() == dango::ulong(0))
  {
    return;
  }

  auto& a_registry = dango::detail::cond_var_registry_access::s_registry;

  a_registry.regist(this, a_timeout);

  dango::detail::pthread_cond_wait(storage(), dango::detail::mutex_get_storage(mutex_ref()), a_timeout.remaining_ms());

  a_registry.unregist(this, a_timeout);
}

void
dango::
detail::
cond_var_control::
notify
()noexcept
{
  dango_assert_noassume(m_init.has_executed());

  dango::detail::pthread_cond_signal(storage());
}

void
dango::
detail::
cond_var_control::
notify_all
()noexcept
{
  dango_assert_noassume(m_init.has_executed());

  dango::detail::pthread_cond_broadcast(storage());
}

#endif // DANGO_PLATFORM_LINUX_OR_APPLE

#ifdef DANGO_PLATFORM_WINDOWS

/*** tick_count_suspend_bias_help ***/

namespace
{
  auto
  tick_count_suspend_bias_help
  ()noexcept->dango::tick_count_pair
  {
    auto a_current = dango::detail::perf_count_suspend_bias();
    auto& [a_tick, a_bias] = a_current;

    dango_assert(a_tick >= a_bias);

    a_tick -= a_bias;

    return a_current;
  }
}

/*** mutex_control ***/

void
dango::
detail::
mutex_control::
initialize
()noexcept
{
  dango::detail::srw_lock_init(storage());
}

void
dango::
detail::
mutex_control::
destroy
()noexcept
{
  dango::detail::srw_lock_destroy(storage());
}

auto
dango::
detail::
mutex_control::
acquire
()noexcept->mutex_control*
{
  dango_assert_noassume(m_init.has_executed());

  dango::detail::srw_lock_acquire(storage());

  return this;
}

auto
dango::
detail::
mutex_control::
try_acquire
()noexcept->mutex_control*
{
  dango_assert_noassume(m_init.has_executed());

  if(dango::detail::srw_lock_try_acquire(storage()))
  {
    return this;
  }

  return dango::null;
}

void
dango::
detail::
mutex_control::
release
()noexcept
{
  dango_assert_noassume(m_init.has_executed());

  dango::detail::srw_lock_release(storage());
}

/*** cond_var_control ***/

void
dango::
detail::
cond_var_control::
initialize
()noexcept
{
  dango::detail::condition_variable_init(storage());
}

void
dango::
detail::
cond_var_control::
destroy
()noexcept
{
  dango::detail::condition_variable_destroy(storage());
}

void
dango::
detail::
cond_var_control::
wait
()noexcept
{
  dango_assert_noassume(m_init.has_executed());

  dango::detail::condition_variable_wait(storage(), dango::detail::mutex_get_storage(mutex_ref()));
}

void
dango::
detail::
cond_var_control::
wait
(dango::timeout const& a_timeout)noexcept
{
  dango_assert_noassume(m_init.has_executed());

  if(a_timeout.remaining_ms() == dango::ulong(0))
  {
    return;
  }

  auto& a_registry = dango::detail::cond_var_registry_access::s_registry;

  auto& a_manager = dango::detail::windows_timer_res_access::s_manager;

  a_registry.regist(this, a_timeout);

  a_manager.activate(a_timeout);

  dango::detail::condition_variable_wait(storage(), dango::detail::mutex_get_storage(mutex_ref()), a_timeout.remaining_ms());

  a_manager.deactivate(a_timeout);

  a_registry.unregist(this, a_timeout);
}

void
dango::
detail::
cond_var_control::
notify
()noexcept
{
  dango_assert_noassume(m_init.has_executed());

  dango::detail::condition_variable_wake(storage());
}

void
dango::
detail::
cond_var_control::
notify_all
()noexcept
{
  dango_assert_noassume(m_init.has_executed());

  dango::detail::condition_variable_wake_all(storage());
}

/*** windows_timer_res_manager ***/

constexpr
dango::
detail::
windows_timer_res_manager::
windows_timer_res_manager
()noexcept:
m_mutex{ },
m_cond{ m_mutex },
m_alive{ true },
m_waiting{ false },
m_timer_state{ timer_state::DEACTIVATED },
m_count{ dango::usize(0) }
{

}

constinit
dango::detail::windows_timer_res_manager
dango::
detail::
windows_timer_res_access::
s_manager{ };

void
dango::
detail::
windows_timer_res_manager::
activate
(dango::timeout const& a_timeout)noexcept
{
  if(!a_timeout.is_high_res())
  {
    return;
  }

  dango_crit_full(m_cond, a_crit)
  {
    if(m_count++ != dango::usize(0))
    {
      dango_assert(m_timer_state == timer_state::ACTIVATED);

      return;
    }

    dango_assert(m_timer_state != timer_state::ACTIVATED);

    if(m_timer_state == timer_state::DEACTIVATED)
    {
      dango::detail::time_begin_period();

      m_timer_state = timer_state::ACTIVATED;

      return;
    }

    dango_assert(m_timer_state == timer_state::DEACTIVATING);

    m_timer_state = timer_state::ACTIVATED;

    if(m_waiting)
    {
      a_crit.notify();
    }
  }
}

void
dango::
detail::
windows_timer_res_manager::
deactivate
(dango::timeout const& a_timeout)noexcept
{
  if(!a_timeout.is_high_res())
  {
    return;
  }

  dango_crit_full(m_cond, a_crit)
  {
    dango_assert(m_timer_state == timer_state::ACTIVATED);

    if(--m_count != dango::usize(0))
    {
      return;
    }

    if(!m_alive)
    {
      dango::detail::time_end_period();

      m_timer_state = timer_state::DEACTIVATED;

      return;
    }

    m_timer_state = timer_state::DEACTIVATING;

    if(m_waiting)
    {
      a_crit.notify();
    }
  }
}

void
dango::
detail::
windows_timer_res_manager::
notify_exit
()noexcept
{
  dango_crit_full(m_cond, a_crit)
  {
    dango_assert(m_alive);

    m_alive = false;

    if(m_timer_state == timer_state::DEACTIVATING)
    {
      dango::detail::time_end_period();

      m_timer_state = timer_state::DEACTIVATED;
    }

    if(m_waiting)
    {
      a_crit.notify();
    }
  }
}

void
dango::
detail::
windows_timer_res_manager::
thread_func
()noexcept
{
  do
  {
    if(wait())
    {
      break;
    }

    auto const a_timeout =
      dango::timeout::make_rel(dango::tick::from_sec(60));

    if(timed_wait(a_timeout))
    {
      break;
    }
  }
  while(true);
}

auto
dango::
detail::
windows_timer_res_manager::
wait
()noexcept->bool
{
  dango_crit_full(m_cond, a_crit)
  {
    do
    {
      if(!m_alive)
      {
        return true;
      }

      if(m_timer_state == timer_state::DEACTIVATING)
      {
        break;
      }

      m_waiting = true;

      a_crit.wait();

      m_waiting = false;
    }
    while(true);
  }

  return false;
}

auto
dango::
detail::
windows_timer_res_manager::
timed_wait
(dango::timeout const& a_timeout)noexcept->bool
{
  dango_crit_full(m_cond, a_crit)
  {
    do
    {
      if(!m_alive)
      {
        return true;
      }

      if(m_timer_state != timer_state::DEACTIVATING)
      {
        return false;
      }

      if(a_timeout.has_expired())
      {
        break;
      }

      m_waiting = true;

      a_crit.wait(a_timeout);

      m_waiting = false;
    }
    while(true);

    dango_assert(m_timer_state == timer_state::DEACTIVATING);

    dango::detail::time_end_period();

    m_timer_state = timer_state::DEACTIVATED;
  }

  return false;
}

/*** windows_timer_res_daemon ***/

namespace
dango::detail
{
  DANGO_DEFINE_GLOBAL_EXTERN(s_windows_timer_res_daemon, { })
}

auto
dango::
detail::
windows_timer_res_daemon::
start_thread
()noexcept->dango::thread
{
  auto& a_manager = dango::detail::windows_timer_res_access::s_manager;

  try
  {
    return dango::thread::create_daemon([]()noexcept->void{ a_manager.thread_func(); });
  }
  catch(...)
  {
    dango_unreachable_terminate_msg(u8"windows timer-res manager thread creation failed");
  }
}

void
dango::
detail::
windows_timer_res_daemon::
stop_thread
(dango::thread const& a_thread)noexcept
{
  auto& a_manager = dango::detail::windows_timer_res_access::s_manager;

  a_manager.notify_exit();

  a_thread.join();
}

#endif // DANGO_PLATFORM_WINDOWS
