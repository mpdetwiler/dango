#ifndef __DANGO_TRAITS_HPP__
#define __DANGO_TRAITS_HPP__ 1

#include "dango-def.hpp"

/*** declval ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr auto declval()noexcept->tp_type&&;
}

/*** is_same ***/

namespace
dango
{
  template
  <typename tp_type1, typename tp_type2>
  constexpr bool const is_same = false;

  template
  <typename tp_type>
  constexpr bool const is_same<tp_type, tp_type> = true;
}

/*** remove_const ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct remove_const_help;

  template
  <typename tp_type>
  struct remove_const_help<tp_type const>;
}

namespace
dango
{
  template
  <typename tp_type>
  using remove_const =
    typename dango::detail::remove_const_help<tp_type>::type;
}

template
<typename tp_type>
struct
dango::
detail::
remove_const_help
{
  using type = tp_type;
};

template
<typename tp_type>
struct
dango::
detail::
remove_const_help<tp_type const>
{
  using type = tp_type;
};

/*** remove_volatile ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct remove_volatile_help;

  template
  <typename tp_type>
  struct remove_volatile_help<tp_type volatile>;
}

namespace
dango
{
  template
  <typename tp_type>
  using remove_volatile =
    typename dango::detail::remove_volatile_help<tp_type>::type;
}

template
<typename tp_type>
struct
dango::
detail::
remove_volatile_help
{
  using type = tp_type;
};

template
<typename tp_type>
struct
dango::
detail::
remove_volatile_help<tp_type volatile>
{
  using type = tp_type;
};

/*** remove_cv ***/

namespace
dango
{
  template
  <typename tp_type>
  using remove_cv = dango::remove_volatile<dango::remove_const<tp_type>>;
}

/*** is_void ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_void =
    dango::is_same<dango::remove_cv<tp_type>, void>;
}

/*** is_null_ptr ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_null_ptr =
    dango::is_same<dango::remove_cv<tp_type>, dango::nullptr_tag>;
}

/*** is_uint ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  constexpr bool const is_uint_help =
    dango::is_same<tp_type, dango::u_byte> ||
    dango::is_same<tp_type, dango::u_short> ||
    dango::is_same<tp_type, dango::u_int> ||
    dango::is_same<tp_type, dango::u_long> ||
    dango::is_same<tp_type, dango::u_cent>;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_uint =
    dango::detail::is_uint_help<dango::remove_cv<tp_type>>;
}

/*** is_sint ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  constexpr bool const is_sint_help =
    dango::is_same<tp_type, dango::s_byte> ||
    dango::is_same<tp_type, dango::s_short> ||
    dango::is_same<tp_type, dango::s_int> ||
    dango::is_same<tp_type, dango::s_long> ||
    dango::is_same<tp_type, dango::s_cent>;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_sint =
    dango::detail::is_sint_help<dango::remove_cv<tp_type>>;
}

/*** is_int ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_int =
    dango::is_uint<tp_type> || dango::is_sint<tp_type>;
}

/*** is_integral ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  constexpr bool const is_integral_help =
    dango::is_same<tp_type, bool> ||
    dango::is_same<tp_type, char> ||
    dango::is_same<tp_type, dango::wchar> ||
    dango::is_same<tp_type, dango::dchar> ||
    dango::is_same<tp_type, wchar_t> ||
    dango::is_int<tp_type>;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_integral =
    dango::detail::is_integral_help<dango::remove_cv<tp_type>>;
}

/*** is_float ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  constexpr bool const is_float_help =
    dango::is_same<tp_type, float> ||
    dango::is_same<tp_type, double> ||
    dango::is_same<tp_type, dango::real>;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_float =
    dango::detail::is_float_help<dango::remove_cv<tp_type>>;
}

/*** is_array ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_array = false;

  template
  <typename tp_type>
  constexpr bool const is_array<tp_type[]> = true;

  template
  <typename tp_type, dango::usize tp_size>
  constexpr bool const is_array<tp_type[tp_size]> = true;
}

/*** is_enum ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_enum = __is_enum(tp_type);
}

/*** is_union ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_union = __is_union(tp_type);
}

/*** is_class ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_class = __is_class(tp_type);
}

/*** is_ptr ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  constexpr bool const is_ptr_help = false;

  template
  <typename tp_type>
  constexpr bool const is_ptr_help<tp_type*> = true;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_ptr =
    dango::detail::is_ptr_help<dango::remove_cv<tp_type>>;
}

/*** is_lvalue_ref ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_lvalue_ref = false;

  template
  <typename tp_type>
  constexpr bool const is_lvalue_ref<tp_type&> = true;
}

/*** is_rvalue_ref ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_rvalue_ref = false;

  template
  <typename tp_type>
  constexpr bool const is_rvalue_ref<tp_type&&> = true;
}

/*** is_ref ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_ref =
    dango::is_lvalue_ref<tp_type> || dango::is_rvalue_ref<tp_type>;
}

/*** is_func ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_func =
    !dango::is_ref<tp_type> &&
    dango::is_same<tp_type const volatile, dango::remove_cv<tp_type>>;
}

/*** is_member_ptr ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  constexpr bool const is_member_ptr_help = false;

  template
  <typename tp_type, typename tp_class>
  constexpr bool const is_member_ptr_help<tp_type tp_class::*> = true;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_member_ptr =
    dango::detail::is_member_ptr_help<dango::remove_cv<tp_type>>;
}

/*** is_member_func_ptr ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  constexpr bool const is_member_func_ptr_help = false;

  template
  <typename tp_type, typename tp_class>
  constexpr bool const is_member_func_ptr_help<tp_type tp_class::*> =
    dango::is_func<tp_type>;
}

namespace dango
{
  template
  <typename tp_type>
  constexpr bool const is_member_func_ptr =
    dango::detail::is_member_func_ptr_help<dango::remove_cv<tp_type>>;
}

/*** is_member_data_ptr ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_member_data_ptr =
    dango::is_member_ptr<tp_type> && !dango::is_member_func_ptr<tp_type>;
}

/*** is_arithmetic ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_arithmetic =
    dango::is_integral<tp_type> || dango::is_float<tp_type>;
}

/*** is_fundamnetal ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_fundamental =
    dango::is_void<tp_type> ||
    dango::is_null_ptr<tp_type> ||
    dango::is_arithmetic<tp_type>;
}

/*** is_scalar ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_scalar =
    dango::is_arithmetic<tp_type> ||
    dango::is_enum<tp_type> ||
    dango::is_ptr<tp_type> ||
    dango::is_member_ptr<tp_type> ||
    dango::is_null_ptr<tp_type>;
}

/*** is_object ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_object =
    dango::is_scalar<tp_type> ||
    dango::is_array<tp_type> ||
    dango::is_union<tp_type> ||
    dango::is_class<tp_type>;
}

/*** is_compound ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_compound = !dango::is_fundamental<tp_type>;
}

/*** is_const ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_const = false;

  template
  <typename tp_type>
  constexpr bool const is_const<tp_type const> = true;
}

/*** is_volatile ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_volatile = false;

  template
  <typename tp_type>
  constexpr bool const is_volatile<tp_type volatile> = true;
}

/*** is_trivial ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_trivial = __is_trivial(tp_type);
}

/*** is_trivially_copyable ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_trivially_copyable = __is_trivially_copyable(tp_type);
}

/*** is_standard_layout ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_standard_layout = __is_standard_layout(tp_type);
}

/*** is_empty ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_empty = __is_empty(tp_type);
}

/*** is_polymorphic ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_polymorphic = __is_polymorphic(tp_type);
}

/*** is_abstract ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_abstract = __is_abstract(tp_type);
}

/*** is_signed ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_signed =
    dango::is_arithmetic<tp_type> && (tp_type(-1) < tp_type(0));
}

/*** is_unsigned ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_unsigned =
    dango::is_arithmetic<tp_type> && (tp_type(-1) > tp_type(0));
}

/*** is_base_of ***/

namespace
dango
{
  template
  <typename tp_base, typename tp_derived>
  constexpr bool const is_base_of = __is_base_of(tp_base, tp_derived);
}

/*** is_convertible is_noexcept_convertible***/

namespace
dango::detail
{
  template
  <typename tp_arg>
  constexpr void
  is_convertible_test(tp_arg)noexcept;

  enum class
  is_convertible_normal_tag:
  dango::uint32
  {

  };

  inline constexpr dango::detail::is_convertible_normal_tag const is_convertible_normal{ };

  enum class
  is_convertible_noexcept_tag:
  dango::uint32
  {

  };

  inline constexpr dango::detail::is_convertible_noexcept_tag const is_convertible_noexcept{ };

  template
  <
    typename tp_from,
    typename tp_to,
    typename tp_enabled =
      decltype(dango::detail::is_convertible_test<tp_to>(dango::declval<tp_from>()))
  >
  constexpr auto
  is_convertible_help
  (dango::detail::is_convertible_normal_tag const)noexcept->bool
  {
    return true;
  }

  template
  <
    typename tp_from,
    typename tp_to,
    typename tp_enabled =
      decltype(dango::detail::is_convertible_test<tp_to>(dango::declval<tp_from>()))
  >
  constexpr auto
  is_convertible_help
  (dango::detail::is_convertible_noexcept_tag const)noexcept->bool
  {
    return noexcept(dango::detail::is_convertible_test<tp_to>(dango::declval<tp_from>()));
  }

  template
  <typename tp_from,typename tp_to>
  constexpr auto
  is_convertible_help
  (...)noexcept->bool
  {
    return false;
  }
}

namespace
dango
{
  template
  <typename tp_from, typename tp_to>
  constexpr bool const is_convertible =
    (
      dango::is_void<tp_from> &&
      dango::is_void<tp_to>
    ) ||
    (
      !dango::is_array<tp_to> &&
      !dango::is_func<tp_to> &&
      dango::detail::is_convertible_help<tp_from, tp_to>(dango::detail::is_convertible_normal)
    );

  template
  <typename tp_from, typename tp_to>
  constexpr bool const is_noexcept_convertible =
    (
      dango::is_void<tp_from> &&
      dango::is_void<tp_to>
    ) ||
    (
      !dango::is_array<tp_to> &&
      !dango::is_func<tp_to> &&
      dango::detail::is_convertible_help<tp_from, tp_to>(dango::detail::is_convertible_noexcept)
    );
}

/*** conditional ***/

namespace
dango::detail
{
  template
  <bool tp_cond, typename tp_true_type, typename tp_false_type>
  struct conditional_help;

  template
  <typename tp_true_type, typename tp_false_type>
  struct conditional_help<true, tp_true_type, tp_false_type>;
}

namespace
dango
{
  template
  <bool tp_cond, typename tp_true_type, typename tp_false_type>
  using conditional =
    typename dango::detail::conditional_help<tp_cond, tp_true_type, tp_false_type>;
}

template
<bool tp_cond, typename tp_true_type, typename tp_false_type>
struct
dango::
detail::
conditional_help
{
  using type = tp_false_type;
};

template
<typename tp_true_type, typename tp_false_type>
struct
dango::
detail::
conditional_help<true, tp_true_type, tp_false_type>
{
  using type = tp_true_type;
};


/*** enable_if ***/

namespace
dango::detail
{
  template
  <bool tp_cond, typename tp_type>
  struct enable_if_help;

  template
  <typename tp_type>
  struct enable_if_help<true, tp_type>;
}

namespace
dango
{
  enum class
  enable_tag:
  dango::uint32
  {

  };

  inline constexpr dango::enable_tag const enable_val{ };

  template
  <bool tp_cond, typename tp_type = dango::enable_tag>
  using enable_if =
    typename dango::detail::enable_if_help<tp_cond, tp_type>::type;
}

template
<bool tp_cond, typename tp_type>
struct
dango::
detail::
enable_if_help
{

};

template
<typename tp_type>
struct
dango::
detail::
enable_if_help<true, tp_type>
{
  using type = tp_type;
};

#endif

