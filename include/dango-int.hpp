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
  constexpr auto const max_val_help<dango::builtin::uchar> = dango::builtin::uchar(-1);
  template<>
  constexpr auto const max_val_help<dango::builtin::ushort> = dango::builtin::ushort(-1);
  template<>
  constexpr auto const max_val_help<dango::builtin::uint> = dango::builtin::uint(-1);
  template<>
  constexpr auto const max_val_help<dango::builtin::ulong> = dango::builtin::ulong(-1);
  template<>
  constexpr auto const max_val_help<dango::builtin::ulonglong> = dango::builtin::ulonglong(-1);

  template<>
  constexpr auto const min_val_help<dango::builtin::uchar> = dango::builtin::uchar(0);
  template<>
  constexpr auto const min_val_help<dango::builtin::ushort> = dango::builtin::ushort(0);
  template<>
  constexpr auto const min_val_help<dango::builtin::uint> = dango::builtin::uint(0);
  template<>
  constexpr auto const min_val_help<dango::builtin::ulong> = dango::builtin::ulong(0);
  template<>
  constexpr auto const min_val_help<dango::builtin::ulonglong> = dango::builtin::ulonglong(0);

  template<>
  constexpr auto const max_val_help<dango::builtin::schar> = dango::builtin::schar{ __SCHAR_MAX__ };
  template<>
  constexpr auto const max_val_help<dango::builtin::sshort> = dango::builtin::sshort{ __SHRT_MAX__ };
  template<>
  constexpr auto const max_val_help<dango::builtin::sint> = dango::builtin::sint{ __INT_MAX__ };
  template<>
  constexpr auto const max_val_help<dango::builtin::slong> = dango::builtin::slong{ __LONG_MAX__ };
  template<>
  constexpr auto const max_val_help<dango::builtin::slonglong> = dango::builtin::slonglong{ __LONG_LONG_MAX__ };

  template<>
  constexpr auto const min_val_help<dango::builtin::schar> =
    dango::builtin::schar(-detail::max_val_help<dango::builtin::schar> - 1);

  template<>
  constexpr auto const min_val_help<dango::builtin::sshort> =
    dango::builtin::sshort(-detail::max_val_help<dango::builtin::sshort> - 1);

  template<>
  constexpr auto const min_val_help<dango::builtin::sint> =
    dango::builtin::sint(-detail::max_val_help<dango::builtin::sint> - 1);

  template<>
  constexpr auto const min_val_help<dango::builtin::slong> =
    dango::builtin::slong(-detail::max_val_help<dango::builtin::slong> - 1);

  template<>
  constexpr auto const min_val_help<dango::builtin::slonglong> =
    dango::builtin::slonglong(-detail::max_val_help<dango::builtin::slonglong> - 1);
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

