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

#ifndef DANGO_PLATFORM_WINDOWS
#ifndef DANGO_NO_DEBUG
DANGO_UNIT_TEST_BEGIN(inline_global_address_test)
{
  auto const a_extern = dango::detail::test::inline_global_address_extern();
  auto const a_inline = dango::detail::test::inline_global_address_inline();

  test_print("inline_global_address_extern: %p\n", a_extern);
  test_print("inline_global_address_inline: %p\n", a_inline);

  dango_assert_terminate(a_extern == a_inline);
}
DANGO_UNIT_TEST_END
#endif
#endif

}
