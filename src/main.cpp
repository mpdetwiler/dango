#include "dango-def.hpp"
#include "dango-int.hpp"
#include "dango-macro.hpp"

void func()noexcept;

#include <cstdio>

auto
main
()noexcept->dango::s_int
{
  printf("%llu\n", dango::u_cent(dango::SIZE_MAX));

  func();

  return 0;
}

