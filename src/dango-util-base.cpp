#include "dango.hpp"

void
dango::
infinite_loop
()noexcept
{
  auto a_true = true;

  do
  {
    dango::thread_yield();
  }
  while(dango::volatile_load(&a_true));

  dango_unreachable;
}

