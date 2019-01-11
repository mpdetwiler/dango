#ifndef __DANGO_GLOBAL_HPP__
#define __DANGO_GLOBAL_HPP__ 1

#include "dango-concurrent.hpp"

#define DANGO_GLOBAL_STORAGE_ENABLE_SPEC(type, ret, construct) \
type, \
ret, \
construct, \
dango::enable_if \
< \
  dango::is_object<type> && \
  dango::is_same<dango::remove_cv<type>, ret> \
>

namespace
dango::detail
{
  template
  <
    typename tp_type,
    typename tp_ret,
    tp_ret(& tp_construct)()noexcept,
    typename tp_enabled = dango::enable_tag
  >
  class global_storage;

  template
  <
    typename tp_type,
    typename tp_ret,
    tp_ret(& tp_construct)()noexcept
  >
  class global_storage<DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>;
}

template
<
  typename tp_type,
  typename tp_ret,
  tp_ret(& tp_construct)()noexcept
>
class alignas(dango::cache_align_type)
dango::
detail::
global_storage
<DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>
final
{
public:
  template
  <global_storage& tp_storage>
  class strong_incrementer;

  template
  <global_storage& tp_storage>
  class weak_incrementer;
public:
  constexpr global_storage()noexcept;
  ~global_storage()noexcept = default;
private:
  void increment()noexcept;
  auto try_increment()noexcept->bool;
  void decrement()noexcept;
  constexpr auto get()const noexcept->tp_type*;
private:
  mutable dango::aligned_storage<sizeof(tp_ret), alignof(tp_ret)> m_storage;
  dango::exec_once m_init;
  dango::spin_mutex m_lock;
  dango::usize m_ref_count;
private:
  DANGO_IMMOBILE(global_storage)
};

/*** strong_incrementer ***/

template
<
  typename tp_type,
  typename tp_ret,
  tp_ret(& tp_construct)()noexcept
>
template
<
  dango::
  detail::
  global_storage
  <DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>&
  tp_storage
>
class
dango::
detail::
global_storage
<DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>::
strong_incrementer
final
{
public:
  strong_incrementer()noexcept;
  ~strong_incrementer()noexcept;
public:
  DANGO_IMMOBILE(strong_incrementer)
};

template
<
  typename tp_type,
  typename tp_ret,
  tp_ret(& tp_construct)()noexcept
>
template
<
  dango::
  detail::
  global_storage
  <DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>&
  tp_storage
>
dango::
detail::
global_storage
<DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>::
strong_incrementer<tp_storage>::
strong_incrementer
()noexcept
{
  tp_storage.increment();
}

template
<
  typename tp_type,
  typename tp_ret,
  tp_ret(& tp_construct)()noexcept
>
template
<
  dango::
  detail::
  global_storage
  <DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>&
  tp_storage
>
dango::
detail::
global_storage
<DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>::
strong_incrementer<tp_storage>::
~strong_incrementer
()noexcept
{
  tp_storage.decrement();
}

/*** weak_incrementer ***/

template
<
  typename tp_type,
  typename tp_ret,
  tp_ret(& tp_construct)()noexcept
>
template
<
  dango::
  detail::
  global_storage
  <DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>&
  tp_storage
>
class
dango::
detail::
global_storage
<DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>::
weak_incrementer
final
{
public:
  weak_incrementer(DANGO_SRC_LOC_ARG())noexcept;
  ~weak_incrementer()noexcept;
public:
  constexpr auto get()const noexcept->tp_type*{ return tp_storage.get(); }
  constexpr auto operator -> ()const noexcept->tp_type*{ return tp_storage.get(); }
  constexpr auto operator * ()const noexcept->tp_type&{ return *tp_storage.get(); }
  explicit constexpr operator bool()const noexcept{ return true; }
public:
  DANGO_IMMOBILE(weak_incrementer)
};

template
<
  typename tp_type,
  typename tp_ret,
  tp_ret(& tp_construct)()noexcept
>
template
<
  dango::
  detail::
  global_storage
  <DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>&
  tp_storage
>
dango::
detail::
global_storage
<DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>::
weak_incrementer<tp_storage>::
weak_incrementer
(DANGO_SRC_LOC_ARG(a_loc))noexcept
{
  bool const dango_global_alive = tp_storage.try_increment();

  dango_assert_loc(dango_global_alive, a_loc);

  if(dango_global_alive)
  {
    return;
  }

  dango::terminate();
}

template
<
  typename tp_type,
  typename tp_ret,
  tp_ret(& tp_construct)()noexcept
>
template
<
  dango::
  detail::
  global_storage
  <DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>&
  tp_storage
>
dango::
detail::
global_storage
<DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>::
weak_incrementer<tp_storage>::
~weak_incrementer
()noexcept
{
  tp_storage.decrement();
}

/*** global_storage ***/

template
<
  typename tp_type,
  typename tp_ret,
  tp_ret(& tp_construct)()noexcept
>
constexpr
dango::
detail::
global_storage
<DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>::
global_storage
()noexcept:
m_storage{ },
m_init{ },
m_lock{ },
m_ref_count{ dango::usize(0) }
{

}

template
<
  typename tp_type,
  typename tp_ret,
  tp_ret(& tp_construct)()noexcept
>
void
dango::
detail::
global_storage
<DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>::
increment
()noexcept
{
  m_init.exec
  (
    [this]()noexcept->void
    {
      ::new (dango::placement, m_storage.get()) tp_ret{ tp_construct() };
    }
  );

  dango_crit(m_lock)
  {
    ++m_ref_count;
  }
}

template
<
  typename tp_type,
  typename tp_ret,
  tp_ret(& tp_construct)()noexcept
>
auto
dango::
detail::
global_storage
<DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>::
try_increment
()noexcept->bool
{
  dango_crit(m_lock)
  {
    if(m_ref_count == dango::usize(0))
    {
      return false;
    }

    ++m_ref_count;
  }

  return true;
}

template
<
  typename tp_type,
  typename tp_ret,
  tp_ret(& tp_construct)()noexcept
>
void
dango::
detail::
global_storage
<DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>::
decrement
()noexcept
{
  dango_crit(m_lock)
  {
    if(--m_ref_count != dango::usize(0))
    {
      return;
    }
  }

  auto const a_ptr = static_cast<tp_ret*>(m_storage.get());

  dango::destructor(a_ptr);
}

template
<
  typename tp_type,
  typename tp_ret,
  tp_ret(& tp_construct)()noexcept
>
constexpr auto
dango::
detail::
global_storage
<DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>::
get
()const noexcept->tp_type*
{
  return static_cast<tp_type*>(m_storage.get());
}

#undef DANGO_GLOBAL_STORAGE_ENABLE_SPEC

#define DANGO_DEFINE_GLOBAL_IMPL(linkage, name, type, ...) \
namespace \
name##_dango_global \
{ \
  using value_type = type; \
  using return_type = dango::remove_cv<value_type>; \
  linkage auto \
  construct \
  ()noexcept->return_type \
  { \
    return return_type{ __VA_ARGS__ }; \
  } \
  using storage_type = \
    dango::detail::global_storage<value_type, return_type, construct>; \
  linkage storage_type s_storage{ }; \
  using strong_type = storage_type::strong_incrementer<s_storage>; \
  using weak_type = storage_type::weak_incrementer<s_storage>; \
  static strong_type const s_strong{ }; \
} \
[[nodiscard]] linkage auto \
name \
(DANGO_SRC_LOC_ARG_DEFAULT(a_loc)) \
noexcept->name##_dango_global::weak_type \
{ \
  static name##_dango_global::strong_type const s_strong{ }; \
  return name##_dango_global::weak_type{ DANGO_SRC_LOC_ARG_FORWARD(a_loc) }; \
}

#define DANGO_DEFINE_GLOBAL_INLINE(name, type, ...) \
DANGO_DEFINE_GLOBAL_IMPL(inline, name, type, __VA_ARGS__)

#define DANGO_DEFINE_GLOBAL_STATIC(name, type, ...) \
DANGO_DEFINE_GLOBAL_IMPL(static, name, type, __VA_ARGS__)

#define dango_access_global(global_name, local_name) \
if constexpr(auto const local_name = global_name())

#endif

