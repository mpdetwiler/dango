#include "dango-def.hpp"
#include "dango-int.hpp"
#include "dango-macro.hpp"

void func()noexcept;

#include <cstdio>

auto
main
()noexcept->dango::s_int
{
  printf("%lli\n", dango::s_cent(dango::integer::MIN_VAL<sint8>));
  printf("%lli\n", dango::s_cent(dango::integer::MIN_VAL<sint16>));
  printf("%lli\n", dango::s_cent(dango::integer::MIN_VAL<sint32>));
  printf("%lli\n", dango::s_cent(dango::integer::MIN_VAL<sint64>));

  func();

  return 0;
}

