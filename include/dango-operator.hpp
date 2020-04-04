#ifndef DANGO_OPERATOR_HPP_INCLUDED
#define DANGO_OPERATOR_HPP_INCLUDED

/*** dango_operator_is_null ***/

namespace
dango
{
  template
  <typename tp_type>
  concept has_dango_operator_is_null =
    dango::is_class<tp_type> && !dango::is_volatile<tp_type> &&
    requires(tp_type const& a_arg){ { a_arg.dango_operator_is_null() }->dango::is_convertible_ret<bool>; };

  template
  <typename tp_type>
  concept has_noexcept_dango_operator_is_null =
    dango::has_dango_operator_is_null<tp_type> &&
    requires(tp_type const& a_arg){ { a_arg.dango_operator_is_null() }noexcept->dango::is_noexcept_convertible_ret<bool>; };
}

namespace
dango
{
  template
  <dango::has_dango_operator_is_null tp_type>
  constexpr auto
  operator ==
  (tp_type const& a_arg, dango::null_tag const)noexcept(dango::has_noexcept_dango_operator_is_null<tp_type>)->bool
  {
    return a_arg.dango_operator_is_null();
  }

  template
  <dango::has_dango_operator_is_null tp_type>
  constexpr auto
  operator ==
  (dango::null_tag const, tp_type const& a_arg)noexcept(dango::has_noexcept_dango_operator_is_null<tp_type>)->bool
  {
    return a_arg.dango_operator_is_null();
  }

  template
  <dango::has_dango_operator_is_null tp_type>
  constexpr auto
  operator !=
  (tp_type const& a_arg, dango::null_tag const)noexcept(dango::has_noexcept_dango_operator_is_null<tp_type>)->bool
  {
    return !a_arg.dango_operator_is_null();
  }

  template
  <dango::has_dango_operator_is_null tp_type>
  constexpr auto
  operator !=
  (dango::null_tag const, tp_type const& a_arg)noexcept(dango::has_noexcept_dango_operator_is_null<tp_type>)->bool
  {
    return !a_arg.dango_operator_is_null();
  }
}

/*** dango_operator_equals ***/

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept has_dango_operator_equals =
    dango::is_class<tp_lhs> && dango::is_referenceable<tp_rhs> &&
    !dango::is_volatile<tp_lhs> && !dango::is_volatile<tp_rhs> &&
    requires(tp_lhs const& a_lhs, tp_rhs const& a_rhs)
    { { a_lhs.dango_operator_equals(a_rhs) }->dango::is_convertible_ret<bool>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_dango_operator_equals =
    dango::has_dango_operator_equals<tp_lhs, tp_rhs> &&
    requires(tp_lhs const& a_lhs, tp_rhs const& a_rhs)
    { { a_lhs.dango_operator_equals(a_rhs) }noexcept->dango::is_noexcept_convertible_ret<bool>; };
}

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_dango_operator_equals<tp_lhs, tp_rhs>)
  constexpr auto
  operator ==
  (tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_equals<tp_lhs, tp_rhs>)->bool
  {
    return a_lhs.dango_operator_equals(a_rhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_dango_operator_equals<tp_rhs, tp_lhs> && !dango::has_dango_operator_equals<tp_lhs, tp_rhs>)
  constexpr auto
  operator ==
  (tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_equals<tp_lhs, tp_rhs>)->bool
  {
    return a_rhs.dango_operator_equals(a_lhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_dango_operator_equals<tp_lhs, tp_rhs>)
  constexpr auto
  operator !=
  (tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_equals<tp_lhs, tp_rhs>)->bool
  {
    return !a_lhs.dango_operator_equals(a_rhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_dango_operator_equals<tp_rhs, tp_lhs> && !dango::has_dango_operator_equals<tp_lhs, tp_rhs>)
  constexpr auto
  operator !=
  (tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::has_noexcept_dango_operator_equals<tp_lhs, tp_rhs>)->bool
  {
    return !a_rhs.dango_operator_equals(a_lhs);
  }
}

/*** is_equatable ***/

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept is_equatable =
    dango::is_referenceable<tp_lhs> && dango::is_referenceable<tp_rhs> &&
    requires(dango::remove_cv<tp_lhs> const& a_lhs, dango::remove_cv<tp_rhs> const& a_rhs)
    {
      { a_lhs == a_rhs }->dango::is_convertible_ret<bool>;
      { a_lhs != a_rhs }->dango::is_convertible_ret<bool>;
      { a_lhs == dango::declval<dango::remove_cv<tp_rhs>&&>() }->dango::is_convertible_ret<bool>;
      { a_lhs != dango::declval<dango::remove_cv<tp_rhs>&&>() }->dango::is_convertible_ret<bool>;
      { dango::declval<dango::remove_cv<tp_lhs>&&>() == a_rhs }->dango::is_convertible_ret<bool>;
      { dango::declval<dango::remove_cv<tp_lhs>&&>() != a_rhs }->dango::is_convertible_ret<bool>;
      { dango::declval<dango::remove_cv<tp_lhs>&&>() == dango::declval<dango::remove_cv<tp_rhs>&&>() }->dango::is_convertible_ret<bool>;
      { dango::declval<dango::remove_cv<tp_lhs>&&>() != dango::declval<dango::remove_cv<tp_rhs>&&>() }->dango::is_convertible_ret<bool>;
    };

  template
  <typename tp_lhs, typename tp_rhs>
  concept is_noexcept_equatable =
    dango::is_equatable<tp_lhs, tp_rhs> &&
    requires(dango::remove_cv<tp_lhs> const& a_lhs, dango::remove_cv<tp_rhs> const& a_rhs)
    {
      { a_lhs == a_rhs }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { a_lhs != a_rhs }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { a_lhs == dango::declval<dango::remove_cv<tp_rhs>&&>() }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { a_lhs != dango::declval<dango::remove_cv<tp_rhs>&&>() }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { dango::declval<dango::remove_cv<tp_lhs>&&>() == a_rhs }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { dango::declval<dango::remove_cv<tp_lhs>&&>() != a_rhs }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { dango::declval<dango::remove_cv<tp_lhs>&&>() == dango::declval<dango::remove_cv<tp_rhs>&&>() }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { dango::declval<dango::remove_cv<tp_lhs>&&>() != dango::declval<dango::remove_cv<tp_rhs>&&>() }noexcept->dango::is_noexcept_convertible_ret<bool>;
    };
}

/*** is_nullable ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_null_equatable =
    dango::is_equatable<tp_type, dango::null_tag> && dango::is_equatable<dango::null_tag, tp_type>;

  template
  <typename tp_type>
  concept is_noexcept_null_equatable =
    dango::is_null_equatable<tp_type> &&
    dango::is_noexcept_equatable<tp_type, dango::null_tag> && dango::is_noexcept_equatable<dango::null_tag, tp_type>;

  template
  <typename tp_type>
  concept is_nullable =
    dango::is_null_equatable<tp_type> &&
    dango::is_constructible<dango::remove_cv<tp_type>, dango::null_tag const&> &&
    dango::is_constructible<dango::remove_cv<tp_type>, dango::null_tag&&> &&
    dango::is_convertible<dango::null_tag const&, dango::remove_cv<tp_type>> &&
    dango::is_convertible<dango::null_tag&&, dango::remove_cv<tp_type>> &&
    dango::is_assignable<dango::remove_cv<tp_type>&, dango::null_tag const&> &&
    dango::is_assignable<dango::remove_cv<tp_type>&, dango::null_tag&&>;
}

#endif // DANGO_OPERATOR_HPP_INCLUDED

