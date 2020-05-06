#ifndef DANGO_OPERATOR_HPP_INCLUDED
#define DANGO_OPERATOR_HPP_INCLUDED

/*** dango_operator_is_null ***/

namespace
dango
{
  template
  <typename tp_type>
  concept has_dango_operator_is_null =
    dango::is_class_or_union<dango::remove_ref<tp_type>> &&
    requires(tp_type a_arg){ { dango::forward<tp_type>(a_arg).dango_operator_is_null() }->dango::is_convertible_ret<bool>; };

  template
  <typename tp_type>
  concept has_noexcept_dango_operator_is_null =
    dango::has_dango_operator_is_null<tp_type> &&
    requires(tp_type a_arg){ { dango::forward<tp_type>(a_arg).dango_operator_is_null() }noexcept->dango::is_noexcept_convertible_ret<bool>; };
}

template
<typename tp_type>
requires(dango::has_dango_operator_is_null<tp_type const&>)
constexpr auto
operator ==
(tp_type const& a_arg, dango::null_tag const)noexcept(dango::has_noexcept_dango_operator_is_null<tp_type const&>)->bool
{
  return a_arg.dango_operator_is_null();
}

template
<typename tp_type>
requires(dango::has_dango_operator_is_null<tp_type const&>)
constexpr auto
operator ==
(dango::null_tag const, tp_type const& a_arg)noexcept(dango::has_noexcept_dango_operator_is_null<tp_type const&>)->bool
{
  return a_arg.dango_operator_is_null();
}

template
<typename tp_type>
requires(dango::has_dango_operator_is_null<tp_type const&>)
constexpr auto
operator !=
(tp_type const& a_arg, dango::null_tag const)noexcept(dango::has_noexcept_dango_operator_is_null<tp_type const&>)->bool
{
  return !a_arg.dango_operator_is_null();
}

template
<typename tp_type>
requires(dango::has_dango_operator_is_null<tp_type const&>)
constexpr auto
operator !=
(dango::null_tag const, tp_type const& a_arg)noexcept(dango::has_noexcept_dango_operator_is_null<tp_type const&>)->bool
{
  return !a_arg.dango_operator_is_null();
}

/*** dango_operator_equals ***/

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept has_dango_operator_equals =
    dango::is_class_or_union<dango::remove_ref<tp_lhs>> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::forward<tp_lhs>(a_lhs).dango_operator_equals(dango::forward<tp_rhs>(a_rhs)) }->dango::is_convertible_ret<bool>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_dango_operator_equals =
    dango::has_dango_operator_equals<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::forward<tp_lhs>(a_lhs).dango_operator_equals(dango::forward<tp_rhs>(a_rhs)) }noexcept->dango::is_noexcept_convertible_ret<bool>; };
}

template
<typename tp_lhs, typename tp_rhs>
requires(dango::has_dango_operator_equals<tp_lhs const&, tp_rhs const&>)
constexpr auto
operator ==
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_equals<tp_lhs const&, tp_rhs const&>)->bool
{
  return a_lhs.dango_operator_equals(a_rhs);
}

template
<typename tp_lhs, typename tp_rhs>
requires(!dango::has_dango_operator_equals<tp_lhs const&, tp_rhs const&> && dango::has_dango_operator_equals<tp_rhs const&, tp_lhs const&>)
constexpr auto
operator ==
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_equals<tp_rhs const&, tp_lhs const&>)->bool
{
  return a_rhs.dango_operator_equals(a_lhs);
}

template
<typename tp_lhs, typename tp_rhs>
requires(dango::has_dango_operator_equals<tp_lhs const&, tp_rhs const&>)
constexpr auto
operator !=
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_equals<tp_lhs const&, tp_rhs const&>)->bool
{
  return !a_lhs.dango_operator_equals(a_rhs);
}

template
<typename tp_lhs, typename tp_rhs>
requires(!dango::has_dango_operator_equals<tp_lhs const&, tp_rhs const&> && dango::has_dango_operator_equals<tp_rhs const&, tp_lhs const&>)
constexpr auto
operator !=
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_equals<tp_rhs const&, tp_lhs const&>)->bool
{
  return !a_rhs.dango_operator_equals(a_lhs);
}

/*** dango_operator_compare ***/

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept has_dango_operator_compare =
    dango::is_class_or_union<dango::remove_ref<tp_lhs>> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::forward<tp_lhs>(a_lhs).dango_operator_compare(dango::forward<tp_rhs>(a_rhs)) }->dango::is_convertible_ret<dango::compare_val>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_dango_operator_compare =
    dango::has_dango_operator_compare<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::forward<tp_lhs>(a_lhs).dango_operator_compare(dango::forward<tp_rhs>(a_rhs)) }noexcept->dango::is_noexcept_convertible_ret<dango::compare_val>; };
}

template
<typename tp_lhs, typename tp_rhs>
requires(dango::has_dango_operator_compare<tp_lhs const&, tp_rhs const&>)
constexpr auto
operator <
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_compare<tp_lhs const&, tp_rhs const&>)->bool
{
  return dango::comparison::is_less(a_lhs.dango_operator_compare(a_rhs));
}

template
<typename tp_lhs, typename tp_rhs>
requires(!dango::has_dango_operator_compare<tp_lhs const&, tp_rhs const&> && dango::has_dango_operator_compare<tp_rhs const&, tp_lhs const&>)
constexpr auto
operator <
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_compare<tp_rhs const&, tp_lhs const&>)->bool
{
  return dango::comparison::is_less(dango::comparison::mirror(a_rhs.dango_operator_compare(a_lhs)));
}

template
<typename tp_lhs, typename tp_rhs>
requires(dango::has_dango_operator_compare<tp_lhs const&, tp_rhs const&>)
constexpr auto
operator <=
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_compare<tp_lhs const&, tp_rhs const&>)->bool
{
  return dango::comparison::is_less_equal(a_lhs.dango_operator_compare(a_rhs));
}

template
<typename tp_lhs, typename tp_rhs>
requires(!dango::has_dango_operator_compare<tp_lhs const&, tp_rhs const&> && dango::has_dango_operator_compare<tp_rhs const&, tp_lhs const&>)
constexpr auto
operator <=
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_compare<tp_rhs const&, tp_lhs const&>)->bool
{
  return dango::comparison::is_less_equal(dango::comparison::mirror(a_rhs.dango_operator_compare(a_lhs)));
}

template
<typename tp_lhs, typename tp_rhs>
requires(dango::has_dango_operator_compare<tp_lhs const&, tp_rhs const&>)
constexpr auto
operator >
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_compare<tp_lhs const&, tp_rhs const&>)->bool
{
  return dango::comparison::is_greater(a_lhs.dango_operator_compare(a_rhs));
}

template
<typename tp_lhs, typename tp_rhs>
requires(!dango::has_dango_operator_compare<tp_lhs const&, tp_rhs const&> && dango::has_dango_operator_compare<tp_rhs const&, tp_lhs const&>)
constexpr auto
operator >
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_compare<tp_rhs const&, tp_lhs const&>)->bool
{
  return dango::comparison::is_greater(dango::comparison::mirror(a_rhs.dango_operator_compare(a_lhs)));
}

template
<typename tp_lhs, typename tp_rhs>
requires(dango::has_dango_operator_compare<tp_lhs const&, tp_rhs const&>)
constexpr auto
operator >=
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_compare<tp_lhs const&, tp_rhs const&>)->bool
{
  return dango::comparison::is_greater_equal(a_lhs.dango_operator_compare(a_rhs));
}

template
<typename tp_lhs, typename tp_rhs>
requires(!dango::has_dango_operator_compare<tp_lhs const&, tp_rhs const&> && dango::has_dango_operator_compare<tp_rhs const&, tp_lhs const&>)
constexpr auto
operator >=
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_compare<tp_rhs const&, tp_lhs const&>)->bool
{
  return dango::comparison::is_greater_equal(dango::comparison::mirror(a_rhs.dango_operator_compare(a_lhs)));
}

template
<typename tp_lhs, typename tp_rhs>
requires(dango::has_dango_operator_compare<tp_lhs const&, tp_rhs const&>)
constexpr auto
operator <=>
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_compare<tp_lhs const&, tp_rhs const&>)->dango::compare_val
{
  return a_lhs.dango_operator_compare(a_rhs);
}

template
<typename tp_lhs, typename tp_rhs>
requires(!dango::has_dango_operator_compare<tp_lhs const&, tp_rhs const&> && dango::has_dango_operator_compare<tp_rhs const&, tp_lhs const&>)
constexpr auto
operator <=>
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_compare<tp_rhs const&, tp_lhs const&>)->dango::compare_val
{
  return dango::comparison::mirror(a_rhs.dango_operator_compare(a_lhs));
}

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_dango_operator_compare<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  compare
  (tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_compare<tp_lhs const&, tp_rhs const&>)->dango::compare_val
  {
    return a_lhs.dango_operator_compare(a_rhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(!dango::has_dango_operator_compare<tp_lhs const&, tp_rhs const&> && dango::has_dango_operator_compare<tp_rhs const&, tp_lhs const&>)
  constexpr auto
  compare
  (tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_compare<tp_rhs const&, tp_lhs const&>)->dango::compare_val
  {
    return dango::comparison::mirror(a_rhs.dango_operator_compare(a_lhs));
  }
}

/*** has_dango_operator_swap ***/

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept has_dango_operator_swap =
    dango::is_class_or_union<dango::remove_ref<tp_lhs>> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::forward<tp_lhs>(a_lhs).dango_operator_swap(dango::forward<tp_rhs>(a_rhs)) }->dango::is_convertible_ret<void>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_dango_operator_swap =
    dango::has_dango_operator_swap<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::forward<tp_lhs>(a_lhs).dango_operator_swap(dango::forward<tp_rhs>(a_rhs)) }noexcept->dango::is_noexcept_convertible_ret<void>; };
}

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_dango_operator_swap<tp_lhs&, tp_rhs&>)
  constexpr void
  swap
  (tp_lhs& a_lhs, tp_rhs& a_rhs)noexcept(dango::has_noexcept_dango_operator_swap<tp_lhs&, tp_rhs&>)
  {
    a_lhs.dango_operator_swap(a_rhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(!dango::has_dango_operator_swap<tp_lhs&, tp_rhs&> && dango::has_dango_operator_swap<tp_rhs&, tp_lhs&>)
  constexpr void
  swap
  (tp_lhs& a_lhs, tp_rhs& a_rhs)noexcept(dango::has_noexcept_dango_operator_swap<tp_rhs&, tp_lhs&>)
  {
    a_rhs.dango_operator_swap(a_lhs);
  }
}

/*****************************************************************************************************************************/

/*** is_equatable ***/

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept is_equatable =
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    {
      { dango::forward<tp_lhs>(a_lhs) == dango::forward<tp_rhs>(a_rhs) }->dango::is_convertible_ret<bool>;
      { dango::forward<tp_lhs>(a_lhs) != dango::forward<tp_rhs>(a_rhs) }->dango::is_convertible_ret<bool>;
    };

  template
  <typename tp_lhs, typename tp_rhs>
  concept is_noexcept_equatable =
    dango::is_equatable<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    {
      { dango::forward<tp_lhs>(a_lhs) == dango::forward<tp_rhs>(a_rhs) }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { dango::forward<tp_lhs>(a_lhs) != dango::forward<tp_rhs>(a_rhs) }noexcept->dango::is_noexcept_convertible_ret<bool>;
    };
}

/*** is_nullable ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_null_equatable =
    dango::is_equatable<tp_type, dango::null_tag const&> && dango::is_equatable<tp_type, dango::null_tag&&> &&
    dango::is_equatable<dango::null_tag const&, tp_type> && dango::is_equatable<dango::null_tag&&, tp_type>;

  template
  <typename tp_type>
  concept is_noexcept_null_equatable =
    dango::is_null_equatable<tp_type> &&
    dango::is_noexcept_equatable<tp_type, dango::null_tag const&> && dango::is_noexcept_equatable<tp_type, dango::null_tag&&> &&
    dango::is_noexcept_equatable<dango::null_tag const&, tp_type> && dango::is_noexcept_equatable<dango::null_tag&&, tp_type>;

  template
  <typename tp_type>
  concept is_nullable =
    dango::is_object_exclude_array<tp_type> &&
    dango::is_null_equatable<dango::remove_cv<tp_type> const&> && dango::is_null_equatable<dango::remove_cv<tp_type>&&> &&
    dango::is_constructible<dango::remove_cv<tp_type>, dango::null_tag const&> && dango::is_constructible<dango::remove_cv<tp_type>, dango::null_tag&&> &&
    dango::is_convertible<dango::null_tag const&, dango::remove_cv<tp_type>> && dango::is_convertible<dango::null_tag&&, dango::remove_cv<tp_type>> &&
    dango::is_assignable<dango::remove_cv<tp_type>&, dango::null_tag const&> && dango::is_assignable<dango::remove_cv<tp_type>&, dango::null_tag&&>;
}

/*** is_comparable ***/

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept is_comparable =
    dango::is_equatable<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    {
      { dango::forward<tp_lhs>(a_lhs) <  dango::forward<tp_rhs>(a_rhs) }->dango::is_convertible_ret<bool>;
      { dango::forward<tp_lhs>(a_lhs) <= dango::forward<tp_rhs>(a_rhs) }->dango::is_convertible_ret<bool>;
      { dango::forward<tp_lhs>(a_lhs) >  dango::forward<tp_rhs>(a_rhs) }->dango::is_convertible_ret<bool>;
      { dango::forward<tp_lhs>(a_lhs) >= dango::forward<tp_rhs>(a_rhs) }->dango::is_convertible_ret<bool>;
    };

  template
  <typename tp_lhs, typename tp_rhs>
  concept is_noexcept_comparable =
    dango::is_comparable<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    {
      { dango::forward<tp_lhs>(a_lhs) <  dango::forward<tp_rhs>(a_rhs) }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { dango::forward<tp_lhs>(a_lhs) <= dango::forward<tp_rhs>(a_rhs) }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { dango::forward<tp_lhs>(a_lhs) >  dango::forward<tp_rhs>(a_rhs) }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { dango::forward<tp_lhs>(a_lhs) >= dango::forward<tp_rhs>(a_rhs) }noexcept->dango::is_noexcept_convertible_ret<bool>;
    };
}

/*** is_swappable ***/

namespace
dango::detail_with_dango_swap
{
  using dango::swap;

  template
  <typename tp_lhs, typename tp_rhs>
  concept is_swappable_help =
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    {
      { swap(dango::forward<tp_lhs>(a_lhs), dango::forward<tp_rhs>(a_rhs)) }->dango::is_convertible_ret<void>;
      { swap(dango::forward<tp_rhs>(a_rhs), dango::forward<tp_lhs>(a_lhs)) }->dango::is_convertible_ret<void>;
    };

  template
  <typename tp_lhs, typename tp_rhs>
  concept is_noexcept_swappable_help =
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    {
      { swap(dango::forward<tp_lhs>(a_lhs), dango::forward<tp_rhs>(a_rhs)) }noexcept->dango::is_noexcept_convertible_ret<void>;
      { swap(dango::forward<tp_rhs>(a_rhs), dango::forward<tp_lhs>(a_lhs)) }noexcept->dango::is_noexcept_convertible_ret<void>;
    };
}

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept is_swappable =
    dango::detail_with_dango_swap::is_swappable_help<tp_lhs, tp_rhs>;

  template
  <typename tp_lhs, typename tp_rhs>
  concept is_noexcept_swappable =
    dango::is_swappable<tp_lhs, tp_rhs> &&
    dango::detail_with_dango_swap::is_noexcept_swappable_help<tp_lhs, tp_rhs>;
}

/*****************************************************************************************************************************/

/*** scalar compare ***/

namespace
dango
{
  template
  <dango::is_scalar tp_lhs, dango::is_scalar tp_rhs>
  requires
  (dango::is_comparable<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  compare
  (tp_lhs const a_lhs, tp_rhs const a_rhs)noexcept->dango::compare_val
  {
    return dango::compare_val{ dango::ssize(a_lhs > a_rhs) - dango::ssize(a_lhs < a_rhs) };
  }
}

/*** generic compare ***/

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  requires
  (
    (dango::is_class_or_union<tp_lhs> || dango::is_class_or_union<tp_rhs>) &&
    !dango::has_dango_operator_compare<tp_lhs const&, tp_rhs const&> &&
    !dango::has_dango_operator_compare<tp_rhs const&, tp_lhs const&> &&
    dango::is_comparable<tp_lhs const&, tp_rhs const&>
  )
  constexpr auto
  compare
  (tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::is_noexcept_comparable<tp_lhs const&, tp_rhs const&>)->dango::compare_val
  {
    return dango::compare_val{ dango::ssize(a_lhs > a_rhs) - dango::ssize(a_lhs < a_rhs) };
  }
}

/*** enum min max ***/

namespace
dango
{
  template
  <dango::is_enum tp_arg>
  requires(dango::is_comparable<tp_arg const&, tp_arg const&>)
  constexpr auto
  min
  (tp_arg const a_arg)noexcept->tp_arg
  {
    return a_arg;
  }

  template
  <dango::is_enum tp_arg>
  requires(dango::is_comparable<tp_arg const&, tp_arg const&>)
  constexpr auto
  min
  (tp_arg const a_arg1, tp_arg const a_arg2)noexcept(dango::is_noexcept_comparable<tp_arg const&, tp_arg const&>)->tp_arg
  {
    return a_arg1 < a_arg2 ? a_arg1 : a_arg2;
  }

  template
  <dango::is_enum tp_arg, typename... tp_args>
  requires
  ((sizeof...(tp_args) >= dango::usize(2)) && (dango::is_comparable<tp_arg const&, tp_arg const&> && ... && dango::is_same<tp_args, tp_arg>))
  constexpr auto
  min
  (tp_arg const a_arg, tp_args... a_args)noexcept(dango::is_noexcept_comparable<tp_arg const&, tp_arg const&>)->tp_arg
  {
    return dango::min(a_arg, dango::min(a_args...));
  }
}

namespace
dango
{
  template
  <dango::is_enum tp_arg>
  requires(dango::is_comparable<tp_arg const&, tp_arg const&>)
  constexpr auto
  max
  (tp_arg const a_arg)noexcept->tp_arg
  {
    return a_arg;
  }

  template
  <dango::is_enum tp_arg>
  requires(dango::is_comparable<tp_arg const&, tp_arg const&>)
  constexpr auto
  max
  (tp_arg const a_arg1, tp_arg const a_arg2)noexcept(dango::is_noexcept_comparable<tp_arg const&, tp_arg const&>)->tp_arg
  {
    return a_arg1 > a_arg2 ? a_arg1 : a_arg2;
  }

  template
  <dango::is_enum tp_arg, typename... tp_args>
  requires
  ((sizeof...(tp_args) >= dango::usize(2)) && (dango::is_comparable<tp_arg const&, tp_arg const&> && ... && dango::is_same<tp_args, tp_arg>))
  constexpr auto
  max
  (tp_arg const a_arg, tp_args... a_args)noexcept(dango::is_noexcept_comparable<tp_arg const&, tp_arg const&>)->tp_arg
  {
    return dango::max(a_arg, dango::max(a_args...));
  }
}

/*** generic min max ***/

namespace
dango
{
  template
  <typename tp_arg>
  requires
  (
    dango::is_class_or_union<dango::remove_ref<tp_arg>> &&
    dango::is_comparable<tp_arg, tp_arg> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg>
  )
  constexpr auto
  min
  (tp_arg&& a_arg)
  noexcept(dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg>)->dango::remove_cvref<tp_arg>
  {
    return dango::remove_cvref<tp_arg>{ dango::forward<tp_arg>(a_arg) };
  }

  template
  <typename tp_arg, typename tp_same>
  requires
  (
    dango::is_class_or_union<dango::remove_ref<tp_arg>> && dango::is_same_ignore_cvref<tp_arg, tp_same> &&
    dango::is_comparable<tp_arg, tp_same> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg>, tp_same>
  )
  constexpr auto
  min
  (tp_arg&& a_arg1, tp_same&& a_arg2)
  noexcept
  (
    dango::is_noexcept_comparable<tp_arg, tp_same> &&
    dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg> &&
    dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg>, tp_same>
  )->dango::remove_cvref<tp_arg>
  {
    using ret_type = dango::remove_cvref<tp_arg>;

    if(dango::forward<tp_arg>(a_arg1) < dango::forward<tp_same>(a_arg2))
    {
      return ret_type{ dango::forward<tp_arg>(a_arg1) };
    }
    else
    {
      return ret_type{ dango::forward<tp_same>(a_arg2) };
    }
  }

  template
  <typename tp_arg, typename... tp_args>
  requires
  (
    (sizeof...(tp_args) >= dango::usize(2)) &&
    (dango::is_class_or_union<dango::remove_ref<tp_arg>> && ... && dango::is_same_ignore_cvref<tp_arg, tp_args>) &&
    requires
    {
      { min(dango::declval<tp_args>()...) }->dango::is_same<dango::remove_cvref<tp_arg>>;
      { min(dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }->dango::is_same<dango::remove_cvref<tp_arg>>;
    }
  )
  constexpr auto
  min
  (tp_arg&&, tp_args&&...)
  noexcept
  (
    requires
    {
      { min(dango::declval<tp_args>()...) }noexcept;
      { min(dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }noexcept;
    }
  )->dango::remove_cvref<tp_arg>;
}

template
<typename tp_arg, typename... tp_args>
requires
(
  (sizeof...(tp_args) >= dango::usize(2)) &&
  (dango::is_class_or_union<dango::remove_ref<tp_arg>> && ... && dango::is_same_ignore_cvref<tp_arg, tp_args>) &&
  requires
  {
    { min(dango::declval<tp_args>()...) }->dango::is_same<dango::remove_cvref<tp_arg>>;
    { min(dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }->dango::is_same<dango::remove_cvref<tp_arg>>;
  }
)
constexpr auto
dango::
min
(tp_arg&& a_arg, tp_args&&... a_args)
noexcept
(
  requires
  {
    { min(dango::declval<tp_args>()...) }noexcept;
    { min(dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }noexcept;
  }
)->dango::remove_cvref<tp_arg>
{
  using ret_type = dango::remove_cvref<tp_arg>;

  return ret_type{ min(dango::forward<tp_arg>(a_arg), min(dango::forward<tp_args>(a_args)...)) };
}

namespace
dango
{
  template
  <typename tp_arg>
  requires
  (
    dango::is_class_or_union<dango::remove_ref<tp_arg>> &&
    dango::is_comparable<tp_arg, tp_arg> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg>
  )
  constexpr auto
  max
  (tp_arg&& a_arg)
  noexcept(dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg>)->dango::remove_cvref<tp_arg>
  {
    return dango::remove_cvref<tp_arg>{ dango::forward<tp_arg>(a_arg) };
  }

  template
  <typename tp_arg, typename tp_same>
  requires
  (
    dango::is_class_or_union<dango::remove_ref<tp_arg>> && dango::is_same_ignore_cvref<tp_arg, tp_same> &&
    dango::is_comparable<tp_arg, tp_same> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg>, tp_same>
  )
  constexpr auto
  max
  (tp_arg&& a_arg1, tp_same&& a_arg2)
  noexcept
  (
    dango::is_noexcept_comparable<tp_arg, tp_same> &&
    dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg> &&
    dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg>, tp_same>
  )->dango::remove_cvref<tp_arg>
  {
    using ret_type = dango::remove_cvref<tp_arg>;

    if(dango::forward<tp_arg>(a_arg1) > dango::forward<tp_same>(a_arg2))
    {
      return ret_type{ dango::forward<tp_arg>(a_arg1) };
    }
    else
    {
      return ret_type{ dango::forward<tp_same>(a_arg2) };
    }
  }

  template
  <typename tp_arg, typename... tp_args>
  requires
  (
    (sizeof...(tp_args) >= dango::usize(2)) &&
    (dango::is_class_or_union<dango::remove_ref<tp_arg>> && ... && dango::is_same_ignore_cvref<tp_arg, tp_args>) &&
    requires
    {
      { max(dango::declval<tp_args>()...) }->dango::is_same<dango::remove_cvref<tp_arg>>;
      { max(dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }->dango::is_same<dango::remove_cvref<tp_arg>>;
    }
  )
  constexpr auto
  max
  (tp_arg&&, tp_args&&...)
  noexcept
  (
    requires
    {
      { max(dango::declval<tp_args>()...) }noexcept;
      { max(dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }noexcept;
    }
  )->dango::remove_cvref<tp_arg>;
}

template
<typename tp_arg, typename... tp_args>
requires
(
  (sizeof...(tp_args) >= dango::usize(2)) &&
  (dango::is_class_or_union<dango::remove_ref<tp_arg>> && ... && dango::is_same_ignore_cvref<tp_arg, tp_args>) &&
  requires
  {
    { max(dango::declval<tp_args>()...) }->dango::is_same<dango::remove_cvref<tp_arg>>;
    { max(dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }->dango::is_same<dango::remove_cvref<tp_arg>>;
  }
)
constexpr auto
dango::
max
(tp_arg&& a_arg, tp_args&&... a_args)
noexcept
(
  requires
  {
    { max(dango::declval<tp_args>()...) }noexcept;
    { max(dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }noexcept;
  }
)->dango::remove_cvref<tp_arg>
{
  using ret_type = dango::remove_cvref<tp_arg>;

  return ret_type{ max(dango::forward<tp_arg>(a_arg), max(dango::forward<tp_args>(a_args)...)) };
}

#endif // DANGO_OPERATOR_HPP_INCLUDED

