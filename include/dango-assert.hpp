#ifndef __DANGO_ASSERT_HPP__
#define __DANGO_ASSERT_HPP__ 1

#include "dango-util.hpp"
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
  if(!a_cond)
  {
    __builtin_unreachable();
  }
}

#endif

