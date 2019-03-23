#ifndef __DANGO_OPERATOR_HPP__
#define __DANGO_OPERATOR_HPP__

/*** dango_operator_is_null ***/

namespace
dango
{
  constexpr auto operator == (dango::null_tag, dango::null_tag)noexcept->bool;
  constexpr auto operator != (dango::null_tag, dango::null_tag)noexcept->bool;

  template
  <
    typename tp_type,
    typename tp_ret = decltype(dango::declval<tp_type const&>().dango_operator_is_null())
  >
  constexpr auto
  operator == (tp_type const&, dango::null_tag)
  noexcept(noexcept(dango::declval<tp_type const&>().dango_operator_is_null()))->
  dango::enable_if<dango::is_noexcept_convertible<tp_ret, bool>, bool>;

  template
  <
    typename tp_type,
    typename tp_ret = decltype(dango::declval<tp_type const&>().dango_operator_is_null())
  >
  constexpr auto
  operator == (dango::null_tag, tp_type const&)
  noexcept(noexcept(dango::declval<tp_type const&>().dango_operator_is_null()))->
  dango::enable_if<dango::is_noexcept_convertible<tp_ret, bool>, bool>;

  template
  <
    typename tp_type,
    typename tp_ret = decltype(dango::declval<tp_type const&>().dango_operator_is_null())
  >
  constexpr auto
  operator != (tp_type const&, dango::null_tag)
  noexcept(noexcept(dango::declval<tp_type const&>().dango_operator_is_null()))->
  dango::enable_if<dango::is_noexcept_convertible<tp_ret, bool>, bool>;

  template
  <
    typename tp_type,
    typename tp_ret = decltype(dango::declval<tp_type const&>().dango_operator_is_null())
  >
  constexpr auto
  operator != (dango::null_tag, tp_type const&)
  noexcept(noexcept(dango::declval<tp_type const&>().dango_operator_is_null()))->
  dango::enable_if<dango::is_noexcept_convertible<tp_ret, bool>, bool>;
}

constexpr auto
dango::
operator ==
(dango::null_tag const, dango::null_tag const)noexcept->bool
{
  return true;
}

constexpr auto
dango::
operator !=
(dango::null_tag const, dango::null_tag const)noexcept->bool
{
  return false;
}

template
<
  typename tp_type,
  typename tp_ret
>
constexpr auto
dango::
operator == (tp_type const& a_arg, dango::null_tag const)
noexcept(noexcept(dango::declval<tp_type const&>().dango_operator_is_null()))->
dango::enable_if<dango::is_noexcept_convertible<tp_ret, bool>, bool>
{
  return a_arg.dango_operator_is_null();
}

template
<
  typename tp_type,
  typename tp_ret
>
constexpr auto
dango::
operator == (dango::null_tag const, tp_type const& a_arg)
noexcept(noexcept(dango::declval<tp_type const&>().dango_operator_is_null()))->
dango::enable_if<dango::is_noexcept_convertible<tp_ret, bool>, bool>
{
  return a_arg.dango_operator_is_null();
}

template
<
  typename tp_type,
  typename tp_ret
>
constexpr auto
dango::
operator != (tp_type const& a_arg, dango::null_tag const)
noexcept(noexcept(dango::declval<tp_type const&>().dango_operator_is_null()))->
dango::enable_if<dango::is_noexcept_convertible<tp_ret, bool>, bool>
{
  return !a_arg.dango_operator_is_null();
}

template
<
  typename tp_type,
  typename tp_ret
>
constexpr auto
dango::
operator != (dango::null_tag const, tp_type const& a_arg)
noexcept(noexcept(dango::declval<tp_type const&>().dango_operator_is_null()))->
dango::enable_if<dango::is_noexcept_convertible<tp_ret, bool>, bool>
{
  return !a_arg.dango_operator_is_null();
}


/*** dango_operator_equals ***/

namespace
dango
{
  template
  <
    typename tp_lhs,
    typename tp_rhs,
    typename tp_ret =
      decltype(dango::declval<tp_lhs const&>().dango_operator_equals(dango::declval<tp_rhs const&>()))
  >
  constexpr auto
  operator == (tp_lhs const&, tp_rhs const&)
  noexcept(noexcept(dango::declval<tp_lhs const&>().dango_operator_equals(dango::declval<tp_rhs const&>())))->
  dango::enable_if<dango::is_noexcept_convertible<tp_ret, bool>, bool>;

  template
  <
    typename tp_lhs,
    typename tp_rhs,
    typename tp_ret =
      decltype(dango::declval<tp_lhs const&>().dango_operator_equals(dango::declval<tp_rhs const&>()))
  >
  constexpr auto
  operator != (tp_lhs const&, tp_rhs const&)
  noexcept(noexcept(dango::declval<tp_lhs const&>().dango_operator_equals(dango::declval<tp_rhs const&>())))->
  dango::enable_if<dango::is_noexcept_convertible<tp_ret, bool>, bool>;
}

template
<
  typename tp_lhs,
  typename tp_rhs,
  typename tp_ret
>
constexpr auto
dango::
operator == (tp_lhs const& a_lhs, tp_rhs const& a_rhs)
noexcept(noexcept(dango::declval<tp_lhs const&>().dango_operator_equals(dango::declval<tp_rhs const&>())))->
dango::enable_if<dango::is_noexcept_convertible<tp_ret, bool>, bool>
{
  return a_lhs.dango_operator_equals(a_rhs);
}

template
<
  typename tp_lhs,
  typename tp_rhs,
  typename tp_ret
>
constexpr auto
dango::
operator != (tp_lhs const& a_lhs, tp_rhs const& a_rhs)
noexcept(noexcept(dango::declval<tp_lhs const&>().dango_operator_equals(dango::declval<tp_rhs const&>())))->
dango::enable_if<dango::is_noexcept_convertible<tp_ret, bool>, bool>
{
  return !a_lhs.dango_operator_equals(a_rhs);
}

#endif

