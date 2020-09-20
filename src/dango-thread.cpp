#include "dango-thread.hpp"

#include "dango-mutex.hpp"
#include "dango-cond-var.hpp"

/*** construct_tag ***/

struct
dango::
thread::
construct_tag
final
{
  DANGO_TAG_TYPE(construct_tag)
};

/*** control_block ***/

class
dango::
thread::
control_block
final:
public dango::intrusive_list_elem<control_block>
{
private:
  using super_type = dango::intrusive_list_elem<control_block>;
public:
  DANGO_DEFINE_CLASS_OPERATOR_NEW_DELETE(control_block)
public:
  explicit control_block(bool, dango::thread_id)dango_new_noexcept;
  ~control_block()noexcept = default;
public:
  auto is_daemon()const noexcept->bool{ return m_daemon; }
  void increment()noexcept{ m_ref_count.increment(); }
  auto decrement()noexcept->bool{ return m_ref_count.decrement(); }
  void wait()noexcept;
  void wait(dango::timeout const&)noexcept;
  void notify_all()noexcept;
  auto get_id()const noexcept->dango::thread_id;
  auto is_alive()const noexcept->bool;
private:
  auto non_null_id(dango::crit_section const&)const noexcept->bool{ return m_thread_id != dango::thread_id::none; }
private:
  DANGO_CACHE_LINE_START
  dango::atomic_ref_count m_ref_count;
  DANGO_CACHE_LINE_START
  bool const m_daemon;
  DANGO_CACHE_LINE_START
  mutable dango::mutex m_mutex;
  mutable dango::cond_var m_cond;
  DANGO_CACHE_LINE_START
  dango::thread_id m_thread_id;
  dango::usize m_waiter_count;
public:
  DANGO_DELETE_DEFAULT(control_block)
  DANGO_UNMOVEABLE(control_block)
};

dango::
thread::
control_block::
control_block
(bool const a_daemon, dango::thread_id const a_thread_id)dango_new_noexcept:
super_type{ },
m_ref_count{ },
m_daemon{ a_daemon },
m_mutex{ },
m_cond{ m_mutex },
m_thread_id{ a_thread_id },
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
    while(non_null_id(a_crit))
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
    while(non_null_id(a_crit) && !a_timeout.has_expired())
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
    m_thread_id = dango::thread_id::none;

    if(m_waiter_count != dango::usize(0))
    {
      a_crit.notify_all();
    }
  }
}

auto
dango::
thread::
control_block::
get_id
()const noexcept->dango::thread_id
{
  dango_crit(m_mutex)
  {
    return m_thread_id;
  }
}

auto
dango::
thread::
control_block::
is_alive
()const noexcept->bool
{
  dango_crit_full(m_mutex, a_crit)
  {
    return non_null_id(a_crit);
  }
}

/*** registry ***/

class
dango::
thread::
registry
final
{
public:
  explicit constexpr registry()noexcept;
  constexpr ~registry()noexcept = default;
  void regist(control_block*)noexcept;
  void unregist(control_block*)noexcept;
  void join_all()noexcept;
private:
  auto join_first()noexcept->bool;
private:
  dango::static_mutex m_mutex;
  dango::intrusive_list<control_block> m_list;
public:
  DANGO_UNMOVEABLE(registry)
};

constexpr
dango::
thread::
registry::
registry
()noexcept:
m_mutex{ },
m_list{ }
{

}

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

    a_thread = dango::thread{ a_control };
  }

  a_thread.join();

  return true;
}

constinit
dango::thread::registry
dango::
thread::
s_registry{ };

/*** notifier ***/

class
dango::
thread::
notifier
final
{
public:
  explicit
  notifier
  (dango::thread const& a_thread)noexcept:
  m_control{ a_thread.m_control }
  {
    if(!m_control->is_daemon())
    {
      dango::thread::s_registry.regist(m_control);
    }
  }

  ~notifier
  ()noexcept
  {
    if(!m_control->is_daemon())
    {
      dango::thread::s_registry.unregist(m_control);
    }

    m_control->notify_all();
  }
private:
  control_block* const m_control;
public:
  DANGO_DELETE_DEFAULT(notifier)
  DANGO_UNMOVEABLE(notifier)
};

/*** thread ***/

auto
dango::
thread::
control_block_new
(bool const a_daemon)noexcept->control_block*
{
  try
  {
    return new thread::control_block{ a_daemon, thread::self_id() };
  }
  catch(...)
  {
    dango_unreachable_terminate_msg(u8"thread control block allocation failed");
  }
}

void
dango::
thread::
control_block_inc
(control_block& a_control)noexcept
{
  a_control.increment();
}

void
dango::
thread::
control_block_dec
(control_block& a_control)noexcept
{
  if(a_control.decrement())
  {
    delete &a_control;
  }
}

auto
dango::
thread::
thread_self_init
(bool const a_daemon)noexcept->dango::thread const*
{
  static thread_local auto const t_daemon = a_daemon;

  DANGO_DEFINE_FUNC_SCOPE_THREAD_LOCAL(dango::thread const, t_thread, { construct_tag{ }, t_daemon });

  static thread_local notifier const t_notifier{ t_thread() };

  return t_thread_try_access();
}

auto
dango::
thread::
self_id
()noexcept->dango::thread_id
{
  static thread_local constinit bool const t_self = false;

  return dango::thread_id{ dango::ptr_as_uint(&t_self) };
}

void
dango::
thread::
main_join
()noexcept
{
  dango::thread::s_registry.join_all();
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

dango::
thread::
thread
(construct_tag const, bool const a_daemon)noexcept:
m_control{ control_block_new(a_daemon) }
{

}

dango::
thread::
thread
(control_block* const a_control)noexcept:
m_control{ a_control }
{
  dango_assert(m_control != dango::null);

  m_control->increment();
}

auto
dango::
thread::
is_alive
()const noexcept->bool
{
  dango_assert(m_control != dango::null);

  return m_control->is_alive();
}

void
dango::
thread::
join
()const noexcept
{
  dango_assert(m_control != dango::null);

  m_control->wait();
}

void
dango::
thread::
join
(dango::timeout const& a_timeout)const noexcept
{
  dango_assert(m_control != dango::null);

  m_control->wait(a_timeout);
}

auto
dango::
thread::
is_daemon
()const noexcept->bool
{
  dango_assert(m_control != dango::null);

  return m_control->is_daemon();
}

auto
dango::
thread::
get_id
()const noexcept->dango::thread_id
{
  dango_assert(m_control != dango::null);

  return m_control->get_id();
}

/*** bad_thread ***/

auto
dango::
bad_thread::
message()const noexcept->dango::bchar const*
{
  return u8"dango::bad_thread";
}

#include "dango-concurrent-private.hpp"

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
