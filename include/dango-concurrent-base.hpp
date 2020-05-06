#ifndef DANGO_CONCURRENT_BASE_HPP_INCLUDED
#define DANGO_CONCURRENT_BASE_HPP_INCLUDED

/*** dango_crit ***/

#define dango_crit(lockable) \
dango_crit_full(lockable, DANGO_APPEND_LINE(dango_crit_var_))

#define dango_crit_full(lockable, local_name) \
if constexpr(auto const local_name = (lockable).lock(); true)

#define dango_crit_cond(cond, mutex, local_name) \
if constexpr(auto const local_name = (cond).lock(mutex); true)

#define dango_try_crit(lockable) \
dango_try_crit_full(lockable, DANGO_APPEND_LINE(dango_crit_var_))

#define dango_try_crit_full(lockable, local_name) \
if(auto const local_name = (lockable).try_lock(); local_name)

#define dango_try_crit_cond(cond, mutex, local_name) \
if(auto const local_name = (cond).try_lock(mutex); local_name)

/*** thread_yield ***/

namespace
dango
{
  DANGO_EXPORT void thread_yield()noexcept;
}

/*** exec_once ***/

namespace
dango::detail
{
  void spin_yield(dango::uint&)noexcept;

  inline constexpr auto const c_spin_count_init = dango::uint(128);
}

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
private:
  enum class
  state:
  dango::ubyte
  {
    EXECUTED, EXECUTING, INITIAL
  };
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
  bool try_acquire()noexcept;
  void release(bool)noexcept;
private:
  dango::atomic<state> m_state;
public:
  DANGO_IMMOBILE(exec_once)
};

constexpr
dango::
exec_once::
exec_once
()noexcept:
m_state{ state::INITIAL }
{

}

inline auto
dango::
exec_once::
try_acquire
()noexcept->bool
{
  using dango::mem_order::acquire;

  if(dango::likely(m_state.load<acquire>() == state::EXECUTED))
  {
    return false;
  }

  auto a_count = detail::c_spin_count_init;

  do
  {
    auto a_expected = state::INITIAL;

    if(m_state.compare_exchange<acquire, acquire>(a_expected, state::EXECUTING))
    {
      break;
    }

    if(a_expected == state::EXECUTED)
    {
      return false;
    }

    while(m_state.load<acquire>() == state::EXECUTING)
    {
      detail::spin_yield(a_count);
    }
  }
  while(true);

  return true;
}

inline void
dango::
exec_once::
release
(bool const a_success)noexcept
{
  using dango::mem_order::release;

  m_state.store<release>(a_success ? state::EXECUTED : state::INITIAL);
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
  if(dango::likely(!try_acquire()))
  {
    return false;
  }

  if constexpr(dango::is_noexcept_callable_ret<void, tp_func, tp_args...>)
  {
    dango::forward<tp_func>(a_func)(dango::forward<tp_args>(a_args)...);
  }
  else
  {
    auto a_guard = dango::make_guard([this]()noexcept->void{ release(false); });

    dango::forward<tp_func>(a_func)(dango::forward<tp_args>(a_args)...);

    a_guard.dismiss();
  }

  release(true);

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
  if(dango::likely(!try_acquire()))
  {
    return false;
  }

  bool a_success;

  if constexpr(dango::is_noexcept_callable_ret<bool, tp_func, tp_args...>)
  {
    a_success =  dango::forward<tp_func>(a_func)(dango::forward<tp_args>(a_args)...);
  }
  else
  {
    auto a_guard = dango::make_guard([this]()noexcept->void{ release(false); });

    a_success =  dango::forward<tp_func>(a_func)(dango::forward<tp_args>(a_args)...);

    a_guard.dismiss();
  }

  release(a_success);

  return a_success;
}

inline auto
dango::
exec_once::
has_executed
()const noexcept->bool
{
  using dango::mem_order::acquire;

  auto a_count = detail::c_spin_count_init;

  do
  {
    auto const a_state = m_state.load<acquire>();

    if(a_state == state::EXECUTED)
    {
      break;
    }

    if(a_state == state::INITIAL)
    {
      return false;
    }

    detail::spin_yield(a_count);
  }
  while(true);

  return true;
}

inline void
dango::
exec_once::
reset
()noexcept
{
  using dango::mem_order::acquire;

  if(m_state.load<acquire>() == state::INITIAL)
  {
    return;
  }

  auto a_count = detail::c_spin_count_init;

  do
  {
    auto a_expected = state::EXECUTED;

    if(m_state.compare_exchange<acquire, acquire>(a_expected, state::INITIAL))
    {
      break;
    }

    if(a_expected == state::INITIAL)
    {
      break;
    }

    while(m_state.load<acquire>() == state::EXECUTING)
    {
      detail::spin_yield(a_count);
    }
  }
  while(true);
}

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
  dango::atomic<bool> m_locked;
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
m_locked{ false }
{

}

inline auto
dango::
spin_mutex::
acquire
()noexcept->spin_mutex*
{
  using dango::mem_order::acquire;

  auto a_count = detail::c_spin_count_init;

  while(m_locked.exchange<acquire>(true))
  {
    while(m_locked.load<acquire>())
    {
      detail::spin_yield(a_count);
    }
  }

  return this;
}

inline auto
dango::
spin_mutex::
try_acquire
()noexcept->spin_mutex*
{
  using dango::mem_order::acquire;

  if(m_locked.load<acquire>())
  {
    return dango::null;
  }

  if(m_locked.exchange<acquire>(true))
  {
    return dango::null;
  }

  return this;
}

inline void
dango::
spin_mutex::
release
()noexcept
{
  m_locked.store<dango::mem_order::release>(false);
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

/*** spin_yield ***/

#ifndef DANGO_NO_MULTICORE
inline void
dango::
detail::
spin_yield
(dango::uint& a_count)noexcept
{
  if(a_count != dango::uint(0))
  {
    --a_count;

    __builtin_ia32_pause();

    return;
  }

  dango::thread_yield();
}
#else
inline void
dango::
detail::
spin_yield
(dango::uint&)noexcept
{
  dango::thread_yield();
}
#endif

/*** is_all_decay_constructible is_decay_construct_correct_order is_all_decay_copy_constructible ***/

namespace
dango::detail
{
  template
  <typename tp_arg>
  concept is_decay_construct_field_help =
    !dango::is_noexcept_constructible<dango::decay<tp_arg>, tp_arg> ||
    dango::is_lvalue_ref<tp_arg> ||
    dango::is_trivial_constructible<dango::decay<tp_arg>, tp_arg>;

  template
  <typename... tp_args>
  constexpr auto
  is_decay_construct_correct_order_help()noexcept->bool
  {
    constexpr bool const a_field[sizeof...(tp_args)]{ bool(dango::detail::is_decay_construct_field_help<tp_args>)... };

    auto a_true_allowed = true;

    for(auto const a_val : a_field)
    {
      if(a_val)
      {
        if(a_true_allowed)
        {
          continue;
        }
        else
        {
          return false;
        }
      }

      a_true_allowed = false;
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
    ( ... && dango::is_constructible<dango::decay<tp_args>, tp_args>);

  template
  <typename... tp_args>
  concept is_decay_construct_correct_order =
    dango::detail::is_decay_construct_correct_order_help<tp_args...>();

  template
  <typename... tp_args>
  concept is_all_decay_copy_constructible =
    ( ... && dango::is_constructible<dango::decay<tp_args>, dango::remove_ref<tp_args>&>);
}

#endif // DANGO_CONCURRENT_BASE_HPP_INCLUDED

