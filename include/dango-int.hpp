#ifndef __DANGO_INT_HPP__
#define __DANGO_INT_HPP__

namespace
dango::detail
{
  template
  <typename tp_integer>
  constexpr dango::incomplete_type const max_val_help;

  template
  <typename tp_integer>
  constexpr dango::incomplete_type const min_val_help;

  template<>
  constexpr auto const max_val_help<dango::u_byte> = dango::u_byte(-1);
  template<>
  constexpr auto const max_val_help<dango::u_short> = dango::u_short(-1);
  template<>
  constexpr auto const max_val_help<dango::u_int> = dango::u_int(-1);
  template<>
  constexpr auto const max_val_help<dango::u_long> = dango::u_long(-1);
  template<>
  constexpr auto const max_val_help<dango::u_cent> = dango::u_cent(-1);

  template<>
  constexpr auto const min_val_help<dango::u_byte> = dango::u_byte(0);
  template<>
  constexpr auto const min_val_help<dango::u_short> = dango::u_short(0);
  template<>
  constexpr auto const min_val_help<dango::u_int> = dango::u_int(0);
  template<>
  constexpr auto const min_val_help<dango::u_long> = dango::u_long(0);
  template<>
  constexpr auto const min_val_help<dango::u_cent> = dango::u_cent(0);

  template<>
  constexpr auto const max_val_help<dango::s_byte> = dango::s_byte{ __SCHAR_MAX__ };
  template<>
  constexpr auto const max_val_help<dango::s_short> = dango::s_short{ __SHRT_MAX__ };
  template<>
  constexpr auto const max_val_help<dango::s_int> = dango::s_int{ __INT_MAX__ };
  template<>
  constexpr auto const max_val_help<dango::s_long> = dango::s_long{ __LONG_MAX__ };
  template<>
  constexpr auto const max_val_help<dango::s_cent> = dango::s_cent{ __LONG_LONG_MAX__ };

  template<>
  constexpr auto const min_val_help<dango::s_byte> =
    dango::s_byte(-detail::max_val_help<dango::s_byte> - 1);

  template<>
  constexpr auto const min_val_help<dango::s_short> =
    dango::s_short(-detail::max_val_help<dango::s_short> - 1);

  template<>
  constexpr auto const min_val_help<dango::s_int> =
    dango::s_int(-detail::max_val_help<dango::s_int> - 1);

  template<>
  constexpr auto const min_val_help<dango::s_long> =
    dango::s_long(-detail::max_val_help<dango::s_long> - 1);

  template<>
  constexpr auto const min_val_help<dango::s_cent> =
    dango::s_cent(-detail::max_val_help<dango::s_cent> - 1);
}

namespace
dango::integer
{
  template
  <typename tp_integer, typename tp_enabled = dango::enable_tag>
  constexpr dango::incomplete_type const MAX_VAL;

  template
  <typename tp_integer>
  constexpr dango::remove_cv<tp_integer> const
  MAX_VAL<tp_integer, dango::enable_if<dango::is_int<tp_integer>>> =
    dango::detail::max_val_help<dango::remove_cv<tp_integer>>;

  template
  <typename tp_integer, typename tp_enabled = dango::enable_tag>
  constexpr dango::incomplete_type const MIN_VAL;

  template
  <typename tp_integer>
  constexpr dango::remove_cv<tp_integer> const
  MIN_VAL<tp_integer, dango::enable_if<dango::is_int<tp_integer>>> =
    dango::detail::min_val_help<dango::remove_cv<tp_integer>>;
}

namespace
dango
{
	inline constexpr dango::usize const SIZE_MAX_VAL =
		dango::integer::MAX_VAL<dango::usize> <= dango::integer::MAX_VAL<dango::ssize> ?
		dango::integer::MAX_VAL<dango::usize> :
		dango::usize(dango::integer::MAX_VAL<dango::ssize>);
}

#endif

