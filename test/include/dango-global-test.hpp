#ifndef DANGO_GLOBAL_TEST_HPP_INCLUDED
#define DANGO_GLOBAL_TEST_HPP_INCLUDED

#include "dango.hpp"

namespace
test
{
  struct
  global_printer
  final
  {
    global_printer()noexcept;
    ~global_printer()noexcept;
    void print(char const*)const noexcept;
  };

  DANGO_DEFINE_GLOBAL_INLINE(global_printer const, s_global_printer, { })
}

#endif
