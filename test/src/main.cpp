#include "dango.hpp"
#include "dango-global-test.hpp"
#include "dango-test-print.hpp"

auto
main
()noexcept(false)->int
{
  test_print("main entered\n");

  test_print("main exiting\n");

  return 0;
}

namespace
{

DANGO_UNIT_TEST_BEGIN(main_test)
{
  auto const a_rs = dango::polymorphic_allocator<>::make<dango::basic_mem_resource>();
}
DANGO_UNIT_TEST_END

}

