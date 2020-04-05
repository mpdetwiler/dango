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
if(auto const local_name = (lockable).try_lock())

#define dango_try_crit_cond(cond, mutex, local_name) \
if(auto const local_name = (cond).try_lock(mutex))

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
  constexpr exec_once()noexcept;
  ~exec_once()noexcept = default;

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
  constexpr auto const acquire = dango::mem_order::acquire;

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
  constexpr auto const release = dango::mem_order::release;

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
  constexpr auto const acquire = dango::mem_order::acquire;

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
  constexpr auto const acquire = dango::mem_order::acquire;

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
  class locker;
  class try_locker;
public:
  constexpr spin_mutex()noexcept;
  ~spin_mutex()noexcept = default;
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

class
dango::
spin_mutex::
locker
final
{
public:
  locker(spin_mutex* const a_lock)noexcept:m_lock{ a_lock->acquire() }{ }
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
final
{
public:
  try_locker(spin_mutex* const a_lock)noexcept:m_lock{ a_lock->try_acquire() }{ }
  ~try_locker()noexcept{ if(m_lock){ m_lock->release(); } }
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
  constexpr auto const acquire = dango::mem_order::acquire;

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
  constexpr auto const acquire = dango::mem_order::acquire;

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

#endif // DANGO_CONCURRENT_BASE_HPP_INCLUDED

