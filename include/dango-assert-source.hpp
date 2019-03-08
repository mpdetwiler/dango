#ifdef DANGO_SOURCE_FILE

namespace
dango::assert_cpp
{
  dango::atomic<dango::assert_log_handler> s_assert_log_handler{ nullptr };
}

constexpr char const* const
dango::
detail::
unreachable_expr = "unreachable";

constexpr char const* const
dango::
detail::
unreachable_msg = "unreachable statement reached";

auto
dango::
set_assert_log_handler
(dango::assert_log_handler const a_handler)noexcept->dango::assert_log_handler
{
  return assert_cpp::s_assert_log_handler.exchange(a_handler);
}

auto
dango::
get_assert_log_handler
()noexcept->dango::assert_log_handler
{
  return assert_cpp::s_assert_log_handler.load();
}

void
dango::
detail::
assert_fail_once
()noexcept
{
  static dango::atomic<bool> s_assert_fail_once{ false };

  auto const a_spin = s_assert_fail_once.exchange<dango::mem_order::acquire>(true);

  while(a_spin)
  {
    detail::thread_yield();
  }
}

#include <stdio.h>

void
dango::
detail::
default_assert_log_handler
(
  char const* const a_expr,
  char const* const a_msg,
  dango::source_location const& a_loc
)
noexcept
{
  fprintf
  (
    stderr,
    "%s[%u] %s: assertion[%s] failed: %s\n",
    a_loc.file(),
    dango::u_int(a_loc.line()),
    a_loc.function(),
    a_expr,
    a_msg
  );
}

#include <exception>

auto
dango::
set_terminate
(dango::terminate_handler const a_handler)noexcept->dango::terminate_handler
{
  auto const a_result = std::set_terminate(a_handler);

  return reinterpret_cast<dango::terminate_handler>(a_result);
}

auto
dango::
get_terminate
()noexcept->dango::terminate_handler
{
  auto const a_result = std::get_terminate();

  return reinterpret_cast<dango::terminate_handler>(a_result);
}

void
dango::
terminate
()noexcept
{
  std::terminate();
}

#endif

