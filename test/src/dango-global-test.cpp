#include "dango-global-test.hpp"

#include <cstdio>

test::
global_printer::
global_printer
()noexcept
{
  dango_access_global(dango::detail::s_cond_var_registry_thread, a_registry_thread)
  {

  }

  fprintf(stderr, "global_printer::global_printer()\n");
}

test::
global_printer::
~global_printer
()noexcept
{
  fprintf(stderr, "global_printer::~global_printer()\n");
}

void
test::
global_printer::
print
()const noexcept
{
  fprintf(stderr, "global_printer::print()\n");
}
