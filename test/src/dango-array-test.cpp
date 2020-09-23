#include "dango-array.hpp"
#include "dango-test-print.hpp"

template class dango::array<float, 4>;
template class dango::array<float, 0>;

static_assert(dango::array{ "hello" } == dango::array{ "hello" });
static_assert(dango::array{ "hello" } != dango::array{ "hi" });
static_assert(dango::array{ "hello" } != dango::array{ "hella" });
static_assert((dango::array{ "hello" } <=> dango::array{ "hello" }).is_eq());
static_assert((dango::array{ "hello" } <=> dango::array{ "hella" }).is_gt());
static_assert((dango::array{ "hello" } <=> dango::array{ "hellu" }).is_lt());
static_assert(dango::tuple_get<4>(dango::array{ "hello" }) == 'o');
static_assert(dango::array{ 1, 2, 3 } == dango::array{ 1, 2, 3 });
static_assert(dango::array{ 1, 2, 3 } < dango::array{ 1, 2, 4 });
static_assert(dango::array{ 1, 2, 3L, 4u } < dango::array{ 1, 2, 4 });
static_assert(dango::array{ "hello" }.size() == 6);
static_assert(sizeof(decltype(dango::array{ "hello" })) == 6);

namespace
{

DANGO_UNIT_TEST_BEGIN(array_test1)
{
  for(auto& a_val : dango::array{ "hello" })
  {
    test_print("%c", a_val);
  }

  test_print("\n");

  for(auto& a_val : dango::array<char, 0>{ })
  {
    test_print("%c", a_val);
  }

  dango::array<char, 6> a_array{ "hello" };

  dango::swap(a_array, a_array);

  auto const& [a, b, c, d, e, f] = a_array;

  dango_assert_terminate(a == 'h');
  dango_assert_terminate(b == 'e');
  dango_assert_terminate(c == 'l');
  dango_assert_terminate(d == 'l');
  dango_assert_terminate(e == 'o');
  dango_assert_terminate(f == '\0');
}
DANGO_UNIT_TEST_END

}
