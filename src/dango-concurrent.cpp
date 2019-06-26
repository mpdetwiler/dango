#include "dango.hpp"

/*** thread_yield ***/

void
dango::
detail::
thread_yield
()noexcept
{
  dango::thread::yield();
}

/*** init_tick_count ***/

auto
dango::
detail::
init_tick_count
()noexcept->detail::tick_count_tuple const&
{
  using u64 = dango::uint64;
  using tuple_type = detail::tick_count_tuple;

  constexpr auto const c_impl =
  []()noexcept->tuple_type
  {
    u64 x0;
    u64 y0;
    u64 z0;

    {
      u64 x1;
      u64 y1;
      u64 z1;

      do
      {
        x0 = detail::tick_count();
        y0 = detail::tick_count_sa();
        z0 = detail::suspend_bias();

        x1 = detail::tick_count();
        y1 = detail::tick_count_sa();
        z1 = detail::suspend_bias();
      }
      while((x0 != x1) || (y0 != y1) || (z0 != z1));
    }

    return tuple_type{ x0, y0, z0 };
  };

  static tuple_type const s_init{ c_impl() };

  return s_init;
}

/*** control_block ***/

auto
dango::
thread::
control_block::
operator new
(dango::usize const a_size)dango_new_noexcept(true)->void*
{
  return dango::operator_new(a_size, alignof(control_block)).dismiss();
}

constexpr
dango::
thread::
control_block::
control_block
(bool const a_daemon)noexcept:
super_type{ },
m_ref_count{ dango::usize(1) },
m_daemon{ a_daemon },
m_mutex{ },
m_cond{ m_mutex },
m_alive{ true },
m_waiter_count{ dango::usize(0) }
{

}

void
dango::
thread::
control_block::
wait
()noexcept
{
  dango_crit_full(m_cond, a_crit)
  {
    while(m_alive)
    {
      ++m_waiter_count;

      a_crit.wait();

      --m_waiter_count;
    }
  }
}

void
dango::
thread::
control_block::
wait
(dango::timeout const& a_timeout)noexcept
{
  dango_crit_full(m_cond, a_crit)
  {
    while(m_alive && !a_timeout.has_expired())
    {
      ++m_waiter_count;

      a_crit.wait(a_timeout);

      --m_waiter_count;
    }
  }
}

void
dango::
thread::
control_block::
notify_all
()noexcept
{
  dango_crit_full(m_cond, a_crit)
  {
    m_alive = false;

    if(m_waiter_count != dango::usize(0))
    {
      m_cond.notify_all();
    }
  }
}

auto
dango::
thread::
control_block::
is_alive
()const noexcept->bool
{
  dango_crit(m_mutex)
  {
    return m_alive;
  }
}

/*** registry ***/

void
dango::
thread::
registry::
regist
(control_block* const a_elem)noexcept
{
  dango_crit(m_mutex)
  {
    m_list.add_last(a_elem);
  }
}

void
dango::
thread::
registry::
unregist
(control_block* const a_elem)noexcept
{
  dango_crit(m_mutex)
  {
    m_list.remove(a_elem);
  }
}

void
dango::
thread::
registry::
join_all
()noexcept
{
  while(join_first());
}

auto
dango::
thread::
registry::
join_first
()noexcept->bool
{
  dango::thread a_thread{ dango::null };

  dango_crit(m_mutex)
  {
    if(m_list.is_empty())
    {
      return false;
    }

    auto const a_control = m_list.first();

    a_thread = thread{ a_control };
  }

  a_thread.join();

  return true;
}

/*** notifier ***/

class
dango::
thread::
notifier
final
{
public:
  notifier(thread const&)noexcept;
  ~notifier()noexcept;
private:
  control_block* const m_control;
public:
  DANGO_DELETE_DEFAULT(notifier)
  DANGO_IMMOBILE(notifier)
};

dango::
thread::
notifier::
notifier
(thread const& a_thread)noexcept:
m_control{ a_thread.m_control }
{
  if(!m_control->is_daemon())
  {
    dango::thread::s_registry.regist(m_control);
  }
}

dango::
thread::
notifier::
~notifier
()noexcept
{
  if(!m_control->is_daemon())
  {
    dango::thread::s_registry.unregist(m_control);
  }

  m_control->notify_all();
}

/*** thread ***/

struct
dango::
thread::
construct_tag
final
{
  DANGO_TAG_TYPE(construct_tag)
};

dango::
thread::
thread
(construct_tag const, bool const a_daemon)noexcept:
m_control{ new_control_block(a_daemon) }
{

}

dango::
thread::
thread
(control_block* const a_control)noexcept:
m_control{ a_control }
{
  dango_assert(m_control != nullptr);

  m_control->increment();
}

auto
dango::
thread::
is_alive
()const noexcept->bool
{
  dango_assert(m_control != nullptr);

  return m_control->is_alive();
}

void
dango::
thread::
join
()const noexcept
{
  dango_assert(m_control != nullptr);

  m_control->wait();
}

void
dango::
thread::
join
(dango::timeout const& a_timeout)const noexcept
{
  dango_assert(m_control != nullptr);

  m_control->wait(a_timeout);
}

auto
dango::
thread::
new_control_block
(bool const a_daemon)noexcept->control_block*
{
  try
  {
    return new control_block{ a_daemon };
  }
  catch(...)
  {
#ifndef DANGO_NO_DEBUG
    dango_unreachable_msg("thread control block allocation failed");
#else
    dango::terminate();
#endif
  }
}

auto
dango::
thread::
thread_self_init
(bool const a_daemon)noexcept->thread const&
{
  thread_local thread const t_thread{ construct_tag{ }, a_daemon };

  thread_local notifier const t_notifier{ t_thread };

  return t_thread;
}

void
dango::
thread::
sleep
(dango::timeout const& a_timeout)noexcept
{
  static dango::static_mutex s_mutex{ };
  static dango::static_cond_var_mutex s_cond{ s_mutex };

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

void
dango::
thread::
sleep_rel
(dango::uint64 const a_interval)noexcept
{
  auto const a_timeout = dango::make_timeout_rel(a_interval);

  thread::sleep(a_timeout);
}

void
dango::
thread::
sleep_rel_hr
(dango::uint64 const a_interval)noexcept
{
  auto const a_timeout = dango::make_timeout_rel_hr(a_interval);

  thread::sleep(a_timeout);
}

void
dango::
thread::
sleep_rel_sa
(dango::uint64 const a_interval)noexcept
{
  auto const a_timeout = dango::make_timeout_rel_sa(a_interval);

  thread::sleep(a_timeout);
}

void
dango::
thread::
sleep_rel_hr_sa
(dango::uint64 const a_interval)noexcept
{
  auto const a_timeout = dango::make_timeout_rel_hr_sa(a_interval);

  thread::sleep(a_timeout);
}

/*** cond_var_registry ***/

void
dango::
detail::
cond_var_registry::
regist
(cond_type* const a_cond, dango::timeout const& a_timeout)noexcept
{
  dango_assert(a_cond != nullptr);

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
      m_cond.notify();
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
  dango_assert(a_cond != nullptr);

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
      m_cond.notify();
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
      m_cond.notify();
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

    auto a_bias = dango::get_suspend_bias();

    auto a_timeout = dango::make_timeout_rel(dango::uint64(0));

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
(dango::uint64& a_prev_bias, dango::timeout& a_timeout)noexcept->bool
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
        a_timeout.add(dango::uint64(1'000));

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
(dango::uint64& a_prev_bias)noexcept->bool
{
  auto const a_bias = dango::get_suspend_bias();

  auto const a_delta = a_bias - a_prev_bias;

  a_prev_bias = a_bias;

  return a_delta < dango::uint64(5);
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

auto
dango::
detail::
cond_var_registry_thread::
start_thread
()noexcept->dango::thread
{
  constexpr auto& c_registry = detail::cond_var_registry_access::s_registry;

  try
  {
    return dango::thread::create_daemon([]()noexcept->void{ c_registry.thread_func(); });
  }
  catch(...)
  {
#ifndef DANGO_NO_DEBUG
    dango_unreachable_msg("cond_var watcher-thread creation failed");
#else
    dango::terminate();
#endif
  }
}

dango::
detail::
cond_var_registry_thread::
cond_var_registry_thread
()noexcept:
m_thread{ start_thread() }
{

}

dango::
detail::
cond_var_registry_thread::
~cond_var_registry_thread
()noexcept
{
  constexpr auto& c_registry = detail::cond_var_registry_access::s_registry;

  c_registry.notify_exit();

  m_thread.join();
}

#include <dango-shared-concurrent.hpp>

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
  if(dango::shared::create_thread(a_thread_func, a_thread_data))
  {
    return;
  }

  throw "thread creation failed"; // TODO
}

void
dango::
thread::
yield
()noexcept
{
  dango::shared::yield_thread();
}

#ifdef __linux__

/*** tick_count ***/

namespace
{
  auto suspend_bias_help()noexcept->dango::uint64;
}

auto
dango::
detail::
tick_count
()noexcept->dango::uint64
{
  return dango::shared::tick_count_monotonic();
}

auto
dango::
detail::
tick_count_sa
()noexcept->dango::uint64
{
  return dango::shared::tick_count_boottime();
}

auto
dango::
detail::
suspend_bias
()noexcept->dango::uint64
{
  static dango::spin_mutex s_lock{ };
  static auto s_prev = dango::uint64(0);

  dango::uint64 a_result;

  dango_crit(s_lock)
  {
    a_result = suspend_bias_help();

    a_result = dango::max(a_result, s_prev);

    s_prev = a_result;
  }

  return a_result;
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
  using state_type = dango::shared::futex_type;

  enum
  state:state_type
  {
    UNLOCKED,
    LOCKED,
    CONTENDED
  };
public:
  constexpr mutex_impl()noexcept;
  ~mutex_impl()noexcept = default;
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

constexpr
dango::
detail::
mutex_base::
mutex_impl::
mutex_impl()noexcept:
m_state{ state::UNLOCKED }
{

}

auto
dango::
detail::
mutex_base::
get
()noexcept->mutex_impl*
{
  return static_cast<mutex_impl*>(m_storage.get());
}

void
dango::
detail::
mutex_base::
initialize
()noexcept
{
  using type = mutex_impl;

  static_assert(sizeof(type) <= sizeof(detail::primitive_storage));
  static_assert(alignof(type) <= alignof(detail::primitive_storage));

  ::new (dango::placement, m_storage.get()) type{ };
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
#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

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
#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  if(get()->try_lock())
  {
    return this;
  }

  return nullptr;
}

void
dango::
detail::
mutex_base::
release
()noexcept
{
#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

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
  using seq_type = dango::shared::futex_type;
public:
  constexpr cond_var_impl()noexcept:
  m_seq{ seq_type(0) },
  m_lock{ },
  m_mutex{ nullptr },
  m_wait_count{ dango::usize(0) }
  { }
  ~cond_var_impl()noexcept = default;
  void notify()noexcept;
  void notify_all()noexcept;
  void wait(mutex_ptr)noexcept;
  void wait(mutex_ptr, dango::uint64)noexcept;
private:
  void increment(mutex_ptr)noexcept;
  void decrement(mutex_ptr)noexcept;
private:
  seq_type m_seq;
  dango::spin_mutex m_lock;
  mutex_ptr m_mutex;
  dango::usize m_wait_count;
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
  return static_cast<cond_var_impl*>(m_storage.get());
}

void
dango::
detail::
cond_var_base::
initialize
()noexcept
{
  using type = cond_var_impl;

  static_assert(sizeof(type) <= sizeof(detail::primitive_storage));
  static_assert(alignof(type) <= alignof(detail::primitive_storage));

  ::new (dango::placement, m_storage.get()) type{ };
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
wait
(mutex_type* const a_lock)noexcept
{
  dango_assert(a_lock != nullptr);

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  get()->wait(a_lock->get());
}

void
dango::
detail::
cond_var_base::
wait
(mutex_type* const a_lock, dango::timeout const& a_timeout)noexcept
{
  dango_assert(a_lock != nullptr);

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  auto const a_rem = a_timeout.remaining();

  if(a_rem == dango::uint64(0))
  {
    return;
  }

  constexpr auto& c_registry = detail::cond_var_registry_access::s_registry;

  c_registry.regist(this, a_timeout);

  get()->wait(a_lock->get(), a_rem);

  c_registry.unregist(this, a_timeout);
}

void
dango::
detail::
cond_var_base::
notify
()noexcept
{
  init();

  get()->notify();
}

void
dango::
detail::
cond_var_base::
notify_all
()noexcept
{
  init();

  get()->notify_all();
}

/*** mutex_impl ***/

namespace
{
  auto spin_relax(dango::uint32&)noexcept->bool;

  constexpr auto const c_spin_count_init = dango::uint32(128);
}

void
dango::
detail::
mutex_base::
mutex_impl::
lock
()noexcept
{
  constexpr auto const acquire = dango::mem_order::acquire;

  state_type a_last;

  auto a_count = c_spin_count_init;

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
    dango::shared::futex_wait(&m_state, state::CONTENDED);

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
  constexpr auto const acquire = dango::mem_order::acquire;

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
  constexpr auto const acquire = dango::mem_order::acquire;

  state_type a_last;

  auto a_count = c_spin_count_init;

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
    dango::shared::futex_wait(&m_state, state::CONTENDED);

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
  constexpr auto const release = dango::mem_order::release;

  auto const a_last = dango::atomic_exchange<release>(&m_state, state::UNLOCKED);

  dango_assert(a_last != state::UNLOCKED);

  if(a_last == state::LOCKED)
  {
    return;
  }

  dango::shared::futex_wake(&m_state);
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
()noexcept
{
  constexpr auto const relaxed = dango::mem_order::relaxed;

  dango_crit(m_lock)
  {
    if(m_wait_count == dango::usize(0))
    {
      return;
    }
  }

  dango::atomic_add_fetch<relaxed>(&m_seq, seq_type(1));

  dango::shared::futex_wake(&m_seq);
}

void
dango::
detail::
cond_var_base::
cond_var_impl::
notify_all
()noexcept
{
  constexpr auto const relaxed = dango::mem_order::relaxed;

  mutex_ptr a_mutex;

  dango_crit(m_lock)
  {
    if(m_wait_count == dango::usize(0))
    {
      return;
    }

    a_mutex = m_mutex;
  }

  dango_assert(a_mutex != nullptr);

  dango::atomic_add_fetch<relaxed>(&m_seq, seq_type(1));

  dango::shared::futex_wake_requeue(&m_seq, a_mutex->futex_address());
}

void
dango::
detail::
cond_var_base::
cond_var_impl::
wait
(mutex_ptr const a_mutex)noexcept
{
  constexpr auto const relaxed = dango::mem_order::relaxed;

  dango_assert(a_mutex != nullptr);

  auto const a_seq = dango::atomic_load<relaxed>(&m_seq);

  increment(a_mutex);

  a_mutex->unlock();

  dango::shared::futex_wait(&m_seq, a_seq);

  a_mutex->relock();

  decrement(a_mutex);
}

void
dango::
detail::
cond_var_base::
cond_var_impl::
wait
(mutex_ptr const a_mutex, dango::uint64 const a_interval)noexcept
{
  constexpr auto const relaxed = dango::mem_order::relaxed;

  dango_assert(a_mutex != nullptr);

  auto const a_seq = dango::atomic_load<relaxed>(&m_seq);

  increment(a_mutex);

  a_mutex->unlock();

  dango::shared::futex_wait(&m_seq, a_seq, a_interval);

  a_mutex->relock();

  decrement(a_mutex);
}

void
dango::
detail::
cond_var_base::
cond_var_impl::
increment
(mutex_ptr const a_mutex)noexcept
{
  dango_crit(m_lock)
  {
    if(m_wait_count++ == dango::usize(0))
    {
      dango_assert(m_mutex == nullptr);

      m_mutex = a_mutex;
    }

    dango_assert(m_mutex == a_mutex);
  }
}

void
dango::
detail::
cond_var_base::
cond_var_impl::
decrement
(mutex_ptr const a_mutex)noexcept
{
  dango_crit(m_lock)
  {
    dango_assert(m_mutex == a_mutex);

    if(--m_wait_count == dango::usize(0))
    {
      m_mutex = nullptr;
    }
  }
}

/*** static ***/

namespace
{
  auto
  suspend_bias_help
  ()noexcept->dango::uint64
  {
    using u64 = dango::uint64;

    u64 x0;
    u64 y0;

    {
      u64 x1;
      u64 y1;

      do
      {
        x0 = dango::detail::tick_count();
        y0 = dango::detail::tick_count_sa();

        x1 = dango::detail::tick_count();
        y1 = dango::detail::tick_count_sa();
      }
      while((x0 != x1) || (y0 != y1));
    }

    dango_assert(y0 >= x0);

    return y0 - x0;
  }

#ifndef DANGO_NO_MULTICORE
  auto
  spin_relax
  (dango::uint32& a_count)noexcept->bool
  {
    if(a_count == dango::uint32(0))
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
  (dango::uint32&)noexcept->bool
  {
    return false;
  }
#endif
}

#endif /* __linux__ */

#ifdef _WIN32

#include <windows.h>

/*** tick_count ***/

namespace
{
  using time_spec = dango::pair<dango::uint64, dango::uint64>;

  auto time_spec_from_perf(dango::uint64, dango::uint64)noexcept->time_spec;
  void time_spec_add(time_spec&, time_spec const&)noexcept;
  auto tick_count_help(dango::uint64*)noexcept->dango::uint64;
}

auto
dango::
detail::
tick_count
()noexcept->dango::uint64
{
  dango::uint64 a_bias;

  auto const a_count = tick_count_help(&a_bias);

  return a_count - a_bias;
}


auto
dango::
detail::
tick_count_sa
()noexcept->dango::uint64
{
  return tick_count_help(nullptr);
}

auto
dango::
detail::
suspend_bias
()noexcept->dango::uint64
{
  dango::uint64 a_bias;

  tick_count_help(&a_bias);

  return a_bias;
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
  using lock_type = SRWLOCK;
public:
  mutex_impl()noexcept;
  ~mutex_impl()noexcept = default;
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
  return static_cast<mutex_impl*>(m_storage.get());
}

void
dango::
detail::
mutex_base::
initialize
()noexcept
{
  using type = mutex_impl;

  static_assert(sizeof(type) <= sizeof(detail::primitive_storage));
  static_assert(alignof(type) <= alignof(detail::primitive_storage));

  ::new (dango::placement, m_storage.get()) type{ };
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
#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

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
#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  if(get()->try_lock())
  {
    return this;
  }

  return nullptr;
}

void
dango::
detail::
mutex_base::
release
()noexcept
{
#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

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
  using cond_type = CONDITION_VARIABLE;
  using mutex_ptr = dango::detail::mutex_base::mutex_impl*;
public:
  cond_var_impl()noexcept;
  ~cond_var_impl()noexcept = default;
  void notify()noexcept;
  void notify_all()noexcept;
  void wait(mutex_ptr)noexcept;
  void wait(mutex_ptr, dango::uint64)noexcept;
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
  return static_cast<cond_var_impl*>(m_storage.get());
}

void
dango::
detail::
cond_var_base::
initialize
()noexcept
{
  using type = cond_var_impl;

  static_assert(sizeof(type) <= sizeof(detail::primitive_storage));
  static_assert(alignof(type) <= alignof(detail::primitive_storage));

  ::new (dango::placement, m_storage.get()) type{ };
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
wait
(mutex_type* const a_lock)noexcept
{
  dango_assert(a_lock != nullptr);

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  get()->wait(a_lock->get());
}

void
dango::
detail::
cond_var_base::
wait
(mutex_type* const a_lock, dango::timeout const& a_timeout)noexcept
{
  dango_assert(a_lock != nullptr);

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  auto const a_rem = a_timeout.remaining();

  if(a_rem == dango::uint64(0))
  {
    return;
  }

  constexpr auto& c_manager = detail::windows_timer_res_access::s_manager;

  constexpr auto& c_registry = detail::cond_var_registry_access::s_registry;

  c_manager.activate(a_timeout);

  c_registry.regist(this, a_timeout);

  get()->wait(a_lock->get(), a_rem);

  c_registry.unregist(this, a_timeout);

  c_manager.deactivate(a_timeout);
}

void
dango::
detail::
cond_var_base::
notify
()noexcept
{
  init();

  get()->notify();
}

void
dango::
detail::
cond_var_base::
notify_all
()noexcept
{
  init();

  get()->notify_all();
}

/*** windows_timer_res_access ***/

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

dango::detail::windows_timer_res_manager
dango::
detail::
windows_timer_res_access::
s_manager{ };

/*** windows_timer_res_manager ***/

auto
dango::
detail::
windows_timer_res_manager::
min_period
()noexcept->dango::uint32
{
  constexpr auto c_impl =
  []()noexcept->dango::uint32
  {
      TIMECAPS a_caps;

      auto const a_error = timeGetDevCaps(&a_caps, sizeof(a_caps));

      if(a_error == MMSYSERR_NOERROR)
      {
        return dango::uint32(a_caps.wPeriodMin);
      }

#ifndef DANGO_NO_DEBUG
      dango_unreachable_msg("timeGetDevCaps failed");
#else
      dango::terminate();
#endif
  };

  static auto const s_period = c_impl();

  return s_period;
}

void
dango::
detail::
windows_timer_res_manager::
begin_period
()noexcept
{
  auto const a_period = min_period();

  auto const a_error = timeBeginPeriod(UINT(a_period));

  if(a_error == TIMERR_NOERROR)
  {
    return;
  }

#ifndef DANGO_NO_DEBUG
  dango_unreachable_msg("timeBeginPeriod failed");
#else
  dango::terminate();
#endif
}

void
dango::
detail::
windows_timer_res_manager::
end_period
()noexcept
{
  auto const a_period = min_period();

  auto const a_error = timeEndPeriod(UINT(a_period));

  if(a_error == TIMERR_NOERROR)
  {
    return;
  }

#ifndef DANGO_NO_DEBUG
  dango_unreachable_msg("timeEndPeriod failed");
#else
  dango::terminate();
#endif
}

void
dango::
detail::
windows_timer_res_manager::
activate
(dango::timeout const& a_timeout)noexcept
{
  if(!a_timeout.requires_high_resolution())
  {
    return;
  }

  dango_crit(m_cond)
  {
    if(m_count++ != dango::usize(0))
    {
      dango_assert(m_timer_state == timer_state::ACTIVATED);

      return;
    }

    dango_assert(m_timer_state != timer_state::ACTIVATED);

    if(m_timer_state == timer_state::DEACTIVATED)
    {
      begin_period();

      m_timer_state = timer_state::ACTIVATED;

      return;
    }

    dango_assert(m_timer_state == timer_state::DEACTIVATING);

    m_timer_state = timer_state::ACTIVATED;

    if(m_waiting)
    {
      m_cond.notify();
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
  if(!a_timeout.requires_high_resolution())
  {
    return;
  }

  dango_crit(m_cond)
  {
    dango_assert(m_timer_state == timer_state::ACTIVATED);

    if(--m_count != dango::usize(0))
    {
      return;
    }

    if(!m_alive)
    {
      end_period();

      m_timer_state = timer_state::DEACTIVATED;

      return;
    }

    m_timer_state = timer_state::DEACTIVATING;

    if(m_waiting)
    {
      m_cond.notify();
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
  dango_crit(m_cond)
  {
    dango_assert(m_alive);

    m_alive = false;

    if(m_timer_state == timer_state::DEACTIVATING)
    {
      end_period();

      m_timer_state = timer_state::DEACTIVATED;
    }

    if(m_waiting)
    {
      m_cond.notify();
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
      dango::make_timeout_rel(dango::uint64(60'000));

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

    end_period();

    m_timer_state = timer_state::DEACTIVATED;
  }

  return false;
}

/*** windows_timer_res_daemon ***/

auto
dango::
detail::
windows_timer_res_daemon::
start_thread
()noexcept->dango::thread
{
  constexpr auto& c_manager = detail::windows_timer_res_access::s_manager;

  try
  {
    return dango::thread::create_daemon([]()noexcept->void{ c_manager.thread_func(); });
  }
  catch(...)
  {
#ifndef DANGO_NO_DEBUG
    dango_unreachable_msg("windows timer-res manager thread creation failed");
#else
    dango::terminate();
#endif
  }
}

dango::
detail::
windows_timer_res_daemon::
windows_timer_res_daemon
()noexcept:
m_thread{ start_thread() }
{

}

dango::
detail::
windows_timer_res_daemon::
~windows_timer_res_daemon
()noexcept
{
  constexpr auto& c_manager = detail::windows_timer_res_access::s_manager;

  c_manager.notify_exit();

  m_thread.join();
}

/*** mutex_impl ***/

dango::
detail::
mutex_base::
mutex_impl::
mutex_impl
()noexcept
{
  InitializeSRWLock(&m_lock);
}

void
dango::
detail::
mutex_base::
mutex_impl::
lock
()noexcept
{
  AcquireSRWLockExclusive(&m_lock);
}

auto
dango::
detail::
mutex_base::
mutex_impl::
try_lock
()noexcept->bool
{
  auto const a_result = TryAcquireSRWLockExclusive(&m_lock);

  return bool(a_result);
}

void
dango::
detail::
mutex_base::
mutex_impl::
unlock
()noexcept
{
  ReleaseSRWLockExclusive(&m_lock);
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
()noexcept
{
  InitializeConditionVariable(&m_cond);
}

void
dango::
detail::
cond_var_base::
cond_var_impl::
notify
()noexcept
{
  WakeConditionVariable(&m_cond);
}

void
dango::
detail::
cond_var_base::
cond_var_impl::
notify_all
()noexcept
{
  WakeAllConditionVariable(&m_cond);
}

void
dango::
detail::
cond_var_base::
cond_var_impl::
wait
(mutex_ptr const a_mutex)noexcept
{
  dango_assert(a_mutex != nullptr);

  SleepConditionVariableSRW
  (
    &m_cond,
    a_mutex->lock_ptr(),
    DWORD(INFINITE),
    ULONG(0)
  );
}

void
dango::
detail::
cond_var_base::
cond_var_impl::
wait
(mutex_ptr const a_mutex, dango::uint64 const a_interval)noexcept
{
  dango_assert(a_mutex != nullptr);

  using u64 = dango::uint64;

  constexpr auto const c_max_interval = u64(DWORD(INFINITE) - DWORD(1));

  auto const a_spec = dango::min(a_interval, c_max_interval);

  dango_assert(DWORD(a_spec) != DWORD(INFINITE));

  SleepConditionVariableSRW
  (
    &m_cond,
    a_mutex->lock_ptr(),
    DWORD(a_spec),
    ULONG(0)
  );
}

/*** static ***/

namespace
{
  auto
  time_spec_from_perf
  (dango::uint64 const a_count, dango::uint64 const a_freq)noexcept->time_spec
  {
    using u64 = dango::uint64;

    constexpr auto const c_billion = u64(1'000'000'000);

    auto const a_div = a_count / a_freq;
    auto const a_mod = a_count % a_freq;

    return time_spec{ a_div, (a_mod * c_billion) / a_freq };
  }

  void
  time_spec_add
  (time_spec& a_spec, time_spec const& a_add)noexcept
  {
    using u64 = dango::uint64;

    constexpr auto const c_billion = u64(1'000'000'000);

    auto& [a_s1, a_n1] = a_spec;

    auto const& [a_s2, a_n2] = a_add;

    dango_assert(a_n1 < c_billion);
    dango_assert(a_n2 < c_billion);

    a_s1 += a_s2;
    a_n1 += a_n2;

    auto const a_overflow = u64(a_n1 >= c_billion);

    a_s1 += a_overflow;
    a_n1 -= a_overflow * c_billion;

    dango_assert(a_n1 < c_billion);
  }

  auto
  tick_count_help
  (dango::uint64* const a_suspend_bias)noexcept->dango::uint64
  {
    using u64 = dango::uint64;

    static dango::spin_mutex s_lock{ };
    static time_spec s_current{ u64(0), u64(0) };
    static auto s_init_bias = u64(0);
    static auto s_prev_count = dango::shared::perf_count_suspend_bias(s_init_bias);

    auto const a_freq = dango::shared::perf_freq();

    u64 a_sec;
    u64 a_nsec;
    u64 a_bias;

    dango_crit(s_lock)
    {
      auto const a_count = dango::shared::perf_count_suspend_bias(a_bias);
      auto const a_count_delta = a_count - s_prev_count;

      s_prev_count = a_count;

      auto const a_delta = time_spec_from_perf(a_count_delta, a_freq);

      time_spec_add(s_current, a_delta);

      a_sec = s_current.first();
      a_nsec = s_current.second();
    }

    dango_assert(a_bias >= s_init_bias);

    if(a_suspend_bias)
    {
      (*a_suspend_bias) = (a_bias - s_init_bias) / u64(10'000);
    }

    return (a_sec * u64(1'000)) + (a_nsec / u64(1'000'000));
  }
}

#endif /* _WIN32 */

