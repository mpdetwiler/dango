#ifndef __DANGO_TRAITS_HPP__
#define __DANGO_TRAITS_HPP__

/*** incomplete_type ***/

namespace
dango
{
  struct incomplete_type;
}

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

/*** preserve_cv ***/

namespace
dango::detail
{
  template
  <typename tp_from, typename tp_to>
  struct preserve_cv_help;

  template
  <typename tp_from, typename tp_to>
  struct preserve_cv_help<tp_from const, tp_to>;

  template
  <typename tp_from, typename tp_to>
  struct preserve_cv_help<tp_from volatile, tp_to>;

  template
  <typename tp_from, typename tp_to>
  struct preserve_cv_help<tp_from const volatile, tp_to>;
}

namespace
dango
{
  template
  <typename tp_from, typename tp_to>
  using preserve_cv =
    typename detail::preserve_cv_help<tp_from, tp_to>::type;
}

template
<typename tp_from, typename tp_to>
struct
dango::
detail::
preserve_cv_help
final
{
  using type = tp_to;

  DANGO_UNINSTANTIABLE(preserve_cv_help)
};

template
<typename tp_from, typename tp_to>
struct
dango::
detail::
preserve_cv_help<tp_from const, tp_to>
final
{
  using type = tp_to const;

  DANGO_UNINSTANTIABLE(preserve_cv_help)
};

template
<typename tp_from, typename tp_to>
struct
dango::
detail::
preserve_cv_help<tp_from volatile, tp_to>
final
{
  using type = tp_to volatile;

  DANGO_UNINSTANTIABLE(preserve_cv_help)
};

template
<typename tp_from, typename tp_to>
struct
dango::
detail::
preserve_cv_help<tp_from const volatile, tp_to>
final
{
  using type = tp_to const volatile;

  DANGO_UNINSTANTIABLE(preserve_cv_help)
};

/*** make_uint ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct make_uint_help;

  template<>
  struct make_uint_help<dango::s_byte>;
  template<>
  struct make_uint_help<dango::s_short>;
  template<>
  struct make_uint_help<dango::s_int>;
  template<>
  struct make_uint_help<dango::s_long>;
  template<>
  struct make_uint_help<dango::s_cent>;
}

namespace
dango
{
  template
  <typename tp_type>
  using make_uint =
    dango::preserve_cv<tp_type, typename detail::make_uint_help<dango::remove_cv<tp_type>>::type>;
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
make_uint_help<dango::s_byte>
final
{
  using type = dango::u_byte;

  DANGO_UNINSTANTIABLE(make_uint_help)
};

template<>
struct
dango::
detail::
make_uint_help<dango::s_short>
final
{
  using type = dango::u_short;

  DANGO_UNINSTANTIABLE(make_uint_help)
};

template<>
struct
dango::
detail::
make_uint_help<dango::s_int>
final
{
  using type = dango::u_int;

  DANGO_UNINSTANTIABLE(make_uint_help)
};

template<>
struct
dango::
detail::
make_uint_help<dango::s_long>
final
{
  using type = dango::u_long;

  DANGO_UNINSTANTIABLE(make_uint_help)
};

template<>
struct
dango::
detail::
make_uint_help<dango::s_cent>
final
{
  using type = dango::u_cent;

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
  struct make_sint_help<dango::u_byte>;
  template<>
  struct make_sint_help<dango::u_short>;
  template<>
  struct make_sint_help<dango::u_int>;
  template<>
  struct make_sint_help<dango::u_long>;
  template<>
  struct make_sint_help<dango::u_cent>;
}

namespace
dango
{
  template
  <typename tp_type>
  using make_sint =
    dango::preserve_cv<tp_type, typename detail::make_sint_help<dango::remove_cv<tp_type>>::type>;
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
make_sint_help<dango::u_byte>
final
{
  using type = dango::s_byte;

  DANGO_UNINSTANTIABLE(make_sint_help)
};

template<>
struct
dango::
detail::
make_sint_help<dango::u_short>
final
{
  using type = dango::s_short;

  DANGO_UNINSTANTIABLE(make_sint_help)
};

template<>
struct
dango::
detail::
make_sint_help<dango::u_int>
final
{
  using type = dango::s_int;

  DANGO_UNINSTANTIABLE(make_sint_help)
};

template<>
struct
dango::
detail::
make_sint_help<dango::u_long>
final
{
  using type = dango::s_long;

  DANGO_UNINSTANTIABLE(make_sint_help)
};

template<>
struct
dango::
detail::
make_sint_help<dango::u_cent>
final
{
  using type = dango::s_cent;

  DANGO_UNINSTANTIABLE(make_sint_help)
};

/*** void_type ***/

namespace
dango
{
  template
  <typename... tp_types>
  using void_type = void;
}

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
    typename detail::enable_if_help<tp_cond, tp_type>::type;
}

template
<bool tp_cond, typename tp_type>
struct
dango::
detail::
enable_if_help
final
{
  DANGO_UNINSTANTIABLE(enable_if_help)
};

template
<typename tp_type>
struct
dango::
detail::
enable_if_help<true, tp_type>
final
{
  using type = tp_type;

  DANGO_UNINSTANTIABLE(enable_if_help)
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

/*** test tags ***/

namespace
dango::detail
{
  struct
  normal_test_tag
  final
  {
    DANGO_TAG_TYPE(normal_test_tag)
  };

  struct
  trivial_test_tag
  final
  {
    DANGO_TAG_TYPE(trivial_test_tag)
  };

  struct
  noexcept_test_tag
  final
  {
    DANGO_TAG_TYPE(noexcept_test_tag)
  };

  struct
  virtual_test_tag
  final
  {
    DANGO_TAG_TYPE(virtual_test_tag)
  };

  inline constexpr detail::normal_test_tag const normal_test_val{ };
  inline constexpr detail::trivial_test_tag const trivial_test_val{ };
  inline constexpr detail::noexcept_test_tag const noexcept_test_val{ };
  inline constexpr detail::virtual_test_tag const virtual_test_val{ };
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
    detail::is_uint_help<dango::remove_cv<tp_type>>;
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
    detail::is_sint_help<dango::remove_cv<tp_type>>;
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

/*** is_nonbool_integral ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  constexpr bool const is_nonbool_integral_help =
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
  constexpr bool const is_nonbool_integral =
    detail::is_nonbool_integral_help<dango::remove_cv<tp_type>>;
}

/* is_integral */

namespace
dango::detail
{
  template
  <typename tp_type>
  constexpr bool const is_integral_help =
    dango::is_same<tp_type, bool> ||
    dango::detail::is_nonbool_integral_help<tp_type>;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_integral =
    detail::is_integral_help<dango::remove_cv<tp_type>>;
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
    detail::is_float_help<dango::remove_cv<tp_type>>;
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

/*** array_rank ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr dango::usize const array_rank = dango::usize(0);

  template
  <typename tp_type>
  constexpr dango::usize const array_rank<tp_type[]> =
    dango::array_rank<tp_type> + dango::usize(1);

  template
  <typename tp_type, dango::usize tp_size>
  constexpr dango::usize const array_rank<tp_type[tp_size]> =
    dango::array_rank<tp_type> + dango::usize(1);
}

/*** array_size ***/

namespace
dango
{
  template
  <typename tp_type, dango::usize tp_dim = dango::usize(0)>
  constexpr dango::usize const array_size = dango::usize(0);

  template
  <typename tp_type>
  constexpr dango::usize const array_size<tp_type[], dango::usize(0)> = dango::usize(0);

  template
  <typename tp_type, dango::usize tp_dim>
  constexpr dango::usize const array_size<tp_type[], tp_dim> =
    dango::array_size<tp_type, tp_dim - dango::usize(1)>;

  template
  <typename tp_type, dango::usize tp_size>
  constexpr dango::usize const array_size<tp_type[tp_size], dango::usize(0)> = tp_size;

  template
  <typename tp_type, dango::usize tp_size, dango::usize tp_dim>
  constexpr dango::usize const array_size<tp_type[tp_size], tp_dim> =
    dango::array_size<tp_type, tp_dim - dango::usize(1)>;
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
    detail::is_ptr_help<dango::remove_cv<tp_type>>;
}

/*** is_func_ptr ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  constexpr bool const is_func_ptr_help = false;

  template
  <typename tp_type>
  constexpr bool const is_func_ptr_help<tp_type*> =
    dango::is_func<tp_type>;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_func_ptr =
    detail::is_func_ptr_help<dango::remove_cv<tp_type>>;
}

/*** is_object_ptr ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  constexpr bool const is_object_ptr_help = false;

  template
  <typename tp_type>
  constexpr bool const is_object_ptr_help<tp_type*> =
    !dango::is_void<tp_type> && !dango::is_func<tp_type>;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_object_ptr =
    detail::is_object_ptr_help<dango::remove_cv<tp_type>>;
}

/*** is_void_ptr ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  constexpr bool const is_void_ptr_help = false;

  template
  <typename tp_type>
  constexpr bool const is_void_ptr_help<tp_type*> =
    dango::is_void<tp_type>;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_void_ptr =
    detail::is_void_ptr_help<dango::remove_cv<tp_type>>;
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
    detail::is_member_ptr_help<dango::remove_cv<tp_type>>;
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
    detail::is_member_func_ptr_help<dango::remove_cv<tp_type>>;
}

/*** is_member_object_ptr ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_member_object_ptr =
    dango::is_member_ptr<tp_type> && !dango::is_member_func_ptr<tp_type>;
}

/*** is_nonbool_arithmetic ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_nonbool_arithmetic =
    dango::is_nonbool_integral<tp_type> || dango::is_float<tp_type>;
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

/*** is_trivial_copyable ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_trivial_copyable = __is_trivially_copyable(tp_type);
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
  <typename tp_type, typename tp_enabled = dango::enable_tag>
  constexpr bool const is_signed = false;

  template
  <typename tp_type>
  constexpr bool const is_signed<tp_type, dango::enable_if<dango::is_arithmetic<tp_type>>> =
    (tp_type(-1) < tp_type(0));
}

/*** is_unsigned ***/

namespace
dango
{
  template
  <typename tp_type, typename tp_enabled = dango::enable_tag>
  constexpr bool const is_unsigned = false;

  template
  <typename tp_type>
  constexpr bool const is_unsigned<tp_type, dango::enable_if<dango::is_arithmetic<tp_type>>> =
    (tp_type(-1) > tp_type(0));
}

/*** is_base_of ***/

namespace
dango
{
  template
  <typename tp_base, typename tp_derived>
  constexpr bool const is_base_of = __is_base_of(tp_base, tp_derived);
}

/*** is_destructible is_trivial_destructible is_noexcept_destructible has_virtual_destructor ***/

namespace
dango::detail
{
  template
  <
    typename tp_type,
    typename tp_base = dango::remove_cv<tp_type>,
    typename tp_enabled = decltype(dango::declval<tp_base&>().~tp_base())
  >
  constexpr auto
  is_destructible_help
  (detail::normal_test_tag const)noexcept->bool
  {
    return true;
  }

  template
  <
    typename tp_type,
    typename tp_base = dango::remove_cv<tp_type>,
    typename tp_enabled = decltype(dango::declval<tp_base&>().~tp_base())
  >
  constexpr auto
  is_destructible_help
  (detail::trivial_test_tag const)noexcept->bool
  {
    return __has_trivial_destructor(tp_base);
  }

  template
  <
    typename tp_type,
    typename tp_base = dango::remove_cv<tp_type>,
    typename tp_enabled = decltype(dango::declval<tp_base&>().~tp_base())
  >
  constexpr auto
  is_destructible_help
  (detail::noexcept_test_tag const)noexcept->bool
  {
    return noexcept(dango::declval<tp_base&>().~tp_base());
  }

  template
  <
    typename tp_type,
    typename tp_base = dango::remove_cv<tp_type>,
    typename tp_enabled = decltype(dango::declval<tp_base&>().~tp_base())
  >
  constexpr auto
  is_destructible_help
  (detail::virtual_test_tag const)noexcept->bool
  {
    return __has_virtual_destructor(tp_base);
  }

  template
  <typename tp_anything, typename tp_tag>
  constexpr auto
  is_destructible_help
  (tp_tag const)noexcept->bool
  {
    return false;
  }
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_destructible =
    detail::is_destructible_help<tp_type>(detail::normal_test_val);

  template
  <typename tp_type>
  constexpr bool const is_destructible<tp_type&> = true;
  template
  <typename tp_type>
  constexpr bool const is_destructible<tp_type&&> = true;
  template
  <typename tp_type>
  constexpr bool const is_destructible<tp_type[]> = false;
  template
  <typename tp_type, dango::usize tp_size>
  constexpr bool const is_destructible<tp_type[tp_size]> =
    dango::is_destructible<tp_type>;


  template
  <typename tp_type>
  constexpr bool const is_trivial_destructible =
    detail::is_destructible_help<tp_type>(detail::trivial_test_val);

  template
  <typename tp_type>
  constexpr bool const is_trivial_destructible<tp_type&> = true;
  template
  <typename tp_type>
  constexpr bool const is_trivial_destructible<tp_type&&> = true;
  template
  <typename tp_type>
  constexpr bool const is_trivial_destructible<tp_type[]> = false;
  template
  <typename tp_type, dango::usize tp_size>
  constexpr bool const is_trivial_destructible<tp_type[tp_size]> =
    dango::is_trivial_destructible<tp_type>;

  template
  <typename tp_type>
  constexpr bool const is_noexcept_destructible =
    detail::is_destructible_help<tp_type>(detail::noexcept_test_val);

  template
  <typename tp_type>
  constexpr bool const is_noexcept_destructible<tp_type&> = true;
  template
  <typename tp_type>
  constexpr bool const is_noexcept_destructible<tp_type&&> = true;
  template
  <typename tp_type>
  constexpr bool const is_noexcept_destructible<tp_type[]> = false;
  template
  <typename tp_type, dango::usize tp_size>
  constexpr bool const is_noexcept_destructible<tp_type[tp_size]> =
    dango::is_noexcept_destructible<tp_type>;

  template
  <typename tp_type>
  constexpr bool const has_virtual_destructor =
    detail::is_destructible_help<tp_type>(detail::virtual_test_val);

  template
  <typename tp_type>
  constexpr bool const has_virtual_destructor<tp_type&> = false;
  template
  <typename tp_type>
  constexpr bool const has_virtual_destructor<tp_type&&> = false;
  template
  <typename tp_type>
  constexpr bool const has_virtual_destructor<tp_type[]> = false;
  template
  <typename tp_type, dango::usize tp_size>
  constexpr bool const has_virtual_destructor<tp_type[tp_size]> = false;
}

/*** is_convertible is_noexcept_convertible is_convertible_ret is_noexcept_convertible_ret ***/

namespace
dango::detail
{
  template
  <typename tp_arg>
  constexpr void
  is_convertible_test(tp_arg)noexcept;

  template
  <
    typename tp_from,
    typename tp_to,
    typename tp_enabled =
      decltype(detail::is_convertible_test<tp_to>(dango::declval<tp_from>()))
  >
  constexpr auto
  is_convertible_help
  (detail::normal_test_tag const)noexcept->bool
  {
    return true;
  }

  template
  <
    typename tp_from,
    typename tp_to,
    typename tp_enabled =
      decltype(detail::is_convertible_test<tp_to>(dango::declval<tp_from>()))
  >
  constexpr auto
  is_convertible_help
  (detail::noexcept_test_tag const)noexcept->bool
  {
    return noexcept(detail::is_convertible_test<tp_to>(dango::declval<tp_from>()));
  }

  template
  <typename... tp_anything, typename tp_tag>
  constexpr auto
  is_convertible_help
  (tp_tag const)noexcept->bool
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
    !dango::is_array<tp_to> &&
    !dango::is_func<tp_to> &&
    (
      (dango::is_void<tp_from> && dango::is_void<tp_to>) ||
      detail::is_convertible_help<tp_from, tp_to>(detail::normal_test_val)
    );

  template
  <typename tp_from, typename tp_to>
  constexpr bool const is_noexcept_convertible =
    !dango::is_array<tp_to> &&
    !dango::is_func<tp_to> &&
    (
      (dango::is_void<tp_from> && dango::is_void<tp_to>) ||
      detail::is_convertible_help<tp_from, tp_to>(detail::noexcept_test_val)
    );

  template
  <typename tp_from, typename tp_to>
  constexpr bool const is_convertible_ret =
    !dango::is_array<tp_to> &&
    !dango::is_func<tp_to> &&
    (
      (dango::is_void<tp_from> && dango::is_void<tp_to>) ||
      (dango::is_same<dango::remove_cv<tp_from>, dango::remove_cv<tp_to>> && dango::is_destructible<tp_to>) ||
      detail::is_convertible_help<tp_from, tp_to>(detail::normal_test_val)
    );

  template
  <typename tp_from, typename tp_to>
  constexpr bool const is_noexcept_convertible_ret =
    !dango::is_array<tp_to> &&
    !dango::is_func<tp_to> &&
    (
      (dango::is_void<tp_from> && dango::is_void<tp_to>) ||
      (dango::is_same<dango::remove_cv<tp_from>, dango::remove_cv<tp_to>> && dango::is_noexcept_destructible<tp_to>) ||
      detail::is_convertible_help<tp_from, tp_to>(detail::noexcept_test_val)
    );
}

/*** is_callable is_callable_ret ***/

namespace
dango::detail
{
  template
  <
    typename tp_func,
    typename... tp_args,
    typename tp_enabled = decltype(dango::declval<tp_func>()(dango::declval<tp_args>()...))
  >
  constexpr auto
  is_callable_help
  (detail::normal_test_tag const)noexcept->bool
  {
    return true;
  }

  template
  <
    typename tp_func,
    typename... tp_args,
    typename tp_enabled = decltype(dango::declval<tp_func>()(dango::declval<tp_args>()...))
  >
  constexpr auto
  is_callable_help
  (detail::noexcept_test_tag const)noexcept->bool
  {
    return noexcept(dango::declval<tp_func>()(dango::declval<tp_args>()...));
  }

  template
  <typename... tp_anything, typename tp_tag>
  constexpr auto
  is_callable_help
  (tp_tag const)noexcept->bool
  {
    return false;
  }

  template
  <
    typename tp_ret,
    typename tp_func,
    typename... tp_args,
    typename tp_enabled_ret = decltype(dango::declval<tp_func>()(dango::declval<tp_args>()...))
  >
  constexpr auto
  is_callable_ret_help
  (detail::normal_test_tag const)noexcept->bool
  {
    return dango::is_convertible_ret<tp_enabled_ret, tp_ret>;
  }

  template
  <
    typename tp_ret,
    typename tp_func,
    typename... tp_args,
    typename tp_enabled_ret = decltype(dango::declval<tp_func>()(dango::declval<tp_args>()...))
  >
  constexpr auto
  is_callable_ret_help
  (detail::noexcept_test_tag const)noexcept->bool
  {
    return dango::is_noexcept_convertible_ret<tp_enabled_ret, tp_ret> && noexcept(dango::declval<tp_func>()(dango::declval<tp_args>()...));
  }

  template
  <typename... tp_anything, typename tp_tag>
  constexpr auto
  is_callable_ret_help
  (tp_tag const)noexcept->bool
  {
    return false;
  }
}

namespace
dango
{
  template
  <typename tp_func, typename... tp_args>
  constexpr bool const is_callable =
    detail::is_callable_help<tp_func, tp_args...>(detail::normal_test_val);

  template
  <typename tp_func, typename... tp_args>
  constexpr bool const is_noexcept_callable =
    detail::is_callable_help<tp_func, tp_args...>(detail::noexcept_test_val);

  template
  <typename tp_ret, typename tp_func, typename... tp_args>
  constexpr bool const is_callable_ret =
    detail::is_callable_ret_help<tp_ret, tp_func, tp_args...>(detail::normal_test_val);

  template
  <typename tp_ret, typename tp_func, typename... tp_args>
  constexpr bool const is_noexcept_callable_ret =
    detail::is_callable_ret_help<tp_ret, tp_func, tp_args...>(detail::noexcept_test_val);
}

/*** is_constructible is_trivial_constructible is_noexcept_constructible ***/

namespace
dango::detail
{
  template
  <
    typename tp_type,
    typename... tp_args,
    dango::enable_if<__is_constructible(tp_type, tp_args...)> = dango::enable_val
  >
  constexpr auto
  is_constructible_help
  (detail::normal_test_tag const)noexcept->bool
  {
    return true;
  }

  template
  <
    typename tp_type,
    typename... tp_args,
    dango::enable_if<__is_constructible(tp_type, tp_args...)> = dango::enable_val
  >
  constexpr auto
  is_constructible_help
  (detail::trivial_test_tag const)noexcept->bool
  {
    return __is_trivially_constructible(tp_type, tp_args...);
  }

  template
  <
    typename tp_type,
    typename... tp_args,
    dango::enable_if<__is_constructible(tp_type, tp_args...)> = dango::enable_val
  >
  constexpr auto
  is_constructible_help
  (detail::noexcept_test_tag const)noexcept->bool
  {
    return noexcept(tp_type(dango::declval<tp_args>()...));
  }

  template
  <typename... tp_anything, typename tp_tag>
  constexpr auto
  is_constructible_help
  (tp_tag const)noexcept->bool
  {
    return false;
  }
}

namespace
dango
{
  template
  <typename tp_type, typename... tp_args>
  constexpr bool const is_constructible =
    detail::is_constructible_help<tp_type, tp_args...>(detail::normal_test_val);

  template
  <typename tp_type, typename... tp_args>
  constexpr bool const is_trivial_constructible =
    detail::is_constructible_help<tp_type, tp_args...>(detail::trivial_test_val);

  template
  <typename tp_type, typename... tp_args>
  constexpr bool const is_noexcept_constructible =
    detail::is_constructible_help<tp_type, tp_args...>(detail::noexcept_test_val);

  template
  <typename tp_type>
  constexpr bool const is_noexcept_constructible<tp_type[]> =
    dango::is_noexcept_constructible<tp_type>;
  template
  <typename tp_type, dango::usize tp_size>
  constexpr bool const is_noexcept_constructible<tp_type[tp_size]> =
    dango::is_noexcept_constructible<tp_type>;
}

/*** is_default_constructible is_trivial_default_constructible is_noexcept_default_constructible ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_default_constructible =
    dango::is_constructible<tp_type>;

  template
  <typename tp_type>
  constexpr bool const is_trivial_default_constructible =
    dango::is_trivial_constructible<tp_type>;

  template
  <typename tp_type>
  constexpr bool const is_noexcept_default_constructible =
    dango::is_noexcept_constructible<tp_type>;
}

/*** is_copy_constructible is_trivial_copy_constructible is_noexcept_copy_constructible ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_copy_constructible =
    dango::is_constructible<tp_type, tp_type const&>;

  template
  <typename tp_type>
  constexpr bool const is_trivial_copy_constructible =
    dango::is_trivial_constructible<tp_type, tp_type const&>;

  template
  <typename tp_type>
  constexpr bool const is_noexcept_copy_constructible =
    dango::is_noexcept_constructible<tp_type, tp_type const&>;
}

/*** is_move_constructible is_trivial_move_constructible is_noexcept_move_constructible ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_move_constructible =
    dango::is_constructible<tp_type, tp_type&&>;

  template
  <typename tp_type>
  constexpr bool const is_trivial_move_constructible =
    dango::is_trivial_constructible<tp_type, tp_type&&>;

  template
  <typename tp_type>
  constexpr bool const is_noexcept_move_constructible =
    dango::is_noexcept_constructible<tp_type, tp_type&&>;
}

/*** is_assignable is_trivial_assignable is_noexcept_assignable ***/

namespace
dango::detail
{
  template
  <
    typename tp_type,
    typename tp_arg,
    dango::enable_if<__is_assignable(tp_type, tp_arg)> = dango::enable_val
  >
  constexpr auto
  is_assignable_help
  (detail::normal_test_tag const)noexcept->bool
  {
    return true;
  }

  template
  <
    typename tp_type,
    typename tp_arg,
    dango::enable_if<__is_assignable(tp_type, tp_arg)> = dango::enable_val
  >
  constexpr auto
  is_assignable_help
  (detail::trivial_test_tag const)noexcept->bool
  {
    return __is_trivially_assignable(tp_type, tp_arg);
  }

  template
  <
    typename tp_type,
    typename tp_arg,
    dango::enable_if<__is_assignable(tp_type, tp_arg)> = dango::enable_val
  >
  constexpr auto
  is_assignable_help
  (detail::noexcept_test_tag const)noexcept->bool
  {
    return noexcept(dango::declval<tp_type>() = dango::declval<tp_arg>());
  }

  template
  <typename... tp_anything, typename tp_tag>
  constexpr auto
  is_assignable_help
  (tp_tag const)noexcept->bool
  {
    return false;
  }
}

namespace
dango
{
  template
  <typename tp_type, typename tp_arg>
  constexpr bool const is_assignable =
    detail::is_assignable_help<tp_type, tp_arg>(detail::normal_test_val);

  template
  <typename tp_type, typename tp_arg>
  constexpr bool const is_trivial_assignable =
    detail::is_assignable_help<tp_type, tp_arg>(detail::trivial_test_val);

  template
  <typename tp_type, typename tp_arg>
  constexpr bool const is_noexcept_assignable =
    detail::is_assignable_help<tp_type, tp_arg>(detail::noexcept_test_val);
}

/*** is_copy_assignable is_trivial_copy_assignable is_noexcept_copy_assignable ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_copy_assignable =
    dango::is_assignable<tp_type&, tp_type const&>;

  template
  <typename tp_type>
  constexpr bool const is_trivial_copy_assignable =
    dango::is_trivial_assignable<tp_type&, tp_type const&>;

  template
  <typename tp_type>
  constexpr bool const is_noexcept_copy_assignable =
    dango::is_noexcept_assignable<tp_type&, tp_type const&>;
}

/*** is_move_assignable is_trivial_move_assignable is_noexcept_move_assignable ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_move_assignable =
    dango::is_assignable<tp_type&, tp_type&&>;

  template
  <typename tp_type>
  constexpr bool const is_trivial_move_assignable =
    dango::is_trivial_assignable<tp_type&, tp_type&&>;

  template
  <typename tp_type>
  constexpr bool const is_noexcept_move_assignable =
    dango::is_noexcept_assignable<tp_type&, tp_type&&>;
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
  dango::uint32
  {
      little = dango::uint32(__ORDER_LITTLE_ENDIAN__),
      big = dango::uint32(__ORDER_BIG_ENDIAN__),
      native = dango::uint32(__BYTE_ORDER__)
  };
}

/*** sizeof_with_void alignof_with_void ***/

namespace
dango
{
  template
  <typename tp_type, dango::usize tp_default = dango::usize(1), typename tp_enabled = dango::enable_tag>
  constexpr dango::usize const sizeof_with_void = sizeof(tp_type);
  template
  <typename tp_type, dango::usize tp_default>
  constexpr dango::usize const sizeof_with_void<tp_type, tp_default, dango::enable_if<dango::is_void<tp_type>>> = tp_default;

  template
  <typename tp_type, dango::usize tp_default = dango::usize(1), typename tp_enabled = dango::enable_tag>
  constexpr dango::usize const alignof_with_void = alignof(tp_type);
  template
  <typename tp_type, dango::usize tp_default>
  constexpr dango::usize const alignof_with_void<tp_type, tp_default, dango::enable_if<dango::is_void<tp_type>>> = tp_default;
}

#endif

