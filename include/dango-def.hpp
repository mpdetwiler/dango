#ifndef DANGO_DEF_HPP_INCLUDED
#define DANGO_DEF_HPP_INCLUDED

#define dango_restrict __restrict__

#ifndef DANGO_NO_KEYWORDS
#define restrict dango_restrict
#endif

namespace
dango::builtin
{
  using uchar = unsigned char;
  using ushort = unsigned short int;
  using uint = unsigned int;
  using ulong = unsigned long int;
  using ulonglong = unsigned long long int;

  using schar = signed char;
  using sshort = signed short int;
  using sint = signed int;
  using slong = signed long int;
  using slonglong = signed long long int;
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

  using ubyte = std::uint8_t;
  using ushort = std::uint16_t;
  using uint = std::uint32_t;
  using ulong = std::uint64_t;
  using uptr = std::uintptr_t;

  using sbyte = std::int8_t;
  using sshort = std::int16_t;
  using sint = std::int32_t;
  using slong = std::int64_t;
  using sptr = std::intptr_t;

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

#ifndef DANGO_CACHE_LINE_SIZE
#define DANGO_CACHE_LINE_SIZE 64
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

  enum class compare_val:dango::ssize{ };

  enum class hash_val:dango::usize{ };
}

namespace
dango::comparison
{
  constexpr auto is_equal(dango::compare_val const a_val)noexcept->bool{ return dango::ssize(a_val) == dango::ssize(0); }
  constexpr auto is_less(dango::compare_val const a_val)noexcept->bool{ return dango::ssize(a_val) < dango::ssize(0); }
  constexpr auto is_less_equal(dango::compare_val const a_val)noexcept->bool{ return dango::ssize(a_val) <= dango::ssize(0); }
  constexpr auto is_greater(dango::compare_val const a_val)noexcept->bool{ return dango::ssize(a_val) > dango::ssize(0); }
  constexpr auto is_greater_equal(dango::compare_val const a_val)noexcept->bool{ return dango::ssize(a_val) >= dango::ssize(0); }
  constexpr auto mirror(dango::compare_val const a_val)noexcept->dango::compare_val{ return dango::compare_val{ -dango::ssize(a_val) }; }
}

#endif // DANGO_DEF_HPP_INCLUDED

