#ifndef DANGO_TRAITS_HPP_INCLUDED
#define DANGO_TRAITS_HPP_INCLUDED

#include "dango-def.hpp"

#define DANGO_TRAITS_DEFINE_IGNORE_REF(name) \
  template<typename tp_type> \
  concept name##_ignore_ref = dango::name<tp_type> || dango::name<dango::remove_ref<tp_type>>;

/*** declval ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr auto declval()noexcept->tp_type&&;
}

/*** priority_tag ***/

namespace
dango
{
  template
  <dango::uint tp_prio>
  struct priority_tag;

  template<>
  struct priority_tag<dango::uint(0)>;
}

template
<dango::uint tp_prio>
struct
dango::
priority_tag:
dango::priority_tag<tp_prio - dango::uint(1)>
{
  DANGO_TAG_TYPE(priority_tag)
};

template<>
struct
dango::
priority_tag<dango::uint(0)>
{
  DANGO_TAG_TYPE(priority_tag)
};

/*** type_tag ***/

namespace
dango
{
  template
  <typename tp_type>
  struct
  type_tag
  final
  {
    using type = tp_type;

    DANGO_TAG_TYPE(type_tag)
  };

  template
  <typename tp_type>
  inline constexpr dango::type_tag<tp_type> const type_val{ };
}

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

/*** parenth_type ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct parenth_type_help;

  template
  <typename tp_ret, typename tp_type, bool tp_noexcept>
  struct parenth_type_help<tp_ret(tp_type)noexcept(tp_noexcept)>;
}

namespace
dango
{
  template
  <typename tp_type>
  using parenth_type =
    typename dango::detail::parenth_type_help<tp_type>::type;
}

template
<typename tp_ret, typename tp_type, bool tp_noexcept>
struct
dango::
detail::
parenth_type_help<tp_ret(tp_type)noexcept(tp_noexcept)>
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(parenth_type_help)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_null_tag)
}

/*** is_bool ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_bool = dango::is_same_ignore_cv<tp_type, bool>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_bool)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_uint)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_sint)
}

/*** is_int ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_int =
    dango::is_uint<tp_type> || dango::is_sint<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_int)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_char)
}

/*** is_integral_exclude_bool ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_integral_exclude_bool =
    dango::is_int<tp_type> || dango::is_char<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_integral_exclude_bool)
}

/* is_integral */

namespace
dango
{
  template
  <typename tp_type>
  concept is_integral =
    dango::is_integral_exclude_bool<tp_type> || dango::is_bool<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_integral)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_float)
}

/*** is_num ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_num =
    dango::is_int<tp_type> || dango::is_float<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_num)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_array_unknown_bound)
  DANGO_TRAITS_DEFINE_IGNORE_REF(is_array_known_bound)
  DANGO_TRAITS_DEFINE_IGNORE_REF(is_array)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_enum)
}

/*** is_union ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_union = bool(__is_union(tp_type));

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_union)
}

/*** is_class ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_class = bool(__is_class(tp_type));

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_class)
}

/*** is_class_or_union ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_class_or_union =
    dango::is_class<tp_type> || dango::is_union<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_class_or_union)
}

/*** is_user_defined ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_user_defined =
    dango::is_class_or_union<tp_type> || dango::is_enum<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_user_defined)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_func)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_unqualified_func)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_ptr)
}

/*** is_func_ptr ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_func_ptr =
    dango::is_ptr<tp_type> && dango::is_func<dango::remove_ptr<tp_type>>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_func_ptr)
}

/*** is_void_ptr ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_void_ptr =
    dango::is_ptr<tp_type> && dango::is_void<dango::remove_ptr<tp_type>>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_void_ptr)
}

/*** is_object_ptr ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_object_ptr =
    dango::is_ptr<tp_type> && !dango::is_func_ptr<tp_type> && !dango::is_void_ptr<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_object_ptr)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_member_ptr)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_member_func_ptr)
}

/*** is_member_object_ptr ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_member_object_ptr =
    dango::is_member_ptr<tp_type> && !dango::is_member_func_ptr<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_member_object_ptr)
}

/*** is_arithmetic_exclude_bool ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_arithmetic_exclude_bool =
    dango::is_integral_exclude_bool<tp_type> || dango::is_float<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_arithmetic_exclude_bool)
}

/*** is_arithmetic ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_arithmetic =
    dango::is_arithmetic_exclude_bool<tp_type> || dango::is_bool<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_arithmetic)
}

/*** is_arithmetic_include_ptr ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_arithmetic_include_ptr =
    dango::is_arithmetic<tp_type> || dango::is_object_ptr<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_arithmetic_include_ptr)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_fundamental)

  template
  <typename tp_type>
  concept is_compound = !dango::is_fundamental<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_compound)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_scalar)
}

/*** is_object_exclude_array ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_object_exclude_array =
    dango::is_scalar<tp_type> || dango::is_class_or_union<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_object_exclude_array)
}

/*** is_object ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_object =
    dango::is_object_exclude_array<tp_type> || dango::is_array<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_object)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_pointable)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_const)
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

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_volatile)
}

/*** is_const_or_volatile is_const_and_volatile ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_const_or_volatile =
    dango::is_const<tp_type> || dango::is_volatile<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_const_or_volatile)

  template
  <typename tp_type>
  concept is_const_and_volatile =
    dango::is_const<tp_type> && dango::is_volatile<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_const_and_volatile)
}

/*** is_trivial_copyable ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_trivial_copyable =
    dango::is_object<tp_type> && bool(__is_trivially_copyable(tp_type));

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_trivial_copyable)
}

/*** is_standard_layout ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_standard_layout =
    dango::is_object<tp_type> && bool(__is_standard_layout(tp_type));

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_standard_layout)
}

/*** is_empty ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_empty =
    dango::is_class<tp_type> && bool(__is_empty(tp_type));

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_empty)
}

/*** is_polymorphic ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_polymorphic =
    dango::is_class<tp_type> && bool(__is_polymorphic(tp_type));

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_polymorphic)
}

/*** is_abstract ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_abstract =
    dango::is_polymorphic<tp_type> && bool(__is_abstract(tp_type));

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_abstract)
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
  concept is_signed =
    dango::is_arithmetic<tp_type> && dango::detail::is_signed_help<dango::remove_cv<tp_type>>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_signed)
}

/*** is_unsigned ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_unsigned =
    dango::is_arithmetic<tp_type> && !dango::is_signed<tp_type>;

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_unsigned)
}

/*** is_base_of ***/

namespace
dango
{
  template
  <typename tp_base, typename tp_derived>
  concept is_base_of =
    dango::is_class<tp_base> &&
    dango::is_class<tp_derived> &&
    bool(__is_base_of(tp_base, tp_derived));

  template
  <typename tp_base, typename tp_derived>
  concept is_base_of_ignore_ref =
    dango::is_base_of<tp_base, tp_derived> ||
    dango::is_base_of<dango::remove_ref<tp_base>, dango::remove_ref<tp_derived>>;
}

/*** is_derived_from ***/

namespace
dango
{
  template
  <typename tp_derived, typename tp_base>
  concept is_derived_from = dango::is_base_of<tp_base, tp_derived>;

  template
  <typename tp_derived, typename tp_base>
  concept is_derived_from_ignore_ref =
    dango::is_base_of_ignore_ref<tp_base, tp_derived>;
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
  <dango::is_referenceable_ignore_ref tp_type, typename tp_default>
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

  template
  <typename tp_type>
  concept is_qualified_destructible =
    dango::is_object_exclude_array<tp_type> &&
    requires{ { dango::declval<tp_type&>().tp_type::~tp_type() }; };

  template
  <typename tp_type>
  concept is_noexcept_qualified_destructible =
    dango::is_qualified_destructible<tp_type> &&
    requires{ { dango::declval<tp_type&>().tp_type::~tp_type() }noexcept; };

  template
  <typename tp_type>
  concept is_trivial_qualified_destructible =
    dango::is_noexcept_qualified_destructible<tp_type> &&
    bool(__has_trivial_destructor(tp_type));
}

/*** is_convertible_arg is_noexcept_convertible_arg is_convertible_ret is_noexcept_convertible_ret ***/

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

/*** is_static_castable is_noexcept_static_castable ***/

namespace
dango
{
  template
  <typename tp_fr, typename tp_to>
  concept is_static_castable =
    dango::is_referenceable_ignore_ref<tp_fr> &&
    dango::is_object_ignore_ref<tp_to> &&
    requires{ { static_cast<tp_to>(dango::declval<tp_fr>()) }; };

  template
  <typename tp_fr, typename tp_to>
  concept is_noexcept_static_castable =
    dango::is_static_castable<tp_fr, tp_to> &&
    requires{ { static_cast<tp_to>(dango::declval<tp_fr>()) }noexcept; };
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

/*** is_parenth_constructible ***/

namespace
dango::detail
{
  template
  <typename tp_type, typename... tp_args>
  concept is_parenth_constructible_help = bool(__is_constructible(tp_type, tp_args...));

  template
  <typename tp_type, typename... tp_args>
  inline constexpr bool const is_noexcept_parenth_constructible_help = requires{ { tp_type(dango::declval<tp_args>()...) }noexcept; };
  template
  <dango::is_ref tp_type, typename... tp_args>
  inline constexpr bool const is_noexcept_parenth_constructible_help<tp_type, tp_args...> = true;

  template
  <typename tp_type, typename... tp_args>
  concept is_trivial_parenth_constructible_help = bool(__is_trivially_constructible(tp_type, tp_args...));
}

namespace
dango
{
  template
  <typename tp_type, typename... tp_args>
  concept is_parenth_constructible =
    dango::is_destructible<tp_type> && dango::detail::is_parenth_constructible_help<tp_type, tp_args...>;

  template
  <typename tp_type, typename... tp_args>
  concept is_noexcept_parenth_constructible =
    dango::is_noexcept_destructible<tp_type> &&
    dango::is_parenth_constructible<tp_type, tp_args...> &&
    dango::detail::is_noexcept_parenth_constructible_help<tp_type, tp_args...>;

  template
  <typename tp_type, typename... tp_args>
  concept is_trivial_parenth_constructible =
    dango::is_trivial_destructible<tp_type> &&
    dango::is_noexcept_parenth_constructible<tp_type, tp_args...> &&
    dango::detail::is_trivial_parenth_constructible_help<tp_type, tp_args...>;

  template
  <typename tp_type, typename... tp_args>
  concept is_parenth_constructible_and_noexcept_destructible =
    dango::is_parenth_constructible<tp_type, tp_args...> && dango::is_noexcept_destructible<tp_type>;
}

/*** is_brace_constructible ***/

namespace
dango::detail
{
  template
  <typename tp_type, typename... tp_args>
  inline constexpr bool const is_brace_constructible_help = requires{ { tp_type{ dango::declval<tp_args>()... } }; };
  template
  <dango::is_ref tp_type, typename... tp_args>
  inline constexpr bool const is_brace_constructible_help<tp_type, tp_args...> = true;
}

namespace
dango
{
  template
  <typename tp_type, typename... tp_args>
  concept is_brace_constructible =
    dango::is_parenth_constructible<tp_type, tp_args...> &&
    dango::detail::is_brace_constructible_help<tp_type, tp_args...>;

  template
  <typename tp_type, typename... tp_args>
  concept is_noexcept_brace_constructible =
    dango::is_noexcept_parenth_constructible<tp_type, tp_args...> &&
    dango::is_brace_constructible<tp_type, tp_args...>;

  template
  <typename tp_type, typename... tp_args>
  concept is_trivial_brace_constructible =
    dango::is_trivial_parenth_constructible<tp_type, tp_args...> &&
    dango::is_noexcept_brace_constructible<tp_type, tp_args...>;

  template
  <typename tp_type, typename... tp_args>
  concept is_brace_constructible_and_noexcept_destructible =
    dango::is_brace_constructible<tp_type, tp_args...> && dango::is_noexcept_destructible<tp_type>;
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

/*** is_trivial ***/

namespace
dango
{
#ifdef DANGO_USING_CLANG
  template
  <typename tp_type>
  concept is_trivial =
    dango::is_trivial_copyable<tp_type> &&
    dango::is_trivial_default_constructible<tp_type>;
    // bool(__is_trivial(tp_type)); <- seems to be bugged in clang 11. breaks for certain types that pass in clang 10 and GCC 10.
#else
  template
  <typename tp_type>
  concept is_trivial =
    dango::is_trivial_copyable<tp_type> &&
    dango::is_trivial_default_constructible<tp_type> &&
    bool(__is_trivial(tp_type));
#endif

  DANGO_TRAITS_DEFINE_IGNORE_REF(is_trivial)
}

/*** common_type ***/

namespace
dango::custom
{
  template
  <typename tp_type1, typename tp_type2>
  struct common_type;
}

namespace
dango::detail
{
  using common_type_prio = dango::priority_tag<dango::uint(3)>;

  template
  <typename tp_type1, typename tp_type2, typename tp_result = typename dango::custom::common_type<dango::decay<tp_type1>, dango::decay<tp_type2>>::type>
  requires
  (
    !dango::is_ref<tp_result> && !dango::is_const_or_volatile<tp_result> &&
    dango::is_brace_constructible<tp_result, tp_type1> &&
    dango::is_brace_constructible<tp_result, tp_type2>
  )
  constexpr auto
  common_type_test
  (dango::priority_tag<dango::uint(3)>)
  noexcept->dango::type_tag<tp_result>;

  template
  <typename tp_type1, typename tp_type2, typename tp_result = typename dango::custom::common_type<dango::decay<tp_type2>, dango::decay<tp_type1>>::type>
  requires
  (
    !dango::is_ref<tp_result> && !dango::is_const_or_volatile<tp_result> &&
    dango::is_brace_constructible<tp_result, tp_type1> &&
    dango::is_brace_constructible<tp_result, tp_type2>
  )
  constexpr auto
  common_type_test
  (dango::priority_tag<dango::uint(2)>)
  noexcept->dango::type_tag<tp_result>;

  template
  <typename tp_type1, typename tp_type2>
  constexpr auto
  common_type_test
  (dango::priority_tag<dango::uint(1)>)
  noexcept->dango::type_tag<dango::decay<decltype(false ? dango::declval<tp_type1>() : dango::declval<tp_type2>())>>;

  template
  <typename tp_type1, typename tp_type2>
  constexpr auto
  common_type_test
  (dango::priority_tag<dango::uint(0)>)
  noexcept->dango::type_tag<dango::decay<decltype(false ? dango::declval<dango::remove_ref<tp_type1> const&>() : dango::declval<dango::remove_ref<tp_type2> const&>())>>;
}

namespace
dango::detail
{
  template
  <typename... tp_types>
  struct common_type_help;

  template
  <typename... tp_types>
  using common_type_type =
    typename dango::detail::common_type_help<tp_types...>::type;
}

namespace
dango
{
  template
  <typename... tp_types>
  concept has_common_type =
    requires{ typename dango::detail::common_type_type<tp_types...>; };

  template
  <typename... tp_types>
  requires(dango::has_common_type<tp_types...>)
  using common_type =
    dango::detail::common_type_type<tp_types...>;
}

namespace
dango::detail
{
  template
  <typename... tp_types>
  struct
  common_type_help
  final
  {
    DANGO_UNCONSTRUCTIBLE(common_type_help)
  };

  template
  <typename tp_type>
  requires(requires{ typename dango::detail::common_type_type<tp_type, tp_type>; })
  struct
  common_type_help<tp_type>
  final
  {
    using type = dango::detail::common_type_type<tp_type, tp_type>;

    DANGO_UNCONSTRUCTIBLE(common_type_help)
  };

  template
  <typename tp_type1, typename tp_type2>
  requires(requires{ { dango::detail::common_type_test<tp_type1, tp_type2>(dango::detail::common_type_prio{ }) }; })
  struct
  common_type_help<tp_type1, tp_type2>
  final
  {
    using type =
      typename decltype(dango::detail::common_type_test<tp_type1, tp_type2>(dango::detail::common_type_prio{ }))::type;

    DANGO_UNCONSTRUCTIBLE(common_type_help)
  };

  template
  <typename tp_type1, typename tp_type2, typename... tp_next>
  requires
  (
    (sizeof...(tp_next) != dango::usize(0)) &&
    requires{ typename dango::detail::common_type_type<dango::detail::common_type_type<tp_type1, tp_type2>, tp_next...>; }
  )
  struct
  common_type_help<tp_type1, tp_type2, tp_next...>
  final
  {
    using type = dango::detail::common_type_type<dango::detail::common_type_type<tp_type1, tp_type2>, tp_next...>;

    DANGO_UNCONSTRUCTIBLE(common_type_help)
  };
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

#define DANGO_TRAITS_DEFINE_INT_MAPPING(type1, type2) \
  template<> struct make_uint_help<type1> \
  final{ using type = type2; DANGO_UNCONSTRUCTIBLE(make_uint_help) };

  DANGO_TRAITS_DEFINE_INT_MAPPING(dango::integer::s_char,     dango::integer::u_char)
  DANGO_TRAITS_DEFINE_INT_MAPPING(dango::integer::s_short,    dango::integer::u_short)
  DANGO_TRAITS_DEFINE_INT_MAPPING(dango::integer::s_int,      dango::integer::u_int)
  DANGO_TRAITS_DEFINE_INT_MAPPING(dango::integer::s_long,     dango::integer::u_long)
  DANGO_TRAITS_DEFINE_INT_MAPPING(dango::integer::s_longlong, dango::integer::u_longlong)

  DANGO_TRAITS_DEFINE_INT_MAPPING(char,         dango::integer::u_char)
  DANGO_TRAITS_DEFINE_INT_MAPPING(dango::bchar, dango::ubyte)
  DANGO_TRAITS_DEFINE_INT_MAPPING(dango::wchar, dango::ushort)
  DANGO_TRAITS_DEFINE_INT_MAPPING(dango::dchar, dango::uint)

#ifdef DANGO_PLATFORM_WINDOWS
  DANGO_TRAITS_DEFINE_INT_MAPPING(wchar_t,      dango::ushort)
#else
  DANGO_TRAITS_DEFINE_INT_MAPPING(wchar_t,      dango::uint)
#endif

#undef DANGO_TRAITS_DEFINE_INT_MAPPING
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

#define DANGO_TRAITS_DEFINE_INT_MAPPING(type1, type2) \
  template<> struct make_sint_help<type1> \
  final{ using type = type2; DANGO_UNCONSTRUCTIBLE(make_sint_help) };

  DANGO_TRAITS_DEFINE_INT_MAPPING(dango::integer::u_char,     dango::integer::s_char)
  DANGO_TRAITS_DEFINE_INT_MAPPING(dango::integer::u_short,    dango::integer::s_short)
  DANGO_TRAITS_DEFINE_INT_MAPPING(dango::integer::u_int,      dango::integer::s_int)
  DANGO_TRAITS_DEFINE_INT_MAPPING(dango::integer::u_long,     dango::integer::s_long)
  DANGO_TRAITS_DEFINE_INT_MAPPING(dango::integer::u_longlong, dango::integer::s_longlong)

  DANGO_TRAITS_DEFINE_INT_MAPPING(char,         dango::integer::s_char)
  DANGO_TRAITS_DEFINE_INT_MAPPING(dango::bchar, dango::sbyte)
  DANGO_TRAITS_DEFINE_INT_MAPPING(dango::wchar, dango::sshort)
  DANGO_TRAITS_DEFINE_INT_MAPPING(dango::dchar, dango::sint)

#ifdef DANGO_PLATFORM_WINDOWS
  DANGO_TRAITS_DEFINE_INT_MAPPING(wchar_t,      dango::sshort)
#else
  DANGO_TRAITS_DEFINE_INT_MAPPING(wchar_t,      dango::sint)
#endif

#undef DANGO_TRAITS_DEFINE_INT_MAPPING
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
  <dango::is_integral tp_int>
  inline constexpr auto const bit_width =
    dango::detail::bit_width_help<dango::remove_cv<tp_int>>();

  template
  <dango::is_bool tp_int>
  inline constexpr auto const bit_width<tp_int> = dango::usize(1);
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

/*** integer_seq ***/

namespace
dango
{
  template
  <typename tp_int, tp_int... tp_integers>
  struct
  integer_seq
  final
  {
    DANGO_TAG_TYPE(integer_seq)
  };

#ifdef DANGO_USING_GCC
  template
  <typename tp_int, tp_int tp_len>
  using make_integer_seq = dango::integer_seq<tp_int, __integer_pack(tp_len)...>;
#endif

#ifdef DANGO_USING_CLANG
  template
  <typename tp_int, tp_int tp_len>
  using make_integer_seq = __make_integer_seq<dango::integer_seq, tp_int, tp_len>;
#endif

  template
  <dango::usize... tp_indices>
  using index_seq = dango::integer_seq<dango::usize, tp_indices...>;

  template
  <dango::usize tp_len>
  using make_index_seq = dango::make_integer_seq<dango::usize, tp_len>;
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

/*** move forward ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr auto move(tp_type&&)noexcept = delete;

  template
  <typename tp_type>
  requires(dango::is_lvalue_ref<tp_type>)
  constexpr auto
  move
  (tp_type&& a_arg)noexcept->dango::remove_ref<tp_type>&&
  {
    return static_cast<dango::remove_ref<tp_type>&&>(a_arg);
  }

  template
  <typename tp_type>
  constexpr auto forward(dango::remove_ref<tp_type>&&)noexcept = delete;

  template
  <typename tp_type>
  constexpr auto
  forward
  (dango::remove_ref<tp_type>& a_arg)noexcept->tp_type&&
  {
    return static_cast<tp_type&&>(a_arg);
  }
}

/*** as_const ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr auto
  as_const
  (tp_type&& a_arg)noexcept->dango::remove_ref<tp_type> const&
  {
    return a_arg;
  }
}

/*** move_if_noexcept forward_if_noexcept ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr auto move_if_noexcept(tp_type&&)noexcept = delete;

  template
  <typename tp_type>
  requires(dango::is_lvalue_ref<tp_type> && dango::is_noexcept_or_copy_constructible<dango::remove_ref<tp_type>&&>)
  constexpr auto
  move_if_noexcept
  (tp_type&& a_arg)noexcept->decltype(auto)
  {
    if constexpr(dango::is_noexcept_brace_constructible<dango::decay<tp_type>, dango::remove_ref<tp_type>&&>)
    {
      return dango::move(a_arg);
    }
    else
    {
      return dango::as_const(a_arg);
    }
  }

  template
  <typename tp_type>
  constexpr auto
  forward_if_noexcept(dango::remove_ref<tp_type>&&)noexcept = delete;

  template
  <typename tp_type>
  requires(!dango::is_noexcept_or_copy_constructible<tp_type>)
  constexpr auto
  forward_if_noexcept(dango::remove_ref<tp_type>&)noexcept = delete;

  template
  <typename tp_type>
  requires(dango::is_noexcept_or_copy_constructible<tp_type>)
  constexpr auto
  forward_if_noexcept
  (dango::remove_ref<tp_type>& a_arg)noexcept->decltype(auto)
  {
    if constexpr(dango::is_noexcept_brace_constructible<dango::decay<tp_type>, tp_type>)
    {
      return dango::forward<tp_type>(a_arg);
    }
    else
    {
      return dango::as_const(a_arg);
    }
  }
}

/*** workaround for GCC bug 81043 ***/

#ifdef DANGO_USING_GCC
namespace
dango::detail
{
  template
  <dango::uint, typename, typename...>
  using spec_id = void;
}
#endif

#undef DANGO_TRAITS_DEFINE_IGNORE_REF

#endif // DANGO_TRAITS_HPP_INCLUDED
