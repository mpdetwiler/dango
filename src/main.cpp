#include "dango.hpp"

#include <cstdio>

#include <windows.h>

auto
main
()noexcept->dango::s_int
{
  timeBeginPeriod(DWORD(1));

  fprintf(stdout, "tick_count: %llu\n", dango::u_cent(dango::get_tick_count()));
  fprintf(stdout, "tick_count_sa: %llu\n", dango::u_cent(dango::get_tick_count_sa()));
  fprintf(stdout, "sleep...\n");

  dango::thread::sleep(30'000);

  fprintf(stdout, "tick_count: %llu\n", dango::u_cent(dango::get_tick_count()));
  fprintf(stdout, "tick_count_sa: %llu\n", dango::u_cent(dango::get_tick_count_sa()));
  fprintf(stdout, "sleep_sa...\n");

  dango::thread::sleep_sa(30'000);

  for(auto a_i = uint32(0); a_i != uint32(40); ++a_i)
  {
    fprintf(stdout, "tick_count: %llu\n", dango::u_cent(dango::get_tick_count()));
    fprintf(stdout, "tick_count_sa: %llu\n", dango::u_cent(dango::get_tick_count_sa()));
    dango::thread::sleep(5);
  }

  timeEndPeriod(1);

  return 0;
}

