#include "dango-concurrent.hpp"
#include "dango-test-print.hpp"

namespace
{

DANGO_UNIT_TEST_BEGIN(tick_count_test)
{
  using dango::integer::u_int;
  using dango::timeout_flags::HIGH_RES;

  auto const [a_tick1, a_bias1] = dango::tick_count_suspend_bias();

  dango_assert_terminate(a_tick1 >= dango::ms_tick_count(0) && a_bias1 >= dango::ms_tick_count(0));

  test_print("tick=%u bias=%u\n", u_int(dango::tick_count_ms(a_tick1)), u_int(dango::tick_count_ms(a_bias1)));

  dango::thread::sleep_rel(dango::ms_tick_count(100), HIGH_RES);

  auto const [a_tick2, a_bias2] = dango::tick_count_suspend_bias();

  dango_assert_terminate(a_tick2 >= a_tick1 + dango::ms_tick_count(100));
  dango_assert_terminate(a_bias2 >= a_bias1);

  test_print("tick=%u bias=%u\n", u_int(dango::tick_count_ms(a_tick2)), u_int(dango::tick_count_ms(a_bias2)));
}
DANGO_UNIT_TEST_END

DANGO_UNIT_TEST_BEGIN(tick_count_test_mt)
{
  using dango::integer::u_int;
  using dango::timeout_flags::HIGH_RES;

  auto const [a_tick1, a_bias1] = dango::tick_count_suspend_bias();

  dango_assert_terminate(dango::tick_count_ms(a_tick1) >= 0 && dango::tick_count_ms(a_bias1) >= 0);

  test_print("tick=%u bias=%u\n", u_int(dango::tick_count_ms(a_tick1)), u_int(dango::tick_count_ms(a_bias1)));

  dango::thread a_threads[10];

  auto a_id = u_int(0);

  for(auto& a_thread: a_threads)
  {
    a_thread =
    dango::thread::create_daemon
    (
      [a_id, a_tick1 = a_tick1, a_bias1 = a_bias1]()noexcept->void
      {
        auto const [a_tick2, a_bias2] = dango::tick_count_suspend_bias();

        dango_assert_terminate(a_tick2 >= a_tick1);
        dango_assert_terminate(a_bias2 >= a_bias1);

        test_print("id=%u tick=%u bias=%u\n", a_id, u_int(dango::tick_count_ms(a_tick2)), u_int(dango::tick_count_ms(a_bias2)));

        dango::thread::sleep_rel(dango::ms_tick_count(1'000), HIGH_RES);

        auto const [a_tick3, a_bias3] = dango::tick_count_suspend_bias();

        dango_assert_terminate(a_tick3 >= a_tick2 + dango::ms_tick_count(1'000));
        dango_assert_terminate(a_bias3 >= a_bias2);

        test_print("id=%u tick=%u bias=%u\n", a_id, u_int(dango::tick_count_ms(a_tick3)), u_int(dango::tick_count_ms(a_bias3)));
      }
    );

    ++a_id;
  }

  for(auto const& a_thread: a_threads)
  {
    a_thread.join();
  }

  auto const [a_tick2, a_bias2] = dango::tick_count_suspend_bias();

  dango_assert_terminate(a_tick2 >= a_tick1 + dango::ms_tick_count(1'000));
  dango_assert_terminate(a_bias2 >= a_bias1);

  test_print("tick=%u bias=%u\n", u_int(dango::tick_count_ms(a_tick2)), u_int(dango::tick_count_ms(a_bias2)));
}
DANGO_UNIT_TEST_END



}
