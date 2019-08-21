#ifndef __DANGO_DEF_HPP__
#define __DANGO_DEF_HPP__ 1

#ifndef __GNUG__
#error unsupported compiler
#endif

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

  using wchar = wchar_t;
}

namespace
dango
{
  using null_tag = decltype(nullptr);

  inline constexpr dango::null_tag const null{ };

  using usize = decltype(sizeof(char));
  using ssize = decltype(static_cast<char*>(dango::null) - static_cast<char*>(dango::null));

  using ubyte = __UINT8_TYPE__;
  using ushort = __UINT16_TYPE__;
  using uint = __UINT32_TYPE__;
  using ulong = __UINT64_TYPE__;
  using uptr = __UINTPTR_TYPE__;

  using sbyte = __INT8_TYPE__;
  using sshort = __INT16_TYPE__;
  using sint = __INT32_TYPE__;
  using slong = __INT64_TYPE__;
  using sptr = __INTPTR_TYPE__;

  using wchar = char16_t;
  using dchar = char32_t;

  using real = long double;
}

#ifndef DANGO_NO_BASIC_TYPES

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
  dango::null_tag               m_02;
  char                          m_03;
  dango::builtin::wchar         m_04;
  dango::wchar                  m_05;
  dango::dchar                  m_06;
  dango::builtin::uchar         m_07;
  dango::builtin::ushort        m_08;
  dango::builtin::uint          m_09;
  dango::builtin::ulong         m_10;
  dango::builtin::ulonglong     m_11;
  dango::builtin::schar         m_12;
  dango::builtin::sshort        m_13;
  dango::builtin::sint          m_14;
  dango::builtin::slong         m_15;
  dango::builtin::slonglong     m_16;
  float                         m_17;
  double                        m_18;
  dango::real                   m_19;
  void(* m_20)()noexcept;
  void(fundamental_union::* m_21)()noexcept;
  char fundamental_union::* m_22;
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
  dango::ubyte
  {

  };
}

#endif
