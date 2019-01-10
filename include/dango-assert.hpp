#ifndef __DANGO_ASSERT_HPP__
#define __DANGO_ASSERT_HPP__ 1

#include "dango-traits.hpp"
#include "dango-macro.hpp"

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
public:
  static constexpr auto
  current
  (
    char const* = __builtin_FILE(),
    char const* = __builtin_FUNCTION(),
    dango::s_int = __builtin_LINE()
  )
  noexcept->source_location;
private:
  constexpr
  source_location
  (
    char const*,
    char const*,
    dango::uint32
  )
  noexcept;
public:
  ~source_location()noexcept = default;

  constexpr auto file()const noexcept->char const*;
  constexpr auto function()const noexcept->char const*;
  constexpr auto line()const noexcept->dango::uint32;
private:
  char const* const m_file;
  char const* const m_func;
  dango::uint32 const m_line;
public:
  DANGO_DELETE_DEFAULT(source_location)
  DANGO_IMMOBILE(source_location)
};

constexpr
dango::
source_location::
source_location
(
  char const* const a_file,
  char const* const a_func,
  dango::uint32 const a_line
)
noexcept:
m_file{ a_file },
m_func{ a_func },
m_line{ a_line }
{

}

constexpr auto
dango::
source_location::
file
()const noexcept->char const*
{
  return m_file;
}

constexpr auto
dango::
source_location::
function
()const noexcept->char const*
{
  return m_func;
}

constexpr auto
dango::
source_location::
line
()const noexcept->dango::uint32
{
  return m_line;
}

constexpr auto
dango::
source_location::
current
(
  char const* const a_file,
  char const* const a_func,
  dango::s_int const a_line
)
noexcept->source_location
{
  return source_location{ a_file, a_func, dango::uint32(a_line) };
}

/*** likely unlikely ***/

namespace
dango
{
  constexpr auto likely(bool)noexcept->bool;
  constexpr auto unlikely(bool)noexcept->bool;
}

constexpr auto
dango::
likely
(bool const a_cond)noexcept->bool
{
  return bool(__builtin_expect(a_cond, true));
}

constexpr auto
dango::
unlikely
(bool const a_cond)noexcept->bool
{
  return bool(__builtin_expect(a_cond, false));
}

/*** assume ***/

namespace
dango
{
  constexpr void assume(bool)noexcept;
}

constexpr void
dango::
assume
(bool const a_cond)noexcept
{
  if(dango::unlikely(!a_cond))
  {
    __builtin_unreachable();
  }
}

/*** dango_assert ***/

namespace
dango
{
  using assert_log_func = void(*)(char const*, dango::source_location const&)noexcept;

  auto set_assert_log_handler(dango::assert_log_func)noexcept->assert_log_func;

  auto get_assert_log_handler()noexcept->assert_log_func;
}

namespace
dango::detail
{
  void assert_fail_log(char const*, dango::source_location const&)noexcept;

  void assert_fail_once()noexcept;

  void default_assert_log_handler(char const*, dango::source_location const&)noexcept;
}

inline void
dango::
detail::
assert_fail_log
(char const* const a_message, dango::source_location const& a_loc)noexcept
{
  detail::assert_fail_once();

  auto const a_handler = dango::get_assert_log_handler();

  if(a_handler)
  {
    a_handler(a_message, a_loc);
  }
  else
  {
    detail::default_assert_log_handler(a_message, a_loc);
  }
}

namespace
dango::detail
{
  constexpr void
  assert_func
  (
    bool,
    char const*,
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
  char const* const a_message,
  dango::source_location const& a_loc
)
noexcept
{
  if(dango::unlikely(!a_cond))
  {
    detail::assert_fail_log(a_message, a_loc);

    __builtin_trap();
  }
}

#ifndef DANGO_NO_DEBUG
#define dango_assert(cond) dango::detail::assert_func(bool(cond), #cond)
#define dango_assert_loc(cond, loc) dango::detail::assert_func(bool(cond), #cond, loc)
#else
#define dango_assert(cond) dango::assume(bool(cond))
#define dango_assert_loc(cond, loc) dango_assert(cond)
#endif

/*** dango_unreachable ***/

namespace
dango::detail
{
  void
  unreachable_func
  (dango::source_location const& = dango::source_location::current())noexcept;

  extern char const* const unreachable_message;
}

inline void
dango::
detail::
unreachable_func
(dango::source_location const& a_loc)noexcept
{
  detail::assert_fail_log(unreachable_message, a_loc);

  __builtin_trap();
}

#ifndef DANGO_NO_DEBUG
#define dango_unreachable dango::detail::unreachable_func()
#else
#define dango_unreachable __builtin_unreachable()
#endif

/*** source location arg ***/

#ifndef DANGO_NO_DEBUG

#define DANGO_SRC_LOC_ARG_DEFAULT(name) \
dango::source_location const& name = dango::source_location::current()

#define DANGO_SRC_LOC_ARG(name) dango::source_location const& name

#else

namespace
dango::detail
{
  struct
  assert_dummy_tag
  final
  {

  };

  inline constexpr assert_dummy_tag const assert_dummy_val{ };
}


#define DANGO_SRC_LOC_ARG_DEFAULT(name) \
dango::detail::assert_dummy_tag = dango::detail::assert_dummy_val

#define DANGO_SRC_LOC_ARG(name) dango::detail::assert_dummy_tag const

#endif

#ifdef DANGO_SOURCE_FILE

#include "dango-assert.hpp"
#include "dango-atomic.hpp"

namespace
dango::detail
{
  namespace
  {
    dango::atomic<dango::assert_log_func> s_assert_log_handler{ nullptr };
    dango::atomic<bool> s_assert_fail_once{ false };
  }
}

char const* const
dango::
detail::
unreachable_message = "unreachable statement reached";

auto
dango::
set_assert_log_handler
(dango::assert_log_func const a_handler)noexcept->dango::assert_log_func
{
  return detail::s_assert_log_handler.exchange(a_handler);
}

auto
dango::
get_assert_log_handler
()noexcept->dango::assert_log_func
{
  return detail::s_assert_log_handler.load();
}

void
dango::
detail::
assert_fail_once
()noexcept
{
  while(detail::s_assert_fail_once.exchange(true));
}

#include <stdio.h>

void
dango::
detail::
default_assert_log_handler
(char const* const a_message, dango::source_location const& a_loc)noexcept
{
  fprintf
  (
    stderr,
    "%s[%u] %s: assertion failed: %s\n",
    a_loc.file(),
    dango::u_int(a_loc.line()),
    a_loc.function(),
    a_message
  );
}

#endif

#endif

