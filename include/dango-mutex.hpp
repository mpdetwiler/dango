#ifndef DANGO_MUTEX_HPP_INCLUDED
#define DANGO_MUTEX_HPP_INCLUDED

#include "dango-concurrent.hpp"

/*** mutex_base ***/

namespace
dango::detail
{
  class mutex_control;
  class mutex_control_dynamic;
  class mutex_storage;

  using sync_primitive_storage =
    dango::aligned_union<dango::cache_align_type>;
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
  DANGO_DELETE_CLASS_OPERATOR_NEW_DELETE
public:
  explicit constexpr
  mutex_control()noexcept:
  m_init{ },
  m_storage{ }
  { }
  constexpr ~mutex_control()noexcept = default;
private:
  DANGO_EXPORT void initialize()noexcept;
protected:
  DANGO_EXPORT void destroy()noexcept;
public:
  DANGO_EXPORT auto acquire()noexcept->mutex_control*;
  DANGO_EXPORT auto try_acquire()noexcept->mutex_control*;
  DANGO_EXPORT void release()noexcept;
public:
  void init()noexcept{ m_init.exec([this]()noexcept->void{ initialize(); }); }
  virtual void increment()noexcept{ }
  virtual auto decrement()noexcept->bool{ return false; }
  auto storage()noexcept->dango::detail::mutex_storage&{ return m_storage; }
private:
  dango::exec_once m_init;
  dango::detail::mutex_storage m_storage;
public:
  DANGO_UNMOVEABLE(mutex_control)
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
  DANGO_DEFINE_CLASS_OPERATOR_NEW_DELETE(mutex_control_dynamic)
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
  DANGO_UNMOVEABLE(mutex_control_dynamic)
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
  constexpr ~mutex_base()noexcept = default;
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
  DANGO_UNMOVEABLE(mutex_base)
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
  ~locker()noexcept{ unlock(); }
public:
  void
  unlock()noexcept
  {
    if(m_mutex)
    {
      m_mutex->release();
      m_mutex = dango::null;
    }
  }
private:
  control_type* m_mutex;
public:
  DANGO_DELETE_DEFAULT(locker)
  DANGO_UNMOVEABLE(locker)
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
  ~try_locker()noexcept{ unlock(); }
public:
  void
  unlock()noexcept
  {
    if(m_mutex)
    {
      m_mutex->release();
      m_mutex = dango::null;
    }
  }
public:
  explicit operator bool()const{ return m_mutex != dango::null; }
private:
  control_type* m_mutex;
public:
  DANGO_DELETE_DEFAULT(try_locker)
  DANGO_UNMOVEABLE(try_locker)
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
  DANGO_UNMOVEABLE(mutex)
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
  constexpr ~static_mutex()noexcept = default;
private:
  control_type_static m_control_storage;
public:
  DANGO_UNMOVEABLE(static_mutex)
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

#endif // DANGO_MUTEX_HPP_INCLUDED
