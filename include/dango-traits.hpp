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
    dango::detail::is_void_class<tp_type>::value;
}

template
<typename tp_type>
struct
dango::
detail::
is_void_class:
dango::detail::bool_constant<dango::is_same<void, dango::remove_cv<tp_type>>>
{

};

/*** is_nullptr ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct is_nullptr_class;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_nullptr =
    dango::detail::is_nullptr_class<tp_type>::value;
}

template
<typename tp_type>
struct
dango::
detail::
is_nullptr_class:
dango::detail::bool_constant<dango::is_same<dango::nullptr_tag, dango::remove_cv<tp_type>>>
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
    dango::detail::is_int_class<dango::remove_cv<tp_type>>::value;
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
dango::detail
{
  template
  <typename tp_type>
  struct is_array_class;

  template
  <typename tp_type>
  struct is_array_class<tp_type[]>;

  template
  <typename tp_type, dango::usize tp_size>
  struct is_array_class<tp_type[tp_size]>;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_array =
    dango::detail::is_array_class<tp_type>::value;
}

template
<typename tp_type>
struct
dango::
detail::
is_array_class:
dango::detail::false_type
{

};

template
<typename tp_type>
struct
dango::
detail::
is_array_class<tp_type[]>:
dango::detail::true_type
{

};

template
<typename tp_type, dango::usize tp_size>
struct
dango::
detail::
is_array_class<tp_type[tp_size]>:
dango::detail::true_type
{

};

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

/*** is_function ***/

// TODO

/*** is_pointer ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct is_pointer_class;

  template
  <typename tp_type>
  struct is_pointer_class<tp_type*>;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_pointer =
    dango::detail::is_pointer_class<dango::remove_cv<tp_type>>::value;
}

template
<typename tp_type>
struct
dango::
detail::
is_pointer_class:
dango::detail::false_type
{

};

template
<typename tp_type>
struct
dango::
detail::
is_pointer_class<tp_type*>:
dango::detail::true_type
{

};

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
  template
  <bool tp_cond, typename tp_type = void>
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

