#include "dango.hpp"
#include "dango-test-print.hpp"

namespace
{

DANGO_UNIT_TEST_BEGIN(tick_count_test)
{
  using dango::timeout_flags::HIGH_RES;

  auto const [a_count1, a_bias1] = dango::tick_count_suspend_bias();

  dango_assert_terminate(a_count1 >= 0 && a_bias1 >= 0);

  test_print("count=%u bias=%u\n", uint(a_count1), uint(a_bias1));

  dango::thread::sleep_rel(100, HIGH_RES);

  auto const [a_count2, a_bias2] = dango::tick_count_suspend_bias();

  dango_assert_terminate(a_count2 >= a_count1 + 100);
  dango_assert_terminate(a_bias2 >= a_bias1);

  test_print("count=%u bias=%u\n", uint(a_count2), uint(a_bias2));
}
DANGO_UNIT_TEST_END

}

