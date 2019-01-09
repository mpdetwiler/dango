#ifndef __DANGO_MEM_HPP__
#define __DANGO_MEM_HPP__ 1

#include "dango-util.hpp"
#include "dango-assert.hpp"
#include "dango-macro.hpp"

/*** placement new ***/

namespace
dango
{
  struct
  placement_tag
  final
  {

  };

  inline constexpr dango::placement_tag const placement{ };
}

[[nodiscard]] constexpr auto
operator new
(dango::usize, dango::placement_tag, void*)noexcept->void*;

[[nodiscard]] constexpr auto
operator new[]
(dango::usize, dango::placement_tag, void*)noexcept->void*;

constexpr void
operator delete
(void*, dango::placement_tag, void*)noexcept;

constexpr void
operator delete[]
(void*, dango::placement_tag, void*)noexcept;

constexpr auto
operator new
(
  dango::usize const,
  dango::placement_tag const,
  void* const a_addr
)
noexcept->void*
{
  return a_addr;
}

constexpr auto
operator new[]
(
  dango::usize const,
  dango::placement_tag const,
  void* const a_addr
)
noexcept->void*
{
  return a_addr;
}

constexpr void
operator delete
(
  void* const,
  dango::placement_tag const,
  void* const
)
noexcept
{

}

constexpr void
operator delete[]
(
  void* const,
  dango::placement_tag const,
  void* const
)
noexcept
{

}

/*** dango_new_noexcept ***/

namespace
dango
{
#ifndef DANGO_NEW_NOEXCEPT
  inline constexpr bool const c_operator_new_noexcept = false;
#else
  inline constexpr bool const c_operator_new_noexcept = true;
#endif
}

namespace
dango::detail
{
  constexpr auto true_bool()noexcept->bool;

  template
  <typename tp_type>
  constexpr auto
  true_bool
  (tp_type&&)noexcept->
  dango::enable_if<dango::is_constructible<bool, tp_type>, bool>;
}

constexpr auto
dango::
detail::
true_bool
()noexcept->bool
{
  return true;
}

template
<typename tp_type>
constexpr auto
dango::
detail::
true_bool
(tp_type&& a_arg)noexcept->
dango::enable_if<dango::is_constructible<bool, tp_type>, bool>
{
  return bool(dango::forward<tp_type>(a_arg));
}

#define dango_new_noexcept(cond) \
noexcept(dango::c_operator_new_noexcept && dango::detail::true_bool(cond))

/*** launder ***/

namespace
dango
{
  template
  <typename tp_type>
  [[nodiscard]] constexpr auto
  launder
  (tp_type*)noexcept->
  dango::enable_if<dango::is_object<tp_type>, tp_type*>;
}

template
<typename tp_type>
constexpr auto
dango::
launder
(tp_type* const a_arg)noexcept->
dango::enable_if<dango::is_object<tp_type>, tp_type*>
{
  return __builtin_launder(a_arg);
}

/*** address_of ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr auto
  address_of
  (tp_type&)noexcept->
  dango::enable_if<dango::is_object<tp_type>, tp_type*>;
}

template
<typename tp_type>
constexpr auto
dango::
address_of
(tp_type& a_arg)noexcept->
dango::enable_if<dango::is_object<tp_type>, tp_type*>
{
  using cast_type = dango::preserve_cv<tp_type, dango::byte>;

  auto& a_ref = reinterpret_cast<cast_type&>(a_arg);

  return reinterpret_cast<tp_type*>(&a_ref);
}

/*** is_aligned ***/

namespace
dango
{
  constexpr auto is_aligned(void const volatile*, dango::usize)noexcept->bool;
}

constexpr auto
dango::
is_aligned
(void const volatile* const a_ptr, dango::usize const a_align)noexcept->bool
{
  dango_assert(dango::is_pow_two(a_align));

  auto const a_int = reinterpret_cast<dango::uintptr>(a_ptr);

  return (a_int % dango::uintptr(a_align)) == dango::uintptr(0);
}

/*** param_ptr ***/

namespace
dango::detail
{
  template
  <typename tp_type, typename tp_enabled = dango::enable_tag>
  constexpr dango::usize const param_ptr_align_help;

  template
  <typename tp_type>
  constexpr dango::usize const
  param_ptr_align_help<tp_type, dango::enable_if<dango::is_object<tp_type>>> =
    alignof(tp_type);

  template
  <typename tp_type>
  constexpr dango::usize const
  param_ptr_align_help<tp_type, dango::enable_if<dango::is_void<tp_type>>> =
    dango::usize(1);

  template
  <typename tp_type>
  constexpr dango::usize const
  param_ptr_align_help<tp_type, dango::enable_if<dango::is_func<tp_type>>> =
    dango::usize(1);
}

namespace
dango
{
  template
  <
    typename tp_type,
    bool tp_nullable = true,
    dango::usize tp_align = detail::param_ptr_align_help<tp_type>,
    typename tp_enabled = dango::enable_tag
  >
  class param_ptr;
}

#define DANGO_PARAM_PTR_ENABLE_SPEC(type, nullable, align) \
type, \
nullable, \
align, \
dango::enable_if \
< \
  ( \
    dango::is_object<type> || \
    dango::is_func<type> || \
    dango::is_void<type> \
  ) && \
  dango::is_pow_two(align) \
>

namespace
dango
{
  template
  <typename tp_type, bool tp_nullable, dango::usize tp_align>
  class
  param_ptr
  <DANGO_PARAM_PTR_ENABLE_SPEC(tp_type, tp_nullable, tp_align)>;
}

template
<typename tp_type, bool tp_nullable, dango::usize tp_align>
class
dango::
param_ptr
<DANGO_PARAM_PTR_ENABLE_SPEC(tp_type, tp_nullable, tp_align)>
final
{
public:
  using value_type = tp_type*;

  static dango::usize const c_align;
private:

public:
  template
  <
    bool tp_tp_nullable = tp_nullable,
    dango::enable_if<tp_tp_nullable> tp_enabled = dango::enable_val
  >
  constexpr
  param_ptr
  (dango::nullptr_tag const)noexcept:
  m_ptr{ nullptr }
  {

  }

  template
  <
    bool tp_tp_nullable = tp_nullable,
    dango::enable_if<!tp_tp_nullable> tp_enabled = dango::enable_val
  >
  constexpr
  param_ptr
  (dango::nullptr_tag)noexcept = delete;

  constexpr param_ptr(param_ptr const&)noexcept = default;

  template
  <
    typename tp_tp_type,
    typename tp_this = param_ptr,
    dango::enable_if
    <
      dango::is_noexcept_constructible
      <
        typename tp_this::value_type,
        tp_tp_type
      > &&
      dango::is_noexcept_convertible
      <
        tp_tp_type,
        typename tp_this::value_type
      >
    > tp_enabled = dango::enable_val
  >
  constexpr param_ptr(tp_tp_type&& a_arg, DANGO_SRC_LOC_ARG_DEFAULT())noexcept;

  template
  <
    typename tp_tp_type,
    typename tp_this = param_ptr,
    dango::enable_if
    <
      dango::is_noexcept_constructible
      <
        typename tp_this::value_type,
        tp_tp_type
      > &&
      !dango::is_noexcept_convertible
      <
        tp_tp_type,
        typename tp_this::value_type
      >
    > tp_enabled = dango::enable_val
  >
  explicit constexpr param_ptr(tp_tp_type&& a_arg, DANGO_SRC_LOC_ARG_DEFAULT())noexcept;

  template
  <
    typename tp_tp_type,
    bool tp_tp_nullable,
    dango::usize tp_tp_align,
    typename tp_other = dango::param_ptr<tp_tp_type, tp_tp_nullable, tp_tp_align>,
    dango::enable_if
    <
      dango::is_noexcept_constructible
      <
        value_type,
        typename tp_other::value_type
      > &&
      (tp_nullable || !tp_tp_nullable) &&
      (dango::is_func<tp_type> || dango::is_gequal(tp_other::c_align, c_align))
    > tp_enabled = dango::enable_val
  >
  constexpr
  param_ptr
  (param_ptr<tp_tp_type, tp_tp_nullable, tp_tp_align> const& a_ptr)noexcept:
  m_ptr{ a_ptr.get() }
  {

  }

  ~param_ptr()noexcept = default;

  constexpr auto get()const noexcept->value_type;

  constexpr operator value_type()const noexcept;

  explicit constexpr operator bool()const noexcept;
private:
  value_type const m_ptr;
public:
  DANGO_DELETE_DEFAULT(param_ptr)
  DANGO_UNASSIGNABLE(param_ptr)
};

template
<typename tp_type, bool tp_nullable, dango::usize tp_align>
constexpr dango::usize const
dango::
param_ptr
<DANGO_PARAM_PTR_ENABLE_SPEC(tp_type, tp_nullable, tp_align)>::
c_align = dango::max(tp_align, detail::param_ptr_align_help<tp_type>);

template
<typename tp_type, bool tp_nullable, dango::usize tp_align>
template
<
  typename tp_tp_type,
  typename tp_this,
  dango::enable_if
  <
    dango::is_noexcept_constructible
    <
      typename tp_this::value_type,
      tp_tp_type
    > &&
    dango::is_noexcept_convertible
    <
      tp_tp_type,
      typename tp_this::value_type
    >
  > tp_enabled
>
constexpr
dango::
param_ptr
<DANGO_PARAM_PTR_ENABLE_SPEC(tp_type, tp_nullable, tp_align)>::
param_ptr
(tp_tp_type&& a_arg, DANGO_SRC_LOC_ARG(a_loc))noexcept:
m_ptr{ dango::forward<tp_tp_type>(a_arg) }
{
  if constexpr(!tp_nullable)
  {
    dango_assert_loc(m_ptr != nullptr, a_loc);
  }

  if constexpr(!dango::is_func<tp_type>)
  {
    dango_assert_loc(dango::is_aligned(m_ptr, c_align), a_loc);
  }
}

template
<typename tp_type, bool tp_nullable, dango::usize tp_align>
template
<
  typename tp_tp_type,
  typename tp_this,
  dango::enable_if
  <
    dango::is_noexcept_constructible
    <
      typename tp_this::value_type,
      tp_tp_type
    > &&
    !dango::is_noexcept_convertible
    <
      tp_tp_type,
      typename tp_this::value_type
    >
  > tp_enabled
>
constexpr
dango::
param_ptr
<DANGO_PARAM_PTR_ENABLE_SPEC(tp_type, tp_nullable, tp_align)>::
param_ptr
(tp_tp_type&& a_arg, DANGO_SRC_LOC_ARG(a_loc))noexcept:
m_ptr{ dango::forward<tp_tp_type>(a_arg) }
{
  if constexpr(!tp_nullable)
  {
    dango_assert_loc(m_ptr != nullptr, a_loc);
  }

  if constexpr(!dango::is_func<tp_type>)
  {
    dango_assert_loc(dango::is_aligned(m_ptr, c_align), a_loc);
  }
}

template
<typename tp_type, bool tp_nullable, dango::usize tp_align>
constexpr auto
dango::
param_ptr
<DANGO_PARAM_PTR_ENABLE_SPEC(tp_type, tp_nullable, tp_align)>::
get
()const noexcept->value_type
{
  if constexpr(!tp_nullable)
  {
    dango::assume(m_ptr != nullptr);
  }

  if constexpr(!dango::is_func<tp_type>)
  {
    void const volatile* const a_ptr = m_ptr;

    void* const a_result =
      __builtin_assume_aligned(const_cast<void const*>(a_ptr), c_align);

    return static_cast<value_type>(a_result);
  }
  else
  {
    return m_ptr;
  }
}

template
<typename tp_type, bool tp_nullable, dango::usize tp_align>
constexpr
dango::
param_ptr
<DANGO_PARAM_PTR_ENABLE_SPEC(tp_type, tp_nullable, tp_align)>::
operator value_type
()const noexcept
{
  return get();
}

template
<typename tp_type, bool tp_nullable, dango::usize tp_align>
constexpr
dango::
param_ptr
<DANGO_PARAM_PTR_ENABLE_SPEC(tp_type, tp_nullable, tp_align)>::
operator bool
()const noexcept
{
  if constexpr(!tp_nullable)
  {
    return true;
  }
  else
  {
    return m_ptr != nullptr;
  }
}

#undef DANGO_PARAM_PTR_ENABLE_SPEC

/*** mem_copy ***/

namespace
dango
{
  template
  <dango::usize tp_align>
  constexpr auto
  mem_copy
  (
    dango::param_ptr<void, false, tp_align> const&,
    dango::param_ptr<void const, false, tp_align> const&,
    dango::usize
  )
  noexcept->dango::param_ptr<void, false, tp_align>;

  constexpr auto
  mem_copy
  (
    dango::param_ptr<void, false> const&,
    dango::param_ptr<void const, false> const&,
    dango::usize
  )
  noexcept->dango::param_ptr<void, false>;
}

template
<dango::usize tp_align>
constexpr auto
dango::
mem_copy
(
  dango::param_ptr<void, false, tp_align> const& a_dest,
  dango::param_ptr<void const, false, tp_align> const& a_source,
  dango::usize const a_count
)
noexcept->dango::param_ptr<void, false, tp_align>
{
  __builtin_memcpy(a_dest, a_source, a_count);

  return a_dest;
}

constexpr auto
dango::
mem_copy
(
  dango::param_ptr<void, false> const& a_dest,
  dango::param_ptr<void const, false> const& a_source,
  dango::usize const a_count
)
noexcept->dango::param_ptr<void, false>
{
  return dango::mem_copy<dango::usize(1)>(a_dest, a_source, a_count);
}





#endif

