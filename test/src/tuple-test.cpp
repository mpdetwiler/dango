#include "dango.hpp"

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


static_assert(dango::is_same<decltype(dango::tuple<int, float>{ } <=> dango::tuple<int, float, dango::bchar>{ }), dango::compare_val_partial>);
static_assert(dango::is_same<decltype(dango::tuple<int, dango::wchar>{ } <=> dango::tuple<int, dango::wchar, dango::bchar>{ }), dango::compare_val>);

static_assert(dango::tuple<char8_t, char8_t, char8_t>{ u8'a', u8'b', u8'c' } < dango::tuple<char8_t, char8_t>{ u8'a', u8'c' });
static_assert(dango::tuple<char8_t, char8_t>{ u8'a', u8'c' } > dango::tuple<char8_t, char8_t, char8_t>{ u8'a', u8'b', u8'c' });

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

namespace
tuple_test
{
  void test();

  void
  tuple_swap_test()
  {
    int x = 5, y = 7, z = 9;
    int a = 4, b = 6, c = 8;

    auto const t1 = dango::make_tie(x, y, z);
    auto const t2 = dango::make_tie(a, b, c);

    dango::swap(t1, t2);

    dango_assert(x == 4 && y == 6 && z == 8);
    dango_assert(a == 5 && b == 7 && c == 9);

    dango::swap(dango::make_tie(z, y, x), dango::make_tie(a, b, c));

    dango_assert(x == 9 && y == 7 && z == 5);
    dango_assert(a == 8 && b == 6 && c == 4);
  }

#define dango_offset(s, m) \
  dango::usize(reinterpret_cast<dango::byte const*>(dango::address_of(s.m)) - reinterpret_cast<dango::byte const*>(dango::address_of(s)))

  void
  tuple_offset_test()
  {
    size_test_struct a_s;

    size_test_tuple a_t;

    dango_assert(dango_offset(a_s, first)  == dango_offset(a_t, first()));
    dango_assert(dango_offset(a_s, second) == dango_offset(a_t, second()));
    dango_assert(dango_offset(a_s, third)  == dango_offset(a_t, third()));
    dango_assert(dango_offset(a_s, fourth) == dango_offset(a_t, fourth()));
  }

#undef dango_offset
}

void
tuple_test::
test()
{
  tuple_swap_test();
  tuple_offset_test();
}

