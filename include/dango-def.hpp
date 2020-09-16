#ifndef DANGO_DEF_HPP_INCLUDED
#define DANGO_DEF_HPP_INCLUDED

#include "dango-macro.hpp"

namespace
dango::integer
{
  using u_char     = unsigned char;
  using u_short    = unsigned short int;
  using u_int      = unsigned int;
  using u_long     = unsigned long int;
  using u_longlong = unsigned long long int;

  using s_char     = signed char;
  using s_short    = signed short int;
  using s_int      = signed int;
  using s_long     = signed long int;
  using s_longlong = signed long long int;
}

namespace
dango
{
  using null_tag = std::nullptr_t;

  inline constexpr dango::null_tag const null{ };

  using bchar = char8_t;
  using wchar = char16_t;
  using dchar = char32_t;

  using usize = std::size_t;
  using ssize = std::ptrdiff_t;

  using ubyte  = std::uint8_t;
  using ushort = std::uint16_t;
  using uint   = std::uint32_t;
  using ulong  = std::uint64_t;
  using uptr   = std::uintptr_t;

  using sbyte  = std::int8_t;
  using sshort = std::int16_t;
  using sint   = std::int32_t;
  using slong  = std::int64_t;
  using sptr   = std::intptr_t;

  using real = long double;
}

#ifndef DANGO_NO_BASIC_TYPES

using dango::null_tag;
using dango::null;
using dango::bchar;
using dango::wchar;
using dango::dchar;
using dango::usize;
using dango::ssize;
using dango::ubyte;
using dango::ushort;
using dango::uint;
using dango::ulong;
using dango::uptr;
using dango::sbyte;
using dango::sshort;
using dango::sint;
using dango::slong;
using dango::sptr;
using dango::real;

#endif

namespace
dango::detail
{
  inline constexpr auto const CACHE_LINE_SIZE = dango::usize(DANGO_CACHE_LINE_SIZE);
}

namespace
dango
{
  struct alignas(std::max_align_t)
  max_align_type
  {

  };

  struct alignas(dango::detail::CACHE_LINE_SIZE)
  cache_align_type
  {

  };

  enum class [[gnu::may_alias]] byte:dango::ubyte{ };

  void constexpr_unreachable()noexcept;
}

#endif // DANGO_DEF_HPP_INCLUDED
