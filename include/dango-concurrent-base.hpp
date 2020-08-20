#ifndef DANGO_CONCURRENT_BASE_HPP_INCLUDED
#define DANGO_CONCURRENT_BASE_HPP_INCLUDED

/*** dango_crit ***/

#define dango_crit(lockable) \
dango_crit_full(lockable, DANGO_APPEND_LINE(dango_crit_guard_))

#define dango_crit_full(lockable, local_name) \
if constexpr(auto const local_name = (lockable).lock(); true)

#define dango_try_crit(lockable) \
dango_try_crit_full(lockable, DANGO_APPEND_LINE(dango_crit_guard_))

#define dango_try_crit_full(lockable, local_name) \
if(auto const local_name = (lockable).try_lock(); local_name)

/*** thread_yield ***/

namespace
dango
{
  DANGO_EXPORT void thread_yield(dango::uint)noexcept;
}

/*** busy_wait_while ***/

namespace
dango
{
  template
  <typename tp_cond>
  requires(dango::is_callable_ret<bool, tp_cond>)
  constexpr void
  busy_wait_while
  (tp_cond&&, dango::uint const = dango::uint(16))
  noexcept(dango::is_noexcept_callable_ret<bool, tp_cond>);
}

#ifndef DANGO_NO_MULTICORE
template
<typename tp_cond>
requires(dango::is_callable_ret<bool, tp_cond>)
constexpr void
dango::
busy_wait_while
(tp_cond&& a_cond, dango::uint const a_count)
noexcept(dango::is_noexcept_callable_ret<bool, tp_cond>)
{
  for(auto a_spin = dango::uint(0); dango::forward<tp_cond>(a_cond)(); ++a_spin)
  {
    if(a_spin < a_count)
    {
      __builtin_ia32_pause();

      continue;
    }

    dango::thread_yield(dango::uint(1));

    a_spin = dango::uint(0);
  }
}
#else
template
<typename tp_cond>
requires(dango::is_callable_ret<bool, tp_cond>)
constexpr void
dango::
busy_wait_while
(tp_cond&& a_cond, dango::uint const)
noexcept(dango::is_noexcept_callable_ret<bool, tp_cond>)
{
  while(dango::forward<tp_cond>(a_cond)())
  {
    dango::thread_yield(dango::uint(1));
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
    DANGO_IMMOBILE(crit_section)
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
  DANGO_IMMOBILE(spin_mutex)
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
  ~locker()noexcept{ m_lock->release(); }
private:
  spin_mutex* const m_lock;
public:
  DANGO_DELETE_DEFAULT(locker)
  DANGO_IMMOBILE(locker)
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
  ~try_locker()noexcept{ if(m_lock){ m_lock->release(); } }
public:
  explicit operator bool()const{ return m_lock != dango::null; }
private:
  spin_mutex* const m_lock;
public:
  DANGO_DELETE_DEFAULT(try_locker)
  DANGO_IMMOBILE(try_locker)
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

  dango::busy_wait_while([this, a_in]()noexcept->bool{ return m_out.load<acquire>() != a_in; });

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

  auto const a_in = m_in.load<relaxed>();

  if(m_out.load<relaxed>() != a_in)
  {
    return dango::null;
  }

  auto a_expected = a_in;

  if(m_in.compare_exchange<acquire, relaxed>(a_expected, a_in + count_type(1)))
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
  DANGO_IMMOBILE(exec_once)
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
  DANGO_IMMOBILE(atomic_ref_count)
};

/*** is_all_decay_constructible is_decay_construct_correct_order is_all_decay_copy_constructible ***/

namespace
dango::detail
{
  template
  <typename... tp_args>
  constexpr auto
  is_decay_construct_correct_order_help()noexcept->bool
  {
    constexpr auto const c_size = sizeof...(tp_args) + dango::usize(1);

    constexpr bool const c_throwing_construct[c_size] =
      { true, (!dango::is_noexcept_constructible<dango::decay<tp_args>, tp_args>)... };

    constexpr bool const c_non_trivial_move_construct[c_size] =
      { false, (!dango::is_lvalue_ref<tp_args> && !dango::is_trivial_constructible<dango::decay<tp_args>, tp_args>)... };

    auto a_throwing_construct_allowed = true;

    for(auto a_i = dango::usize(0); a_i != c_size; ++a_i)
    {
      auto const a_throwing_construct = c_throwing_construct[a_i];
      auto const a_non_trivial_move_construct = c_non_trivial_move_construct[a_i];

      dango_assert(!a_throwing_construct || !a_non_trivial_move_construct);

      if(a_throwing_construct)
      {
        if(a_throwing_construct_allowed)
        {
          continue;
        }

        return false;
      }

      if(a_non_trivial_move_construct)
      {
        a_throwing_construct_allowed = false;
      }
    }

    return true;
  }
}

namespace
dango
{
  template
  <typename... tp_args>
  concept is_all_decay_constructible =
    ( ... && dango::is_constructible<dango::decay<tp_args>, dango::conditional<dango::is_noexcept_constructible<dango::decay<tp_args>, tp_args>, tp_args, dango::remove_ref<tp_args>&>>);

  template
  <typename... tp_args>
  concept is_decay_construct_correct_order =
    dango::detail::is_decay_construct_correct_order_help<dango::conditional<dango::is_noexcept_constructible<dango::decay<tp_args>, tp_args>, tp_args, dango::remove_ref<tp_args>&>...>();

  template
  <typename... tp_args>
  concept is_all_decay_copy_constructible =
    ( ... && dango::is_constructible<dango::decay<tp_args>, dango::remove_ref<tp_args>&>);
}

/*** decay_forward ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr auto
  decay_forward
  (dango::remove_ref<tp_type>& a_arg)noexcept->decltype(auto)
  {
    if constexpr(dango::is_noexcept_constructible<dango::decay<tp_type>, tp_type>)
    {
      return dango::forward<tp_type>(a_arg);
    }
    else
    {
      return a_arg;
    }
  }

  template
  <typename tp_type>
  constexpr auto
  decay_forward
  (dango::remove_ref<tp_type>&& a_arg)noexcept->decltype(auto)
  {
    return dango::decay_forward<tp_type>(a_arg);
  }
}

#endif // DANGO_CONCURRENT_BASE_HPP_INCLUDED

