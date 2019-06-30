#ifndef __DANGO_CONCURRENT_HPP__
#define __DANGO_CONCURRENT_HPP__

/*** get_tick_count ***/

namespace
dango
{
  auto get_tick_count()noexcept->dango::uint64;
  auto get_tick_count_sa()noexcept->dango::uint64;
  auto get_suspend_bias()noexcept->dango::uint64;
}

namespace
dango::detail
{
  auto tick_count()noexcept->dango::uint64;
  auto tick_count_sa()noexcept->dango::uint64;
  auto suspend_bias()noexcept->dango::uint64;

  using tick_count_tuple =
    dango::tuple<dango::uint64, dango::uint64, dango::uint64>;

  auto init_tick_count()noexcept->detail::tick_count_tuple const&;
}

inline auto
dango::
get_tick_count
()noexcept->dango::uint64
{
  auto const a_init = detail::init_tick_count().first();

  auto const a_count = detail::tick_count();

  return a_count - a_init;
}

inline auto
dango::
get_tick_count_sa
()noexcept->dango::uint64
{
  auto const a_init = detail::init_tick_count().second();

  auto const a_count = detail::tick_count_sa();

  return a_count - a_init;
}

inline auto
dango::
get_suspend_bias
()noexcept->dango::uint64
{
  auto const a_init = detail::init_tick_count().third();

  auto const a_count = detail::suspend_bias();

  return a_count - a_init;
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
  explicit constexpr timeout(value_type)noexcept;
public:
  virtual ~timeout()noexcept = default;
  auto has_expired()const noexcept->bool;
  auto remaining()const noexcept->value_type;
  auto get()const noexcept->value_type;
  void set(value_type)noexcept;
  void set_rel(value_type)noexcept;
  void add(value_type)noexcept;
  virtual auto is_suspend_aware()const noexcept->bool = 0;
  virtual auto requires_high_resolution()const noexcept->bool = 0;
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
protected:
  static auto get_tc()noexcept->value_type{ return dango::get_tick_count(); }
public:
  static auto make(value_type)noexcept->timeout_impl;
  static auto make_rel(value_type)noexcept->timeout_impl;
protected:
  explicit constexpr timeout_impl(value_type)noexcept;
public:
  virtual ~timeout_impl()noexcept override = default;
  virtual auto is_suspend_aware()const noexcept->bool override final;
  virtual auto requires_high_resolution()const noexcept->bool override;
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
  auto const a_now = get_tc();

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
requires_high_resolution
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
  return get_tc();
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
  explicit constexpr timeout_impl_hr(value_type)noexcept;
public:
  virtual ~timeout_impl_hr()noexcept override = default;
  virtual auto requires_high_resolution()const noexcept->bool override;
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
  auto const a_now = get_tc();

  return timeout_impl_hr{ safe_add(a_now, a_interval) };
}

constexpr
dango::
detail::
timeout_impl_hr::
timeout_impl_hr
(value_type const a_timeout)noexcept:
super_type{ a_timeout }
{

}

inline auto
dango::
detail::
timeout_impl_hr::
requires_high_resolution
()const noexcept->bool
{
  return true;
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
protected:
  static auto get_tc()noexcept->value_type{ return dango::get_tick_count_sa(); }
public:
  static auto make(value_type)noexcept->timeout_impl_sa;
  static auto make_rel(value_type)noexcept->timeout_impl_sa;
protected:
  explicit constexpr timeout_impl_sa(value_type)noexcept;
public:
  virtual ~timeout_impl_sa()noexcept override = default;
  virtual auto is_suspend_aware()const noexcept->bool override final;
  virtual auto requires_high_resolution()const noexcept->bool override;
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
  auto const a_now = get_tc();

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
requires_high_resolution
()const noexcept->bool
{
  return false;
}

inline auto
dango::
detail::
timeout_impl_sa::
tick_count
()const noexcept->value_type
{
  return get_tc();
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
  explicit constexpr timeout_impl_hr_sa(value_type)noexcept;
public:
  virtual ~timeout_impl_hr_sa()noexcept override = default;
  virtual auto requires_high_resolution()const noexcept->bool override;
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
  auto const a_now = get_tc();

  return timeout_impl_hr_sa{ safe_add(a_now, a_interval) };
}

constexpr
dango::
detail::
timeout_impl_hr_sa::
timeout_impl_hr_sa
(value_type const a_timeout)noexcept:
super_type{ a_timeout }
{

}

inline auto
dango::
detail::
timeout_impl_hr_sa::
requires_high_resolution
()const noexcept->bool
{
  return true;
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
  class cond_var_base;
  class cond_var_registry;
}

class alignas(dango::cache_align_type)
dango::
detail::
cond_var_base:
public dango::intrusive_list_elem<cond_var_base>
{
  friend dango::detail::cond_var_registry;
private:
  using super_type = dango::intrusive_list_elem<cond_var_base>;
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
  struct construct_tag;
  class control_block;
  class registry;
  class notifier;

  template
  <typename tp_func, typename... tp_args>
  class runnable;

  template
  <typename tp_func>
  class runnable<tp_func>;
private:
  static auto new_control_block(bool)noexcept->control_block*;
  static auto thread_self_init(bool)noexcept->thread const&;
  static void start_thread(thread_start_func, void*)noexcept(false);

  template
  <typename tp_func>
  static void thread_start_address(void*)noexcept(false);

  static registry s_registry;
  static thread const& s_main_init;
  static dango::uint64 const s_tick_count_init;
public:
  static void yield()noexcept;
  static auto self()noexcept->thread const&;
  static void main_join()noexcept;
  [[nodiscard]] static auto main_join_finally()noexcept;

  template
  <typename tp_func, typename... tp_args>
  static auto
  create(bool, tp_func&&, tp_args&&...)noexcept(false)->
  dango::enable_if
  <
    (dango::is_constructible<dango::decay<tp_func>, tp_func> && ... && dango::is_constructible<dango::decay<tp_args>, tp_args>) &&
    (dango::is_noexcept_destructible<dango::decay<tp_func>> && ... && dango::is_noexcept_destructible<dango::decay<tp_args>>) &&
    dango::is_callable_ret<void, dango::decay<tp_func>&, dango::tuple_get_type<dango::decay<tp_args>, false>...>,
    dango::thread
  >;

  template
  <typename tp_func, typename... tp_args>
  static auto
  create(tp_func&&, tp_args&&...)noexcept(false)->
  dango::enable_if
  <
    (dango::is_constructible<dango::decay<tp_func>, tp_func> && ... && dango::is_constructible<dango::decay<tp_args>, tp_args>) &&
    (dango::is_noexcept_destructible<dango::decay<tp_func>> && ... && dango::is_noexcept_destructible<dango::decay<tp_args>>) &&
    dango::is_callable_ret<void, dango::decay<tp_func>&, dango::tuple_get_type<dango::decay<tp_args>, false>...>,
    dango::thread
  >;

  template
  <typename tp_func, typename... tp_args>
  static auto
  create_daemon(tp_func&&, tp_args&&...)noexcept(false)->
  dango::enable_if
  <
    (dango::is_constructible<dango::decay<tp_func>, tp_func> && ... && dango::is_constructible<dango::decay<tp_args>, tp_args>) &&
    (dango::is_noexcept_destructible<dango::decay<tp_func>> && ... && dango::is_noexcept_destructible<dango::decay<tp_args>>) &&
    dango::is_callable_ret<void, dango::decay<tp_func>&, dango::tuple_get_type<dango::decay<tp_args>, false>...>,
    dango::thread
  >;

  static void sleep(dango::timeout const&)noexcept;
  static void sleep_rel(dango::uint64)noexcept;
  static void sleep_rel_hr(dango::uint64)noexcept;
  static void sleep_rel_sa(dango::uint64)noexcept;
  static void sleep_rel_hr_sa(dango::uint64)noexcept;
private:
  explicit thread(construct_tag, bool)noexcept;
  explicit thread(control_block*)noexcept;
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
  constexpr auto is_daemon()const noexcept->bool;
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
final:
public dango::intrusive_list_elem<control_block>
{
private:
  using super_type = dango::intrusive_list_elem<control_block>;
public:
  static auto operator new(dango::usize)dango_new_noexcept(true)->void*;
  static void operator delete(void*, dango::usize)noexcept;
public:
  explicit constexpr control_block(bool)noexcept;
  ~control_block()noexcept;
  void increment()noexcept;
  auto decrement()noexcept->bool;
  void wait()noexcept;
  void wait(dango::timeout const&)noexcept;
  void notify_all()noexcept;
  auto is_alive()const noexcept->bool;
  constexpr auto is_daemon()const noexcept->bool;
private:
  dango::atomic<dango::usize> m_ref_count;
  bool const m_daemon;
  mutable dango::mutex m_mutex;
  mutable dango::cond_var_mutex m_cond;
  bool m_alive;
  dango::usize m_waiter_count;
public:
  DANGO_DELETE_DEFAULT(control_block)
  DANGO_IMMOBILE(control_block)
};

inline void
dango::
thread::
control_block::
operator delete
(void* const a_ptr, dango::usize const a_size)noexcept
{
  dango::operator_delete(a_ptr, a_size, alignof(control_block));
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
  auto const a_prev = m_ref_count.fetch_add<dango::mem_order::acquire>(dango::usize(1));

  dango_assert(a_prev != dango::usize(0));
}

inline auto
dango::
thread::
control_block::
decrement
()noexcept->bool
{
  return m_ref_count.sub_fetch<dango::mem_order::release>(dango::usize(1)) == dango::usize(0);
}

constexpr auto
dango::
thread::
control_block::
is_daemon
()const noexcept->bool
{
  return m_daemon;
}

/*** registry ***/

class
dango::
thread::
registry
final
{
public:
  constexpr registry()noexcept;
  ~registry()noexcept = default;
  void regist(control_block*)noexcept;
  void unregist(control_block*)noexcept;
  void join_all()noexcept;
private:
  auto join_first()noexcept->bool;
private:
  dango::static_mutex m_mutex;
  dango::intrusive_list<control_block> m_list;
public:
  DANGO_IMMOBILE(registry)
};

constexpr
dango::
thread::
registry::
registry
()noexcept:
m_mutex{ },
m_list{ }
{

}

/*** runnable ***/

template
<typename tp_func, typename... tp_args>
class alignas(dango::cache_align_type)
dango::
thread::
runnable
final
{
public:
  static auto
  operator new
  (dango::usize const a_size)dango_new_noexcept(true)->void*
  {
    return dango::operator_new(a_size, alignof(runnable)).dismiss();
  }

  static void
  operator delete
  (void* const a_ptr, dango::usize const a_size)noexcept
  {
    dango::operator_delete(a_ptr, a_size, alignof(runnable));
  }
public:
  template
  <typename tp_tp_func, typename... tp_tp_args>
  explicit constexpr
  runnable
  (tp_tp_func&& a_func, tp_tp_args&&... a_args)noexcept(false):
  m_func{ dango::forward<tp_tp_func>(a_func) },
  m_args{ dango::forward<tp_tp_args>(a_args)... }
  {

  }

  ~runnable()noexcept = default;

  constexpr void
  run()noexcept(false)
  {
    m_args->*m_func;
  }
private:
  tp_func m_func;
  dango::tuple<tp_args...> m_args;
public:
  DANGO_DELETE_DEFAULT(runnable)
  DANGO_IMMOBILE(runnable)
};

template
<typename tp_func>
class alignas(dango::cache_align_type)
dango::
thread::
runnable<tp_func>
final
{
public:
  static auto
  operator new
  (dango::usize const a_size)dango_new_noexcept(true)->void*
  {
    return dango::operator_new(a_size, alignof(runnable)).dismiss();
  }

  static void
  operator delete
  (void* const a_ptr, dango::usize const a_size)noexcept
  {
    dango::operator_delete(a_ptr, a_size, alignof(runnable));
  }
public:
  template
  <typename tp_tp_func>
  explicit constexpr
  runnable
  (tp_tp_func&& a_func)noexcept(false):
  m_func{ dango::forward<tp_tp_func>(a_func) }
  {

  }

  ~runnable()noexcept = default;

  constexpr void
  run()noexcept(false)
  {
    m_func();
  }
private:
  tp_func m_func;
public:
  DANGO_DELETE_DEFAULT(runnable)
  DANGO_IMMOBILE(runnable)
};

/*** thread ***/

inline dango::thread::registry
dango::
thread::
s_registry{ };

inline dango::thread const&
dango::
thread::
s_main_init = dango::thread::self();

inline dango::uint64 const
dango::
thread::
s_tick_count_init = dango::get_tick_count();

inline auto
dango::
thread::
self
()noexcept->thread const&
{
  return thread_self_init(true);
}

inline void
dango::
thread::
main_join
()noexcept
{
  dango::thread::s_registry.join_all();
}

inline auto
dango::
thread::
main_join_finally
()noexcept->auto
{
  return dango::make_finally(dango::thread::main_join);
}

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
is_daemon
()const noexcept->bool
{
  dango_assert(m_control != nullptr);

  return m_control->is_daemon();
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
<typename tp_func, typename... tp_args>
auto
dango::
thread::
create
(bool const a_daemon, tp_func&& a_thread_func, tp_args&&... a_args)noexcept(false)->
dango::enable_if
<
  (dango::is_constructible<dango::decay<tp_func>, tp_func> && ... && dango::is_constructible<dango::decay<tp_args>, tp_args>) &&
  (dango::is_noexcept_destructible<dango::decay<tp_func>> && ... && dango::is_noexcept_destructible<dango::decay<tp_args>>) &&
  dango::is_callable_ret<void, dango::decay<tp_func>&, dango::tuple_get_type<dango::decay<tp_args>, false>...>,
  dango::thread
>
{
  constexpr auto const acquire = dango::mem_order::acquire;
  constexpr auto const release = dango::mem_order::release;

  using run_type = runnable<dango::decay<tp_func>, dango::decay<tp_args>...>;

  auto const a_runnable = new run_type{ dango::forward<tp_func>(a_thread_func), dango::forward<tp_args>(a_args)... };

  auto a_guard =
  dango::make_guard
  (
    [a_runnable]()noexcept->void
    {
      delete a_runnable;
    }
  );

  thread a_ret{ dango::null };

  dango::atomic<bool> a_starting{ true };

  auto a_func =
  [
    a_daemon,
    a_runnable,
    &a_ret,
    &a_starting
  ]
  ()noexcept(false)->void
  {
    a_ret = thread_self_init(a_daemon);

    a_starting.store<release>(false);

    auto const a_fin =
    dango::make_finally
    (
      [a_runnable]()noexcept->void
      {
        delete a_runnable;
      }
    );

    a_runnable->run();
  };

  start_thread(thread_start_address<decltype(a_func)>, &a_func);

  {
    auto a_count = detail::c_spin_count_init;

    while(a_starting.load<acquire>())
    {
      detail::spin_yield(a_count);
    }
  }

  a_guard.dismiss();

  return a_ret;
}

template
<typename tp_func, typename... tp_args>
auto
dango::
thread::
create
(tp_func&& a_thread_func, tp_args&&... a_args)noexcept(false)->
dango::enable_if
<
  (dango::is_constructible<dango::decay<tp_func>, tp_func> && ... && dango::is_constructible<dango::decay<tp_args>, tp_args>) &&
  (dango::is_noexcept_destructible<dango::decay<tp_func>> && ... && dango::is_noexcept_destructible<dango::decay<tp_args>>) &&
  dango::is_callable_ret<void, dango::decay<tp_func>&, dango::tuple_get_type<dango::decay<tp_args>, false>...>,
  dango::thread
>
{
  return thread::create(false, dango::forward<tp_func>(a_thread_func), dango::forward<tp_args>(a_args)...);
}

template
<typename tp_func, typename... tp_args>
auto
dango::
thread::
create_daemon
(tp_func&& a_thread_func, tp_args&&... a_args)noexcept(false)->
dango::enable_if
<
  (dango::is_constructible<dango::decay<tp_func>, tp_func> && ... && dango::is_constructible<dango::decay<tp_args>, tp_args>) &&
  (dango::is_noexcept_destructible<dango::decay<tp_func>> && ... && dango::is_noexcept_destructible<dango::decay<tp_args>>) &&
  dango::is_callable_ret<void, dango::decay<tp_func>&, dango::tuple_get_type<dango::decay<tp_args>, false>...>,
  dango::thread
>
{
  return thread::create(true, dango::forward<tp_func>(a_thread_func), dango::forward<tp_args>(a_args)...);
}

template
<typename tp_func>
void
dango::
thread::
thread_start_address
(void* const a_data)noexcept(false)
{
  static_assert(!dango::is_ref<tp_func>);
  static_assert(!dango::is_const<tp_func> && !dango::is_volatile<tp_func>);
  static_assert(dango::is_noexcept_move_constructible<tp_func>);
  static_assert(dango::is_callable_ret<void, tp_func const&>);
  static_assert(dango::is_noexcept_destructible<tp_func>);

  auto const a_func_ptr = static_cast<tp_func*>(a_data);

  tp_func const a_func = dango::move(*a_func_ptr);

  a_func();
}

/*** cond_var_registry ***/

namespace
dango::detail
{
  class cond_var_registry;
  class cond_var_registry_thread;
  class cond_var_registry_access;
}

class
dango::
detail::
cond_var_registry
final
{
  friend dango::detail::cond_var_registry_access;
private:
  using cond_type = dango::detail::cond_var_base;
  using list_type = dango::intrusive_list<cond_type>;
private:
  static void remove(cond_type*)noexcept;
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
  list_type m_list[2];
  list_type* m_external_list;
  list_type* m_internal_list;
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
m_list{ { }, { } },
m_external_list{ &m_list[0] },
m_internal_list{ &m_list[1] },
m_alive{ true },
m_waiting{ false }
{

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

/*** windows_timer_res_manager ***/

namespace
dango::detail
{
  class windows_timer_res_access;
  class windows_timer_res_manager;
  class windows_timer_res_daemon;
}

class
dango::
detail::
windows_timer_res_access
final
{
  friend dango::detail::windows_timer_res_daemon;
  friend dango::detail::cond_var_base;
private:
  static dango::detail::windows_timer_res_manager s_manager;
public:
  DANGO_UNINSTANTIABLE(windows_timer_res_access)
};

class
dango::
detail::
windows_timer_res_manager
final
{
  friend dango::detail::windows_timer_res_access;
private:
  enum class
  timer_state:
  dango::uint8
  {
    ACTIVATED, DEACTIVATING, DEACTIVATED
  };
public:
  void activate(dango::timeout const&)noexcept;
  void deactivate(dango::timeout const&)noexcept;
  void notify_exit()noexcept;
  void thread_func()noexcept;
private:
  constexpr windows_timer_res_manager()noexcept;
  ~windows_timer_res_manager()noexcept = default;
  auto wait()noexcept->bool;
  auto timed_wait(dango::timeout const&)noexcept->bool;
private:
  dango::static_mutex m_mutex;
  dango::static_cond_var_mutex m_cond;
  bool m_alive;
  bool m_waiting;
  timer_state m_timer_state;
  dango::usize m_count;
public:
  DANGO_IMMOBILE(windows_timer_res_manager)
};

class
dango::
detail::
windows_timer_res_daemon
final
{
private:
  static auto start_thread()noexcept->dango::thread;
public:
  windows_timer_res_daemon()noexcept;
  ~windows_timer_res_daemon()noexcept;
private:
  dango::thread const m_thread;
public:
  DANGO_IMMOBILE(windows_timer_res_daemon)
};

namespace
dango::detail
{
  DANGO_DEFINE_GLOBAL_INLINE_CV(s_windows_timer_res_daemon, const, detail::windows_timer_res_daemon{ })
}

#endif

#endif

