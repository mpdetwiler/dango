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
(char const* const a_str)const noexcept
{
  fprintf(stderr, "global_printer::print(): %s\n", a_str);
}

namespace
{

DANGO_UNIT_TEST_BEGIN(global_access_test)
{
  dango_access_global(test::s_global_printer, a_printer)
  {
    a_printer->print("global_access_test");
  }
}
DANGO_UNIT_TEST_END

}
