#ifndef __DANGO_DEF_HPP__
#define __DANGO_DEF_HPP__ 1

#ifndef __GNUG__
#error unsupported compiler
#endif

#ifndef DANGO_NO_KEYWORDS
#define restrict __restrict__
#endif

namespace dango
{
  using nullptr_tag = decltype(nullptr);

  using usize = decltype(sizeof(char));
  using isize = decltype(static_cast<char*>(nullptr) - static_cast<char*>(nullptr));

  using u8 = __UINT8_TYPE__;
  using u16 = __UINT16_TYPE__;
  using u32 = __UINT32_TYPE__;
  using u64 = __UINT64_TYPE__;
  using uptr = __UINTPTR_TYPE__;

  using i8 = __INT8_TYPE__;
  using i16 = __INT16_TYPE__;
  using i32 = __INT32_TYPE__;
  using i64 = __INT64_TYPE__;
  using iptr = __INTPTR_TYPE__;

  using wchar = char16_t;
  using dchar = char32_t;

  using real = long double;

  using signed_char = signed char;
  using signed_short = signed short int;
  using signed_int = signed int;
  using signed_long = signed long int;
  using signed_long_long = signed long long int;

  using unsigned_char = unsigned char;
  using unsigned_short = unsigned short int;
  using unsigned_int = unsigned int;
  using unsigned_long = unsigned long int;
  using unsigned_long_long = unsigned long long int;

  enum class
  byte:
  dango::unsigned_char
  {

  };

  struct null_tag
  {

  };

  inline constexpr dango::null_tag const null{};
}

#ifndef DANGO_NO_BASIC_TYPES

#endif

#endif
