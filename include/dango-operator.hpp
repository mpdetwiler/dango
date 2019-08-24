#ifndef __DANGO_OPERATOR_HPP__
#define __DANGO_OPERATOR_HPP__

namespace
dango
{
  template
  <typename tp_type, typename tp_enabled = dango::enable_tag>
  constexpr bool const has_dango_operator_is_null = false;

  template
  <typename tp_type>
  constexpr bool const
  has_dango_operator_is_null
  <
    tp_type,
    dango::expr_check
    <
      dango::enable_if<dango::is_class<tp_type>>,
      dango::enable_if<dango::is_convertible_ret<decltype(dango::declval<dango::remove_const<tp_type> const&>().dango_operator_is_null()), bool>>
    >
  > = true;

  template
  <typename tp_type, typename tp_enabled = dango::enable_tag>
  constexpr bool const has_noexcept_dango_operator_is_null = false;

  template
  <typename tp_type>
  constexpr bool const
  has_noexcept_dango_operator_is_null
  <tp_type, dango::enable_if<dango::has_dango_operator_is_null<tp_type>>> =
    noexcept(dango::declval<dango::remove_const<tp_type> const&>().dango_operator_is_null());
}

namespace
dango
{
  template
  <typename tp_type, typename tp_enabled = dango::enable_tag>
  constexpr bool const is_null_equatable = false;

  template
  <typename tp_type>
  constexpr bool const
  is_null_equatable
  <
    tp_type,
    dango::expr_check
    <
      dango::enable_if<dango::is_object<tp_type>>,
      dango::enable_if<dango::is_convertible_ret<decltype(dango::declval<dango::remove_const<tp_type> const&>() == dango::declval<dango::null_tag const&>()), bool>>,
      dango::enable_if<dango::is_convertible_ret<decltype(dango::declval<dango::null_tag const&>() == dango::declval<dango::remove_const<tp_type> const&>()), bool>>,
      dango::enable_if<dango::is_convertible_ret<decltype(dango::declval<dango::remove_const<tp_type> const&>() != dango::declval<dango::null_tag const&>()), bool>>,
      dango::enable_if<dango::is_convertible_ret<decltype(dango::declval<dango::null_tag const&>() != dango::declval<dango::remove_const<tp_type> const&>()), bool>>
    >
  > = true;

  template
  <typename tp_type, typename tp_enabled = dango::enable_tag>
  constexpr bool const is_noexcept_null_equatable = false;

  template
  <typename tp_type>
  constexpr bool const
  is_noexcept_null_equatable
  <tp_type, dango::enable_if<dango::is_null_equatable<tp_type>>> =
    noexcept(dango::declval<dango::remove_const<tp_type> const&>() == dango::declval<dango::null_tag const&>()) &&
    noexcept(dango::declval<dango::null_tag const&>() == dango::declval<dango::remove_const<tp_type> const&>()) &&
    noexcept(dango::declval<dango::remove_const<tp_type> const&>() != dango::declval<dango::null_tag const&>()) &&
    noexcept(dango::declval<dango::null_tag const&>() != dango::declval<dango::remove_const<tp_type> const&>());

  template
  <typename tp_type>
  constexpr bool const is_nullable =
    dango::is_object<tp_type> &&
    dango::is_convertible<dango::null_tag const&, dango::remove_cv<tp_type>> &&
    dango::is_assignable<dango::remove_const<tp_type>&, dango::null_tag const&> &&
    dango::is_null_equatable<tp_type>;
}

/*** dango_operator_is_null ***/

namespace
dango
{
  template
  <typename tp_type, dango::enable_if<dango::has_dango_operator_is_null<tp_type>> = dango::enable_val>
  constexpr auto
  operator ==
  (tp_type const&, dango::null_tag)noexcept(dango::has_noexcept_dango_operator_is_null<tp_type>)->decltype(auto);

  template
  <typename tp_type, dango::enable_if<dango::has_dango_operator_is_null<tp_type>> = dango::enable_val>
  constexpr auto
  operator ==
  (dango::null_tag, tp_type const&)noexcept(dango::has_noexcept_dango_operator_is_null<tp_type>)->decltype(auto);

  template
  <typename tp_type, dango::enable_if<dango::has_dango_operator_is_null<tp_type>> = dango::enable_val>
  constexpr auto
  operator !=
  (tp_type const&, dango::null_tag)noexcept(dango::has_noexcept_dango_operator_is_null<tp_type>)->decltype(auto);

  template
  <typename tp_type, dango::enable_if<dango::has_dango_operator_is_null<tp_type>> = dango::enable_val>
  constexpr auto
  operator !=
  (dango::null_tag, tp_type const&)noexcept(dango::has_noexcept_dango_operator_is_null<tp_type>)->decltype(auto);
}

template
<typename tp_type, dango::enable_if<dango::has_dango_operator_is_null<tp_type>> = dango::enable_val>
constexpr auto
dango::
operator ==
(tp_type const& a_arg, dango::null_tag const)noexcept(dango::has_noexcept_dango_operator_is_null<tp_type>)->decltype(auto)
{
  return a_arg.dango_operator_is_null();
}

template
<typename tp_type, dango::enable_if<dango::has_dango_operator_is_null<tp_type>> = dango::enable_val>
constexpr auto
dango::
operator ==
(dango::null_tag const, tp_type const& a_arg)noexcept(dango::has_noexcept_dango_operator_is_null<tp_type>)->decltype(auto)
{
  return a_arg.dango_operator_is_null();
}

template
<typename tp_type, dango::enable_if<dango::has_dango_operator_is_null<tp_type>> = dango::enable_val>
constexpr auto
dango::
operator !=
(tp_type const& a_arg, dango::null_tag const)noexcept(dango::has_noexcept_dango_operator_is_null<tp_type>)->decltype(auto)
{
  return !a_arg.dango_operator_is_null();
}

template
<typename tp_type, dango::enable_if<dango::has_dango_operator_is_null<tp_type>> = dango::enable_val>
constexpr auto
dango::
operator !=
(dango::null_tag const, tp_type const& a_arg)noexcept(dango::has_noexcept_dango_operator_is_null<tp_type>)->decltype(auto)
{
  return !a_arg.dango_operator_is_null();
}

/*** dango_operator_equals ***/

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs, typename tp_enabled = dango::enable_tag>
  constexpr bool const has_dango_operator_equals = false;

  template
  <typename tp_lhs, typename tp_rhs>
  constexpr bool const
  has_dango_operator_equals
  <
    tp_lhs,
    tp_rhs,
    dango::expr_check
    <
      dango::enable_if<dango::is_class<tp_lhs> && dango::is_object<tp_rhs>>,
      dango::enable_if
      <
        dango::is_convertible_ret<decltype(dango::declval<dango::remove_const<tp_lhs> const&>().dango_operator_equals(dango::declval<dango::remove_const<tp_rhs> const&>())), bool>
      >
    >
  > = true;

  template
  <typename tp_lhs, typename tp_rhs, typename tp_enabled = dango::enable_tag>
  constexpr bool const has_noexcept_dango_operator_equals = false;

  template
  <typename tp_lhs, typename tp_rhs>
  constexpr bool const
  has_noexcept_dango_operator_equals
  <tp_lhs, tp_rhs, dango::enable_if<dango::has_dango_operator_equals<tp_lhs, tp_rhs>>> =
    noexcept(dango::declval<dango::remove_const<tp_lhs> const&>().dango_operator_equals(dango::declval<dango::remove_const<tp_rhs> const&>()));
}

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs, typename tp_enabled = dango::enable_tag>
  constexpr bool const is_equatable = false;

  template
  <typename tp_lhs, typename tp_rhs>
  constexpr bool const
  is_equatable
  <
    tp_lhs,
    tp_rhs,
    dango::expr_check
    <
      dango::enable_if<dango::is_object<tp_lhs> && dango::is_object<tp_rhs>>,
      dango::enable_if<dango::is_convertible_ret<decltype(dango::declval<dango::remove_const<tp_lhs> const&>() == dango::declval<dango::remove_const<tp_rhs> const&>()), bool>>,
      dango::enable_if<dango::is_convertible_ret<decltype(dango::declval<dango::remove_const<tp_lhs> const&>() != dango::declval<dango::remove_const<tp_rhs> const&>()), bool>>
    >
  > = true;

  template
  <typename tp_lhs, typename tp_rhs, typename tp_enabled = dango::enable_tag>
  constexpr bool const is_noexcept_equatable = false;

  template
  <typename tp_lhs, typename tp_rhs>
  constexpr bool const
  is_noexcept_equatable
  <tp_lhs, tp_rhs, dango::enable_if<dango::is_equatable<tp_lhs, tp_rhs>>> =
    noexcept(dango::declval<dango::remove_const<tp_lhs> const&>() == dango::declval<dango::remove_const<tp_rhs> const&>()) &&
    noexcept(dango::declval<dango::remove_const<tp_lhs> const&>() != dango::declval<dango::remove_const<tp_rhs> const&>());
}

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs, dango::enable_if<dango::has_dango_operator_equals<tp_lhs, tp_rhs>> = dango::enable_val>
  constexpr auto
  operator ==
  (tp_lhs const&, tp_rhs const&)noexcept(dango::has_noexcept_dango_operator_equals<tp_lhs, tp_rhs>)->decltype(auto);

  template
  <typename tp_lhs, typename tp_rhs, dango::enable_if<dango::has_dango_operator_equals<tp_lhs, tp_rhs>> = dango::enable_val>
  constexpr auto
  operator !=
  (tp_lhs const&, tp_rhs const&)noexcept(dango::has_noexcept_dango_operator_equals<tp_lhs, tp_rhs>)->decltype(auto);
}

template
<typename tp_lhs, typename tp_rhs, dango::enable_if<dango::has_dango_operator_equals<tp_lhs, tp_rhs>> = dango::enable_val>
constexpr auto
dango::
operator ==
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_equals<tp_lhs, tp_rhs>)->decltype(auto)
{
  return a_lhs.dango_operator_equals(a_rhs);
}

template
<typename tp_lhs, typename tp_rhs, dango::enable_if<dango::has_dango_operator_equals<tp_lhs, tp_rhs>> = dango::enable_val>
constexpr auto
dango::
operator !=
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_equals<tp_lhs, tp_rhs>)->decltype(auto)
{
  return !a_lhs.dango_operator_equals(a_rhs);
}

#endif

