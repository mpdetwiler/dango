#include "dango.hpp"
#include "dango-global-test.hpp"

#include <cstdio>

auto
main
()noexcept(false)->int
{
  printf("main entered\n");

  printf("main exiting\n");

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

