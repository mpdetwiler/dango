#include "dango-concurrent.hpp"
#include "dango-test-print.hpp"
#include "dango-thread.hpp"

namespace
{

DANGO_UNIT_TEST_BEGIN(tick_count_test)
{
  using dango::integer::u_int;
  using dango::tick::zero;
  using dango::tick::to_milli;
  using dango::tick::from_milli;
  using dango::timeout_flags::HIGH_RES;

  auto const [a_tick1, a_bias1] = dango::current_tick_and_suspend_bias();

  auto a_timeout = dango::timeout::make(a_tick1, HIGH_RES);

  dango_assert_terminate(a_tick1 >= zero && a_bias1 >= zero);

  test_print("tick=%u bias=%u\n", u_int(to_milli(a_tick1)), u_int(to_milli(a_bias1)));

  a_timeout.add(from_milli(100));

  dango::thread::sleep(a_timeout);

  auto const [a_tick2, a_bias2] = dango::current_tick_and_suspend_bias();

  dango_assert_terminate(a_tick2 >= a_tick1 + from_milli(100));
  dango_assert_terminate(a_bias2 >= a_bias1);

  test_print("tick=%u bias=%u\n", u_int(to_milli(a_tick2)), u_int(to_milli(a_bias2)));
}
DANGO_UNIT_TEST_END

DANGO_UNIT_TEST_BEGIN(tick_count_test_mt)
{
  using dango::integer::u_int;
  using dango::tick::zero;
  using dango::tick::to_milli;
  using dango::tick::from_milli;
  using dango::timeout_flags::HIGH_RES;

  auto const [a_tick1, a_bias1] = dango::current_tick_and_suspend_bias();

  dango_assert_terminate(a_tick1 >= zero && a_bias1 >= zero);

  test_print("tick=%u bias=%u\n", u_int(to_milli(a_tick1)), u_int(to_milli(a_bias1)));

  dango::thread a_threads[10];

  auto a_id = u_int(0);

  for(auto& a_thread: a_threads)
  {
    a_thread =
    dango::thread::create_daemon
    (
      [a_id, a_tick1 = a_tick1, a_bias1 = a_bias1]()noexcept->void
      {
        auto a_timeout = dango::timeout::make(a_tick1, HIGH_RES);

        auto const [a_tick2, a_bias2] = dango::current_tick_and_suspend_bias();

        dango_assert_terminate(a_tick2 >= a_tick1);
        dango_assert_terminate(a_bias2 >= a_bias1);

        test_print("id=%u tick=%u bias=%u\n", a_id, u_int(to_milli(a_tick2)), u_int(to_milli(a_bias2)));

        a_timeout.add(from_milli(1'000));

        dango::thread::sleep(a_timeout);

        auto const [a_tick3, a_bias3] = dango::current_tick_and_suspend_bias();

        test_print("id=%u tick=%u bias=%u\n", a_id, u_int(to_milli(a_tick3)), u_int(to_milli(a_bias3)));
      }
    );

    ++a_id;
  }

  for(auto const& a_thread: a_threads)
  {
    a_thread.join();
  }

  auto const [a_tick2, a_bias2] = dango::current_tick_and_suspend_bias();

  dango_assert_terminate(a_tick2 >= a_tick1 + from_milli(1'000));
  dango_assert_terminate(a_bias2 >= a_bias1);

  test_print("tick=%u bias=%u\n", u_int(to_milli(a_tick2)), u_int(to_milli(a_bias2)));
}
DANGO_UNIT_TEST_END



}
