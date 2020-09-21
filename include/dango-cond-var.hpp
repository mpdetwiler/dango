#ifndef DANGO_COND_VAR_HPP_INCLUDED
#define DANGO_COND_VAR_HPP_INCLUDED

#include "dango-concurrent.hpp"
#include "dango-intrusive-list.hpp"
#include "dango-mutex.hpp"
#include "dango-thread.hpp"

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
  class shared_cond_var;
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

class
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
  DANGO_DELETE_CLASS_OPERATOR_NEW_DELETE
public:
  explicit constexpr
  cond_var_control
  (mutex_type& a_mutex)noexcept:
  super_type{ },
  m_suspend_aware_count{ count_type(0) },
  m_init{ },
  m_mutex{ &a_mutex },
  m_storage{ }
  { }
  constexpr ~cond_var_control()noexcept = default;
private:
  DANGO_EXPORT void initialize()noexcept;
protected:
  DANGO_EXPORT void destroy()noexcept;
public:
  DANGO_EXPORT void wait()noexcept;
  DANGO_EXPORT void wait(dango::timeout const&)noexcept;
  DANGO_EXPORT void notify()noexcept;
  DANGO_EXPORT void notify_all()noexcept;
public:
  void init()noexcept{ m_init.exec([this]()noexcept->void{ initialize(); }); }
  constexpr virtual void increment()noexcept{ }
  constexpr virtual auto decrement()noexcept->bool{ return false; }
  constexpr auto mutex_ref()const noexcept->mutex_type&{ return *m_mutex; }
  constexpr auto storage()noexcept->dango::detail::cond_var_storage&{ return m_storage; }
  auto increment_suspend_aware_count(dango::crit_section const&)noexcept->bool;
  auto decrement_suspend_aware_count(dango::crit_section const&)noexcept->bool;
  auto make_shared()noexcept->dango::shared_cond_var;
private:
  count_type m_suspend_aware_count;
  DANGO_CACHE_LINE_START
  dango::exec_once m_init;
  DANGO_CACHE_LINE_START
  mutex_type* const m_mutex;
  dango::detail::cond_var_storage m_storage;
public:
  DANGO_DELETE_DEFAULT(cond_var_control)
  DANGO_UNMOVEABLE(cond_var_control)
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
  DANGO_DEFINE_CLASS_OPERATOR_NEW_DELETE(cond_var_control_dynamic)
public:
  explicit
  cond_var_control_dynamic
  (mutex_type& a_mutex)noexcept:
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
  DANGO_UNMOVEABLE(cond_var_control_dynamic)
};

namespace
dango::detail
{
  class cond_var_base;
}

namespace
dango
{
  class cond_var;
  class static_cond_var;
  class shared_cond_var;
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
  constexpr ~cond_var_base()noexcept = default;
public:
  [[nodiscard]] auto lock()noexcept->locker;
  [[nodiscard]] auto try_lock()noexcept->try_locker;
public:
  constexpr auto get_control(dango::detail::sync_private::tag const)const noexcept->control_type*{ return m_control; }
private:
  control_type* const m_control;
public:
  DANGO_UNMOVEABLE(cond_var_base)
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
private:
  using super_type = dango::mutex_locker;
public:
  explicit
  locker
  (dango::detail::sync_private::tag const a_tag, control_type& a_control)noexcept:
  super_type{ a_tag, *(a_control.mutex_ref().get_control(a_tag)) },
  m_cond{ (void(a_control.init()), &a_control) }
  { }
  ~locker()noexcept = default;
public:
  void notify()const noexcept{ dango_assert(is_locked()); m_cond->notify(); }
  void notify_all()const noexcept{ dango_assert(is_locked()); m_cond->notify_all(); }
  void wait()const noexcept{ dango_assert(is_locked()); m_cond->wait(); }
  void wait(dango::timeout const& a_timeout)const noexcept{ dango_assert(is_locked()); m_cond->wait(a_timeout); }
private:
  control_type* const m_cond;
public:
  DANGO_DELETE_DEFAULT(locker)
  DANGO_UNMOVEABLE(locker)
};

class
dango::
detail::
cond_var_base::
try_locker
final:
public dango::mutex_try_locker
{
private:
  using super_type = dango::mutex_try_locker;
public:
  explicit
  try_locker
  (dango::detail::sync_private::tag const a_tag, control_type& a_control)noexcept:
  super_type{ a_tag, *(a_control.mutex_ref().get_control(a_tag)) },
  m_cond{ (void(a_control.init()), &a_control) }
  { }
  ~try_locker()noexcept = default;
public:
  void notify()const noexcept{ dango_assert(is_locked()); m_cond->notify(); }
  void notify_all()const noexcept{ dango_assert(is_locked()); m_cond->notify_all(); }
  void wait()const noexcept{ dango_assert(is_locked()); m_cond->wait(); }
  void wait(dango::timeout const& a_timeout)const noexcept{ dango_assert(is_locked()); m_cond->wait(a_timeout); }
private:
  control_type* const m_cond;
public:
  DANGO_DELETE_DEFAULT(try_locker)
  DANGO_UNMOVEABLE(try_locker)
};

inline auto
dango::
detail::
cond_var_base::
lock
()noexcept->locker
{
  return locker{ dango::detail::sync_private::tag{ }, *m_control };
}

inline auto
dango::
detail::
cond_var_base::
try_lock
()noexcept->try_locker
{
  return try_locker{ dango::detail::sync_private::tag{ }, *m_control };
}

/*** cond_var ***/

class
dango::
cond_var
final:
public dango::detail::cond_var_base
{
private:
  using super_type = dango::detail::cond_var_base;
  using control_type_dynamic = dango::detail::cond_var_control_dynamic;
public:
  explicit cond_var(mutex_type&)dango_new_noexcept;
  ~cond_var()noexcept;
private:
  dango::shared_mutex m_mutex;
public:
  DANGO_DELETE_DEFAULT(cond_var)
  DANGO_UNMOVEABLE(cond_var)
};

inline
dango::
cond_var::
cond_var(mutex_type& a_mutex)dango_new_noexcept:
super_type{ new control_type_dynamic{ a_mutex } },
m_mutex{ a_mutex }
{

}

inline
dango::
cond_var::
~cond_var
()noexcept
{
  auto const a_control = get_control(dango::detail::sync_private::tag{ });

  if(a_control->decrement())
  {
    delete static_cast<control_type_dynamic*>(a_control);
  }
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
  constexpr ~static_cond_var()noexcept = default;
private:
  control_type_static m_control_storage;
public:
  DANGO_DELETE_DEFAULT(static_cond_var)
  DANGO_UNMOVEABLE(static_cond_var)
};

constexpr
dango::
static_cond_var::
static_cond_var
(mutex_type_static& a_mutex)noexcept:
super_type{ &m_control_storage },
m_control_storage{ a_mutex }
{

}

/*** shared_cond_var ***/

class
dango::
shared_cond_var
final
{
private:
  using control_type = dango::detail::cond_var_control;
  using control_type_dynamic = dango::detail::cond_var_control_dynamic;
public:
  explicit
  shared_cond_var
  (dango::detail::sync_private::tag const, control_type& a_control)noexcept:
  m_mutex{ a_control.mutex_ref() },
  m_control{ &a_control }
  {
    m_control->increment();
  }
public:
  explicit constexpr
  shared_cond_var
  (dango::detail::cond_var_base& a_cond)noexcept:
  m_mutex{ a_cond.get_control(dango::detail::sync_private::tag{ })->mutex_ref() },
  m_control{ a_cond.get_control(dango::detail::sync_private::tag{ }) }
  {
    m_control->increment();
  }

  explicit constexpr
  shared_cond_var()noexcept:
  m_mutex{ },
  m_control{ dango::null }
  { }

  constexpr
  shared_cond_var
  (dango::null_tag const)noexcept:
  m_mutex{ dango::null },
  m_control{ dango::null }
  { }

  constexpr
  shared_cond_var
  (shared_cond_var const& a_cond)noexcept:
  m_mutex{ a_cond.m_mutex },
  m_control{ a_cond.m_control }
  {
    if(m_control)
    {
      m_control->increment();
    }
  }

  constexpr
  shared_cond_var
  (shared_cond_var&& a_cond)noexcept:
  m_mutex{ dango::move(a_cond).m_mutex },
  m_control{ dango::exchange(a_cond.m_control, dango::null) }
  { }

  constexpr
  ~shared_cond_var()noexcept
  {
    if(m_control && m_control->decrement())
    {
      delete static_cast<control_type_dynamic*>(m_control);
    }
  }

  DANGO_DEFINE_NULL_SWAP_ASSIGN(shared_cond_var, constexpr, true)
  DANGO_DEFINE_COPY_SWAP_ASSIGN(shared_cond_var, constexpr, true)
  DANGO_DEFINE_MOVE_SWAP_ASSIGN(shared_cond_var, constexpr, true)
  DANGO_DEFINE_NULLABLE_OPERATOR_BOOL(constexpr, true)
public:
  constexpr auto dango_operator_is_null()const noexcept->bool{ return dango::is_null(m_control); }
  constexpr auto dango_operator_equals(shared_cond_var const& a_cond)const noexcept->bool{ return dango::equals(m_control, a_cond.m_control); }
  constexpr auto dango_operator_compare(shared_cond_var const& a_cond)const noexcept->auto{ return dango::compare(m_control, a_cond.m_control); }

  constexpr void
  dango_operator_swap
  (shared_cond_var& a_cond)& noexcept
  {
    dango::swap(m_mutex, a_cond.m_mutex);
    dango::swap(m_control, a_cond.m_control);
  }
public:
  [[nodiscard]] auto
  lock()const noexcept->auto
  {
    dango_assert_nonnull(*this);

    return dango::cond_var_locker{ dango::detail::sync_private::tag{ }, *m_control };
  }

  [[nodiscard]] auto
  try_lock()const noexcept->auto
  {
    dango_assert_nonnull(*this);

    return dango::cond_var_try_locker{ dango::detail::sync_private::tag{ }, *m_control };
  }
private:
  dango::shared_mutex m_mutex;
  control_type* m_control;
};

#endif // DANGO_COND_VAR_HPP_INCLUDED
