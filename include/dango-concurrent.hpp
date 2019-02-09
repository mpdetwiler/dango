#ifndef __DANGO_CONCURRENT_HPP__
#define __DANGO_CONCURRENT_HPP__ 1

#include "dango-concurrent-base.hpp"
#include "dango-int.hpp"
#include "dango-assert.hpp"
#include "dango-mem.hpp"
#include "dango-global.hpp"

/*** get_tick_count ***/

namespace
dango
{
  auto get_tick_count()noexcept->dango::uint64;
  auto get_tick_count_sa()noexcept->dango::uint64;
}

/*** timeout ***/

namespace
dango
{
  class timeout;
}

class
dango::
timeout
{
protected:
  using value_type = dango::uint64;
protected:
  static constexpr auto safe_add(value_type, value_type)noexcept->value_type;
protected:
  constexpr timeout(value_type)noexcept;
public:
  virtual ~timeout()noexcept = default;
  auto has_expired()const noexcept->bool;
  auto remaining()const noexcept->value_type;
  auto get()const noexcept->value_type;
  void set(value_type)noexcept;
  void set_rel(value_type)noexcept;
  void add(value_type)noexcept;
  virtual auto is_suspend_aware()const noexcept->bool = 0;
private:
  virtual auto tick_count()const noexcept->value_type = 0;
private:
  value_type m_timeout;
public:
  DANGO_DELETE_DEFAULT(timeout)
  DANGO_IMMOBILE(timeout)
};

constexpr auto
dango::
timeout::
safe_add
(value_type const a_lhs, value_type const a_rhs)noexcept->value_type
{
  value_type const a_max_add =
    dango::integer::MAX_VAL<value_type> - a_lhs;

#ifndef DANGO_NO_DEBUG
  dango_assert(a_rhs <= a_max_add);
#endif

  auto const a_add =
    dango::min(a_max_add, a_rhs);

  return a_lhs + a_add;
}

constexpr
dango::
timeout::
timeout
(value_type const a_timeout)noexcept:
m_timeout{ a_timeout }
{

}

inline auto
dango::
timeout::
has_expired
()const noexcept->bool
{
  return tick_count() >= m_timeout;
}

inline auto
dango::
timeout::
remaining
()const noexcept->value_type
{
  auto const a_now = tick_count();

  if(a_now >= m_timeout)
  {
    return value_type(0);
  }

  return m_timeout - a_now;
}

inline auto
dango::
timeout::
get
()const noexcept->value_type
{
  return m_timeout;
}

inline void
dango::
timeout::
set
(value_type const a_timeout)noexcept
{
  m_timeout = a_timeout;
}

inline void
dango::
timeout::
set_rel
(value_type const a_interval)noexcept
{
  auto const a_now = tick_count();

  m_timeout = safe_add(a_now, a_interval);
}

inline void
dango::
timeout::
add
(value_type const a_val)noexcept
{
  m_timeout = safe_add(m_timeout, a_val);
}

/*** timeout_impl ***/

namespace
dango::detail
{
  struct timeout_impl;
}

struct
dango::
detail::
timeout_impl:
dango::timeout
{
private:
  using super_type = dango::timeout;
public:
  static auto make(value_type)noexcept->timeout_impl;
  static auto make_rel(value_type)noexcept->timeout_impl;
protected:
  constexpr timeout_impl(value_type)noexcept;
public:
  virtual ~timeout_impl()noexcept override = default;
  virtual auto is_suspend_aware()const noexcept->bool override final;
private:
  virtual auto tick_count()const noexcept->value_type override final;
public:
  DANGO_DELETE_DEFAULT(timeout_impl)
  DANGO_IMMOBILE(timeout_impl)
};

inline auto
dango::
detail::
timeout_impl::
make
(value_type const a_timeout)noexcept->timeout_impl
{
  return timeout_impl{ a_timeout };
}

inline auto
dango::
detail::
timeout_impl::
make_rel
(value_type const a_interval)noexcept->timeout_impl
{
  auto const a_now = dango::get_tick_count();

  return timeout_impl{ safe_add(a_now, a_interval) };
}

constexpr
dango::
detail::
timeout_impl::
timeout_impl
(value_type const a_timeout)noexcept:
super_type{ a_timeout }
{

}

inline auto
dango::
detail::
timeout_impl::
is_suspend_aware
()const noexcept->bool
{
  return false;
}

inline auto
dango::
detail::
timeout_impl::
tick_count
()const noexcept->value_type
{
  return dango::get_tick_count();
}

/*** timeout_impl_hr ***/

namespace
dango::detail
{
  struct timeout_impl_hr;
}

struct
dango::
detail::
timeout_impl_hr
final:
dango::detail::timeout_impl
{
private:
  using super_type = dango::detail::timeout_impl;
public:
  static auto make(value_type)noexcept->timeout_impl_hr;
  static auto make_rel(value_type)noexcept->timeout_impl_hr;
private:
  timeout_impl_hr(value_type)noexcept;
public:
  virtual ~timeout_impl_hr()noexcept override;
public:
  DANGO_DELETE_DEFAULT(timeout_impl_hr)
  DANGO_IMMOBILE(timeout_impl_hr)
};

inline auto
dango::
detail::
timeout_impl_hr::
make
(value_type const a_timeout)noexcept->timeout_impl_hr
{
  return timeout_impl_hr{ a_timeout };
}

inline auto
dango::
detail::
timeout_impl_hr::
make_rel
(value_type const a_interval)noexcept->timeout_impl_hr
{
  auto const a_now = dango::get_tick_count();

  return timeout_impl_hr{ safe_add(a_now, a_interval) };
}

/*** timeout_impl_sa ***/

namespace
dango::detail
{
  struct timeout_impl_sa;
}

struct
dango::
detail::
timeout_impl_sa:
dango::timeout
{
private:
  using super_type = dango::timeout;
public:
  static auto make(value_type)noexcept->timeout_impl_sa;
  static auto make_rel(value_type)noexcept->timeout_impl_sa;
protected:
  constexpr timeout_impl_sa(value_type)noexcept;
public:
  virtual ~timeout_impl_sa()noexcept override = default;
  virtual auto is_suspend_aware()const noexcept->bool override final;
private:
  virtual auto tick_count()const noexcept->value_type override final;
public:
  DANGO_DELETE_DEFAULT(timeout_impl_sa)
  DANGO_IMMOBILE(timeout_impl_sa)
};

inline auto
dango::
detail::
timeout_impl_sa::
make
(value_type const a_timeout)noexcept->timeout_impl_sa
{
  return timeout_impl_sa{ a_timeout };
}

inline auto
dango::
detail::
timeout_impl_sa::
make_rel
(value_type const a_interval)noexcept->timeout_impl_sa
{
  auto const a_now = dango::get_tick_count_sa();

  return timeout_impl_sa{ safe_add(a_now, a_interval) };
}

constexpr
dango::
detail::
timeout_impl_sa::
timeout_impl_sa
(value_type const a_timeout)noexcept:
super_type{ a_timeout }
{

}

inline auto
dango::
detail::
timeout_impl_sa::
is_suspend_aware
()const noexcept->bool
{
  return true;
}

inline auto
dango::
detail::
timeout_impl_sa::
tick_count
()const noexcept->value_type
{
  return dango::get_tick_count_sa();
}

/*** timeout_impl_hr_sa ***/

namespace
dango::detail
{
  struct timeout_impl_hr_sa;
}

struct
dango::
detail::
timeout_impl_hr_sa
final:
dango::detail::timeout_impl_sa
{
private:
  using super_type = dango::detail::timeout_impl_sa;
public:
  static auto make(value_type)noexcept->timeout_impl_hr_sa;
  static auto make_rel(value_type)noexcept->timeout_impl_hr_sa;
private:
  timeout_impl_hr_sa(value_type)noexcept;
public:
  virtual ~timeout_impl_hr_sa()noexcept override;
public:
  DANGO_DELETE_DEFAULT(timeout_impl_hr_sa)
  DANGO_IMMOBILE(timeout_impl_hr_sa)
};

inline auto
dango::
detail::
timeout_impl_hr_sa::
make
(value_type const a_timeout)noexcept->timeout_impl_hr_sa
{
  return timeout_impl_hr_sa{ a_timeout };
}

inline auto
dango::
detail::
timeout_impl_hr_sa::
make_rel
(value_type const a_interval)noexcept->timeout_impl_hr_sa
{
  auto const a_now = dango::get_tick_count_sa();

  return timeout_impl_hr_sa{ safe_add(a_now, a_interval) };
}

/*** make_timeout ***/

namespace
dango
{
  auto make_timeout(dango::uint64)noexcept->detail::timeout_impl;
  auto make_timeout_hr(dango::uint64)noexcept->detail::timeout_impl_hr;
  auto make_timeout_sa(dango::uint64)noexcept->detail::timeout_impl_sa;
  auto make_timeout_hr_sa(dango::uint64)noexcept->detail::timeout_impl_hr_sa;

  auto make_timeout_rel(dango::uint64)noexcept->detail::timeout_impl;
  auto make_timeout_rel_hr(dango::uint64)noexcept->detail::timeout_impl_hr;
  auto make_timeout_rel_sa(dango::uint64)noexcept->detail::timeout_impl_sa;
  auto make_timeout_rel_hr_sa(dango::uint64)noexcept->detail::timeout_impl_hr_sa;
}

inline auto
dango::
make_timeout
(dango::uint64 const a_timeout)noexcept->detail::timeout_impl
{
  return detail::timeout_impl::make(a_timeout);
}

inline auto
dango::
make_timeout_hr
(dango::uint64 const a_timeout)noexcept->detail::timeout_impl_hr
{
  return detail::timeout_impl_hr::make(a_timeout);
}

inline auto
dango::
make_timeout_sa
(dango::uint64 const a_timeout)noexcept->detail::timeout_impl_sa
{
  return detail::timeout_impl_sa::make(a_timeout);
}

inline auto
dango::
make_timeout_hr_sa
(dango::uint64 const a_timeout)noexcept->detail::timeout_impl_hr_sa
{
  return detail::timeout_impl_hr_sa::make(a_timeout);
}

inline auto
dango::
make_timeout_rel
(dango::uint64 const a_interval)noexcept->detail::timeout_impl
{
  return detail::timeout_impl::make_rel(a_interval);
}

inline auto
dango::
make_timeout_rel_hr
(dango::uint64 const a_interval)noexcept->detail::timeout_impl_hr
{
  return detail::timeout_impl_hr::make_rel(a_interval);
}

inline auto
dango::
make_timeout_rel_sa
(dango::uint64 const a_interval)noexcept->detail::timeout_impl_sa
{
  return detail::timeout_impl_sa::make_rel(a_interval);
}

inline auto
dango::
make_timeout_rel_hr_sa
(dango::uint64 const a_interval)noexcept->detail::timeout_impl_hr_sa
{
  return detail::timeout_impl_hr_sa::make_rel(a_interval);
}

/*** mutex_base ***/

namespace
dango::detail
{
  using primitive_storage =
    dango::aligned_storage<dango::usize(24), alignof(void*)>;

  class mutex_base;
  class cond_var_base;
}

class alignas(dango::cache_align_type)
dango::
detail::
mutex_base
{
  friend dango::detail::cond_var_base;
private:
  class mutex_impl;
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
  auto get()noexcept->mutex_impl*;
  void init()noexcept;
  void initialize()noexcept;
  auto acquire()noexcept->mutex_base*;
  auto try_acquire()noexcept->mutex_base*;
  void release()noexcept;
private:
  dango::exec_once m_init;
  detail::primitive_storage m_storage;
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
m_init{ },
m_storage{ }
{

}

inline void
dango::
detail::
mutex_base::
init
()noexcept
{
  m_init.exec([this]()noexcept->void{ initialize(); });
}

inline auto
dango::
detail::
mutex_base::
lock
()noexcept->locker
{
  init();

  return locker{ this };
}

inline auto
dango::
detail::
mutex_base::
try_lock
()noexcept->try_locker
{
  init();

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
  class cond_var_elem;
  class cond_var_base;
  class cond_var_registry;
}

class
dango::
detail::
cond_var_elem
{
  friend dango::detail::cond_var_registry;
private:
  using value_type = dango::detail::cond_var_elem*;
protected:
  constexpr cond_var_elem()noexcept:
  m_prev{ nullptr },
  m_next{ nullptr }
  { }
  ~cond_var_elem()noexcept = default;
private:
  value_type m_prev;
  value_type m_next;
public:
  DANGO_IMMOBILE(cond_var_elem)
};

class alignas(dango::cache_align_type)
dango::
detail::
cond_var_base:
dango::detail::cond_var_elem
{
  friend detail::cond_var_registry;
private:
  using super_type = dango::detail::cond_var_elem;
  class cond_var_impl;
  class locker;
  class try_locker;
protected:
  using mutex_type = dango::detail::mutex_base;
protected:
  constexpr cond_var_base()noexcept;
  ~cond_var_base()noexcept = default;
  auto lock(mutex_type*)noexcept->locker;
  auto try_lock(mutex_type*)noexcept->try_locker;
  void destroy()noexcept;
public:
  void notify()noexcept;
  void notify_all()noexcept;
private:
  auto get()noexcept->cond_var_impl*;
  void init()noexcept;
  void initialize()noexcept;
  void wait(mutex_type*)noexcept;
  void wait(mutex_type*, dango::timeout const&)noexcept;
private:
  dango::usize m_ref_count;
  dango::exec_once m_init;
  detail::primitive_storage m_storage;
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
  { }
  ~locker()noexcept{ m_lock->release(); }
  void wait()const noexcept{ m_cond->wait(m_lock); }
  void wait(dango::timeout const& a_timeout)const noexcept{ m_cond->wait(m_lock, a_timeout); }
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
  { }
  ~try_locker()noexcept{ if(m_lock){ m_lock->release(); } }
  explicit operator bool()const{ return m_lock != nullptr; }
  void wait()const noexcept{ m_cond->wait(m_lock); }
  void wait(dango::timeout const& a_timeout)const noexcept{ m_cond->wait(m_lock, a_timeout); }
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
super_type{ },
m_ref_count{ dango::usize(0) },
m_init{ },
m_storage{ }
{

}

inline void
dango::
detail::
cond_var_base::
init
()noexcept
{
  m_init.exec([this]()noexcept->void{ initialize(); });
}

inline auto
dango::
detail::
cond_var_base::
lock
(mutex_type* const a_lock)noexcept->locker
{
  a_lock->init();

  init();

  return locker{ a_lock, this };
}

inline auto
dango::
detail::
cond_var_base::
try_lock
(mutex_type* const a_lock)noexcept->try_locker
{
  a_lock->init();

  init();

  return try_locker{ a_lock, this };
}

/*** cond_var ***/

namespace
dango
{
  class cond_var;
}

struct
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
  [[nodiscard]] auto lock(mutex_type&)noexcept->auto;
  [[nodiscard]] auto try_lock(mutex_type&)noexcept->auto;
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
(mutex_type& a_lock)noexcept->auto
{
  return super_type::lock(&a_lock);
}

inline auto
dango::
cond_var::
try_lock
(mutex_type& a_lock)noexcept->auto
{
  return super_type::try_lock(&a_lock);
}

/*** static_cond_var ***/

namespace
dango
{
  class static_cond_var;
}

struct
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
  [[nodiscard]] auto lock(mutex_type&)noexcept->auto;
  [[nodiscard]] auto try_lock(mutex_type&)noexcept->auto;
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
(mutex_type& a_lock)noexcept->auto
{
  return super_type::lock(&a_lock);
}

inline auto
dango::
static_cond_var::
try_lock
(mutex_type& a_lock)noexcept->auto
{
  return super_type::try_lock(&a_lock);
}

/*** cond_var_mutex ***/

namespace
dango
{
  class cond_var_mutex;
}

struct
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
  [[nodiscard]] auto lock()noexcept->auto;
  [[nodiscard]] auto try_lock()noexcept->auto;
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
()noexcept->auto
{
  return super_type::lock(m_lock);
}

inline auto
dango::
cond_var_mutex::
try_lock
()noexcept->auto
{
  return super_type::try_lock(m_lock);
}

/*** static_cond_var_mutex ***/

namespace
dango
{
  class static_cond_var_mutex;
}

struct
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
  [[nodiscard]] auto lock()noexcept->auto;
  [[nodiscard]] auto try_lock()noexcept->auto;
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
()noexcept->auto
{
  return super_type::lock(m_lock);
}

inline auto
dango::
static_cond_var_mutex::
try_lock
()noexcept->auto
{
  return super_type::try_lock(m_lock);
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
  struct construct_tag;
private:
  static auto new_control_block()noexcept->control_block*;

  static void
  start_thread
  (thread_start_func, void*)noexcept(false);

  template
  <typename tp_func>
  static void thread_start_address(void*)noexcept(false);

  static thread const& s_main_init;
  static dango::uint64 const s_tick_count_init;
  static dango::uint64 const s_tick_count_sa_init;
public:
  static void yield()noexcept;
  static auto self()noexcept->thread const&;

  template
  <typename tp_func>
  static auto
  create(tp_func&&)noexcept(false)->
  dango::enable_if
  <
    dango::is_callable_ret<dango::decay<tp_func>&, void> &&
    dango::is_noexcept_destructible<dango::decay<tp_func>>,
    thread
  >;

  static void sleep(dango::uint64)noexcept;
  static void sleep_hr(dango::uint64)noexcept;
  static void sleep_sa(dango::uint64)noexcept;
  static void sleep_hr_sa(dango::uint64)noexcept;
private:
  thread(construct_tag)noexcept;
public:
  constexpr thread()noexcept;
  constexpr thread(dango::null_tag)noexcept;
  thread(thread const&)noexcept;
  thread(thread&&)noexcept;
  ~thread()noexcept;
  auto operator = (dango::null_tag)&noexcept->thread&;
  auto operator = (thread const&)&noexcept->thread&;
  auto operator = (thread&&)&noexcept->thread&;
  explicit constexpr operator bool()const noexcept;
  auto is_alive()const noexcept->bool;
  void join()const noexcept;
  void join(dango::timeout const&)const noexcept;
  constexpr auto dango_operator_is_null()const noexcept->bool;
  constexpr auto dango_operator_equals(thread const&)const noexcept->bool;
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
  void wait(dango::timeout const&)noexcept;
  void notify_all()noexcept;
  auto is_alive()const noexcept->bool;
private:
  dango::atomic<dango::usize> m_ref_count;
  mutable dango::mutex m_mutex;
  mutable dango::cond_var_mutex m_cond;
  bool m_alive;
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

inline dango::uint64 const
dango::
thread::
s_tick_count_init = dango::get_tick_count();

inline dango::uint64 const
dango::
thread::
s_tick_count_sa_init = dango::get_tick_count_sa();

constexpr
dango::
thread::
thread
()noexcept:
thread{ dango::null }
{

}

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

constexpr auto
dango::
thread::
dango_operator_is_null
()const noexcept->bool
{
  return m_control == nullptr;
}

constexpr auto
dango::
thread::
dango_operator_equals
(thread const& a_thread)const noexcept->bool
{
  return m_control == a_thread.m_control;
}

template
<typename tp_func>
auto
dango::
thread::
create
(tp_func&& a_thread_func)noexcept(false)->
dango::enable_if
<
  dango::is_callable_ret<dango::decay<tp_func>&, void> &&
  dango::is_noexcept_destructible<dango::decay<tp_func>>,
  thread
>
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

  auto a_count = detail::c_spin_count_init;

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
  static_assert(dango::is_callable_ret<tp_func const&, void>);
  static_assert(dango::is_noexcept_destructible<tp_func>);

  auto const a_func_ptr = static_cast<tp_func*>(a_data);

  auto const a_func = dango::move(*a_func_ptr);

  a_func();
}

/*** cond_var_registry ***/

namespace
dango::detail
{
  class cond_var_registry;
  class cond_var_sentinel;
  class cond_var_registry_thread;
  class cond_var_registry_access;
}

class
dango::
detail::
cond_var_sentinel
final:
public dango::detail::cond_var_elem
{
private:
  using super_type = dango::detail::cond_var_elem;
public:
  constexpr cond_var_sentinel()noexcept:super_type{ }{ }
  ~cond_var_sentinel()noexcept = default;
public:
  DANGO_IMMOBILE(cond_var_sentinel)
};

class
dango::
detail::
cond_var_registry
final
{
  friend dango::detail::cond_var_registry_access;
private:
  using cond_type = dango::detail::cond_var_base;
private:
  static void remove(cond_type*)noexcept;
  static auto list_empty(detail::cond_var_sentinel const*, detail::cond_var_elem const*)noexcept->bool;
  static auto current_bias()noexcept->dango::uint64;
  static auto bias_okay(dango::uint64&)noexcept->bool;
public:
  void regist(cond_type*, dango::timeout const&)noexcept;
  void unregist(cond_type*, dango::timeout const&)noexcept;
  void notify_exit()noexcept;
  void thread_func()noexcept;
private:
  constexpr cond_var_registry()noexcept;
  ~cond_var_registry()noexcept = default;
  void add(cond_type*)noexcept;
  auto wait_empty()noexcept->bool;
  auto poll_bias(dango::uint64&, dango::timeout&)noexcept->bool;
  auto pop_internal()noexcept->bool;
private:
  dango::static_mutex m_mutex;
  dango::static_cond_var_mutex m_cond;
  detail::cond_var_sentinel m_head_sentinel[dango::usize(2)];
  detail::cond_var_sentinel m_tail_sentinel[dango::usize(2)];
  detail::cond_var_sentinel* m_external_head;
  detail::cond_var_sentinel* m_internal_head;
  detail::cond_var_elem* m_external_tail;
  detail::cond_var_elem* m_internal_tail;
  bool m_alive;
  bool m_waiting;
public:
  DANGO_IMMOBILE(cond_var_registry)
};

constexpr
dango::
detail::
cond_var_registry::
cond_var_registry
()noexcept:
m_mutex{ },
m_cond{ m_mutex },
m_head_sentinel{ { }, { } },
m_tail_sentinel{ { }, { } },
m_external_head{ &m_head_sentinel[0] },
m_internal_head{ &m_head_sentinel[1] },
m_external_tail{ &m_tail_sentinel[0] },
m_internal_tail{ &m_tail_sentinel[1] },
m_alive{ true },
m_waiting{ false }
{
  m_head_sentinel[0].m_next = &m_tail_sentinel[0];
  m_tail_sentinel[0].m_prev = &m_head_sentinel[0];
  m_head_sentinel[1].m_next = &m_tail_sentinel[1];
  m_tail_sentinel[1].m_prev = &m_head_sentinel[1];
}

class
dango::
detail::
cond_var_registry_access
final
{
  friend dango::detail::cond_var_base;
  friend dango::detail::cond_var_registry_thread;
private:
  static dango::detail::cond_var_registry s_registry;
public:
  DANGO_UNINSTANTIABLE(cond_var_registry_access)
};

inline dango::detail::cond_var_registry
dango::
detail::
cond_var_registry_access::
s_registry{ };

class
dango::
detail::
cond_var_registry_thread
final
{
private:
  static auto start_thread()noexcept->dango::thread;
public:
  cond_var_registry_thread()noexcept;
  ~cond_var_registry_thread()noexcept;
private:
  dango::thread const m_thread;
public:
  DANGO_IMMOBILE(cond_var_registry_thread)
};

namespace
dango::detail
{
  DANGO_DEFINE_GLOBAL_INLINE_CV(s_cond_var_registry_thread, const, detail::cond_var_registry_thread{ })
}

#ifdef _WIN32

/*** windows_timer_agent ***/

namespace
dango::detail
{
  class windows_timer_agent;
  class windows_timer_agent_thread;
  class windows_timer_agent_access;
}

class
dango::
detail::
windows_timer_agent
final
{
  friend dango::detail::windows_timer_agent_access;
public:
  void increment()noexcept;
  void decrement()noexcept;
  void notify_exit()noexcept;
  void thread_func()noexcept;
private:
  constexpr windows_timer_agent()noexcept;
  ~windows_timer_agent()noexcept = default;
  auto wait()noexcept->bool;
  auto timed_wait(dango::timeout const&)noexcept->bool;
private:
  dango::static_mutex m_mutex;
  dango::static_cond_var_mutex m_cond;
  bool m_alive;
  bool m_waiting;
  bool m_active;
  dango::uint32 m_min_period;
  dango::usize m_req_count;
public:
  DANGO_IMMOBILE(windows_timer_agent)
};

constexpr
dango::
detail::
windows_timer_agent::
windows_timer_agent
()noexcept:
m_mutex{ },
m_cond{ m_mutex },
m_alive{ true },
m_waiting{ false },
m_active{ false },
m_min_period{ dango::uint32(0) },
m_req_count{ dango::usize(0) }
{

}

class
dango::
detail::
windows_timer_agent_access
final
{
  friend dango::detail::windows_timer_agent_thread;
  friend dango::detail::timeout_impl_hr;
  friend dango::detail::timeout_impl_hr_sa;
private:
  static dango::detail::windows_timer_agent s_agent;
public:
  DANGO_UNINSTANTIABLE(windows_timer_agent_access)
};

inline dango::detail::windows_timer_agent
dango::
detail::
windows_timer_agent_access::
s_agent{ };

class
dango::
detail::
windows_timer_agent_thread
final
{
private:
  static auto start_thread()noexcept->dango::thread;
public:
  windows_timer_agent_thread()noexcept;
  ~windows_timer_agent_thread()noexcept;
private:
  dango::thread const m_thread;
public:
  DANGO_IMMOBILE(windows_timer_agent_thread)
};

namespace
dango::detail
{
  DANGO_DEFINE_GLOBAL_INLINE_CV(s_windows_timer_agent_thread, const, detail::windows_timer_agent_thread{ })
}

#endif

#ifdef _WIN32

inline
dango::
detail::
timeout_impl_hr::
timeout_impl_hr
(dango::uint64 const a_timeout)noexcept:
super_type{ a_timeout }
{
  constexpr auto& c_agent = dango::detail::windows_timer_agent_access::s_agent;

  c_agent.increment();
}

inline
dango::
detail::
timeout_impl_hr::
~timeout_impl_hr
()noexcept
{
  constexpr auto& c_agent = dango::detail::windows_timer_agent_access::s_agent;

  c_agent.decrement();
}

inline
dango::
detail::
timeout_impl_hr_sa::
timeout_impl_hr_sa
(dango::uint64 const a_timeout)noexcept:
super_type{ a_timeout }
{
  constexpr auto& c_agent = dango::detail::windows_timer_agent_access::s_agent;

  c_agent.increment();
}

inline
dango::
detail::
timeout_impl_hr_sa::
~timeout_impl_hr_sa
()noexcept
{
  constexpr auto& c_agent = dango::detail::windows_timer_agent_access::s_agent;

  c_agent.decrement();
}

#else

inline
dango::
detail::
timeout_impl_hr::
timeout_impl_hr
(dango::uint64 const a_timeout)noexcept:
super_type{ a_timeout }
{

}

inline
dango::
detail::
timeout_impl_hr::
~timeout_impl_hr
()noexcept = default;

inline
dango::
detail::
timeout_impl_hr_sa::
timeout_impl_hr_sa
(dango::uint64 const a_timeout)noexcept:
super_type{ a_timeout }
{

}

inline
dango::
detail::
timeout_impl_hr_sa::
~timeout_impl_hr_sa
()noexcept = default;

#endif

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
(construct_tag const)noexcept:
m_control{ new_control_block() }
{

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
  thread_local thread const t_thread{ construct_tag{ } };

  thread_local notifier const t_notifier{ t_thread };

  return t_thread;
}

void
dango::
thread::
sleep
(dango::uint64 const a_interval)noexcept
{
  static dango::static_mutex s_mutex{ };
  static dango::static_cond_var_mutex s_cond{ s_mutex };

  if(a_interval == dango::uint64(0))
  {
    return;
  }

  auto const a_timeout = dango::make_timeout_rel(a_interval);

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
sleep_hr
(dango::uint64 const a_interval)noexcept
{
  static dango::static_mutex s_mutex{ };
  static dango::static_cond_var_mutex s_cond{ s_mutex };

  if(a_interval == dango::uint64(0))
  {
    return;
  }

  auto const a_timeout = dango::make_timeout_rel_hr(a_interval);

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
sleep_sa
(dango::uint64 const a_interval)noexcept
{
  static dango::static_mutex s_mutex{ };
  static dango::static_cond_var_mutex s_cond{ s_mutex };

  if(a_interval == dango::uint64(0))
  {
    return;
  }

  auto const a_timeout = dango::make_timeout_rel_sa(a_interval);

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
sleep_hr_sa
(dango::uint64 const a_interval)noexcept
{
  static dango::static_mutex s_mutex{ };
  static dango::static_cond_var_mutex s_cond{ s_mutex };

  if(a_interval == dango::uint64(0))
  {
    return;
  }

  auto const a_timeout = dango::make_timeout_rel_hr_sa(a_interval);

  dango_crit_full(s_cond, a_crit)
  {
    while(!a_timeout.has_expired())
    {
      a_crit.wait(a_timeout);
    }
  }
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

    auto a_bias = current_bias();

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
  dango_assert(a_cond != nullptr);
  dango_assert(a_cond->m_prev == nullptr);
  dango_assert(a_cond->m_next == nullptr);

  auto const a_prev = m_external_tail->m_prev;
  auto const a_next = m_external_tail;

  dango_assert(a_prev->m_next == a_next);
  dango_assert(a_next->m_prev == a_prev);

  a_cond->m_prev = a_prev;
  a_cond->m_next = a_next;
  a_next->m_prev = a_cond;
  a_prev->m_next = a_cond;
}

void
dango::
detail::
cond_var_registry::
remove
(cond_type* const a_cond)noexcept
{
  dango_assert(a_cond != nullptr);
  dango_assert(a_cond->m_prev != nullptr);
  dango_assert(a_cond->m_next != nullptr);

  auto const a_prev = a_cond->m_prev;
  auto const a_next = a_cond->m_next;

  dango_assert(a_prev->m_next == a_cond);
  dango_assert(a_next->m_prev == a_cond);

  a_next->m_prev = a_prev;
  a_prev->m_next = a_next;
  a_cond->m_prev = nullptr;
  a_cond->m_next = nullptr;
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
      if(!list_empty(m_external_head, m_external_tail))
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
      if(list_empty(m_external_head, m_external_tail))
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

    dango::swap(m_external_head, m_internal_head);
    dango::swap(m_external_tail, m_internal_tail);

    dango_assert(list_empty(m_external_head, m_external_tail));
  }

  while(pop_internal());

  return a_alive;
}

auto
dango::
detail::
cond_var_registry::
list_empty
(detail::cond_var_sentinel const* const a_head, detail::cond_var_elem const* const a_tail)noexcept->bool
{
  return a_head->m_next == a_tail;
}

auto
dango::
detail::
cond_var_registry::
current_bias
()noexcept->dango::uint64
{
  auto const a_current = dango::get_tick_count();

  auto const a_current_sa = dango::get_tick_count_sa();

  if(a_current >= a_current_sa)
  {
    return dango::uint64(0);
  }

  return a_current_sa - a_current;
}

auto
dango::
detail::
cond_var_registry::
bias_okay
(dango::uint64& a_prev_bias)noexcept->bool
{
  auto const a_bias = current_bias();

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
    if(list_empty(m_internal_head, m_internal_tail))
    {
      return false;
    }

    auto const a_cond = static_cast<detail::cond_var_base*>(m_internal_head->m_next);

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
    return dango::thread::create([]()noexcept->void{ c_registry.thread_func(); });
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

#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <pthread.h>

/*** get_tick_count ***/

namespace
dango::concurrent_cpp
{
  static auto tick_count(clockid_t)noexcept->dango::uint64;
}

auto
dango::
get_tick_count
()noexcept->dango::uint64
{
  constexpr auto const c_clock = clockid_t(CLOCK_MONOTONIC);

  static auto const s_init = concurrent_cpp::tick_count(c_clock);

  auto const a_count = concurrent_cpp::tick_count(c_clock);

  return a_count - s_init;
}

auto
dango::
get_tick_count_sa
()noexcept->dango::uint64
{
  constexpr auto const c_clock = clockid_t(CLOCK_BOOTTIME);

  static auto const s_init = concurrent_cpp::tick_count(c_clock);

  auto const a_count = concurrent_cpp::tick_count(c_clock);

  return a_count - s_init;
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
  return dango::launder(static_cast<mutex_impl*>(m_storage.get()));
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
  return dango::launder(static_cast<cond_var_impl*>(m_storage.get()));
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
tick_count
(clockid_t const a_clock)noexcept->dango::uint64
{
  using u64 = dango::uint64;

  constexpr auto const c_mul = u64(1'000);
  constexpr auto const c_div = u64(1'000'000);
  constexpr auto const c_half = c_div / u64(2);

  timespec a_spec;

  auto const a_result =
    clock_gettime(a_clock, &a_spec);

  dango_assert(a_result == 0);

  auto const a_sec = u64(a_spec.tv_sec);
  auto const a_nsec = u64(a_spec.tv_nsec);

  return (a_sec * c_mul) + (a_nsec / c_div) + u64((a_nsec % c_div) >= c_half);
}

template
<typename tp_func>
static auto
dango::
concurrent_cpp::
thread_start_address
(void* const a_data)noexcept->void*
{
  static_assert(!dango::is_const<tp_func> && !dango::is_volatile<tp_func>);
  static_assert(dango::is_noexcept_move_constructible<tp_func>);
  static_assert(dango::is_callable_ret<tp_func const&, void>);
  static_assert(dango::is_noexcept_destructible<tp_func>);

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

#define DANGO_WINDOWS_VER 0x0601

#ifdef WINVER
#if (WINVER < DANGO_WINDOWS_VER)
#undef WINVER
#define WINVER DANGO_WINDOWS_VER
#endif
#else
#define WINVER DANGO_WINDOWS_VER
#endif

#ifdef _WIN32_WINNT
#if (_WIN32_WINNT < DANGO_WINDOWS_VER)
#undef _WIN32_WINNT
#define _WIN32_WINNT DANGO_WINDOWS_VER
#endif
#else
#define _WIN32_WINNT DANGO_WINDOWS_VER
#endif

#include <windows.h>

/*** get_tick_count ***/

namespace
dango::concurrent_cpp
{
  static auto perf_freq()noexcept->dango::uint64;
  static auto perf_count()noexcept->dango::uint64;
  static auto perf_count(dango::uint64&)noexcept->dango::uint64;
}

auto
dango::
get_tick_count
()noexcept->dango::uint64
{
  using u64 = dango::uint64;

  static dango::spin_mutex s_lock{ };
  static auto s_init_bias_mutable = u64(0);
  static auto s_prev = concurrent_cpp::perf_count(s_init_bias_mutable);
  static auto s_current = u64(0);

  constexpr auto const& c_init_bias = s_init_bias_mutable;

  u64 a_result;
  u64 a_bias;

  auto const a_freq = concurrent_cpp::perf_freq();
  auto const a_half = a_freq / u64(2);

  dango_crit(s_lock)
  {
    auto const a_count = concurrent_cpp::perf_count(a_bias);
    auto const a_delta = a_count - s_prev;
    auto const a_mul = a_delta * u64(1'000);

    s_prev = a_count;

    s_current += (a_mul / a_freq) + u64((a_mul % a_freq) >= a_half);

    a_result = s_current;
  }

  a_result -= (a_bias - c_init_bias);

  return a_result;
}

auto
dango::
get_tick_count_sa
()noexcept->dango::uint64
{
  using u64 = dango::uint64;

  static dango::spin_mutex s_lock{ };
  static auto s_prev = concurrent_cpp::perf_count();
  static auto s_current = u64(0);

  u64 a_result;

  auto const a_freq = concurrent_cpp::perf_freq();
  auto const a_half = a_freq / u64(2);

  dango_crit(s_lock)
  {
    auto const a_count = concurrent_cpp::perf_count();
    auto const a_delta = a_count - s_prev;
    auto const a_mul = a_delta * u64(1'000);

    s_prev = a_count;

    s_current += (a_mul / a_freq) + u64((a_mul % a_freq) >= a_half);

    a_result = s_current;
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
  return dango::launder(static_cast<mutex_impl*>(m_storage.get()));
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
  return dango::launder(static_cast<cond_var_impl*>(m_storage.get()));
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

/*** windows_timer_agent ***/

void
dango::
detail::
windows_timer_agent::
increment
()noexcept
{
  dango_crit(m_cond)
  {
    if(m_req_count++ != dango::usize(0))
    {
      return;
    }

    if(m_min_period == dango::uint32(0))
    {
      TIMECAPS a_caps;

      timeGetDevCaps(&a_caps, sizeof(a_caps));

      auto const a_min = dango::uint32(a_caps.wPeriodMin);

      m_min_period = dango::max(dango::uint32(1), a_min);
    }

    if(!m_active)
    {
      m_active = true;

      timeBeginPeriod(m_min_period);
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
windows_timer_agent::
decrement
()noexcept
{
  dango_crit(m_cond)
  {
    if(--m_req_count != dango::usize(0))
    {
      return;
    }

    if(!m_alive && m_active)
    {
      m_active = false;

      timeEndPeriod(m_min_period);
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
windows_timer_agent::
notify_exit
()noexcept
{
  dango_crit(m_cond)
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
windows_timer_agent::
thread_func
()noexcept
{
  do
  {
    if(wait())
    {
      break;
    }

    auto const a_timeout = dango::make_timeout_rel(dango::uint64(60'000));

    if(timed_wait(a_timeout))
    {
      break;
    }
  }
  while(true);

  dango_crit(m_mutex)
  {
    if(m_active && m_req_count == dango::usize(0))
    {
      m_active = false;

      timeEndPeriod(m_min_period);
    }
  }
}

auto
dango::
detail::
windows_timer_agent::
wait
()noexcept->bool
{
  dango_crit_full(m_cond, a_crit)
  {
    do
    {
      dango_assert(dango::logical_implication(m_req_count != dango::uint32(0), m_active));

      if(!m_alive)
      {
        return true;
      }

      if(m_active && m_req_count == dango::usize(0))
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
windows_timer_agent::
timed_wait
(dango::timeout const& a_timeout)noexcept->bool
{
  dango_crit_full(m_cond, a_crit)
  {
    do
    {
      dango_assert(dango::logical_implication(m_req_count != dango::uint32(0), m_active));

      if(!m_alive)
      {
        return true;
      }

      if(m_req_count != dango::usize(0))
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

    dango_assert(m_active);

    m_active = false;

    timeEndPeriod(m_min_period);
  }

  return false;
}

/*** windows_timer_agent_thread ***/

auto
dango::
detail::
windows_timer_agent_thread::
start_thread
()noexcept->dango::thread
{
  constexpr auto& c_agent = detail::windows_timer_agent_access::s_agent;

  try
  {
    return dango::thread::create([]()noexcept->void{ c_agent.thread_func(); });
  }
  catch(...)
  {
#ifndef DANGO_NO_DEBUG
    dango_unreachable_msg("windows timer agent thread creation failed");
#else
    dango::terminate();
#endif
  }
}

dango::
detail::
windows_timer_agent_thread::
windows_timer_agent_thread
()noexcept:
m_thread{ start_thread() }
{

}

dango::
detail::
windows_timer_agent_thread::
~windows_timer_agent_thread
()noexcept
{
  constexpr auto& c_agent = detail::windows_timer_agent_access::s_agent;

  c_agent.notify_exit();

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
  static constexpr auto const c_func =
  []()noexcept->dango::uint64
  {
    LARGE_INTEGER a_result;

    QueryPerformanceFrequency(&a_result);

    return dango::uint64(a_result.QuadPart);
  };

  static auto const s_result = c_func();

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
perf_count
(dango::uint64& a_suspend_bias)noexcept->dango::uint64
{
  using u64 = dango::uint64;

  static auto& s_bias_ref = *reinterpret_cast<ULONGLONG const volatile*>(dango::uintptr(0x7FFE0000 + 0x3B0));

  constexpr auto const c_div = u64(10'000);
  constexpr auto const c_half = c_div / u64(2);

  u64 a_bias;
  u64 a_result;

  do
  {
    a_bias = s_bias_ref;
    a_result = concurrent_cpp::perf_count();
  }
  while(a_bias != s_bias_ref);

  a_suspend_bias = (a_bias / c_div) + u64((a_bias % c_div) >= c_half);

  return a_result;
}

template
<typename tp_func>
static auto WINAPI
dango::
concurrent_cpp::
thread_start_address
(LPVOID const a_data)noexcept->DWORD
{
  static_assert(!dango::is_const<tp_func> && !dango::is_volatile<tp_func>);
  static_assert(dango::is_noexcept_move_constructible<tp_func>);
  static_assert(dango::is_callable_ret<tp_func const&, void>);
  static_assert(dango::is_noexcept_destructible<tp_func>);

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

  return DWORD(0);
}

#endif /* _WIN32 */

#endif /* DANGO_SOURCE_FILE */

#endif

