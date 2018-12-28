#ifndef __DANGO_DEF_HPP__
#define __DANGO_DEF_HPP__ 1

#ifndef __GNUG__
#error unsupported compiler
#endif

#define dango_restrict __restrict__

namespace dango
{
  using unsigned_char = unsigned char;
  using unsigned_short = unsigned short int;
  using unsigned_int = unsigned int;
  using unsigned_long = unsigned long int;
  using unsigned_long_long = unsigned long long int;

  using signed_char = signed char;
  using signed_short = signed short int;
  using signed_int = signed int;
  using signed_long = signed long int;
  using signed_long_long = signed long long int;

  using long_double = long double;
}

namespace dango
{
  using nullptr_tag = decltype(nullptr);

  using usize = decltype(sizeof(char));
  using isize = decltype(static_cast<char*>(nullptr) - static_cast<char*>(nullptr));

  using uint8 = __UINT8_TYPE__;
  using uint16 = __UINT16_TYPE__;
  using uint32 = __UINT32_TYPE__;
  using uint64 = __UINT64_TYPE__;
  using uintptr = __UINTPTR_TYPE__;

  using int8 = __INT8_TYPE__;
  using int16 = __INT16_TYPE__;
  using int32 = __INT32_TYPE__;
  using int64 = __INT64_TYPE__;
  using intptr = __INTPTR_TYPE__;

  using char8 = char;
  using char16 = char16_t;
  using char32 = char32_t;

  using float32 = float;
  using float64 = double;

  struct
  null_tag
  final
  {

  };

  inline constexpr dango::null_tag const null{};
}

#ifndef DANGO_NO_BASIC_TYPES

using dango::nullptr_tag;
using dango::usize;
using dango::isize;
using dango::uint8;
using dango::uint16;
using dango::uint32;
using dango::uint64;
using dango::uintptr;
using dango::int8;
using dango::int16;
using dango::int32;
using dango::int64;
using dango::intptr;
using dango::char8;
using dango::char16;
using dango::char32;
using dango::float32;
using dango::float64;
using dango::null_tag;
using dango::null;

#endif

#ifndef DANGO_CACHE_LINE_SIZE
#define DANGO_CACHE_LINE_SIZE 64
#endif

namespace dango::detail
{
  inline constexpr auto const CACHE_LINE_SIZE = dango::usize(DANGO_CACHE_LINE_SIZE);

  union fundamental_union;
}

union
dango::
detail::
fundamental_union
{
private:
  bool                          m_00;
  void*                         m_01;
  dango::nullptr_tag            m_02;
  char                          m_03;
  wchar_t                       m_04;
  char16_t                      m_06;
  char32_t                      m_07;
  dango::unsigned_char          m_08;
  dango::unsigned_short         m_09;
  dango::unsigned_int           m_10;
  dango::unsigned_long          m_11;
  dango::unsigned_long_long     m_12;
  dango::signed_char            m_13;
  dango::signed_short           m_14;
  dango::signed_int             m_15;
  dango::signed_long            m_16;
  dango::signed_long_long       m_17;
  float                         m_18;
  double                        m_19;
  dango::long_double            m_20;
};

namespace dango
{
  struct alignas(dango::detail::fundamental_union)
  max_align_type
  {

  };

  struct alignas(dango::detail::CACHE_LINE_SIZE)
  cache_align_type
  {

  };

  enum class
  byte:
  dango::unsigned_char
  {

  };
}

#endif
