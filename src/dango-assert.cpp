#include "dango.hpp"

#include <cstdio>
#include <exception>

static dango::atomic<dango::assert_log_handler> s_assert_log_handler{ dango::null };

auto
dango::
set_assert_log_handler
(dango::assert_log_handler const a_handler)noexcept->dango::assert_log_handler
{
  return s_assert_log_handler.exchange(a_handler);
}

auto
dango::
get_assert_log_handler
()noexcept->dango::assert_log_handler
{
  return s_assert_log_handler.load();
}

void
dango::
detail::
assert_fail_once
()noexcept
{
  thread_local bool t_recursion = false;

  if(t_recursion)
  {
    fprintf(stderr, "assertion failure entered recursively\n");

    dango::trap_instruction();
  }

  t_recursion = true;

  static dango::atomic<bool> s_assert_fail_once{ false };

  if(s_assert_fail_once.exchange<dango::mem_order::acquire>(true))
  {
    dango::infinite_loop();
  }
}

void
dango::
detail::
default_assert_log_handler
(
  dango::bchar const* const a_expr,
  dango::bchar const* const a_msg,
  dango::source_location const& a_loc
)
noexcept
{
  if(a_msg)
  {
    fprintf
    (
      stderr,
      "%s[%u]: %s: assertion \"%s\" failed:\n%s\n",
      a_loc.file_as_char(),
      dango::builtin::uint(a_loc.line()),
      a_loc.function_as_char(),
      dango::bchar_as_char(a_expr),
      dango::bchar_as_char(a_msg)
    );

    return;
  }

  fprintf
  (
    stderr,
    "%s[%u]: %s: assertion \"%s\" failed\n",
    a_loc.file_as_char(),
    dango::builtin::uint(a_loc.line()),
    a_loc.function_as_char(),
    dango::bchar_as_char(a_expr)
  );
}

auto
dango::
set_terminate
(dango::terminate_handler const a_handler)noexcept->dango::terminate_handler
{
  auto const a_result = std::set_terminate(a_handler);

  return a_result;
}

auto
dango::
get_terminate
()noexcept->dango::terminate_handler
{
  auto const a_result = std::get_terminate();

  return a_result;
}

void
dango::
terminate
()noexcept
{
  std::terminate();
}

void
dango::
infinite_loop
()noexcept
{
  auto a_temp = true;

  auto const volatile& a_read = a_temp;

  do
  {
    if(a_read)
    {
      detail::thread_yield();
    }
  }
  while(true);
}

