#ifndef DANGO_CONCURRENT_HPP_INCLUDED
#define DANGO_CONCURRENT_HPP_INCLUDED

/*** get_tick_count ***/

namespace
dango
{
  using tick_count_type = dango::slong;

  auto get_tick_count()noexcept->dango::tick_count_type;
  auto get_tick_count_sa()noexcept->dango::tick_count_type;
  auto get_suspend_bias()noexcept->dango::tick_count_type;
}

namespace
dango::detail
{
  DANGO_EXPORT auto tick_count()noexcept->dango::tick_count_type;
  DANGO_EXPORT auto tick_count_sa()noexcept->dango::tick_count_type;
  DANGO_EXPORT auto suspend_bias()noexcept->dango::tick_count_type;

  using tick_count_tuple =
    dango::tuple<dango::tick_count_type, dango::tick_count_type, dango::tick_count_type>;

  DANGO_EXPORT auto init_tick_count()noexcept->detail::tick_count_tuple const&;
}

inline auto
dango::
get_tick_count
()noexcept->dango::tick_count_type
{
  auto const a_init = detail::init_tick_count().first();

  auto const a_count = detail::tick_count();

  return a_count - a_init;
}

inline auto
dango::
get_tick_count_sa
()noexcept->dango::tick_count_type
{
  auto const a_init = detail::init_tick_count().second();

  auto const a_count = detail::tick_count_sa();

  return a_count - a_init;
}

inline auto
dango::
get_suspend_bias
()noexcept->dango::tick_count_type
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
  using value_type = dango::tick_count_type;
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

  m_timeout = dango::integer::safe_add(a_now, a_interval);
}

inline void
dango::
timeout::
add
(value_type const a_val)noexcept
{
  m_timeout = dango::integer::safe_add(m_timeout, a_val);
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

  return timeout_impl{ dango::integer::safe_add(a_now, a_interval) };
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

  return timeout_impl_hr{ dango::integer::safe_add(a_now, a_interval) };
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

  return timeout_impl_sa{ dango::integer::safe_add(a_now, a_interval) };
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

  return timeout_impl_hr_sa{ dango::integer::safe_add(a_now, a_interval) };
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
  auto make_timeout(dango::tick_count_type)noexcept->detail::timeout_impl;
  auto make_timeout_hr(dango::tick_count_type)noexcept->detail::timeout_impl_hr;
  auto make_timeout_sa(dango::tick_count_type)noexcept->detail::timeout_impl_sa;
  auto make_timeout_hr_sa(dango::tick_count_type)noexcept->detail::timeout_impl_hr_sa;

  auto make_timeout_rel(dango::tick_count_type)noexcept->detail::timeout_impl;
  auto make_timeout_rel_hr(dango::tick_count_type)noexcept->detail::timeout_impl_hr;
  auto make_timeout_rel_sa(dango::tick_count_type)noexcept->detail::timeout_impl_sa;
  auto make_timeout_rel_hr_sa(dango::tick_count_type)noexcept->detail::timeout_impl_hr_sa;
}

inline auto
dango::
make_timeout
(dango::tick_count_type const a_timeout)noexcept->detail::timeout_impl
{
  return detail::timeout_impl::make(a_timeout);
}

inline auto
dango::
make_timeout_hr
(dango::tick_count_type const a_timeout)noexcept->detail::timeout_impl_hr
{
  return detail::timeout_impl_hr::make(a_timeout);
}

inline auto
dango::
make_timeout_sa
(dango::tick_count_type const a_timeout)noexcept->detail::timeout_impl_sa
{
  return detail::timeout_impl_sa::make(a_timeout);
}

inline auto
dango::
make_timeout_hr_sa
(dango::tick_count_type const a_timeout)noexcept->detail::timeout_impl_hr_sa
{
  return detail::timeout_impl_hr_sa::make(a_timeout);
}

inline auto
dango::
make_timeout_rel
(dango::tick_count_type const a_interval)noexcept->detail::timeout_impl
{
  return detail::timeout_impl::make_rel(a_interval);
}

inline auto
dango::
make_timeout_rel_hr
(dango::tick_count_type const a_interval)noexcept->detail::timeout_impl_hr
{
  return detail::timeout_impl_hr::make_rel(a_interval);
}

inline auto
dango::
make_timeout_rel_sa
(dango::tick_count_type const a_interval)noexcept->detail::timeout_impl_sa
{
  return detail::timeout_impl_sa::make_rel(a_interval);
}

inline auto
dango::
make_timeout_rel_hr_sa
(dango::tick_count_type const a_interval)noexcept->detail::timeout_impl_hr_sa
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
protected:
  DANGO_EXPORT void destroy()noexcept;
public:
  [[nodiscard]] auto lock()noexcept->locker;
  [[nodiscard]] auto try_lock()noexcept->try_locker;
private:
  auto get()noexcept->mutex_impl*;
  void init()noexcept;
private:
  DANGO_EXPORT void initialize()noexcept;
  DANGO_EXPORT auto acquire()noexcept->mutex_base*;
  DANGO_EXPORT auto try_acquire()noexcept->mutex_base*;
  DANGO_EXPORT void release()noexcept;
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
  explicit operator bool()const{ return m_lock != dango::null; }
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
  struct mutex;
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
  struct static_mutex;
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
protected:
  DANGO_EXPORT void destroy()noexcept;
public:
  DANGO_EXPORT void notify()noexcept;
  DANGO_EXPORT void notify_all()noexcept;
private:
  auto get()noexcept->cond_var_impl*;
  void init()noexcept;
private:
  DANGO_EXPORT void initialize()noexcept;
  DANGO_EXPORT void wait(mutex_type*)noexcept;
  DANGO_EXPORT void wait(mutex_type*, dango::timeout const&)noexcept;
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
  explicit operator bool()const{ return m_lock != dango::null; }
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
  struct cond_var;
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
  struct static_cond_var;
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
  struct cond_var_mutex;
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
  struct static_cond_var_mutex;
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

  enum class
  thread_ID:
  dango::uptr
  {
    null_ID = dango::ptr_as_uint(dango::null)
  };
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
  DANGO_EXPORT static auto thread_self_init(bool)noexcept->thread const&;
  DANGO_EXPORT static auto thread_self_access_check(bool, void const**)noexcept->bool;
  DANGO_EXPORT static void start_thread(thread_start_func, void*)noexcept(false);
  static auto new_control_block(bool)noexcept->control_block*;

  template
  <typename tp_func>
  static void thread_start_address(void*)noexcept(false);

  DANGO_EXPORT_ONLY static registry s_registry;
public:
  static void yield()noexcept;
  static auto self()noexcept->thread const&;
  static auto can_call_self()noexcept->bool;
  static auto self_ID()noexcept->dango::thread_ID;
  static void main_join()noexcept;
  [[nodiscard]] static auto main_join_finally()noexcept;

  template
  <typename tp_func, typename... tp_args>
  requires
  (
    (dango::is_noexcept_destructible<dango::decay<tp_func>> && ... && dango::is_noexcept_destructible<dango::decay<tp_args>>) &&
    dango::is_brace_constructible<dango::decay<tp_func>, tp_func> &&
    dango::is_brace_constructible<dango::tuple<dango::decay<tp_args>...>, tp_args...> &&
    dango::is_callable_ret<void, dango::decay<tp_func>&, dango::tuple_get_type<dango::tuple_model&, dango::decay<tp_args>>...>
  )
  static auto
  create(bool, tp_func&&, tp_args&&...)noexcept(false)->dango::thread;

  template
  <typename tp_func, typename... tp_args>
  requires
  (
    (dango::is_noexcept_destructible<dango::decay<tp_func>> && ... && dango::is_noexcept_destructible<dango::decay<tp_args>>) &&
    dango::is_brace_constructible<dango::decay<tp_func>, tp_func> &&
    dango::is_brace_constructible<dango::tuple<dango::decay<tp_args>...>, tp_args...> &&
    dango::is_callable_ret<void, dango::decay<tp_func>&, dango::tuple_get_type<dango::tuple_model&, dango::decay<tp_args>>...>
  )
  static auto
  create(tp_func&& a_thread_func, tp_args&&... a_args)noexcept(false)->dango::thread
  {
    return thread::create(false, dango::forward<tp_func>(a_thread_func), dango::forward<tp_args>(a_args)...);
  }

  template
  <typename tp_func, typename... tp_args>
  requires
  (
    (dango::is_noexcept_destructible<dango::decay<tp_func>> && ... && dango::is_noexcept_destructible<dango::decay<tp_args>>) &&
    dango::is_brace_constructible<dango::decay<tp_func>, tp_func> &&
    dango::is_brace_constructible<dango::tuple<dango::decay<tp_args>...>, tp_args...> &&
    dango::is_callable_ret<void, dango::decay<tp_func>&, dango::tuple_get_type<dango::tuple_model&, dango::decay<tp_args>>...>
  )
  static auto
  create_daemon(tp_func&& a_thread_func, tp_args&&... a_args)noexcept(false)->dango::thread
  {
    return thread::create(true, dango::forward<tp_func>(a_thread_func), dango::forward<tp_args>(a_args)...);
  }

  DANGO_EXPORT static void sleep(dango::timeout const&)noexcept;
  static void sleep_rel(dango::tick_count_type)noexcept;
  static void sleep_rel_hr(dango::tick_count_type)noexcept;
  static void sleep_rel_sa(dango::tick_count_type)noexcept;
  static void sleep_rel_hr_sa(dango::tick_count_type)noexcept;
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
  constexpr auto get_ID()const noexcept->dango::thread_ID;
  constexpr auto dango_operator_is_null()const noexcept->bool;
  constexpr auto dango_operator_equals(dango::null_tag)const noexcept = delete;
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
  static auto operator new(dango::usize)dango_new_noexcept->void*;
  static void operator delete(void*, dango::usize)noexcept;
public:
  explicit constexpr control_block(bool, dango::thread_ID)noexcept;
  ~control_block()noexcept = default;
  void increment()noexcept;
  auto decrement()noexcept->bool;
  void wait()noexcept;
  void wait(dango::timeout const&)noexcept;
  void notify_all()noexcept;
  auto is_alive()const noexcept->bool;
  constexpr auto is_daemon()const noexcept->bool;
  constexpr auto get_ID()const noexcept->dango::thread_ID;
private:
  dango::atomic<dango::usize> m_ref_count;
  bool const m_daemon;
  dango::thread_ID const m_thread_ID;
  mutable dango::mutex m_mutex;
  mutable dango::cond_var_mutex m_cond;
  bool m_alive;
  dango::usize m_waiter_count;
public:
  DANGO_DELETE_DEFAULT(control_block)
  DANGO_IMMOBILE(control_block)
};

inline auto
dango::
thread::
control_block::
operator new
(dango::usize const a_size)dango_new_noexcept->void*
{
  return dango::operator_new(a_size, alignof(control_block));
}

inline void
dango::
thread::
control_block::
operator delete
(void* const a_ptr, dango::usize const a_size)noexcept
{
  dango::operator_delete(a_ptr, a_size, alignof(control_block));
}

constexpr
dango::
thread::
control_block::
control_block
(bool const a_daemon, dango::thread_ID const a_thread_ID)noexcept:
super_type{ },
m_ref_count{ dango::usize(1) },
m_daemon{ a_daemon },
m_thread_ID{ a_thread_ID },
m_mutex{ },
m_cond{ m_mutex },
m_alive{ true },
m_waiter_count{ dango::usize(0) }
{

}

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

inline void
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

inline void
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

inline void
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

inline auto
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

constexpr auto
dango::
thread::
control_block::
is_daemon
()const noexcept->bool
{
  return m_daemon;
}

constexpr auto
dango::
thread::
control_block::
get_ID
()const noexcept->dango::thread_ID
{
  return m_thread_ID;
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

inline void
dango::
thread::
registry::
regist
(control_block* const a_elem)noexcept
{
  dango_crit(m_mutex)
  {
    m_list.add_last(a_elem);
  }
}

inline void
dango::
thread::
registry::
unregist
(control_block* const a_elem)noexcept
{
  dango_crit(m_mutex)
  {
    m_list.remove(a_elem);
  }
}

inline void
dango::
thread::
registry::
join_all
()noexcept
{
  while(join_first());
}

inline auto
dango::
thread::
registry::
join_first
()noexcept->bool
{
  dango::thread a_thread{ dango::null };

  dango_crit(m_mutex)
  {
    if(m_list.is_empty())
    {
      return false;
    }

    auto const a_control = m_list.first();

    a_thread = thread{ a_control };
  }

  a_thread.join();

  return true;
}

/*** notifier ***/

class
dango::
thread::
notifier
final
{
public:
  notifier(thread const&)noexcept;
  ~notifier()noexcept;
private:
  control_block* const m_control;
public:
  DANGO_DELETE_DEFAULT(notifier)
  DANGO_IMMOBILE(notifier)
};

inline
dango::
thread::
notifier::
notifier
(thread const& a_thread)noexcept:
m_control{ a_thread.m_control }
{
  if(!m_control->is_daemon())
  {
    dango::thread::s_registry.regist(m_control);
  }
}

inline
dango::
thread::
notifier::
~notifier
()noexcept
{
  if(!m_control->is_daemon())
  {
    dango::thread::s_registry.unregist(m_control);
  }

  m_control->notify_all();

  thread::thread_self_access_check(true, dango::null);
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
  (dango::usize const a_size)dango_new_noexcept->void*
  {
    return dango::operator_new(a_size, alignof(runnable));
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
  (dango::usize const a_size)dango_new_noexcept->void*
  {
    return dango::operator_new(a_size, alignof(runnable));
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

struct
dango::
thread::
construct_tag
final
{
  DANGO_TAG_TYPE(construct_tag)
};

inline auto
dango::
thread::
new_control_block
(bool const a_daemon)noexcept->control_block*
{
  try
  {
    return new control_block{ a_daemon, thread::self_ID() };
  }
  catch(...)
  {
    dango_unreachable_terminate_msg(u8"thread control block allocation failed");
  }
}

inline
dango::
thread::
thread
(construct_tag const, bool const a_daemon)noexcept:
m_control{ new_control_block(a_daemon) }
{

}

inline
dango::
thread::
thread
(control_block* const a_control)noexcept:
m_control{ a_control }
{
  dango_assert(m_control != dango::null);

  m_control->increment();
}

inline auto
dango::
thread::
is_alive
()const noexcept->bool
{
  dango_assert(m_control != dango::null);

  return m_control->is_alive();
}

inline void
dango::
thread::
join
()const noexcept
{
  dango_assert(m_control != dango::null);

  m_control->wait();
}

inline void
dango::
thread::
join
(dango::timeout const& a_timeout)const noexcept
{
  dango_assert(m_control != dango::null);

  m_control->wait(a_timeout);
}

inline void
dango::
thread::
yield
()noexcept
{
  dango::thread_yield();
}

inline auto
dango::
thread::
self
()noexcept->thread const&
{
  dango_assert(can_call_self());

  return thread_self_init(true);
}

inline auto
dango::
thread::
can_call_self
()noexcept->bool
{
  return thread_self_access_check(false, dango::null);
}

inline auto
dango::
thread::
self_ID
()noexcept->dango::thread_ID
{
  void const* a_result;

  thread_self_access_check(false, &a_result);

  return dango::thread_ID{ dango::ptr_as_uint(a_result) };
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

inline void
dango::
thread::
sleep_rel
(dango::tick_count_type const a_interval)noexcept
{
  auto const a_timeout = dango::make_timeout_rel(a_interval);

  thread::sleep(a_timeout);
}

inline void
dango::
thread::
sleep_rel_hr
(dango::tick_count_type const a_interval)noexcept
{
  auto const a_timeout = dango::make_timeout_rel_hr(a_interval);

  thread::sleep(a_timeout);
}

inline void
dango::
thread::
sleep_rel_sa
(dango::tick_count_type const a_interval)noexcept
{
  auto const a_timeout = dango::make_timeout_rel_sa(a_interval);

  thread::sleep(a_timeout);
}

inline void
dango::
thread::
sleep_rel_hr_sa
(dango::tick_count_type const a_interval)noexcept
{
  auto const a_timeout = dango::make_timeout_rel_hr_sa(a_interval);

  thread::sleep(a_timeout);
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
m_control{ dango::null }
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
  a_thread.m_control = dango::null;
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
  return m_control != dango::null;
}

constexpr auto
dango::
thread::
is_daemon
()const noexcept->bool
{
  dango_assert(m_control != dango::null);

  return m_control->is_daemon();
}

constexpr auto
dango::
thread::
get_ID
()const noexcept->dango::thread_ID
{
  dango_assert(m_control != dango::null);

  return m_control->get_ID();
}

constexpr auto
dango::
thread::
dango_operator_is_null
()const noexcept->bool
{
  return m_control == dango::null;
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
requires
(
  (dango::is_noexcept_destructible<dango::decay<tp_func>> && ... && dango::is_noexcept_destructible<dango::decay<tp_args>>) &&
  dango::is_brace_constructible<dango::decay<tp_func>, tp_func> &&
  dango::is_brace_constructible<dango::tuple<dango::decay<tp_args>...>, tp_args...> &&
  dango::is_callable_ret<void, dango::decay<tp_func>&, dango::tuple_get_type<dango::tuple_model&, dango::decay<tp_args>>...>
)
auto
dango::
thread::
create
(bool const a_daemon, tp_func&& a_thread_func, tp_args&&... a_args)noexcept(false)->dango::thread
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
  class cond_var_registry_access;
  class cond_var_registry;
  class cond_var_registry_thread;
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
  DANGO_EXPORT_ONLY static dango::detail::cond_var_registry s_registry;
public:
  DANGO_UNINSTANTIABLE(cond_var_registry_access)
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
  using list_type = dango::intrusive_list<cond_type>;
private:
  DANGO_EXPORT static void remove(cond_type*)noexcept;
  DANGO_EXPORT static auto bias_okay(dango::tick_count_type&)noexcept->bool;
public:
  DANGO_EXPORT void regist(cond_type*, dango::timeout const&)noexcept;
  DANGO_EXPORT void unregist(cond_type*, dango::timeout const&)noexcept;
  DANGO_EXPORT void notify_exit()noexcept;
  DANGO_EXPORT void thread_func()noexcept;
private:
  constexpr cond_var_registry()noexcept;
  ~cond_var_registry()noexcept = default;
private:
  DANGO_EXPORT void add(cond_type*)noexcept;
  DANGO_EXPORT auto wait_empty()noexcept->bool;
  DANGO_EXPORT auto poll_bias(dango::tick_count_type&, dango::timeout&)noexcept->bool;
  DANGO_EXPORT auto pop_internal()noexcept->bool;
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

/*** cond_var_registry_thread ***/

class
dango::
detail::
cond_var_registry_thread
final
{
private:
  DANGO_EXPORT static auto start_thread()noexcept->dango::thread;
public:
  cond_var_registry_thread()noexcept;
  ~cond_var_registry_thread()noexcept;
private:
  dango::thread const m_thread;
public:
  DANGO_IMMOBILE(cond_var_registry_thread)
};

inline
dango::
detail::
cond_var_registry_thread::
cond_var_registry_thread
()noexcept:
m_thread{ start_thread() }
{

}

inline
dango::
detail::
cond_var_registry_thread::
~cond_var_registry_thread
()noexcept
{
  constexpr auto& c_registry = dango::detail::cond_var_registry_access::s_registry;

  c_registry.notify_exit();

  m_thread.join();
}

namespace
dango::detail
{
  DANGO_DECLARE_GLOBAL_EXTERN(dango::detail::cond_var_registry_thread const, s_cond_var_registry_thread)
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
  DANGO_EXPORT_ONLY static dango::detail::windows_timer_res_manager s_manager;
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
  dango::ubyte
  {
    ACTIVATED, DEACTIVATING, DEACTIVATED
  };
public:
  DANGO_EXPORT void activate(dango::timeout const&)noexcept;
  DANGO_EXPORT void deactivate(dango::timeout const&)noexcept;
  DANGO_EXPORT void notify_exit()noexcept;
  DANGO_EXPORT void thread_func()noexcept;
private:
  constexpr windows_timer_res_manager()noexcept;
  ~windows_timer_res_manager()noexcept = default;
private:
  DANGO_EXPORT auto wait()noexcept->bool;
  DANGO_EXPORT auto timed_wait(dango::timeout const&)noexcept->bool;
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

/*** windows_timer_res_daemon ***/

class
dango::
detail::
windows_timer_res_daemon
final
{
private:
  DANGO_EXPORT static auto start_thread()noexcept->dango::thread;
public:
  windows_timer_res_daemon()noexcept;
  ~windows_timer_res_daemon()noexcept;
private:
  dango::thread const m_thread;
public:
  DANGO_IMMOBILE(windows_timer_res_daemon)
};

inline
dango::
detail::
windows_timer_res_daemon::
windows_timer_res_daemon
()noexcept:
m_thread{ start_thread() }
{

}

inline
dango::
detail::
windows_timer_res_daemon::
~windows_timer_res_daemon
()noexcept
{
  constexpr auto& c_manager = dango::detail::windows_timer_res_access::s_manager;

  c_manager.notify_exit();

  m_thread.join();
}

namespace
dango::detail
{
  DANGO_DECLARE_GLOBAL_EXTERN(dango::detail::windows_timer_res_daemon const, s_windows_timer_res_daemon)
}

#endif // _WIN32

#endif // DANGO_CONCURRENT_HPP_INCLUDED

