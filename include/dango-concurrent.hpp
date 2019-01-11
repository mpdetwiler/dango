#ifndef __DANGO_CONCURRENT_HPP__
#define __DANGO_CONCURRENT_HPP__ 1

#include "dango-macro.hpp"
#include "dango-traits.hpp"
#include "dango-atomic.hpp"
#include "dango-assert.hpp"
#include "dango-mem.hpp"

/*** dango_crit dango_try_crit ***/

#define dango_crit(name) \
if constexpr(auto const DANGO_APPEND_LINE(dango_crit_var_) = (name).lock())

#define dango_try_crit(name) \
if(auto const DANGO_APPEND_LINE(dango_crit_var_) = (name).try_lock())

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
    auto a_expected = state::INITIAL;

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
  constexpr auto const acquire = dango::mem_order::acquire;

  auto a_count = dango::uint32(0);

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
  constexpr auto const release = dango::mem_order::release;

  if(m_state.load<acquire>() == state::INITIAL)
  {
    return;
  }

  auto a_count = dango::uint32(0);

  do
  {
    auto a_expected = state::EXECUTED;

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
  explicit constexpr operator bool()const{ return true; }
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
  explicit operator bool()const{ return m_lock != nullptr; }
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
spin_mutex::
try_acquire
()noexcept->spin_mutex*
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

/*** mutex ***/

namespace
dango::detail
{
  using primitive_storage = dango::aligned_storage<dango::usize(48)>;
}

namespace
dango
{
  class mutex;
}

class alignas(dango::cache_align_type)
dango::
mutex
final
{
private:
  class locker;
  class try_locker;
public:
  constexpr mutex()noexcept;

  ~mutex()noexcept;

  [[nodiscard]] auto lock()noexcept->locker;
  [[nodiscard]] auto try_lock()noexcept->try_locker;
private:
  template
  <typename tp_type>
  auto get()noexcept->tp_type*;
  void init()noexcept;
  auto acquire()noexcept->mutex*;
  auto try_acquire()noexcept->mutex*;
  void release()noexcept;
private:
  detail::primitive_storage m_storage;
  dango::exec_once m_init;
public:
  DANGO_IMMOBILE(mutex)
};

class
dango::
mutex::
locker
final
{
public:
  locker(mutex* const a_lock)noexcept:m_lock{ a_lock->acquire() }{ }
  ~locker()noexcept{ m_lock->release(); }
  explicit constexpr operator bool()const{ return true; }
private:
  mutex* const m_lock;
public:
  DANGO_DELETE_DEFAULT(locker)
  DANGO_IMMOBILE(locker)
};

class
dango::
mutex::
try_locker
final
{
public:
  try_locker(mutex* const a_lock)noexcept:m_lock{ a_lock->try_acquire() }{ }
  ~try_locker()noexcept{ if(m_lock){ m_lock->release(); } }
  explicit operator bool()const{ return m_lock != nullptr; }
private:
  mutex* const m_lock;
public:
  DANGO_DELETE_DEFAULT(try_locker)
  DANGO_IMMOBILE(try_locker)
};

constexpr
dango::
mutex::
mutex
()noexcept:
m_storage{ },
m_init{ }
{

}

inline auto
dango::
mutex::
lock
()noexcept->locker
{
  return locker{ this };
}

inline auto
dango::
mutex::
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
#include <errno.h>

/*** mutex ***/

dango::
mutex::
~mutex
()noexcept
{
  using type = pthread_mutex_t;

  if(!m_init.has_executed())
  {
    return;
  }

  auto const a_result = pthread_mutex_destroy(get<type>());

  dango_assert(a_result == 0);
}

template
<typename tp_type>
auto
dango::
mutex::
get
()noexcept->tp_type*
{
  using type = pthread_mutex_t;

  static_assert(dango::is_same<dango::remove_cv<tp_type>, type>);

  return static_cast<type*>(m_storage.get());
}

void
dango::
mutex::
init
()noexcept
{
  using type = pthread_mutex_t;

  static_assert(sizeof(type) <= sizeof(detail::primitive_storage));
  static_assert(alignof(type) <= alignof(detail::primitive_storage));

  m_init.exec
  (
    [this]()noexcept->void
    {
      auto const a_prim = ::new (dango::placement, m_storage.get()) type;

      auto const a_result = pthread_mutex_init(a_prim, nullptr);

      dango_assert(a_result == 0);
    }
  );
}

auto
dango::
mutex::
acquire
()noexcept->mutex*
{
  using type = pthread_mutex_t;

  init();

  auto const a_result = pthread_mutex_lock(get<type>());

  dango_assert(a_result == 0);

  return this;
}

auto
dango::
mutex::
try_acquire
()noexcept->mutex*
{
  using type = pthread_mutex_t;

  init();

  auto const a_result = pthread_mutex_trylock(get<type>());

  if(a_result == 0)
  {
    return this;
  }

  dango_assert(a_result == EBUSY);

  return nullptr;
}

void
dango::
mutex::
release
()noexcept
{
  using type = pthread_mutex_t;

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  auto const  a_result = pthread_mutex_unlock(get<type>());

  dango_assert(a_result == 0);
}

/*** thread ***/

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

