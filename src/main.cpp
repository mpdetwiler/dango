#include "dango.hpp"

#include <cstdio>

auto
main
()noexcept->dango::s_int
{
  dango::thread::sleep(100);
  dango::thread::sleep_sa(100);

  dango::mutex a_mutex{ };
  dango::cond_var_mutex a_cond{ a_mutex };

  {
    auto const a_deadline = dango::make_deadline_rel(30'000);

    fprintf(stdout, "tick_count: %llu\n", dango::u_cent(dango::get_tick_count()));
    fprintf(stdout, "tick_count_sa: %llu\n", dango::u_cent(dango::get_tick_count_sa()));
    fprintf(stdout, "sleep...\n");

    dango_crit_full(a_cond, a_crit)
    {
      while(!a_deadline.has_passed())
      {
        a_crit.wait(a_deadline);

        fprintf(stdout, "wake\n");
      }
    }
  }

  {
    auto const a_deadline = dango::make_deadline_rel_sa(30'000);

    fprintf(stdout, "tick_count: %llu\n", dango::u_cent(dango::get_tick_count()));
    fprintf(stdout, "tick_count_sa: %llu\n", dango::u_cent(dango::get_tick_count_sa()));
    fprintf(stdout, "sleep_sa...\n");

    dango_crit_full(a_cond, a_crit)
    {
      while(!a_deadline.has_passed())
      {
        a_crit.wait(a_deadline);

        fprintf(stdout, "wake\n");
      }
    }
  }

  fprintf(stdout, "tick_count: %llu\n", dango::u_cent(dango::get_tick_count()));
  fprintf(stdout, "tick_count_sa: %llu\n", dango::u_cent(dango::get_tick_count_sa()));

  auto a_deadline = dango::make_deadline_rel(5);

  for(auto a_i = uint32(0); a_i != uint32(40); ++a_i)
  {
    fprintf(stdout, "tick_count: %llu\n", dango::u_cent(dango::get_tick_count()));

    dango_crit_full(a_cond, a_crit)
    {
      while(!a_deadline.has_passed())
      {
        a_crit.wait(a_deadline);
      }
    }

    a_deadline.add(5);
  }

  return 0;
}

