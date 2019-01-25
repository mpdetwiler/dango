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

/*** destructor ***/

namespace
dango
{
  template
  <typename tp_type>
  auto
  destructor
  (tp_type const volatile*)
  noexcept(dango::is_noexcept_destructible<tp_type>)->
  dango::enable_if<dango::is_destructible<tp_type>, void>;
}

template
<typename tp_type>
auto
dango::
destructor
(tp_type const volatile* const a_ptr)
noexcept(dango::is_noexcept_destructible<tp_type>)->
dango::enable_if<dango::is_destructible<tp_type>, void>
{
  dango_assert(a_ptr != nullptr);

  a_ptr->~tp_type();
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

/*** operator_new operator_delete ***/

namespace
dango
{
  class heap_ptr;

  auto operator_new(dango::usize, dango::usize)dango_new_noexcept(true)->dango::heap_ptr;
  void operator_delete(void*, dango::usize, dango::usize)noexcept;
}

/*** heap_ptr ***/

namespace
dango
{
  class heap_ptr;
}

class
dango::
heap_ptr
final
{
  friend auto dango::operator_new(dango::usize, dango::usize)dango_new_noexcept()->dango::heap_ptr;
private:
  using value_type = void*;
private:
  constexpr heap_ptr(value_type, dango::usize, dango::usize)noexcept;
public:
  ~heap_ptr()noexcept;
  auto get()const noexcept->value_type;
  auto size()const noexcept->dango::usize;
  auto align()const noexcept->dango::usize;
  auto dismiss()noexcept->value_type;
private:
  value_type m_ptr;
  dango::usize const m_size;
  dango::usize const m_align;
public:
  DANGO_DELETE_DEFAULT(heap_ptr)
  DANGO_IMMOBILE(heap_ptr)
};

constexpr
dango::
heap_ptr::
heap_ptr
(
  value_type const a_ptr,
  dango::usize const a_size,
  dango::usize const a_align
)
noexcept:
m_ptr{ a_ptr },
m_size{ a_size },
m_align{ a_align }
{

}

inline
dango::
heap_ptr::
~heap_ptr
()noexcept
{
  if(m_ptr)
  {
    dango::operator_delete(m_ptr, m_size, m_align);
  }
}

inline auto
dango::
heap_ptr::
get
()const noexcept->value_type
{
  return m_ptr;
}

inline auto
dango::
heap_ptr::
size
()const noexcept->dango::usize
{
  return m_size;
}

inline auto
dango::
heap_ptr::
align
()const noexcept->dango::usize
{
  return m_align;
}

inline auto
dango::
heap_ptr::
dismiss
()noexcept->value_type
{
  auto const a_ptr = m_ptr;

  m_ptr = nullptr;

  return a_ptr;
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

/*** aligned_storage ***/

#define DANGO_ALIGNED_STORAGE_ENABLE_SPEC(size, align) \
size, \
align, \
dango::enable_if \
< \
  (size != dango::usize(0)) && \
  dango::is_pow_two(align) \
>

namespace
dango
{
  template
  <
    dango::usize tp_size,
    dango::usize tp_align = alignof(dango::max_align_type),
    typename tp_enabled = dango::enable_tag
  >
  class aligned_storage;

  template
  <dango::usize tp_size, dango::usize tp_align>
  class aligned_storage<DANGO_ALIGNED_STORAGE_ENABLE_SPEC(tp_size, tp_align)>;
}

template
<dango::usize tp_size, dango::usize tp_align>
class
dango::
aligned_storage<DANGO_ALIGNED_STORAGE_ENABLE_SPEC(tp_size, tp_align)>
{
public:
  static constexpr dango::usize const c_size = dango::next_multiple(tp_size, tp_align);
  static constexpr dango::usize const c_align = tp_align;
public:
  constexpr aligned_storage()noexcept = default;

  ~aligned_storage()noexcept = default;

  constexpr auto get()noexcept->void*;
  constexpr auto get()const noexcept->void const*;
private:
  alignas(c_align) dango::byte m_storage[c_size];
public:
  DANGO_IMMOBILE(aligned_storage)
};

template
<dango::usize tp_size, dango::usize tp_align>
constexpr auto
dango::
aligned_storage<DANGO_ALIGNED_STORAGE_ENABLE_SPEC(tp_size, tp_align)>::
get
()noexcept->void*
{
  return m_storage;
}

template
<dango::usize tp_size, dango::usize tp_align>
constexpr auto
dango::
aligned_storage<DANGO_ALIGNED_STORAGE_ENABLE_SPEC(tp_size, tp_align)>::
get
()const noexcept->void const*
{
  return m_storage;
}

#undef DANGO_ALIGNED_STORAGE_ENABLE_SPEC

#ifdef DANGO_SOURCE_FILE

#include <new>

auto
dango::
operator_new
(
  dango::usize const a_size_arg,
  dango::usize const a_align_arg
)
dango_new_noexcept(true)->dango::heap_ptr
{
  dango_assert(a_size_arg != dango::usize(0));
  dango_assert(dango::is_pow_two(a_align_arg));

  auto const a_size = dango::next_multiple(a_size_arg, a_align_arg);
  auto const a_align = std::align_val_t{ a_align_arg };

  if constexpr(dango::c_operator_new_noexcept)
  {
    try
    {
      auto const a_ptr = ::operator new(a_size, a_align);

      return dango::heap_ptr{ a_ptr, a_size_arg, a_align_arg };
    }
    catch(...)
    {
#ifndef DANGO_NO_DEBUG
      dango_unreachable_msg("memory allocation failed");
#else
      dango::terminate();
#endif
    }
  }
  else
  {
    auto const a_ptr = ::operator new(a_size, a_align);

    return dango::heap_ptr{ a_ptr, a_size_arg, a_align_arg };
  }
}

void
dango::
operator_delete
(
  void* const a_ptr,
  dango::usize const a_size_arg,
  dango::usize const a_align_arg
)
noexcept
{
  dango_assert(a_ptr != nullptr);
  dango_assert(a_size_arg != dango::usize(0));
  dango_assert(dango::is_pow_two(a_align_arg));

  auto const a_size = dango::next_multiple(a_size_arg, a_align_arg);
  auto const a_align = std::align_val_t{ a_align_arg };

  ::operator delete(a_ptr, a_size, a_align);
}

#endif

#endif

