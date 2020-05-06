#ifndef DANGO_TRAITS_HPP_INCLUDED
#define DANGO_TRAITS_HPP_INCLUDED

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
    typename detail::remove_const_help<tp_type>::type;
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

  DANGO_UNINSTANTIABLE(remove_const_help)
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

  DANGO_UNINSTANTIABLE(remove_const_help)
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
    typename detail::remove_volatile_help<tp_type>::type;
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

  DANGO_UNINSTANTIABLE(remove_volatile_help)
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

  DANGO_UNINSTANTIABLE(remove_volatile_help)
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
    typename detail::remove_ref_help<tp_type>::type;
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

  DANGO_UNINSTANTIABLE(remove_ref_help)
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

  DANGO_UNINSTANTIABLE(remove_ref_help)
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

  DANGO_UNINSTANTIABLE(remove_ref_help)
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
    typename detail::remove_ptr_help<tp_type>::type;
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

  DANGO_UNINSTANTIABLE(remove_ptr_help)
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

  DANGO_UNINSTANTIABLE(remove_ptr_help)
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

  DANGO_UNINSTANTIABLE(remove_ptr_help)
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

  DANGO_UNINSTANTIABLE(remove_ptr_help)
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

  DANGO_UNINSTANTIABLE(remove_ptr_help)
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
    typename detail::remove_all_ptr_help<tp_type>::type;
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

  DANGO_UNINSTANTIABLE(remove_all_ptr_help)
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

  DANGO_UNINSTANTIABLE(remove_all_ptr_help)
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

  DANGO_UNINSTANTIABLE(remove_all_ptr_help)
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

  DANGO_UNINSTANTIABLE(remove_all_ptr_help)
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

  DANGO_UNINSTANTIABLE(remove_all_ptr_help)
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
    typename detail::remove_array_help<tp_type>::type;
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

  DANGO_UNINSTANTIABLE(remove_array_help)
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

  DANGO_UNINSTANTIABLE(remove_array_help)
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

  DANGO_UNINSTANTIABLE(remove_array_help)
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
    typename detail::remove_all_array_help<tp_type>::type;
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

  DANGO_UNINSTANTIABLE(remove_all_array_help)
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

  DANGO_UNINSTANTIABLE(remove_all_array_help)
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

  DANGO_UNINSTANTIABLE(remove_all_array_help)
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
    typename detail::copy_cv_help<tp_from, tp_to>::type;
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

  DANGO_UNINSTANTIABLE(copy_cv_help)
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

  DANGO_UNINSTANTIABLE(copy_cv_help)
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

  DANGO_UNINSTANTIABLE(copy_cv_help)
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

  DANGO_UNINSTANTIABLE(copy_cv_help)
};

/*** make_uint ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct make_uint_help;

  template<>
  struct make_uint_help<dango::builtin::schar>;
  template<>
  struct make_uint_help<dango::builtin::sshort>;
  template<>
  struct make_uint_help<dango::builtin::sint>;
  template<>
  struct make_uint_help<dango::builtin::slong>;
  template<>
  struct make_uint_help<dango::builtin::slonglong>;
}

namespace
dango
{
  template
  <typename tp_type>
  using make_uint =
    dango::copy_cv<tp_type, typename detail::make_uint_help<dango::remove_cv<tp_type>>::type>;
}

template
<typename tp_type>
struct
dango::
detail::
make_uint_help
final
{
  using type = tp_type;

  DANGO_UNINSTANTIABLE(make_uint_help)
};

template<>
struct
dango::
detail::
make_uint_help<dango::builtin::schar>
final
{
  using type = dango::builtin::uchar;

  DANGO_UNINSTANTIABLE(make_uint_help)
};

template<>
struct
dango::
detail::
make_uint_help<dango::builtin::sshort>
final
{
  using type = dango::builtin::ushort;

  DANGO_UNINSTANTIABLE(make_uint_help)
};

template<>
struct
dango::
detail::
make_uint_help<dango::builtin::sint>
final
{
  using type = dango::builtin::uint;

  DANGO_UNINSTANTIABLE(make_uint_help)
};

template<>
struct
dango::
detail::
make_uint_help<dango::builtin::slong>
final
{
  using type = dango::builtin::ulong;

  DANGO_UNINSTANTIABLE(make_uint_help)
};

template<>
struct
dango::
detail::
make_uint_help<dango::builtin::slonglong>
final
{
  using type = dango::builtin::ulonglong;

  DANGO_UNINSTANTIABLE(make_uint_help)
};

/*** make_sint ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct make_sint_help;

  template<>
  struct make_sint_help<dango::builtin::uchar>;
  template<>
  struct make_sint_help<dango::builtin::ushort>;
  template<>
  struct make_sint_help<dango::builtin::uint>;
  template<>
  struct make_sint_help<dango::builtin::ulong>;
  template<>
  struct make_sint_help<dango::builtin::ulonglong>;
}

namespace
dango
{
  template
  <typename tp_type>
  using make_sint =
    dango::copy_cv<tp_type, typename detail::make_sint_help<dango::remove_cv<tp_type>>::type>;
}

template
<typename tp_type>
struct
dango::
detail::
make_sint_help
final
{
  using type = tp_type;

  DANGO_UNINSTANTIABLE(make_sint_help)
};

template<>
struct
dango::
detail::
make_sint_help<dango::builtin::uchar>
final
{
  using type = dango::builtin::schar;

  DANGO_UNINSTANTIABLE(make_sint_help)
};

template<>
struct
dango::
detail::
make_sint_help<dango::builtin::ushort>
final
{
  using type = dango::builtin::sshort;

  DANGO_UNINSTANTIABLE(make_sint_help)
};

template<>
struct
dango::
detail::
make_sint_help<dango::builtin::uint>
final
{
  using type = dango::builtin::sint;

  DANGO_UNINSTANTIABLE(make_sint_help)
};

template<>
struct
dango::
detail::
make_sint_help<dango::builtin::ulong>
final
{
  using type = dango::builtin::slong;

  DANGO_UNINSTANTIABLE(make_sint_help)
};

template<>
struct
dango::
detail::
make_sint_help<dango::builtin::ulonglong>
final
{
  using type = dango::builtin::slonglong;

  DANGO_UNINSTANTIABLE(make_sint_help)
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
    typename detail::decay_help<dango::remove_ref<tp_type>>::type;
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

  DANGO_UNINSTANTIABLE(decay_help)
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

  DANGO_UNINSTANTIABLE(decay_help)
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

  DANGO_UNINSTANTIABLE(decay_help)
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

  DANGO_UNINSTANTIABLE(decay_help)
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
    typename detail::conditional_help<tp_cond, tp_true_type, tp_false_type>::type;
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

  DANGO_UNINSTANTIABLE(conditional_help)
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

  DANGO_UNINSTANTIABLE(conditional_help)
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
    dango::is_same_ignore_cv<tp_type, dango::builtin::uchar> ||
    dango::is_same_ignore_cv<tp_type, dango::builtin::ushort> ||
    dango::is_same_ignore_cv<tp_type, dango::builtin::uint> ||
    dango::is_same_ignore_cv<tp_type, dango::builtin::ulong> ||
    dango::is_same_ignore_cv<tp_type, dango::builtin::ulonglong>;
}

/*** is_sint ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_sint =
    dango::is_same_ignore_cv<tp_type, dango::builtin::schar> ||
    dango::is_same_ignore_cv<tp_type, dango::builtin::sshort> ||
    dango::is_same_ignore_cv<tp_type, dango::builtin::sint> ||
    dango::is_same_ignore_cv<tp_type, dango::builtin::slong> ||
    dango::is_same_ignore_cv<tp_type, dango::builtin::slonglong>;
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

/*** is_array is_array_rt_bound is_array_ct_bound ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  inline constexpr bool const is_array_rt_bound_help = false;
  template
  <typename tp_type>
  inline constexpr bool const is_array_rt_bound_help<tp_type[]> = true;

  template
  <typename tp_type>
  inline constexpr bool const is_array_ct_bound_help = false;
  template
  <typename tp_type, dango::usize tp_size>
  inline constexpr bool const is_array_ct_bound_help<tp_type[tp_size]> = true;
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_array_rt_bound = dango::detail::is_array_rt_bound_help<tp_type>;

  template
  <typename tp_type>
  concept is_array_ct_bound = dango::detail::is_array_ct_bound_help<tp_type>;

  template
  <typename tp_type>
  concept is_array =
    dango::is_array_rt_bound<tp_type> || dango::is_array_ct_bound<tp_type>;
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
}

/*** is_union ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_union = bool(__is_union(tp_type));
}

/*** is_class ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_class = bool(__is_class(tp_type));
}

/*** is_class_or_union ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_class_or_union =
    dango::is_class<tp_type> || dango::is_union<tp_type>;
}

/*** is_lvalue_ref ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  inline constexpr bool const is_lvalue_ref_help = false;

  template
  <typename tp_type>
  inline constexpr bool const is_lvalue_ref_help<tp_type&> = true;
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_lvalue_ref = dango::detail::is_lvalue_ref_help<tp_type>;
}

/*** is_rvalue_ref ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  inline constexpr bool const is_rvalue_ref_help = false;

  template
  <typename tp_type>
  inline constexpr bool const is_rvalue_ref_help<tp_type&&> = true;
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_rvalue_ref = dango::detail::is_rvalue_ref_help<tp_type>;
}

/*** is_ref ***/

namespace
dango
{
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

/*** is_fundamnetal ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_fundamental =
    dango::is_void<tp_type> ||
    dango::is_null_tag<tp_type> ||
    dango::is_arithmetic<tp_type>;
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
}

/*** is_object ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_object =
    dango::is_object_exclude_array<tp_type> || dango::is_array<tp_type>;
}

/*** is_compound ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_compound = !dango::is_fundamental<tp_type>;
}

/*** is_referenceable ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_referenceable =
    dango::is_object<tp_type> || dango::is_unqualified_func<tp_type>;
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
  <dango::is_array_rt_bound tp_type>
  inline constexpr bool const is_destructible_help<tp_type> = false;
  template
  <dango::is_array_ct_bound tp_type>
  inline constexpr bool const is_destructible_help<tp_type> =
    dango::detail::is_destructible_help<dango::remove_all_array<tp_type>>;

  template
  <typename tp_type>
  inline constexpr bool const is_noexcept_destructible_help = requires{ { dango::declval<tp_type&>().~tp_type() }noexcept; };
  template
  <dango::is_ref tp_type>
  inline constexpr bool const is_noexcept_destructible_help<tp_type> = true;
  template
  <dango::is_array_rt_bound tp_type>
  inline constexpr bool const is_noexcept_destructible_help<tp_type> = false;
  template
  <dango::is_array_ct_bound tp_type>
  inline constexpr bool const is_noexcept_destructible_help<tp_type> =
    dango::detail::is_noexcept_destructible_help<dango::remove_all_array<tp_type>>;

  template
  <typename tp_type>
  inline constexpr bool const is_trivial_destructible_help = bool(__has_trivial_destructor(tp_type));
  template
  <dango::is_ref tp_type>
  inline constexpr bool const is_trivial_destructible_help<tp_type> = true;
  template
  <dango::is_array_rt_bound tp_type>
  inline constexpr bool const is_trivial_destructible_help<tp_type> = false;
  template
  <dango::is_array_ct_bound tp_type>
  inline constexpr bool const is_trivial_destructible_help<tp_type> =
    dango::detail::is_trivial_destructible_help<dango::remove_all_array<tp_type>>;
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_destructible =
    (dango::is_object<tp_type> || dango::is_ref<tp_type>) && dango::detail::is_destructible_help<dango::remove_cv<tp_type>>;

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
  <typename tp_type1, typename tp_type2>
  concept both_void = dango::is_void<tp_type1> && dango::is_void<tp_type2>;
  template
  <typename tp_type1, typename tp_type2>
  concept neither_void = !dango::is_void<tp_type1> && !dango::is_void<tp_type2>;

  template
  <typename tp_type>
  concept is_destructible_exclude_array = !dango::is_array<tp_type> && dango::is_destructible<tp_type>;
  template
  <typename tp_type>
  concept is_noexcept_destructible_exclude_array =
    dango::detail::is_destructible_exclude_array<tp_type> && dango::is_noexcept_destructible<tp_type>;

  template
  <typename tp_from, typename tp_to>
  concept is_convertible_help =
    dango::detail::is_destructible_exclude_array<tp_to> &&
    requires{ dango::detail::is_convertible_test<tp_to>(dango::declval<tp_from>()); };
  template
  <typename tp_from, typename tp_to>
  concept is_noexcept_convertible_help =
    dango::detail::is_noexcept_destructible_exclude_array<tp_to> &&
    requires{ { dango::detail::is_convertible_test<tp_to>(dango::declval<tp_from>()) }noexcept; };

  template
  <typename tp_from, typename tp_to>
  concept is_convertible_ret_help =
    (dango::is_same_ignore_cv<tp_from, tp_to> && dango::detail::is_destructible_exclude_array<tp_to>) ||
    dango::detail::is_convertible_help<tp_from, tp_to>;
  template
  <typename tp_from, typename tp_to>
  concept is_noexcept_convertible_ret_help =
    (dango::is_same_ignore_cv<tp_from, tp_to> && dango::detail::is_noexcept_destructible_exclude_array<tp_to>) ||
    dango::detail::is_noexcept_convertible_help<tp_from, tp_to>;
}

namespace
dango
{
  template
  <typename tp_from, typename tp_to>
  concept is_convertible =
    dango::detail::both_void<tp_from, tp_to> ||
    (dango::detail::neither_void<tp_from, tp_to> && dango::detail::is_convertible_help<tp_from, tp_to>);

  template
  <typename tp_from, typename tp_to>
  concept is_noexcept_convertible =
    dango::detail::both_void<tp_from, tp_to> ||
    (dango::detail::neither_void<tp_from, tp_to> && dango::detail::is_noexcept_convertible_help<tp_from, tp_to>);

  template
  <typename tp_from, typename tp_to>
  concept is_convertible_ret =
    dango::detail::both_void<tp_from, tp_to> ||
    (dango::detail::neither_void<tp_from, tp_to> && dango::detail::is_convertible_ret_help<tp_from, tp_to>);

  template
  <typename tp_from, typename tp_to>
  concept is_noexcept_convertible_ret =
    dango::detail::both_void<tp_from, tp_to> ||
    (dango::detail::neither_void<tp_from, tp_to> && dango::detail::is_noexcept_convertible_ret_help<tp_from, tp_to>);
}

/*** is_callable is_noexcept_callable is_callable_ret is_noexcept_callable_ret ***/

namespace
dango
{
  template
  <typename tp_func, typename... tp_args>
  concept is_callable = requires{ dango::declval<tp_func>()(dango::declval<tp_args>()...); };

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
    dango::is_constructible<tp_type, tp_args...> &&
    dango::is_noexcept_destructible<tp_type> &&
    dango::detail::is_noexcept_constructible_help<tp_type, tp_args...>;

  template
  <typename tp_type, typename... tp_args>
  concept is_trivial_constructible =
    dango::is_noexcept_constructible<tp_type, tp_args...> &&
    dango::is_trivial_destructible<tp_type> &&
    dango::detail::is_trivial_constructible_help<tp_type, tp_args...>;
}

/*** is_brace_constructible is_noexcept_brace_constructible ***/

namespace
dango::detail
{
  template
  <typename tp_type, typename... tp_args>
  inline constexpr bool const is_brace_constructible_help = requires{ { tp_type{ dango::declval<tp_args>()... } }; };
  template
  <dango::is_ref tp_type, typename... tp_args>
  inline constexpr bool const is_brace_constructible_help<tp_type, tp_args...> = dango::is_constructible<tp_type, tp_args...>;

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
    dango::is_destructible<tp_type> && dango::detail::is_brace_constructible_help<tp_type, tp_args...>;

  template
  <typename tp_type, typename... tp_args>
  concept is_noexcept_brace_constructible =
    dango::is_brace_constructible<tp_type, tp_args...> &&
    dango::is_noexcept_destructible<tp_type> &&
    dango::detail::is_noexcept_brace_constructible_help<tp_type, tp_args...>;
}

/*** is_default_constructible is_trivial_default_constructible is_noexcept_default_constructible ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_default_constructible =
    !dango::is_ref<tp_type> && dango::is_constructible<tp_type>;

  template
  <typename tp_type>
  concept is_noexcept_default_constructible =
    dango::is_default_constructible<tp_type> && dango::is_noexcept_constructible<tp_type>;

  template
  <typename tp_type>
  concept is_trivial_default_constructible =
    dango::is_noexcept_default_constructible<tp_type> && dango::is_trivial_constructible<tp_type>;
}

/*** is_copy_constructible is_trivial_copy_constructible is_noexcept_copy_constructible ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_copy_constructible =
    dango::is_ref<tp_type> || dango::is_constructible<tp_type, tp_type const&>;

  template
  <typename tp_type>
  concept is_noexcept_copy_constructible =
    dango::is_ref<tp_type> || (dango::is_copy_constructible<tp_type> && dango::is_noexcept_constructible<tp_type, tp_type const&>);

  template
  <typename tp_type>
  concept is_trivial_copy_constructible =
    dango::is_ref<tp_type> || (dango::is_noexcept_copy_constructible<tp_type> && dango::is_trivial_constructible<tp_type, tp_type const&>);
}

/*** is_move_constructible is_trivial_move_constructible is_noexcept_move_constructible ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_move_constructible =
    dango::is_ref<tp_type> || dango::is_constructible<tp_type, tp_type&&>;

  template
  <typename tp_type>
  concept is_noexcept_move_constructible =
    dango::is_ref<tp_type> || (dango::is_move_constructible<tp_type> && dango::is_noexcept_constructible<tp_type, tp_type&&>);

  template
  <typename tp_type>
  concept is_trivial_move_constructible =
    dango::is_ref<tp_type> || (dango::is_noexcept_move_constructible<tp_type> && dango::is_trivial_constructible<tp_type, tp_type&&>);
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
    !dango::is_ref<tp_type> && dango::is_assignable<tp_type&, tp_type const&>;

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
    !dango::is_ref<tp_type> && dango::is_assignable<tp_type&, tp_type&&>;

  template
  <typename tp_type>
  concept is_noexcept_move_assignable =
    dango::is_move_assignable<tp_type> && dango::is_noexcept_assignable<tp_type&, tp_type&&>;

  template
  <typename tp_type>
  concept is_trivial_move_assignable =
    dango::is_noexcept_move_assignable<tp_type> && dango::is_trivial_assignable<tp_type&, tp_type&&>;
}

/*** underlying_type ***/

namespace
dango::detail
{
  template
  <typename tp_type, bool tp_enum = dango::is_enum<tp_type>>
  struct underlying_type_help;

  template
  <typename tp_type>
  struct underlying_type_help<tp_type, true>;
}

namespace
dango
{
  template
  <typename tp_enum>
  using underlying_type =
    typename detail::underlying_type_help<tp_enum>::type;
}

template
<typename tp_type, bool tp_enum>
struct
dango::
detail::
underlying_type_help
final
{
  using type = tp_type;

  DANGO_UNINSTANTIABLE(underlying_type_help)
};

template
<typename tp_type>
struct
dango::
detail::
underlying_type_help<tp_type, true>
final
{
  using type = __underlying_type(tp_type);

  DANGO_UNINSTANTIABLE(underlying_type_help)
};

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

/*** sizeof_with_void alignof_with_void ***/

namespace
dango
{
  template
  <typename tp_type, dango::usize tp_default = dango::usize(1)>
  inline constexpr dango::usize const sizeof_with_void = sizeof(tp_type);
  template
  <dango::is_void tp_type, dango::usize tp_default>
  inline constexpr dango::usize const sizeof_with_void<tp_type, tp_default> = tp_default;

  template
  <typename tp_type, dango::usize tp_default = dango::usize(1)>
  inline constexpr dango::usize const alignof_with_void = alignof(tp_type);
  template
  <dango::is_void tp_type, dango::usize tp_default>
  inline constexpr dango::usize const alignof_with_void<tp_type, tp_default> = tp_default;
}

#endif // DANGO_TRAITS_HPP_INCLUDED

