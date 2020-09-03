#include "dango-global-test.hpp"
#include "dango-test-print.hpp"

test::
global_printer::
global_printer
()noexcept
{
  test_print("global_printer::global_printer()\n");
}

test::
global_printer::
~global_printer
()noexcept
{
  test_print("global_printer::~global_printer()\n");
}

void
test::
global_printer::
print
(char const* const a_str)const noexcept
{
  test_print("global_printer::print(): %s\n", a_str);
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
