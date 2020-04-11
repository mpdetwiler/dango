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
  using null_tag = decltype(nullptr);

  inline constexpr dango::null_tag const null{ };

  using bchar = char8_t;
  using wchar = char16_t;
  using dchar = char32_t;

  using usize = decltype(sizeof(dango::bchar));
  using ssize = decltype(static_cast<dango::bchar*>(dango::null) - static_cast<dango::bchar*>(dango::null));

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
  wchar_t                       m_04;
  dango::bchar                  m_05;
  dango::wchar                  m_06;
  dango::dchar                  m_07;
  dango::builtin::uchar         m_08;
  dango::builtin::ushort        m_09;
  dango::builtin::uint          m_10;
  dango::builtin::ulong         m_11;
  dango::builtin::ulonglong     m_12;
  dango::builtin::schar         m_13;
  dango::builtin::sshort        m_14;
  dango::builtin::sint          m_15;
  dango::builtin::slong         m_16;
  dango::builtin::slonglong     m_17;
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

