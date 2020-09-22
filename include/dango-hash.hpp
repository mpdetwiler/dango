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
  using value_type = dango::usize;
public:
  template
  <dango::is_uint tp_uint>
  constexpr hash_val(tp_uint)noexcept;
  DANGO_ALL_DEFAULT_CONSTEXPR_NOEXCEPT(hash_val, true)
  explicit constexpr operator value_type()const noexcept{ return m_value; }
public:
  constexpr auto as_int()const noexcept->value_type{ return m_value; }
  constexpr auto as_int_mod(value_type const a_mod)const noexcept->value_type;
  constexpr void combine_with(dango::hash_val const&)noexcept;
private:
  value_type m_value;
};

template
<dango::is_uint tp_uint>
constexpr
dango::
hash_val::
hash_val
(tp_uint const a_val)noexcept:
m_value{ value_type(a_val) }
{
  constexpr auto const c_arg_width = dango::bit_width<tp_uint>;
  constexpr auto const c_int_width = dango::bit_width<value_type>;

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
(value_type const a_mod)const noexcept->value_type
{
  if(a_mod == value_type(0))
  {
    return m_value;
  }

  return m_value % a_mod;
}

constexpr void
dango::
hash_val::
combine_with
(dango::hash_val const& a_val)noexcept
{
  auto const a_ls = value_type(as_int() << value_type(6));
  auto const a_rs = value_type(as_int() >> value_type(2));

  if constexpr(dango::bit_width<value_type> <= dango::usize(32))
  {
    constexpr auto const c_num = dango::uint(0x9E'37'79'B9LU);

    m_value ^= value_type(a_val.as_int() + c_num + a_ls + a_rs);
  }
  else
  {
    constexpr auto const c_num = dango::ulong(0x9E'37'79'B9'7F'4A'7C'16LLU);

    m_value ^= value_type(a_val.as_int() + c_num + a_ls + a_rs);
  }
}

/*** hash ***/

namespace
dango::custom
{
  template
  <typename tp_type>
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
    { { dango::custom::operator_hash<dango::remove_cvref<tp_type>>::hash(dango::forward<tp_type>(a_arg)) }->dango::is_noexcept_convertible_ret<dango::hash_val>; };

  template
  <typename tp_type>
  concept has_noexcept_operator_hash_struct =
    dango::has_operator_hash_struct<tp_type> &&
    requires(tp_type a_arg)
    { { dango::custom::operator_hash<dango::remove_cvref<tp_type>>::hash(dango::forward<tp_type>(a_arg)) }noexcept; };

  template
  <typename tp_type>
  concept has_operator_hash_method =
    dango::is_class_or_union_ignore_ref<tp_type> &&
    requires(tp_type a_arg){ { dango::forward<tp_type>(a_arg).dango_operator_hash() }->dango::is_noexcept_convertible_ret<dango::hash_val>; };

  template
  <typename tp_type>
  concept has_noexcept_operator_hash_method =
    dango::has_operator_hash_method<tp_type> &&
    requires(tp_type a_arg){ { dango::forward<tp_type>(a_arg).dango_operator_hash() }noexcept; };
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
  concept has_hash =
    dango::is_referenceable_ignore_ref<tp_type> &&
    requires(tp_type a_arg)
    { { dango::detail::hash_help(dango::detail::hash_help_prio{ }, dango::forward<tp_type>(a_arg)) }->dango::is_same<dango::hash_val>; };

  template
  <typename tp_type>
  concept has_noexcept_hash =
    dango::has_hash<tp_type> &&
    requires(tp_type a_arg)
    { { dango::detail::hash_help(dango::detail::hash_help_prio{ }, dango::forward<tp_type>(a_arg)) }noexcept; };

  template
  <typename tp_type>
  concept is_hashable =
    dango::has_hash<tp_type> &&
    dango::is_equatable<tp_type>;

  template
  <typename tp_type>
  concept is_noexcept_hashable =
    dango::is_hashable<tp_type> &&
    dango::has_noexcept_hash<tp_type> &&
    dango::is_noexcept_equatable<tp_type>;

  inline constexpr auto const hash =
    []<typename tp_arg>
    (tp_arg const& a_arg)constexpr
    noexcept(dango::has_noexcept_hash<tp_arg const&>)->dango::hash_val
    requires(dango::is_hashable<tp_arg const&>)
    {
      return dango::detail::hash_help(dango::detail::hash_help_prio{ }, a_arg);
    };

  inline constexpr auto const multi_hash =
    []<typename... tp_args>
    (tp_args const&... a_args)constexpr
    noexcept(( ... && dango::has_noexcept_hash<tp_args const&>))->dango::hash_val
    requires(( ... && dango::is_hashable<tp_args const&>))
    {
      dango::hash_val a_ret{ dango::usize(0) };

      ( ... , void(a_ret.combine_with(dango::hash(a_args))));

      return a_ret;
    };
}

namespace
dango::custom
{
  template<>
  struct
  operator_hash<bool>
  final
  {
    static constexpr auto
    hash
    (bool const a_val)noexcept->dango::hash_val
    {
      return dango::hash_val{ dango::usize(a_val) };
    }
  };

#define DANGO_DEFINE_INTEGRAL_HASH(type) \
  template<> struct operator_hash<type> final \
  { \
    using uint_type = dango::make_uint<type>; \
    static constexpr auto \
    hash(type const a_val)noexcept->dango::hash_val \
    { return dango::hash_val{ uint_type(a_val) }; } \
    DANGO_UNCONSTRUCTIBLE(operator_hash) \
  };

  DANGO_DEFINE_INTEGRAL_HASH(dango::integer::u_char)
  DANGO_DEFINE_INTEGRAL_HASH(dango::integer::u_short)
  DANGO_DEFINE_INTEGRAL_HASH(dango::integer::u_int)
  DANGO_DEFINE_INTEGRAL_HASH(dango::integer::u_long)
  DANGO_DEFINE_INTEGRAL_HASH(dango::integer::u_longlong)

  DANGO_DEFINE_INTEGRAL_HASH(dango::integer::s_char)
  DANGO_DEFINE_INTEGRAL_HASH(dango::integer::s_short)
  DANGO_DEFINE_INTEGRAL_HASH(dango::integer::s_int)
  DANGO_DEFINE_INTEGRAL_HASH(dango::integer::s_long)
  DANGO_DEFINE_INTEGRAL_HASH(dango::integer::s_longlong)

  DANGO_DEFINE_INTEGRAL_HASH(char)
  DANGO_DEFINE_INTEGRAL_HASH(wchar_t)
  DANGO_DEFINE_INTEGRAL_HASH(dango::bchar)
  DANGO_DEFINE_INTEGRAL_HASH(dango::wchar)
  DANGO_DEFINE_INTEGRAL_HASH(dango::dchar)

#undef DANGO_DEFINE_INTEGRAL_HASH

  template<>
  struct
  operator_hash<float>;

  template<>
  struct
  operator_hash<double>;

  template<>
  struct
  operator_hash<dango::real>;

  template
  <typename tp_type>
  struct
  operator_hash<tp_type*>;

  template
  <typename tp_ret, typename... tp_args, bool tp_noexcept>
  struct
  operator_hash<tp_ret(*)(tp_args...)noexcept(tp_noexcept)>;

  template
  <typename tp_type, dango::usize tp_size>
  struct
  operator_hash<tp_type[tp_size]>;
}

template<>
struct
dango::
custom::
operator_hash<float>
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

  DANGO_UNCONSTRUCTIBLE(operator_hash)
};

template<>
struct
dango::
custom::
operator_hash<double>
final
{
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

  DANGO_UNCONSTRUCTIBLE(operator_hash)
};

template<>
struct
dango::
custom::
operator_hash<dango::real>
final
{
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

template
<typename tp_ret, typename... tp_args, bool tp_noexcept>
struct
dango::
custom::
operator_hash<tp_ret(*)(tp_args...)noexcept(tp_noexcept)>
{
  using ptr_type = tp_ret(*)(tp_args...)noexcept(tp_noexcept);

  static auto
  hash
  (ptr_type const a_val)noexcept->dango::hash_val
  {
    dango::uptr a_ret{ };

    static_assert(sizeof(a_val) == sizeof(a_ret));

    dango::mem_copy(&a_ret, &a_val, sizeof(a_val));

    return dango::hash_val{ a_ret };
  }

  DANGO_UNCONSTRUCTIBLE(operator_hash)
};

template
<typename tp_elem, dango::usize tp_size>
struct
dango::
custom::
operator_hash<tp_elem[tp_size]>
final
{
  template
  <dango::is_same_ignore_cv<tp_elem> tp_arg>
  requires(dango::is_hashable<tp_arg const&>)
  static constexpr auto
  hash
  (tp_arg const(& a_vals)[tp_size])
  noexcept(dango::has_noexcept_hash<tp_arg const&>)->dango::hash_val
  {
    dango::hash_val a_ret{ dango::usize(0) };

    for(auto const& a_val : a_vals)
    {
      a_ret.combine_with(dango::hash(a_val));
    }

    return a_ret;
  }

  DANGO_UNCONSTRUCTIBLE(operator_hash)
};

#endif
