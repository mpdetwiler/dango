#ifndef DANGO_TRAITS_HPP_INCLUDED
#define DANGO_TRAITS_HPP_INCLUDED

#include "dango-def.hpp"

/*** type_identity ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct
  type_identity_help
  final
  {
    using type = tp_type;

    DANGO_UNCONSTRUCTIBLE(type_identity_help)
  };
}

namespace
dango
{
  template
  <typename tp_type>
  using type_identity =
    typename dango::detail::type_identity_help<tp_type>::type;
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
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_const_help)
};

template
<typename tp_type>
struct
dango::
detail::
remove_const_help<tp_type const>
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_const_help)
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
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_volatile_help)
};

template
<typename tp_type>
struct
dango::
detail::
remove_volatile_help<tp_type volatile>
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_volatile_help)
};

/*** remove_cv ***/

namespace
dango
{
  template
  <typename tp_type>
  using remove_cv = dango::remove_volatile<dango::remove_const<tp_type>>;
}

/*** remove_ref ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct remove_ref_help;

  template
  <typename tp_type>
  struct remove_ref_help<tp_type&>;

  template
  <typename tp_type>
  struct remove_ref_help<tp_type&&>;
}

namespace
dango
{
  template
  <typename tp_type>
  using remove_ref =
    typename dango::detail::remove_ref_help<tp_type>::type;
}

template
<typename tp_type>
struct
dango::
detail::
remove_ref_help
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_ref_help)
};

template
<typename tp_type>
struct
dango::
detail::
remove_ref_help<tp_type&>
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_ref_help)
};

template
<typename tp_type>
struct
dango::
detail::
remove_ref_help<tp_type&&>
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_ref_help)
};

/*** remove_cvref ***/

namespace
dango
{
  template
  <typename tp_type>
  using remove_cvref =
    dango::remove_cv<dango::remove_ref<tp_type>>;
}

/*** remove_ptr ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct remove_ptr_help;

  template
  <typename tp_type>
  struct remove_ptr_help<tp_type*>;

  template
  <typename tp_type>
  struct remove_ptr_help<tp_type* const>;

  template
  <typename tp_type>
  struct remove_ptr_help<tp_type* volatile>;

  template
  <typename tp_type>
  struct remove_ptr_help<tp_type* const volatile>;
}

namespace
dango
{
  template
  <typename tp_type>
  using remove_ptr =
    typename dango::detail::remove_ptr_help<tp_type>::type;
}

template
<typename tp_type>
struct
dango::
detail::
remove_ptr_help
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_ptr_help)
};

template
<typename tp_type>
struct
dango::
detail::
remove_ptr_help<tp_type*>
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_ptr_help)
};

template
<typename tp_type>
struct
dango::
detail::
remove_ptr_help<tp_type* const>
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_ptr_help)
};

template
<typename tp_type>
struct
dango::
detail::
remove_ptr_help<tp_type* volatile>
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_ptr_help)
};

template
<typename tp_type>
struct
dango::
detail::
remove_ptr_help<tp_type* const volatile>
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_ptr_help)
};

/*** remove_all_ptr ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct remove_all_ptr_help;

  template
  <typename tp_type>
  struct remove_all_ptr_help<tp_type*>;

  template
  <typename tp_type>
  struct remove_all_ptr_help<tp_type* const>;

  template
  <typename tp_type>
  struct remove_all_ptr_help<tp_type* volatile>;

  template
  <typename tp_type>
  struct remove_all_ptr_help<tp_type* const volatile>;
}

namespace
dango
{
  template
  <typename tp_type>
  using remove_all_ptr =
    typename dango::detail::remove_all_ptr_help<tp_type>::type;
}

template
<typename tp_type>
struct
dango::
detail::
remove_all_ptr_help
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_all_ptr_help)
};

template
<typename tp_type>
struct
dango::
detail::
remove_all_ptr_help<tp_type*>
final
{
  using type = dango::remove_all_ptr<tp_type>;

  DANGO_UNCONSTRUCTIBLE(remove_all_ptr_help)
};

template
<typename tp_type>
struct
dango::
detail::
remove_all_ptr_help<tp_type* const>
final
{
  using type = dango::remove_all_ptr<tp_type>;

  DANGO_UNCONSTRUCTIBLE(remove_all_ptr_help)
};

template
<typename tp_type>
struct
dango::
detail::
remove_all_ptr_help<tp_type* volatile>
final
{
  using type = dango::remove_all_ptr<tp_type>;

  DANGO_UNCONSTRUCTIBLE(remove_all_ptr_help)
};

template
<typename tp_type>
struct
dango::
detail::
remove_all_ptr_help<tp_type* const volatile>
final
{
  using type = dango::remove_all_ptr<tp_type>;

  DANGO_UNCONSTRUCTIBLE(remove_all_ptr_help)
};

/*** remove_array ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct remove_array_help;

  template
  <typename tp_type>
  struct remove_array_help<tp_type[]>;

  template
  <typename tp_type, dango::usize tp_size>
  struct remove_array_help<tp_type[tp_size]>;
}

namespace
dango
{
  template
  <typename tp_type>
  using remove_array =
    typename dango::detail::remove_array_help<tp_type>::type;
}

template
<typename tp_type>
struct
dango::
detail::
remove_array_help
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_array_help)
};

template
<typename tp_type>
struct
dango::
detail::
remove_array_help<tp_type[]>
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_array_help)
};

template
<typename tp_type, dango::usize tp_size>
struct
dango::
detail::
remove_array_help<tp_type[tp_size]>
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_array_help)
};

/*** remove_all_array ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct remove_all_array_help;

  template
  <typename tp_type>
  struct remove_all_array_help<tp_type[]>;

  template
  <typename tp_type, dango::usize tp_size>
  struct remove_all_array_help<tp_type[tp_size]>;
}

namespace
dango
{
  template
  <typename tp_type>
  using remove_all_array =
    typename dango::detail::remove_all_array_help<tp_type>::type;
}

template
<typename tp_type>
struct
dango::
detail::
remove_all_array_help
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(remove_all_array_help)
};

template
<typename tp_type>
struct
dango::
detail::
remove_all_array_help<tp_type[]>
final
{
  using type = dango::remove_all_array<tp_type>;

  DANGO_UNCONSTRUCTIBLE(remove_all_array_help)
};

template
<typename tp_type, dango::usize tp_size>
struct
dango::
detail::
remove_all_array_help<tp_type[tp_size]>
final
{
  using type = dango::remove_all_array<tp_type>;

  DANGO_UNCONSTRUCTIBLE(remove_all_array_help)
};

/*** copy_cv ***/

namespace
dango::detail
{
  template
  <typename tp_from, typename tp_to>
  struct copy_cv_help;

  template
  <typename tp_from, typename tp_to>
  struct copy_cv_help<tp_from const, tp_to>;

  template
  <typename tp_from, typename tp_to>
  struct copy_cv_help<tp_from volatile, tp_to>;

  template
  <typename tp_from, typename tp_to>
  struct copy_cv_help<tp_from const volatile, tp_to>;
}

namespace
dango
{
  template
  <typename tp_from, typename tp_to>
  using copy_cv =
    typename dango::detail::copy_cv_help<tp_from, tp_to>::type;
}

template
<typename tp_from, typename tp_to>
struct
dango::
detail::
copy_cv_help
final
{
  using type = tp_to;

  DANGO_UNCONSTRUCTIBLE(copy_cv_help)
};

template
<typename tp_from, typename tp_to>
struct
dango::
detail::
copy_cv_help<tp_from const, tp_to>
final
{
  using type = tp_to const;

  DANGO_UNCONSTRUCTIBLE(copy_cv_help)
};

template
<typename tp_from, typename tp_to>
struct
dango::
detail::
copy_cv_help<tp_from volatile, tp_to>
final
{
  using type = tp_to volatile;

  DANGO_UNCONSTRUCTIBLE(copy_cv_help)
};

template
<typename tp_from, typename tp_to>
struct
dango::
detail::
copy_cv_help<tp_from const volatile, tp_to>
final
{
  using type = tp_to const volatile;

  DANGO_UNCONSTRUCTIBLE(copy_cv_help)
};

/*** decay ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct decay_help;

  template
  <typename tp_type>
  struct decay_help<tp_type[]>;

  template
  <typename tp_type, dango::usize tp_size>
  struct decay_help<tp_type[tp_size]>;

  template
  <typename tp_ret, typename... tp_args, bool tp_noexcept>
  struct decay_help<tp_ret(tp_args...)noexcept(tp_noexcept)>;
}

namespace
dango
{
  template
  <typename tp_type>
  using decay =
    typename dango::detail::decay_help<dango::remove_ref<tp_type>>::type;
}

template
<typename tp_type>
struct
dango::
detail::
decay_help
final
{
  using type = dango::remove_cv<tp_type>;

  DANGO_UNCONSTRUCTIBLE(decay_help)
};

template
<typename tp_type>
struct
dango::
detail::
decay_help<tp_type[]>
final
{
  using type = tp_type*;

  DANGO_UNCONSTRUCTIBLE(decay_help)
};

template
<typename tp_type, dango::usize tp_size>
struct
dango::
detail::
decay_help<tp_type[tp_size]>
final
{
  using type = tp_type*;

  DANGO_UNCONSTRUCTIBLE(decay_help)
};

template
<typename tp_ret, typename... tp_args, bool tp_noexcept>
struct
dango::
detail::
decay_help<tp_ret(tp_args...)noexcept(tp_noexcept)>
final
{
  using type = tp_ret(*)(tp_args...)noexcept(tp_noexcept);

  DANGO_UNCONSTRUCTIBLE(decay_help)
};

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
    typename dango::detail::conditional_help<tp_cond, tp_true_type, tp_false_type>::type;
}

template
<bool tp_cond, typename tp_true_type, typename tp_false_type>
struct
dango::
detail::
conditional_help
final
{
  using type = tp_false_type;

  DANGO_UNCONSTRUCTIBLE(conditional_help)
};

template
<typename tp_true_type, typename tp_false_type>
struct
dango::
detail::
conditional_help<true, tp_true_type, tp_false_type>
final
{
  using type = tp_true_type;

  DANGO_UNCONSTRUCTIBLE(conditional_help)
};

/*** is_same ***/

namespace
dango::detail
{
  template
  <typename tp_type1, typename tp_type2>
  inline constexpr bool const is_same_help = false;

  template
  <typename tp_type>
  inline constexpr bool const is_same_help<tp_type, tp_type> = true;
}

namespace
dango
{
  template
  <typename tp_type1, typename tp_type2>
  concept is_same =
    dango::detail::is_same_help<tp_type1, tp_type2>;

  template
  <typename tp_type1, typename tp_type2>
  concept is_same_ignore_cv =
    dango::is_same<tp_type1, tp_type2> || dango::is_same<dango::remove_cv<tp_type1>, dango::remove_cv<tp_type2>>;

  template
  <typename tp_type1, typename tp_type2>
  concept is_same_ignore_ref =
    dango::is_same<tp_type1, tp_type2> || dango::is_same<dango::remove_ref<tp_type1>, dango::remove_ref<tp_type2>>;

  template
  <typename tp_type1, typename tp_type2>
  concept is_same_ignore_cvref =
    dango::is_same_ignore_cv<tp_type1, tp_type2> ||
    dango::is_same_ignore_ref<tp_type1, tp_type2> ||
    dango::is_same_ignore_cv<dango::remove_ref<tp_type1>, dango::remove_ref<tp_type2>>;
}

/*** is_void ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_void = dango::is_same_ignore_cv<tp_type, void>;
}

/*** is_null_tag ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_null_tag = dango::is_same_ignore_cv<tp_type, dango::null_tag>;
}

/*** is_bool ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_bool = dango::is_same_ignore_cv<tp_type, bool>;
}

/*** declval ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr auto declval()noexcept->tp_type&&;

  template
  <dango::is_void tp_type>
  constexpr void declval()noexcept;
}

/*** is_uint ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_uint =
    dango::is_same_ignore_cv<tp_type, dango::integer::u_char> ||
    dango::is_same_ignore_cv<tp_type, dango::integer::u_short> ||
    dango::is_same_ignore_cv<tp_type, dango::integer::u_int> ||
    dango::is_same_ignore_cv<tp_type, dango::integer::u_long> ||
    dango::is_same_ignore_cv<tp_type, dango::integer::u_longlong>;
}

/*** is_sint ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_sint =
    dango::is_same_ignore_cv<tp_type, dango::integer::s_char> ||
    dango::is_same_ignore_cv<tp_type, dango::integer::s_short> ||
    dango::is_same_ignore_cv<tp_type, dango::integer::s_int> ||
    dango::is_same_ignore_cv<tp_type, dango::integer::s_long> ||
    dango::is_same_ignore_cv<tp_type, dango::integer::s_longlong>;
}

/*** is_int ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_int =
    dango::is_uint<tp_type> || dango::is_sint<tp_type>;
}

/*** is_char ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_char =
    dango::is_same_ignore_cv<tp_type, char> ||
    dango::is_same_ignore_cv<tp_type, wchar_t> ||
    dango::is_same_ignore_cv<tp_type, dango::bchar> ||
    dango::is_same_ignore_cv<tp_type, dango::wchar> ||
    dango::is_same_ignore_cv<tp_type, dango::dchar>;
}

/*** is_integral_exclude_bool ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_integral_exclude_bool =
    dango::is_int<tp_type> || dango::is_char<tp_type>;
}

/* is_integral */

namespace
dango
{
  template
  <typename tp_type>
  concept is_integral =
    dango::is_integral_exclude_bool<tp_type> || dango::is_bool<tp_type>;
}

/*** is_float ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_float =
    dango::is_same_ignore_cv<tp_type, float> ||
    dango::is_same_ignore_cv<tp_type, double> ||
    dango::is_same_ignore_cv<tp_type, dango::real>;
}

/*** is_num ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_num =
    dango::is_int<tp_type> || dango::is_float<tp_type>;
}

/*** is_array is_array_unknown_bound is_array_known_bound ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  inline constexpr auto const is_array_help = dango::uint(0);
  template
  <typename tp_type>
  inline constexpr auto const is_array_help<tp_type[]> = dango::uint(1);
  template
  <typename tp_type, dango::usize tp_size>
  inline constexpr auto const is_array_help<tp_type[tp_size]> = dango::uint(2);
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_array_unknown_bound =
    (dango::detail::is_array_help<tp_type> == dango::uint(1));

  template
  <typename tp_type>
  concept is_array_known_bound =
    (dango::detail::is_array_help<tp_type> == dango::uint(2));

  template
  <typename tp_type>
  concept is_array =
    dango::is_array_unknown_bound<tp_type> || dango::is_array_known_bound<tp_type>;
}

/*** array_rank ***/

namespace
dango
{
  template
  <typename tp_type>
  inline constexpr dango::usize const array_rank = dango::usize(0);

  template
  <typename tp_type>
  inline constexpr dango::usize const array_rank<tp_type[]> =
    dango::array_rank<tp_type> + dango::usize(1);

  template
  <typename tp_type, dango::usize tp_size>
  inline constexpr dango::usize const array_rank<tp_type[tp_size]> =
    dango::array_rank<tp_type> + dango::usize(1);
}

/*** array_size ***/

namespace
dango
{
  template
  <typename tp_type, dango::usize tp_dim = dango::usize(0)>
  inline constexpr dango::usize const array_size = dango::usize(0);

  template
  <typename tp_type>
  inline constexpr dango::usize const array_size<tp_type[], dango::usize(0)> = dango::usize(0);

  template
  <typename tp_type, dango::usize tp_dim>
  inline constexpr dango::usize const array_size<tp_type[], tp_dim> =
    dango::array_size<tp_type, tp_dim - dango::usize(1)>;

  template
  <typename tp_type, dango::usize tp_size>
  inline constexpr dango::usize const array_size<tp_type[tp_size], dango::usize(0)> = tp_size;

  template
  <typename tp_type, dango::usize tp_size, dango::usize tp_dim>
  inline constexpr dango::usize const array_size<tp_type[tp_size], tp_dim> =
    dango::array_size<tp_type, tp_dim - dango::usize(1)>;
}

/*** is_enum ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_enum = bool(__is_enum(tp_type));

  template
  <typename tp_type>
  concept is_enum_ignore_ref =
    dango::is_enum<tp_type> ||
    dango::is_enum<dango::remove_ref<tp_type>>;
}

/*** is_union ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_union = bool(__is_union(tp_type));

  template
  <typename tp_type>
  concept is_union_ignore_ref =
    dango::is_union<tp_type> ||
    dango::is_union<dango::remove_ref<tp_type>>;
}

/*** is_class ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_class = bool(__is_class(tp_type));

  template
  <typename tp_type>
  concept is_class_ignore_ref =
    dango::is_class<tp_type> ||
    dango::is_class<dango::remove_ref<tp_type>>;
}

/*** is_class_or_union ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_class_or_union =
    dango::is_class<tp_type> || dango::is_union<tp_type>;

  template
  <typename tp_type>
  concept is_class_or_union_ignore_ref =
    dango::is_class_or_union<tp_type> ||
    dango::is_class_or_union<dango::remove_ref<tp_type>>;
}

/*** is_user_defined ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_user_defined =
    dango::is_class_or_union<tp_type> || dango::is_enum<tp_type>;

  template
  <typename tp_type>
  concept is_user_defined_ignore_ref =
    dango::is_user_defined<tp_type> ||
    dango::is_user_defined<dango::remove_ref<tp_type>>;
}

/*** is_lvalue_ref is_rvalue_ref is_ref ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  inline constexpr auto const is_ref_help = dango::uint(0);

  template
  <typename tp_type>
  inline constexpr auto const is_ref_help<tp_type&> = dango::uint(1);

  template
  <typename tp_type>
  inline constexpr auto const is_ref_help<tp_type&&> = dango::uint(2);
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_lvalue_ref =
    (dango::detail::is_ref_help<tp_type> == dango::uint(1));

  template
  <typename tp_type>
  concept is_rvalue_ref =
    (dango::detail::is_ref_help<tp_type> == dango::uint(2));

  template
  <typename tp_type>
  concept is_ref =
    dango::is_lvalue_ref<tp_type> || dango::is_rvalue_ref<tp_type>;
}

/*** is_func ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_func =
    !dango::is_ref<tp_type> && dango::is_same<tp_type const volatile, dango::remove_cv<tp_type>>;
}

/*** is_unqualified_func ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  concept is_unqualified_func_help =
    requires{ { dango::declval<tp_type&>() }noexcept; };
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_unqualified_func =
    dango::is_func<tp_type> && dango::detail::is_unqualified_func_help<tp_type>;
}

/*** is_qualified_func ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_qualified_func =
    dango::is_func<tp_type> && !dango::detail::is_unqualified_func_help<tp_type>;
}

/*** is_ptr ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  inline constexpr bool const is_ptr_help = false;

  template
  <typename tp_type>
  inline constexpr bool const is_ptr_help<tp_type*> = true;
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_ptr = dango::detail::is_ptr_help<dango::remove_cv<tp_type>>;
}

/*** is_func_ptr ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_func_ptr =
    dango::is_ptr<tp_type> && dango::is_func<dango::remove_ptr<tp_type>>;
}

/*** is_void_ptr ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_void_ptr =
    dango::is_ptr<tp_type> && dango::is_void<dango::remove_ptr<tp_type>>;
}

/*** is_object_ptr ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_object_ptr =
    dango::is_ptr<tp_type> && !dango::is_func_ptr<tp_type> && !dango::is_void_ptr<tp_type>;
}

/*** is_member_ptr ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  inline constexpr bool const is_member_ptr_help = false;

  template
  <typename tp_type, typename tp_class>
  inline constexpr bool const is_member_ptr_help<tp_type tp_class::*> = true;
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_member_ptr = dango::detail::is_member_ptr_help<dango::remove_cv<tp_type>>;
}

/*** is_member_func_ptr ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  inline constexpr bool const is_member_func_ptr_help = false;

  template
  <typename tp_type, typename tp_class>
  inline constexpr bool const is_member_func_ptr_help<tp_type tp_class::*> = dango::is_func<tp_type>;
}

namespace dango
{
  template
  <typename tp_type>
  concept is_member_func_ptr =
    dango::is_member_ptr<tp_type> && dango::detail::is_member_func_ptr_help<dango::remove_cv<tp_type>>;
}

/*** is_member_object_ptr ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_member_object_ptr =
    dango::is_member_ptr<tp_type> && !dango::is_member_func_ptr<tp_type>;
}

/*** is_arithmetic_exclude_bool ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_arithmetic_exclude_bool =
    dango::is_integral_exclude_bool<tp_type> || dango::is_float<tp_type>;
}

/*** is_arithmetic ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_arithmetic =
    dango::is_arithmetic_exclude_bool<tp_type> || dango::is_bool<tp_type>;
}

/*** is_arithmetic_include_ptr ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_arithmetic_include_ptr =
    dango::is_arithmetic<tp_type> || dango::is_object_ptr<tp_type>;
}

/*** is_fundamnetal is_compound ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_fundamental =
    dango::is_void<tp_type> ||
    dango::is_null_tag<tp_type> ||
    dango::is_arithmetic<tp_type>;

  template
  <typename tp_type>
  concept is_compound = !dango::is_fundamental<tp_type>;
}

/*** is_scalar ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_scalar =
    dango::is_arithmetic<tp_type> ||
    dango::is_enum<tp_type> ||
    dango::is_ptr<tp_type> ||
    dango::is_member_ptr<tp_type> ||
    dango::is_null_tag<tp_type>;
}

/*** is_object_exclude_array ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_object_exclude_array =
    dango::is_scalar<tp_type> || dango::is_class_or_union<tp_type>;

  template
  <typename tp_type>
  concept is_object_exclude_array_ignore_ref =
    dango::is_object_exclude_array<tp_type> ||
    dango::is_object_exclude_array<dango::remove_ref<tp_type>>;
}

/*** is_object ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_object =
    dango::is_object_exclude_array<tp_type> || dango::is_array<tp_type>;

  template
  <typename tp_type>
  concept is_object_ignore_ref =
    dango::is_object<tp_type> ||
    dango::is_object<dango::remove_ref<tp_type>>;
}

/*** is_referenceable ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_referenceable =
    dango::is_object<tp_type> || dango::is_unqualified_func<tp_type>;

  template
  <typename tp_type>
  concept is_referenceable_ignore_ref =
    dango::is_ref<tp_type> || dango::is_referenceable<tp_type>;
}

/*** is_pointable ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_pointable =
    dango::is_void<tp_type> || dango::is_object<tp_type> || dango::is_unqualified_func<tp_type>;

  template
  <typename tp_type>
  concept is_pointable_ignore_ref =
    dango::is_pointable<tp_type> ||
    dango::is_pointable<dango::remove_ref<tp_type>>;
}

/*** is_const ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  inline constexpr bool const is_const_help = false;

  template
  <typename tp_type>
  inline constexpr bool const is_const_help<tp_type const> = true;
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_const = dango::detail::is_const_help<tp_type>;
}

/*** is_volatile ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  inline constexpr bool const is_volatile_help = false;

  template
  <typename tp_type>
  inline constexpr bool const is_volatile_help<tp_type volatile> = true;
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_volatile = dango::detail::is_volatile_help<tp_type>;
}

/*** is_const_or_volatile is_const_and_volatile ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_const_or_volatile =
    dango::is_const<tp_type> || dango::is_volatile<tp_type>;

  template
  <typename tp_type>
  concept is_const_and_volatile =
    dango::is_const<tp_type> && dango::is_volatile<tp_type>;
}

/*** is_trivial ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_trivial = bool(__is_trivial(tp_type));
}

/*** is_trivial_copyable ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_trivial_copyable = bool(__is_trivially_copyable(tp_type));
}

/*** is_standard_layout ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_standard_layout = bool(__is_standard_layout(tp_type));
}

/*** is_empty ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_empty = bool(__is_empty(tp_type));
}

/*** is_polymorphic ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_polymorphic = bool(__is_polymorphic(tp_type));
}

/*** is_abstract ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_abstract = bool(__is_abstract(tp_type));
}

/*** is_signed ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  inline constexpr bool const is_signed_help = false;

  template
  <dango::is_arithmetic_exclude_bool tp_type>
  inline constexpr bool const is_signed_help<tp_type> = bool(tp_type(-1) < tp_type(0));
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_signed = dango::detail::is_signed_help<dango::remove_cv<tp_type>>;
}

/*** is_unsigned ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_unsigned =
    dango::is_arithmetic<tp_type> && !dango::is_signed<tp_type>;
}

/*** is_base_of ***/

namespace
dango
{
  template
  <typename tp_base, typename tp_derived>
  concept is_base_of = bool(__is_base_of(tp_base, tp_derived));
}

/*** is_derived_from ***/

namespace
dango
{
  template
  <typename tp_derived, typename tp_base>
  concept is_derived_from = dango::is_base_of<tp_base, tp_derived>;
}

/*** sizeof_with_default alignof_with_default ***/

namespace
dango
{
  template
  <typename tp_type, dango::usize tp_default>
  inline constexpr dango::usize const sizeof_with_default = tp_default;
  template
  <dango::is_object_ignore_ref tp_type, dango::usize tp_default>
  inline constexpr dango::usize const sizeof_with_default<tp_type, tp_default> = sizeof(tp_type);

  template
  <typename tp_type, dango::usize tp_default>
  inline constexpr dango::usize const alignof_with_default = tp_default;
  template
  <dango::is_object_ignore_ref tp_type, dango::usize tp_default>
  inline constexpr dango::usize const alignof_with_default<tp_type, tp_default> = alignof(tp_type);
}

/*** add_lvalue_ref add is_rvalue_ref ***/

namespace
dango::detail
{
  template
  <typename tp_type, typename tp_default>
  struct
  add_lvalue_ref_help
  final
  {
    using type = tp_default;

    DANGO_UNCONSTRUCTIBLE(add_lvalue_ref_help)
  };

  template
  <dango::is_referenceable_ignore_ref tp_type, typename tp_default>
  struct
  add_lvalue_ref_help<tp_type, tp_default>
  final
  {
    using type = tp_type&;

    DANGO_UNCONSTRUCTIBLE(add_lvalue_ref_help)
  };

  template
  <typename tp_type, typename tp_default>
  struct
  add_rvalue_ref_help
  final
  {
    using type = tp_default;

    DANGO_UNCONSTRUCTIBLE(add_rvalue_ref_help)
  };

  template
  <dango::is_referenceable tp_type, typename tp_default>
  struct
  add_rvalue_ref_help<tp_type, tp_default>
  final
  {
    using type = tp_type&&;

    DANGO_UNCONSTRUCTIBLE(add_rvalue_ref_help)
  };
}

namespace
dango
{
  template
  <typename tp_type, typename tp_default = tp_type>
  using add_lvalue_ref =
    typename dango::detail::add_lvalue_ref_help<tp_type, tp_default>::type;

  template
  <typename tp_type, typename tp_default = tp_type>
  using add_rvalue_ref =
    typename dango::detail::add_rvalue_ref_help<tp_type, tp_default>::type;
}

/*** add_pointer ***/

namespace
dango::detail
{
  template
  <typename tp_type, typename tp_defualt>
  struct
  add_ptr_help
  final
  {
    using type = tp_defualt;

    DANGO_UNCONSTRUCTIBLE(add_ptr_help)
  };

  template
  <dango::is_pointable_ignore_ref tp_type, typename tp_default>
  struct
  add_ptr_help<tp_type, tp_default>
  final
  {
    using type = dango::remove_ref<tp_type>*;

    DANGO_UNCONSTRUCTIBLE(add_ptr_help)
  };
}

namespace
dango
{
  template
  <typename tp_type, typename tp_default = tp_type>
  using add_ptr =
    typename dango::detail::add_ptr_help<tp_type, tp_default>::type;
}

/*** is_destructible is_trivial_destructible is_noexcept_destructible has_virtual_destructor ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  inline constexpr bool const is_destructible_help = requires{ dango::declval<tp_type&>().~tp_type(); };
  template
  <dango::is_ref tp_type>
  inline constexpr bool const is_destructible_help<tp_type> = true;
  template
  <dango::is_array_unknown_bound tp_type>
  inline constexpr bool const is_destructible_help<tp_type> = false;
  template
  <dango::is_array_known_bound tp_type>
  inline constexpr bool const is_destructible_help<tp_type> =
    dango::detail::is_destructible_help<dango::remove_all_array<tp_type>>;

  template
  <typename tp_type>
  inline constexpr bool const is_noexcept_destructible_help = requires{ { dango::declval<tp_type&>().~tp_type() }noexcept; };
  template
  <dango::is_ref tp_type>
  inline constexpr bool const is_noexcept_destructible_help<tp_type> = true;
  template
  <dango::is_array_unknown_bound tp_type>
  inline constexpr bool const is_noexcept_destructible_help<tp_type> = false;
  template
  <dango::is_array_known_bound tp_type>
  inline constexpr bool const is_noexcept_destructible_help<tp_type> =
    dango::detail::is_noexcept_destructible_help<dango::remove_all_array<tp_type>>;

  template
  <typename tp_type>
  inline constexpr bool const is_trivial_destructible_help = bool(__has_trivial_destructor(tp_type));
  template
  <dango::is_ref tp_type>
  inline constexpr bool const is_trivial_destructible_help<tp_type> = true;
  template
  <dango::is_array_unknown_bound tp_type>
  inline constexpr bool const is_trivial_destructible_help<tp_type> = false;
  template
  <dango::is_array_known_bound tp_type>
  inline constexpr bool const is_trivial_destructible_help<tp_type> =
    dango::detail::is_trivial_destructible_help<dango::remove_all_array<tp_type>>;
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_destructible =
    dango::is_object_ignore_ref<tp_type> && dango::detail::is_destructible_help<dango::remove_cv<tp_type>>;

  template
  <typename tp_type>
  concept is_noexcept_destructible =
    dango::is_destructible<tp_type> && dango::detail::is_noexcept_destructible_help<dango::remove_cv<tp_type>>;

  template
  <typename tp_type>
  concept is_trivial_destructible =
    dango::is_noexcept_destructible<tp_type> && dango::detail::is_trivial_destructible_help<dango::remove_cv<tp_type>>;

  template
  <typename tp_type>
  concept is_virtual_destructible =
    dango::is_class<tp_type> && dango::is_destructible<tp_type> && bool(__has_virtual_destructor(tp_type));
}

/*** is_convertible is_noexcept_convertible is_convertible_ret is_noexcept_convertible_ret ***/

namespace
dango::detail
{
  template
  <typename tp_arg>
  constexpr void is_convertible_test(tp_arg)noexcept;

  template
  <typename tp_fr, typename tp_to>
  concept is_convertible_arg_help =
    (dango::is_ref<tp_fr> || dango::is_referenceable<tp_fr>) &&
    (dango::is_ref<tp_to> || dango::is_object_exclude_array<tp_to>) &&
    dango::is_destructible<tp_to> &&
    requires{ { dango::detail::is_convertible_test<tp_to>(dango::declval<tp_fr>()) }; };

  template
  <typename tp_fr, typename tp_to>
  concept is_noexcept_convertible_arg_help =
    dango::is_noexcept_destructible<tp_to> &&
    requires{ { dango::detail::is_convertible_test<tp_to>(dango::declval<tp_fr>()) }noexcept; };

  template
  <typename tp_fr, typename tp_to>
  concept is_convertible_ret_help =
    dango::is_same_ignore_cv<tp_fr, tp_to> &&
    (dango::is_void<tp_fr> || dango::is_object_exclude_array<tp_fr>) &&
    (dango::is_void<tp_to> || dango::is_destructible<tp_to>);

  template
  <typename tp_fr, typename tp_to>
  concept is_noexcept_convertible_ret_help =
    (dango::is_void<tp_to> || dango::is_noexcept_destructible<tp_to>);
}

namespace
dango
{
  template
  <typename tp_fr, typename tp_to>
  concept is_convertible_arg =
    dango::detail::is_convertible_arg_help<tp_fr, tp_to>;

  template
  <typename tp_fr, typename tp_to>
  concept is_noexcept_convertible_arg =
    dango::is_convertible_arg<tp_fr, tp_to> &&
    dango::detail::is_noexcept_convertible_arg_help<tp_fr, tp_to>;

  template
  <typename tp_fr, typename tp_to>
  concept is_convertible_ret =
    dango::detail::is_convertible_ret_help<tp_fr, tp_to> || dango::is_convertible_arg<tp_fr, tp_to>;

  template
  <typename tp_fr, typename tp_to>
  concept is_noexcept_convertible_ret =
    dango::is_convertible_ret<tp_fr, tp_to> &&
    (dango::detail::is_noexcept_convertible_ret_help<tp_fr, tp_to> || dango::is_noexcept_convertible_arg<tp_fr, tp_to>);
}

/*** is_callable is_noexcept_callable is_callable_ret is_noexcept_callable_ret ***/

namespace
dango
{
  template
  <typename tp_func, typename... tp_args>
  concept is_callable =
    dango::is_referenceable<dango::remove_ref<tp_func>> &&
    requires{ dango::declval<tp_func>()(dango::declval<tp_args>()...); };

  template
  <typename tp_func, typename... tp_args>
  concept is_noexcept_callable =
    dango::is_callable<tp_func, tp_args...> &&
    requires{ { dango::declval<tp_func>()(dango::declval<tp_args>()...) }noexcept; };

  template
  <typename tp_ret, typename tp_func, typename... tp_args>
  concept is_callable_ret =
    dango::is_callable<tp_func, tp_args...> &&
    requires{ { dango::declval<tp_func>()(dango::declval<tp_args>()...) }->dango::is_convertible_ret<tp_ret>; };

  template
  <typename tp_ret, typename tp_func, typename... tp_args>
  concept is_noexcept_callable_ret =
    dango::is_callable_ret<tp_ret, tp_func, tp_args...> &&
    dango::is_noexcept_callable<tp_func, tp_args...> &&
    requires{ { dango::declval<tp_func>()(dango::declval<tp_args>()...) }noexcept->dango::is_noexcept_convertible_ret<tp_ret>; };
}

/*** is_constructible is_trivial_constructible is_noexcept_constructible ***/

namespace
dango::detail
{
  template
  <typename tp_type, typename... tp_args>
  concept is_constructible_help = bool(__is_constructible(tp_type, tp_args...));

  template
  <typename tp_type, typename... tp_args>
  inline constexpr bool const is_noexcept_constructible_help = requires{ { tp_type(dango::declval<tp_args>()...) }noexcept; };
  template
  <dango::is_ref tp_type, typename... tp_args>
  inline constexpr bool const is_noexcept_constructible_help<tp_type, tp_args...> = true;

  template
  <typename tp_type, typename... tp_args>
  concept is_trivial_constructible_help = bool(__is_trivially_constructible(tp_type, tp_args...));
}

namespace
dango
{
  template
  <typename tp_type, typename... tp_args>
  concept is_constructible =
    dango::is_destructible<tp_type> && dango::detail::is_constructible_help<tp_type, tp_args...>;

  template
  <typename tp_type, typename... tp_args>
  concept is_noexcept_constructible =
    dango::is_noexcept_destructible<tp_type> &&
    dango::is_constructible<tp_type, tp_args...> &&
    dango::detail::is_noexcept_constructible_help<tp_type, tp_args...>;

  template
  <typename tp_type, typename... tp_args>
  concept is_trivial_constructible =
    dango::is_trivial_destructible<tp_type> &&
    dango::is_noexcept_constructible<tp_type, tp_args...> &&
    dango::detail::is_trivial_constructible_help<tp_type, tp_args...>;
}

/*** is_brace_constructible is_noexcept_brace_constructible is_trivial_brace_constructible ***/

namespace
dango::detail
{
  template
  <typename tp_type, typename... tp_args>
  inline constexpr bool const is_brace_constructible_help = requires{ { tp_type{ dango::declval<tp_args>()... } }; };
  template
  <dango::is_ref tp_type, typename... tp_args>
  inline constexpr bool const is_brace_constructible_help<tp_type, tp_args...> = true;

  template
  <typename tp_type, typename... tp_args>
  inline constexpr bool const is_noexcept_brace_constructible_help = requires{ { tp_type{ dango::declval<tp_args>()... } }noexcept; };
  template
  <dango::is_ref tp_type, typename... tp_args>
  inline constexpr bool const is_noexcept_brace_constructible_help<tp_type, tp_args...> = true;
}

namespace
dango
{
  template
  <typename tp_type, typename... tp_args>
  concept is_brace_constructible =
    dango::is_constructible<tp_type, tp_args...> &&
    dango::detail::is_brace_constructible_help<tp_type, tp_args...>;

  template
  <typename tp_type, typename... tp_args>
  concept is_noexcept_brace_constructible =
    dango::is_noexcept_constructible<tp_type, tp_args...> &&
    dango::is_brace_constructible<tp_type, tp_args...> &&
    dango::detail::is_noexcept_brace_constructible_help<tp_type, tp_args...>;

  template
  <typename tp_type, typename... tp_args>
  concept is_trivial_brace_constructible =
    dango::is_trivial_constructible<tp_type, tp_args...> &&
    dango::is_noexcept_brace_constructible<tp_type, tp_args...>;
}

/*** is_default_constructible is_trivial_default_constructible is_noexcept_default_constructible ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_default_constructible =
    dango::is_object<tp_type> && dango::is_brace_constructible<tp_type>;

  template
  <typename tp_type>
  concept is_noexcept_default_constructible =
    dango::is_default_constructible<tp_type> && dango::is_noexcept_brace_constructible<tp_type>;

  template
  <typename tp_type>
  concept is_trivial_default_constructible =
    dango::is_noexcept_default_constructible<tp_type> && dango::is_trivial_brace_constructible<tp_type>;
}

/*** is_copy_constructible is_trivial_copy_constructible is_noexcept_copy_constructible ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_copy_constructible =
    dango::is_ref<tp_type> || dango::is_brace_constructible<tp_type, tp_type const&>;

  template
  <typename tp_type>
  concept is_noexcept_copy_constructible =
    dango::is_copy_constructible<tp_type> &&
    (dango::is_ref<tp_type> || dango::is_noexcept_brace_constructible<tp_type, tp_type const&>);

  template
  <typename tp_type>
  concept is_trivial_copy_constructible =
    dango::is_noexcept_copy_constructible<tp_type> &&
    (dango::is_ref<tp_type> || dango::is_trivial_brace_constructible<tp_type, tp_type const&>);
}

/*** is_move_constructible is_trivial_move_constructible is_noexcept_move_constructible ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_move_constructible =
    dango::is_ref<tp_type> || dango::is_brace_constructible<tp_type, tp_type&&>;

  template
  <typename tp_type>
  concept is_noexcept_move_constructible =
    dango::is_move_constructible<tp_type> &&
    (dango::is_ref<tp_type> || dango::is_noexcept_brace_constructible<tp_type, tp_type&&>);

  template
  <typename tp_type>
  concept is_trivial_move_constructible =
    dango::is_noexcept_move_constructible<tp_type> &&
    (dango::is_ref<tp_type> || dango::is_trivial_brace_constructible<tp_type, tp_type&&>);
}

/*** is_noexcept_or_copy_constructible ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_noexcept_or_copy_constructible =
    dango::is_noexcept_brace_constructible<dango::decay<tp_type>, tp_type> ||
    dango::is_brace_constructible<dango::decay<tp_type>, dango::remove_ref<tp_type> const&>;
}

/*** is_assignable is_trivial_assignable is_noexcept_assignable ***/

namespace
dango
{
  template
  <typename tp_type, typename tp_arg>
  concept is_assignable = bool(__is_assignable(tp_type, tp_arg));

  template
  <typename tp_type, typename tp_arg>
  concept is_noexcept_assignable =
    dango::is_assignable<tp_type, tp_arg> && requires{ { dango::declval<tp_type>() = dango::declval<tp_arg>() }noexcept; };

  template
  <typename tp_type, typename tp_arg>
  concept is_trivial_assignable =
    dango::is_noexcept_assignable<tp_type, tp_arg> && bool(__is_trivially_assignable(tp_type, tp_arg));
}

/*** is_copy_assignable is_trivial_copy_assignable is_noexcept_copy_assignable ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_copy_assignable =
    dango::is_object_exclude_array<tp_type> && dango::is_assignable<tp_type&, tp_type const&>;

  template
  <typename tp_type>
  concept is_noexcept_copy_assignable =
    dango::is_copy_assignable<tp_type> && dango::is_noexcept_assignable<tp_type&, tp_type const&>;

  template
  <typename tp_type>
  concept is_trivial_copy_assignable =
    dango::is_noexcept_copy_assignable<tp_type> && dango::is_trivial_assignable<tp_type&, tp_type const&>;
}

/*** is_move_assignable is_trivial_move_assignable is_noexcept_move_assignable ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_move_assignable =
    dango::is_object_exclude_array<tp_type> && dango::is_assignable<tp_type&, tp_type&&>;

  template
  <typename tp_type>
  concept is_noexcept_move_assignable =
    dango::is_move_assignable<tp_type> && dango::is_noexcept_assignable<tp_type&, tp_type&&>;

  template
  <typename tp_type>
  concept is_trivial_move_assignable =
    dango::is_noexcept_move_assignable<tp_type> && dango::is_trivial_assignable<tp_type&, tp_type&&>;
}

/*** in_exception_safe_order ***/

namespace
dango::detail
{
  template
  <typename... tp_args>
  constexpr auto
  in_exception_safe_order_help()noexcept->bool;
}

template
<typename... tp_args>
constexpr auto
dango::
detail::
in_exception_safe_order_help()noexcept->bool
{
  constexpr auto const c_size = sizeof...(tp_args) + dango::usize(1);

  constexpr bool const c_throwing_construct[c_size] =
    { true, (!dango::is_noexcept_brace_constructible<dango::decay<tp_args>, tp_args>)... };

  constexpr bool const c_non_trivial_move_construct[c_size] =
    { false, (!dango::is_lvalue_ref<tp_args> && !dango::is_trivial_brace_constructible<dango::decay<tp_args>, tp_args>)... };

  auto a_throwing_construct_allowed = true;

  for(auto a_i = dango::usize(0); a_i != c_size; ++a_i)
  {
    auto const a_throwing_construct = c_throwing_construct[a_i];
    auto const a_non_trivial_move_construct = c_non_trivial_move_construct[a_i];

    if(a_throwing_construct && a_non_trivial_move_construct)
    {
      dango::constexpr_unreachable();
    }

    if(a_throwing_construct)
    {
      if(a_throwing_construct_allowed)
      {
        continue;
      }

      return false;
    }

    if(a_non_trivial_move_construct)
    {
      a_throwing_construct_allowed = false;
    }
  }

  return true;
}

namespace
dango
{
  template
  <typename... tp_args>
  concept in_exception_safe_order =
    ( ... && dango::is_noexcept_or_copy_constructible<tp_args>) &&
    dango::detail::in_exception_safe_order_help
    <dango::conditional<dango::is_noexcept_brace_constructible<dango::decay<tp_args>, tp_args>, tp_args, dango::remove_ref<tp_args> const&>...>();
}

/*** underlying_type ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct underlying_type_help;

  template
  <dango::is_enum tp_type>
  struct underlying_type_help<tp_type>;
}

namespace
dango
{
  template
  <typename tp_enum>
  using underlying_type =
    typename dango::detail::underlying_type_help<tp_enum>::type;
}

template
<typename tp_type>
struct
dango::
detail::
underlying_type_help
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(underlying_type_help)
};

template
<dango::is_enum tp_type>
struct
dango::
detail::
underlying_type_help<tp_type>
final
{
  using type = __underlying_type(tp_type);

  DANGO_UNCONSTRUCTIBLE(underlying_type_help)
};

/*** make_uint ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct
  make_uint_help
  final
  {
    using type = tp_type;

    DANGO_UNCONSTRUCTIBLE(make_uint_help)
  };

#define DANGO_DEFINE_INT_MAPPING(type1, type2) \
  template<> struct make_uint_help<type1> \
  final{ using type = type2; DANGO_UNCONSTRUCTIBLE(make_uint_help) };

  DANGO_DEFINE_INT_MAPPING(dango::integer::s_char,     dango::integer::u_char)
  DANGO_DEFINE_INT_MAPPING(dango::integer::s_short,    dango::integer::u_short)
  DANGO_DEFINE_INT_MAPPING(dango::integer::s_int,      dango::integer::u_int)
  DANGO_DEFINE_INT_MAPPING(dango::integer::s_long,     dango::integer::u_long)
  DANGO_DEFINE_INT_MAPPING(dango::integer::s_longlong, dango::integer::u_longlong)

  DANGO_DEFINE_INT_MAPPING(char,         dango::integer::u_char)
  DANGO_DEFINE_INT_MAPPING(dango::bchar, dango::ubyte)
  DANGO_DEFINE_INT_MAPPING(dango::wchar, dango::ushort)
  DANGO_DEFINE_INT_MAPPING(dango::dchar, dango::uint)

#ifdef DANGO_PLATFORM_WINDOWS
  DANGO_DEFINE_INT_MAPPING(wchar_t,      dango::ushort)
#else
  DANGO_DEFINE_INT_MAPPING(wchar_t,      dango::uint)
#endif

#undef DANGO_DEFINE_INT_MAPPING
}

namespace
dango
{
  template
  <typename tp_type>
  using make_uint =
    dango::copy_cv<tp_type, typename dango::detail::make_uint_help<dango::underlying_type<dango::remove_cv<tp_type>>>::type>;
}

/*** make_sint ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct
  make_sint_help
  final
  {
    using type = tp_type;

    DANGO_UNCONSTRUCTIBLE(make_sint_help)
  };

#define DANGO_DEFINE_INT_MAPPING(type1, type2) \
  template<> struct make_sint_help<type1> \
  final{ using type = type2; DANGO_UNCONSTRUCTIBLE(make_sint_help) };

  DANGO_DEFINE_INT_MAPPING(dango::integer::u_char,     dango::integer::s_char)
  DANGO_DEFINE_INT_MAPPING(dango::integer::u_short,    dango::integer::s_short)
  DANGO_DEFINE_INT_MAPPING(dango::integer::u_int,      dango::integer::s_int)
  DANGO_DEFINE_INT_MAPPING(dango::integer::u_long,     dango::integer::s_long)
  DANGO_DEFINE_INT_MAPPING(dango::integer::u_longlong, dango::integer::s_longlong)

  DANGO_DEFINE_INT_MAPPING(char,         dango::integer::s_char)
  DANGO_DEFINE_INT_MAPPING(dango::bchar, dango::sbyte)
  DANGO_DEFINE_INT_MAPPING(dango::wchar, dango::sshort)
  DANGO_DEFINE_INT_MAPPING(dango::dchar, dango::sint)

#ifdef DANGO_PLATFORM_WINDOWS
  DANGO_DEFINE_INT_MAPPING(wchar_t,      dango::sshort)
#else
  DANGO_DEFINE_INT_MAPPING(wchar_t,      dango::sint)
#endif

#undef DANGO_DEFINE_INT_MAPPING
}

namespace
dango
{
  template
  <typename tp_type>
  using make_sint =
    dango::copy_cv<tp_type, typename dango::detail::make_sint_help<dango::underlying_type<dango::remove_cv<tp_type>>>::type>;
}

/*** endian ***/

namespace
dango
{
  enum class
  endian:
  dango::uint
  {
      little = dango::uint(__ORDER_LITTLE_ENDIAN__),
      big = dango::uint(__ORDER_BIG_ENDIAN__),
      native = dango::uint(__BYTE_ORDER__)
  };
}

/*** bit_width ***/

namespace
dango::detail
{
  template
  <typename tp_int>
  constexpr auto
  bit_width_help()noexcept->dango::usize
  {
    using tp_uint = dango::make_uint<tp_int>;

    auto a_count = dango::usize(0);

    for(auto a_uint = tp_uint(-1); a_uint != tp_uint(0); a_uint >>= tp_uint(1))
    {
      ++a_count;
    }

    return a_count;
  }
}

namespace
dango
{
  template
  <dango::is_integral_exclude_bool tp_int>
  inline constexpr auto const bit_width =
    dango::detail::bit_width_help<dango::remove_cv<tp_int>>();
}

/*** constexpr_check ***/

namespace
dango
{
  template
  <typename tp_type, tp_type tp_expr>
  struct
  constexpr_check
  final
  {
    DANGO_TAG_TYPE(constexpr_check)
  };
}

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

/*** present_if ***/

namespace
dango
{
  template
  <dango::uint>
  struct
  present_if_type
  final
  {
    DANGO_TAG_TYPE(present_if_type)
  };

  template
  <bool tp_cond, typename tp_type, dango::uint tp_id = dango::uint(0)>
  using present_if =
    dango::conditional<tp_cond, tp_type, dango::present_if_type<tp_id>>;
}

// workaround for GCC bug 81043

#ifdef DANGO_USING_GCC
#define DANGO_GCC_BUG_81043_WORKAROUND \
  , dango::is_same<void> = void
#define DANGO_GCC_BUG_81043_WORKAROUND_ND \
  , dango::is_same<void>
#define DANGO_GCC_BUG_81043_WORKAROUND_ID(idno, ...) \
  , dango::detail::spec_id<dango::uint(idno), __VA_ARGS__>
namespace
dango::detail
{
  template
  <dango::uint, typename, typename...>
  using spec_id = void;
}
#else
#define DANGO_GCC_BUG_81043_WORKAROUND
#define DANGO_GCC_BUG_81043_WORKAROUND_ND
#define DANGO_GCC_BUG_81043_WORKAROUND_ID(idno, ...)
#endif

#endif // DANGO_TRAITS_HPP_INCLUDED

