#ifndef __DANGO_INT_HPP__
#define __DANGO_INT_HPP__ 1

#include "dango-def.hpp"

namespace
dango::integer
{
  template
  <typename tp_integer>
  constexpr tp_integer const MAX_VAL;

  template
  <typename tp_integer>
  constexpr tp_integer const MIN_VAL;

  template<>
  constexpr auto const MAX_VAL<dango::u_byte> = dango::u_byte(-1);
  template<>
  constexpr auto const MAX_VAL<dango::u_short> = dango::u_short(-1);
  template<>
  constexpr auto const MAX_VAL<dango::u_int> = dango::u_int(-1);
  template<>
  constexpr auto const MAX_VAL<dango::u_long> = dango::u_long(-1);
  template<>
  constexpr auto const MAX_VAL<dango::u_cent> = dango::u_cent(-1);

  template<>
  constexpr auto const MIN_VAL<dango::u_byte> = dango::u_byte(0);
  template<>
  constexpr auto const MIN_VAL<dango::u_short> = dango::u_short(0);
  template<>
  constexpr auto const MIN_VAL<dango::u_int> = dango::u_int(0);
  template<>
  constexpr auto const MIN_VAL<dango::u_long> = dango::u_long(0);
  template<>
  constexpr auto const MIN_VAL<dango::u_cent> = dango::u_cent(0);

  template<>
  constexpr auto const MAX_VAL<dango::s_byte> = dango::s_byte{ __SCHAR_MAX__ };
  template<>
  constexpr auto const MAX_VAL<dango::s_short> = dango::s_short{ __SHRT_MAX__ };
  template<>
  constexpr auto const MAX_VAL<dango::s_int> = dango::s_int{ __INT_MAX__ };
  template<>
  constexpr auto const MAX_VAL<dango::s_long> = dango::s_long{ __LONG_MAX__ };
  template<>
  constexpr auto const MAX_VAL<dango::s_cent> = dango::s_cent{ __LONG_LONG_MAX__ };

  template<>
  constexpr auto const MIN_VAL<dango::s_byte> =
    dango::s_byte(-dango::integer::MAX_VAL<dango::s_byte> - 1);

  template<>
  constexpr auto const MIN_VAL<dango::s_short> =
    dango::s_short(-dango::integer::MAX_VAL<dango::s_short> - 1);

  template<>
  constexpr auto const MIN_VAL<dango::s_int> =
    dango::s_int(-dango::integer::MAX_VAL<dango::s_int> - 1);

  template<>
  constexpr auto const MIN_VAL<dango::s_long> =
    dango::s_long(-dango::integer::MAX_VAL<dango::s_long> - 1);

  template<>
  constexpr auto const MIN_VAL<dango::s_cent> =
    dango::s_cent(-dango::integer::MAX_VAL<dango::s_cent> - 1);

  template
  <typename tp_integer>
  constexpr auto const MAX_VAL<tp_integer const> = dango::integer::MAX_VAL<tp_integer>;
  template
  <typename tp_integer>
  constexpr auto const MAX_VAL<tp_integer volatile> = dango::integer::MAX_VAL<tp_integer>;
  template
  <typename tp_integer>
  constexpr auto const MAX_VAL<tp_integer const volatile> = dango::integer::MAX_VAL<tp_integer>;

  template
  <typename tp_integer>
  constexpr auto const MIN_VAL<tp_integer const> = dango::integer::MIN_VAL<tp_integer>;
  template
  <typename tp_integer>
  constexpr auto const MIN_VAL<tp_integer volatile> = dango::integer::MIN_VAL<tp_integer>;
  template
  <typename tp_integer>
  constexpr auto const MIN_VAL<tp_integer const volatile> = dango::integer::MIN_VAL<tp_integer>;
}

namespace
dango
{
	inline constexpr dango::usize const SIZE_MAX =
		dango::integer::MAX_VAL<dango::usize> <= dango::integer::MAX_VAL<dango::ssize> ?
		dango::integer::MAX_VAL<dango::usize> :
		dango::usize(dango::integer::MAX_VAL<dango::ssize>);
}

#endif

