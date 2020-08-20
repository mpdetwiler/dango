#include "dango.hpp"

void
dango::
infinite_loop
()noexcept
{
  auto a_true = true;

  do
  {
    dango::thread_yield(dango::uint(100));
  }
  while(dango::volatile_load(&a_true));

  dango_unreachable;
}

