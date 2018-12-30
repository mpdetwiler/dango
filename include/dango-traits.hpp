#ifndef __DANGO_TRAITS_HPP__
#define __DANGO_TRAITS_HPP__ 1

#include "dango-def.hpp"

/*** integral_constant ***/

namespace
dango::detail
{
  template
  <typename tp_integral, tp_integral tp_value>
  struct integral_constant;

  template
  <bool tp_value>
  using bool_constant = dango::detail::integral_constant<bool, tp_value>;

  using true_type = dango::detail::bool_constant<true>;

  using false_type = dango::detail::bool_constant<false>;
}

template
<typename tp_integral, tp_integral tp_value>
struct
dango::
detail::
integral_constant
{
  using value_type = tp_integral;

  static tp_integral const value;
};

template
<typename tp_integral, tp_integral tp_value>
constexpr tp_integral const
dango::
detail::
integral_constant<tp_integral, tp_value>::
value = tp_value;

/*** is_same ***/

namespace
dango::detail
{
  template
  <typename tp_type1, typename tp_type2>
  struct is_same_class;

  template
  <typename tp_type>
  struct is_same_class<tp_type, tp_type>;
}

namespace
dango
{
  template
  <typename tp_type1, typename tp_type2>
  constexpr bool const is_same =
    dango::detail::is_same_class<tp_type1, tp_type2>::value;
}

template
<typename tp_type1, typename tp_type2>
struct
dango::
detail::
is_same_class:
dango::detail::false_type
{

};

template
<typename tp_type>
struct
dango::
detail::
is_same_class<tp_type, tp_type>:
dango::detail::true_type
{

};

/*** remove_const ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct remove_const_class;

  template
  <typename tp_type>
  struct remove_const_class<tp_type const>;
}

namespace
dango
{
  template
  <typename tp_type>
  using remove_const =
    typename dango::detail::remove_const_class<tp_type>::type;
}

template
<typename tp_type>
struct
dango::
detail::
remove_const_class
{
  using type = tp_type;
};

template
<typename tp_type>
struct
dango::
detail::
remove_const_class<tp_type const>
{
  using type = tp_type;
};

/*** remove_volatile ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct remove_volatile_class;

  template
  <typename tp_type>
  struct remove_volatile_class<tp_type volatile>;
}

namespace
dango
{
  template
  <typename tp_type>
  using remove_volatile =
    typename dango::detail::remove_volatile_class<tp_type>::type;
}

template
<typename tp_type>
struct
dango::
detail::
remove_volatile_class
{
  using type = tp_type;
};

template
<typename tp_type>
struct
dango::
detail::
remove_volatile_class<tp_type volatile>
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
dango::detail
{
  template
  <typename tp_type>
  struct is_void_class;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_void =
    dango::detail::is_void_class<dango::remove_cv<tp_type>>::value;
}

template
<typename tp_type>
struct
dango::
detail::
is_void_class:
dango::detail::bool_constant<dango::is_same<void, tp_type>>
{

};

/*** is_null_ptr ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct is_null_ptr_class;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_null_ptr =
    dango::detail::is_null_ptr_class<dango::remove_cv<tp_type>>::value;
}

template
<typename tp_type>
struct
dango::
detail::
is_null_ptr_class:
dango::detail::bool_constant<dango::is_same<dango::nullptr_tag, tp_type>>
{

};

/*** is_uint ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct is_uint_class;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_uint =
    dango::detail::is_uint_class<dango::remove_cv<tp_type>>::value;
}

template
<typename tp_type>
struct
dango::
detail::
is_uint_class:
dango::detail::bool_constant
<
  dango::is_same<tp_type, dango::u_byte> ||
  dango::is_same<tp_type, dango::u_short> ||
  dango::is_same<tp_type, dango::u_int> ||
  dango::is_same<tp_type, dango::u_long> ||
  dango::is_same<tp_type, dango::u_cent>
>
{

};

/*** is_sint ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct is_sint_class;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_sint =
    dango::detail::is_sint_class<dango::remove_cv<tp_type>>::value;
}

template
<typename tp_type>
struct
dango::
detail::
is_sint_class:
dango::detail::bool_constant
<
  dango::is_same<tp_type, dango::s_byte> ||
  dango::is_same<tp_type, dango::s_short> ||
  dango::is_same<tp_type, dango::s_int> ||
  dango::is_same<tp_type, dango::s_long> ||
  dango::is_same<tp_type, dango::s_cent>
>
{

};

/*** is_int ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct is_int_class;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_int =
    dango::detail::is_int_class<tp_type>::value;
}

template
<typename tp_type>
struct
dango::
detail::
is_int_class:
dango::detail::bool_constant
<
  dango::is_uint<tp_type> ||
  dango::is_sint<tp_type>
>
{

};

/*** is_integral ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct is_integral_class;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_integral =
    dango::detail::is_integral_class<dango::remove_cv<tp_type>>::value;
}

template
<typename tp_type>
struct
dango::
detail::
is_integral_class:
dango::detail::bool_constant
<
  dango::is_same<tp_type, bool> ||
  dango::is_same<tp_type, char> ||
  dango::is_same<tp_type, dango::wchar> ||
  dango::is_same<tp_type, dango::dchar> ||
  dango::is_same<tp_type, wchar_t> ||
  dango::is_int<tp_type>
>
{

};

/*** is_float ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct is_float_class;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_float =
    dango::detail::is_float_class<dango::remove_cv<tp_type>>::value;
}

template
<typename tp_type>
struct
dango::
detail::
is_float_class:
dango::detail::bool_constant
<
  dango::is_same<tp_type, float> ||
  dango::is_same<tp_type, double> ||
  dango::is_same<tp_type, dango::real>
>
{

};

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
  struct is_ptr_class;

  template
  <typename tp_type>
  struct is_ptr_class<tp_type*>;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_ptr =
    dango::detail::is_ptr_class<dango::remove_cv<tp_type>>::value;
}

template
<typename tp_type>
struct
dango::
detail::
is_ptr_class:
dango::detail::false_type
{

};

template
<typename tp_type>
struct
dango::
detail::
is_ptr_class<tp_type*>:
dango::detail::true_type
{

};

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
  struct is_member_ptr_class;

  template
  <typename tp_type, typename tp_class>
  struct is_member_ptr_class<tp_type tp_class::*>;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_member_ptr =
    dango::detail::is_member_ptr_class<dango::remove_cv<tp_type>>::value;
}

template
<typename tp_type>
struct
dango::
detail::
is_member_ptr_class:
dango::detail::false_type
{

};

template
<typename tp_type, typename tp_class>
struct
dango::
detail::
is_member_ptr_class<tp_type tp_class::*>:
dango::detail::true_type
{

};

/*** is_member_func_ptr ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct is_member_func_ptr_class;

  template
  <typename tp_type, typename tp_class>
  struct is_member_func_ptr_class<tp_type tp_class::*>;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_member_func_ptr =
    dango::detail::is_member_func_ptr_class<dango::remove_cv<tp_type>>::value;
}

template
<typename tp_type>
struct
dango::
detail::
is_member_func_ptr_class:
dango::detail::false_type
{

};

template
<typename tp_type, typename tp_class>
struct
dango::
detail::
is_member_func_ptr_class<tp_type tp_class::*>:
dango::detail::bool_constant<dango::is_func<tp_type>>
{

};

/*** is_member_data_ptr ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_member_data_ptr =
    dango::is_member_ptr<tp_type> &&
    !dango::is_member_func_ptr<tp_type>;
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

/*** conditional ***/

namespace
dango::detail
{
  template
  <bool tp_cond, typename tp_true_type, typename tp_false_type>
  struct conditional_class;

  template
  <typename tp_true_type, typename tp_false_type>
  struct conditional_class<true, tp_true_type, tp_false_type>;
}

namespace
dango
{
  template
  <bool tp_cond, typename tp_true_type, typename tp_false_type>
  using conditional =
    typename dango::detail::conditional_class<tp_cond, tp_true_type, tp_false_type>;
}

template
<bool tp_cond, typename tp_true_type, typename tp_false_type>
struct
dango::
detail::
conditional_class
{
  using type = tp_false_type;
};

template
<typename tp_true_type, typename tp_false_type>
struct
dango::
detail::
conditional_class<true, tp_true_type, tp_false_type>
{
  using type = tp_true_type;
};


/*** enable_if ***/

namespace
dango::detail
{
  template
  <bool tp_cond, typename tp_type>
  struct enable_if_class;

  template
  <typename tp_type>
  struct enable_if_class<true, tp_type>;
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
    typename dango::detail::enable_if_class<tp_cond, tp_type>::type;
}

template
<bool tp_cond, typename tp_type>
struct
dango::
detail::
enable_if_class
{

};

template
<typename tp_type>
struct
dango::
detail::
enable_if_class<true, tp_type>
{
  using type = tp_type;
};

#endif

