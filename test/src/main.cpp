#include "dango.hpp"
#include "dango-global-test.hpp"
#include "dango-test-print.hpp"
#include "dango-concurrent.hpp"

auto
main
()noexcept(false)->int
{
  test_print("main entered\n");

  dango::tick_count_pair a_pairs[1'000];

  for(auto& a_pair : a_pairs)
  {
    a_pair = dango::current_tick_and_suspend_bias();

    dango::thread_yield(true);
  }

  for(auto const& a_pair : a_pairs)
  {
    auto const& [a_tick, a_bias] = a_pair;

    test_print
    (
      "[%lld, %lld]\n",
      dango::integer::s_longlong(dango::tick::to_milli(a_tick)),
      dango::integer::s_longlong(dango::tick::to_milli(a_bias))
    );
  }

  test_print("main exiting\n");

  return 0;
}
