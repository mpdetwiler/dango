#ifndef __DANGO_CONCURRENT_HPP__
#define __DANGO_CONCURRENT_HPP__ 1

#include "dango-concurrent-base.hpp"
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

/*** deadline ***/

namespace
dango
{
  class deadline;
}

class
dango::
deadline
{
protected:
  using value_type = dango::uint64;
protected:
  static constexpr auto safe_add(value_type, value_type)noexcept->value_type;
protected:
  constexpr deadline(value_type)noexcept;
public:
  virtual ~deadline()noexcept = default;
  auto has_passed()const noexcept->bool;
  auto remaining()const noexcept->value_type;
  auto get()const noexcept->value_type;
  void set(value_type)noexcept;
  void set_rel(value_type)noexcept;
  void add(value_type)noexcept;
  virtual auto is_suspend_aware()const noexcept->bool = 0;
private:
  virtual auto tick_count()const noexcept->value_type = 0;
private:
  value_type m_deadline;
public:
  DANGO_DELETE_DEFAULT(deadline)
  DANGO_IMMOBILE(deadline)
};

constexpr auto
dango::
deadline::
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
deadline::
deadline
(value_type const a_deadline)noexcept:
m_deadline{ a_deadline }
{

}

inline auto
dango::
deadline::
has_passed
()const noexcept->bool
{
  return tick_count() >= m_deadline;
}

inline auto
dango::
deadline::
remaining
()const noexcept->value_type
{
  auto const a_now = tick_count();

  if(a_now >= m_deadline)
  {
    return value_type(0);
  }

  return m_deadline - a_now;
}

inline auto
dango::
deadline::
get
()const noexcept->value_type
{
  return m_deadline;
}

inline void
dango::
deadline::
set
(value_type const a_deadline)noexcept
{
  m_deadline = a_deadline;
}

inline void
dango::
deadline::
set_rel
(value_type const a_interval)noexcept
{
  auto const a_now = tick_count();

  m_deadline = safe_add(a_now, a_interval);
}

inline void
dango::
deadline::
add
(value_type const a_val)noexcept
{
  m_deadline = safe_add(m_deadline, a_val);
}

namespace
dango::detail
{
  struct deadline_unbiased;
}

struct
dango::
detail::
deadline_unbiased
final:
dango::deadline
{
private:
  using super_type = dango::deadline;
public:
  static auto make(value_type)noexcept->deadline_unbiased;
  static auto make_rel(value_type)noexcept->deadline_unbiased;
private:
  constexpr deadline_unbiased(value_type)noexcept;
public:
  virtual ~deadline_unbiased()noexcept override = default;
  virtual auto is_suspend_aware()const noexcept->bool override;
private:
  virtual auto tick_count()const noexcept->value_type override;
public:
  DANGO_DELETE_DEFAULT(deadline_unbiased)
  DANGO_IMMOBILE(deadline_unbiased)
};

inline auto
dango::
detail::
deadline_unbiased::
make
(value_type const a_deadline)noexcept->deadline_unbiased
{
  return deadline_unbiased{ a_deadline };
}

inline auto
dango::
detail::
deadline_unbiased::
make_rel
(value_type const a_interval)noexcept->deadline_unbiased
{
  auto const a_now = dango::get_tick_count();

  return deadline_unbiased{ safe_add(a_now, a_interval) };
}

constexpr
dango::
detail::
deadline_unbiased::
deadline_unbiased
(value_type const a_deadline)noexcept:
super_type{ a_deadline }
{

}

inline auto
dango::
detail::
deadline_unbiased::
is_suspend_aware
()const noexcept->bool
{
  return false;
}

inline auto
dango::
detail::
deadline_unbiased::
tick_count
()const noexcept->value_type
{
  return dango::get_tick_count();
}

namespace
dango::detail
{
  struct deadline_biased;
}

struct
dango::
detail::
deadline_biased
final:
dango::deadline
{
private:
  using super_type = dango::deadline;
public:
  static auto make(value_type)noexcept->deadline_biased;
  static auto make_rel(value_type)noexcept->deadline_biased;
private:
  constexpr deadline_biased(value_type)noexcept;
public:
  virtual ~deadline_biased()noexcept override = default;
  virtual auto is_suspend_aware()const noexcept->bool override;
private:
  virtual auto tick_count()const noexcept->value_type override;
public:
  DANGO_DELETE_DEFAULT(deadline_biased)
  DANGO_IMMOBILE(deadline_biased)
};

inline auto
dango::
detail::
deadline_biased::
make
(value_type const a_deadline)noexcept->deadline_biased
{
  return deadline_biased{ a_deadline };
}

inline auto
dango::
detail::
deadline_biased::
make_rel
(value_type const a_interval)noexcept->deadline_biased
{
  auto const a_now = dango::get_tick_count_sa();

  return deadline_biased{ safe_add(a_now, a_interval) };
}

constexpr
dango::
detail::
deadline_biased::
deadline_biased
(value_type const a_deadline)noexcept:
super_type{ a_deadline }
{

}

inline auto
dango::
detail::
deadline_biased::
is_suspend_aware
()const noexcept->bool
{
  return true;
}

inline auto
dango::
detail::
deadline_biased::
tick_count
()const noexcept->value_type
{
  return dango::get_tick_count_sa();
}

/*** make_deadline ***/

namespace
dango
{
  auto make_deadline(dango::uint64)noexcept->detail::deadline_unbiased;
  auto make_deadline_sa(dango::uint64)noexcept->detail::deadline_biased;
  auto make_deadline_rel(dango::uint64)noexcept->detail::deadline_unbiased;
  auto make_deadline_rel_sa(dango::uint64)noexcept->detail::deadline_biased;
}

inline auto
dango::
make_deadline
(dango::uint64 const a_deadline)noexcept->detail::deadline_unbiased
{
  return detail::deadline_unbiased::make(a_deadline);
}

inline auto
dango::
make_deadline_sa
(dango::uint64 const a_deadline)noexcept->detail::deadline_biased
{
  return detail::deadline_biased::make(a_deadline);
}

inline auto
dango::
make_deadline_rel
(dango::uint64 const a_interval)noexcept->detail::deadline_unbiased
{
  return detail::deadline_unbiased::make_rel(a_interval);
}

inline auto
dango::
make_deadline_rel_sa
(dango::uint64 const a_interval)noexcept->detail::deadline_biased
{
  return detail::deadline_biased::make_rel(a_interval);
}

/*** mutex_base ***/

namespace
dango::detail
{
  using primitive_storage =
    dango::aligned_storage<dango::usize(16), alignof(void*)>;

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
  template
  <typename tp_type>
  auto get()noexcept->tp_type*;
  void init()noexcept;
  void initialize()noexcept;
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
public dango::detail::cond_var_elem
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
private:
  template
  <typename tp_type>
  auto get()noexcept->tp_type*;
  void init()noexcept;
  void initialize()noexcept;
  void wait(mutex_type*)noexcept;
  void wait(mutex_type*, dango::deadline const&)noexcept;
  void notify(mutex_type*)noexcept;
  void notify_all(mutex_type*)noexcept;
  void regist(mutex_type*, dango::deadline const&)noexcept;
  void unregist(mutex_type*, dango::deadline const&)noexcept;
  void process()noexcept;
private:
  mutex_type* m_current_mutex;
  dango::usize m_ref_count;
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
  { }
  ~locker()noexcept{ m_lock->release(); }
  void wait()const noexcept{ m_cond->wait(m_lock); }
  void wait(dango::deadline const& a_deadline)const noexcept{ m_cond->wait(m_lock, a_deadline); }
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
  { }
  ~try_locker()noexcept{ if(m_lock){ m_lock->release(); } }
  explicit operator bool()const{ return m_lock != nullptr; }
  void wait()const noexcept{ m_cond->wait(m_lock); }
  void wait(dango::deadline const& a_deadline)const noexcept{ m_cond->wait(m_lock, a_deadline); }
  void notify()const noexcept{ m_cond->notify(m_lock); }
  void notify_all()const noexcept{ m_cond->notify_all(m_lock); }
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
m_current_mutex{ nullptr },
m_ref_count{ dango::usize(0) },
m_storage{ },
m_init{ }
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
  static void sleep(dango::uint64)noexcept;
  static void sleep_sa(dango::uint64)noexcept;

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
  auto is_alive()const noexcept->bool;
  void join()const noexcept;
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
public:
  [[nodiscard]] auto lock()noexcept->auto;
  void add(cond_type*)noexcept;
  void remove(cond_type*)noexcept;
  void notify_exit()noexcept;
  void thread_func()noexcept;
private:
  constexpr cond_var_registry()noexcept;
  ~cond_var_registry()noexcept = default;
  auto wait_empty()noexcept->bool;
  auto poll(dango::deadline&)noexcept->bool;
  auto check_bias()noexcept->bool;
private:
  dango::static_mutex m_mutex;
  dango::static_cond_var_mutex m_cond;
  detail::cond_var_sentinel m_head_sentinel[dango::usize(2)];
  detail::cond_var_sentinel m_tail_sentinel[dango::usize(2)];
  detail::cond_var_sentinel* m_external_head;
  detail::cond_var_sentinel* m_internal_head;
  detail::cond_var_elem* m_tail;
  bool m_alive;
  dango::uint64 m_last_bias;
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
m_external_head{ &m_head_sentinel[dango::usize(0)] },
m_internal_head{ &m_head_sentinel[dango::usize(1)] },
m_tail{ m_external_head },
m_alive{ true },
m_last_bias{ dango::uint64(0) }
{
  m_head_sentinel[dango::usize(0)].m_next = &m_tail_sentinel[dango::usize(0)];
  m_tail_sentinel[dango::usize(0)].m_prev = &m_head_sentinel[dango::usize(0)];
  m_head_sentinel[dango::usize(1)].m_next = &m_tail_sentinel[dango::usize(1)];
  m_tail_sentinel[dango::usize(1)].m_prev = &m_head_sentinel[dango::usize(1)];
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
notify_all
()noexcept
{
  dango_crit_full(m_cond, a_crit)
  {
    m_alive = false;

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

  auto const a_deadline = dango::make_deadline_rel(a_interval);

  dango_crit_full(s_cond, a_crit)
  {
    while(!a_deadline.has_passed())
    {
      a_crit.wait(a_deadline);
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

  auto const a_deadline = dango::make_deadline_rel_sa(a_interval);

  dango_crit_full(s_cond, a_crit)
  {
    while(!a_deadline.has_passed())
    {
      a_crit.wait(a_deadline);
    }
  }
}

/*** cond_var_registry ***/

auto
dango::
detail::
cond_var_registry::
lock
()noexcept->auto
{
  return m_cond.lock();
}

void
dango::
detail::
cond_var_registry::
add
(cond_type* const a_cond)noexcept
{
  auto const a_prev = m_tail->m_prev;
  auto const a_next = m_tail;

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
  auto const a_prev = a_cond->m_prev;
  auto const a_next = a_cond->m_next;

  a_next->m_prev = a_prev;
  a_prev->m_next = a_next;
  a_cond->m_prev = nullptr;
  a_cond->m_next = nullptr;
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

    a_crit.notify();
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

    auto a_deadline = dango::make_deadline_rel(dango::uint64(100));

    while(poll(a_deadline));
  }
  while(true);
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
    while(m_external_head->m_next == m_tail)
    {
      if(!m_alive)
      {
        return true;
      }

      a_crit.wait();
    }
  }

  return false;
}

auto
dango::
detail::
cond_var_registry::
poll
(dango::deadline& a_deadline)noexcept->bool
{
  bool a_alive;
  detail::cond_var_elem* a_head;
  detail::cond_var_elem* a_tail;

  dango_crit_full(m_cond, a_crit)
  {
    do
    {
      if(m_external_head->m_next == m_tail)
      {
        return false;
      }

      a_alive = m_alive;

      if(a_deadline.has_passed())
      {
        a_deadline.add(dango::uint64(100));

        break;
      }

      if(!a_alive)
      {
        break;
      }

      a_crit.wait(a_deadline);
    }
    while(true);

    if(check_bias())
    {
      return a_alive;
    }

    a_head = m_external_head->m_next;
    a_tail = m_tail;

    dango::swap(m_external_head, m_internal_head);

    m_tail = m_external_head->m_next;
  }

  auto a_current = a_head;

  while(a_current != a_tail)
  {
    auto const a_next = a_current->m_next;
    auto const a_cond = static_cast<detail::cond_var_base*>(a_current);

    a_cond->process();

    a_current = a_next;
  }

  return a_alive;
}

auto
dango::
detail::
cond_var_registry::
check_bias
()noexcept->bool
{
  auto const a_current = dango::get_tick_count();
  auto const a_current_sa = dango::get_tick_count_sa();
  auto const a_bias = a_current_sa - a_current;
  auto const a_delta = a_bias - m_last_bias;

  m_last_bias = a_bias;

  return a_delta < dango::uint64(5);
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
    dango_unreachable_msg("cond_var watcher thread creation failed");
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

/*** cond_var_base ***/

void
dango::
detail::
cond_var_base::
regist
(mutex_type* const a_lock, dango::deadline const& a_deadline)noexcept
{
  if(!a_deadline.is_suspend_aware())
  {
    return;
  }

  constexpr auto& c_registry = detail::cond_var_registry_access::s_registry;

  dango_crit_full(c_registry, a_crit)
  {
    auto const a_prev = m_ref_count++;

    if(a_prev != dango::usize(0))
    {
      return;
    }

    dango_assert(m_current_mutex == nullptr);

    m_current_mutex = a_lock;

    c_registry.add(this);

    a_crit.notify();
  }
}

void
dango::
detail::
cond_var_base::
unregist
(mutex_type* const a_lock, dango::deadline const& a_deadline)noexcept
{
  if(!a_deadline.is_suspend_aware())
  {
    return;
  }

  constexpr auto& c_registry = detail::cond_var_registry_access::s_registry;

  dango_crit_full(c_registry, a_crit)
  {
    auto const a_current = --m_ref_count;

    if(a_current != dango::usize(0))
    {
      return;
    }

    dango_assert(m_current_mutex == a_lock);

    m_current_mutex = nullptr;

    c_registry.remove(this);

    a_crit.notify();
  }
}

void
dango::
detail::
cond_var_base::
process
()noexcept
{
  constexpr auto& c_registry = detail::cond_var_registry_access::s_registry;

  dango_crit_full(c_registry, a_crit)
  {
    dango_assert(m_ref_count != dango::usize(0));
    dango_assert(m_current_mutex != nullptr);

    dango_crit_cond(*this, m_current_mutex, a_notify_crit)
    {
      a_notify_crit.notify_all();
    }

    c_registry.remove(this);
    c_registry.add(this);
  }
}

#ifdef __linux__

#include <pthread.h>
#include <errno.h>

/*** get_tick_count ***/

namespace
dango::concurrent_cpp
{
  static auto tick_count(clockid_t)noexcept->dango::uint64;

  static constexpr auto const COND_VAR_CLOCK = clockid_t(CLOCK_REALTIME);
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
initialize
()noexcept
{
  using type = pthread_mutex_t;

  static_assert(sizeof(type) <= sizeof(detail::primitive_storage));
  static_assert(alignof(type) <= alignof(detail::primitive_storage));

  pthread_mutexattr_t a_attr;

  {
    auto const a_ret = pthread_mutexattr_init(&a_attr);

    dango_assert(a_ret == 0);
  }

  {
    auto const a_ret = pthread_mutexattr_settype(&a_attr, PTHREAD_MUTEX_NORMAL);

    dango_assert(a_ret == 0);
  }

  auto const a_prim = ::new (dango::placement, m_storage.get()) type;

  auto const a_result = pthread_mutex_init(a_prim, &a_attr);

  dango_assert(a_result == 0);

  {
    auto const a_ret = pthread_mutexattr_destroy(&a_attr);

    dango_assert(a_ret == 0);
  }
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

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

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

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

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
initialize
()noexcept
{
  using type = pthread_cond_t;

  static_assert(sizeof(type) <= sizeof(detail::primitive_storage));
  static_assert(alignof(type) <= alignof(detail::primitive_storage));

  pthread_condattr_t a_attr;

  {
    auto const a_ret = pthread_condattr_init(&a_attr);

    dango_assert(a_ret == 0);
  }

  {
    auto const a_ret = pthread_condattr_setclock(&a_attr, concurrent_cpp::COND_VAR_CLOCK);

    dango_assert(a_ret == 0);
  }

  auto const a_prim = ::new (dango::placement, m_storage.get()) type;

  auto const a_result = pthread_cond_init(a_prim, &a_attr);

  dango_assert(a_result == 0);

  {
    auto const a_ret = pthread_condattr_destroy(&a_attr);

    dango_assert(a_ret == 0);
  }
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

  dango_assert(a_lock != nullptr);

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  auto const a_result =
    pthread_cond_wait(get<type>(), a_lock->get<lock_type>());

  dango_assert(a_result == 0);
}

void
dango::
detail::
cond_var_base::
wait
(mutex_type* const a_lock, dango::deadline const& a_deadline)noexcept
{
  using type = pthread_cond_t;
  using lock_type = pthread_mutex_t;

  dango_assert(a_lock != nullptr);

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  using u64 = dango::uint64;

  auto const a_rem = a_deadline.remaining();

  if(a_rem == u64(0))
  {
    return;
  }

  auto const a_now = concurrent_cpp::tick_count(concurrent_cpp::COND_VAR_CLOCK);

  timespec a_spec;

  {
    auto const a_dead = a_now + a_rem;
    auto const a_sec = a_dead / u64(1'000);
    auto const a_mod = a_dead % u64(1'000);
    auto const a_nsec = a_mod * u64(1'000'000);

    a_spec.tv_sec = a_sec;
    a_spec.tv_nsec = a_nsec;
  }

  auto const a_result =
  pthread_cond_timedwait
  (
    get<type>(),
    a_lock->get<lock_type>(),
    &a_spec
  );

  dango_assert((a_result == 0) || (a_result == ETIMEDOUT));
}

void
dango::
detail::
cond_var_base::
notify
(mutex_type* const a_lock)noexcept
{
  using type = pthread_cond_t;

  dango_assert(a_lock != nullptr);

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  auto const a_result =
    pthread_cond_signal(get<type>());

  dango_assert(a_result == 0);
}

void
dango::
detail::
cond_var_base::
notify_all
(mutex_type* const a_lock)noexcept
{
  using type = pthread_cond_t;

  dango_assert(a_lock != nullptr);

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

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
  sched_yield();
}

static auto
dango::
concurrent_cpp::
tick_count
(clockid_t const a_clock)noexcept->dango::uint64
{
  using u64 = dango::uint64;

  constexpr auto const c_mul = u64(1'000);
  constexpr auto const c_div = u64(1'000'000);

  timespec a_spec;

  auto const a_result =
    clock_gettime(a_clock, &a_spec);

  dango_assert(a_result == 0);

  auto const a_sec = u64(a_spec.tv_sec);
  auto const a_nsec = u64(a_spec.tv_nsec);

  return (a_sec * c_mul) + (a_nsec / c_div);
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
  static_assert(dango::is_callable_ret<tp_func&, void>);
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

#endif /* __linux__ */

#ifdef _WIN32

#define DANGO_WINDOWS_VER 0x0601

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

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
  static dango::spin_mutex s_lock{ };
  static auto s_init_bias_mutable = dango::uint64(0);
  static auto s_prev = concurrent_cpp::perf_count(s_init_bias_mutable);
  static auto s_current = dango::uint64(0);

  constexpr auto const& c_init_bias = s_init_bias_mutable;

  dango::uint64 a_result;
  dango::uint64 a_bias;

  auto const a_freq = concurrent_cpp::perf_freq();
  auto const a_count = concurrent_cpp::perf_count(a_bias);

  dango_crit(s_lock)
  {
    auto const a_delta = a_count - s_prev;

    s_prev = a_count;

    s_current += (a_delta * dango::uint64(1'000)) / a_freq;

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
  static dango::spin_mutex s_lock{ };
  static auto s_prev = concurrent_cpp::perf_count();
  static auto s_current = dango::uint64(0);

  dango::uint64 a_result;

  auto const a_freq = concurrent_cpp::perf_freq();
  auto const a_count = concurrent_cpp::perf_count();

  dango_crit(s_lock)
  {
    auto const a_delta = a_count - s_prev;

    s_prev = a_count;

    s_current += (a_delta * dango::uint64(1'000)) / a_freq;

    a_result = s_current;
  }

  return a_result;
}

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
  using type = SRWLOCK;

  static_assert(dango::is_same<dango::remove_cv<tp_type>, type>);

  return dango::launder(static_cast<type*>(m_storage.get()));
}

void
dango::
detail::
mutex_base::
initialize
()noexcept
{
  using type = SRWLOCK;

  static_assert(sizeof(type) <= sizeof(detail::primitive_storage));
  static_assert(alignof(type) <= alignof(detail::primitive_storage));

  auto const a_prim = ::new (dango::placement, m_storage.get()) type;

  InitializeSRWLock(a_prim);
}

void
dango::
detail::
mutex_base::
destroy
()noexcept
{

}

auto
dango::
detail::
mutex_base::
acquire
()noexcept->mutex_base*
{
  using type = SRWLOCK;

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  AcquireSRWLockExclusive(get<type>());

  return this;
}

auto
dango::
detail::
mutex_base::
try_acquire
()noexcept->mutex_base*
{
  using type = SRWLOCK;

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  auto const a_result = TryAcquireSRWLockExclusive(get<type>());

  if(a_result)
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
  using type = SRWLOCK;

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  ReleaseSRWLockExclusive(get<type>());
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
  using type = CONDITION_VARIABLE;

  static_assert(dango::is_same<dango::remove_cv<tp_type>, type>);

  return dango::launder(static_cast<type*>(m_storage.get()));
}

void
dango::
detail::
cond_var_base::
initialize
()noexcept
{
  using type = CONDITION_VARIABLE;

  static_assert(sizeof(type) <= sizeof(detail::primitive_storage));
  static_assert(alignof(type) <= alignof(detail::primitive_storage));

  auto const a_prim = ::new (dango::placement, m_storage.get()) type;

  InitializeConditionVariable(a_prim);
}

void
dango::
detail::
cond_var_base::
destroy
()noexcept
{

}

void
dango::
detail::
cond_var_base::
wait
(mutex_type* const a_lock)noexcept
{
  using type = CONDITION_VARIABLE;
  using lock_type = SRWLOCK;

  dango_assert(a_lock != nullptr);

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  SleepConditionVariableSRW
  (
    get<type>(),
    a_lock->get<lock_type>(),
    DWORD(INFINITE),
    ULONG(0)
  );
}

void
dango::
detail::
cond_var_base::
wait
(mutex_type* const a_lock, dango::deadline const& a_deadline)noexcept
{
  using type = CONDITION_VARIABLE;
  using lock_type = SRWLOCK;

  dango_assert(a_lock != nullptr);

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  using u64 = dango::uint64;

  constexpr auto const c_max_interval = u64(DWORD(INFINITE) - DWORD(1));

  auto const a_rem = a_deadline.remaining();

  if(a_rem == u64(0))
  {
    return;
  }

  auto const a_interval = dango::min(a_rem, c_max_interval);

  dango_assert(a_interval != u64(INFINITE));

  regist(a_lock, a_deadline);

  SleepConditionVariableSRW
  (
    get<type>(),
    a_lock->get<lock_type>(),
    DWORD(a_interval),
    ULONG(0)
  );

  unregist(a_lock, a_deadline);
}

void
dango::
detail::
cond_var_base::
notify
(mutex_type* const a_lock)noexcept
{
  using type = CONDITION_VARIABLE;

  dango_assert(a_lock != nullptr);

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  WakeConditionVariable(get<type>());
}

void
dango::
detail::
cond_var_base::
notify_all
(mutex_type* const a_lock)noexcept
{
  using type = CONDITION_VARIABLE;

  dango_assert(a_lock != nullptr);

#ifndef DANGO_NO_DEBUG
  dango_assert(m_init.has_executed());
#endif

  WakeAllConditionVariable(get<type>());
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
  Sleep(DWORD(0));
}

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
  static auto& s_bias_ref = *reinterpret_cast<ULONGLONG const volatile*>(dango::uintptr(0x7FFE0000 + 0x3B0));

  dango::uint64 a_bias;
  dango::uint64 a_result;

  do
  {
    a_bias = s_bias_ref;
    a_result = concurrent_cpp::perf_count();
  }
  while(a_bias != s_bias_ref);

  a_suspend_bias = a_bias / dango::uint64(10'000);

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
  static_assert(dango::is_callable_ret<tp_func&, void>);
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

