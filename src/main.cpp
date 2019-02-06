#include "dango.hpp"

#include <cstdio>

auto
main
()noexcept->dango::s_int
{
  /*{
    TIMECAPS a_caps;

    timeGetDevCaps(&a_caps, sizeof(a_caps));

    printf("min period: %u\n", dango::u_int(a_caps.wPeriodMin));
    printf("max period: %u\n", dango::u_int(a_caps.wPeriodMax));

    timeBeginPeriod(a_caps.wPeriodMin);
  }*/

  dango::thread a_threads[]{ null, null, null, null, null };

  dango::mutex a_mutex{ };

  auto a_id = dango::uint32(0);

  for(auto& a_thread:a_threads)
  {
    a_thread =
    dango::thread::create
    (
      [a_id, &a_mutex]()noexcept->void
      {
        auto a_tick = dango::get_tick_count();
        auto a_tick_sa = dango::get_tick_count_sa();

        fprintf
        (
          stderr,
          "thread[%u] sleep... (%llu) (%llu)\n",
          dango::u_int(a_id),
          dango::u_cent(a_tick),
          dango::u_cent(a_tick_sa)
        );

        dango::thread::sleep(30'000);

        a_tick = dango::get_tick_count();
        a_tick_sa = dango::get_tick_count_sa();

        fprintf
        (
          stderr,
          "thread[%u] sleep_sa... (%llu) (%llu)\n",
          dango::u_int(a_id),
          dango::u_cent(a_tick),
          dango::u_cent(a_tick_sa)
        );

        dango::thread::sleep_sa(30'000);

        a_tick = dango::get_tick_count();
        a_tick_sa = dango::get_tick_count_sa();

        fprintf
        (
          stderr,
          "thread[%u] serial sleep... (%llu) (%llu)\n",
          dango::u_int(a_id),
          dango::u_cent(a_tick),
          dango::u_cent(a_tick_sa)
        );

        dango_crit(a_mutex)
        {
          dango::thread::sleep(5'000);
        }

        a_tick = dango::get_tick_count();
        a_tick_sa = dango::get_tick_count_sa();

        fprintf
        (
          stderr,
          "thread[%u] done (%llu) (%llu)\n",
          dango::u_int(a_id),
          dango::u_cent(a_tick),
          dango::u_cent(a_tick_sa)
        );
      }
    );

    ++a_id;
  }

  for(auto const& a_thread:a_threads)
  {
    a_thread.join();
  }

  return 0;
}

