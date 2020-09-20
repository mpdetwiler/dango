#include "dango-cond-var.hpp"

#include "dango-mutex.hpp"
#include "dango-thread.hpp"

/*** cond_var_control ***/

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

/*** cond_var_registry ***/

namespace
dango::detail
{
  class cond_var_registry_access;
  class cond_var_registry;
}

class
dango::
detail::
cond_var_registry_access
final
{
  friend dango::detail::cond_var_control;
  friend dango::detail::cond_var_registry_thread;
private:
  static dango::detail::cond_var_registry s_registry;
public:
  DANGO_UNCONSTRUCTIBLE(cond_var_registry_access)
};

class
dango::
detail::
cond_var_registry
final
{
  friend dango::detail::cond_var_registry_access;
private:
  using cond_type = dango::detail::cond_var_control;
  using list_type = dango::intrusive_list<cond_type>;
private:
  static void remove(cond_type*)noexcept;
  static auto bias_okay(dango::tick_count_type&)noexcept->bool;
public:
  void regist(cond_type*, dango::timeout const&)noexcept;
  void unregist(cond_type*, dango::timeout const&)noexcept;
  void notify_exit()noexcept;
  void thread_func()noexcept;
private:
  constexpr explicit cond_var_registry()noexcept;
  constexpr ~cond_var_registry()noexcept = default;
private:
  void add(cond_type*)noexcept;
  auto wait_empty()noexcept->bool;
  auto poll_bias(dango::tick_count_type&, dango::timeout&)noexcept->bool;
  auto pop_internal()noexcept->bool;
private:
  dango::static_mutex m_mutex;
  dango::static_cond_var m_cond;
  list_type m_list[2];
  list_type* m_external_list;
  list_type* m_internal_list;
  bool m_alive;
  bool m_waiting;
public:
  DANGO_UNMOVEABLE(cond_var_registry)
};

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

constinit
dango::detail::cond_var_registry
dango::
detail::
cond_var_registry_access::
s_registry{ };

/*** cond_var_registry_thread ***/

auto
dango::
detail::
cond_var_registry_thread::
start_thread
()noexcept->dango::thread
{
  constexpr auto& c_registry = dango::detail::cond_var_registry_access::s_registry;

  try
  {
    return dango::thread::create_daemon([]()noexcept->void{ c_registry.thread_func(); });
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
  constexpr auto& c_registry = dango::detail::cond_var_registry_access::s_registry;

  c_registry.notify_exit();

  a_thread.join();
}

namespace
dango::detail
{
  DANGO_DEFINE_GLOBAL_EXTERN(s_cond_var_registry_thread, { })
}

/*** windows_timer_res_manager ***/

#ifdef DANGO_PLATFORM_WINDOWS

namespace
dango::detail
{
  class windows_timer_res_access;
  class windows_timer_res_manager;
}

class
dango::
detail::
windows_timer_res_access
final
{
  friend dango::detail::windows_timer_res_daemon;
  friend dango::detail::cond_var_control;
private:
  static dango::detail::windows_timer_res_manager s_manager;
public:
  DANGO_UNCONSTRUCTIBLE(windows_timer_res_access)
};

class
dango::
detail::
windows_timer_res_manager
final
{
  friend dango::detail::windows_timer_res_access;
private:
  enum class
  timer_state:
  dango::ubyte
  {
    ACTIVATED, DEACTIVATING, DEACTIVATED
  };
public:
  void activate(dango::timeout const&)noexcept;
  void deactivate(dango::timeout const&)noexcept;
  void notify_exit()noexcept;
  void thread_func()noexcept;
private:
  constexpr windows_timer_res_manager()noexcept;
  constexpr ~windows_timer_res_manager()noexcept = default;
private:
  auto wait()noexcept->bool;
  auto timed_wait(dango::timeout const&)noexcept->bool;
private:
  dango::static_mutex m_mutex;
  dango::static_cond_var m_cond;
  bool m_alive;
  bool m_waiting;
  timer_state m_timer_state;
  dango::usize m_count;
public:
  DANGO_UNMOVEABLE(windows_timer_res_manager)
};

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

constinit
dango::detail::windows_timer_res_manager
dango::
detail::
windows_timer_res_access::
s_manager{ };

/*** windows_timer_res_daemon ***/

auto
dango::
detail::
windows_timer_res_daemon::
start_thread
()noexcept->dango::thread
{
  constexpr auto& c_manager = dango::detail::windows_timer_res_access::s_manager;

  try
  {
    return dango::thread::create_daemon([]()noexcept->void{ c_manager.thread_func(); });
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
  constexpr auto& c_manager = dango::detail::windows_timer_res_access::s_manager;

  c_manager.notify_exit();

  a_thread.join();
}

namespace
dango::detail
{
  DANGO_DEFINE_GLOBAL_EXTERN(s_windows_timer_res_daemon, { })
}

#endif // DANGO_PLATFORM_WINDOWS

/*** platform specific ***/

#include "dango-concurrent-private.hpp"

#ifdef DANGO_PLATFORM_LINUX_OR_APPLE

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

  constexpr auto& c_registry = dango::detail::cond_var_registry_access::s_registry;

  c_registry.regist(this, a_timeout);

  dango::detail::pthread_cond_wait(storage(), dango::detail::mutex_get_storage(mutex_ref()), a_timeout.remaining_ms());

  c_registry.unregist(this, a_timeout);
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

  constexpr auto& c_registry = dango::detail::cond_var_registry_access::s_registry;

  constexpr auto& c_manager = dango::detail::windows_timer_res_access::s_manager;

  c_registry.regist(this, a_timeout);

  c_manager.activate(a_timeout);

  dango::detail::condition_variable_wait(storage(), dango::detail::mutex_get_storage(mutex_ref()), a_timeout.remaining_ms());

  c_manager.deactivate(a_timeout);

  c_registry.unregist(this, a_timeout);
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

#endif // DANGO_PLATFORM_WINDOWS
