#ifndef DANGO_MUTEX_HPP_INCLUDED
#define DANGO_MUTEX_HPP_INCLUDED

#include "dango-concurrent.hpp"

namespace
dango::detail
{
  class mutex_control;
  class mutex_control_dynamic;
  class mutex_storage;
  class mutex_base;

  class cond_var_control;
  class cond_var_control_dynamic;
  class cond_var_storage;
  class cond_var_base;
}

namespace
dango
{
  class mutex;
  class static_mutex;
  class shared_mutex;

  class cond_var;
  class static_cond_var;
  class shared_cond_var;
}

namespace
dango::detail
{
  struct
  sync_private
  final
  {
    friend dango::detail::mutex_base;
    friend dango::detail::cond_var_control;
    friend dango::detail::cond_var_base;
    friend dango::mutex;
    friend dango::shared_mutex;
    friend dango::cond_var;
    friend dango::shared_cond_var;
  private:
    struct
    tag
    final
    {
      DANGO_TAG_TYPE(tag)
    };
  public:
    DANGO_UNCONSTRUCTIBLE(sync_private)
  };

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
  constexpr virtual void increment()noexcept{ }
  constexpr virtual auto decrement()noexcept->bool{ return false; }
  constexpr auto storage()noexcept->dango::detail::mutex_storage&{ return m_storage; }
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
  explicit
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

class
dango::
detail::
mutex_base
{
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
public:
  constexpr auto get_control(dango::detail::sync_private::tag const)const noexcept->control_type*{ return m_control; }
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
private:
  using super_type = dango::crit_section;
public:
  explicit
  locker
  (dango::detail::sync_private::tag const, control_type& a_control)noexcept:
  super_type{ },
  m_mutex{ (void(a_control.init()), a_control.acquire()) }
  { }
  ~locker()noexcept{ if(is_locked()){ m_mutex->release(); } }
public:
  void
  unlock()noexcept
  {
    if(is_locked())
    {
      dango::exchange(m_mutex, dango::null)->release();
    }
  }
  auto is_locked()const noexcept->bool{ return !dango::is_null(m_mutex); }
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
private:
  using super_type = dango::crit_section;
public:
  explicit
  try_locker(dango::detail::sync_private::tag const, control_type& a_control)noexcept:
  super_type{ },
  m_mutex{ (void(a_control.init()), a_control.try_acquire()) }
  { }
  ~try_locker()noexcept{ if(is_locked()){ m_mutex->release(); } }
public:
  explicit operator bool()const{ return is_locked(); }
public:
  void
  unlock()noexcept
  {
    if(is_locked())
    {
      dango::exchange(m_mutex, dango::null)->release();
    }
  }
  auto is_locked()const noexcept->bool{ return !dango::is_null(m_mutex); }
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
  return locker{ dango::detail::sync_private::tag{ }, *m_control };
}

inline auto
dango::
detail::
mutex_base::
try_lock
()noexcept->try_locker
{
  return try_locker{ dango::detail::sync_private::tag{ }, *m_control };
}

/*** mutex ***/

class
dango::
mutex
final:
public dango::detail::mutex_base
{
private:
  using super_type = dango::detail::mutex_base;
  using control_type_dynamic = dango::detail::mutex_control_dynamic;
public:
  explicit mutex()dango_new_noexcept;
  ~mutex()noexcept;
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
  auto const a_control = get_control(dango::detail::sync_private::tag{ });

  if(a_control->decrement())
  {
    delete static_cast<control_type_dynamic*>(a_control);
  }
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
  using control_type_static = dango::detail::mutex_control;
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

/*** shared_mutex ***/

class
dango::
shared_mutex
final
{
private:
  using control_type = dango::detail::mutex_control;
  using control_type_dynamic = dango::detail::mutex_control_dynamic;
public:
  explicit constexpr
  shared_mutex
  (dango::detail::mutex_base& a_mutex)noexcept:
  m_control{ a_mutex.get_control(dango::detail::sync_private::tag{ }) }
  {
    m_control->increment();
  }

  explicit constexpr
  shared_mutex()noexcept:
  m_control{ dango::null }
  { }

  constexpr
  shared_mutex
  (dango::null_tag const)noexcept:
  m_control{ dango::null }
  { }

  constexpr
  shared_mutex
  (shared_mutex const& a_mut)noexcept:
  m_control{ a_mut.m_control }
  {
    if(m_control)
    {
      m_control->increment();
    }
  }

  constexpr
  shared_mutex
  (shared_mutex&& a_mut)noexcept:
  m_control{ dango::exchange(a_mut.m_control, dango::null) }
  { }

  constexpr
  ~shared_mutex()noexcept
  {
    if(m_control && m_control->decrement())
    {
      delete static_cast<control_type_dynamic*>(m_control);
    }
  }

  DANGO_DEFINE_NULL_SWAP_ASSIGN(shared_mutex, constexpr, true)
  DANGO_DEFINE_COPY_SWAP_ASSIGN(shared_mutex, constexpr, true)
  DANGO_DEFINE_MOVE_SWAP_ASSIGN(shared_mutex, constexpr, true)
  DANGO_DEFINE_NULLABLE_OPERATOR_BOOL(constexpr, true)
public:
  constexpr auto dango_operator_is_null()const noexcept->bool{ return dango::is_null(m_control); }
  constexpr auto dango_operator_equals(shared_mutex const& a_mut)const noexcept->bool{ return dango::equals(m_control, a_mut.m_control); }
  constexpr auto dango_operator_compare(shared_mutex const& a_mut)const noexcept->auto{ return dango::compare(m_control, a_mut.m_control); }
  constexpr void dango_operator_swap(shared_mutex& a_mut)& noexcept{ dango::swap(m_control, a_mut.m_control); }
public:
  [[nodiscard]] auto
  lock()const noexcept->auto
  {
    dango_assert_nonnull(*this);

    return dango::mutex_locker{ dango::detail::sync_private::tag{ }, *m_control };
  }

  [[nodiscard]] auto
  try_lock()const noexcept->auto
  {
    dango_assert_nonnull(*this);

    return dango::mutex_try_locker{ dango::detail::sync_private::tag{ }, *m_control };
  }
private:
  control_type* m_control;
};

#endif // DANGO_MUTEX_HPP_INCLUDED
