#include "dango-util-base.hpp"
#include "dango-assert.hpp"
#include "dango-thread.hpp"

void
dango::
infinite_loop
()noexcept
{
  auto a_true = true;

  do
  {
    dango::thread::sleep_rel(dango::tick::from_sec(60));
  }
  while(dango::volatile_load(&a_true));

  dango_unreachable;
}
