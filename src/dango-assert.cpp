#include "dango-assert.hpp"

#include <stdio.h>

void
dango::
detail::
default_log_assert
(char const* const a_message, dango::source_location const& a_loc)noexcept
{
  printf
  (
    "%s[%u] %s: assertion failed: %s\n",
    a_loc.file(),
    dango::u_int(a_loc.line()),
    a_loc.function(),
    a_message
  );
}

