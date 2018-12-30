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
dango::detail::is_same_class<void, dango::remove_cv<tp_type>>
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
dango::detail::is_same_class<dango::nullptr_tag, dango::remove_cv<tp_type>>
{

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

