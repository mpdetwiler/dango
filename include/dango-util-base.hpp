#ifndef DANGO_UTIL_BASE_HPP_INCLUDED
#define DANGO_UTIL_BASE_HPP_INCLUDED

#include "dango-traits.hpp"
#include "dango-compare-val.hpp"

/*** in_constexpr_context ***/

namespace
dango
{
  constexpr auto in_constexpr_context()noexcept->bool;
}

constexpr auto
dango::
in_constexpr_context
()noexcept->bool
{
  return __builtin_is_constant_evaluated();
}

/*** likely unlikely ***/

namespace
dango
{
  constexpr auto likely(bool)noexcept->bool;
  constexpr auto unlikely(bool)noexcept->bool;
}

constexpr auto
dango::
likely
(bool const a_cond)noexcept->bool
{
  return bool(__builtin_expect(a_cond, true));
}

constexpr auto
dango::
unlikely
(bool const a_cond)noexcept->bool
{
  return bool(__builtin_expect(a_cond, false));
}

/*** assume ***/

namespace
dango
{
  constexpr void assume(bool)noexcept;
}

constexpr void
dango::
assume
(bool const a_cond)noexcept
{
  if(dango::unlikely(!a_cond))
  {
    __builtin_unreachable();
  }
}

/*** infinite_loop ***/

namespace
dango
{
  [[noreturn]] DANGO_EXPORT void infinite_loop()noexcept;
}

/*** trap_instruction ***/

namespace
dango
{
  [[noreturn]] void trap_instruction()noexcept;
}

inline void
dango::
trap_instruction()noexcept
{
  __builtin_trap();

  dango::infinite_loop();
}

/*** bchar_as_char char_as_bchar ***/

namespace
dango
{
  template
  <dango::is_same_ignore_cv<dango::bchar> tp_char>
  constexpr auto
  bchar_as_char
  (tp_char* const a_ptr)noexcept->auto
  {
    using ret_type = dango::copy_cv<tp_char, char>*;

    if(a_ptr)
    {
      return reinterpret_cast<ret_type>(a_ptr);
    }
    else
    {
      return static_cast<ret_type>(dango::null);
    }
  }

  template
  <dango::is_same_ignore_cv<char> tp_char>
  constexpr auto
  char_as_bchar
  (tp_char* const a_ptr)noexcept->auto
  {
    using ret_type = dango::copy_cv<tp_char, dango::bchar>*;

    if(a_ptr)
    {
      return reinterpret_cast<ret_type>(a_ptr);
    }
    else
    {
      return static_cast<ret_type>(dango::null);
    }
  }
}

/*** is_pow_two ***/

namespace
dango
{
  template
  <dango::is_int tp_int>
  constexpr auto
  is_pow_two
  (tp_int const a_arg)noexcept->bool
  {
    if(a_arg <= tp_int(0))
    {
      return false;
    }

    return tp_int(a_arg & (a_arg - tp_int(1))) == tp_int(0);
  }
}

/*** logic ***/

namespace
dango
{
  constexpr auto logic_implies(bool const a_lhs, bool const a_rhs)noexcept->bool{ return !a_lhs || a_rhs; }
  constexpr auto logic_equivalent(bool const a_lhs, bool const a_rhs)noexcept->bool{ return a_lhs == a_rhs; }
  constexpr auto logic_and(bool const a_lhs, bool const a_rhs)noexcept->bool{ return a_lhs && a_rhs; }
  constexpr auto logic_or(bool const a_lhs, bool const a_rhs)noexcept->bool{ return a_lhs || a_rhs; }
  constexpr auto logic_xor(bool const a_lhs, bool const a_rhs)noexcept->bool{ return a_lhs != a_rhs; }
  constexpr auto logic_not(bool const a_arg)noexcept->bool{ return !a_arg; }
}

/*** next_multiple ***/

namespace
dango
{
  template
  <dango::is_uint tp_uint>
  constexpr auto
  next_multiple
  (tp_uint const a_arg1, tp_uint const a_arg2)noexcept->tp_uint
  {
    tp_uint const a_div = a_arg1 / a_arg2;
    tp_uint const a_mod = a_arg1 % a_arg2;

    return (a_mod != tp_uint(0)) ? (a_arg2 * (a_div + tp_uint(1))) : a_arg1;
  }
}

/*** destructor destructor_as ***/

namespace
dango
{
  template
  <dango::is_object_exclude_array tp_type>
  requires(dango::is_destructible<tp_type>)
  constexpr void
  destructor
  (tp_type const volatile* const a_ptr)noexcept(dango::is_noexcept_destructible<tp_type>)
  {
    if(a_ptr)
    {
      a_ptr->~tp_type();
    }
  }

  template
  <dango::is_object_exclude_array tp_type_as, dango::is_object_exclude_array tp_type>
  requires(requires{ { dango::declval<tp_type const volatile* const&>()->tp_type_as::~tp_type_as() }; })
  constexpr void
  destructor_as
  (tp_type const volatile* const a_ptr)
  noexcept(requires{ { dango::declval<tp_type const volatile* const&>()->tp_type_as::~tp_type_as() }noexcept; })
  {
    if(a_ptr)
    {
      a_ptr->tp_type_as::~tp_type_as();
    }
  }
}

/*** launder ***/

namespace
dango
{
  template
  <dango::is_object tp_type>
  [[nodiscard]] constexpr auto
  launder(tp_type* const a_ptr)noexcept->tp_type*
  {
    return __builtin_launder(a_ptr);
  }
}

/*** addressof ***/

namespace
dango
{
  template
  <typename tp_type>
  requires(dango::is_object_ignore_ref<tp_type> && dango::is_lvalue_ref<tp_type>)
  constexpr auto
  addressof
  (tp_type&& a_arg)noexcept->dango::remove_ref<tp_type>*
  {
    return __builtin_addressof(a_arg);
  }

  template
  <typename tp_type>
  requires(dango::is_object_ignore_ref<tp_type> && !dango::is_lvalue_ref<tp_type>)
  constexpr auto
  addressof
  (tp_type&&)noexcept = delete;
}

/*** ptr_as_uint ptr_as_sint ***/

namespace
dango
{
  constexpr auto ptr_as_uint(void const volatile*)noexcept->dango::uptr;
  constexpr auto ptr_as_sint(void const volatile*)noexcept->dango::sptr;
}

constexpr auto
dango::
ptr_as_uint
(void const volatile* const a_ptr)noexcept->dango::uptr
{
  using ret_type = dango::uptr;

  if(a_ptr)
  {
    return reinterpret_cast<ret_type>(a_ptr);
  }
  else
  {
    return ret_type(0);
  }
}

constexpr auto
dango::
ptr_as_sint
(void const volatile* const a_ptr)noexcept->dango::sptr
{
  using ret_type = dango::sptr;

  if(a_ptr)
  {
    return reinterpret_cast<ret_type>(a_ptr);
  }
  else
  {
    return ret_type(0);
  }
}

/*** volatile_load volatile_store ***/

namespace
dango
{
  template
  <dango::is_scalar tp_type>
  constexpr auto
  volatile_load
  (tp_type const volatile* const a_addr)noexcept->tp_type
  {
    return *a_addr;
  }

  template
  <dango::is_scalar tp_type, typename tp_arg>
  requires
  (
    !dango::is_const<tp_type> &&
    dango::is_brace_constructible<tp_type, tp_arg> &&
    dango::is_convertible_arg<tp_arg, tp_type>
  )
  constexpr void
  volatile_store
  (tp_type volatile* const a_addr, tp_arg&& a_arg)
  noexcept(dango::is_noexcept_brace_constructible<tp_type, tp_arg>)
  {
    *a_addr = tp_type{ dango::forward<tp_arg>(a_arg) };
  }
}

/*** mem_copy ***/

namespace
dango
{
  constexpr auto mem_copy(void*, void const*, dango::usize)noexcept->void*;
}

constexpr auto
dango::
mem_copy
(
  void* const a_dest,
  void const* const a_source,
  dango::usize const a_count
)
noexcept->void*
{
  return __builtin_memcpy(a_dest, a_source, a_count);
}

/*** swap ***/

namespace
dango::custom
{
  template
  <typename tp_lhs, typename tp_rhs>
  struct operator_swap;
}

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept has_operator_swap_struct =
    dango::is_object_ignore_ref<tp_lhs> && dango::is_object_ignore_ref<tp_rhs> &&
    requires
    { { dango::custom::operator_swap<dango::remove_cvref<tp_lhs>, dango::remove_cvref<tp_rhs>>::swap(dango::declval<tp_lhs>(), dango::declval<tp_rhs>()) }->dango::is_convertible_ret<void>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_operator_swap_struct =
    dango::has_operator_swap_struct<tp_lhs, tp_rhs> &&
    requires
    { { dango::custom::operator_swap<dango::remove_cvref<tp_lhs>, dango::remove_cvref<tp_rhs>>::swap(dango::declval<tp_lhs>(), dango::declval<tp_rhs>()) }noexcept->dango::is_noexcept_convertible_ret<void>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_operator_swap_method =
    dango::is_class_or_union_ignore_ref<tp_lhs> && dango::is_object_ignore_ref<tp_rhs> &&
    requires
    { { dango::declval<tp_lhs>().dango_operator_swap(dango::declval<tp_rhs>()) }->dango::is_convertible_ret<void>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_operator_swap_method =
    dango::has_operator_swap_method<tp_lhs, tp_rhs> &&
    requires
    { { dango::declval<tp_lhs>().dango_operator_swap(dango::declval<tp_rhs>()) }noexcept->dango::is_noexcept_convertible_ret<void>; };
}

namespace
dango::detail
{
  using swap_help_prio = dango::priority_tag<dango::uint(4)>;

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_swap_struct<tp_lhs, tp_rhs>)
  constexpr void
  swap_help
  (dango::priority_tag<dango::uint(4)> const, tp_lhs&& a_lhs, tp_rhs&& a_rhs)
  noexcept(dango::has_noexcept_operator_swap_struct<tp_lhs, tp_rhs>)
  {
    dango::custom::operator_swap<dango::remove_cvref<tp_lhs>, dango::remove_cvref<tp_rhs>>::swap(dango::forward<tp_lhs>(a_lhs), dango::forward<tp_rhs>(a_rhs));
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_swap_struct<tp_rhs, tp_lhs>)
  constexpr void
  swap_help
  (dango::priority_tag<dango::uint(3)> const, tp_lhs&& a_lhs, tp_rhs&& a_rhs)
  noexcept(dango::has_noexcept_operator_swap_struct<tp_rhs, tp_lhs>)
  {
    dango::custom::operator_swap<dango::remove_cvref<tp_rhs>, dango::remove_cvref<tp_lhs>>::swap(dango::forward<tp_rhs>(a_rhs), dango::forward<tp_lhs>(a_lhs));
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_swap_method<tp_lhs, tp_rhs>)
  constexpr void
  swap_help
  (dango::priority_tag<dango::uint(2)> const, tp_lhs&& a_lhs, tp_rhs&& a_rhs)
  noexcept(dango::has_noexcept_operator_swap_method<tp_lhs, tp_rhs>)
  {
    dango::forward<tp_lhs>(a_lhs).dango_operator_swap(dango::forward<tp_rhs>(a_rhs));
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_swap_method<tp_rhs, tp_lhs>)
  constexpr void
  swap_help
  (dango::priority_tag<dango::uint(1)> const, tp_lhs&& a_lhs, tp_rhs&& a_rhs)
  noexcept(dango::has_noexcept_operator_swap_method<tp_rhs, tp_lhs>)
  {
    dango::forward<tp_rhs>(a_rhs).dango_operator_swap(dango::forward<tp_lhs>(a_lhs));
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires
  (
    !dango::is_const<tp_lhs> && !dango::is_const<tp_rhs> &&
    dango::is_brace_constructible<dango::remove_volatile<tp_lhs>, tp_lhs&&> &&
    dango::is_assignable<tp_lhs&, tp_rhs&&> &&
    dango::is_assignable<tp_rhs&, dango::remove_volatile<tp_lhs>&&>
  )
  constexpr void
  swap_help
  (dango::priority_tag<dango::uint(0)> const, tp_lhs& a_lhs, tp_rhs& a_rhs)
  noexcept
  (
    dango::is_noexcept_brace_constructible<dango::remove_volatile<tp_lhs>, tp_lhs&&> &&
    dango::is_noexcept_assignable<tp_lhs&, tp_rhs&&> &&
    dango::is_noexcept_assignable<tp_rhs&, dango::remove_volatile<tp_lhs>&&>
  )
  {
    dango::remove_volatile<tp_lhs> a_temp{ dango::move(a_lhs) };

    a_lhs = dango::move(a_rhs);

    a_rhs = dango::move(a_temp);
  }
}

/*** is_swappable is_noexcept_swappable swap ***/

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs = tp_lhs>
  concept is_swappable =
    dango::is_object_ignore_ref<tp_lhs> && dango::is_object_ignore_ref<tp_rhs> &&
    requires
    { { dango::detail::swap_help(dango::detail::swap_help_prio{ }, dango::declval<tp_lhs>(), dango::declval<tp_rhs>()) }->dango::is_convertible_ret<void>; };

  template
  <typename tp_lhs, typename tp_rhs = tp_lhs>
  concept is_noexcept_swappable =
    dango::is_swappable<tp_lhs, tp_rhs> &&
    requires
    { { dango::detail::swap_help(dango::detail::swap_help_prio{ }, dango::declval<tp_lhs>(), dango::declval<tp_rhs>()) }noexcept->dango::is_noexcept_convertible_ret<void>; };

  inline constexpr auto const swap =
    []<typename tp_lhs, typename tp_rhs>
    (tp_lhs&& a_lhs, tp_rhs&& a_rhs)constexpr
    noexcept(dango::is_noexcept_swappable<tp_lhs, tp_rhs>)->void
    requires(dango::is_swappable<tp_lhs, tp_rhs>)
    {
      dango::detail::swap_help(dango::detail::swap_help_prio{ }, dango::forward<tp_lhs>(a_lhs), dango::forward<tp_rhs>(a_rhs));
    };
}

/*** exchange ***/

namespace
dango
{
  inline constexpr auto const exchange =
    []<typename tp_lhs, typename tp_rhs>
    (tp_lhs& a_lhs, tp_rhs&& a_rhs)constexpr
    noexcept
    (
      dango::is_noexcept_brace_constructible<dango::remove_volatile<tp_lhs>, tp_rhs> &&
      dango::is_noexcept_swappable<dango::remove_volatile<tp_lhs>&, tp_lhs&> &&
      dango::is_noexcept_move_constructible<dango::remove_volatile<tp_lhs>>
    )->dango::remove_volatile<tp_lhs>
    requires
    (
      !dango::is_const<tp_lhs> &&
      dango::is_brace_constructible<dango::remove_volatile<tp_lhs>, tp_rhs> &&
      dango::is_swappable<dango::remove_volatile<tp_lhs>&, tp_lhs&> &&
      dango::is_move_constructible<dango::remove_volatile<tp_lhs>>
    )
    {
      dango::remove_volatile<tp_lhs> a_temp{ dango::forward<tp_rhs>(a_rhs) };

      dango::swap(a_temp, a_lhs);

      return a_temp;
    };
}

/*** swap for arrays of same dim ***/

namespace
dango::custom
{
  template
  <typename tp_lhs, typename tp_rhs, dango::usize tp_size>
  struct
  operator_swap<tp_lhs[tp_size], tp_rhs[tp_size]>;
}

template
<typename tp_lhs, typename tp_rhs, dango::usize tp_size>
struct
dango::
custom::
operator_swap<tp_lhs[tp_size], tp_rhs[tp_size]>
final
{
  template
  <dango::is_same_ignore_cv<tp_lhs> tp_arg1, dango::is_same_ignore_cv<tp_rhs> tp_arg2>
  requires(dango::is_swappable<tp_arg1&, tp_arg2&>)
  static constexpr void
  swap(tp_arg1(& a_lhs)[tp_size], tp_arg2(& a_rhs)[tp_size])
  noexcept(dango::is_noexcept_swappable<tp_arg1&, tp_arg2&>)
  {
    for(auto a_i = dango::usize(0); a_i < tp_size; ++a_i)
    {
      dango::swap(a_lhs[a_i], a_rhs[a_i]);
    }
  }

  DANGO_UNCONSTRUCTIBLE(operator_swap)
};

/*** has_equality_ops has_noexcept_equality_ops ***/

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs = tp_lhs>
  concept has_equality_ops =
    dango::is_referenceable_ignore_ref<tp_lhs> && dango::is_referenceable_ignore_ref<tp_rhs> &&
    requires
    {
      { dango::declval<tp_lhs>() == dango::declval<tp_rhs>() }->dango::is_convertible_ret<bool>;
      { dango::declval<tp_lhs>() != dango::declval<tp_rhs>() }->dango::is_convertible_ret<bool>;
    };

  template
  <typename tp_lhs, typename tp_rhs = tp_lhs>
  concept has_noexcept_equality_ops =
    dango::has_equality_ops<tp_lhs, tp_rhs> &&
    requires
    {
      { dango::declval<tp_lhs>() == dango::declval<tp_rhs>() }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { dango::declval<tp_lhs>() != dango::declval<tp_rhs>() }noexcept->dango::is_noexcept_convertible_ret<bool>;
    };
}

/*** equals  ***/

namespace
dango::custom
{
  template
  <typename type>
  struct operator_is_null;

  template
  <typename tp_lhs, typename tp_rhs>
  struct operator_equals;

  template
  <typename tp_lhs, typename tp_rhs>
  struct operator_compare;
}

namespace
dango
{
  template
  <typename tp_type>
  concept has_operator_is_null_struct =
    dango::is_referenceable_ignore_ref<tp_type> &&
    requires
    { { dango::custom::operator_is_null<dango::remove_cvref<tp_type>>::is_null(dango::declval<tp_type>()) }->dango::is_convertible_ret<bool>; };

  template
  <typename tp_type>
  concept has_noexcept_operator_is_null_struct =
    dango::has_operator_is_null_struct<tp_type> &&
    requires
    { { dango::custom::operator_is_null<dango::remove_cvref<tp_type>>::is_null(dango::declval<tp_type>()) }noexcept->dango::is_noexcept_convertible_ret<bool>; };

  template
  <typename tp_type>
  concept has_operator_is_null_method =
    dango::is_class_or_union_ignore_ref<tp_type> &&
    requires{ { dango::declval<tp_type>().dango_operator_is_null() }->dango::is_convertible_ret<bool>; };

  template
  <typename tp_type>
  concept has_noexcept_operator_is_null_method =
    dango::has_operator_is_null_method<tp_type> &&
    requires{ { dango::declval<tp_type>().dango_operator_is_null() }noexcept->dango::is_noexcept_convertible_ret<bool>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_operator_equals_struct =
    dango::is_referenceable_ignore_ref<tp_lhs> && dango::is_referenceable_ignore_ref<tp_rhs> &&
    requires
    {
      {
        dango::custom::operator_equals<dango::remove_cvref<tp_lhs>, dango::remove_cvref<tp_rhs>>::equals(dango::declval<tp_lhs>(), dango::declval<tp_rhs>())
      }->dango::is_convertible_ret<bool>;
    };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_operator_equals_struct =
    dango::has_operator_equals_struct<tp_lhs, tp_rhs> &&
    requires
    {
      {
        dango::custom::operator_equals<dango::remove_cvref<tp_lhs>, dango::remove_cvref<tp_rhs>>::equals(dango::declval<tp_lhs>(), dango::declval<tp_rhs>())
      }noexcept->dango::is_noexcept_convertible_ret<bool>;
    };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_operator_equals_method =
    dango::is_class_or_union_ignore_ref<tp_lhs> && dango::is_referenceable_ignore_ref<tp_rhs> &&
    requires
    { { dango::declval<tp_lhs>().dango_operator_equals(dango::declval<tp_rhs>()) }->dango::is_convertible_ret<bool>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_operator_equals_method =
    dango::has_operator_equals_method<tp_lhs, tp_rhs> &&
    requires
    { { dango::declval<tp_lhs>().dango_operator_equals(dango::declval<tp_rhs>()) }noexcept->dango::is_noexcept_convertible_ret<bool>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_operator_compare_struct =
    dango::is_referenceable_ignore_ref<tp_lhs> && dango::is_referenceable_ignore_ref<tp_rhs> &&
    requires
    {
      {
        dango::custom::operator_compare<dango::remove_cvref<tp_lhs>, dango::remove_cvref<tp_rhs>>::compare(dango::declval<tp_lhs>(), dango::declval<tp_rhs>())
      }->dango::comparison::is_convertible;
    };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_operator_compare_struct =
    dango::has_operator_compare_struct<tp_lhs, tp_rhs> &&
    requires
    { { dango::custom::operator_compare<dango::remove_cvref<tp_lhs>, dango::remove_cvref<tp_rhs>>::compare(dango::declval<tp_lhs>(), dango::declval<tp_rhs>()) }noexcept; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_operator_compare_method =
    dango::is_class_or_union_ignore_ref<tp_lhs> && dango::is_referenceable_ignore_ref<tp_rhs> &&
    requires
    { { dango::declval<tp_lhs>().dango_operator_compare(dango::declval<tp_rhs>()) }->dango::comparison::is_convertible; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_operator_compare_method =
    dango::has_operator_compare_method<tp_lhs, tp_rhs> &&
    requires
    { { dango::declval<tp_lhs>().dango_operator_compare(dango::declval<tp_rhs>()) }noexcept; };
}

namespace
dango::detail
{
  using equals_help_prio = dango::priority_tag<dango::uint(11)>;

  template
  <typename tp_lhs>
  requires(dango::has_operator_is_null_struct<tp_lhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(11)> const, tp_lhs const& a_lhs, dango::null_tag const)
  noexcept(dango::has_noexcept_operator_is_null_struct<tp_lhs const&>)->bool
  {
    return dango::custom::operator_is_null<dango::remove_cv<tp_lhs>>::is_null(a_lhs);
  }

  template
  <typename tp_rhs>
  requires(dango::has_operator_is_null_struct<tp_rhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(10)> const, dango::null_tag const, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_is_null_struct<tp_rhs const&>)->bool
  {
    return dango::custom::operator_is_null<dango::remove_cv<tp_rhs>>::is_null(a_rhs);
  }

  template
  <typename tp_lhs>
  requires(dango::has_operator_is_null_method<tp_lhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(9)> const, tp_lhs const& a_lhs, dango::null_tag const)
  noexcept(dango::has_noexcept_operator_is_null_method<tp_lhs const&>)->bool
  {
    return a_lhs.dango_operator_is_null();
  }

  template
  <typename tp_rhs>
  requires(dango::has_operator_is_null_method<tp_rhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(8)> const, dango::null_tag const, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_is_null_method<tp_rhs const&>)->bool
  {
    return a_rhs.dango_operator_is_null();
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_equals_struct<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(7)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_equals_struct<tp_lhs const&, tp_rhs const&>)->bool
  {
    return dango::custom::operator_equals<dango::remove_cv<tp_lhs>, dango::remove_cv<tp_rhs>>::equals(a_lhs, a_rhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_equals_struct<tp_rhs const&, tp_lhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(6)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_equals_struct<tp_rhs const&, tp_lhs const&>)->bool
  {
    return dango::custom::operator_equals<dango::remove_cv<tp_rhs>, dango::remove_cv<tp_lhs>>::equals(a_rhs, a_lhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_equals_method<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(5)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_equals_method<tp_lhs const&, tp_rhs const&>)->bool
  {
    return a_lhs.dango_operator_equals(a_rhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_equals_method<tp_rhs const&, tp_lhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(4)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_equals_method<tp_rhs const&, tp_lhs const&>)->bool
  {
    return a_rhs.dango_operator_equals(a_lhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_compare_struct<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(3)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_compare_struct<tp_lhs const&, tp_rhs const&>)->bool
  {
    return dango::comparison::strongest(dango::custom::operator_compare<dango::remove_cv<tp_lhs>, dango::remove_cv<tp_rhs>>::compare(a_lhs, a_rhs)).is_eq();
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_compare_struct<tp_rhs const&, tp_lhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(2)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_compare_struct<tp_rhs const&, tp_lhs const&>)->bool
  {
    return dango::comparison::strongest(dango::custom::operator_compare<dango::remove_cv<tp_rhs>, dango::remove_cv<tp_lhs>>::compare(a_rhs, a_lhs)).is_eq();
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_compare_method<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(1)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_compare_method<tp_lhs const&, tp_rhs const&>)->bool
  {
    return dango::comparison::strongest(a_lhs.dango_operator_compare(a_rhs)).is_eq();
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_compare_method<tp_rhs const&, tp_lhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(0)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_compare_method<tp_rhs const&, tp_lhs const&>)->bool
  {
    return dango::comparison::strongest(a_rhs.dango_operator_compare(a_lhs)).is_eq();
  }

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_equals_help =
    dango::is_referenceable_ignore_ref<tp_lhs> && dango::is_referenceable_ignore_ref<tp_rhs> &&
    requires
    { { dango::detail::equals_help(dango::detail::equals_help_prio{ }, dango::declval<tp_lhs>(), dango::declval<tp_rhs>()) }->dango::is_convertible_ret<bool>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_equals_help =
    dango::detail::has_equals_help<tp_lhs, tp_rhs> &&
    requires
    { { dango::detail::equals_help(dango::detail::equals_help_prio{ }, dango::declval<tp_lhs>(), dango::declval<tp_rhs>()) }noexcept->dango::is_noexcept_convertible_ret<bool>; };
}

namespace
dango::operators
{
  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::detail::has_equals_help<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  operator ==
  (tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::detail::has_noexcept_equals_help<tp_lhs const&, tp_rhs const&>)->bool
  {
    return dango::detail::equals_help(dango::detail::equals_help_prio{ }, a_lhs, a_rhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::detail::has_equals_help<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  operator !=
  (tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::detail::has_noexcept_equals_help<tp_lhs const&, tp_rhs const&>)->bool
  {
    return !dango::detail::equals_help(dango::detail::equals_help_prio{ }, a_lhs, a_rhs);
  }
}

namespace
dango
{
  DANGO_USING_EQUALITY_OPERATORS
}

/*** is_equatable is_noexcept_equatable equals ***/

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs = tp_lhs>
  concept is_equatable =
    dango::detail::has_equals_help<tp_lhs, tp_rhs> || dango::has_equality_ops<tp_lhs, tp_rhs>;

  template
  <typename tp_lhs, typename tp_rhs = tp_lhs>
  concept is_noexcept_equatable =
    dango::is_equatable<tp_lhs, tp_rhs> &&
    (dango::detail::has_noexcept_equals_help<tp_lhs, tp_rhs> ||
    (!dango::detail::has_equals_help<tp_lhs, tp_rhs> && dango::has_noexcept_equality_ops<tp_lhs, tp_rhs>));

  inline constexpr auto const equals =
    []<typename tp_lhs, typename tp_rhs>
    (tp_lhs const& a_lhs, tp_rhs const& a_rhs)constexpr
    noexcept(dango::is_noexcept_equatable<tp_lhs const&, tp_rhs const&>)->bool
    requires(dango::is_equatable<tp_lhs const&, tp_rhs const&>)
    {
      if constexpr(dango::detail::has_equals_help<tp_lhs const&, tp_rhs const&>)
      {
        return dango::detail::equals_help(dango::detail::equals_help_prio{ }, a_lhs, a_rhs);
      }
      else
      {
        return a_lhs == a_rhs;
      }
    };
}

/*** equals for arrays ***/

namespace
dango::custom
{
  template
  <typename tp_lhs, dango::usize tp_lsz, typename tp_rhs, dango::usize tp_rsz>
  struct
  operator_equals<tp_lhs[tp_lsz], tp_rhs[tp_rsz]>;
}

template
<typename tp_lhs, dango::usize tp_lsz, typename tp_rhs, dango::usize tp_rsz>
struct
dango::
custom::
operator_equals<tp_lhs[tp_lsz], tp_rhs[tp_rsz]>
final
{
  template
  <dango::is_same_ignore_cv<tp_lhs> tp_arg1, dango::is_same_ignore_cv<tp_rhs> tp_arg2>
  requires(dango::is_equatable<tp_arg1 const&, tp_arg2 const&>)
  static constexpr auto
  equals
  (tp_arg1 const(& a_lhs)[tp_lsz], tp_arg2 const(& a_rhs)[tp_rsz])
  noexcept((tp_lsz != tp_rsz) || dango::is_noexcept_equatable<tp_arg1 const&, tp_arg2 const&>)->bool
  {
    if constexpr(tp_lsz != tp_rsz)
    {
      return false;
    }
    else
    {
      for(auto a_i = dango::usize(0); a_i < tp_lsz; ++a_i)
      {
        if(!dango::equals(a_lhs[a_i], a_rhs[a_i]))
        {
          return false;
        }
      }

      return true;
    }
  }

  DANGO_UNCONSTRUCTIBLE(operator_equals)
};

/*** is_null_equatable is_noexcept_null_equatable is_null ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_null_equatable =
    dango::is_equatable<tp_type, dango::null_tag const&> &&
    dango::is_equatable<tp_type, dango::null_tag&&> &&
    dango::is_equatable<dango::null_tag const&, tp_type> &&
    dango::is_equatable<dango::null_tag&&, tp_type>;

  template
  <typename tp_type>
  concept is_noexcept_null_equatable =
    dango::is_null_equatable<tp_type> &&
    dango::is_noexcept_equatable<tp_type, dango::null_tag const&> &&
    dango::is_noexcept_equatable<tp_type, dango::null_tag&&> &&
    dango::is_noexcept_equatable<dango::null_tag const&, tp_type> &&
    dango::is_noexcept_equatable<dango::null_tag&&, tp_type>;


  inline constexpr auto const is_null =
    []<typename tp_arg>
    (tp_arg const& a_arg)constexpr
    noexcept(dango::is_noexcept_null_equatable<tp_arg const&>)->bool
    requires(dango::is_null_equatable<tp_arg const&>)
    {
      return dango::equals(a_arg, dango::null);
    };
}

/*** has_comparison_ops has_noexcept_comparison_ops has_spaceship_op ***/

namespace
dango::detail
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept has_comparison_ops_help =
    dango::is_user_defined_ignore_ref<tp_lhs> ||
    dango::is_user_defined_ignore_ref<tp_rhs> ||
    requires{ { dango::declval<tp_lhs>() <=> dango::declval<tp_rhs>() }; };
}

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs = tp_lhs>
  concept has_comparison_ops =
    dango::is_object_exclude_array_ignore_ref<tp_lhs> &&
    dango::is_object_exclude_array_ignore_ref<tp_rhs> &&
    dango::detail::has_comparison_ops_help<tp_lhs, tp_rhs> &&
    dango::has_equality_ops<tp_lhs, tp_rhs> &&
    requires
    {
      { dango::declval<tp_lhs>() <  dango::declval<tp_rhs>() }->dango::is_convertible_ret<bool>;
      { dango::declval<tp_lhs>() <= dango::declval<tp_rhs>() }->dango::is_convertible_ret<bool>;
      { dango::declval<tp_lhs>() >  dango::declval<tp_rhs>() }->dango::is_convertible_ret<bool>;
      { dango::declval<tp_lhs>() >= dango::declval<tp_rhs>() }->dango::is_convertible_ret<bool>;
    };

  template
  <typename tp_lhs, typename tp_rhs = tp_lhs>
  concept has_noexcept_comparison_ops =
    dango::has_comparison_ops<tp_lhs, tp_rhs> &&
    dango::has_noexcept_equality_ops<tp_lhs, tp_rhs> &&
    requires
    {
      { dango::declval<tp_lhs>() <  dango::declval<tp_rhs>() }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { dango::declval<tp_lhs>() <= dango::declval<tp_rhs>() }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { dango::declval<tp_lhs>() >  dango::declval<tp_rhs>() }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { dango::declval<tp_lhs>() >= dango::declval<tp_rhs>() }noexcept->dango::is_noexcept_convertible_ret<bool>;
    };
}

namespace
dango::detail
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept has_spaceship_op_help1 =
    dango::has_comparison_ops<tp_lhs, tp_rhs> &&
    requires{ { dango::declval<tp_lhs>() <=> dango::declval<tp_rhs>() }->dango::comparison::is_convertible; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_spaceship_op_help2 =
    !dango::has_noexcept_comparison_ops<tp_lhs, tp_rhs> ||
    requires{ { dango::declval<tp_lhs>() <=> dango::declval<tp_rhs>() }noexcept; };
}

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs = tp_lhs>
  concept has_spaceship_op =
    dango::detail::has_spaceship_op_help1<tp_lhs, tp_rhs> &&
    dango::detail::has_spaceship_op_help2<tp_lhs, tp_rhs>;
}

/*** compare ***/

namespace
dango::detail
{
  using compare_help_prio = dango::priority_tag<dango::uint(3)>;

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_compare_struct<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  compare_help
  (dango::priority_tag<dango::uint(3)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_compare_struct<tp_lhs const&, tp_rhs const&>)->auto
  {
    return dango::comparison::strongest(dango::custom::operator_compare<dango::remove_cv<tp_lhs>, dango::remove_cv<tp_rhs>>::compare(a_lhs, a_rhs));
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_compare_struct<tp_rhs const&, tp_lhs const&>)
  constexpr auto
  compare_help
  (dango::priority_tag<dango::uint(2)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_compare_struct<tp_rhs const&, tp_lhs const&>)->auto
  {
    return dango::comparison::strongest(dango::custom::operator_compare<dango::remove_cv<tp_rhs>, dango::remove_cv<tp_lhs>>::compare(a_rhs, a_lhs)).mirror();
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_compare_method<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  compare_help
  (dango::priority_tag<dango::uint(1)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_compare_method<tp_lhs const&, tp_rhs const&>)->auto
  {
    return dango::comparison::strongest(a_lhs.dango_operator_compare(a_rhs));
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_compare_method<tp_rhs const&, tp_lhs const&>)
  constexpr auto
  compare_help
  (dango::priority_tag<dango::uint(0)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_compare_method<tp_rhs const&, tp_lhs const&>)->auto
  {
    return dango::comparison::strongest(a_rhs.dango_operator_compare(a_lhs)).mirror();
  }

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_compare_help =
    dango::is_referenceable_ignore_ref<tp_lhs> && dango::is_referenceable_ignore_ref<tp_rhs> &&
    requires
    { { dango::detail::compare_help(dango::detail::compare_help_prio{ }, dango::declval<tp_lhs>(), dango::declval<tp_rhs>()) }->dango::is_compare_val; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_compare_help =
    dango::detail::has_compare_help<tp_lhs, tp_rhs> &&
    requires
    { { dango::detail::compare_help(dango::detail::compare_help_prio{ }, dango::declval<tp_lhs>(), dango::declval<tp_rhs>()) }noexcept; };
}

namespace
dango::operators
{
  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::detail::has_compare_help<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  operator <
  (tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::detail::has_noexcept_compare_help<tp_lhs const&, tp_rhs const&>)->bool
  {
    return dango::detail::compare_help(dango::detail::compare_help_prio{ }, a_lhs, a_rhs).is_lt();
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::detail::has_compare_help<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  operator <=
  (tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::detail::has_noexcept_compare_help<tp_lhs const&, tp_rhs const&>)->bool
  {
    return dango::detail::compare_help(dango::detail::compare_help_prio{ }, a_lhs, a_rhs).is_lteq();
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::detail::has_compare_help<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  operator >
  (tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::detail::has_noexcept_compare_help<tp_lhs const&, tp_rhs const&>)->bool
  {
    return dango::detail::compare_help(dango::detail::compare_help_prio{ }, a_lhs, a_rhs).is_gt();
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::detail::has_compare_help<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  operator >=
  (tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::detail::has_noexcept_compare_help<tp_lhs const&, tp_rhs const&>)->bool
  {
    return dango::detail::compare_help(dango::detail::compare_help_prio{ }, a_lhs, a_rhs).is_gteq();
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::detail::has_compare_help<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  operator <=>
  (tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::detail::has_noexcept_compare_help<tp_lhs const&, tp_rhs const&>)->auto
  {
    return dango::detail::compare_help(dango::detail::compare_help_prio{ }, a_lhs, a_rhs);
  }
}

namespace
dango
{
  DANGO_USING_COMPARISON_OPERATORS
}

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs = tp_lhs>
  concept is_comparable =
    dango::detail::has_compare_help<tp_lhs, tp_rhs> || dango::has_comparison_ops<tp_lhs, tp_rhs>;

  template
  <typename tp_lhs, typename tp_rhs = tp_lhs>
  concept is_noexcept_comparable =
    dango::is_comparable<tp_lhs, tp_rhs> &&
    (dango::detail::has_noexcept_compare_help<tp_lhs, tp_rhs> ||
    (!dango::detail::has_compare_help<tp_lhs, tp_rhs> && dango::has_noexcept_comparison_ops<tp_lhs, tp_rhs>));

// GCC <=> is currently bugged in constexpr (GCC 10.2.1). the workaround is to avoid actually evaluating <=> expressions when in constexpr.

  inline constexpr auto compare =
    []<typename tp_lhs, typename tp_rhs>
    (tp_lhs const& a_lhs, tp_rhs const& a_rhs)constexpr
    noexcept(dango::is_noexcept_comparable<tp_lhs const&, tp_rhs const&>)->auto
    requires(dango::is_comparable<tp_lhs const&, tp_rhs const&>)
    {
      if constexpr(dango::detail::has_compare_help<tp_lhs const&, tp_rhs const&>)
      {
        return dango::detail::compare_help(dango::detail::compare_help_prio{ }, a_lhs, a_rhs);
      }
      else
      {
        if constexpr(dango::has_spaceship_op<tp_lhs const&, tp_rhs const&>)
        {
#ifdef DANGO_USING_GCC
          if(dango::in_constexpr_context())
          {
            using ret_type = decltype(dango::comparison::strongest(a_lhs <=> a_rhs));

            if(a_lhs == a_rhs)
            {
              return ret_type::equivalent;
            }

            if(a_lhs < a_rhs)
            {
              return ret_type::less;
            }

            if constexpr(dango::is_same<ret_type, dango::compare_val_p>)
            {
              if(!(a_lhs > a_rhs))
              {
                return ret_type::unordered;
              }
            }

            return ret_type::greater;
          }
          else
          {
            return dango::comparison::strongest(a_lhs <=> a_rhs);
          }
#else // !DANGO_USING_GCC
          return dango::comparison::strongest(a_lhs <=> a_rhs);
#endif // DANGO_USING_GCC
        }
        else // no <=> operator assume type is strongly ordered
        {
          using ret_type = dango::compare_val_s;

          if(a_lhs == a_rhs)
          {
            return ret_type::equal;
          }

          if(a_lhs < a_rhs)
          {
            return ret_type::less;
          }

          return ret_type::greater;
        }
      }
    };
}

/*** comparison for pointers ***/

namespace
dango::custom
{
  template
  <typename tp_lhs, typename tp_rhs>
  struct
  operator_compare<tp_lhs*, tp_rhs*>;
}

template
<typename tp_lhs, typename tp_rhs>
struct
dango::
custom::
operator_compare<tp_lhs*, tp_rhs*>
final
{
  using voidp = void const volatile*;

  static constexpr auto
  compare
  (voidp const a_lhs, voidp const a_rhs)noexcept->auto
  {
    if(dango::in_constexpr_context())
    {
    #ifdef DANGO_USING_GCC
      return dango::compare_val_s{ dango::sint(a_lhs > a_rhs) - dango::sint(a_lhs < a_rhs) };
    #else
      return dango::comparison::strongest(a_lhs <=> a_rhs);
    #endif
    }
    else
    {
      return dango::comparison::strongest(dango::ptr_as_uint(a_lhs) <=> dango::ptr_as_uint(a_rhs));
    }
  }

  DANGO_UNCONSTRUCTIBLE(operator_compare)
};

/*** is_comparator ***/

namespace
dango
{
  template
  <typename tp_cmp, typename tp_lhs, typename tp_rhs>
  concept is_comparator =
    dango::is_callable<tp_cmp, tp_lhs, tp_rhs> &&
    requires
    { { dango::declval<tp_cmp>()(dango::declval<tp_lhs>(), dango::declval<tp_rhs>()) }->dango::comparison::is_convertible; };

  template
  <typename tp_cmp, typename tp_lhs, typename tp_rhs>
  concept is_noexcept_comparator =
    dango::is_comparator<tp_cmp, tp_lhs, tp_rhs> &&
    dango::is_noexcept_callable<tp_cmp, tp_lhs, tp_rhs> &&
    requires
    { { dango::declval<tp_cmp>()(dango::declval<tp_lhs>(), dango::declval<tp_rhs>()) }noexcept; };
}

/*** min max ***/

namespace
dango::detail
{
  // scalar types

  template
  <bool tp_max, typename tp_cmp, dango::is_scalar tp_arg>
  requires(dango::is_comparator<tp_cmp, tp_arg const&, tp_arg const&>)
  constexpr auto
  min_max_help
  (tp_cmp&& a_cmp, tp_arg const a_arg1, tp_arg const a_arg2)
  noexcept(dango::is_noexcept_comparator<tp_cmp, tp_arg const&, tp_arg const&>)->tp_arg
  {
    auto const a_result =
      dango::comparison::strongest(dango::forward<tp_cmp>(a_cmp)(a_arg1, a_arg2));

    if constexpr(tp_max)
    {
      return a_result.is_gt() ? a_arg1 : a_arg2;
    }
    else
    {
      return a_result.is_lt() ? a_arg1 : a_arg2;
    }
  }

  template
  <bool tp_max, typename tp_cmp, dango::is_scalar tp_arg, dango::is_same<tp_arg>... tp_args>
  requires((sizeof...(tp_args) >= dango::usize(2)) && dango::is_comparator<tp_cmp, tp_arg const&, tp_arg const&>)
  constexpr auto
  min_max_help
  (tp_cmp&& a_cmp, tp_arg const a_arg, tp_args const... a_args)
  noexcept(dango::is_noexcept_comparator<tp_cmp, tp_arg const&, tp_arg const&>)->auto
  {
    return dango::detail::min_max_help<tp_max>(dango::forward<tp_cmp>(a_cmp), a_arg, dango::detail::min_max_help<tp_max>(dango::forward<tp_cmp>(a_cmp), a_args...));
  }

  // class types

  template
  <bool tp_max, typename tp_cmp, dango::is_class_or_union_ignore_ref tp_arg1, dango::is_same_ignore_cvref<tp_arg1> tp_arg2>
  requires
  (
    dango::is_comparator<tp_cmp, dango::remove_ref<tp_arg1> const&, dango::remove_ref<tp_arg2> const&> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg1>, tp_arg1> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg1>, tp_arg2>
  )
  constexpr auto
  min_max_help
  (tp_cmp&& a_cmp, tp_arg1&& a_arg1, tp_arg2&& a_arg2)
  noexcept
  (
    dango::is_noexcept_comparator<tp_cmp, dango::remove_ref<tp_arg1> const&, dango::remove_ref<tp_arg2> const&> &&
    dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg1>, tp_arg1> &&
    dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg1>, tp_arg2>
  )->dango::remove_cvref<tp_arg1>
  {
    using ret_type = dango::remove_cvref<tp_arg1>;

    auto const a_result =
      dango::comparison::strongest(dango::forward<tp_cmp>(a_cmp)(dango::as_const(a_arg1), dango::as_const(a_arg2)));

    if constexpr(tp_max)
    {
      return a_result.is_gt() ? ret_type{ dango::forward<tp_arg1>(a_arg1) } : ret_type{ dango::forward<tp_arg2>(a_arg2) };
    }
    else
    {
      return a_result.is_lt() ? ret_type{ dango::forward<tp_arg1>(a_arg1) } : ret_type{ dango::forward<tp_arg2>(a_arg2) };
    }
  }

  template
  <bool tp_max, typename tp_cmp, typename... tp_args>
  struct min_max_deferred;

  template
  <bool tp_max, typename tp_cmp, dango::is_class_or_union_ignore_ref tp_arg, dango::is_same_ignore_cvref<tp_arg>... tp_args>
  requires(sizeof...(tp_args) >= dango::usize(2))
  constexpr auto
  min_max_help
  (tp_cmp&& a_cmp, tp_arg&& a_arg, tp_args&&... a_args)
  noexcept
  (
    requires
    {
      { dango::detail::min_max_deferred<tp_max, tp_cmp&&, tp_args&&...>::m(dango::declval<tp_cmp>(), dango::declval<tp_args>()...) }noexcept;
      { dango::detail::min_max_help<tp_max>(dango::declval<tp_cmp>(), dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }noexcept;
    }
  )->auto
  requires
  (
    requires
    {
      { dango::detail::min_max_deferred<tp_max, tp_cmp&&, tp_args&&...>::m(dango::declval<tp_cmp>(), dango::declval<tp_args>()...) };
      { dango::detail::min_max_help<tp_max>(dango::declval<tp_cmp>(), dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) };
    }
  )
  {
    return
      dango::detail::min_max_help<tp_max>
      (
        dango::forward<tp_cmp>(a_cmp),
        dango::forward<tp_arg>(a_arg),
        dango::detail::min_max_deferred<tp_max, tp_cmp&&, tp_args&&...>::m(dango::forward<tp_cmp>(a_cmp), dango::forward<tp_args>(a_args)...)
      );
  }

  template
  <bool tp_max, typename tp_cmp, typename... tp_args>
  struct
  min_max_deferred
  final
  {
    static constexpr auto
    m(tp_cmp a_cmp, tp_args... a_args)
    noexcept(requires{ { dango::detail::min_max_help<tp_max>(dango::declval<tp_cmp>(), dango::declval<tp_args>()...) }noexcept; })->auto
    requires(requires{ { dango::detail::min_max_help<tp_max>(dango::declval<tp_cmp>(), dango::declval<tp_args>()...) }; })
    {
      return dango::detail::min_max_help<tp_max>(dango::forward<tp_cmp>(a_cmp), dango::forward<tp_args>(a_args)...);
    }

    DANGO_UNCONSTRUCTIBLE(min_max_deferred)
  };

  // dispatchers

  template
  <bool tp_max, typename tp_cmp, typename... tp_args>
  requires(sizeof...(tp_args) >= dango::usize(2))
  constexpr auto
  min_max_dispatch
  (tp_cmp&& a_cmp, tp_args&&... a_args)
  noexcept(requires{ { dango::detail::min_max_help<tp_max>(dango::declval<tp_cmp>(), dango::declval<tp_args>()...) }noexcept; })->auto
  requires(requires{ { dango::detail::min_max_help<tp_max>(dango::declval<tp_cmp>(), dango::declval<tp_args>()...) }; })
  {
    return dango::detail::min_max_help<tp_max>(dango::forward<tp_cmp>(a_cmp), dango::forward<tp_args>(a_args)...);
  }

  template
  <bool tp_max, typename tp_cmp, typename tp_arg>
  requires(dango::is_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg>)
  constexpr auto
  min_max_dispatch
  (tp_cmp&&, tp_arg&& a_arg)
  noexcept(dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg>)->auto
  {
    return dango::remove_cvref<tp_arg>{ dango::forward<tp_arg>(a_arg) };
  }

  template
  <bool tp_max, typename tp_cmp>
  constexpr void
  min_max_dispatch(tp_cmp&&)noexcept{ }
}

namespace
dango
{
  inline constexpr auto min_c =
    []<typename tp_cmp, typename... tp_args>
    (tp_cmp&& a_cmp, tp_args&&... a_args)constexpr
    noexcept(requires{ { dango::detail::min_max_dispatch<false>(dango::declval<tp_cmp>(), dango::declval<tp_args>()...) }noexcept; })->decltype(auto)
    requires(requires{ { dango::detail::min_max_dispatch<false>(dango::declval<tp_cmp>(), dango::declval<tp_args>()...) }; })
    {
      return dango::detail::min_max_dispatch<false>(dango::forward<tp_cmp>(a_cmp), dango::forward<tp_args>(a_args)...);
    };

  inline constexpr auto min =
    []<typename... tp_args>
    (tp_args&&... a_args)constexpr
    noexcept(requires{ { dango::detail::min_max_dispatch<false>(dango::compare, dango::declval<tp_args>()...) }noexcept; })->decltype(auto)
    requires(requires{ { dango::detail::min_max_dispatch<false>(dango::compare, dango::declval<tp_args>()...) }; })
    {
      return dango::detail::min_max_dispatch<false>(dango::compare, dango::forward<tp_args>(a_args)...);
    };

  inline constexpr auto max_c =
    []<typename tp_cmp, typename... tp_args>
    (tp_cmp&& a_cmp, tp_args&&... a_args)constexpr
    noexcept(requires{ { dango::detail::min_max_dispatch<true>(dango::declval<tp_cmp>(), dango::declval<tp_args>()...) }noexcept; })->decltype(auto)
    requires(requires{ { dango::detail::min_max_dispatch<true>(dango::declval<tp_cmp>(), dango::declval<tp_args>()...) }; })
    {
      return dango::detail::min_max_dispatch<true>(dango::forward<tp_cmp>(a_cmp), dango::forward<tp_args>(a_args)...);
    };

  inline constexpr auto max =
    []<typename... tp_args>
    (tp_args&&... a_args)constexpr
    noexcept(requires{ { dango::detail::min_max_dispatch<true>(dango::compare, dango::declval<tp_args>()...) }noexcept; })->decltype(auto)
    requires(requires{ { dango::detail::min_max_dispatch<true>(dango::compare, dango::declval<tp_args>()...) }; })
    {
      return dango::detail::min_max_dispatch<true>(dango::compare, dango::forward<tp_args>(a_args)...);
    };
}

/*** compare for arrays ***/

namespace
dango::custom
{
  template
  <typename tp_lhs, dango::usize tp_lsz, typename tp_rhs, dango::usize tp_rsz>
  struct
  operator_compare<tp_lhs[tp_lsz], tp_rhs[tp_rsz]>;
}

template
<typename tp_lhs, dango::usize tp_lsz, typename tp_rhs, dango::usize tp_rsz>
struct
dango::
custom::
operator_compare<tp_lhs[tp_lsz], tp_rhs[tp_rsz]>
final
{
  using ret_type =
    decltype(dango::compare(dango::declval<tp_lhs const&>(), dango::declval<tp_rhs const&>()));

  template
  <dango::is_same_ignore_cv<tp_lhs> tp_arg1, dango::is_same_ignore_cv<tp_rhs> tp_arg2>
  requires(dango::is_comparable<tp_arg1 const&, tp_arg2 const&>)
  static constexpr auto
  compare
  (tp_arg1 const(& a_lhs)[tp_lsz], tp_arg2 const(& a_rhs)[tp_rsz])
  noexcept(dango::is_noexcept_comparable<tp_arg1 const&, tp_arg2 const&>)->ret_type
  {
    constexpr auto const c_size = dango::min(tp_lsz, tp_rsz);

    for(auto a_i = dango::usize(0); a_i < c_size; ++a_i)
    {
      auto const a_result = dango::compare(a_lhs[a_i], a_rhs[a_i]);

      if(a_result.is_neq())
      {
        return a_result;
      }
    }

    return dango::compare(tp_lsz, tp_rsz);
  }

  DANGO_UNCONSTRUCTIBLE(operator_compare)
};

/*** operator_tuple ***/

namespace
dango::custom
{
  template
  <typename tp_type>
  struct operator_tuple;
}

namespace
dango::detail
{
  template
  <typename tp_type, dango::usize tp_index>
  using operator_tuple_struct_elem_type =
    typename dango::custom::operator_tuple<dango::remove_cvref<tp_type>>::template elem_type<tp_index, dango::remove_ref<tp_type>>;

  template
  <typename tp_type, dango::usize tp_index>
  using operator_tuple_elem_type_member =
    typename dango::remove_cvref<tp_type>::template dango_operator_tuple_elem<tp_index, dango::remove_ref<tp_type>>;
}

namespace
dango
{
  template
  <typename tp_type>
  concept has_operator_tuple_struct_size =
    dango::is_object_ignore_ref<tp_type> &&
    requires{ { dango::constexpr_check<dango::usize, dango::custom::operator_tuple<dango::remove_cvref<tp_type>>::size>{ } }noexcept; };

  template
  <typename tp_type>
  concept has_operator_tuple_size_member =
    dango::is_class_or_union_ignore_ref<tp_type> &&
    requires{ { dango::constexpr_check<dango::usize, dango::remove_cvref<tp_type>::dango_operator_tuple_size>{ } }noexcept; };

  template
  <typename tp_type, dango::usize tp_index>
  concept has_operator_tuple_struct_elem_type =
    dango::is_object_ignore_ref<tp_type> &&
    requires{ typename dango::detail::operator_tuple_struct_elem_type<tp_type, tp_index>; };

  template
  <typename tp_type, dango::usize tp_index>
  concept has_operator_tuple_elem_type_member =
    dango::is_class_or_union_ignore_ref<tp_type> &&
    requires{ typename dango::detail::operator_tuple_elem_type_member<tp_type, tp_index>; };

  template
  <typename tp_type, dango::usize tp_index>
  concept has_operator_tuple_struct_get =
    dango::is_object_ignore_ref<tp_type> &&
    requires
    { { dango::custom::operator_tuple<dango::remove_cvref<tp_type>>::template get<tp_index>(dango::declval<tp_type>()) }; };

  template
  <typename tp_type, dango::usize tp_index>
  concept has_noexcept_operator_tuple_struct_get =
    dango::has_operator_tuple_struct_get<tp_type, tp_index> &&
    requires
    { { dango::custom::operator_tuple<dango::remove_cvref<tp_type>>::template get<tp_index>(dango::declval<tp_type>()) }noexcept; };

  template
  <typename tp_type, dango::usize tp_index>
  concept has_operator_tuple_get_method =
    dango::is_class_or_union_ignore_ref<tp_type> &&
    requires
    { { dango::declval<tp_type>().template dango_operator_tuple_get<tp_index>() }; };

  template
  <typename tp_type, dango::usize tp_index>
  concept has_noexcept_operator_tuple_get_method =
    dango::has_operator_tuple_get_method<tp_type, tp_index> &&
    requires
    { { dango::declval<tp_type>().template dango_operator_tuple_get<tp_index>() }noexcept; };
}

namespace
dango::detail
{
  using tuple_help_prio = dango::priority_tag<dango::uint(1)>;

  template
  <typename tp_type>
  requires(dango::has_operator_tuple_struct_size<tp_type>)
  constexpr auto
  tuple_size_help
  (dango::priority_tag<dango::uint(1)> const)noexcept->dango::usize
  {
    return dango::custom::operator_tuple<dango::remove_cvref<tp_type>>::size;
  }

  template
  <typename tp_type>
  requires(dango::has_operator_tuple_size_member<tp_type>)
  constexpr auto
  tuple_size_help
  (dango::priority_tag<dango::uint(0)> const)noexcept->dango::usize
  {
    return dango::remove_cvref<tp_type>::dango_operator_tuple_size;
  }

  template
  <typename tp_type, dango::usize tp_index>
  requires(dango::has_operator_tuple_struct_elem_type<tp_type, tp_index>)
  constexpr auto
  tuple_elem_type_help
  (dango::priority_tag<dango::uint(1)> const)
  noexcept->dango::type_tag<dango::detail::operator_tuple_struct_elem_type<tp_type, tp_index>>;

  template
  <typename tp_type, dango::usize tp_index>
  requires(dango::has_operator_tuple_elem_type_member<tp_type, tp_index>)
  constexpr auto
  tuple_elem_type_help
  (dango::priority_tag<dango::uint(0)> const)
  noexcept->dango::type_tag<dango::detail::operator_tuple_elem_type_member<tp_type, tp_index>>;

  template
  <dango::usize tp_index, typename tp_type>
  requires(dango::has_operator_tuple_struct_get<tp_type, tp_index>)
  constexpr auto
  tuple_get_help
  (dango::priority_tag<dango::uint(1)> const, tp_type&& a_arg)
  noexcept(dango::has_noexcept_operator_tuple_struct_get<tp_type, tp_index>)->decltype(auto)
  {
    return dango::custom::operator_tuple<dango::remove_cvref<tp_type>>::template get<tp_index>(dango::forward<tp_type>(a_arg));
  }

  template
  <dango::usize tp_index, typename tp_type>
  requires(dango::has_operator_tuple_get_method<tp_type, tp_index>)
  constexpr auto
  tuple_get_help
  (dango::priority_tag<dango::uint(0)> const, tp_type&& a_arg)
  noexcept(dango::has_noexcept_operator_tuple_get_method<tp_type, tp_index>)->decltype(auto)
  {
    return dango::forward<tp_type>(a_arg).template dango_operator_tuple_get<tp_index>();
  }
}

/*** has_tuple_size/elem_type/get tuple_size/elem_type/get etc ***/

namespace
dango
{
  template
  <typename tp_type>
  concept has_tuple_size =
    dango::is_object_ignore_ref<tp_type> &&
    requires{ { dango::detail::tuple_size_help<tp_type>(dango::detail::tuple_help_prio{ }) }noexcept; };

  template
  <typename tp_type, dango::usize tp_index>
  concept has_tuple_elem_type =
    dango::is_object_ignore_ref<tp_type> &&
    requires{ { dango::detail::tuple_elem_type_help<tp_type, tp_index>(dango::detail::tuple_help_prio{ }) }noexcept; };

  template
  <typename tp_type, dango::usize tp_index>
  concept has_tuple_get =
    dango::is_object_ignore_ref<tp_type> &&
    requires{ { dango::detail::tuple_get_help<tp_index>(dango::detail::tuple_help_prio{ }, dango::declval<tp_type>()) }; };

  template
  <typename tp_type, dango::usize tp_index>
  concept has_noexcept_tuple_get =
    dango::has_tuple_get<tp_type, tp_index> &&
    requires{ { dango::detail::tuple_get_help<tp_index>(dango::detail::tuple_help_prio{ }, dango::declval<tp_type>()) }noexcept; };

  template
  <dango::has_tuple_size tp_type>
  inline constexpr dango::usize const tuple_size =
    dango::detail::tuple_size_help<tp_type>(dango::detail::tuple_help_prio{ });

  template
  <dango::usize tp_index, dango::has_tuple_elem_type<tp_index> tp_type>
  using tuple_elem_type =
    typename decltype(dango::detail::tuple_elem_type_help<tp_type, tp_index>(dango::detail::tuple_help_prio{ }))::type;

  template
  <dango::usize tp_index>
  inline constexpr auto const tuple_get =
    []<typename tp_type>
    (tp_type&& a_arg)constexpr
    noexcept(dango::has_noexcept_tuple_get<tp_type, tp_index>)->decltype(auto)
    requires(dango::has_tuple_get<tp_type, tp_index>)
    {
      return dango::detail::tuple_get_help<tp_index>(dango::detail::tuple_help_prio{ }, dango::forward<tp_type>(a_arg));
    };
}

/*** is_tuple_like ***/

namespace
dango::detail
{
  template
  <typename tp_type, dango::usize... tp_indices>
  constexpr auto
  is_tuple_like_help
  (dango::index_seq<tp_indices...> const)noexcept->bool
  {
    return ( ... && (dango::has_tuple_elem_type<tp_type, tp_indices> && dango::has_tuple_get<tp_type, tp_indices>));
  }
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_tuple_like =
    dango::has_tuple_size<tp_type> &&
    dango::detail::is_tuple_like_help<tp_type>(dango::make_index_seq<dango::tuple_size<tp_type>>{ });
}

/*** structured bindings for tuple-like types ***/

namespace
std
{
  template
  <dango::is_tuple_like tp_type>
  struct
  tuple_size<tp_type>
  {
    static inline constexpr auto const value = dango::tuple_size<tp_type>;

    DANGO_UNCONSTRUCTIBLE(tuple_size)
  };

  template
  <dango::usize tp_index, dango::is_tuple_like tp_type>
  requires(tp_index < dango::tuple_size<tp_type>)
  struct
  tuple_element<tp_index, tp_type>
  {
    using type = dango::tuple_elem_type<tp_index, tp_type>;

    DANGO_UNCONSTRUCTIBLE(tuple_element)
  };
}

namespace
dango::operators
{
  template
  <dango::usize tp_index, dango::is_tuple_like tp_type>
  requires(tp_index < dango::tuple_size<tp_type>)
  constexpr auto
  get
  (tp_type&& a_arg)
  noexcept(dango::has_noexcept_tuple_get<tp_type, tp_index>)->decltype(auto)
  {
    return dango::tuple_get<tp_index>(dango::forward<tp_type>(a_arg));
  }
}

namespace
dango
{
  DANGO_USING_STRUCTURED_BINDING_OPERATORS
}

/*** operator_tuple for arrays ***/

namespace
dango::custom
{
  template
  <typename tp_elem, dango::usize tp_size>
  struct operator_tuple<tp_elem[tp_size]>;
}

template
<typename tp_elem, dango::usize tp_size>
struct
dango::
custom::
operator_tuple<tp_elem[tp_size]>
final
{
  using array_type = tp_elem[tp_size];

  static inline constexpr auto const size = tp_size;

  template
  <dango::usize tp_index, dango::is_same_ignore_cv<array_type> tp_array>
  requires(tp_index < size)
  using elem_type = dango::remove_array<tp_array>;

  template
  <dango::usize tp_index, dango::is_same_ignore_cv<tp_elem> tp_arg>
  requires(tp_index < size)
  static constexpr auto
  get
  (tp_arg* const a_array)noexcept->tp_arg&
  {
    return a_array[tp_index];
  }

  DANGO_UNCONSTRUCTIBLE(operator_tuple)
};

/*** iter_begin iter_end ***/

namespace
dango::custom
{
  template
  <typename tp_type>
  struct operator_iter;
}

namespace
dango
{
  template
  <typename tp_type>
  concept has_operator_iter_struct_begin =
    dango::is_object_ignore_ref<tp_type> &&
    requires{ { dango::custom::operator_iter<dango::remove_cvref<tp_type>>::begin(dango::declval<tp_type>()) }; };

  template
  <typename tp_type>
  concept has_noexcept_operator_iter_struct_begin =
    dango::has_operator_iter_struct_begin<tp_type> &&
    requires{ { dango::custom::operator_iter<dango::remove_cvref<tp_type>>::begin(dango::declval<tp_type>()) }noexcept; };

  template
  <typename tp_type>
  concept has_operator_iter_method_begin =
    dango::is_class_or_union_ignore_ref<tp_type> &&
    requires{ { dango::declval<tp_type>().dango_operator_begin() }; };

  template
  <typename tp_type>
  concept has_noexcept_operator_iter_method_begin =
    dango::has_operator_iter_method_begin<tp_type> &&
    requires{ { dango::declval<tp_type>().dango_operator_begin() }noexcept; };
}

namespace
dango
{
  template
  <typename tp_type>
  concept has_operator_iter_struct_end =
    dango::is_object_ignore_ref<tp_type> &&
    requires{ { dango::custom::operator_iter<dango::remove_cvref<tp_type>>::end(dango::declval<tp_type>()) }; };

  template
  <typename tp_type>
  concept has_noexcept_operator_iter_struct_end =
    dango::has_operator_iter_struct_end<tp_type> &&
    requires{ { dango::custom::operator_iter<dango::remove_cvref<tp_type>>::end(dango::declval<tp_type>()) }noexcept; };

  template
  <typename tp_type>
  concept has_operator_iter_method_end =
    dango::is_class_or_union_ignore_ref<tp_type> &&
    requires{ { dango::declval<tp_type>().dango_operator_end() }; };

  template
  <typename tp_type>
  concept has_noexcept_operator_iter_method_end =
    dango::has_operator_iter_method_end<tp_type> &&
    requires{ { dango::declval<tp_type>().dango_operator_end() }noexcept; };
}

namespace
dango::detail
{
  using iter_help_prio = dango::priority_tag<dango::uint(1)>;

  template
  <typename tp_type>
  requires(dango::has_operator_iter_struct_begin<tp_type>)
  constexpr auto
  iter_begin_help
  (dango::priority_tag<dango::uint(1)> const, tp_type&& a_arg)
  noexcept(dango::has_noexcept_operator_iter_struct_begin<tp_type>)->decltype(auto)
  {
    return dango::custom::operator_iter<dango::remove_cvref<tp_type>>::begin(dango::forward<tp_type>(a_arg));
  }

  template
  <typename tp_type>
  requires(dango::has_operator_iter_struct_end<tp_type>)
  constexpr auto
  iter_end_help
  (dango::priority_tag<dango::uint(1)> const, tp_type&& a_arg)
  noexcept(dango::has_noexcept_operator_iter_struct_end<tp_type>)->decltype(auto)
  {
    return dango::custom::operator_iter<dango::remove_cvref<tp_type>>::end(dango::forward<tp_type>(a_arg));
  }

  template
  <typename tp_type>
  requires(dango::has_operator_iter_method_begin<tp_type>)
  constexpr auto
  iter_begin_help
  (dango::priority_tag<dango::uint(0)> const, tp_type&& a_arg)
  noexcept(dango::has_noexcept_operator_iter_method_begin<tp_type>)->decltype(auto)
  {
    return dango::forward<tp_type>(a_arg).dango_operator_begin();
  }

  template
  <typename tp_type>
  requires(dango::has_operator_iter_method_end<tp_type>)
  constexpr auto
  iter_end_help
  (dango::priority_tag<dango::uint(0)> const, tp_type&& a_arg)
  noexcept(dango::has_noexcept_operator_iter_method_end<tp_type>)->decltype(auto)
  {
    return dango::forward<tp_type>(a_arg).dango_operator_end();
  }
}

namespace
dango
{
  template
  <typename tp_type>
  concept has_iter_begin =
    dango::is_object_ignore_ref<tp_type> &&
    requires{ { dango::detail::iter_begin_help(dango::detail::iter_help_prio{ }, dango::declval<tp_type>()) }; };

  template
  <typename tp_type>
  concept has_noexcept_iter_begin =
    dango::has_iter_begin<tp_type> &&
    requires{ { dango::detail::iter_begin_help(dango::detail::iter_help_prio{ }, dango::declval<tp_type>()) }noexcept; };

  template
  <typename tp_type>
  concept has_iter_end =
    dango::is_object_ignore_ref<tp_type> &&
    requires{ { dango::detail::iter_end_help(dango::detail::iter_help_prio{ }, dango::declval<tp_type>()) }; };

  template
  <typename tp_type>
  concept has_noexcept_iter_end =
    dango::has_iter_end<tp_type> &&
    requires{ { dango::detail::iter_end_help(dango::detail::iter_help_prio{ }, dango::declval<tp_type>()) }noexcept; };

  template
  <dango::has_iter_begin tp_type>
  using iter_begin_type =
    decltype(dango::detail::iter_begin_help(dango::detail::iter_help_prio{ }, dango::declval<tp_type>()));

  template
  <dango::has_iter_end tp_type>
  using iter_end_type =
    decltype(dango::detail::iter_end_help(dango::detail::iter_help_prio{ }, dango::declval<tp_type>()));

  template
  <typename tp_type>
  concept is_iterable =
    dango::has_iter_begin<tp_type> &&
    dango::has_iter_end<tp_type> &&
    dango::is_noexcept_equatable<dango::iter_begin_type<tp_type>, dango::iter_end_type<tp_type>>;

  template
  <typename tp_type>
  concept is_noexcept_iterable =
    dango::is_iterable<tp_type> &&
    dango::has_noexcept_iter_begin<tp_type> &&
    dango::has_noexcept_iter_end<tp_type>;

  inline constexpr auto const iter_begin =
    []<typename tp_type>
    (tp_type&& a_arg)constexpr
    noexcept(dango::has_noexcept_iter_begin<tp_type>)->decltype(auto)
    requires(dango::is_iterable<tp_type>)
    {
      return dango::detail::iter_begin_help(dango::detail::iter_help_prio{ }, dango::forward<tp_type>(a_arg));
    };

  inline constexpr auto const iter_end =
    []<typename tp_type>
    (tp_type&& a_arg)constexpr
    noexcept(dango::has_noexcept_iter_end<tp_type>)->decltype(auto)
    requires(dango::is_iterable<tp_type>)
    {
      return dango::detail::iter_end_help(dango::detail::iter_help_prio{ }, dango::forward<tp_type>(a_arg));
    };

  inline constexpr auto const iter_cbegin =
    []<typename tp_type>
    (tp_type const& a_arg)constexpr
    noexcept(dango::has_noexcept_iter_begin<tp_type const&>)->decltype(auto)
    requires(dango::is_iterable<tp_type const&>)
    {
      return dango::iter_begin(a_arg);
    };

  inline constexpr auto const iter_cend =
    []<typename tp_type>
    (tp_type const& a_arg)constexpr
    noexcept(dango::has_noexcept_iter_end<tp_type const&>)->decltype(auto)
    requires(dango::is_iterable<tp_type const&>)
    {
      return dango::iter_end(a_arg);
    };
}

namespace
dango::operators
{
  template
  <typename tp_type>
  requires(dango::is_iterable<tp_type>)
  constexpr auto
  begin
  (tp_type&& a_arg)
  noexcept(dango::has_noexcept_iter_begin<tp_type>)->decltype(auto)
  {
    return dango::iter_begin(dango::forward<tp_type>(a_arg));
  }

  template
  <typename tp_type>
  requires(dango::is_iterable<tp_type>)
  constexpr auto
  end
  (tp_type&& a_arg)
  noexcept(dango::has_noexcept_iter_end<tp_type>)->decltype(auto)
  {
    return dango::iter_end(dango::forward<tp_type>(a_arg));
  }
}

namespace
dango
{
  DANGO_USING_RANGE_BASED_FOR_OPERATORS
}

/*** operator_iter for arrays ***/

namespace
dango::custom
{
  template
  <typename tp_elem, dango::usize tp_size>
  struct operator_iter<tp_elem[tp_size]>;
}

template
<typename tp_elem, dango::usize tp_size>
struct
dango::
custom::
operator_iter<tp_elem[tp_size]>
final
{
  template
  <dango::is_same_ignore_cv<tp_elem> tp_arg>
  static constexpr auto
  begin
  (tp_arg* const a_array)noexcept->tp_arg*
  {
    return a_array;
  }

  static constexpr auto
  end
  (tp_elem const volatile* const a_array)noexcept->void const volatile*
  {
    return a_array + tp_size;
  }

  DANGO_UNCONSTRUCTIBLE(operator_iter)
};

/*** clone ***/

namespace
dango::custom
{
  template
  <typename tp_type>
  struct operator_clone;
}

namespace
dango
{
  template
  <typename tp_type>
  concept has_operator_clone_struct =
    dango::is_object_exclude_array_ignore_ref<tp_type> &&
    requires
    { { dango::custom::operator_clone<dango::remove_cvref<tp_type>>::clone(dango::declval<tp_type>()) }->dango::is_same<dango::remove_cvref<tp_type>>; };

  template
  <typename tp_type>
  concept has_noexcept_operator_clone_struct =
    dango::has_operator_clone_struct<tp_type> &&
    requires
    { { dango::custom::operator_clone<dango::remove_cvref<tp_type>>::clone(dango::declval<tp_type>()) }noexcept->dango::is_same<dango::remove_cvref<tp_type>>; };

  template
  <typename tp_type>
  concept has_operator_clone_method =
    dango::is_class_or_union_ignore_ref<tp_type> &&
    requires{ { dango::declval<tp_type>().dango_operator_clone() }->dango::is_same<dango::remove_cvref<tp_type>>; };

  template
  <typename tp_type>
  concept has_noexcept_operator_clone_method =
    dango::has_operator_clone_method<tp_type> &&
    requires{ { dango::declval<tp_type>().dango_operator_clone() }noexcept->dango::is_same<dango::remove_cvref<tp_type>>; };
}

namespace
dango::detail
{
  using clone_help_prio = dango::priority_tag<dango::uint(2)>;

  template
  <typename tp_type>
  requires(dango::has_operator_clone_struct<tp_type const&>)
  constexpr auto
  clone_help
  (dango::priority_tag<dango::uint(2)> const, tp_type const& a_arg)
  noexcept(dango::has_noexcept_operator_clone_struct<tp_type const&>)->dango::remove_cv<tp_type>
  {
    return dango::custom::operator_clone<dango::remove_cv<tp_type>>::clone(a_arg);
  }

  template
  <typename tp_type>
  requires(dango::has_operator_clone_method<tp_type const&>)
  constexpr auto
  clone_help
  (dango::priority_tag<dango::uint(1)> const, tp_type const& a_arg)
  noexcept(dango::has_noexcept_operator_clone_method<tp_type const&>)->dango::remove_cv<tp_type>
  {
    return a_arg.dango_operator_clone();
  }

  template
  <typename tp_type>
  requires(dango::is_brace_constructible<dango::remove_cv<tp_type>, tp_type const&>)
  constexpr auto
  clone_help
  (dango::priority_tag<dango::uint(0)> const, tp_type const& a_arg)
  noexcept(dango::is_noexcept_brace_constructible<dango::remove_cv<tp_type>, tp_type const&>)->auto
  {
    return dango::remove_cv<tp_type>{ a_arg };
  }
}

/*** is_cloneable is_noexcept_cloneable clone ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_cloneable =
    dango::is_object_exclude_array_ignore_ref<tp_type> &&
    requires{ { dango::detail::clone_help(dango::detail::clone_help_prio{ }, dango::declval<tp_type>()) }->dango::is_same<dango::remove_cvref<tp_type>>; };

  template
  <typename tp_type>
  concept is_noexcept_cloneable =
    dango::is_cloneable<tp_type> &&
    requires{ { dango::detail::clone_help(dango::detail::clone_help_prio{ }, dango::declval<tp_type>()) }noexcept; };

  inline constexpr auto const clone =
    []<typename tp_type>
    (tp_type const& a_arg)constexpr
    noexcept(dango::is_noexcept_cloneable<tp_type const&>)->auto
    requires(dango::is_cloneable<tp_type const&>)
    {
      return dango::detail::clone_help(dango::detail::clone_help_prio{ }, a_arg);
    };
}

/*** aligned_storage ***/

namespace
dango::detail
{
  template
  <dango::usize tp_size, dango::usize tp_align>
  concept aligned_storage_constraint_spec =
    (tp_size != dango::usize(0)) && dango::is_pow_two(tp_align);
}

namespace
dango
{
  template
  <dango::usize tp_size, dango::usize tp_align = alignof(dango::max_align_type)>
  requires(dango::detail::aligned_storage_constraint_spec<tp_size, tp_align>)
  class aligned_storage;

  template
  <typename... tp_types>
  requires(( ... && dango::is_object<tp_types>))
  using aligned_union =
    dango::aligned_storage
    <dango::max(dango::usize(1), sizeof(tp_types)...), dango::max(dango::usize(1), alignof(tp_types)...)>;
}

template
<dango::usize tp_size, dango::usize tp_align>
requires(dango::detail::aligned_storage_constraint_spec<tp_size, tp_align>)
class
dango::
aligned_storage
{
public:
  static inline constexpr dango::usize const c_size = dango::next_multiple(tp_size, tp_align);
  static inline constexpr dango::usize const c_align = tp_align;
public:
  explicit constexpr aligned_storage()noexcept = default;
  constexpr ~aligned_storage()noexcept = default;
public:
  constexpr auto get()const noexcept->void*{ return bytes; }

  template
  <dango::is_object tp_type>
  requires((sizeof(tp_type) <= c_size) && (alignof(tp_type) <= c_align))
  constexpr auto
  get_as()const noexcept->tp_type*
  {
    return static_cast<tp_type*>(get());
  }

  template
  <dango::is_object tp_type>
  requires((sizeof(tp_type) <= c_size) && (alignof(tp_type) <= c_align))
  constexpr auto
  launder_get()const noexcept->tp_type*
  {
    return dango::launder(get_as<tp_type>());
  }
public:
  alignas(c_align) mutable dango::byte bytes[c_size];
public:
  DANGO_UNMOVEABLE(aligned_storage)
};

namespace
dango
{
  template
  <dango::usize tp_size, dango::usize tp_align>
  constexpr auto
  aligned_storage_get
  (dango::aligned_storage<tp_size, tp_align> const& a_storage)noexcept->void*
  {
    return a_storage.bytes;
  }

  template
  <dango::is_object tp_type, dango::usize tp_size, dango::usize tp_align>
  requires((sizeof(tp_type) <= dango::next_multiple(tp_size, tp_align)) && (alignof(tp_type) <= tp_align))
  constexpr auto
  aligned_storage_get_as
  (dango::aligned_storage<tp_size, tp_align> const& a_storage)noexcept->tp_type*
  {
    return static_cast<tp_type*>(dango::aligned_storage_get(a_storage));
  }

  template
  <dango::is_object tp_type, dango::usize tp_size, dango::usize tp_align>
  requires((sizeof(tp_type) <= dango::next_multiple(tp_size, tp_align)) && (alignof(tp_type) <= tp_align))
  constexpr auto
  aligned_storage_launder_get
  (dango::aligned_storage<tp_size, tp_align> const& a_storage)noexcept->tp_type*
  {
    return dango::launder(dango::aligned_storage_get_as<tp_type>(a_storage));
  }
}

#endif
