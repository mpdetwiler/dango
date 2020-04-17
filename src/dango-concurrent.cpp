#include "dango.hpp"

/*** tick_count ***/

namespace
{
  auto tick_count_suspend_bias_help()noexcept->dango::tick_count_pair;
}

auto
dango::
tick_count_suspend_bias
()noexcept->dango::tick_count_pair
{
  static constinit dango::spin_mutex s_lock{ };
  static auto s_last = tick_count_suspend_bias_help();
  static auto const s_init = s_last;

  dango::tick_count_pair a_current;

  auto& [a_tick, a_bias] = a_current;

  dango_crit(s_lock)
  {
    a_current = tick_count_suspend_bias_help();
    a_tick = dango::max(a_tick, s_last.first());
    a_bias = dango::max(a_bias, s_last.second());
    s_last = a_current;
  }

  a_tick -= dango::min(a_tick, s_init.first());
  a_bias -= dango::min(a_bias, s_init.second());

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
  static constinit dango::static_cond_var_mutex s_cond{ s_mutex };

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
    if(a_cond->m_ref_count++ != dango::usize(0))
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
    if(--a_cond->m_ref_count != dango::usize(0))
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

    auto a_bias = dango::suspend_bias();

    auto a_timeout = dango::timeout::make_rel(dango::tick_count_type(0));

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
        a_timeout.add(dango::tick_count_type(1'000));

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
  auto const a_bias = dango::suspend_bias();

  auto const a_delta = a_bias - a_prev_bias;

  a_prev_bias = a_bias;

  return a_delta < dango::tick_count_type(4);
}

auto
dango::
detail::
cond_var_registry::
pop_internal
()noexcept->bool
{
  dango_crit(m_mutex)
  {
    if(m_internal_list->is_empty())
    {
      return false;
    }

    auto const a_cond = m_internal_list->first();

    dango_assert(a_cond->m_ref_count != dango::usize(0));

    remove(a_cond);

    add(a_cond);

    a_cond->notify_all();
  }

  return true;
}

/*** cond_var_registry_thread ***/

namespace
dango::detail
{
  DANGO_DEFINE_GLOBAL_EXTERN(dango::detail::cond_var_registry_thread const, s_cond_var_registry_thread, { })
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

  throw u8"thread creation failed"; // TODO
}

#ifdef __linux__

/*** tick_count_suspend_bias_help ***/

namespace
{
  auto
  tick_count_suspend_bias_help
  ()noexcept->dango::tick_count_pair
  {
    using tc64 = dango::tick_count_type;

    static constexpr auto const c_div = tc64(10);

    tc64 a_mono;
    tc64 a_boot;

    {
      tc64 a_temp;

      do
      {
        a_boot = dango::detail::tick_count_boottime();

        a_mono = dango::detail::tick_count_monotonic();

        a_temp = dango::detail::tick_count_boottime();
      }
      while(a_boot != a_temp);
    }

    auto const a_bias = dango::max(tc64(0), a_boot - a_mono);

    return dango::tick_count_pair{ (a_mono / c_div) , (a_bias / c_div) };
  }
}

/*** mutex_base ***/

class
dango::
detail::
mutex_base::
mutex_impl
final
{
private:
  using state_type = dango::detail::futex_type;

  enum
  state:state_type
  {
    UNLOCKED,
    LOCKED,
    CONTENDED
  };
public:
  explicit constexpr
  mutex_impl()noexcept:
  m_state{ state::UNLOCKED }
  { }
  ~mutex_impl()noexcept = default;
public:
  void lock()noexcept;
  auto try_lock()noexcept->bool;
  void relock()noexcept;
  void unlock()noexcept;
  auto futex_address()noexcept->state_type*;
private:
  state_type m_state;
public:
  DANGO_IMMOBILE(mutex_impl)
};

auto
dango::
detail::
mutex_base::
get
()noexcept->mutex_impl*
{
  using type = mutex_impl;

  return m_storage.launder_get<type>();
}

void
dango::
detail::
mutex_base::
initialize
()noexcept
{
  using type = mutex_impl;

  static_assert(sizeof(type) <= sizeof(dango::detail::primitive_storage));
  static_assert(alignof(type) <= alignof(dango::detail::primitive_storage));

  dango::placement_new_brace<type>(m_storage.get());
}

void
dango::
detail::
mutex_base::
destroy
()noexcept
{
  if(m_init.has_executed())
  {
    dango::destructor(get());
  }
}

auto
dango::
detail::
mutex_base::
acquire
()noexcept->mutex_base*
{
  dango_assert_noassume(m_init.has_executed());

  get()->lock();

  return this;
}

auto
dango::
detail::
mutex_base::
try_acquire
()noexcept->mutex_base*
{
  dango_assert_noassume(m_init.has_executed());

  if(get()->try_lock())
  {
    return this;
  }

  return dango::null;
}

void
dango::
detail::
mutex_base::
release
()noexcept
{
  dango_assert_noassume(m_init.has_executed());

  get()->unlock();
}

/*** cond_var_base ***/

class
dango::
detail::
cond_var_base::
cond_var_impl
final
{
private:
  using mutex_ptr = dango::detail::mutex_base::mutex_impl*;
  using seq_type = dango::detail::futex_type;
public:
  explicit constexpr
  cond_var_impl()noexcept:
  m_seq{ seq_type(0) }
  { }
  ~cond_var_impl()noexcept = default;
public:
  void notify(dango::spin_mutex_locker const&, mutex_ptr)noexcept;
  void notify_all(dango::spin_mutex_locker const&, mutex_ptr)noexcept;
  void wait(mutex_ptr)noexcept;
  void wait(mutex_ptr, dango::tick_count_type)noexcept;
private:
  seq_type m_seq;
public:
  DANGO_IMMOBILE(cond_var_impl)
};

auto
dango::
detail::
cond_var_base::
get
()noexcept->cond_var_impl*
{
  using type = cond_var_impl;

  return m_storage.launder_get<type>();
}

void
dango::
detail::
cond_var_base::
initialize
()noexcept
{
  using type = cond_var_impl;

  static_assert(sizeof(type) <= sizeof(dango::detail::primitive_storage));
  static_assert(alignof(type) <= alignof(dango::detail::primitive_storage));

  dango::placement_new_brace<type>(m_storage.get());
}

void
dango::
detail::
cond_var_base::
destroy
()noexcept
{
  if(m_init.has_executed())
  {
    dango::destructor(get());
  }
}

void
dango::
detail::
cond_var_base::
increment
(mutex_type* const a_mutex)noexcept
{
  dango_assert(a_mutex != dango::null);

  dango_crit(m_lock)
  {
    if(m_wait_count++ == dango::usize(0))
    {
      m_current_mutex = a_mutex;
    }

    dango_assert(m_current_mutex == a_mutex);
  }
}

void
dango::
detail::
cond_var_base::
decrement
(mutex_type* const a_mutex)noexcept
{
  dango_assert(a_mutex != dango::null);

  dango_crit(m_lock)
  {
    dango_assert(m_current_mutex == a_mutex);

    if(--m_wait_count == dango::usize(0))
    {
      m_current_mutex = dango::null;
    }
  }
}

void
dango::
detail::
cond_var_base::
wait
(mutex_type* const a_mutex)noexcept
{
  dango_assert(a_mutex != dango::null);

  dango_assert_noassume(m_init.has_executed());

  increment(a_mutex);

  get()->wait(a_mutex->get());

  decrement(a_mutex);
}

void
dango::
detail::
cond_var_base::
wait
(mutex_type* const a_mutex, dango::timeout const& a_timeout)noexcept
{
  dango_assert(a_mutex != dango::null);

  dango_assert_noassume(m_init.has_executed());

  auto const a_rem = a_timeout.remaining();

  if(a_rem == dango::tick_count_type(0))
  {
    return;
  }

  auto& a_registry = dango::detail::cond_var_registry_access::s_registry;

  increment(a_mutex);

  a_registry.regist(this, a_timeout);

  get()->wait(a_mutex->get(), a_rem);

  a_registry.unregist(this, a_timeout);

  decrement(a_mutex);
}

void
dango::
detail::
cond_var_base::
notify
()noexcept
{
  dango_assert_noassume(m_init.has_executed());

  dango_crit_full(m_lock, a_crit)
  {
    if(m_wait_count == dango::usize(0))
    {
      return;
    }

    dango_assert(m_current_mutex != dango::null);

    get()->notify(a_crit, m_current_mutex->get());
  }
}

void
dango::
detail::
cond_var_base::
notify_all
()noexcept
{
  dango_assert_noassume(m_init.has_executed());

  dango_crit_full(m_lock, a_crit)
  {
    if(m_wait_count == dango::usize(0))
    {
      return;
    }

    dango_assert(m_current_mutex != dango::null);

    get()->notify_all(a_crit, m_current_mutex->get());
  }
}

void
dango::
detail::
cond_var_base::
unlocked_notify
(bool const a_only_one)noexcept
{
  dango_assert_noassume(m_init.has_executed());

  mutex_type* a_mutex;

  do
  {
    dango_crit(m_lock)
    {
      if(m_wait_count == dango::usize(0))
      {
        return;
      }

      a_mutex = m_current_mutex;
    }

    dango_assert(a_mutex != dango::null);

    dango_crit(*a_mutex)
    {
      dango_crit_full(m_lock, a_crit)
      {
        if(m_wait_count == dango::usize(0))
        {
          return;
        }

        dango_assert(m_current_mutex != dango::null);

        if(m_current_mutex != a_mutex)
        {
          continue;
        }

        if(a_only_one)
        {
          get()->notify(a_crit, a_mutex->get());
        }
        else
        {
          get()->notify_all(a_crit, a_mutex->get());
        }

        return;
      }
    }
  }
  while(true);
}

/*** mutex_impl ***/

namespace
{
  auto spin_relax(dango::uint&)noexcept->bool;
}

void
dango::
detail::
mutex_base::
mutex_impl::
lock
()noexcept
{
  using dango::mem_order::acquire;

  state_type a_last;

  auto a_count = dango::detail::c_spin_count_init;

  do
  {
    a_last = dango::atomic_load<acquire>(&m_state);

    if(a_last == state::UNLOCKED)
    {
      if(dango::atomic_compare_exchange<acquire, acquire>(&m_state, &a_last, state::LOCKED))
      {
        return;
      }
    }
  }
  while(spin_relax(a_count));

  dango_assert(a_last != state::UNLOCKED);

  if(a_last == state::LOCKED)
  {
    a_last = dango::atomic_exchange<acquire>(&m_state, state::CONTENDED);
  }

  while(a_last != state::UNLOCKED)
  {
    dango::detail::futex_wait(&m_state, state::CONTENDED);

    a_last = dango::atomic_exchange<acquire>(&m_state, state::CONTENDED);
  }
}

auto
dango::
detail::
mutex_base::
mutex_impl::
try_lock
()noexcept->bool
{
  using dango::mem_order::acquire;

  auto a_last = dango::atomic_load<acquire>(&m_state);

  if(a_last != state::UNLOCKED)
  {
    return false;
  }

  return dango::atomic_compare_exchange<acquire, acquire>(&m_state, &a_last, state::LOCKED);
}

void
dango::
detail::
mutex_base::
mutex_impl::
relock
()noexcept
{
  using dango::mem_order::acquire;

  state_type a_last;

  auto a_count = dango::detail::c_spin_count_init;

  do
  {
    a_last = dango::atomic_load<acquire>(&m_state);

    if(a_last == state::UNLOCKED)
    {
      a_last = dango::atomic_exchange<acquire>(&m_state, state::CONTENDED);

      if(a_last == state::UNLOCKED)
      {
        return;
      }
    }
  }
  while(spin_relax(a_count));

  dango_assert(a_last != state::UNLOCKED);

  if(a_last == state::LOCKED)
  {
    a_last = dango::atomic_exchange<acquire>(&m_state, state::CONTENDED);
  }

  while(a_last != state::UNLOCKED)
  {
    dango::detail::futex_wait(&m_state, state::CONTENDED);

    a_last = dango::atomic_exchange<acquire>(&m_state, state::CONTENDED);
  }
}

void
dango::
detail::
mutex_base::
mutex_impl::
unlock
()noexcept
{
  using dango::mem_order::release;

  auto const a_last = dango::atomic_exchange<release>(&m_state, state::UNLOCKED);

  dango_assert(a_last != state::UNLOCKED);

  if(a_last == state::LOCKED)
  {
    return;
  }

  dango::detail::futex_wake(&m_state);
}

auto
dango::
detail::
mutex_base::
mutex_impl::
futex_address
()noexcept->state_type*
{
  return &m_state;
}

/*** cond_var_impl ***/

void
dango::
detail::
cond_var_base::
cond_var_impl::
notify
(dango::spin_mutex_locker const&, mutex_ptr const)noexcept
{
  using dango::mem_order::relaxed;

  dango::atomic_add_fetch<relaxed>(&m_seq, seq_type(1));

  dango::detail::futex_wake(&m_seq);
}

void
dango::
detail::
cond_var_base::
cond_var_impl::
notify_all
(dango::spin_mutex_locker const&, mutex_ptr const a_mutex)noexcept
{
  using dango::mem_order::relaxed;

  dango::atomic_add_fetch<relaxed>(&m_seq, seq_type(1));

  dango::detail::futex_wake_requeue(&m_seq, a_mutex->futex_address());
}

void
dango::
detail::
cond_var_base::
cond_var_impl::
wait
(mutex_ptr const a_mutex)noexcept
{
  dango_assert(a_mutex != dango::null);

  auto const a_seq =
    dango::atomic_load<dango::mem_order::relaxed>(&m_seq);

  a_mutex->unlock();

  dango::detail::futex_wait(&m_seq, a_seq);

  a_mutex->relock();
}

void
dango::
detail::
cond_var_base::
cond_var_impl::
wait
(mutex_ptr const a_mutex, dango::tick_count_type const a_interval)noexcept
{
  dango_assert(a_mutex != dango::null);

  auto const a_seq =
    dango::atomic_load<dango::mem_order::relaxed>(&m_seq);

  a_mutex->unlock();

  dango::detail::futex_wait(&m_seq, a_seq, a_interval);

  a_mutex->relock();
}

/*** static ***/

namespace
{
#ifndef DANGO_NO_MULTICORE
  auto
  spin_relax
  (dango::uint& a_count)noexcept->bool
  {
    if(a_count == dango::uint(0))
    {
      return false;
    }

    --a_count;

    __builtin_ia32_pause();

    return true;
  }
#else
  auto
  spin_relax
  (dango::uint&)noexcept->bool
  {
    return false;
  }
#endif
}

#endif // __linux__

#ifdef _WIN32

/*** tick_count_suspend_bias_help ***/

namespace
{
  auto
  tick_count_suspend_bias_help
  ()noexcept->dango::tick_count_pair
  {
    using tc64 = dango::tick_count_type;

    auto const a_freq = dango::detail::perf_freq();

    auto a_current = dango::detail::perf_count_suspend_bias();

    auto& [a_perf, a_bias] = a_current;

    a_perf = (a_perf * tc64(1'000)) / a_freq;
    a_bias /= tc64(10'000);

    return a_current;
  }
}

/*** mutex_base ***/

class
dango::
detail::
mutex_base::
mutex_impl
final
{
private:
  using lock_type = dango::detail::srw_lock_storage;
public:
  explicit mutex_impl()noexcept;
  ~mutex_impl()noexcept = default;
public:
  void lock()noexcept;
  auto try_lock()noexcept->bool;
  void unlock()noexcept;
  auto lock_ptr()noexcept->lock_type*;
private:
  lock_type m_lock;
public:
  DANGO_IMMOBILE(mutex_impl)
};

auto
dango::
detail::
mutex_base::
get
()noexcept->mutex_impl*
{
  using type = mutex_impl;

  return m_storage.launder_get<type>();
}

void
dango::
detail::
mutex_base::
initialize
()noexcept
{
  using type = mutex_impl;

  static_assert(sizeof(type) <= sizeof(dango::detail::primitive_storage));
  static_assert(alignof(type) <= alignof(dango::detail::primitive_storage));

  dango::placement_new_brace<type>(m_storage.get());
}

void
dango::
detail::
mutex_base::
destroy
()noexcept
{
  if(m_init.has_executed())
  {
    dango::destructor(get());
  }
}

auto
dango::
detail::
mutex_base::
acquire
()noexcept->mutex_base*
{
  dango_assert_noassume(m_init.has_executed());

  get()->lock();

  return this;
}

auto
dango::
detail::
mutex_base::
try_acquire
()noexcept->mutex_base*
{
  dango_assert_noassume(m_init.has_executed());

  if(get()->try_lock())
  {
    return this;
  }

  return dango::null;
}

void
dango::
detail::
mutex_base::
release
()noexcept
{
  dango_assert_noassume(m_init.has_executed());

  get()->unlock();
}

/*** cond_var_base ***/

class
dango::
detail::
cond_var_base::
cond_var_impl
final
{
private:
  using cond_type = dango::detail::condition_variable_storage;
  using mutex_ptr = dango::detail::mutex_base::mutex_impl*;
public:
  explicit cond_var_impl()noexcept;
  ~cond_var_impl()noexcept = default;
public:
  void notify(dango::spin_mutex_locker const&, mutex_ptr)noexcept;
  void notify_all(dango::spin_mutex_locker const&, mutex_ptr)noexcept;
  void wait(mutex_ptr)noexcept;
  void wait(mutex_ptr, dango::tick_count_type)noexcept;
private:
  cond_type m_cond;
public:
  DANGO_IMMOBILE(cond_var_impl)
};

auto
dango::
detail::
cond_var_base::
get
()noexcept->cond_var_impl*
{
  using type = cond_var_impl;

  return m_storage.launder_get<type>();
}

void
dango::
detail::
cond_var_base::
initialize
()noexcept
{
  using type = cond_var_impl;

  static_assert(sizeof(type) <= sizeof(dango::detail::primitive_storage));
  static_assert(alignof(type) <= alignof(dango::detail::primitive_storage));

  dango::placement_new_brace<type>(m_storage.get());
}

void
dango::
detail::
cond_var_base::
destroy
()noexcept
{
  if(m_init.has_executed())
  {
    dango::destructor(get());
  }
}

void
dango::
detail::
cond_var_base::
increment
(mutex_type* const a_mutex)noexcept
{
  dango_assert(a_mutex != dango::null);

  dango_crit(m_lock)
  {
    if(m_wait_count++ == dango::usize(0))
    {
      m_current_mutex = a_mutex;
    }

    dango_assert(m_current_mutex == a_mutex);
  }
}

void
dango::
detail::
cond_var_base::
decrement
(mutex_type* const a_mutex)noexcept
{
  dango_assert(a_mutex != dango::null);

  dango_crit(m_lock)
  {
    dango_assert(m_current_mutex == a_mutex);

    if(--m_wait_count == dango::usize(0))
    {
      m_current_mutex = dango::null;
    }
  }
}

void
dango::
detail::
cond_var_base::
wait
(mutex_type* const a_mutex)noexcept
{
  dango_assert(a_mutex != dango::null);

  dango_assert_noassume(m_init.has_executed());

  increment(a_mutex);

  get()->wait(a_mutex->get());

  decrement(a_mutex);
}

void
dango::
detail::
cond_var_base::
wait
(mutex_type* const a_mutex, dango::timeout const& a_timeout)noexcept
{
  dango_assert(a_mutex != dango::null);

  dango_assert_noassume(m_init.has_executed());

  auto const a_rem = a_timeout.remaining();

  if(a_rem == dango::tick_count_type(0))
  {
    return;
  }

  auto& a_registry = dango::detail::cond_var_registry_access::s_registry;

  auto& a_manager = dango::detail::windows_timer_res_access::s_manager;

  increment(a_mutex);

  a_registry.regist(this, a_timeout);

  a_manager.activate(a_timeout);

  get()->wait(a_mutex->get(), a_rem);

  a_manager.deactivate(a_timeout);

  a_registry.unregist(this, a_timeout);

  decrement(a_mutex);
}

void
dango::
detail::
cond_var_base::
notify
()noexcept
{
  dango_assert_noassume(m_init.has_executed());

  dango_crit_full(m_lock, a_crit)
  {
    if(m_wait_count == dango::usize(0))
    {
      return;
    }

    dango_assert(m_current_mutex != dango::null);

    get()->notify(a_crit, m_current_mutex->get());
  }
}

void
dango::
detail::
cond_var_base::
notify_all
()noexcept
{
  dango_assert_noassume(m_init.has_executed());

  dango_crit_full(m_lock, a_crit)
  {
    if(m_wait_count == dango::usize(0))
    {
      return;
    }

    dango_assert(m_current_mutex != dango::null);

    get()->notify_all(a_crit, m_current_mutex->get());
  }
}

void
dango::
detail::
cond_var_base::
unlocked_notify
(bool const a_only_one)noexcept
{
  dango_assert_noassume(m_init.has_executed());

  mutex_type* a_mutex;

  do
  {
    dango_crit(m_lock)
    {
      if(m_wait_count == dango::usize(0))
      {
        return;
      }

      a_mutex = m_current_mutex;
    }

    dango_assert(a_mutex != dango::null);

    dango_crit(*a_mutex)
    {
      dango_crit_full(m_lock, a_crit)
      {
        if(m_wait_count == dango::usize(0))
        {
          return;
        }

        dango_assert(m_current_mutex != dango::null);

        if(m_current_mutex != a_mutex)
        {
          continue;
        }

        if(a_only_one)
        {
          get()->notify(a_crit, a_mutex->get());
        }
        else
        {
          get()->notify_all(a_crit, a_mutex->get());
        }

        return;
      }
    }
  }
  while(true);
}

/*** mutex_impl ***/

dango::
detail::
mutex_base::
mutex_impl::
mutex_impl
()noexcept:
m_lock{ }
{
  dango::detail::srw_lock_init(&m_lock);
}

void
dango::
detail::
mutex_base::
mutex_impl::
lock
()noexcept
{
  dango::detail::srw_lock_acquire(&m_lock);
}

auto
dango::
detail::
mutex_base::
mutex_impl::
try_lock
()noexcept->bool
{
  return dango::detail::srw_lock_try_acquire(&m_lock);
}

void
dango::
detail::
mutex_base::
mutex_impl::
unlock
()noexcept
{
  dango::detail::srw_lock_release(&m_lock);
}

auto
dango::
detail::
mutex_base::
mutex_impl::
lock_ptr
()noexcept->lock_type*
{
  return &m_lock;
}

/*** cond_var_impl ***/

dango::
detail::
cond_var_base::
cond_var_impl::
cond_var_impl
()noexcept:
m_cond{ }
{
  dango::detail::condition_variable_init(&m_cond);
}

void
dango::
detail::
cond_var_base::
cond_var_impl::
notify
(dango::spin_mutex_locker const&, mutex_ptr const)noexcept
{
  dango::detail::condition_variable_wake(&m_cond);
}

void
dango::
detail::
cond_var_base::
cond_var_impl::
notify_all
(dango::spin_mutex_locker const&, mutex_ptr const)noexcept
{
  dango::detail::condition_variable_wake_all(&m_cond);
}

void
dango::
detail::
cond_var_base::
cond_var_impl::
wait
(mutex_ptr const a_mutex)noexcept
{
  dango_assert(a_mutex != dango::null);

  dango::detail::condition_variable_wait(&m_cond, a_mutex->lock_ptr());
}

void
dango::
detail::
cond_var_base::
cond_var_impl::
wait
(mutex_ptr const a_mutex, dango::tick_count_type const a_interval)noexcept
{
  dango_assert(a_mutex != dango::null);

  dango::detail::condition_variable_wait(&m_cond, a_mutex->lock_ptr(), a_interval);
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
      dango::timeout::make_rel(dango::tick_count_type(60'000));

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
  DANGO_DEFINE_GLOBAL_EXTERN(dango::detail::windows_timer_res_daemon const, s_windows_timer_res_daemon, { })
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

#endif // _WIN32

