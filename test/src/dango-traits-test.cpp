#include "dango-traits.hpp"
#include "dango-tuple.hpp"

/*** is_same ***/

static_assert(dango::is_same<int, int>);
static_assert(dango::is_same<float, float>);
static_assert(!dango::is_same<int, int const>);
static_assert(!dango::is_same<int volatile, int>);
static_assert(!dango::is_same<int&, int>);
static_assert(!dango::is_same<bool, bool*>);

/*** remove_const ***/

static_assert(dango::is_same<dango::remove_const<int>, int>);
static_assert(dango::is_same<dango::remove_const<int const>, int>);
static_assert(dango::is_same<dango::remove_const<int volatile>, int volatile>);
static_assert(dango::is_same<dango::remove_const<int const volatile>, int volatile>);

/*** remove_volatile ***/

static_assert(dango::is_same<dango::remove_volatile<int>, int>);
static_assert(dango::is_same<dango::remove_volatile<int const>, int const>);
static_assert(dango::is_same<dango::remove_volatile<int volatile>, int>);
static_assert(dango::is_same<dango::remove_volatile<int const volatile>, int const>);

/*** remove_cv ***/

static_assert(dango::is_same<dango::remove_cv<int>, int>);
static_assert(dango::is_same<dango::remove_cv<int const>, int>);
static_assert(dango::is_same<dango::remove_cv<int volatile>, int>);
static_assert(dango::is_same<dango::remove_cv<int const volatile>, int>);

/*** remove_ref ***/

static_assert(dango::is_same<dango::remove_ref<int>, int>);
static_assert(dango::is_same<dango::remove_ref<int&>, int>);
static_assert(dango::is_same<dango::remove_ref<int&&>, int>);
static_assert(dango::is_same<dango::remove_ref<int const&>, int const>);
static_assert(dango::is_same<dango::remove_ref<int volatile&>, int volatile>);
static_assert(dango::is_same<dango::remove_ref<int const volatile&&>, int const volatile>);

/*** remove_cvref ***/

static_assert(dango::is_same<dango::remove_cvref<int>, int>);
static_assert(dango::is_same<dango::remove_cvref<int&>, int>);
static_assert(dango::is_same<dango::remove_cvref<int&&>, int>);
static_assert(dango::is_same<dango::remove_cvref<int const&>, int>);
static_assert(dango::is_same<dango::remove_cvref<int volatile&>, int>);
static_assert(dango::is_same<dango::remove_cvref<int const volatile&&>, int>);

/*** remove_ptr ***/

static_assert(dango::is_same<dango::remove_ptr<int>, int>);
static_assert(dango::is_same<dango::remove_ptr<int*>, int>);
static_assert(dango::is_same<dango::remove_ptr<int***>, int**>);
static_assert(dango::is_same<dango::remove_ptr<int&&>, int&&>);
static_assert(dango::is_same<dango::remove_ptr<int const*>, int const>);
static_assert(dango::is_same<dango::remove_ptr<int volatile*>, int volatile>);
static_assert(dango::is_same<dango::remove_ptr<int const volatile*>, int const volatile>);
static_assert(dango::is_same<dango::remove_ptr<int const volatile* const* volatile>, int const volatile* const>);

/*** remove_all_ptr ***/

static_assert(dango::is_same<dango::remove_all_ptr<int>, int>);
static_assert(dango::is_same<dango::remove_all_ptr<int*>, int>);
static_assert(dango::is_same<dango::remove_all_ptr<int******* const************>, int>);
static_assert(dango::is_same<dango::remove_all_ptr<int const* volatile* const* volatile* const* volatile* const>, int const>);
static_assert(dango::is_same<dango::remove_all_ptr<int const volatile* const volatile* const volatile* const volatile>, int const volatile>);

/*** remove_array ***/

static_assert(dango::is_same<dango::remove_array<int>, int>);
static_assert(dango::is_same<dango::remove_array<int[4]>, int>);
static_assert(dango::is_same<dango::remove_array<int[]>, int>);
static_assert(dango::is_same<dango::remove_array<int const[4]>, int const>);
static_assert(dango::is_same<dango::remove_array<int const[3][4][5]>, int const[4][5]>);
static_assert(dango::is_same<dango::remove_array<int const*[4]>, int const*>);
static_assert(dango::is_same<dango::remove_array<int const* const volatile[4]>, int const* const volatile>);
static_assert(dango::is_same<dango::remove_array<int[][5]>, int[5]>);

/*** remove_all_array ***/

static_assert(dango::is_same<dango::remove_all_array<int>, int>);
static_assert(dango::is_same<dango::remove_all_array<int[4]>, int>);
static_assert(dango::is_same<dango::remove_all_array<int[]>, int>);
static_assert(dango::is_same<dango::remove_all_array<int const[4]>, int const>);
static_assert(dango::is_same<dango::remove_all_array<int const[3][4][5]>, int const>);
static_assert(dango::is_same<dango::remove_all_array<int const volatile[1][2][3][4][5][6][7][8][9]>, int const volatile>);
static_assert(dango::is_same<dango::remove_all_array<int const*[4]>, int const*>);
static_assert(dango::is_same<dango::remove_all_array<int[][1][2][3][4][5][6][7][8][9]>, int>);

/*** copy_cv ***/

static_assert(dango::is_same<dango::copy_cv<int, bool>, bool>);
static_assert(dango::is_same<dango::copy_cv<int const, bool>, bool const>);
static_assert(dango::is_same<dango::copy_cv<int volatile, bool>, bool volatile>);
static_assert(dango::is_same<dango::copy_cv<int const volatile, bool>, bool const volatile>);
static_assert(dango::is_same<dango::copy_cv<int, bool const volatile>, bool const volatile>);

/*** decay ***/

static_assert(dango::is_same<dango::decay<int>, int>);
static_assert(dango::is_same<dango::decay<int const>, int>);
static_assert(dango::is_same<dango::decay<int volatile>, int>);
static_assert(dango::is_same<dango::decay<int const volatile>, int>);
static_assert(dango::is_same<dango::decay<int&>, int>);
static_assert(dango::is_same<dango::decay<int const&&>, int>);
static_assert(dango::is_same<dango::decay<int const[]>, int const*>);
static_assert(dango::is_same<dango::decay<int volatile[4]>, int volatile*>);
static_assert(dango::is_same<dango::decay<int[][4]>, int(*)[4]>);
static_assert(dango::is_same<dango::decay<int const[3][4][5]>, int const(*)[4][5]>);
static_assert(dango::is_same<dango::decay<void(float, bool)noexcept(false)>, void(*)(float, bool)noexcept(false)>);
static_assert(dango::is_same<dango::decay<int(&)(float, bool)noexcept(false)>, int(*)(float, bool)noexcept(false)>);
static_assert(dango::is_same<dango::decay<double const(&&)[3][4][5]>, double const(*)[4][5]>);

/*** common_type ***/

static_assert(dango::is_same<dango::common_type<int>, int>);
static_assert(dango::is_same<dango::common_type<int, int>, int>);
static_assert(dango::is_same<dango::common_type<int, int, int>, int>);
static_assert(dango::is_same<dango::common_type<int, long, int, int>, long>);
static_assert(dango::is_same<dango::common_type<int, short, int, int>, int>);
static_assert(dango::is_same<dango::common_type<int, int, unsigned, int>, unsigned>);
static_assert(dango::is_same<dango::common_type<int*, int const*, int volatile*, int const volatile*, void const volatile*>, void const volatile*>);
static_assert(!dango::has_common_type<int, float, int*>);

/*** contitional ***/

static_assert(dango::is_same<dango::conditional<false, int, bool>, bool>);
static_assert(dango::is_same<dango::conditional<true, int, bool>, int>);

/*** is_same_ignore_cv ***/

static_assert(dango::is_same_ignore_cv<int, int>);
static_assert(dango::is_same_ignore_cv<float, float>);
static_assert(dango::is_same_ignore_cv<int, int const>);
static_assert(dango::is_same_ignore_cv<int volatile, int>);
static_assert(!dango::is_same_ignore_cv<int&, int>);
static_assert(!dango::is_same_ignore_cv<bool, bool*>);

/*** is_same_ignore_cvref ***/

static_assert(dango::is_same_ignore_cvref<int, int>);
static_assert(dango::is_same_ignore_cvref<float, float>);
static_assert(dango::is_same_ignore_cvref<int, int const>);
static_assert(dango::is_same_ignore_cvref<int volatile, int>);
static_assert(dango::is_same_ignore_cvref<int&, int>);
static_assert(dango::is_same_ignore_cvref<int const&, int volatile&&>);
static_assert(!dango::is_same_ignore_cvref<bool, bool*>);

/*** is_void ***/

static_assert(dango::is_void<void>);
static_assert(dango::is_void<void const>);
static_assert(dango::is_void<void volatile>);
static_assert(dango::is_void<void const volatile>);

/*** is_null_tag ***/

static_assert(dango::is_null_tag<decltype(nullptr)>);
static_assert(dango::is_null_tag<decltype(nullptr) const>);
static_assert(dango::is_null_tag<decltype(nullptr) volatile>);
static_assert(dango::is_null_tag<decltype(nullptr) const volatile>);

/*** is_bool ***/

static_assert(dango::is_bool<bool>);
static_assert(dango::is_bool<bool const>);
static_assert(dango::is_bool<bool volatile>);
static_assert(dango::is_bool<bool const volatile>);

/*** declval ***/

static_assert(dango::is_same<decltype(dango::declval<int>()), int&&>);
static_assert(dango::is_same<decltype(dango::declval<int const>()), int const&&>);
static_assert(dango::is_same<decltype(dango::declval<int&>()), int&>);
static_assert(dango::is_same<decltype(dango::declval<int const&>()), int const&>);
static_assert(dango::is_same<decltype(dango::declval<int&&>()), int&&>);
static_assert(dango::is_same<decltype(dango::declval<int const&&>()), int const&&>);

/*** is_uint is_sint is_int ***/

static_assert(dango::is_uint<dango::ubyte>);
static_assert(dango::is_uint<dango::ushort const>);
static_assert(dango::is_uint<dango::uint volatile>);
static_assert(dango::is_uint<dango::ulong const volatile>);
static_assert(!dango::is_uint<dango::sbyte>);
static_assert(!dango::is_uint<dango::sshort const>);
static_assert(!dango::is_uint<dango::sint volatile>);
static_assert(!dango::is_uint<dango::slong const volatile>);

static_assert(dango::is_sint<dango::sbyte>);
static_assert(dango::is_sint<dango::sshort const>);
static_assert(dango::is_sint<dango::sint volatile>);
static_assert(dango::is_sint<dango::slong const volatile>);
static_assert(!dango::is_sint<dango::ubyte>);
static_assert(!dango::is_sint<dango::ushort const>);
static_assert(!dango::is_sint<dango::uint volatile>);
static_assert(!dango::is_sint<dango::ulong const volatile>);

static_assert(dango::is_int<dango::integer::u_longlong volatile>);
static_assert(dango::is_int<dango::integer::s_long const>);
static_assert(!dango::is_int<char32_t>);
static_assert(!dango::is_int<double>);

/*** is_char ***/

static_assert(dango::is_char<char>);
static_assert(dango::is_char<wchar_t const>);
static_assert(dango::is_char<char8_t volatile>);
static_assert(dango::is_char<char16_t const volatile>);
static_assert(dango::is_char<char32_t>);

static_assert(!dango::is_char<signed char>);
static_assert(!dango::is_char<unsigned char>);

/*** is_integral_exclude_bool ***/

static_assert(dango::is_integral_exclude_bool<int const>);
static_assert(!dango::is_integral_exclude_bool<bool volatile>);

/*** is_integral ***/

static_assert(dango::is_integral<long volatile>);
static_assert(dango::is_integral<bool>);
static_assert(!dango::is_integral<float const>);

/*** is_float ***/

static_assert(dango::is_float<float>);
static_assert(dango::is_float<double const>);
static_assert(dango::is_float<long double volatile>);
static_assert(!dango::is_float<int>);

/*** is_num ***/

static_assert(dango::is_num<float volatile>);
static_assert(dango::is_num<int const>);
static_assert(!dango::is_num<char16_t>);
static_assert(!dango::is_num<bool>);

/*** is_array is_array_unknown_bound, is_array_known_bound ***/

static_assert(!dango::is_array<float const>);
static_assert(dango::is_array<float[]>);
static_assert(dango::is_array<float volatile[4][4][5]>);
static_assert(dango::is_array_unknown_bound<bool[][5][6]>);
static_assert(!dango::is_array_unknown_bound<bool[4][5][6]>);
static_assert(!dango::is_array_known_bound<bool[][5][6]>);
static_assert(dango::is_array_known_bound<bool[4][5][6]>);

/*** array_rank ***/

static_assert(dango::array_rank<int> == dango::usize(0));
static_assert(dango::array_rank<int const[]> == dango::usize(1));
static_assert(dango::array_rank<int volatile[4]> == dango::usize(1));
static_assert(dango::array_rank<int const volatile[][3]> == dango::usize(2));
static_assert(dango::array_rank<int const volatile[4][3]> == dango::usize(2));
static_assert(dango::array_rank<float[1][2][3][4][5][6][7][8][9][10]> == dango::usize(10));

/*** array_size ***/

static_assert(dango::array_size<int> == dango::usize(0));
static_assert(dango::array_size<int[]> == dango::usize(0));
static_assert(dango::array_size<int[][3][4][5], 0> == dango::usize(0));
static_assert(dango::array_size<int[3][4][5], 0> == dango::usize(3));
static_assert(dango::array_size<int[3][4][5], 1> == dango::usize(4));
static_assert(dango::array_size<int[3][4][5], 2> == dango::usize(5));

namespace
{
  enum test_enum:dango::ulong{ };
  enum class test_enum_class:dango::uint{ };
  class test_class{ };
  struct test_struct{ };
  union test_union{ };
  struct test_incomplete_struct;
  union test_incomplete_union;
}

/*** is_enum ***/

static_assert(dango::is_enum<test_enum>);
static_assert(dango::is_enum<test_enum const>);
static_assert(dango::is_enum<test_enum_class volatile>);
static_assert(dango::is_enum<test_enum_class const volatile>);
static_assert(!dango::is_enum<test_class>);
static_assert(!dango::is_enum<test_struct>);
static_assert(!dango::is_enum<test_union>);
static_assert(!dango::is_enum<test_incomplete_struct>);
static_assert(!dango::is_enum<int>);

/*** is_union ***/

static_assert(dango::is_union<test_union const>);
static_assert(dango::is_union<test_incomplete_union const volatile>);
static_assert(!dango::is_union<test_class const>);
static_assert(!dango::is_union<test_struct volatile>);
static_assert(!dango::is_union<test_incomplete_struct const volatile>);
static_assert(!dango::is_union<test_enum>);
static_assert(!dango::is_union<int&>);

/*** is_class ***/

static_assert(dango::is_class<test_class const>);
static_assert(dango::is_class<test_struct volatile>);
static_assert(dango::is_class<test_incomplete_struct const volatile>);
static_assert(!dango::is_class<test_enum>);
static_assert(!dango::is_class<test_union>);
static_assert(!dango::is_class<int*>);

/*** is_class_or_union ***/

static_assert(dango::is_class_or_union<test_union>);
static_assert(dango::is_class_or_union<test_incomplete_union>);
static_assert(dango::is_class_or_union<test_class>);
static_assert(dango::is_class_or_union<test_struct>);
static_assert(dango::is_class_or_union<test_incomplete_struct>);
static_assert(!dango::is_class_or_union<test_enum_class>);
static_assert(!dango::is_class_or_union<test_class[4]>);
static_assert(!dango::is_class_or_union<int>);

/*** is_user_defined ***/

static_assert(dango::is_user_defined<test_union>);
static_assert(dango::is_user_defined<test_incomplete_union>);
static_assert(dango::is_user_defined<test_class>);
static_assert(dango::is_user_defined<test_struct>);
static_assert(dango::is_user_defined<test_incomplete_struct>);
static_assert(dango::is_user_defined<test_enum>);
static_assert(dango::is_user_defined<test_enum_class>);
static_assert(!dango::is_user_defined<test_class[4]>);
static_assert(!dango::is_user_defined<int>);

/*** is_lvalue_ref is_rvalue_ref is_ref ***/

static_assert(dango::is_lvalue_ref<int const&>);
static_assert(!dango::is_lvalue_ref<int>);
static_assert(!dango::is_lvalue_ref<int&&>);
static_assert(dango::is_rvalue_ref<int volatile&&>);
static_assert(!dango::is_rvalue_ref<int const>);
static_assert(!dango::is_rvalue_ref<int&>);
static_assert(dango::is_ref<int&>);
static_assert(dango::is_ref<int&&>);
static_assert(dango::is_ref<int const&>);
static_assert(dango::is_ref<int const&&>);
static_assert(!dango::is_ref<int const volatile>);

/*** is_func is_qualified_func is_unqualified_func ***/

static_assert(dango::is_func<void(int, bool)noexcept(true)>);
static_assert(!dango::is_func<void(&)(int, bool)noexcept(true)>);
static_assert(!dango::is_func<void(*)(int, bool)noexcept(true)>);
static_assert(dango::is_unqualified_func<int(int, bool)noexcept(false)>);
static_assert(!dango::is_unqualified_func<int(int, bool)const noexcept(false)>);
static_assert(!dango::is_unqualified_func<int(int, bool)&>);
static_assert(dango::is_qualified_func<int(int, bool)const&>);
static_assert(dango::is_qualified_func<int(int, bool)volatile&& noexcept>);
static_assert(!dango::is_qualified_func<int(int, bool)>);
static_assert(!dango::is_qualified_func<int(int, bool)noexcept>);
static_assert(!dango::is_func<int&>);
static_assert(!dango::is_func<bool*>);

/*** is_ptr is_object_ptr is_void_ptr is_func_ptr ***/

static_assert(!dango::is_ptr<int&&>);
static_assert(dango::is_ptr<int* const>);
static_assert(dango::is_object_ptr<int* const>);
static_assert(dango::is_void_ptr<void const volatile* const volatile>);
static_assert(!dango::is_void_ptr<void const volatile* const volatile&>);
static_assert(!dango::is_func_ptr<void(&)(int, bool)noexcept(true)>);
static_assert(dango::is_func_ptr<void(*)(int, bool)noexcept(true)>);

namespace
{
  struct
  test_class_with_member
  {
    int m_int;
    auto get_int()const noexcept->int const&{ return m_int; }
  };
}

/*** is_member_ptr is_member_func_ptr is_member_object_ptr ***/

static_assert(dango::is_member_ptr<decltype(&test_class_with_member::m_int)>);
static_assert(dango::is_member_object_ptr<decltype(&test_class_with_member::m_int)>);
static_assert(!dango::is_member_func_ptr<decltype(&test_class_with_member::m_int)>);

static_assert(dango::is_member_ptr<decltype(&test_class_with_member::get_int)>);
static_assert(!dango::is_member_object_ptr<decltype(&test_class_with_member::get_int)>);
static_assert(dango::is_member_func_ptr<decltype(&test_class_with_member::get_int)>);

/*** is_arithmetic is_arithmetic_exclude_bool ***/

static_assert(dango::is_arithmetic_exclude_bool<float>);
static_assert(dango::is_arithmetic_exclude_bool<int const>);
static_assert(dango::is_arithmetic_exclude_bool<char16_t volatile>);
static_assert(!dango::is_arithmetic_exclude_bool<bool>);

static_assert(dango::is_arithmetic<double const>);
static_assert(dango::is_arithmetic<unsigned long>);
static_assert(dango::is_arithmetic<char8_t const volatile>);
static_assert(dango::is_arithmetic<bool volatile>);

/*** is_fundamental is_compound ***/

static_assert(dango::is_fundamental<double const>);
static_assert(dango::is_fundamental<void volatile>);
static_assert(dango::is_fundamental<decltype(nullptr)>);
static_assert(!dango::is_fundamental<int*>);

static_assert(!dango::is_compound<double>);
static_assert(!dango::is_compound<void>);
static_assert(!dango::is_compound<decltype(nullptr) const>);
static_assert(dango::is_compound<int[2][3][4]>);
static_assert(dango::is_compound<float&&>);

/*** is_scalar ***/

static_assert(dango::is_scalar<int volatile>);
static_assert(dango::is_scalar<int*>);
static_assert(dango::is_scalar<test_enum_class const>);
static_assert(dango::is_scalar<test_enum>);
static_assert(dango::is_scalar<decltype(nullptr) volatile>);
static_assert(dango::is_scalar<decltype(&test_class_with_member::m_int)>);
static_assert(dango::is_scalar<decltype(&test_class_with_member::get_int)>);
static_assert(dango::is_scalar<double(*)()noexcept>);
static_assert(!dango::is_scalar<double()noexcept>);
static_assert(!dango::is_scalar<double(&)()noexcept>);

/*** is_object is_object_exclude_array ***/

static_assert(dango::is_object<test_class const>);
static_assert(dango::is_object<test_union[4]>);
static_assert(!dango::is_object<int(float)&>);
static_assert(!dango::is_object<bool const&&>);
static_assert(dango::is_object<char>);
static_assert(!dango::is_object_exclude_array<int[2][3][4]>);
static_assert(!dango::is_object_exclude_array<int[][2][3][4]>);
static_assert(dango::is_object_exclude_array<int(*)[2][3][4]>);

/*** is_referenceable ***/

static_assert(dango::is_referenceable<int>);
static_assert(!dango::is_referenceable<int&>);
static_assert(!dango::is_referenceable<int&&>);
static_assert(dango::is_referenceable<int(float)noexcept>);
static_assert(!dango::is_referenceable<void()&>);

/*** is_const is_volatile is_const_or_volatile is_const_and_volatile ***/

static_assert(!dango::is_const_or_volatile<int>);
static_assert(dango::is_const_and_volatile<int const volatile>);
static_assert(!dango::is_const_and_volatile<int const>);
static_assert(dango::is_const<float const>);
static_assert(!dango::is_const<float>);
static_assert(dango::is_volatile<int const volatile>);
static_assert(!dango::is_volatile<int const>);

/*** is_trivial is_trivial_copyable ***/

static_assert(dango::is_trivial<int>);
static_assert(dango::is_trivial<test_struct>);
static_assert(dango::is_trivial_copyable<int>);
static_assert(dango::is_trivial_copyable<test_struct>);
static_assert(!dango::is_trivial<int&>);
static_assert(!dango::is_trivial_copyable<int&>);
static_assert(dango::is_trivial<dango::tuple<int, bool, dango::tuple<float, long, dango::tuple<double, char>, unsigned>, test_struct, long double>>);
static_assert(dango::is_trivial_copyable<dango::tuple<int, bool, dango::tuple<float, long, dango::tuple<double, char>, unsigned>, test_struct, long double>>);

/*** is_standard_layout ***/

static_assert(dango::is_standard_layout<int>);
static_assert(dango::is_standard_layout<test_struct>);
static_assert(dango::is_standard_layout<dango::tuple<>>);
static_assert(dango::is_standard_layout<dango::tuple<int>>);
static_assert(!dango::is_standard_layout<dango::tuple<int, float>>);

/*** is_empty ***/

static_assert(dango::is_empty<test_struct>);
static_assert(dango::is_empty<dango::tuple<>>);
static_assert(!dango::is_empty<int>);
static_assert(!dango::is_empty<dango::tuple<int>>);

/*** is_signed is_unsigned ***/

static_assert(dango::is_signed<dango::sint>);
static_assert(dango::is_unsigned<dango::uint>);
static_assert(dango::is_unsigned<bool>);

/*** is_destructible ***/

static_assert(dango::is_destructible<int>);
static_assert(dango::is_destructible<int&>);

/*** is_destructible ***/

namespace
{
  struct unconstructible{ DANGO_UNCONSTRUCTIBLE(unconstructible) };
  class private_destructor{ ~private_destructor()noexcept = default; };
}

static_assert(!dango::is_destructible<void>);
static_assert(!dango::is_destructible<int(float, bool)noexcept>);
static_assert(dango::is_destructible<int const>);
static_assert(!dango::is_destructible<unconstructible>);
static_assert(!dango::is_destructible<private_destructor>);
static_assert(dango::is_destructible<int&>);
static_assert(dango::is_destructible<int&&>);
static_assert(dango::is_noexcept_destructible<int&>);
static_assert(dango::is_noexcept_destructible<int&&>);
static_assert(dango::is_trivial_destructible<int&>);
static_assert(dango::is_trivial_destructible<int&&>);
static_assert(!dango::is_destructible<int[]>);
static_assert(dango::is_destructible<int[4]>);
static_assert(dango::is_destructible<int[3][4][5]>);

/*** is_convertible ***/

namespace
{
  struct
  unmoveable
  {
    constexpr unmoveable()noexcept = default;
    DANGO_UNMOVEABLE(unmoveable)
    constexpr ~unmoveable()noexcept = default;
  };
}

static_assert(!dango::is_convertible_arg<void const, void>);
static_assert(dango::is_convertible_ret<void, void volatile>);
static_assert(dango::is_convertible_arg<int*, int*>);
static_assert(dango::is_convertible_ret<int*, int*>);
static_assert(dango::is_convertible_arg<int const&, int>);
static_assert(dango::is_convertible_ret<int&&, int>);
static_assert(!dango::is_convertible_arg<unmoveable, unmoveable>);
static_assert(dango::is_convertible_ret<unmoveable, unmoveable>);
static_assert(dango::is_convertible_arg<void(int, bool)noexcept, void(*)(int, bool)noexcept(false)>);
static_assert(dango::is_convertible_ret<void(int, bool)noexcept, void(*)(int, bool)noexcept(false)>);

/*** is_callable ***/

static_assert(dango::is_callable<unmoveable(int, float)noexcept, int, float>);
static_assert(dango::is_noexcept_callable<unmoveable(int, float)noexcept, int, float>);
static_assert(dango::is_callable_ret<unmoveable, unmoveable(int, float)noexcept, int, float>);
static_assert(dango::is_noexcept_callable_ret<unmoveable, unmoveable(int, float)noexcept, int, float>);

/*** is_constructible ***/

static_assert(dango::is_constructible<int>);
static_assert(dango::is_constructible<int&, int&>);
static_assert(!dango::is_constructible<unconstructible>);
static_assert(dango::is_noexcept_constructible<unmoveable>);
static_assert(dango::is_trivial_constructible<unmoveable>);
static_assert(!dango::is_constructible<int[]>);
static_assert(dango::is_constructible<int[4]>);
static_assert(!dango::is_constructible<int[4], int, int, int, int, int>);
static_assert(!dango::is_copy_constructible<unmoveable>);
static_assert(!dango::is_move_constructible<unmoveable>);

/*** is_brace_constructible ***/

static_assert(dango::is_brace_constructible<int>);
static_assert(dango::is_brace_constructible<int&, int&>);
static_assert(!dango::is_brace_constructible<unconstructible>);
static_assert(dango::is_noexcept_brace_constructible<unmoveable>);
static_assert(!dango::is_brace_constructible<int[]>);
static_assert(dango::is_brace_constructible<int[4]>);
static_assert(!dango::is_brace_constructible<int[4], int, int, int, int, int>);

/*** is_assignable ***/

static_assert(dango::is_assignable<int&, int&>);
static_assert(dango::is_assignable<int&, int const&>);
static_assert(dango::is_noexcept_assignable<int&, int&>);
static_assert(dango::is_noexcept_assignable<int&, int const&>);
static_assert(dango::is_trivial_assignable<int&, int&>);
static_assert(dango::is_trivial_assignable<int&, int const&>);
static_assert(!dango::is_assignable<unmoveable&, unmoveable&>);
static_assert(!dango::is_assignable<unmoveable&, unmoveable const&>);
static_assert(!dango::is_assignable<unmoveable&, unmoveable&&>);
static_assert(!dango::is_assignable<unmoveable&, unmoveable const&&>);

/*** underlying_type ***/

static_assert(dango::is_same<dango::underlying_type<test_enum_class>, dango::uint>);
static_assert(dango::is_same<dango::underlying_type<test_enum_class const>, dango::uint>);

/*** make_uint make_sint ***/

static_assert(sizeof(dango::integer::s_char)     == sizeof(dango::make_uint<dango::integer::s_char>));
static_assert(sizeof(dango::integer::s_short)    == sizeof(dango::make_uint<dango::integer::s_short>));
static_assert(sizeof(dango::integer::s_int)      == sizeof(dango::make_uint<dango::integer::s_int>));
static_assert(sizeof(dango::integer::s_long)     == sizeof(dango::make_uint<dango::integer::s_long>));
static_assert(sizeof(dango::integer::s_longlong) == sizeof(dango::make_uint<dango::integer::s_longlong>));
static_assert(sizeof(char)                       == sizeof(dango::make_uint<char>));
static_assert(sizeof(wchar_t)                    == sizeof(dango::make_uint<wchar_t>));
static_assert(sizeof(char8_t)                    == sizeof(dango::make_uint<char8_t>));
static_assert(sizeof(char16_t)                   == sizeof(dango::make_uint<char16_t>));
static_assert(sizeof(char32_t)                   == sizeof(dango::make_uint<char32_t>));
static_assert(sizeof(bool)                       == sizeof(dango::make_uint<bool>));

static_assert(sizeof(dango::integer::u_char)     == sizeof(dango::make_sint<dango::integer::u_char>));
static_assert(sizeof(dango::integer::u_short)    == sizeof(dango::make_sint<dango::integer::u_short>));
static_assert(sizeof(dango::integer::u_int)      == sizeof(dango::make_sint<dango::integer::u_int>));
static_assert(sizeof(dango::integer::u_long)     == sizeof(dango::make_sint<dango::integer::u_long>));
static_assert(sizeof(dango::integer::u_longlong) == sizeof(dango::make_sint<dango::integer::u_longlong>));
static_assert(sizeof(char)                       == sizeof(dango::make_sint<char>));
static_assert(sizeof(wchar_t)                    == sizeof(dango::make_sint<wchar_t>));
static_assert(sizeof(char8_t)                    == sizeof(dango::make_sint<char8_t>));
static_assert(sizeof(char16_t)                   == sizeof(dango::make_sint<char16_t>));
static_assert(sizeof(char32_t)                   == sizeof(dango::make_sint<char32_t>));
static_assert(sizeof(bool)                       == sizeof(dango::make_sint<bool>));

static_assert(dango::is_same<dango::make_uint<float>, float>);
static_assert(dango::is_same<dango::make_sint<int*>, int*>);
static_assert(dango::is_same<dango::make_sint<dango::ulong>, dango::slong>);
static_assert(dango::is_same<dango::make_uint<dango::sint>, dango::uint>);
static_assert(dango::is_same<dango::make_sint<dango::ushort> const, dango::sshort const>);
static_assert(dango::is_same<dango::make_uint<dango::sbyte volatile>, dango::ubyte volatile>);
static_assert(dango::is_same<dango::make_uint<test_enum_class const>, dango::uint const>);
static_assert(dango::is_same<dango::make_sint<test_enum_class const>, dango::sint const>);

/*** bit_width ***/

static_assert(dango::bit_width<dango::ubyte>  == 8);
static_assert(dango::bit_width<dango::sbyte const>  == 8);
static_assert(dango::bit_width<dango::ushort volatile> == 16);
static_assert(dango::bit_width<dango::sshort const volatile> == 16);
static_assert(dango::bit_width<dango::uint>   == 32);
static_assert(dango::bit_width<dango::sint const>   == 32);
static_assert(dango::bit_width<dango::ulong volatile>  == 64);
static_assert(dango::bit_width<dango::slong const volatile>  == 64);
