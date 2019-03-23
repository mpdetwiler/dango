#ifndef __DANGO_GLOBAL_HPP__
#define __DANGO_GLOBAL_HPP__

#define DANGO_GLOBAL_STORAGE_ENABLE_SPEC(type, ret, construct) \
type, \
ret, \
construct, \
dango::enable_if \
< \
  !dango::is_array<type> && \
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
  constexpr auto get()const noexcept->tp_type*;
private:
  void increment()noexcept;
  auto try_increment()noexcept->bool;
  void decrement()noexcept;
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
  <tp_type, tp_ret, tp_construct>& tp_storage
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
  <tp_type, tp_ret, tp_construct>& tp_storage
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
  <tp_type, tp_ret, tp_construct>& tp_storage
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
  <tp_type, tp_ret, tp_construct>& tp_storage
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
  <tp_type, tp_ret, tp_construct>& tp_storage
>
dango::
detail::
global_storage
<DANGO_GLOBAL_STORAGE_ENABLE_SPEC(tp_type, tp_ret, tp_construct)>::
weak_incrementer<tp_storage>::
weak_incrementer
(DANGO_SRC_LOC_ARG(a_loc))noexcept
{
  bool const a_alive = tp_storage.try_increment();

#ifndef DANGO_NO_DEBUG
  dango_assert_msg_loc(a_alive, "attempt to access already-destroyed global", a_loc);
#else
  if(!a_alive)
  {
    dango::terminate();
  }
#endif
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
  <tp_type, tp_ret, tp_construct>& tp_storage
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
  bool const a_init =
  m_init.exec
  (
    [this]()noexcept->void
    {
      ::new (dango::placement, m_storage.get()) tp_ret{ tp_construct() };

      m_ref_count = dango::usize(1);
    }
  );

  if(a_init)
  {
    return;
  }

  dango_crit(m_lock)
  {
    if(m_ref_count != dango::usize(0))
    {
      ++m_ref_count;

      return;
    }
  }
#ifndef DANGO_NO_DEBUG
  dango_unreachable_msg("attempt to initialize already-destroyed global");
#else
  dango::terminate();
#endif
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

  dango::destructor(get());
}

#undef DANGO_GLOBAL_STORAGE_ENABLE_SPEC

#define DANGO_DEFINE_GLOBAL_IMPL(linkage, name, cv, ...) \
namespace \
name##_namespace \
{ \
  using value_type = decltype(__VA_ARGS__) cv; \
  static_assert(!dango::is_array<value_type> && dango::is_object<value_type>); \
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
noexcept->name##_namespace::weak_type \
{ \
  static name##_namespace::strong_type const s_strong{ }; \
  return name##_namespace::weak_type{ DANGO_SRC_LOC_ARG_FORWARD(a_loc) }; \
} \
linkage constexpr auto& name##_fast = *name##_namespace::s_storage.get();

#define DANGO_DEFINE_GLOBAL_INLINE_CV(name, cv, ...) \
DANGO_DEFINE_GLOBAL_IMPL(inline, name, cv, __VA_ARGS__)

#define DANGO_DEFINE_GLOBAL_INLINE(name, ...) \
DANGO_DEFINE_GLOBAL_INLINE_CV(name, , __VA_ARGS__)

#define DANGO_DEFINE_GLOBAL_STATIC_CV(name, cv, ...) \
DANGO_DEFINE_GLOBAL_IMPL(static, name, cv, __VA_ARGS__)

#define DANGO_DEFINE_GLOBAL_STATIC(name, ...) \
DANGO_DEFINE_GLOBAL_STATIC_CV(name, , __VA_ARGS__)

#define dango_access_global(global_name, local_name) \
if constexpr(auto const local_name = global_name(); true)

#endif

