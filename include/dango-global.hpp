#ifndef __DANGO_GLOBAL_HPP__
#define __DANGO_GLOBAL_HPP__

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
  <typename tp_type, typename tp_ret>
  using global_storage_enable_spec =
  dango::enable_if
  <
    !dango::is_array<tp_type> &&
    dango::is_object<tp_type> &&
    dango::is_same<dango::remove_cv<tp_type>, tp_ret>
  >;

  template
  <
    typename tp_type,
    typename tp_ret,
    tp_ret(& tp_construct)()noexcept
  >
  class global_storage
  <tp_type, tp_ret, tp_construct, dango::detail::global_storage_enable_spec<tp_type, tp_ret>>;

  template
  <typename tp_type, typename tp_ret, tp_ret(& tp_construct)()noexcept>
  using global_storage_ref =
    dango::detail::global_storage<tp_type, tp_ret, tp_construct, dango::enable_tag>&;
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
<tp_type, tp_ret, tp_construct, dango::detail::global_storage_enable_spec<tp_type, tp_ret>>
final
{
public:
  template
  <dango::detail::global_storage_ref<tp_type, tp_ret, tp_construct> tp_storage>
  class strong_incrementer;

  template
  <dango::detail::global_storage_ref<tp_type, tp_ret, tp_construct>  tp_storage>
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
<dango::detail::global_storage_ref<tp_type, tp_ret, tp_construct> tp_storage>
class
dango::
detail::
global_storage
<tp_type, tp_ret, tp_construct, dango::detail::global_storage_enable_spec<tp_type, tp_ret>>::
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
<dango::detail::global_storage_ref<tp_type, tp_ret, tp_construct> tp_storage>
dango::
detail::
global_storage
<tp_type, tp_ret, tp_construct, dango::detail::global_storage_enable_spec<tp_type, tp_ret>>::
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
<dango::detail::global_storage_ref<tp_type, tp_ret, tp_construct> tp_storage>
dango::
detail::
global_storage
<tp_type, tp_ret, tp_construct, dango::detail::global_storage_enable_spec<tp_type, tp_ret>>::
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
<dango::detail::global_storage_ref<tp_type, tp_ret, tp_construct> tp_storage>
class
dango::
detail::
global_storage
<tp_type, tp_ret, tp_construct, dango::detail::global_storage_enable_spec<tp_type, tp_ret>>::
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
<dango::detail::global_storage_ref<tp_type, tp_ret, tp_construct> tp_storage>
dango::
detail::
global_storage
<tp_type, tp_ret, tp_construct, dango::detail::global_storage_enable_spec<tp_type, tp_ret>>::
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
<dango::detail::global_storage_ref<tp_type, tp_ret, tp_construct> tp_storage>
dango::
detail::
global_storage
<tp_type, tp_ret, tp_construct, dango::detail::global_storage_enable_spec<tp_type, tp_ret>>::
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
<tp_type, tp_ret, tp_construct, dango::detail::global_storage_enable_spec<tp_type, tp_ret>>::
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
<tp_type, tp_ret, tp_construct, dango::detail::global_storage_enable_spec<tp_type, tp_ret>>::
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
<tp_type, tp_ret, tp_construct, dango::detail::global_storage_enable_spec<tp_type, tp_ret>>::
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
<tp_type, tp_ret, tp_construct, dango::detail::global_storage_enable_spec<tp_type, tp_ret>>::
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
<tp_type, tp_ret, tp_construct, dango::detail::global_storage_enable_spec<tp_type, tp_ret>>::
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

#ifndef DANGO_COMPILING_DANGO
#define DANGO_GLOBAL_DEFINE_STATIC_STRONG_INCREMENTER(type_name, name) static type_name const name{ };
#else
#define DANGO_GLOBAL_DEFINE_STATIC_STRONG_INCREMENTER(type_name, name)
#endif

#ifndef DANGO_NO_DEBUG
#define DANGO_GLOBAL_UNREACHABLE_TERMINATE(msg) dango_unreachable_msg(msg)
#else
#define DANGO_GLOBAL_UNREACHABLE_TERMINATE(msg) dango::terminate()
#endif

#define DANGO_DEFINE_GLOBAL_IMPL(linkage, type_name, name, ...) \
namespace \
name##_namespace \
{ \
  using dango_global_value_type = type_name; \
  static_assert(!dango::is_array<dango_global_value_type> && dango::is_object<dango_global_value_type>); \
  using dango_global_return_type = dango::remove_cv<dango_global_value_type>; \
  linkage auto dango_global_construct()noexcept->dango_global_return_type \
  { try{ return dango_global_return_type{ __VA_ARGS__ }; }catch(...){ DANGO_GLOBAL_UNREACHABLE_TERMINATE("constructor of global \"name\" threw exception"); } } \
  using dango_global_storage_type = \
    dango::detail::global_storage<dango_global_value_type, dango_global_return_type, dango_global_construct>; \
  linkage dango_global_storage_type s_dango_global_storage{ }; \
  using dango_global_strong_type = dango_global_storage_type::strong_incrementer<s_dango_global_storage>; \
  using dango_global_weak_type = dango_global_storage_type::weak_incrementer<s_dango_global_storage>; \
  DANGO_GLOBAL_DEFINE_STATIC_STRONG_INCREMENTER(dango_global_strong_type, s_dango_global_strong) \
} \
[[nodiscard]] linkage auto \
name \
(DANGO_SRC_LOC_ARG_DEFAULT(a_loc)) \
noexcept->name##_namespace::dango_global_weak_type \
{ \
  static name##_namespace::dango_global_strong_type const s_strong{ }; \
  return name##_namespace::dango_global_weak_type{ DANGO_SRC_LOC_ARG_FORWARD(a_loc) }; \
}

#define DANGO_DEFINE_GLOBAL(type_name, name, ...) \
DANGO_DEFINE_GLOBAL_IMPL(inline, type_name, name, __VA_ARGS__)

#define DANGO_DEFINE_GLOBAL_STATIC(type_name, name, ...) \
DANGO_DEFINE_GLOBAL_IMPL(static, type_name, name, __VA_ARGS__)

#define dango_access_global(global_name, local_name) \
if constexpr(auto const local_name = global_name(); true)

#endif

