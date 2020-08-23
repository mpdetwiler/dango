#ifndef DANGO_CONCURRENT_HPP_INCLUDED
#define DANGO_CONCURRENT_HPP_INCLUDED

/*** tick_count ***/

namespace
dango
{
  using tick_count_type = dango::slong;
  using tick_count_pair = dango::pair<dango::tick_count_type, dango::tick_count_type>;

  DANGO_EXPORT auto tick_count_suspend_bias()noexcept->dango::tick_count_pair;

  auto tick_count()noexcept->dango::tick_count_type;
  auto tick_count_suspend_aware()noexcept->dango::tick_count_type;
  auto suspend_bias()noexcept->dango::tick_count_type;
}

inline auto
dango::
tick_count
()noexcept->dango::tick_count_type
{
  return dango::tick_count_suspend_bias().first();
}

inline auto
dango::
tick_count_suspend_aware
()noexcept->dango::tick_count_type
{
  auto const a_pair = dango::tick_count_suspend_bias();

  return a_pair.first() + a_pair.second();
}

inline auto
dango::
suspend_bias
()noexcept->dango::tick_count_type
{
  return dango::tick_count_suspend_bias().second();
}

/*** timeout ***/

namespace
dango
{
  enum class timeout_flag:dango::uint;

  constexpr auto
  operator |
  (dango::timeout_flag const a_lhs, dango::timeout_flag const a_rhs)noexcept->dango::timeout_flag
  {
    return dango::timeout_flag{ dango::uint(a_lhs) | dango::uint(a_rhs) };
  }

  constexpr auto
  operator &
  (dango::timeout_flag const a_lhs, dango::timeout_flag const a_rhs)noexcept->dango::timeout_flag
  {
    return dango::timeout_flag{ dango::uint(a_lhs) & dango::uint(a_rhs) };
  }

  enum class
  timeout_flag:dango::uint
  {
    ZERO = dango::uint(0),
    DEFAULT = ZERO,
    HIGH_RES = dango::uint(1 << 0),
    SUSPEND_AWARE = dango::uint(1 << 1),
    valid_combo_00 = DEFAULT,
    valid_combo_01 = HIGH_RES,
    valid_combo_02 = SUSPEND_AWARE,
    valid_combo_03 = HIGH_RES | SUSPEND_AWARE
  };

  namespace
  timeout_flags
  {
    inline constexpr auto const DEFAULT = dango::timeout_flag::DEFAULT;
    inline constexpr auto const HIGH_RES = dango::timeout_flag::HIGH_RES;
    inline constexpr auto const SUSPEND_AWARE = dango::timeout_flag::SUSPEND_AWARE;
  }

  class timeout;
}

namespace
dango::detail
{
  class timeout_base;
}

class
dango::
detail::
timeout_base
{
public:
  using value_type = dango::tick_count_type;
protected:
  explicit constexpr timeout_base()noexcept;
public:
  virtual ~timeout_base()noexcept = default;
public:
  virtual auto is_high_res()const noexcept->bool = 0;
  virtual auto is_suspend_aware()const noexcept->bool = 0;
  auto has_expired()const noexcept->bool;
  auto remaining()const noexcept->value_type;
  auto get()const noexcept->value_type;
  void set(value_type)noexcept;
  void set_rel(value_type)noexcept;
  void add(value_type)noexcept;
private:
  virtual auto this_tick_count()const noexcept->value_type = 0;
private:
  value_type m_timeout;
public:
  DANGO_IMMOBILE(timeout_base)
};

constexpr
dango::
detail::
timeout_base::
timeout_base
()noexcept:
m_timeout{ value_type(0) }
{

}

inline auto
dango::
detail::
timeout_base::
has_expired
()const noexcept->bool
{
  return this_tick_count() >= m_timeout;
}

inline auto
dango::
detail::
timeout_base::
remaining
()const noexcept->value_type
{
  auto const a_now = this_tick_count();

  if(a_now >= m_timeout)
  {
    return value_type(0);
  }

  return m_timeout - a_now;
}

inline auto
dango::
detail::
timeout_base::
get
()const noexcept->value_type
{
  return m_timeout;
}

inline void
dango::
detail::
timeout_base::
set
(value_type const a_abs)noexcept
{
  m_timeout = a_abs;
}

inline void
dango::
detail::
timeout_base::
set_rel
(value_type const a_rel)noexcept
{
  auto const a_now = this_tick_count();

  m_timeout = dango::integer::safe_add(a_now, a_rel);
}

inline void
dango::
detail::
timeout_base::
add
(value_type const a_add)noexcept
{
  m_timeout = dango::integer::safe_add(m_timeout, a_add);
}

/*** timeout_impl ***/

namespace
dango::detail
{
  template
  <dango::timeout_flag tp_flags>
  class timeout_impl;
}

template
<dango::timeout_flag tp_flags>
class
dango::
detail::
timeout_impl
final:
public dango::detail::timeout_base
{
private:
  using super_type = dango::detail::timeout_base;
private:
  static inline constexpr auto const c_high_res = bool(tp_flags & dango::timeout_flag::HIGH_RES);
  static inline constexpr auto const c_suspend_aware = bool(tp_flags & dango::timeout_flag::SUSPEND_AWARE);
private:
  static auto
  this_tick_count_impl()noexcept->value_type
  {
    if constexpr(c_suspend_aware)
    {
      return dango::tick_count_suspend_aware();
    }
    else
    {
      return dango::tick_count();
    }
  }
public:
  explicit constexpr timeout_impl()noexcept:super_type{ }{ }
  ~timeout_impl()noexcept override = default;
  auto is_high_res()const noexcept->bool override{ return c_high_res; }
  auto is_suspend_aware()const noexcept->bool override{ return c_suspend_aware; }
private:
  auto this_tick_count()const noexcept->value_type override{ return this_tick_count_impl(); }
public:
  DANGO_IMMOBILE(timeout_impl)
};

/*** timeout ***/

namespace
dango::detail
{
  using timeout_storage =
    dango::aligned_union
    <
      dango::detail::timeout_impl<dango::timeout_flag::valid_combo_00>,
      dango::detail::timeout_impl<dango::timeout_flag::valid_combo_01>,
      dango::detail::timeout_impl<dango::timeout_flag::valid_combo_02>,
      dango::detail::timeout_impl<dango::timeout_flag::valid_combo_03>
    >;
}

class
dango::
timeout
final
{
public:
  using value_type = dango::detail::timeout_base::value_type;
private:
  template
  <dango::timeout_flag tp_flags>
  using timeout_type = dango::detail::timeout_impl<tp_flags>;
private:
  static auto construct_help(void*, dango::timeout_flag)noexcept->dango::detail::timeout_base*;
public:
  static auto make(value_type, dango::timeout_flag)noexcept->timeout;
  static auto make_rel(value_type, dango::timeout_flag)noexcept->timeout;
  static auto make(value_type const a_value)noexcept->timeout{ return make(a_value, dango::timeout_flag::DEFAULT); }
  static auto make_rel(value_type const a_value)noexcept->timeout{ return make_rel(a_value, dango::timeout_flag::DEFAULT); }
private:
  explicit timeout(value_type, bool, dango::timeout_flag)noexcept;
public:
  ~timeout()noexcept{ dango::destructor(m_timeout); }
public:
  auto is_high_res()const noexcept->bool{ return m_timeout->is_high_res(); }
  auto is_suspend_aware()const noexcept->bool{ return m_timeout->is_suspend_aware(); }
  auto has_expired()const noexcept->bool{ return m_timeout->has_expired(); }
  auto remaining()const noexcept->value_type{ return m_timeout->remaining(); }
  auto get()const noexcept->value_type{ return m_timeout->get(); }
  void set(value_type const a_abs)noexcept{ m_timeout->set(a_abs); }
  void set_rel(value_type const a_rel)noexcept{ m_timeout->set_rel(a_rel); }
  void add(value_type const a_add)noexcept{ m_timeout->add(a_add); }
private:
  dango::detail::timeout_storage m_storage;
  dango::detail::timeout_base* const m_timeout;
public:
  DANGO_DELETE_DEFAULT(timeout)
  DANGO_IMMOBILE(timeout)
};

inline auto
dango::
timeout::
construct_help
(void* const a_storage, dango::timeout_flag const a_flags)noexcept->dango::detail::timeout_base*
{
  using dango::timeout_flags::DEFAULT;
  using dango::timeout_flags::HIGH_RES;
  using dango::timeout_flags::SUSPEND_AWARE;

  switch(a_flags)
  {
    case DEFAULT:
    {
      using type = timeout_type<DEFAULT>;

      return dango_placement_new(a_storage, type, { });
    }
    break;
    case HIGH_RES:
    {
      using type = timeout_type<HIGH_RES>;

      return dango_placement_new(a_storage, type, { });
    }
    break;
    case SUSPEND_AWARE:
    {
      using type = timeout_type<SUSPEND_AWARE>;

      return dango_placement_new(a_storage, type, { });
    }
    break;
    case HIGH_RES | SUSPEND_AWARE:
    {
      using type = timeout_type<HIGH_RES | SUSPEND_AWARE>;

      return dango_placement_new(a_storage, type, { });
    }
    break;
    default: break;
  }

  dango_unreachable;
}

inline auto
dango::
timeout::
make
(value_type const a_value, dango::timeout_flag const a_flags)noexcept->timeout
{
  return timeout{ a_value, false, a_flags };
}

inline auto
dango::
timeout::
make_rel
(value_type const a_value, dango::timeout_flag const a_flags)noexcept->timeout
{
  return timeout{ a_value, true, a_flags };
}

inline
dango::
timeout::
timeout
(value_type const a_value, bool const a_rel, dango::timeout_flag const a_flags)noexcept:
m_storage{ },
m_timeout{ construct_help(m_storage.get(), a_flags) }
{
  dango_assert(m_timeout != dango::null);

  if(a_rel)
  {
    m_timeout->set_rel(a_value);
  }
  else
  {
    m_timeout->set(a_value);
  }
}

/*** mutex_base ***/

namespace
dango::detail
{
  class mutex_control;
  class mutex_control_dynamic;

  using sync_primitive_storage =
    dango::aligned_storage<sizeof(dango::cache_align_type), dango::usize(1) * alignof(dango::cache_align_type)>;

  class mutex_storage;
}

class
dango::
detail::
mutex_storage
final:
public dango::detail::sync_primitive_storage
{
private:
  using super_type = dango::detail::sync_primitive_storage;
public:
  using super_type::super_type;
};

class alignas(dango::cache_align_type)
dango::
detail::
mutex_control
{
public:
  static auto operator new(dango::usize)noexcept->void* = delete;
  static void operator delete(void*, dango::usize)noexcept = delete;
public:
  explicit constexpr
  mutex_control()noexcept:
  m_init{ },
  m_storage{ }
  { }
  ~mutex_control()noexcept = default;
public:
  void init()noexcept{ m_init.exec([this]()noexcept->void{ initialize(); }); }
  virtual void increment()noexcept{ }
  virtual auto decrement()noexcept->bool{ return false; }
  DANGO_EXPORT auto acquire()noexcept->mutex_control*;
  DANGO_EXPORT auto try_acquire()noexcept->mutex_control*;
  DANGO_EXPORT void release()noexcept;
  auto storage()noexcept->dango::detail::mutex_storage&;
protected:
  DANGO_EXPORT void destroy()noexcept;
private:
  DANGO_EXPORT void initialize()noexcept;
private:
  dango::exec_once m_init;
  dango::detail::mutex_storage m_storage;
public:
  DANGO_IMMOBILE(mutex_control)
};

class
dango::
detail::
mutex_control_dynamic
final:
public dango::detail::mutex_control
{
private:
  using super_type = dango::detail::mutex_control;
public:
  static auto
  operator new(dango::usize const a_size)dango_new_noexcept->void*
  {
    return dango::operator_new(a_size, alignof(mutex_control_dynamic));
  }

  static void
  operator delete(void* const a_ptr, dango::usize const a_size)noexcept
  {
    dango::operator_delete(a_ptr, a_size, alignof(mutex_control_dynamic));
  }
public:
  explicit constexpr
  mutex_control_dynamic()noexcept:
  super_type{ },
  m_ref_count{ }
  { init(); }
  ~mutex_control_dynamic()noexcept{ destroy(); }
public:
  void increment()noexcept override{ m_ref_count.increment(); }
  auto decrement()noexcept->bool override{ return m_ref_count.decrement(); }
private:
  dango::atomic_ref_count m_ref_count;
public:
  DANGO_IMMOBILE(mutex_control_dynamic)
};

namespace
dango::detail
{
  class mutex_base;
  class cond_var_control;

  auto mutex_get_storage(dango::detail::mutex_base const&)noexcept->dango::detail::mutex_storage&;
}

namespace
dango
{
  class mutex;
  class static_mutex;
}

class
dango::
detail::
mutex_base
{
  friend auto dango::detail::mutex_get_storage(dango::detail::mutex_base const&)noexcept->dango::detail::mutex_storage&;
public:
  class locker;
  class try_locker;
protected:
  using control_type = dango::detail::mutex_control;
protected:
  explicit constexpr
  mutex_base(control_type* const a_control)noexcept:
  m_control{ a_control }
  { }
  ~mutex_base()noexcept = default;
public:
  [[nodiscard]] auto lock()noexcept->locker;
  [[nodiscard]] auto try_lock()noexcept->try_locker;
  auto reference()const noexcept->dango::mutex;
protected:
  constexpr auto get_control()const noexcept->control_type*{ return m_control; }
private:
  control_type* const m_control;
public:
  DANGO_DELETE_DEFAULT(mutex_base)
  DANGO_IMMOBILE(mutex_base)
};

namespace
dango
{
  using mutex_locker = dango::detail::mutex_base::locker;
  using mutex_try_locker = dango::detail::mutex_base::try_locker;
}

class
dango::
detail::
mutex_base::
locker:
public dango::crit_section
{
  friend auto dango::detail::mutex_base::lock()noexcept->locker;
private:
  using super_type = dango::crit_section;
protected:
  explicit
  locker(mutex_base* const a_mutex)noexcept:
  super_type{ },
  m_mutex{ (void(a_mutex->get_control()->init()), a_mutex->get_control()->acquire()) }
  { }
public:
  ~locker()noexcept{ m_mutex->release(); }
private:
  control_type* const m_mutex;
public:
  DANGO_DELETE_DEFAULT(locker)
  DANGO_IMMOBILE(locker)
};

class
dango::
detail::
mutex_base::
try_locker:
public dango::crit_section
{
  friend auto dango::detail::mutex_base::try_lock()noexcept->try_locker;
private:
  using super_type = dango::crit_section;
protected:
  explicit
  try_locker(mutex_base* const a_mutex)noexcept:
  super_type{ },
  m_mutex{ (void(a_mutex->get_control()->init()), a_mutex->get_control()->try_acquire()) }
  { }
public:
  ~try_locker()noexcept{ if(m_mutex){ m_mutex->release(); } }
public:
  explicit operator bool()const{ return m_mutex != dango::null; }
private:
  control_type* const m_mutex;
public:
  DANGO_DELETE_DEFAULT(try_locker)
  DANGO_IMMOBILE(try_locker)
};

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

class
dango::
mutex
final:
public dango::detail::mutex_base
{
  friend auto dango::detail::mutex_base::reference()const noexcept->dango::mutex;
private:
  using super_type = dango::detail::mutex_base;
  using control_type_dynamic = dango::detail::mutex_control_dynamic;
  struct increment_tag{ DANGO_TAG_TYPE(increment_tag) };
public:
  explicit mutex()dango_new_noexcept;
  ~mutex()noexcept;
private:
  explicit mutex(increment_tag, control_type*)noexcept;
public:
  DANGO_IMMOBILE(mutex)
};

inline
dango::
mutex::
mutex
()dango_new_noexcept:
super_type{ new control_type_dynamic{ } }
{

}

inline
dango::
mutex::
~mutex
()noexcept
{
  auto const a_control = get_control();

  if(a_control->decrement())
  {
    delete static_cast<control_type_dynamic*>(a_control);
  }
}

inline
dango::
mutex::
mutex
(increment_tag const, control_type* const a_control)noexcept:
super_type{ a_control }
{
  a_control->increment();
}

inline auto
dango::
detail::
mutex_base::
reference
()const noexcept->dango::mutex
{
  return dango::mutex{ dango::mutex::increment_tag{ }, get_control() };
}

/*** static_mutex ***/

class
dango::
static_mutex
final:
public dango::detail::mutex_base
{
private:
  using super_type = dango::detail::mutex_base;
  using control_type_static = control_type;
public:
  explicit constexpr static_mutex()noexcept;
  ~static_mutex()noexcept = default;
private:
  control_type_static m_control_storage;
public:
  DANGO_IMMOBILE(static_mutex)
};

constexpr
dango::
static_mutex::
static_mutex
()noexcept:
super_type{ &m_control_storage },
m_control_storage{ }
{

}

/*** cond_var_base ***/

namespace
dango::detail
{
  class cond_var_control;
  class cond_var_control_dynamic;
  class cond_var_storage;
}

namespace
dango
{
  class cond_var;
}

class
dango::
detail::
cond_var_storage
final:
public dango::detail::sync_primitive_storage
{
private:
  using super_type = dango::detail::sync_primitive_storage;
public:
  using super_type::super_type;
};

class alignas(dango::cache_align_type)
dango::
detail::
cond_var_control:
public dango::intrusive_list_elem<cond_var_control>
{
private:
  using super_type = dango::intrusive_list_elem<cond_var_control>;
  using count_type = dango::usize;
  class cond_var_impl;
protected:
  using mutex_type = dango::detail::mutex_base;
public:
  static auto operator new(dango::usize)noexcept->void* = delete;
  static void operator delete(void*, dango::usize)noexcept = delete;
public:
  explicit constexpr
  cond_var_control
  (mutex_type* const a_mutex)noexcept:
  super_type{ },
  m_suspend_aware_count{ count_type(0) },
  m_init{ },
  m_mutex{ a_mutex },
  m_storage{ }
  { }
  ~cond_var_control()noexcept = default;
public:
  void init()noexcept{ m_init.exec([this]()noexcept->void{ initialize(); }); }
  virtual void increment()noexcept{ }
  virtual auto decrement()noexcept->bool{ return false; }
  auto mutex_ref()const noexcept->mutex_type&{ return *m_mutex; }
  DANGO_EXPORT void wait()noexcept;
  DANGO_EXPORT void wait(dango::timeout const&)noexcept;
  DANGO_EXPORT void notify()noexcept;
  DANGO_EXPORT void notify_all()noexcept;
  auto storage()noexcept->dango::detail::cond_var_storage&;
  auto increment_suspend_aware_count(dango::crit_section const&)noexcept->bool;
  auto decrement_suspend_aware_count(dango::crit_section const&)noexcept->bool;
  auto make_reference()noexcept->dango::cond_var;
protected:
  DANGO_EXPORT void destroy()noexcept;
private:
  DANGO_EXPORT void initialize()noexcept;
private:
  count_type m_suspend_aware_count;
  alignas(dango::cache_align_type) dango::exec_once m_init;
  mutex_type* const m_mutex;
  dango::detail::cond_var_storage m_storage;
public:
  DANGO_DELETE_DEFAULT(cond_var_control)
  DANGO_IMMOBILE(cond_var_control)
};

class
dango::
detail::
cond_var_control_dynamic
final:
public dango::detail::cond_var_control
{
private:
  using super_type = dango::detail::cond_var_control;
public:
  static auto
  operator new(dango::usize const a_size)dango_new_noexcept->void*
  {
    return dango::operator_new(a_size, alignof(cond_var_control_dynamic));
  }

  static void
  operator delete(void* const a_ptr, dango::usize const a_size)noexcept
  {
    dango::operator_delete(a_ptr, a_size, alignof(cond_var_control_dynamic));
  }
public:
  explicit constexpr
  cond_var_control_dynamic
  (mutex_type* const a_mutex)noexcept:
  super_type{ a_mutex },
  m_ref_count{ }
  { init(); }
  ~cond_var_control_dynamic()noexcept{ destroy(); }
public:
  void increment()noexcept override{ m_ref_count.increment(); }
  auto decrement()noexcept->bool override{ return m_ref_count.decrement(); }
private:
  dango::atomic_ref_count m_ref_count;
public:
  DANGO_IMMOBILE(cond_var_control_dynamic)
};

namespace
dango::detail
{
  class cond_var_base;
  class cond_var_registry;
}

namespace
dango
{
  class static_cond_var;
}

class
dango::
detail::
cond_var_base
{
protected:
  using control_type = dango::detail::cond_var_control;
  using mutex_type = dango::detail::mutex_base;
public:
  class locker;
  class try_locker;
protected:
  explicit constexpr
  cond_var_base(control_type* const a_control)noexcept:
  m_control{ a_control }
  { }
  ~cond_var_base()noexcept = default;
public:
  [[nodiscard]] auto lock()noexcept->locker;
  [[nodiscard]] auto try_lock()noexcept->try_locker;
  auto reference()const noexcept->dango::cond_var;
protected:
  auto get_control()const noexcept->control_type*{ return m_control; }
private:
  control_type* const m_control;
public:
  DANGO_IMMOBILE(cond_var_base)
};

namespace
dango
{
  using cond_var_locker = dango::detail::cond_var_base::locker;
  using cond_var_try_locker = dango::detail::cond_var_base::try_locker;
}

class
dango::
detail::
cond_var_base::
locker
final:
public dango::mutex_locker
{
  friend auto dango::detail::cond_var_base::lock()noexcept->locker;
private:
  using super_type = dango::mutex_locker;
private:
  explicit
  locker(cond_var_base* const a_cond)noexcept:
  super_type{ &a_cond->get_control()->mutex_ref() },
  m_cond{ a_cond->get_control() }
  { m_cond->init(); }
public:
  ~locker()noexcept = default;
public:
  void notify()const noexcept{ m_cond->notify(); }
  void notify_all()const noexcept{ m_cond->notify_all(); }
  void wait()const noexcept{ m_cond->wait(); }
  void wait(dango::timeout const& a_timeout)const noexcept{ m_cond->wait(a_timeout); }
private:
  control_type* const m_cond;
public:
  DANGO_DELETE_DEFAULT(locker)
  DANGO_IMMOBILE(locker)
};

class
dango::
detail::
cond_var_base::
try_locker
final:
public dango::mutex_try_locker
{
  friend auto dango::detail::cond_var_base::try_lock()noexcept->try_locker;
private:
  using super_type = dango::mutex_try_locker;
private:
  explicit
  try_locker(cond_var_base* const a_cond)noexcept:
  super_type{ &a_cond->get_control()->mutex_ref() },
  m_cond{ a_cond->get_control() }
  { m_cond->init(); }
public:
  ~try_locker()noexcept = default;
public:
  void notify()const noexcept{ m_cond->notify(); }
  void notify_all()const noexcept{ m_cond->notify_all(); }
  void wait()const noexcept{ m_cond->wait(); }
  void wait(dango::timeout const& a_timeout)const noexcept{ m_cond->wait(a_timeout); }
private:
  control_type* const m_cond;
public:
  DANGO_DELETE_DEFAULT(try_locker)
  DANGO_IMMOBILE(try_locker)
};

inline auto
dango::
detail::
cond_var_base::
lock
()noexcept->locker
{
  return locker{ this };
}

inline auto
dango::
detail::
cond_var_base::
try_lock
()noexcept->try_locker
{
  return try_locker{ this };
}

/*** cond_var ***/

class
dango::
cond_var
final:
public dango::detail::cond_var_base
{
  friend auto dango::detail::cond_var_control::make_reference()noexcept->dango::cond_var;
  friend auto dango::detail::cond_var_base::reference()const noexcept->dango::cond_var;
private:
  using super_type = dango::detail::cond_var_base;
  using control_type_dynamic = dango::detail::cond_var_control_dynamic;
  struct increment_tag{ DANGO_TAG_TYPE(increment_tag) };
public:
  explicit cond_var(mutex_type&)dango_new_noexcept;
  ~cond_var()noexcept;
private:
  explicit cond_var(increment_tag, control_type*)noexcept;
private:
  dango::mutex m_mutex;
public:
  DANGO_DELETE_DEFAULT(cond_var)
  DANGO_IMMOBILE(cond_var)
};

inline
dango::
cond_var::
cond_var(mutex_type& a_mutex)dango_new_noexcept:
super_type{ new control_type_dynamic{ &a_mutex } },
m_mutex{ a_mutex.reference() }
{

}

inline
dango::
cond_var::
~cond_var
()noexcept
{
  auto const a_control = get_control();

  if(a_control->decrement())
  {
    delete static_cast<control_type_dynamic*>(a_control);
  }
}

inline
dango::
cond_var::
cond_var(increment_tag const, control_type* const a_control)noexcept:
super_type{ a_control },
m_mutex{ a_control->mutex_ref().reference() }
{
  a_control->increment();
}

inline auto
dango::
detail::
cond_var_base::
reference
()const noexcept->dango::cond_var
{
  return dango::cond_var{ dango::cond_var::increment_tag{ }, m_control };
}

/*** static_cond_var ***/

class
dango::
static_cond_var
final:
public dango::detail::cond_var_base
{
private:
  using super_type = dango::detail::cond_var_base;
  using control_type_static = dango::detail::cond_var_control;
  using mutex_type_static = dango::static_mutex;
public:
  explicit constexpr static_cond_var(mutex_type_static&)noexcept;
  explicit constexpr static_cond_var(mutex_type&)noexcept = delete;
  ~static_cond_var()noexcept = default;
private:
  control_type_static m_control_storage;
public:
  DANGO_DELETE_DEFAULT(static_cond_var)
  DANGO_IMMOBILE(static_cond_var)
};

constexpr
dango::
static_cond_var::
static_cond_var
(mutex_type_static& a_mutex)noexcept:
super_type{ &m_control_storage },
m_control_storage{ &a_mutex }
{

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
    none = dango::ptr_as_uint(dango::null)
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
    (dango::in_exception_safe_order<tp_func, tp_args...> || (dango::is_copy_constructible<dango::decay<tp_func>> && ... && dango::is_copy_constructible<dango::decay<tp_args>>)) &&
    dango::is_callable_ret<void, dango::decay<tp_func>&, dango::tuple_get_type<dango::tuple_model&, dango::decay<tp_args>>...>
  )
  static auto
  create(bool, tp_func&&, tp_args&&...)noexcept(false)->dango::thread;

  template
  <typename tp_func, typename... tp_args>
  requires(requires{ { dango::thread::create(false, dango::declval<tp_func>(), dango::declval<tp_args>()...) }->dango::is_same<dango::thread>; })
  static auto
  create(tp_func&& a_thread_func, tp_args&&... a_thread_args)noexcept(false)->dango::thread
  {
    return dango::thread::create(false, dango::forward<tp_func>(a_thread_func), dango::forward<tp_args>(a_thread_args)...);
  }

  template
  <typename tp_func, typename... tp_args>
  requires(requires{ { dango::thread::create(true, dango::declval<tp_func>(), dango::declval<tp_args>()...) }->dango::is_same<dango::thread>; })
  static auto
  create_daemon(tp_func&& a_thread_func, tp_args&&... a_thread_args)noexcept(false)->dango::thread
  {
    return dango::thread::create(true, dango::forward<tp_func>(a_thread_func), dango::forward<tp_args>(a_thread_args)...);
  }

  DANGO_EXPORT static void sleep(dango::timeout const&)noexcept;
  static void sleep_rel(dango::tick_count_type)noexcept;
  static void sleep_rel(dango::tick_count_type, dango::timeout_flag)noexcept;
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
  auto get_ID()const noexcept->dango::thread_ID;
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
  explicit control_block(bool, dango::thread_ID)dango_new_noexcept;
  ~control_block()noexcept = default;
public:
  constexpr auto is_daemon()const noexcept->bool{ return m_daemon; }
  void increment()noexcept{ m_ref_count.increment(); }
  auto decrement()noexcept->bool{ return m_ref_count.decrement(); }
  void wait()noexcept;
  void wait(dango::timeout const&)noexcept;
  void notify_all()noexcept;
  auto get_ID()const noexcept->dango::thread_ID;
  auto is_alive()const noexcept->bool;
private:
  auto non_null_ID(dango::crit_section const&)const noexcept->bool{ return m_thread_ID != dango::thread_ID::none; }
private:
  dango::atomic_ref_count m_ref_count;
  bool const m_daemon;
  mutable dango::mutex m_mutex;
  mutable dango::cond_var m_cond;
  dango::thread_ID m_thread_ID;
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

inline
dango::
thread::
control_block::
control_block
(bool const a_daemon, dango::thread_ID const a_thread_ID)dango_new_noexcept:
super_type{ },
m_ref_count{ },
m_daemon{ a_daemon },
m_mutex{ },
m_cond{ m_mutex },
m_thread_ID{ a_thread_ID },
m_waiter_count{ dango::usize(0) }
{

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
    while(non_null_ID(a_crit))
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
    while(non_null_ID(a_crit) && !a_timeout.has_expired())
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
    m_thread_ID = dango::thread_ID::none;

    if(m_waiter_count != dango::usize(0))
    {
      a_crit.notify_all();
    }
  }
}

inline auto
dango::
thread::
control_block::
get_ID
()const noexcept->dango::thread_ID
{
  dango_crit(m_mutex)
  {
    return m_thread_ID;
  }
}

inline auto
dango::
thread::
control_block::
is_alive
()const noexcept->bool
{
  dango_crit_full(m_mutex, a_crit)
  {
    return non_null_ID(a_crit);
  }
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
  dango::thread_yield_soft();
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
(dango::tick_count_type const a_rel)noexcept
{
  auto const a_timeout = dango::timeout::make_rel(a_rel);

  thread::sleep(a_timeout);
}

inline void
dango::
thread::
sleep_rel
(dango::tick_count_type const a_rel, dango::timeout_flag const a_flags)noexcept
{
  auto const a_timeout = dango::timeout::make_rel(a_rel, a_flags);

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

inline auto
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
  (dango::in_exception_safe_order<tp_func, tp_args...> || (dango::is_copy_constructible<dango::decay<tp_func>> && ... && dango::is_copy_constructible<dango::decay<tp_args>>)) &&
  dango::is_callable_ret<void, dango::decay<tp_func>&, dango::tuple_get_type<dango::tuple_model&, dango::decay<tp_args>>...>
)
auto
dango::
thread::
create
(bool const a_daemon, tp_func&& a_thread_func, tp_args&&... a_thread_args)noexcept(false)->dango::thread
{
  using dango::mem_order::acquire;
  using dango::mem_order::release;

  using run_type = runnable<dango::decay<tp_func>, dango::decay<tp_args>...>;

  auto const a_runnable =
  [](auto& a_func, auto&... a_args)noexcept(false)->run_type*
  {
    if constexpr(dango::in_exception_safe_order<tp_func, tp_args...>)
    {
      return new run_type{ dango::forward_if_noexcept<tp_func>(a_func), dango::forward_if_noexcept<tp_args>(a_args)... };
    }
    else
    {
      return new run_type{ dango::as_const(a_func), dango::as_const(a_args)... };
    }
  }(a_thread_func, a_thread_args...);

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

  start_thread(&thread_start_address<decltype(a_func)>, &a_func);

  dango::busy_wait_while([&a_starting]()noexcept->bool{ return a_starting.load<acquire>(); }, dango::uint(128));

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

/*** init things before main ***/

#ifndef DANGO_BUILDING_SHARED_LIB
namespace
dango::detail
{
  [[maybe_unused]] inline bool const
  s_concurrent_init_byte
  {(
    void(dango::tick_count_suspend_bias()),
    void(dango::thread::self()),
    false
  )};
}
#endif

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
  friend dango::detail::cond_var_control;
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
  using cond_type = dango::detail::cond_var_control;
  using list_type = dango::intrusive_list<cond_type>;
private:
  static void remove(cond_type*)noexcept;
  static auto bias_okay(dango::tick_count_type&)noexcept->bool;
public:
  void regist(cond_type*, dango::timeout const&)noexcept;
  void unregist(cond_type*, dango::timeout const&)noexcept;
  void notify_exit()noexcept;
  void thread_func()noexcept;
private:
  constexpr cond_var_registry()noexcept;
  ~cond_var_registry()noexcept = default;
private:
  void add(cond_type*)noexcept;
  auto wait_empty()noexcept->bool;
  auto poll_bias(dango::tick_count_type&, dango::timeout&)noexcept->bool;
  auto pop_internal()noexcept->bool;
private:
  dango::static_mutex m_mutex;
  dango::static_cond_var m_cond;
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
  DANGO_EXPORT static void stop_thread(dango::thread const&)noexcept;
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
  stop_thread(m_thread);
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
  friend dango::detail::cond_var_control;
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
  void activate(dango::timeout const&)noexcept;
  void deactivate(dango::timeout const&)noexcept;
  void notify_exit()noexcept;
  void thread_func()noexcept;
private:
  constexpr windows_timer_res_manager()noexcept;
  ~windows_timer_res_manager()noexcept = default;
private:
  auto wait()noexcept->bool;
  auto timed_wait(dango::timeout const&)noexcept->bool;
private:
  dango::static_mutex m_mutex;
  dango::static_cond_var m_cond;
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
  DANGO_EXPORT static void stop_thread(dango::thread const&)noexcept;
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
  stop_thread(m_thread);
}

namespace
dango::detail
{
  DANGO_DECLARE_GLOBAL_EXTERN(dango::detail::windows_timer_res_daemon const, s_windows_timer_res_daemon)
}

#endif // _WIN32

#endif // DANGO_CONCURRENT_HPP_INCLUDED

