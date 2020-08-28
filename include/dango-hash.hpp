#ifndef DANGO_HASH_HPP_INCLUDED
#define DANGO_HASH_HPP_INCLUDED

#include "dango-util-base.hpp"

/*** hash_val ***/

namespace
dango
{
  class hash_val;
}

class
dango::
hash_val
final
{
public:
  using int_type = dango::usize;
public:
  template
  <dango::is_uint tp_uint>
  constexpr hash_val(tp_uint)noexcept;
  DANGO_ALL_DEFAULT_CONSTEXPR_NOEXCEPT(hash_val, true)
  explicit constexpr operator int_type()const noexcept{ return m_value; }
public:
  constexpr auto as_int()const noexcept->int_type{ return m_value; }
  constexpr auto as_int_mod(dango::usize const a_mod)const noexcept->int_type;
private:
  int_type m_value;
};

template
<dango::is_uint tp_uint>
constexpr
dango::
hash_val::
hash_val
(tp_uint const a_val)noexcept:
m_value{ int_type(a_val) }
{
  constexpr auto const c_arg_width = dango::bit_width<tp_uint>;
  constexpr auto const c_int_width = dango::bit_width<int_type>;

  if constexpr(c_arg_width > c_int_width)
  {
    constexpr auto const c_shift = tp_uint(c_int_width);

    for(auto a_up = tp_uint(a_val >> c_shift); a_up != tp_uint(0); a_up >>= c_shift)
    {
      m_value ^= a_up;
    }
  }
}

constexpr auto
dango::
hash_val::
as_int_mod
(dango::usize const a_mod)const noexcept->int_type
{
  if(a_mod == int_type(0))
  {
    return m_value;
  }

  return m_value % a_mod;
}

/*** hash ***/

namespace
dango::custom
{
  template
  <typename tp_type DANGO_GCC_BUG_81043_WORKAROUND>
  struct operator_hash;
}

namespace
dango
{
  template
  <typename tp_type>
  concept has_operator_hash_struct =
    dango::is_object_ignore_ref<tp_type> &&
    requires(tp_type a_arg)
    { { dango::custom::operator_hash<dango::remove_cvref<tp_type>>::hash(dango::forward<tp_type>(a_arg)) }->dango::is_convertible_ret<dango::hash_val>; };

  template
  <typename tp_type>
  concept has_noexcept_operator_hash_struct =
    dango::has_operator_hash_struct<tp_type> &&
    requires(tp_type a_arg)
    { { dango::custom::operator_hash<dango::remove_cvref<tp_type>>::hash(dango::forward<tp_type>(a_arg)) }noexcept->dango::is_noexcept_convertible_ret<dango::hash_val>; };

  template
  <typename tp_type>
  concept has_operator_hash_method =
    dango::is_class_or_union_ignore_ref<tp_type> &&
    requires(tp_type a_arg){ { dango::forward<tp_type>(a_arg).dango_operator_hash() }->dango::is_convertible_ret<dango::hash_val>; };

  template
  <typename tp_type>
  concept has_noexcept_operator_hash_method =
    dango::has_operator_hash_method<tp_type> &&
    requires(tp_type a_arg){ { dango::forward<tp_type>(a_arg).dango_operator_hash() }noexcept->dango::is_noexcept_convertible_ret<dango::hash_val>; };
}

namespace
dango::detail
{
  using hash_help_prio = dango::priority_tag<dango::uint(1)>;

  template
  <typename tp_type>
  requires(dango::has_operator_hash_struct<tp_type const&>)
  constexpr auto
  hash_help
  (dango::priority_tag<dango::uint(1)> const, tp_type const& a_arg)
  noexcept(dango::has_noexcept_operator_hash_struct<tp_type const&>)->dango::hash_val
  {
    return dango::custom::operator_hash<dango::remove_cv<tp_type>>::hash(a_arg);
  }

  template
  <typename tp_type>
  requires(dango::has_operator_hash_method<tp_type const&>)
  constexpr auto
  hash_help
  (dango::priority_tag<dango::uint(0)> const, tp_type const& a_arg)
  noexcept(dango::has_noexcept_operator_hash_method<tp_type const&>)->dango::hash_val
  {
    return a_arg.dango_operator_hash();
  }
}

/*** is_hashable is_noexcept_hashable hash ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_hashable =
    dango::is_equatable<tp_type, tp_type> &&
    requires(tp_type a_arg)
    { { dango::detail::hash_help(dango::detail::hash_help_prio{ }, dango::forward<tp_type>(a_arg)) }->dango::is_convertible_ret<dango::hash_val>; };

  template
  <typename tp_type>
  concept is_noexcept_hashable =
    dango::is_hashable<tp_type> &&
    dango::is_noexcept_equatable<tp_type, tp_type> &&
    requires(tp_type a_arg)
    { { dango::detail::hash_help(dango::detail::hash_help_prio{ }, dango::forward<tp_type>(a_arg)) }noexcept->dango::is_noexcept_convertible_ret<dango::hash_val>; };

  inline constexpr auto const hash =
    []<typename tp_arg>
    (tp_arg const& a_arg)constexpr
    noexcept(dango::is_noexcept_hashable<tp_arg const&>)->dango::hash_val
    requires(dango::is_hashable<tp_arg const&>)
    {
      return dango::detail::hash_help(dango::detail::hash_help_prio{ }, a_arg);
    };
}

// TODO placeholder implementations

namespace
dango::custom
{
  template
  <dango::is_integral tp_type>
  struct
  operator_hash<tp_type DANGO_GCC_BUG_81043_WORKAROUND_ID(0, tp_type)>;

  template
  <dango::is_float tp_type>
  struct
  operator_hash<tp_type DANGO_GCC_BUG_81043_WORKAROUND_ID(1, tp_type)>;

  template
  <typename tp_type>
  struct
  operator_hash<tp_type*>;
}

template
<dango::is_integral tp_type>
struct
dango::
custom::
operator_hash<tp_type DANGO_GCC_BUG_81043_WORKAROUND_ID(0, tp_type)>
final
{
  static constexpr auto
  hash
  (tp_type const a_val)noexcept->dango::hash_val
  {
    return dango::hash_val{ static_cast<dango::make_uint<tp_type>>(a_val) };
  }

  DANGO_UNCONSTRUCTIBLE(operator_hash)
};

template
<dango::is_float tp_type>
struct
dango::
custom::
operator_hash<tp_type DANGO_GCC_BUG_81043_WORKAROUND_ID(1, tp_type)>
final
{
  static auto
  hash
  (float const a_val)noexcept->dango::hash_val
  {
    if(a_val == float(0))
    {
      return dango::hash_val{ dango::usize(0) };
    }

    dango::uint a_ret{ };

    static_assert(sizeof(a_val) == sizeof(a_ret));

    dango::mem_copy(&a_ret, &a_val, sizeof(a_val));

    return dango::hash_val{ a_ret };
  }

  static auto
  hash
  (double const a_val)noexcept->dango::hash_val
  {
    if(a_val == double(0))
    {
      return dango::hash_val{ dango::usize(0) };
    }

    dango::ulong a_ret{ };

    static_assert(sizeof(a_val) == sizeof(a_ret));

    dango::mem_copy(&a_ret, &a_val, sizeof(a_val));

    return dango::hash_val{ a_ret };
  }

  static auto
  hash
  (dango::real const a_val)noexcept->dango::hash_val
  {
    using dango::usize;

    if(a_val == dango::real(0))
    {
      return dango::hash_val{ usize(0) };
    }

    dango::ulong a_ret[usize(2)]{ };

    static_assert(sizeof(a_val) <= sizeof(a_ret));

    dango::mem_copy(a_ret, &a_val, sizeof(a_val));

    return dango::hash_val{ a_ret[usize(0)] ^ a_ret[usize(1)] };
  }

  DANGO_UNCONSTRUCTIBLE(operator_hash)
};

template
<typename tp_type>
struct
dango::
custom::
operator_hash<tp_type*>
final
{
  static auto
  hash
  (void const volatile* const a_val)noexcept->dango::hash_val
  {
    return dango::hash_val{ dango::ptr_as_uint(a_val) };
  }

  DANGO_UNCONSTRUCTIBLE(operator_hash)
};

#endif
