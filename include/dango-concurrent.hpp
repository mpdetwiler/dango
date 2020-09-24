#ifndef DANGO_CONCURRENT_BASE_HPP_INCLUDED
#define DANGO_CONCURRENT_BASE_HPP_INCLUDED

#include "dango-assert.hpp"
#include "dango-atomic.hpp"
#include "dango-int.hpp"
#include "dango-mem.hpp"
#include "dango-tuple.hpp"
#include "dango-util.hpp"

/*** thread_yeld ***/

namespace
dango
{
  DANGO_EXPORT void thread_yield(bool)noexcept;
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
      dango::thread_yield(false);

      ++a_soft_yields;

      continue;
    }

    dango::thread_yield(true);
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
      dango::thread_yield(false);

      ++a_soft_yields;

      continue;
    }

    dango::thread_yield(true);
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
  constexpr ~spin_mutex()noexcept = default;
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
  locker(spin_mutex* const a_mutex)noexcept:
  super_type{ },
  m_mutex{ a_mutex->acquire() }
  { }
public:
  ~locker()noexcept{ if(is_locked()){ m_mutex->release(); } }
public:
  void
  unlock()noexcept
  {
    if(is_locked())
    {
      dango::exchange(m_mutex, dango::null)->release();
    }
  }
  auto is_locked()const noexcept->bool{ return !dango::is_null(m_mutex); }
private:
  spin_mutex* m_mutex;
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
  try_locker(spin_mutex* const a_mutex)noexcept:
  super_type{ },
  m_mutex{ a_mutex->try_acquire() }
  { }
public:
  ~try_locker()noexcept{ if(is_locked()){ m_mutex->release(); } }
public:
  explicit operator bool()const{ return is_locked(); }
public:
  void
  unlock()noexcept
  {
    if(is_locked())
    {
      dango::exchange(m_mutex, dango::null)->release();
    }
  }
  auto is_locked()const noexcept->bool{ return !dango::is_null(m_mutex); }
private:
  spin_mutex* m_mutex;
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

  dango::busy_wait_while(a_cond, dango::uint(32));

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
  constexpr ~exec_once()noexcept = default;
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
private:
  mutable dango::spin_mutex m_lock;
  dango::atomic<bool> m_executed;
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
  using dango::mem_order::acquire;
  using dango::mem_order::release;
  using dango::mem_order::relaxed;

  if(dango::likely(m_executed.load<acquire>()))
  {
    return false;
  }

  dango_crit(m_lock)
  {
    if(m_executed.load<relaxed>())
    {
      return false;
    }

    dango::forward<tp_func>(a_func)(dango::forward<tp_args>(a_args)...);

    m_executed.store<release>(true);
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
  using dango::mem_order::acquire;
  using dango::mem_order::release;
  using dango::mem_order::relaxed;

  if(dango::likely(m_executed.load<acquire>()))
  {
    return false;
  }

  dango_crit(m_lock)
  {
    if(m_executed.load<relaxed>())
    {
      return false;
    }

    if(!dango::forward<tp_func>(a_func)(dango::forward<tp_args>(a_args)...))
    {
      return false;
    }

    m_executed.store<release>(true);
  }

  return true;
}

inline auto
dango::
exec_once::
has_executed
()const noexcept->bool
{
  return dango::likely(m_executed.load<dango::mem_order::acquire>());
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

/*** tick_count ***/

namespace
dango
{
  enum class tick_count_type:dango::slong{ };

  constexpr auto
  operator +
  (dango::tick_count_type const a_lhs, dango::tick_count_type a_rhs)noexcept->dango::tick_count_type
  {
    return dango::tick_count_type{ dango::integer::safe_add(dango::slong(a_lhs), dango::slong(a_rhs)) };
  }

  constexpr auto
  operator -
  (dango::tick_count_type const a_lhs, dango::tick_count_type a_rhs)noexcept->dango::tick_count_type
  {
    return dango::tick_count_type{ dango::integer::safe_sub(dango::slong(a_lhs), dango::slong(a_rhs)) };
  }

  constexpr auto
  operator +=
  (dango::tick_count_type& a_lhs, dango::tick_count_type const a_rhs)noexcept->dango::tick_count_type&
  {
    return (a_lhs = a_lhs + a_rhs);
  }

  constexpr auto
  operator -=
  (dango::tick_count_type& a_lhs, dango::tick_count_type const a_rhs)noexcept->dango::tick_count_type&
  {
    return (a_lhs = a_lhs - a_rhs);
  }
}

namespace
dango::tick
{
  inline constexpr auto const zero = dango::tick_count_type{ 0 };

  constexpr auto
  to_nano
  (dango::tick_count_type const a_count)noexcept->dango::slong{ return dango::slong(a_count) / dango::slong(1L); }
  constexpr auto
  tp_micro
  (dango::tick_count_type const a_count)noexcept->dango::slong{ return dango::slong(a_count) / dango::slong(1'000L); }
  constexpr auto
  to_milli
  (dango::tick_count_type const a_count)noexcept->dango::slong{ return dango::slong(a_count) / dango::slong(1'000'000L); }
  constexpr auto
  to_sec
  (dango::tick_count_type const a_count)noexcept->dango::slong{ return dango::slong(a_count) / dango::slong(1'000'000'000L); }

  constexpr auto
  from_nano
  (dango::slong const a_count)noexcept->auto{ return dango::tick_count_type{ a_count * dango::slong(1L) }; }
  constexpr auto
  from_micro
  (dango::slong const a_count)noexcept->auto{ return dango::tick_count_type{ a_count * dango::slong(1'000L) }; }
  constexpr auto
  from_milli
  (dango::slong const a_count)noexcept->auto{ return dango::tick_count_type{ a_count * dango::slong(1'000'000L) }; }
  constexpr auto
  from_sec
  (dango::slong const a_count)noexcept->auto{ return dango::tick_count_type{ a_count * dango::slong(1'000'000'000L) }; }
}

namespace
dango
{
  using tick_count_pair = dango::pair<dango::tick_count_type, dango::tick_count_type>;

  DANGO_EXPORT auto current_tick_and_suspend_bias()noexcept->dango::tick_count_pair;

  auto current_tick()noexcept->dango::tick_count_type;
  auto current_tick_sa()noexcept->dango::tick_count_type;
  auto current_suspend_bias()noexcept->dango::tick_count_type;
}

inline auto
dango::
current_tick
()noexcept->dango::tick_count_type
{
  return dango::current_tick_and_suspend_bias().first();
}

inline auto
dango::
current_tick_sa
()noexcept->dango::tick_count_type
{
  auto const a_pair = dango::current_tick_and_suspend_bias();

  return a_pair.first() + a_pair.second();
}

inline auto
dango::
current_suspend_bias
()noexcept->dango::tick_count_type
{
  return dango::current_tick_and_suspend_bias().second();
}

#ifndef DANGO_BUILDING_LIB
namespace
dango::detail
{
  [[maybe_unused]] inline bool const
  s_current_tick_init_bool =
    (void(dango::current_tick_and_suspend_bias()), false);
}
#endif

/*** timeout ***/

namespace
dango
{
  enum class timeout_flag:dango::uint;

  constexpr auto
  operator |
  (dango::timeout_flag const a_lhs, dango::timeout_flag const a_rhs)noexcept->dango::timeout_flag
  {
    return dango::timeout_flag{ dango::uint(a_lhs) | dango::uint(a_rhs) };
  }

  constexpr auto
  operator &
  (dango::timeout_flag const a_lhs, dango::timeout_flag const a_rhs)noexcept->dango::timeout_flag
  {
    return dango::timeout_flag{ dango::uint(a_lhs) & dango::uint(a_rhs) };
  }

  enum class
  timeout_flag:dango::uint
  {
    ZERO = dango::uint(0),
    DEFAULT = ZERO,
    HIGH_RES = dango::uint(1 << 0),
    SUSPEND_AWARE = dango::uint(1 << 1),
    valid_combo_00 = DEFAULT,
    valid_combo_01 = HIGH_RES,
    valid_combo_02 = SUSPEND_AWARE,
    valid_combo_03 = HIGH_RES | SUSPEND_AWARE
  };

  namespace
  timeout_flags
  {
    inline constexpr auto const DEFAULT = dango::timeout_flag::DEFAULT;
    inline constexpr auto const HIGH_RES = dango::timeout_flag::HIGH_RES;
    inline constexpr auto const SUSPEND_AWARE = dango::timeout_flag::SUSPEND_AWARE;
  }

  class timeout;
}

namespace
dango::detail
{
  class timeout_base;
}

class
dango::
detail::
timeout_base
{
public:
  using value_type = dango::tick_count_type;
protected:
  explicit constexpr timeout_base()noexcept;
public:
  constexpr virtual ~timeout_base()noexcept = default;
public:
  virtual auto is_high_res()const noexcept->bool = 0;
  virtual auto is_suspend_aware()const noexcept->bool = 0;
  auto has_expired()const noexcept->bool;
  auto remaining()const noexcept->value_type;
  auto get()const noexcept->value_type;
  void set(value_type)noexcept;
  void set_rel(value_type)noexcept;
  void add(value_type)noexcept;
private:
  virtual auto this_tick_count()const noexcept->value_type = 0;
private:
  value_type m_timeout;
public:
  DANGO_UNMOVEABLE(timeout_base)
};

constexpr
dango::
detail::
timeout_base::
timeout_base
()noexcept:
m_timeout{ value_type(0) }
{

}

inline auto
dango::
detail::
timeout_base::
has_expired
()const noexcept->bool
{
  return this_tick_count() >= m_timeout;
}

inline auto
dango::
detail::
timeout_base::
remaining
()const noexcept->value_type
{
  return m_timeout - this_tick_count();
}

inline auto
dango::
detail::
timeout_base::
get
()const noexcept->value_type
{
  return m_timeout;
}

inline void
dango::
detail::
timeout_base::
set
(value_type const a_abs)noexcept
{
  m_timeout = a_abs;
}

inline void
dango::
detail::
timeout_base::
set_rel
(value_type const a_rel)noexcept
{
  m_timeout = (this_tick_count() + a_rel);
}

inline void
dango::
detail::
timeout_base::
add
(value_type const a_add)noexcept
{
  m_timeout += a_add;
}

/*** timeout_impl ***/

namespace
dango::detail
{
  template
  <dango::timeout_flag tp_flags>
  class timeout_impl;
}

template
<dango::timeout_flag tp_flags>
class
dango::
detail::
timeout_impl
final:
public dango::detail::timeout_base
{
private:
  using super_type = dango::detail::timeout_base;
private:
  static inline constexpr auto const c_high_res = bool(tp_flags & dango::timeout_flag::HIGH_RES);
  static inline constexpr auto const c_suspend_aware = bool(tp_flags & dango::timeout_flag::SUSPEND_AWARE);
private:
  static auto
  this_tick_count_impl()noexcept->value_type
  {
    if constexpr(c_suspend_aware)
    {
      return dango::current_tick_sa();
    }
    else
    {
      return dango::current_tick();
    }
  }
public:
  explicit constexpr timeout_impl()noexcept:super_type{ }{ }
  constexpr ~timeout_impl()noexcept override = default;
  auto is_high_res()const noexcept->bool override{ return c_high_res; }
  auto is_suspend_aware()const noexcept->bool override{ return c_suspend_aware; }
private:
  auto this_tick_count()const noexcept->value_type override{ return this_tick_count_impl(); }
public:
  DANGO_UNMOVEABLE(timeout_impl)
};

/*** timeout ***/

namespace
dango::detail
{
  using timeout_storage =
    dango::aligned_union
    <
      dango::detail::timeout_impl<dango::timeout_flag::valid_combo_00>,
      dango::detail::timeout_impl<dango::timeout_flag::valid_combo_01>,
      dango::detail::timeout_impl<dango::timeout_flag::valid_combo_02>,
      dango::detail::timeout_impl<dango::timeout_flag::valid_combo_03>
    >;
}

class
dango::
timeout
final
{
public:
  using value_type = dango::detail::timeout_base::value_type;
private:
  template
  <dango::timeout_flag tp_flags>
  using timeout_type = dango::detail::timeout_impl<tp_flags>;
private:
  static auto construct_help(void*, dango::timeout_flag)noexcept->dango::detail::timeout_base*;
public:
  static auto make(value_type, dango::timeout_flag)noexcept->timeout;
  static auto make_rel(value_type, dango::timeout_flag)noexcept->timeout;
  static auto make(value_type const a_value)noexcept->timeout{ return make(a_value, dango::timeout_flag::DEFAULT); }
  static auto make_rel(value_type const a_value)noexcept->timeout{ return make_rel(a_value, dango::timeout_flag::DEFAULT); }
private:
  explicit timeout(value_type, bool, dango::timeout_flag)noexcept;
public:
  ~timeout()noexcept{ dango::destructor(m_timeout); }
public:
  auto is_high_res()const noexcept->bool{ return m_timeout->is_high_res(); }
  auto is_suspend_aware()const noexcept->bool{ return m_timeout->is_suspend_aware(); }
  auto has_expired()const noexcept->bool{ return m_timeout->has_expired(); }
  auto remaining()const noexcept->value_type{ return m_timeout->remaining(); }
  auto remaining_ms()const noexcept->dango::ulong;
  auto get()const noexcept->value_type{ return m_timeout->get(); }
  void set(value_type const a_abs)noexcept{ m_timeout->set(a_abs); }
  void set_rel(value_type const a_rel)noexcept{ m_timeout->set_rel(a_rel); }
  void add(value_type const a_add)noexcept{ m_timeout->add(a_add); }
private:
  dango::detail::timeout_storage m_storage;
  dango::detail::timeout_base* const m_timeout;
public:
  DANGO_DELETE_DEFAULT(timeout)
  DANGO_UNMOVEABLE(timeout)
};

inline auto
dango::
timeout::
construct_help
(void* const a_storage, dango::timeout_flag const a_flags)noexcept->dango::detail::timeout_base*
{
  using dango::timeout_flags::DEFAULT;
  using dango::timeout_flags::HIGH_RES;
  using dango::timeout_flags::SUSPEND_AWARE;

  switch(a_flags)
  {
    case DEFAULT:
    {
      using type = timeout_type<DEFAULT>;

      return dango_placement_new(a_storage, type, { });
    }
    break;
    case HIGH_RES:
    {
      using type = timeout_type<HIGH_RES>;

      return dango_placement_new(a_storage, type, { });
    }
    break;
    case SUSPEND_AWARE:
    {
      using type = timeout_type<SUSPEND_AWARE>;

      return dango_placement_new(a_storage, type, { });
    }
    break;
    case HIGH_RES | SUSPEND_AWARE:
    {
      using type = timeout_type<HIGH_RES | SUSPEND_AWARE>;

      return dango_placement_new(a_storage, type, { });
    }
    break;
    default: break;
  }

  dango_unreachable;
}

inline auto
dango::
timeout::
make
(value_type const a_value, dango::timeout_flag const a_flags)noexcept->timeout
{
  return timeout{ a_value, false, a_flags };
}

inline auto
dango::
timeout::
make_rel
(value_type const a_value, dango::timeout_flag const a_flags)noexcept->timeout
{
  return timeout{ a_value, true, a_flags };
}

inline
dango::
timeout::
timeout
(value_type const a_value, bool const a_rel, dango::timeout_flag const a_flags)noexcept:
m_storage{ },
m_timeout{ construct_help(m_storage.get(), a_flags) }
{
  dango_assert(m_timeout != dango::null);

  if(a_rel)
  {
    m_timeout->set_rel(a_value);
  }
  else
  {
    m_timeout->set(a_value);
  }
}

inline auto
dango::
timeout::
remaining_ms
()const noexcept->dango::ulong
{
  using u64 = dango::ulong;

  auto const a_rem = remaining();

  if(a_rem <= dango::tick::zero)
  {
    return u64(0);
  }

  auto const a_ns = u64(dango::tick::to_nano(a_rem));
  auto const a_ms = a_ns / u64(1'000'000L);
  auto const a_rm = a_ns % u64(1'000'000L);

#ifdef DANGO_PLATFORM_APPLE
  constexpr auto const c_thresh = u64(2);
#else
  constexpr auto const c_thresh = u64(1);
#endif

  if(is_high_res())
  {
    if(a_ms <= c_thresh)
    {
      return u64(0);
    }

    return a_ms - c_thresh;
  }

  return a_ms + u64(a_rm != u64(0));
}

#endif // DANGO_CONCURRENT_BASE_HPP_INCLUDED
