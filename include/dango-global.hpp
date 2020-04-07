#ifndef DANGO_GLOBAL_HPP_INCLUDED
#define DANGO_GLOBAL_HPP_INCLUDED

namespace
dango::detail
{
  template
  <typename tp_type>
  concept global_storage_constraint_spec =
    dango::is_object_exclude_array<tp_type> && dango::is_noexcept_destructible<tp_type>;

  template
  <typename tp_type>
  using global_storage_construct_ref = dango::remove_cv<tp_type>(&)()noexcept;

  template
  <dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
  class global_storage;

  template
  <typename tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
  using global_storage_ref = dango::detail::global_storage<tp_type, tp_construct>&;
}

template
<dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
class alignas(dango::cache_align_type)
dango::
detail::
global_storage
final
{
public:
  template
  <dango::detail::global_storage_ref<tp_type, tp_construct> tp_storage>
  class strong_incrementer;

  template
  <dango::detail::global_storage_ref<tp_type, tp_construct> tp_storage>
  class weak_incrementer;
public:
  explicit constexpr global_storage()noexcept;
  ~global_storage()noexcept = default;
  constexpr auto get()const noexcept->tp_type*;
private:
  void increment()noexcept;
  auto try_increment()noexcept->bool;
  void decrement()noexcept;
private:
  tp_type* m_ptr;
  dango::aligned_storage<sizeof(tp_type), alignof(tp_type)> m_storage;
  dango::exec_once m_init;
  dango::spin_mutex m_lock;
  dango::usize m_ref_count;
private:
  DANGO_IMMOBILE(global_storage)
};

/*** strong_incrementer ***/

template
<dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
template
<dango::detail::global_storage_ref<tp_type, tp_construct> tp_storage>
class
dango::
detail::
global_storage<tp_type, tp_construct>::
strong_incrementer
final
{
public:
  strong_incrementer()noexcept{ tp_storage.increment(); }
  ~strong_incrementer()noexcept{ tp_storage.decrement(); }
public:
  DANGO_IMMOBILE(strong_incrementer)
};

/*** weak_incrementer ***/

template
<dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
template
<dango::detail::global_storage_ref<tp_type, tp_construct> tp_storage>
class
dango::
detail::
global_storage<tp_type, tp_construct>::
weak_incrementer
final
{
public:
  weak_incrementer(DANGO_SRC_LOC_ARG(a_loc))noexcept
  {
    if(tp_storage.try_increment())
    {
      return;
    }

    dango_unreachable_terminate_msg_loc(u8"attempt to access already-destroyed global", a_loc);
  }

  ~weak_incrementer()noexcept{ tp_storage.decrement(); }
public:
  constexpr auto get()const noexcept->tp_type*{ return tp_storage.get(); }
  constexpr auto operator -> ()const noexcept->tp_type*{ return tp_storage.get(); }
  constexpr auto operator * ()const noexcept->tp_type&{ return *tp_storage.get(); }
public:
  DANGO_IMMOBILE(weak_incrementer)
};

/*** global_storage ***/

template
<dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
constexpr
dango::
detail::
global_storage<tp_type, tp_construct>::
global_storage
()noexcept:
m_ptr{ dango::null },
m_storage{ },
m_init{ },
m_lock{ },
m_ref_count{ dango::usize(0) }
{

}

template
<dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
constexpr auto
dango::
detail::
global_storage<tp_type, tp_construct>::
get
()const noexcept->tp_type*
{
  return m_ptr;
}

template
<dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
void
dango::
detail::
global_storage<tp_type, tp_construct>::
increment
()noexcept
{
  bool const a_init =
  m_init.exec
  (
    [this]()noexcept->void
    {
      m_ptr =
        ::new (dango::placement, m_storage.get(), sizeof(tp_type), alignof(tp_type)) tp_type{ tp_construct() };

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

  dango_unreachable_terminate_msg(u8"attempt to initialize already-destroyed global");
}

template
<dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
auto
dango::
detail::
global_storage<tp_type, tp_construct>::
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
<dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
void
dango::
detail::
global_storage<tp_type, tp_construct>::
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

  auto const a_ptr = get();

  m_ptr = dango::null;

  dango::destructor(a_ptr);
}

#ifndef DANGO_BUILDING_SHARED_LIB
#define DANGO_GLOBAL_DEFINE_STATIC_INC(type_name, var_name) static type_name const var_name{ };
#define DANGO_GLOBAL_DEFINE_INLINE_INC(type_name, var_name, name) namespace name##_namespace{ inline type_name const var_name{ name() }; }
#else
#define DANGO_GLOBAL_DEFINE_STATIC_INC(type_name, var_name)
#define DANGO_GLOBAL_DEFINE_INLINE_INC(type_name, var_name, name)
#endif

/*** extern globals ***/

#define DANGO_DECLARE_GLOBAL_EXTERN(type_name, name, ...) \
namespace name##_namespace \
{ \
  using name##_value_type = type_name; \
  using name##_return_type = dango::remove_cv<name##_value_type>; \
  DANGO_EXPORT auto name##_construct()noexcept->name##_return_type; \
  using name##_storage_type = dango::detail::global_storage<name##_value_type, name##_construct>; \
  DANGO_EXPORT_ONLY extern name##_storage_type s_##name##_storage; \
  using name##_strong_type = name##_storage_type::strong_incrementer<s_##name##_storage>; \
  using name##_weak_type = name##_storage_type::weak_incrementer<s_##name##_storage>; \
  DANGO_GLOBAL_DEFINE_STATIC_INC(name##_strong_type, s_##name##_strong) \
} \
[[nodiscard]] DANGO_EXPORT auto \
name(DANGO_SRC_LOC_ARG_DEFAULT( ))noexcept->name##_namespace::name##_weak_type; \
DANGO_GLOBAL_DEFINE_INLINE_INC(name##_weak_type, s_##name##_weak, name)


#define DANGO_DEFINE_GLOBAL_EXTERN(type_name, name, ...) \
namespace name##_namespace \
{ \
  static_assert(dango::detail::global_storage_constraint_spec<name##_value_type>); \
  auto name##_construct()noexcept->name##_return_type \
  { try{ return name##_return_type __VA_ARGS__ ; }catch(...){ dango_unreachable_terminate_msg(u8"constructor of global \"name\" threw exception"); } } \
  constinit name##_storage_type s_##name##_storage{ }; \
} \
auto \
name(DANGO_SRC_LOC_ARG(a_loc))noexcept->name##_namespace::name##_weak_type \
{ \
  static name##_namespace::name##_strong_type const s_##name##_strong_func{ }; \
  return name##_namespace::name##_weak_type{ DANGO_SRC_LOC_ARG_FORWARD(a_loc) }; \
}

/*** inline globals ***/

#ifndef DANGO_BUILDING_SHARED_LIB
#define DANGO_DEFINE_GLOBAL_INLINE(type_name, name, ...) \
namespace name##_namespace \
{ \
  using name##_value_type = type_name; \
  static_assert(dango::detail::global_storage_constraint_spec<name##_value_type>); \
  using name##_return_type = dango::remove_cv<name##_value_type>; \
  inline auto name##_construct()noexcept->name##_return_type \
  { try{ return name##_return_type __VA_ARGS__ ; }catch(...){ dango_unreachable_terminate_msg(u8"constructor of global \"name\" threw exception"); } } \
  using name##_storage_type = dango::detail::global_storage<name##_value_type, name##_construct>; \
  inline constinit name##_storage_type s_##name##_storage{ }; \
  using name##_strong_type = name##_storage_type::strong_incrementer<s_##name##_storage>; \
  using name##_weak_type = name##_storage_type::weak_incrementer<s_##name##_storage>; \
  static name##_strong_type const s_##name##_strong{ }; \
} \
[[nodiscard]] inline auto \
name(DANGO_SRC_LOC_ARG_DEFAULT(a_loc))noexcept->name##_namespace::name##_weak_type \
{ \
  static name##_namespace::name##_strong_type const s_##name##_strong_func{ }; \
  return name##_namespace::name##_weak_type{ DANGO_SRC_LOC_ARG_FORWARD(a_loc) }; \
} \
namespace name##_namespace{ inline name##_weak_type const s_##name##_weak{ name() }; }
#else
#define DANGO_DEFINE_GLOBAL_INLINE(type_name, name, ...)
#endif

/*** access ***/

#define dango_access_global(global_name, local_name) \
if constexpr(auto const local_name = global_name(); true)

#endif // DANGO_GLOBAL_HPP_INCLUDED

