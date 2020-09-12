#ifndef DANGO_CONCURRENT_BASE_HPP_INCLUDED
#define DANGO_CONCURRENT_BASE_HPP_INCLUDED

#include "dango-assert.hpp"
#include "dango-atomic.hpp"
#include "dango-util.hpp"

/*** dango_crit ***/

#define dango_crit(lockable) \
dango_crit_full(lockable, DANGO_APPEND_LINE(dango_crit_guard_))

#define dango_crit_full(lockable, local_name) \
if constexpr(auto const local_name = (lockable).lock(); true)

#define dango_try_crit(lockable) \
dango_try_crit_full(lockable, DANGO_APPEND_LINE(dango_crit_guard_))

#define dango_try_crit_full(lockable, local_name) \
if(auto const local_name = (lockable).try_lock(); local_name)

/*** thread_yeld_soft thread_yield_hard ***/

namespace
dango::detail
{
  DANGO_EXPORT void thread_sleep(dango::uint)noexcept;
}

namespace
dango
{
  inline void thread_yield_soft()noexcept{ dango::detail::thread_sleep(dango::uint(0)); }
  inline void thread_yield_hard()noexcept{ dango::detail::thread_sleep(dango::uint(1)); }
}

/*** busy_wait_while ***/

namespace
dango
{
  template
  <typename tp_cond>
  requires(dango::is_callable_ret<bool, tp_cond>)
  void
  busy_wait_while
  (tp_cond&&, dango::uint)
  noexcept(dango::is_noexcept_callable_ret<bool, tp_cond>);
}

#ifndef DANGO_NO_MULTICORE
template
<typename tp_cond>
requires(dango::is_callable_ret<bool, tp_cond>)
void
dango::
busy_wait_while
(tp_cond&& a_cond, dango::uint const a_count)
noexcept(dango::is_noexcept_callable_ret<bool, tp_cond>)
{
  auto a_soft_yields = dango::uint(0);

  for(auto a_spin = dango::uint(0); dango::forward<tp_cond>(a_cond)(); ++a_spin)
  {
    if(a_spin < a_count)
    {
      __builtin_ia32_pause();

      continue;
    }

    a_spin = dango::uint(0);

    if(a_soft_yields < dango::uint(4))
    {
      dango::thread_yield_soft();

      ++a_soft_yields;

      continue;
    }

    dango::thread_yield_hard();
  }
}
#else
template
<typename tp_cond>
requires(dango::is_callable_ret<bool, tp_cond>)
void
dango::
busy_wait_while
(tp_cond&& a_cond, dango::uint const)
noexcept(dango::is_noexcept_callable_ret<bool, tp_cond>)
{
  auto a_soft_yields = dango::uint(0);

  while(dango::forward<tp_cond>(a_cond)())
  {
    if(a_soft_yields < dango::uint(4))
    {
      dango::thread_yield_soft();

      ++a_soft_yields;

      continue;
    }

    dango::thread_yield_hard();
  }
}
#endif

/*** crit_section ***/

namespace
dango
{
  class
  crit_section
  {
  protected:
    explicit constexpr crit_section()noexcept = default;
    ~crit_section()noexcept = default;
  public:
    DANGO_UNMOVEABLE(crit_section)
  };
}

/*** spin_mutex ***/

namespace
dango
{
  class spin_mutex;
}

class
dango::
spin_mutex
final
{
private:
  using count_type = dango::uint;
public:
  class locker;
  class try_locker;
public:
  explicit constexpr spin_mutex()noexcept;
  ~spin_mutex()noexcept = default;
public:
  [[nodiscard]] auto lock()noexcept->locker;
  [[nodiscard]] auto try_lock()noexcept->try_locker;
private:
  auto acquire()noexcept->spin_mutex*;
  auto try_acquire()noexcept->spin_mutex*;
  void release()noexcept;
private:
  dango::atomic<count_type> m_in;
  dango::atomic<count_type> m_out;
public:
  DANGO_UNMOVEABLE(spin_mutex)
};

namespace
dango
{
  using spin_mutex_locker = dango::spin_mutex::locker;
  using spin_mutex_try_locker = dango::spin_mutex::try_locker;
}

class
dango::
spin_mutex::
locker
final:
public dango::crit_section
{
  friend auto dango::spin_mutex::lock()noexcept->locker;
private:
  using super_type = dango::crit_section;
private:
  explicit
  locker(spin_mutex* const a_lock)noexcept:
  super_type{ },
  m_lock{ a_lock->acquire() }
  { }
public:
  ~locker()noexcept{ unlock(); }
public:
  void
  unlock()noexcept
  {
    if(m_lock)
    {
      m_lock->release();
      m_lock = dango::null;
    }
  }
private:
  spin_mutex* m_lock;
public:
  DANGO_DELETE_DEFAULT(locker)
  DANGO_UNMOVEABLE(locker)
};

class
dango::
spin_mutex::
try_locker
final:
public dango::crit_section
{
  friend auto dango::spin_mutex::try_lock()noexcept->try_locker;
private:
  using super_type = dango::crit_section;
private:
  explicit
  try_locker(spin_mutex* const a_lock)noexcept:
  super_type{ },
  m_lock{ a_lock->try_acquire() }
  { }
public:
  ~try_locker()noexcept{ unlock(); }
public:
  void
  unlock()noexcept
  {
    if(m_lock)
    {
      m_lock->release();
      m_lock = dango::null;
    }
  }
public:
  explicit operator bool()const{ return m_lock != dango::null; }
private:
  spin_mutex* m_lock;
public:
  DANGO_DELETE_DEFAULT(try_locker)
  DANGO_UNMOVEABLE(try_locker)
};

constexpr
dango::
spin_mutex::
spin_mutex
()noexcept:
m_in{ count_type(0) },
m_out{ count_type(0) }
{

}

inline auto
dango::
spin_mutex::
acquire
()noexcept->spin_mutex*
{
  using dango::mem_order::acquire;
  using dango::mem_order::relaxed;

  auto const a_in = m_in.fetch_add<relaxed>(count_type(1));

  auto const a_cond =
    [this, a_in]()noexcept->bool{ return m_out.load<acquire>() != a_in; };

  dango::busy_wait_while(a_cond, dango::uint(16));

  return this;
}

inline auto
dango::
spin_mutex::
try_acquire
()noexcept->spin_mutex*
{
  using dango::mem_order::acquire;
  using dango::mem_order::relaxed;

  auto a_in = m_in.load<relaxed>();

  if(m_out.load<relaxed>() != a_in)
  {
    return dango::null;
  }

  auto const a_next = a_in + count_type(1);

  if(m_in.compare_exchange<acquire, relaxed>(a_in, a_next))
  {
    return this;
  }

  return dango::null;
}

inline void
dango::
spin_mutex::
release
()noexcept
{
  using dango::mem_order::release;
  using dango::mem_order::relaxed;

  auto const a_out = m_out.load<relaxed>();

  m_out.store<release>(a_out + count_type(1));
}

inline auto
dango::
spin_mutex::
lock
()noexcept->locker
{
  return locker{ this };
}

inline auto
dango::
spin_mutex::
try_lock
()noexcept->try_locker
{
  return try_locker{ this };
}

/*** exec_once ***/

namespace
dango
{
  class exec_once;
}

class
dango::
exec_once
final
{
public:
  explicit constexpr exec_once()noexcept;
  ~exec_once()noexcept = default;
public:
  template
  <typename tp_func, typename... tp_args>
  requires(dango::is_callable_ret<void, tp_func, tp_args...>)
  auto exec
  (tp_func&&, tp_args&&...)
  noexcept(dango::is_noexcept_callable_ret<void, tp_func, tp_args...>)->bool;

  template
  <typename tp_func, typename... tp_args>
  requires(dango::is_callable_ret<bool, tp_func, tp_args...>)
  auto exec
  (tp_func&&, tp_args&&...)
  noexcept(dango::is_noexcept_callable_ret<bool, tp_func, tp_args...>)->bool;
public:
  auto has_executed()const noexcept->bool;
  void reset()noexcept;
private:
  mutable dango::spin_mutex m_lock;
  bool m_executed;
public:
  DANGO_UNMOVEABLE(exec_once)
};

constexpr
dango::
exec_once::
exec_once
()noexcept:
m_lock{ },
m_executed{ false }
{

}

template
<typename tp_func, typename... tp_args>
requires(dango::is_callable_ret<void, tp_func, tp_args...>)
auto
dango::
exec_once::
exec
(tp_func&& a_func, tp_args&&... a_args)
noexcept(dango::is_noexcept_callable_ret<void, tp_func, tp_args...>)->bool
{
  dango_crit(m_lock)
  {
    if(dango::likely(m_executed))
    {
      return false;
    }

    dango::forward<tp_func>(a_func)(dango::forward<tp_args>(a_args)...);

    m_executed = true;
  }

  return true;
}

template
<typename tp_func, typename... tp_args>
requires(dango::is_callable_ret<bool, tp_func, tp_args...>)
auto
dango::
exec_once::
exec
(tp_func&& a_func, tp_args&&... a_args)
noexcept(dango::is_noexcept_callable_ret<bool, tp_func, tp_args...>)->bool
{
  dango_crit(m_lock)
  {
    if(dango::likely(m_executed))
    {
      return false;
    }

    if(!dango::forward<tp_func>(a_func)(dango::forward<tp_args>(a_args)...))
    {
      return false;
    }

    m_executed = true;
  }

  return true;
}

inline auto
dango::
exec_once::
has_executed
()const noexcept->bool
{
  dango_crit(m_lock)
  {
    return m_executed;
  }
}

inline void
dango::
exec_once::
reset
()noexcept
{
  dango_crit(m_lock)
  {
    m_executed = false;
  }
}

/*** atomic_ref_count ***/

namespace
dango
{
  class atomic_ref_count;
}

class
dango::
atomic_ref_count
final
{
public:
  using value_type = dango::usize;
public:
  explicit constexpr
  atomic_ref_count()noexcept:
#ifndef DANGO_NO_DEBUG
  m_lock{ },
#endif
  m_count{ value_type(1) }
  { }
  ~atomic_ref_count()noexcept = default;
public:
#ifdef DANGO_NO_DEBUG
  void
  increment()noexcept
  {
    m_count.add_fetch<dango::mem_order::acquire>(value_type(1));
  }

  [[nodiscard]] auto
  decrement()noexcept->bool
  {
    return m_count.sub_fetch<dango::mem_order::release>(value_type(1)) == value_type(0);
  }
#else
  void
  increment()noexcept
  {
    dango_crit(m_lock)
    {
      dango_assert(m_count != value_type(0));

      ++m_count;
    }
  }

  [[nodiscard]] auto
  decrement()noexcept->bool
  {
    dango_crit(m_lock)
    {
      dango_assert(m_count != value_type(0));

      --m_count;

      return m_count == value_type(0);
    }
  }
#endif
private:
#ifdef DANGO_NO_DEBUG
  dango::atomic<value_type> m_count;
#else
  dango::spin_mutex m_lock;
  value_type m_count;
#endif
public:
  DANGO_UNMOVEABLE(atomic_ref_count)
};

/*** atomic_ref_count_ws ***/

namespace
dango
{
  class atomic_ref_count_ws;
}

class
dango::
atomic_ref_count_ws
final
{
public:
  using value_type = dango::usize;
public:
  explicit constexpr
  atomic_ref_count_ws()noexcept:
#ifndef DANGO_NO_DEBUG
  m_lock{ },
#endif
  m_wcount{ value_type(1) },
  m_scount{ value_type(1) }
  { }
  ~atomic_ref_count_ws()noexcept = default;
public:
#ifdef DANGO_NO_DEBUG
  void
  weak_increment()noexcept
  {
    m_wcount.add_fetch<dango::mem_order::acquire>(value_type(1));
  }

  void
  strong_increment()noexcept
  {
    using dango::mem_order::acquire;

    m_wcount.add_fetch<acquire>(value_type(1));
    m_scount.add_fetch<acquire>(value_type(1));
  }

  [[nodiscard]] auto
  try_strong_increment()noexcept->bool
  {
    using dango::mem_order::acquire;
    using dango::mem_order::release;
    using dango::mem_order::relaxed;

    m_wcount.add_fetch<acquire>(value_type(1));

    auto a_guard =
      dango::make_guard([this]()noexcept->void{ m_wcount.sub_fetch<release>(value_type(1)); });

    auto a_strong = m_scount.load<relaxed>();

    if(a_strong == value_type(0))
    {
      return false;
    }

    auto const a_cond =
      [this, &a_strong]()noexcept->bool
      {
        auto const a_next = a_strong + value_type(1);

        if(m_scount.compare_exchange<acquire, relaxed, true>(a_strong, a_next))
        {
          return false;
        }

        return a_strong != value_type(0);
      };

    dango::busy_wait_while(a_cond, dango::uint(16));

    if(a_strong == value_type(0))
    {
      return false;
    }

    a_guard.dismiss();

    return true;
  }

  [[nodiscard]] auto
  weak_decrement()noexcept->bool
  {
    return m_wcount.sub_fetch<dango::mem_order::release>(value_type(1)) == value_type(0);
  }

  template
  <typename tp_func>
  requires(dango::is_noexcept_callable_ret<void, tp_func>)
  [[nodiscard]] auto
  strong_decrement
  (tp_func&& a_func)noexcept->bool
  {
    if(m_scount.sub_fetch<dango::mem_order::release>(value_type(1)) == value_type(0))
    {
      dango::forward<tp_func>(a_func)();
    }

    return weak_decrement();
  }
#else
  void
  weak_increment()noexcept
  {
    dango_crit(m_lock)
    {
      dango_assert(m_wcount != value_type(0));

      ++m_wcount;
    }
  }

  void
  strong_increment()noexcept
  {
    dango_crit(m_lock)
    {
      dango_assert(m_wcount != value_type(0));
      dango_assert(m_scount != value_type(0));

      ++m_wcount;
      ++m_scount;
    }
  }

  [[nodiscard]] auto
  try_strong_increment()noexcept->bool
  {
    dango_crit(m_lock)
    {
      dango_assert(m_wcount != value_type(0));

      if(m_scount == value_type(0))
      {
        return false;
      }

      ++m_wcount;
      ++m_scount;
    }

    return true;
  }

  [[nodiscard]] auto
  weak_decrement()noexcept->bool
  {
    dango_crit(m_lock)
    {
      dango_assert(m_wcount != value_type(0));

      --m_wcount;

      return m_wcount == value_type(0);
    }
  }

  template
  <typename tp_func>
  requires(dango::is_noexcept_callable_ret<void, tp_func>)
  [[nodiscard]] auto
  strong_decrement
  (tp_func&& a_func)noexcept->bool
  {
    auto a_exec = false;

    dango_crit(m_lock)
    {
      dango_assert(m_scount != value_type(0));

      if(--m_scount == value_type(0))
      {
        a_exec = true;
      }
    }

    if(a_exec)
    {
      dango::forward<tp_func>(a_func)();
    }

    return weak_decrement();
  }
#endif
private:
#ifdef DANGO_NO_DEBUG
  dango::atomic<value_type> m_wcount;
  dango::atomic<value_type> m_scount;
#else
  dango::spin_mutex m_lock;
  value_type m_wcount;
  value_type m_scount;
#endif
public:
  DANGO_UNMOVEABLE(atomic_ref_count_ws)
};

#endif // DANGO_CONCURRENT_BASE_HPP_INCLUDED
