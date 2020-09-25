#include "dango-tuple.hpp"
#include "dango-assert.hpp"
#include "dango-test-print.hpp"

namespace
{
  struct
  size_test_struct
  final
  {
    float first;
    dango::uint second;
    dango::ulong third;
    double fourth;
  };

  using size_test_tuple = dango::tuple<float, dango::uint, dango::ulong, double>;
}

static_assert(sizeof(size_test_struct) == sizeof(size_test_tuple));
static_assert(alignof(size_test_struct) == alignof(size_test_tuple));


static_assert(dango::is_same<decltype(dango::tuple<int, float>{ } <=> dango::tuple<int, float, dango::bchar>{ }), dango::compare_val_p>);
static_assert(dango::is_same<decltype(dango::tuple<int, dango::wchar>{ } <=> dango::tuple<int, dango::wchar, dango::bchar>{ }), dango::compare_val_s>);

static_assert(dango::make_tuple_from_array("abc") < dango::make_tuple_from_array("ac"));
static_assert(dango::make_tuple_from_array("abc") > dango::make_tuple_from_array("aa"));
static_assert(dango::make_tuple_from_array("abc") > dango::make_tuple_from_array("ab"));

static_assert(dango::tuple<>{ } == dango::tuple<>{ });
static_assert(dango::tuple<>{ } != dango::tuple<int>{ });

static_assert( dango::is_copy_constructible<dango::tuple<int&, float&, bool&>>);
static_assert( dango::is_move_constructible<dango::tuple<int&, float&, bool&>>);
static_assert( dango::is_trivial_copy_constructible<dango::tuple<int&, float&, bool&>>);
static_assert( dango::is_trivial_move_constructible<dango::tuple<int&, float&, bool&>>);
static_assert( dango::is_assignable<dango::tuple<int&, float&, bool&>&,        dango::tuple<int&, float&, bool&>&>);

static_assert( dango::is_swappable<dango::tuple<int, float, bool>&,           dango::tuple<int, float, bool>&>);
static_assert(!dango::is_swappable<dango::tuple<int, float, bool> const&,     dango::tuple<int, float, bool>&>);
static_assert(!dango::is_swappable<dango::tuple<int, float, bool>&,           dango::tuple<int, float, bool> const&>);
static_assert(!dango::is_swappable<dango::tuple<int, float, bool> const&,     dango::tuple<int, float, bool> const&>);
static_assert(!dango::is_swappable<dango::tuple<int, float, bool>&&,          dango::tuple<int, float, bool>&>);
static_assert(!dango::is_swappable<dango::tuple<int, float, bool>&,           dango::tuple<int, float, bool>&&>);
static_assert(!dango::is_swappable<dango::tuple<int, float, bool>&&,          dango::tuple<int, float, bool>&&>);

static_assert(!dango::is_swappable<dango::tuple<int&, float, bool>&,           dango::tuple<int&, float, bool>&>);
static_assert(!dango::is_swappable<dango::tuple<int&, float, bool> const&,     dango::tuple<int&, float, bool>&>);
static_assert(!dango::is_swappable<dango::tuple<int&, float, bool>&,           dango::tuple<int&, float, bool> const&>);
static_assert(!dango::is_swappable<dango::tuple<int&, float, bool> const&,     dango::tuple<int&, float, bool> const&>);
static_assert(!dango::is_swappable<dango::tuple<int&, float, bool>&&,          dango::tuple<int&, float, bool>&>);
static_assert(!dango::is_swappable<dango::tuple<int&, float, bool>&,           dango::tuple<int&, float, bool>&&>);
static_assert(!dango::is_swappable<dango::tuple<int&, float, bool>&&,          dango::tuple<int&, float, bool>&&>);

static_assert( dango::is_swappable<dango::tuple<int&, float&, bool&>&,        dango::tuple<int&, float&, bool&>&>);
static_assert( dango::is_swappable<dango::tuple<int&, float&, bool&> const&,  dango::tuple<int&, float&, bool&>&>);
static_assert( dango::is_swappable<dango::tuple<int&, float&, bool&>&,        dango::tuple<int&, float&, bool&> const&>);
static_assert( dango::is_swappable<dango::tuple<int&, float&, bool&> const&,  dango::tuple<int&, float&, bool&> const&>);
static_assert( dango::is_swappable<dango::tuple<int&, float&, bool&>&&,       dango::tuple<int&, float&, bool&>&>);
static_assert( dango::is_swappable<dango::tuple<int&, float&, bool&>&,        dango::tuple<int&, float&, bool&>&&>);
static_assert( dango::is_swappable<dango::tuple<int&, float&, bool&>&&,       dango::tuple<int&, float&, bool&>&&>);

static_assert( dango::has_tuple_size<dango::tuple<>>);
static_assert( dango::tuple_size<dango::tuple<>> == 0);
static_assert(!dango::has_tuple_elem<dango::tuple<>, 0>);
static_assert(!dango::has_tuple_get<dango::tuple<>, 0>);
static_assert( dango::is_tuple_like<dango::tuple<>>);

static_assert(dango::is_noexcept_brace_constructible<dango::tuple<double, double, double>, dango::tuple<float, float, float>&>);
static_assert(dango::is_noexcept_brace_constructible<dango::tuple<double, double, double>, dango::tuple<float, float, float> const&>);
static_assert(dango::is_noexcept_brace_constructible<dango::tuple<double, double, double>, dango::tuple<float, float, float>&&>);
static_assert(dango::is_noexcept_brace_constructible<dango::tuple<double, double, double>, dango::tuple<float, float, float> const&&>);

static_assert(dango::is_noexcept_assignable<dango::tuple<double, double, double>&, dango::tuple<float, float, float>&>);
static_assert(dango::is_noexcept_assignable<dango::tuple<double, double, double>&, dango::tuple<float, float, float> const&>);
static_assert(dango::is_noexcept_assignable<dango::tuple<double, double, double>&, dango::tuple<float, float, float>&&>);
static_assert(dango::is_noexcept_assignable<dango::tuple<double, double, double>&, dango::tuple<float, float, float> const&&>);

static_assert(dango::is_noexcept_brace_constructible<dango::tuple<dango::tuple<float>>, dango::tuple<float>&>);
static_assert(dango::is_noexcept_brace_constructible<dango::tuple<dango::tuple<float>>, dango::tuple<float> const&>);
static_assert(dango::is_noexcept_brace_constructible<dango::tuple<dango::tuple<float>>, dango::tuple<float>&&>);
static_assert(dango::is_noexcept_brace_constructible<dango::tuple<dango::tuple<float>>, dango::tuple<float> const&&>);

static_assert(dango::is_noexcept_brace_constructible<dango::tuple<dango::tuple<dango::tuple<float>>>, float const&>);
static_assert(dango::is_noexcept_assignable<dango::tuple<dango::tuple<dango::tuple<float>>>&, float const&>);

static_assert(dango::is_noexcept_brace_constructible<dango::tuple<dango::tuple<double>>, dango::tuple<float>&>);
static_assert(dango::is_noexcept_brace_constructible<dango::tuple<dango::tuple<double>>, dango::tuple<float> const&>);
static_assert(dango::is_noexcept_brace_constructible<dango::tuple<dango::tuple<double>>, dango::tuple<float>&&>);
static_assert(dango::is_noexcept_brace_constructible<dango::tuple<dango::tuple<double>>, dango::tuple<float> const&&>);

namespace
{
  struct unmoveable
  {
    explicit unmoveable()noexcept = default;
    constexpr ~unmoveable()noexcept = default;
    DANGO_UNMOVEABLE(unmoveable)
  };
}

static_assert(dango::is_noexcept_brace_constructible<dango::tuple<unmoveable>, decltype(dango::default_emplacer)>);

namespace
{

DANGO_UNIT_TEST_BEGIN(tuple_swap_test)
{
  int x = 5, y = 7, z = 9;
  int a = 4, b = 6, c = 8;

  auto const t1 = dango::tie_as_tuple(x, y, z);
  auto const t2 = dango::tie_as_tuple(a, b, c);

  test_print("%llu\n", dango::integer::u_longlong(dango::hash(t1).as_int()));
  test_print("%llu\n", dango::integer::u_longlong(dango::hash(t2).as_int()));

  dango_assert_terminate(dango::tuple_get<0>(t1) == 5);
  dango_assert_terminate(dango::tuple_get<1>(t1) == 7);
  dango_assert_terminate(dango::tuple_get<2>(t1) == 9);

  dango_assert_terminate(dango::tuple_get<0>(t2) == 4);
  dango_assert_terminate(dango::tuple_get<1>(t2) == 6);
  dango_assert_terminate(dango::tuple_get<2>(t2) == 8);

  dango::swap(t1, t2);

  dango_assert_terminate(x == 4 && y == 6 && z == 8);
  dango_assert_terminate(a == 5 && b == 7 && c == 9);

  dango::swap(dango::tie_as_tuple(z, y, x), dango::tie_as_tuple(a, b, c));

  dango_assert_terminate(x == 9 && y == 7 && z == 5);
  dango_assert_terminate(a == 8 && b == 6 && c == 4);
}
DANGO_UNIT_TEST_END

#define dango_offset(s, m) \
  dango::usize(reinterpret_cast<dango::byte const*>(dango::addressof(s.m)) - reinterpret_cast<dango::byte const*>(dango::addressof(s)))

DANGO_UNIT_TEST_BEGIN(tuple_offset_test)
{
  size_test_struct a_s;

  size_test_tuple a_t;

  dango_assert_terminate(dango_offset(a_s, first)  == dango_offset(a_t, first()));
  dango_assert_terminate(dango_offset(a_s, second) == dango_offset(a_t, second()));
  dango_assert_terminate(dango_offset(a_s, third)  == dango_offset(a_t, third()));
  dango_assert_terminate(dango_offset(a_s, fourth) == dango_offset(a_t, fourth()));
}
DANGO_UNIT_TEST_END

#undef dango_offset

}
