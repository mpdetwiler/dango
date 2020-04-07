#include "dango-global-test.hpp"

#include <cstdio>

test::
global_printer::
global_printer
()noexcept
{
  fprintf(stderr, "global_printer::global_printer()\n");
}

test::
global_printer::
~global_printer
()noexcept
{
  fprintf(stderr, "global_printer::~global_printer()\n");
}

