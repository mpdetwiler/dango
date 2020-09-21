#include "dango-assert.hpp"
#include "dango-atomic.hpp"

#include <cstdio>
#include <exception>

static constinit
dango::atomic<dango::assert_log_func>
s_handler_func{ dango::null };

auto
dango::
set_assert_logger
(dango::assert_log_func const a_handler)noexcept->dango::assert_log_func
{
  return s_handler_func.exchange(a_handler);
}

auto
dango::
get_assert_logger
()noexcept->dango::assert_log_func
{
  return s_handler_func.load();
}

void
dango::
detail::
assert_fail_once
()noexcept
{
  static thread_local constinit bool t_recursion = false;

  if(t_recursion)
  {
    std::fprintf(stderr, "assertion failure entered recursively\n");

    dango::trap_instruction();
  }

  t_recursion = true;

  static constinit dango::atomic<bool> s_assert_fail_once{ false };

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
)noexcept
{
  using dango::integer::u_int;

  std::fprintf
  (
    stderr,
    "%s[%u]: %s: assertion \"%s\" failed%s%s\n",
    a_loc.file_as_char(),
    u_int(a_loc.line()),
    a_loc.function_as_char(),
    dango::bchar_as_char(a_expr),
    (a_msg ? ":\n" : ""),
    (a_msg ? dango::bchar_as_char(a_msg) : "")
  );
}

auto
dango::
set_terminate
(dango::terminate_func const a_handler)noexcept->dango::terminate_func
{
  return std::set_terminate(a_handler);
}

auto
dango::
get_terminate
()noexcept->dango::terminate_func
{
  return std::get_terminate();
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
detail::
unit_test_exec
(
  char const* const a_name,
  char const* const a_file,
  int const a_line,
  void(* const a_test)()noexcept(false)
)noexcept
{
  using dango::integer::u_int;

  std::fprintf
  (
    stderr,
    "[unit test begin]: \"%s\" (file=%s line=%u)\n",
    a_name,
    a_file,
    u_int(a_line)
  );

  try
  {
    a_test();
  }
  catch(...)
  {
    std::fprintf
    (
      stderr,
      "[unit test fail]:  \"%s\" threw exception\n",
      a_name
    );

    dango::terminate();
  }

  std::fprintf
  (
    stderr,
    "[unit test end]:   \"%s\"\n\n",
    a_name
  );
}
