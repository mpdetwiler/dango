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
  printf("main_test\n");
}
DANGO_UNIT_TEST_END

}

