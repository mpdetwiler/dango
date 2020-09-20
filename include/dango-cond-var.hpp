#ifndef DANGO_COND_VAR_HPP_INCLUDED
#define DANGO_COND_VAR_HPP_INCLUDED

#include "dango-concurrent.hpp"
#include "dango-intrusive-list.hpp"
#include "dango-mutex.hpp"
#include "dango-thread.hpp"

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
  DANGO_DELETE_CLASS_OPERATOR_NEW_DELETE
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
  virtual void increment()noexcept{ }
  virtual auto decrement()noexcept->bool{ return false; }
  auto mutex_ref()const noexcept->mutex_type&{ return *m_mutex; }
  auto storage()noexcept->dango::detail::cond_var_storage&{ return m_storage; }
  auto increment_suspend_aware_count(dango::crit_section const&)noexcept->bool;
  auto decrement_suspend_aware_count(dango::crit_section const&)noexcept->bool;
  auto make_reference()noexcept->dango::cond_var;
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
  DANGO_UNMOVEABLE(cond_var_control_dynamic)
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
  constexpr ~cond_var_base()noexcept = default;
public:
  [[nodiscard]] auto lock()noexcept->locker;
  [[nodiscard]] auto try_lock()noexcept->try_locker;
  auto reference()const noexcept->dango::cond_var;
protected:
  auto get_control()const noexcept->control_type*{ return m_control; }
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
  DANGO_UNMOVEABLE(try_locker)
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
  DANGO_UNMOVEABLE(cond_var)
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
m_control_storage{ &a_mutex }
{

}

#endif // DANGO_COND_VAR_HPP_INCLUDED
