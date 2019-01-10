#ifndef __DANGO_CONCURRENT_HPP__
#define __DANGO_CONCURRENT_HPP__ 1

#include "dango-macro.hpp"
#include "dango-traits.hpp"
#include "dango-atomic.hpp"
#include "dango-assert.hpp"

/*** exec_once ***/

namespace
dango::detail
{
  void spin_yield(dango::uint32&)noexcept;
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
  dango::uint32
  {
    EXECUTED, EXECUTING, INITIAL
  };
public:
  constexpr exec_once()noexcept;

  ~exec_once()noexcept = default;

  template
  <typename tp_func, typename... tp_args>
  auto exec
  (tp_func&&, tp_args&&...)
  noexcept(dango::is_noexcept_callable_ret<tp_func, void, tp_args...>)->
  dango::enable_if<dango::is_callable_ret<tp_func, void, tp_args...>, bool>;

  template
  <typename tp_func, typename... tp_args>
  auto exec
  (tp_func&&, tp_args&&...)
  noexcept(dango::is_noexcept_callable_ret<tp_func, bool, tp_args...>)->
  dango::enable_if<dango::is_callable_ret<tp_func, bool, tp_args...>, bool>;

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

  auto a_count = dango::uint32(0);

  do
  {
    state a_expected = state::INITIAL;

    if(m_state.compare_exchange<acquire, acquire>(a_expected, state::EXECUTING))
    {
      break;
    }

    if(a_expected == state::EXECUTED)
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
release
(bool const a_success)noexcept
{
  constexpr auto const release = dango::mem_order::release;

  m_state.store<release>(a_success ? state::EXECUTED : state::INITIAL);
}

template
<typename tp_func, typename... tp_args>
auto
dango::
exec_once::
exec
(tp_func&& a_func, tp_args&&... a_args)
noexcept(dango::is_noexcept_callable_ret<tp_func, void, tp_args...>)->
dango::enable_if<dango::is_callable_ret<tp_func, void, tp_args...>, bool>
{
  if(dango::likely(!try_acquire()))
  {
    return false;
  }

  if constexpr(dango::is_noexcept_callable_ret<tp_func, void, tp_args...>)
  {
    a_func(dango::forward<tp_args>(a_args)...);
  }
  else
  {
    try
    {
      a_func(dango::forward<tp_args>(a_args)...);
    }
    catch(...)
    {
      release(false);

      throw;
    }
  }

  release(true);

  return true;
}

template
<typename tp_func, typename... tp_args>
auto
dango::
exec_once::
exec
(tp_func&& a_func, tp_args&&... a_args)
noexcept(dango::is_noexcept_callable_ret<tp_func, bool, tp_args...>)->
dango::enable_if<dango::is_callable_ret<tp_func, bool, tp_args...>, bool>
{
  if(dango::likely(!try_acquire()))
  {
    return false;
  }

  bool a_success;

  if constexpr(dango::is_noexcept_callable_ret<tp_func, bool, tp_args...>)
  {
    a_success = a_func(dango::forward<tp_args>(a_args)...);
  }
  else
  {
    try
    {
      a_success = a_func(dango::forward<tp_args>(a_args)...);
    }
    catch(...)
    {
      release(false);

      throw;
    }
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
  return m_state.load<dango::mem_order::acquire>() == state::EXECUTED;
}

inline void
dango::
exec_once::
reset
()noexcept
{
  constexpr auto const acquire = dango::mem_order::acquire;
  constexpr auto const release = dango::mem_order::release;

  if(m_state.load<acquire>() == state::INITIAL)
  {
    return;
  }

  auto a_count = dango::uint32(0);

  do
  {
    state a_expected = state::EXECUTED;

    if(m_state.compare_exchange<release, acquire>(a_expected, state::INITIAL))
    {
      break;
    }

    if(a_expected == state::INITIAL)
    {
      break;
    }

    detail::spin_yield(a_count);
  }
  while(true);
}

/*** spin_lock ***/

namespace
dango
{
  class spin_lock;
}

class
dango::
spin_lock
final
{
private:
  class locker;
  class try_locker;
public:
  constexpr spin_lock()noexcept;

  ~spin_lock()noexcept = default;

  [[nodiscard]] auto lock()noexcept->locker;
  [[nodiscard]] auto try_lock()noexcept->try_locker;
private:
  auto acquire()noexcept->spin_lock*;
  auto try_acquire()noexcept->spin_lock*;
  void release()noexcept;
private:
  dango::atomic<bool> m_locked;
public:
  DANGO_IMMOBILE(spin_lock)
};

class
dango::
spin_lock::
locker
final
{
public:
  locker(spin_lock* const a_lock)noexcept:m_lock{ a_lock->acquire() }{ }
  ~locker()noexcept{ m_lock->release(); }
private:
  spin_lock* const m_lock;
public:
  DANGO_DELETE_DEFAULT(locker)
  DANGO_IMMOBILE(locker)
};

class
dango::
spin_lock::
try_locker
final
{
public:
  try_locker(spin_lock* const a_lock)noexcept:m_lock{ a_lock->try_acquire() }{ }
  ~try_locker()noexcept{ if(m_lock){ m_lock->release(); } }
  explicit operator bool()const{ return m_lock != nullptr; }
private:
  spin_lock* const m_lock;
public:
  DANGO_DELETE_DEFAULT(try_locker)
  DANGO_IMMOBILE(try_locker)
};

constexpr
dango::
spin_lock::
spin_lock
()noexcept:
m_locked{ false }
{

}

inline auto
dango::
spin_lock::
acquire
()noexcept->spin_lock*
{
  constexpr auto const acquire = dango::mem_order::acquire;

  auto a_count = dango::uint32(0);

  do
  {
    if(!m_locked.load<acquire>())
    {
      if(!m_locked.exchange<acquire>(true))
      {
        break;
      }
    }

    detail::spin_yield(a_count);
  }
  while(true);

  return this;
}

inline auto
dango::
spin_lock::
try_acquire
()noexcept->spin_lock*
{
  constexpr auto const acquire = dango::mem_order::acquire;

  if(m_locked.load<acquire>())
  {
    return nullptr;
  }

  if(m_locked.exchange<acquire>(true))
  {
    return nullptr;
  }

  return this;
}

inline void
dango::
spin_lock::
release
()noexcept
{
  m_locked.store<dango::mem_order::release>(false);
}

inline auto
dango::
spin_lock::
lock
()noexcept->locker
{
  return locker{ this };
}

inline auto
dango::
spin_lock::
try_lock
()noexcept->try_locker
{
  return try_locker{ this };
}

/*** thread ***/

namespace
dango
{
  class thread;
}

class
dango::
thread
final
{
public:
  static void yield()noexcept;
};


inline void
dango::
detail::
spin_yield
(dango::uint32& a_count)noexcept
{
  auto const a_current = a_count++;

  if(a_current < dango::uint32(10))
  {
    return;
  }

  if(a_current < dango::uint32(100))
  {
    __builtin_ia32_pause();

    return;
  }

  dango::thread::yield();
}

#ifdef DANGO_SOURCE_FILE

#ifdef __linux__

#include <pthread.h>

void
dango::
thread::
yield
()noexcept
{
  pthread_yield();
}

#endif /* __linux__ */

#endif /* DANGO_SOURCE_FILE */

#endif

