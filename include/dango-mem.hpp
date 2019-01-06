#ifndef __DANGO_MEM_HPP__
#define __DANGO_MEM_HPP__ 1

#include "dango-util.hpp"
#include "dango-assert.hpp"
#include "dango-macro.hpp"

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

/*** aligned_ptr ***/

#define DANGO_ALIGNED_PTR_ENABLE_SPEC(type, align)  \
dango::enable_if                                    \
<                                                   \
  (dango::is_object<type> ||                        \
  dango::is_void<type>) &&                          \
  dango::is_pow_two(align),                         \
  dango::enable_tag                                 \
>

namespace
dango::detail
{
  template
  <typename tp_type>
  constexpr dango::usize const alignof_with_void_help = alignof(tp_type);

  template<>
  constexpr dango::usize const alignof_with_void_help<void> = dango::usize(1);
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr dango::usize const alignof_with_void =
    detail::alignof_with_void_help<dango::remove_cv<tp_type>>;
}

namespace
dango
{
  template
  <
    typename tp_type,
    dango::usize tp_align = dango::alignof_with_void<tp_type>,
    typename tp_enabled = dango::enable_tag
  >
  class aligned_ptr;

  template
  <typename tp_type, dango::usize tp_align>
  class
  aligned_ptr
  <tp_type, tp_align, DANGO_ALIGNED_PTR_ENABLE_SPEC(tp_type, tp_align)>;
}

template
<typename tp_type, dango::usize tp_align>
class
dango::
aligned_ptr
<tp_type, tp_align, DANGO_ALIGNED_PTR_ENABLE_SPEC(tp_type, tp_align)>
final
{
public:
  using value_type = tp_type*;

  static dango::usize const c_align;
public:
  constexpr aligned_ptr(dango::null_tag)noexcept;

  explicit constexpr aligned_ptr(value_type)noexcept;

  constexpr aligned_ptr(aligned_ptr const&)noexcept = default;

  template
  <
    typename tp_tp_type,
    dango::usize tp_tp_align,
    typename tp_other = dango::aligned_ptr<tp_tp_type, tp_tp_align>,
    dango::enable_if
    <
      dango::is_constructible<value_type, typename tp_other::value_type const&> &&
      dango::is_gequal(tp_other::c_align, c_align)
    > = dango::enable_val
  >
  constexpr
  aligned_ptr
  (dango::aligned_ptr<tp_tp_type, tp_tp_align> const& a_ptr)noexcept:
  m_ptr{ a_ptr.m_ptr }
  {

  }

  ~aligned_ptr()noexcept = default;

  constexpr auto operator = (aligned_ptr const&)noexcept->aligned_ptr& = default;

  template
  <
    typename tp_tp_type,
    dango::usize tp_tp_align,
    typename tp_other = dango::aligned_ptr<tp_tp_type, tp_tp_align>,
    dango::enable_if
    <
      dango::is_assignable<value_type&, typename tp_other::value_type const&> &&
      dango::is_gequal(tp_other::c_align, c_align)
    > = dango::enable_val
  >
  constexpr auto
  operator =
  (dango::aligned_ptr<tp_tp_type, tp_tp_align> const& a_ptr)noexcept->aligned_ptr&
  {
    m_ptr = a_ptr.m_ptr;

    return *this;
  }

private:
  value_type m_ptr;
public:
  DANGO_DELETE_DEFAULT(aligned_ptr)

  template
  <typename, dango::usize, typename tp_enabled>
  friend class aligned_ptr;
};

template
<typename tp_type, dango::usize tp_align>
constexpr dango::usize const
dango::
aligned_ptr
<tp_type, tp_align, DANGO_ALIGNED_PTR_ENABLE_SPEC(tp_type, tp_align)>::
c_align = dango::max(tp_align, dango::alignof_with_void<tp_type>);

template
<typename tp_type, dango::usize tp_align>
constexpr
dango::
aligned_ptr
<tp_type, tp_align, DANGO_ALIGNED_PTR_ENABLE_SPEC(tp_type, tp_align)>::
aligned_ptr
(dango::null_tag const)noexcept:
m_ptr{ nullptr }
{

}

template
<typename tp_type, dango::usize tp_align>
constexpr
dango::
aligned_ptr
<tp_type, tp_align, DANGO_ALIGNED_PTR_ENABLE_SPEC(tp_type, tp_align)>::
aligned_ptr
(value_type const a_ptr)noexcept:
m_ptr{ a_ptr }
{
  dango_assert(dango::is_aligned(a_ptr, c_align));
}

#undef DANGO_ALIGNED_PTR_ENABLE_SPEC

#endif

