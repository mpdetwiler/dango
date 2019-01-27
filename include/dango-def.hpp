#ifndef __DANGO_DEF_HPP__
#define __DANGO_DEF_HPP__ 1

#ifndef __GNUG__
#error unsupported compiler
#endif

#define dango_restrict __restrict__

#ifndef DANGO_NO_KEYWORDS
#define restrict dango_restrict
#endif

#include "dango-macro.hpp"

namespace
dango
{
  using u_byte = unsigned char;
  using u_short = unsigned short int;
  using u_int = unsigned int;
  using u_long = unsigned long int;
  using u_cent = unsigned long long int;

  using s_byte = signed char;
  using s_short = signed short int;
  using s_int = signed int;
  using s_long = signed long int;
  using s_cent = signed long long int;
}

namespace
dango
{
  using nullptr_tag = decltype(nullptr);

  using usize = decltype(sizeof(char));
  using ssize = decltype(static_cast<char*>(nullptr) - static_cast<char*>(nullptr));

  using uint8 = __UINT8_TYPE__;
  using uint16 = __UINT16_TYPE__;
  using uint32 = __UINT32_TYPE__;
  using uint64 = __UINT64_TYPE__;
  using uintptr = __UINTPTR_TYPE__;

  using sint8 = __INT8_TYPE__;
  using sint16 = __INT16_TYPE__;
  using sint32 = __INT32_TYPE__;
  using sint64 = __INT64_TYPE__;
  using sintptr = __INTPTR_TYPE__;

  using wchar = char16_t;
  using dchar = char32_t;

  using real = long double;

  struct
  null_tag
  final
  {
    DANGO_TAG_TYPE(null_tag)
  };

  inline constexpr dango::null_tag const null{ };
}

#ifndef DANGO_NO_BASIC_TYPES

using dango::nullptr_tag;
using dango::usize;
using dango::ssize;
using dango::uint8;
using dango::uint16;
using dango::uint32;
using dango::uint64;
using dango::uintptr;
using dango::sint8;
using dango::sint16;
using dango::sint32;
using dango::sint64;
using dango::sintptr;
using dango::wchar;
using dango::dchar;
using dango::real;
using dango::null_tag;
using dango::null;

#endif

#ifndef DANGO_CACHE_LINE_SIZE
#define DANGO_CACHE_LINE_SIZE 64
#endif

namespace
dango::detail
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
  dango::wchar                  m_06;
  dango::dchar                  m_07;
  dango::u_byte                 m_08;
  dango::u_short                m_09;
  dango::u_int                  m_10;
  dango::u_long                 m_11;
  dango::u_cent                 m_12;
  dango::s_byte                 m_13;
  dango::s_short                m_14;
  dango::s_int                  m_15;
  dango::s_long                 m_16;
  dango::s_cent                 m_17;
  float                         m_18;
  double                        m_19;
  dango::real                   m_20;
  void(* m_21)()noexcept;
  void(fundamental_union::* m_22)()noexcept;
  char fundamental_union::* m_23;
};

namespace
dango
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
  dango::u_byte
  {

  };
}

#endif
