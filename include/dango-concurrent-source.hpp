#ifdef DANGO_SOURCE_FILE

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

#ifdef __linux__

/*** tick_count ***/

namespace
dango::concurrent_cpp
{
  static auto tick_count_help(clockid_t)noexcept->dango::uint64;
  static auto suspend_bias_help()noexcept->dango::uint64;
}

auto
dango::
detail::
tick_count
()noexcept->dango::uint64
{
  return concurrent_cpp::tick_count_help(CLOCK_MONOTONIC);
}

auto
dango::
detail::
tick_count_sa
()noexcept->dango::uint64
{
  return concurrent_cpp::tick_count_help(CLOCK_BOOTTIME);
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
    a_result = concurrent_cpp::suspend_bias_help();

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
  using state_type = dango::sint32;
  enum
  state:
  state_type
  {
    UNLOCKED, LOCKED, CONTENDED
  };
public:
  constexpr mutex_impl()noexcept:m_state{ state::UNLOCKED }{ }
  ~mutex_impl()noexcept = default;
  void lock()noexcept;
  auto try_lock()noexcept->bool;
  void relock()noexcept;
  void unlock()noexcept;
  auto futex_address()noexcept->state_type*;
private:
  dango::atomic<state_type> m_state;
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
  using mutex_ptr = dango::detail::mutex_base::mutex_impl*;
  using seq_type = dango::sint32;
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
  dango::atomic<seq_type> m_seq;
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

/*** thread ***/

namespace
dango::concurrent_cpp
{
  template
  <typename tp_func>
  static auto thread_start_address(void*)noexcept->void*;
}

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
  constexpr auto const acquire = dango::mem_order::acquire;
  constexpr auto const release = dango::mem_order::release;

  dango::atomic<bool> a_starting{ true };

  auto a_func =
  [
    a_thread_func,
    a_thread_data,
    &a_starting
  ]
  ()noexcept(false)->void
  {
    pthread_detach(pthread_self());

    a_starting.store<release>(false);

    a_thread_func(a_thread_data);
  };

  {
    pthread_t a_thread_ID;

    auto const a_result =
    pthread_create
    (
      &a_thread_ID,
      nullptr,
      concurrent_cpp::thread_start_address<decltype(a_func)>,
      &a_func
    );

    if(a_result == EAGAIN)
    {
      throw "thread creation failed"; // TODO
    }

    dango_assert(a_result == 0);
  }

  auto a_count = detail::c_spin_count_init;

  while(a_starting.load<acquire>())
  {
    detail::spin_yield(a_count);
  }
}

void
dango::
thread::
yield
()noexcept
{
  sched_yield();
}

/*** mutex_impl ***/

namespace
dango::concurrent_cpp
{
  static auto
  sys_futex
  (
    dango::s_int*,
    dango::s_int,
    dango::s_int,
    timespec const*,
    dango::s_int*,
    dango::s_int
  )
  noexcept->dango::s_int;

  static auto spin_relax(dango::uint32&)noexcept->bool;

  static constexpr auto const c_spin_count_init = dango::uint32(128);
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

  auto a_count = concurrent_cpp::c_spin_count_init;

  do
  {
    a_last = m_state.load<acquire>();

    if(a_last == state::UNLOCKED)
    {
      if(m_state.compare_exchange<acquire, acquire>(a_last, state::LOCKED))
      {
        return;
      }
    }
  }
  while(concurrent_cpp::spin_relax(a_count));

  dango_assert(a_last != state::UNLOCKED);

  if(a_last == state::LOCKED)
  {
    a_last = m_state.exchange<acquire>(state::CONTENDED);
  }

  while(a_last != state::UNLOCKED)
  {
    concurrent_cpp::sys_futex
    (
      m_state.address(),
      FUTEX_WAIT_PRIVATE,
      state::CONTENDED,
      nullptr,
      nullptr,
      dango::s_int(0)
    );

    a_last = m_state.exchange<acquire>(state::CONTENDED);
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

  state_type a_last = m_state.load<acquire>();

  if(a_last != state::UNLOCKED)
  {
    return false;
  }

  return m_state.compare_exchange<acquire, acquire>(a_last, state::LOCKED);
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

  auto a_count = concurrent_cpp::c_spin_count_init;

  do
  {
    a_last = m_state.load<acquire>();

    if(a_last == state::UNLOCKED)
    {
      a_last = m_state.exchange<acquire>(state::CONTENDED);

      if(a_last == state::UNLOCKED)
      {
        return;
      }
    }
  }
  while(concurrent_cpp::spin_relax(a_count));

  dango_assert(a_last != state::UNLOCKED);

  if(a_last == state::LOCKED)
  {
    a_last = m_state.exchange<acquire>(state::CONTENDED);
  }

  while(a_last != state::UNLOCKED)
  {
    concurrent_cpp::sys_futex
    (
      m_state.address(),
      FUTEX_WAIT_PRIVATE,
      state::CONTENDED,
      nullptr,
      nullptr,
      dango::s_int(0)
    );

    a_last = m_state.exchange<acquire>(state::CONTENDED);
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

  auto const a_last = m_state.exchange<release>(state::UNLOCKED);

  dango_assert(a_last != state::UNLOCKED);

  if(a_last == state::LOCKED)
  {
    return;
  }

  concurrent_cpp::sys_futex
  (
    m_state.address(),
    FUTEX_WAKE_PRIVATE,
    dango::s_int(1),
    nullptr,
    nullptr,
    dango::s_int(0)
  );
}

auto
dango::
detail::
mutex_base::
mutex_impl::
futex_address
()noexcept->state_type*
{
  return m_state.address();
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

  m_seq.add_fetch<relaxed>(seq_type(1));

  concurrent_cpp::sys_futex
  (
    m_seq.address(),
    FUTEX_WAKE_PRIVATE,
    dango::s_int(1),
    nullptr,
    nullptr,
    dango::s_int(0)
  );
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

  m_seq.add_fetch<relaxed>(seq_type(1));

  constexpr auto const c_all = dango::uintptr(dango::integer::MAX_VAL<dango::s_int>);

  concurrent_cpp::sys_futex
  (
    m_seq.address(),
    FUTEX_REQUEUE_PRIVATE,
    dango::s_int(1),
    reinterpret_cast<timespec const*>(c_all),
    a_mutex->futex_address(),
    dango::s_int(0)
  );
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

  auto const a_seq = m_seq.load<relaxed>();

  increment(a_mutex);

  a_mutex->unlock();

  concurrent_cpp::sys_futex
  (
    m_seq.address(),
    FUTEX_WAIT_PRIVATE,
    a_seq,
    nullptr,
    nullptr,
    dango::s_int(0)
  );

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

  auto const a_seq = m_seq.load<relaxed>();

  increment(a_mutex);

  a_mutex->unlock();

  using u64 = dango::uint64;

  timespec a_spec;

  {
    auto const a_sec = a_interval / u64(1'000);
    auto const a_mod = a_interval % u64(1'000);
    auto const a_nsec = a_mod * u64(1'000'000);

    a_spec.tv_sec = a_sec;
    a_spec.tv_nsec = a_nsec;
  }

  concurrent_cpp::sys_futex
  (
    m_seq.address(),
    FUTEX_WAIT_PRIVATE,
    a_seq,
    &a_spec,
    nullptr,
    dango::s_int(0)
  );

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

static auto
dango::
concurrent_cpp::
tick_count_help
(clockid_t const a_clock)noexcept->dango::uint64
{
  using u64 = dango::uint64;

  constexpr auto const c_mul = u64(1'000);
  constexpr auto const c_div = u64(1'000'000);

  timespec a_spec;

  auto const a_result =
    clock_gettime(a_clock, &a_spec);

  dango_assert(a_result == 0);

  auto const a_sec = u64(a_spec.tv_sec);
  auto const a_nsec = u64(a_spec.tv_nsec);

  return (a_sec * c_mul) + (a_nsec / c_div);
}

static auto
dango::
concurrent_cpp::
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

template
<typename tp_func>
static auto
dango::
concurrent_cpp::
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
#ifndef DANGO_NO_DEBUG
    dango_unreachable_msg("uncaught exception in thread");
#else
    dango::terminate();
#endif
  }

  return nullptr;
}

static auto
dango::
concurrent_cpp::
sys_futex
(
  dango::s_int* const a_addr1,
  dango::s_int const a_futex_op,
  dango::s_int const a_val1,
  timespec const* const a_timeout,
  dango::s_int* const a_addr2,
  dango::s_int const a_val3
)
noexcept->dango::s_int
{
  auto const a_result =
  syscall
  (
    SYS_futex,
    a_addr1,
    a_futex_op,
    a_val1,
    a_timeout,
    a_addr2,
    a_val3
  );

  return a_result;
}

#ifndef DANGO_NO_MULTICORE
static auto
dango::
concurrent_cpp::
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
static auto
dango::
concurrent_cpp::
spin_relax
(dango::uint32&)noexcept->bool
{
  return false;
}
#endif

#endif /* __linux__ */

#ifdef _WIN32

/*** tick_count ***/

namespace
dango::concurrent_cpp
{
  using time_spec = dango::pair<dango::uint64, dango::uint64>;

  static auto perf_freq()noexcept->dango::uint64;
  static auto perf_count()noexcept->dango::uint64;
  static auto perf_count_suspend_bias(dango::uint64&)noexcept->dango::uint64;
  static auto time_spec_from_perf(dango::uint64, dango::uint64)noexcept->concurrent_cpp::time_spec;
  static void time_spec_add(concurrent_cpp::time_spec&, concurrent_cpp::time_spec const&)noexcept;
  static auto tick_count_help(dango::uint64*)noexcept->dango::uint64;
}

auto
dango::
detail::
tick_count
()noexcept->dango::uint64
{
  dango::uint64 a_bias;

  auto const a_count = concurrent_cpp::tick_count_help(&a_bias);

  return a_count - a_bias;
}


auto
dango::
detail::
tick_count_sa
()noexcept->dango::uint64
{
  return concurrent_cpp::tick_count_help(nullptr);
}

auto
dango::
detail::
suspend_bias
()noexcept->dango::uint64
{
  dango::uint64 a_bias;

  concurrent_cpp::tick_count_help(&a_bias);

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

/*** thread ***/

namespace
dango::concurrent_cpp
{
  template
  <typename tp_func>
  static auto WINAPI thread_start_address(LPVOID)noexcept->DWORD;
}

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
  constexpr auto const acquire = dango::mem_order::acquire;
  constexpr auto const release = dango::mem_order::release;

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
    auto const a_handle =
    CreateThread
    (
      nullptr,
      SIZE_T(0),
      concurrent_cpp::thread_start_address<decltype(a_func)>,
      &a_func,
      DWORD(0),
      nullptr
    );

    if(a_handle == NULL)
    {
      throw "thread creation failed"; // TODO
    }

    auto const a_result = CloseHandle(a_handle);

    dango_assert(a_result != 0);
  }

  auto a_count = detail::c_spin_count_init;

  while(a_starting.load<acquire>())
  {
    detail::spin_yield(a_count);
  }
}

void
dango::
thread::
yield
()noexcept
{
  Sleep(DWORD(0));
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

static auto
dango::
concurrent_cpp::
perf_freq
()noexcept->dango::uint64
{
  static constexpr auto const c_impl =
  []()noexcept->dango::uint64
  {
    LARGE_INTEGER a_result;

    QueryPerformanceFrequency(&a_result);

    return dango::uint64(a_result.QuadPart);
  };

  static auto const s_result = c_impl();

  return s_result;
}

static auto
dango::
concurrent_cpp::
perf_count
()noexcept->dango::uint64
{
  LARGE_INTEGER a_result;

  QueryPerformanceCounter(&a_result);

  return dango::uint64(a_result.QuadPart);
}

static auto
dango::
concurrent_cpp::
perf_count_suspend_bias
(dango::uint64& a_suspend_bias)noexcept->dango::uint64
{
  using u64 = dango::uint64;

  static auto& s_bias_ref =
    *reinterpret_cast<ULONGLONG const volatile*>(dango::uintptr(0x7FFE0000 + 0x3B0));

  u64 a_bias;
  u64 a_count;

  do
  {
    a_bias = u64(s_bias_ref);
    a_count = concurrent_cpp::perf_count();
  }
  while(a_bias != u64(s_bias_ref));

  a_suspend_bias = a_bias;

  return a_count;
}

static auto
dango::
concurrent_cpp::
time_spec_from_perf
(dango::uint64 const a_count, dango::uint64 const a_freq)noexcept->concurrent_cpp::time_spec
{
  using u64 = dango::uint64;

  constexpr auto const c_billion = u64(1'000'000'000);

  auto const a_div = a_count / a_freq;
  auto const a_mod = a_count % a_freq;

  return concurrent_cpp::time_spec{ a_div, (a_mod * c_billion) / a_freq };
}

static void
dango::
concurrent_cpp::
time_spec_add
(concurrent_cpp::time_spec& a_spec, concurrent_cpp::time_spec const& a_add)noexcept
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

static auto
dango::
concurrent_cpp::
tick_count_help
(dango::uint64* const a_suspend_bias)noexcept->dango::uint64
{
  using u64 = dango::uint64;

  static dango::spin_mutex s_lock{ };
  static concurrent_cpp::time_spec s_current{ u64(0), u64(0) };
  static auto s_init_bias = u64(0);
  static auto s_prev_count = concurrent_cpp::perf_count_suspend_bias(s_init_bias);

  auto const a_freq = concurrent_cpp::perf_freq();

  u64 a_sec;
  u64 a_nsec;
  u64 a_bias;

  dango_crit(s_lock)
  {
    auto const a_count = concurrent_cpp::perf_count_suspend_bias(a_bias);
    auto const a_count_delta = a_count - s_prev_count;

    s_prev_count = a_count;

    auto const a_delta = concurrent_cpp::time_spec_from_perf(a_count_delta, a_freq);

    concurrent_cpp::time_spec_add(s_current, a_delta);

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

template
<typename tp_func>
static auto WINAPI
dango::
concurrent_cpp::
thread_start_address
(LPVOID const a_data)noexcept->DWORD
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
#ifndef DANGO_NO_DEBUG
    dango_unreachable_msg("uncaught exception in thread");
#else
    dango::terminate();
#endif
  }

  return DWORD(0);
}

#endif /* _WIN32 */

#endif /* DANGO_SOURCE_FILE */

