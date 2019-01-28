#ifndef __DANGO_CONCURRENT_HPP__
#define __DANGO_CONCURRENT_HPP__ 1

#include "dango-macro.hpp"
#include "dango-traits.hpp"
#include "dango-atomic.hpp"
#include "dango-assert.hpp"
#include "dango-mem.hpp"

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
  dango::uint8
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

/*** mutex_base ***/

namespace
dango::detail
{
  using primitive_storage = dango::aligned_storage<dango::usize(48)>;
  class mutex_base;
  class cond_var_base;
}

class alignas(dango::cache_align_type)
dango::
detail::
mutex_base
{
  friend dango::detail::cond_var_base;
protected:
  class locker;
  class try_locker;
protected:
  constexpr mutex_base()noexcept;
  ~mutex_base()noexcept = default;
  void destroy()noexcept;
public:
  [[nodiscard]] auto lock()noexcept->locker;
  [[nodiscard]] auto try_lock()noexcept->try_locker;
private:
  template
  <typename tp_type>
  auto get()noexcept->tp_type*;
  void init()noexcept;
  auto acquire()noexcept->mutex_base*;
  auto try_acquire()noexcept->mutex_base*;
  void release()noexcept;
private:
  detail::primitive_storage m_storage;
  dango::exec_once m_init;
public:
  DANGO_IMMOBILE(mutex_base)
};

class
dango::
detail::
mutex_base::
locker
final
{
public:
  locker(mutex_base* const a_lock)noexcept:m_lock{ a_lock->acquire() }{ }
  ~locker()noexcept{ m_lock->release(); }
private:
  mutex_base* const m_lock;
public:
  DANGO_DELETE_DEFAULT(locker)
  DANGO_IMMOBILE(locker)
};

class
dango::
detail::
mutex_base::
try_locker
final
{
public:
  try_locker(mutex_base* const a_lock)noexcept:m_lock{ a_lock->try_acquire() }{ }
  ~try_locker()noexcept{ if(m_lock){ m_lock->release(); } }
  explicit operator bool()const{ return m_lock != nullptr; }
private:
  mutex_base* const m_lock;
public:
  DANGO_DELETE_DEFAULT(try_locker)
  DANGO_IMMOBILE(try_locker)
};

constexpr
dango::
detail::
mutex_base::
mutex_base
()noexcept:
m_storage{ },
m_init{ }
{

}

inline auto
dango::
detail::
mutex_base::
lock
()noexcept->locker
{
  return locker{ this };
}

inline auto
dango::
detail::
mutex_base::
try_lock
()noexcept->try_locker
{
  return try_locker{ this };
}

/*** mutex ***/

namespace
dango
{
  class mutex;
}

struct
dango::
mutex
final:
dango::detail::mutex_base
{
private:
  using super_type = dango::detail::mutex_base;
public:
  constexpr mutex()noexcept;
  ~mutex()noexcept;
public:
  DANGO_IMMOBILE(mutex)
};

constexpr
dango::
mutex::
mutex
()noexcept:
super_type{ }
{

}

inline
dango::
mutex::
~mutex
()noexcept
{
  destroy();
}

/*** static_mutex ***/

namespace
dango
{
  class static_mutex;
}

struct
dango::
static_mutex
final:
dango::detail::mutex_base
{
private:
  using super_type = dango::detail::mutex_base;
public:
  constexpr static_mutex()noexcept;
  ~static_mutex()noexcept = default;
public:
  DANGO_IMMOBILE(static_mutex)
};

constexpr
dango::
static_mutex::
static_mutex
()noexcept:
super_type{ }
{

}

/*** cond_var_base ***/

namespace
dango::detail
{
  class cond_var_base;
}

class alignas(dango::cache_align_type)
dango::
detail::
cond_var_base
{
protected:
  class locker;
  class try_locker;
  using mutex_type = dango::detail::mutex_base;
protected:
  constexpr cond_var_base()noexcept;
  ~cond_var_base()noexcept = default;
  void destroy()noexcept;
private:
  template
  <typename tp_type>
  auto get()noexcept->tp_type*;
  void init()noexcept;
  void wait(mutex_type*)noexcept;
  void notify(mutex_type*)noexcept;
  void notify_all(mutex_type*)noexcept;
private:
  detail::primitive_storage m_storage;
  dango::exec_once m_init;
public:
  DANGO_IMMOBILE(cond_var_base)
};

class
dango::
detail::
cond_var_base::
locker
final
{
public:
  locker(mutex_type* const a_lock, cond_var_base* const a_cond)noexcept:
  m_lock{ a_lock->acquire() },
  m_cond{ a_cond }
  {

  }

  ~locker()noexcept{ m_lock->release(); }
  void wait()const noexcept{ m_cond->wait(m_lock); }
  void notify()const noexcept{ m_cond->notify(m_lock); }
  void notify_all()const noexcept{ m_cond->notify_all(m_lock); }
private:
  mutex_type* const m_lock;
  cond_var_base* const m_cond;
public:
  DANGO_DELETE_DEFAULT(locker)
  DANGO_IMMOBILE(locker)
};

class
dango::
detail::
cond_var_base::
try_locker
final
{
public:
  try_locker(mutex_type* const a_lock, cond_var_base* const a_cond)noexcept:
  m_lock{ a_lock->try_acquire() },
  m_cond{ a_cond }
  {

  }

  ~try_locker()noexcept{ if(m_lock){ m_lock->release(); } }
  explicit operator bool()const{ return m_lock != nullptr; }

  void wait(DANGO_SRC_LOC_ARG_DEFAULT(a_loc))const noexcept
  {
    dango_assert_loc(m_lock != nullptr, a_loc); m_cond->wait(m_lock);
  }

  void notify(DANGO_SRC_LOC_ARG_DEFAULT(a_loc))const noexcept
  {
    dango_assert_loc(m_lock != nullptr, a_loc); m_cond->notify(m_lock);
  }

  void notify_all(DANGO_SRC_LOC_ARG_DEFAULT(a_loc))const noexcept
  {
    dango_assert_loc(m_lock != nullptr, a_loc); m_cond->notify_all(m_lock);
  }
private:
  mutex_type* const m_lock;
  cond_var_base* const m_cond;
public:
  DANGO_DELETE_DEFAULT(try_locker)
  DANGO_IMMOBILE(try_locker)
};

constexpr
dango::
detail::
cond_var_base::
cond_var_base
()noexcept:
m_storage{ },
m_init{ }
{

}

/*** cond_var ***/

namespace
dango
{
  class cond_var;
}

class
dango::
cond_var
final:
dango::detail::cond_var_base
{
private:
  using super_type = dango::detail::cond_var_base;
public:
  constexpr cond_var()noexcept;
  ~cond_var()noexcept;
  [[nodiscard]] auto lock(mutex_type&)noexcept->locker;
  [[nodiscard]] auto try_lock(mutex_type&)noexcept->try_locker;
public:
  DANGO_IMMOBILE(cond_var)
};

constexpr
dango::
cond_var::
cond_var
()noexcept:
super_type{ }
{

}

inline
dango::
cond_var::
~cond_var
()noexcept
{
  destroy();
}

inline auto
dango::
cond_var::
lock
(mutex_type& a_lock)noexcept->locker
{
  return locker{ &a_lock, this };
}

inline auto
dango::
cond_var::
try_lock
(mutex_type& a_lock)noexcept->try_locker
{
  return try_locker{ &a_lock, this };
}

/*** static_cond_var ***/

namespace
dango
{
  class static_cond_var;
}

class
dango::
static_cond_var
final:
dango::detail::cond_var_base
{
private:
  using super_type = dango::detail::cond_var_base;
public:
  constexpr static_cond_var()noexcept;
  ~static_cond_var()noexcept = default;
  [[nodiscard]] auto lock(mutex_type&)noexcept->locker;
  [[nodiscard]] auto try_lock(mutex_type&)noexcept->try_locker;
public:
  DANGO_IMMOBILE(static_cond_var)
};

constexpr
dango::
static_cond_var::
static_cond_var
()noexcept:
super_type{ }
{

}

inline auto
dango::
static_cond_var::
lock
(mutex_type& a_lock)noexcept->locker
{
  return locker{ &a_lock, this };
}

inline auto
dango::
static_cond_var::
try_lock
(mutex_type& a_lock)noexcept->try_locker
{
  return try_locker{ &a_lock, this };
}

/*** cond_var_mutex ***/

namespace
dango
{
  class cond_var_mutex;
}

class
dango::
cond_var_mutex
final:
dango::detail::cond_var_base
{
private:
  using super_type = dango::detail::cond_var_base;
public:
  explicit constexpr cond_var_mutex(mutex_type&)noexcept;
  ~cond_var_mutex()noexcept;
  [[nodiscard]] auto lock()noexcept->locker;
  [[nodiscard]] auto try_lock()noexcept->try_locker;
private:
  mutex_type* const m_lock;
public:
  DANGO_DELETE_DEFAULT(cond_var_mutex)
  DANGO_IMMOBILE(cond_var_mutex)
};

constexpr
dango::
cond_var_mutex::
cond_var_mutex
(mutex_type& a_lock)noexcept:
super_type{ },
m_lock{ &a_lock }
{

}

inline
dango::
cond_var_mutex::
~cond_var_mutex
()noexcept
{
  destroy();
}

inline auto
dango::
cond_var_mutex::
lock
()noexcept->locker
{
  return locker{ m_lock, this };
}

inline auto
dango::
cond_var_mutex::
try_lock
()noexcept->try_locker
{
  return try_locker{ m_lock, this };
}

/*** static_cond_var_mutex ***/

namespace
dango
{
  class static_cond_var_mutex;
}

class
dango::
static_cond_var_mutex
final:
dango::detail::cond_var_base
{
private:
  using super_type = dango::detail::cond_var_base;
  using mutex_type = dango::static_mutex;
public:
  explicit constexpr static_cond_var_mutex(mutex_type&)noexcept;
  ~static_cond_var_mutex()noexcept = default;
  [[nodiscard]] auto lock()noexcept->locker;
  [[nodiscard]] auto try_lock()noexcept->try_locker;
private:
  mutex_type* const m_lock;
public:
  DANGO_DELETE_DEFAULT(static_cond_var_mutex)
  DANGO_IMMOBILE(static_cond_var_mutex)
};

constexpr
dango::
static_cond_var_mutex::
static_cond_var_mutex
(mutex_type& a_lock)noexcept:
super_type{ },
m_lock{ &a_lock }
{

}

inline auto
dango::
static_cond_var_mutex::
lock
()noexcept->locker
{
  return locker{ m_lock, this };
}

inline auto
dango::
static_cond_var_mutex::
try_lock
()noexcept->try_locker
{
  return try_locker{ m_lock, this };
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
private:
  using thread_start_func = void(*)(void*)noexcept(false);
  class control_block;
  class notifier;
private:
  static auto new_control_block()noexcept->control_block*;

  static void
  start_thread
  (thread_start_func, void*)noexcept(false);

  template
  <typename tp_func>
  static void thread_start_address(void*)noexcept(false);

  static thread const& s_main_init;
public:
  static void yield()noexcept;
  static auto self()noexcept->thread const&;

  template
  <typename tp_func>
  static auto
  create(tp_func&&)noexcept(false)->
  dango::enable_if<dango::is_callable_ret<dango::decay<tp_func>&, void>, thread>;
private:
  thread()noexcept;
public:
  constexpr thread(dango::null_tag)noexcept;
  thread(thread const&)noexcept;
  thread(thread&&)noexcept;
  ~thread()noexcept;
  auto operator = (dango::null_tag)&noexcept->thread&;
  auto operator = (thread const&)&noexcept->thread&;
  auto operator = (thread&&)&noexcept->thread&;
  explicit constexpr operator bool()const noexcept;
  auto is_running()const noexcept->bool;
  void join()const noexcept;
private:
  control_block* m_control;
};

/*** control_block ***/

class
dango::
thread::
control_block
final
{
public:
  static auto operator new(dango::usize)dango_new_noexcept(true)->void*;
  static void operator delete(void*)noexcept;
public:
  constexpr control_block()noexcept;
  ~control_block()noexcept;
  void increment()noexcept;
  auto decrement()noexcept->bool;
  void wait()noexcept;
  void notify_all()noexcept;
  auto is_running()const noexcept->bool;
private:
  dango::atomic<dango::usize> m_ref_count;
  mutable dango::mutex m_mutex;
  mutable dango::cond_var_mutex m_cond;
  bool m_running;
  dango::usize m_waiter_count;
public:
  DANGO_IMMOBILE(control_block)
};

inline void
dango::
thread::
control_block::
operator delete
(void* const a_ptr)noexcept
{
  dango::operator_delete(a_ptr, sizeof(control_block), alignof(control_block));
}

inline
dango::
thread::
control_block::
~control_block
()noexcept = default;

inline void
dango::
thread::
control_block::
increment
()noexcept
{
  m_ref_count.add_fetch(dango::usize(1));
}

inline auto
dango::
thread::
control_block::
decrement
()noexcept->bool
{
  return m_ref_count.sub_fetch(dango::usize(1)) == dango::usize(0);
}

/*** thread ***/

inline dango::thread const&
dango::
thread::
s_main_init = dango::thread::self();

constexpr
dango::
thread::
thread
(dango::null_tag const)noexcept:
m_control{ nullptr }
{

}

inline
dango::
thread::
thread
(thread const& a_thread)noexcept:
m_control{ a_thread.m_control }
{
  if(m_control)
  {
    m_control->increment();
  }
}

inline
dango::
thread::
thread
(thread&& a_thread)noexcept:
m_control{ a_thread.m_control }
{
  a_thread.m_control = nullptr;
}

inline
dango::
thread::
~thread
()noexcept
{
  if(m_control && m_control->decrement())
  {
    delete m_control;
  }
}

inline auto
dango::
thread::
operator =
(dango::null_tag const)&noexcept->thread&
{
  thread a_temp{ dango::null };

  dango::swap(m_control, a_temp.m_control);

  return *this;
}

inline auto
dango::
thread::
operator =
(thread const& a_thread)&noexcept->thread&
{
  thread a_temp{ a_thread };

  dango::swap(m_control, a_temp.m_control);

  return *this;
}

inline auto
dango::
thread::
operator =
(thread&& a_thread)&noexcept->thread&
{
  thread a_temp{ dango::move(a_thread) };

  dango::swap(m_control, a_temp.m_control);

  return *this;
}

constexpr
dango::
thread::
operator bool
()const noexcept
{
  return m_control != nullptr;
}

template
<typename tp_func>
auto
dango::
thread::
create
(tp_func&& a_thread_func)noexcept(false)->
dango::enable_if<dango::is_callable_ret<dango::decay<tp_func>&, void>, thread>
{
  constexpr auto const acquire = dango::mem_order::acquire;
  constexpr auto const release = dango::mem_order::release;

  using func_type = dango::decay<tp_func>;

  auto a_mem = dango::operator_new(sizeof(func_type), alignof(func_type));

  auto const a_thread_func_ptr =
    ::new (dango::placement, a_mem.get()) func_type{ dango::forward<tp_func>(a_thread_func) };

  auto a_guard =
  dango::make_guard
  (
    [a_thread_func_ptr]()noexcept->void
    {
      dango::destructor(a_thread_func_ptr);
    }
  );

  thread a_ret{ dango::null };

  dango::atomic<bool> a_starting{ true };

  auto a_func =
  [
    a_mem = a_mem.get(),
    &a_thread_func = *a_thread_func_ptr,
    &a_ret,
    &a_starting
  ]
  ()noexcept(false)->void
  {
    a_ret = dango::thread::self();

    a_starting.store<release>(false);

    auto const a_finally =
    dango::make_finally
    (
      [a_mem, &a_thread_func]()noexcept->void
      {
        dango::destructor(&a_thread_func);

        dango::operator_delete(a_mem, sizeof(func_type), alignof(func_type));
      }
    );

    a_thread_func();
  };

  start_thread(thread_start_address<decltype(a_func)>, &a_func);

  auto a_count = dango::uint32(0);

  while(a_starting.load<acquire>())
  {
    detail::spin_yield(a_count);
  }

  a_guard.dismiss();

  a_mem.dismiss();

  return a_ret;
}

template
<typename tp_func>
void
dango::
thread::
thread_start_address
(void* const a_data)noexcept(false)
{
  static_assert(!dango::is_const<tp_func> && !dango::is_volatile<tp_func>);
  static_assert(dango::is_noexcept_move_constructible<tp_func>);
  static_assert(dango::is_callable_ret<tp_func&, void>);

  auto const a_func_ptr = static_cast<tp_func*>(a_data);

  auto const a_func = dango::move(*a_func_ptr);

  a_func();
}

/*** spin_yield ***/

#ifndef DANGO_NO_MULTICORE
inline void
dango::
detail::
spin_yield
(dango::uint32& a_count)noexcept
{
  auto const a_current = a_count++;

  if(a_current < dango::uint32(16))
  {
    return;
  }

  if(a_current < dango::uint32(128))
  {
    __builtin_ia32_pause();

    return;
  }

  dango::thread::yield();
}
#else
inline void
dango::
detail::
spin_yield
(dango::uint32&)noexcept
{
  dango::thread::yield();
}
#endif

#ifdef DANGO_SOURCE_FILE

/*** control_block ***/

auto
dango::
thread::
control_block::
operator new
(dango::usize const a_size)dango_new_noexcept(true)->void*
{
  constexpr auto c_size = sizeof(control_block);

  dango_assert(a_size == c_size);

  return dango::operator_new(c_size, alignof(control_block)).dismiss();
}

constexpr
dango::
thread::
control_block::
control_block
()noexcept:
m_ref_count{ dango::usize(1) },
m_mutex{ },
m_cond{ m_mutex },
m_running{ true },
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
    while(m_running)
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
notify_all
()noexcept
{
  dango_crit_full(m_cond, a_crit)
  {
    m_running = false;

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
is_running
()const noexcept->bool
{
  dango_crit(m_mutex)
  {
    return m_running;
  }
}

/*** notifier ***/

class
dango::
thread::
notifier
final
{
public:
  constexpr notifier(thread const&)noexcept;
  ~notifier()noexcept;
private:
  control_block* const m_control;
public:
  DANGO_DELETE_DEFAULT(notifier)
  DANGO_IMMOBILE(notifier)
};

constexpr
dango::
thread::
notifier::
notifier
(thread const& a_thread)noexcept:
m_control{ a_thread.m_control }
{

}

dango::
thread::
notifier::
~notifier
()noexcept
{
  m_control->notify_all();
}

/*** thread ***/

dango::
thread::
thread
()noexcept:
m_control{ new_control_block() }
{

}

auto
dango::
thread::
is_running
()const noexcept->bool
{
  dango_assert(m_control != nullptr);

  return m_control->is_running();
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

auto
dango::
thread::
new_control_block
()noexcept->control_block*
{
  try
  {
    return new control_block{ };
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
self
()noexcept->thread const&
{
  thread_local thread const t_thread{ };

  thread_local notifier const t_notifier{ t_thread };

  return t_thread;
}

#ifdef __linux__

#include <pthread.h>
#include <errno.h>

/*** mutex_base ***/

template
<typename tp_type>
auto
dango::
detail::
mutex_base::
get
()noexcept->tp_type*
{
  using type = pthread_mutex_t;

  static_assert(dango::is_same<dango::remove_cv<tp_type>, type>);

  return dango::launder(static_cast<type*>(m_storage.get()));
}

void
dango::
detail::
mutex_base::
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
      pthread_mutexattr_t a_attr;

      pthread_mutexattr_init(&a_attr);
      pthread_mutexattr_settype(&a_attr, PTHREAD_MUTEX_NORMAL);

      auto const a_prim = ::new (dango::placement, m_storage.get()) type;

      auto const a_result = pthread_mutex_init(a_prim, &a_attr);

      dango_assert(a_result == 0);

      pthread_mutexattr_destroy(&a_attr);
    }
  );
}

void
dango::
detail::
mutex_base::
destroy
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

auto
dango::
detail::
mutex_base::
acquire
()noexcept->mutex_base*
{
  using type = pthread_mutex_t;

  init();

  auto const a_result = pthread_mutex_lock(get<type>());

  dango_assert(a_result == 0);

  return this;
}

auto
dango::
detail::
mutex_base::
try_acquire
()noexcept->mutex_base*
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
detail::
mutex_base::
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

/*** cond_var_base ***/

template
<typename tp_type>
auto
dango::
detail::
cond_var_base::
get
()noexcept->tp_type*
{
  using type = pthread_cond_t;

  static_assert(dango::is_same<dango::remove_cv<tp_type>, type>);

  return dango::launder(static_cast<type*>(m_storage.get()));
}

void
dango::
detail::
cond_var_base::
init
()noexcept
{
  using type = pthread_cond_t;

  static_assert(sizeof(type) <= sizeof(detail::primitive_storage));
  static_assert(alignof(type) <= alignof(detail::primitive_storage));

  m_init.exec
  (
    [this]()noexcept->void
    {
      pthread_condattr_t a_attr;

      pthread_condattr_init(&a_attr);
      pthread_condattr_setclock(&a_attr, CLOCK_MONOTONIC);

      auto const a_prim = ::new (dango::placement, m_storage.get()) type;

      auto const a_result = pthread_cond_init(a_prim, &a_attr);

      dango_assert(a_result == 0);

      pthread_condattr_destroy(&a_attr);
    }
  );
}

void
dango::
detail::
cond_var_base::
destroy
()noexcept
{
  using type = pthread_cond_t;

  if(!m_init.has_executed())
  {
    return;
  }

  auto const a_result = pthread_cond_destroy(get<type>());

  dango_assert(a_result == 0);
}

void
dango::
detail::
cond_var_base::
wait
(mutex_type* const a_lock)noexcept
{
  using type = pthread_cond_t;
  using lock_type = pthread_mutex_t;

  init();

  auto const a_result =
    pthread_cond_wait(get<type>(), a_lock->get<lock_type>());

  dango_assert(a_result == 0);
}

void
dango::
detail::
cond_var_base::
notify
(mutex_type* const)noexcept
{
  using type = pthread_cond_t;

  init();

  auto const a_result =
    pthread_cond_signal(get<type>());

  dango_assert(a_result == 0);
}

void
dango::
detail::
cond_var_base::
notify_all
(mutex_type* const)noexcept
{
  using type = pthread_cond_t;

  init();

  auto const a_result =
    pthread_cond_broadcast(get<type>());

  dango_assert(a_result == 0);
}

/*** thread ***/

namespace
dango::concurrent_cpp
{
  template
  <typename tp_func>
  auto thread_start_address(void*)noexcept->void*;
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

  auto a_count = dango::uint32(0);

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
  pthread_yield();
}

template
<typename tp_func>
auto
dango::
concurrent_cpp::
thread_start_address
(void* const a_data)noexcept->void*
{
  static_assert(!dango::is_const<tp_func> && !dango::is_volatile<tp_func>);
  static_assert(dango::is_noexcept_move_constructible<tp_func>);
  static_assert(dango::is_callable_ret<tp_func&, void>);

  auto const a_func_ptr = static_cast<tp_func*>(a_data);

  auto const a_func = dango::move(*a_func_ptr);

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

#endif /* __linux__ */

#endif /* DANGO_SOURCE_FILE */

#endif

