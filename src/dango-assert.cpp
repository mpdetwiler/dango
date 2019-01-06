#include "dango-assert.hpp"
#include "dango-atomic.hpp"

namespace
{
  dango::atomic<dango::assert_log_func> s_log_handler{ nullptr };
}

auto
dango::
set_assert_log_handler
(dango::assert_log_func const a_handler)noexcept->dango::assert_log_func
{
  return s_log_handler.exchange(a_handler);
}

auto
dango::
get_assert_log_handler
()noexcept->dango::assert_log_func
{
  return s_log_handler.load();
}

void
dango::
detail::
assert_fail_once
()noexcept
{
  static dango::atomic<bool> s_blocker{ false };

  while(s_blocker.exchange(true));
}

#include <stdio.h>

void
dango::
detail::
default_assert_log_handler
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

