#include "dango-array.hpp"
#include "dango-test-print.hpp"

template class dango::array<float, 4>;
template class dango::array<float, 0>;

static_assert(dango::array<char, 6>{ "hello" } == dango::array<char, 6>{ "hello" });
static_assert(dango::array<char, 6>{ "hello" } != dango::array<char, 3>{ "hi" });
static_assert(dango::array<char, 6>{ "hello" } != dango::array<char, 6>{ "hella" });
static_assert((dango::array<char, 6>{ "hello" } <=> dango::array<char, 6>{ "hello" }).is_eq());
static_assert(dango::get<4>(dango::array<char, 6>{ "hello" }) == 'o');

namespace
{

DANGO_UNIT_TEST_BEGIN(array_test1)
{
  for(auto& a_val : dango::array<char, 6>{ "hello" })
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
}
DANGO_UNIT_TEST_END

}
