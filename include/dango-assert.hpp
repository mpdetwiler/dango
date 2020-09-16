#ifndef DANGO_ASSERT_HPP_INCLUDED
#define DANGO_ASSERT_HPP_INCLUDED

#include "dango-util-base.hpp"

/*** source_location ***/

namespace
dango
{
  class source_location;
}

class
dango::
source_location
final
{
private:
  using char_type = char;
  using int_type = dango::integer::s_int;
public:
  static constexpr auto
  current
  (
    char_type const* = __builtin_FILE(),
    char_type const* = __builtin_FUNCTION(),
    int_type = __builtin_LINE()
  )
  noexcept->source_location;
private:
  constexpr
  source_location
  (
    char_type const* const a_file,
    char_type const* const a_func,
    int_type const a_line
  )
  noexcept:
  m_file{ a_file },
  m_func{ a_func },
  m_line{ a_line }
  { }
public:
  constexpr source_location(source_location const&)noexcept = default;
  constexpr source_location(source_location&&)noexcept = default;
  constexpr ~source_location()noexcept = default;
public:
  constexpr auto file_as_char()const noexcept->char_type const*{ return m_file; }
  constexpr auto function_as_char()const noexcept->char_type const*{ return m_func; }
  constexpr auto line()const noexcept->dango::uint{ return dango::uint(m_line); }
  auto file()const noexcept->dango::bchar const*{ return dango::char_as_bchar(file_as_char()); }
  auto function()const noexcept->dango::bchar const*{ return dango::char_as_bchar(function_as_char()); }
private:
  char_type const* const m_file;
  char_type const* const m_func;
  int_type const m_line;
public:
  DANGO_DELETE_DEFAULT(source_location)
  DANGO_UNASSIGNABLE(source_location)
};

constexpr auto
dango::
source_location::
current
(
  char_type const* const a_file,
  char_type const* const a_func,
  int_type const a_line
)
noexcept->source_location
{
  return source_location{ a_file, a_func, a_line };
}

/*** dango_assert ***/

namespace
dango
{
  using assert_log_func =
    void(*)(dango::bchar const*, dango::bchar const*, dango::source_location const&)noexcept;

  DANGO_EXPORT auto set_assert_logger(dango::assert_log_func)noexcept->dango::assert_log_func;
  DANGO_EXPORT auto get_assert_logger()noexcept->dango::assert_log_func;
}

namespace
dango::detail
{
  void
  assert_fail_log
  (
    dango::bchar const*,
    dango::bchar const*,
    dango::source_location const&
  )
  noexcept;

  DANGO_EXPORT void assert_fail_once()noexcept;

  DANGO_EXPORT void
  default_assert_log_handler
  (
    dango::bchar const*,
    dango::bchar const*,
    dango::source_location const&
  )
  noexcept;
}

inline void
dango::
detail::
assert_fail_log
(
  dango::bchar const* const a_expr,
  dango::bchar const* const a_msg,
  dango::source_location const& a_loc
)
noexcept
{
  dango::detail::assert_fail_once();

  auto const a_handler = dango::get_assert_logger();

  if(a_handler)
  {
    a_handler(a_expr, a_msg, a_loc);
  }
  else
  {
    dango::detail::default_assert_log_handler(a_expr, a_msg, a_loc);
  }
}

namespace
dango::detail
{
  constexpr void
  assert_func
  (
    bool,
    dango::bchar const*,
    dango::bchar const*,
    dango::source_location const& = dango::source_location::current()
  )
  noexcept;
}

constexpr void
dango::
detail::
assert_func
(
  bool const a_cond,
  dango::bchar const* const a_expr,
  dango::bchar const* const a_msg,
  dango::source_location const& a_loc
)
noexcept
{
  if(dango::unlikely(!a_cond))
  {
    dango::detail::assert_fail_log(a_expr, a_msg, a_loc);

    dango::trap_instruction();
  }
}

/*** dango_unreachable ***/

namespace
dango::detail
{
  [[noreturn]] void
  unreachable_func(dango::bchar const* const, dango::source_location const& = dango::source_location::current())noexcept;

  inline constexpr dango::bchar const unreachable_expr[]{ u8"unreachable" };
  inline constexpr dango::bchar const unreachable_message[]{ u8"unreachable statement reached" };
}

inline void
dango::
detail::
unreachable_func
(dango::bchar const* const a_msg, dango::source_location const& a_loc)noexcept
{
  dango::detail::assert_fail_log(dango::detail::unreachable_expr, a_msg, a_loc);

  dango::trap_instruction();
}

/*** terminate get_terminate set_terminate ***/

namespace
dango
{
  using terminate_func = void(*)()noexcept(false);

  DANGO_EXPORT auto set_terminate(dango::terminate_func)noexcept->dango::terminate_func;
  DANGO_EXPORT auto get_terminate()noexcept->dango::terminate_func;

  [[noreturn]] DANGO_EXPORT void terminate()noexcept;
}

/*** source location arg ***/

#ifdef DANGO_NO_DEBUG
namespace
dango::detail
{
  struct
  assert_dummy_tag
  final
  {
    DANGO_TAG_TYPE(assert_dummy_tag)
  };

  inline constexpr assert_dummy_tag const assert_dummy_val{ };
}
#endif

/*** unit_test_exec ***/

namespace
dango::detail
{
  DANGO_EXPORT void unit_test_exec(char const*, char const*, int, void(*)()noexcept(false))noexcept;
}

#endif // DANGO_ASSERT_HPP_INCLUDED
