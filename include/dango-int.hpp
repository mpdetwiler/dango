#ifndef DANGO_INT_HPP_INCLUDED
#define DANGO_INT_HPP_INCLUDED

namespace
dango::detail
{
  template
  <dango::is_int tp_integer>
  requires(!dango::is_const_or_volatile<tp_integer>)
  inline constexpr auto const max_val_help = tp_integer(0);

  template
  <dango::is_int tp_integer>
  requires(!dango::is_const_or_volatile<tp_integer>)
  inline constexpr auto const min_val_help = tp_integer(0);

  template<>
  inline constexpr auto const max_val_help<dango::builtin::uchar> = dango::builtin::uchar(-1);
  template<>
  inline constexpr auto const max_val_help<dango::builtin::ushort> = dango::builtin::ushort(-1);
  template<>
  inline constexpr auto const max_val_help<dango::builtin::uint> = dango::builtin::uint(-1);
  template<>
  inline constexpr auto const max_val_help<dango::builtin::ulong> = dango::builtin::ulong(-1);
  template<>
  inline constexpr auto const max_val_help<dango::builtin::ulonglong> = dango::builtin::ulonglong(-1);

  template<>
  inline constexpr auto const min_val_help<dango::builtin::uchar> = dango::builtin::uchar(0);
  template<>
  inline constexpr auto const min_val_help<dango::builtin::ushort> = dango::builtin::ushort(0);
  template<>
  inline constexpr auto const min_val_help<dango::builtin::uint> = dango::builtin::uint(0);
  template<>
  inline constexpr auto const min_val_help<dango::builtin::ulong> = dango::builtin::ulong(0);
  template<>
  inline constexpr auto const min_val_help<dango::builtin::ulonglong> = dango::builtin::ulonglong(0);

  template<>
  inline constexpr auto const max_val_help<dango::builtin::schar> = dango::builtin::schar{ SCHAR_MAX };
  template<>
  inline constexpr auto const max_val_help<dango::builtin::sshort> = dango::builtin::sshort{ SHRT_MAX };
  template<>
  inline constexpr auto const max_val_help<dango::builtin::sint> = dango::builtin::sint{ INT_MAX };
  template<>
  inline constexpr auto const max_val_help<dango::builtin::slong> = dango::builtin::slong{ LONG_MAX };
  template<>
  inline constexpr auto const max_val_help<dango::builtin::slonglong> = dango::builtin::slonglong{ LLONG_MAX };

  template<>
  inline constexpr auto const min_val_help<dango::builtin::schar> = dango::builtin::schar{ SCHAR_MIN };
  template<>
  inline constexpr auto const min_val_help<dango::builtin::sshort> = dango::builtin::sshort{ SHRT_MIN };
  template<>
  inline constexpr auto const min_val_help<dango::builtin::sint> = dango::builtin::sint{ INT_MIN };
  template<>
  inline constexpr auto const min_val_help<dango::builtin::slong> = dango::builtin::slong{ LONG_MIN };
  template<>
  inline constexpr auto const min_val_help<dango::builtin::slonglong> = dango::builtin::slonglong{ LLONG_MIN };
}

namespace
dango::integer
{
  template
  <dango::is_int tp_integer>
  inline constexpr auto const MIN_VAL = dango::detail::min_val_help<dango::remove_cv<tp_integer>>;

  template
  <dango::is_int tp_integer>
  inline constexpr auto const MAX_VAL = dango::detail::max_val_help<dango::remove_cv<tp_integer>>;
}

namespace
dango
{
	inline constexpr dango::usize const SIZE_MAX_VAL =
		dango::integer::MAX_VAL<dango::usize> <= dango::integer::MAX_VAL<dango::ssize> ?
		dango::integer::MAX_VAL<dango::usize> :
		dango::usize(dango::integer::MAX_VAL<dango::ssize>);
}

/*** signed safe arithmetic tests ***/

namespace
dango::integer
{
  template
  <dango::is_sint tp_int>
  constexpr auto
  safe_add_test
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->bool
  {
    if(a_rhs >= tp_int(0))
    {
      return a_lhs <= tp_int(dango::integer::MAX_VAL<tp_int> - a_rhs);
    }

    return a_lhs >= tp_int(dango::integer::MIN_VAL<tp_int> - a_rhs);
  }

  template
  <dango::is_sint tp_int>
  constexpr auto
  safe_sub_test
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->bool
  {
    if(a_rhs >= tp_int(0))
    {
      return a_lhs >= tp_int(dango::integer::MIN_VAL<tp_int> + a_rhs);
    }

    return a_lhs <= tp_int(dango::integer::MAX_VAL<tp_int> + a_rhs);
  }

  template
  <dango::is_sint tp_int>
  constexpr auto
  safe_mul_test
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->bool
  {
    if(a_lhs == tp_int(0) || a_rhs == tp_int(0))
    {
      return true;
    }

    constexpr auto const c_min = dango::integer::MIN_VAL<tp_int>;
    constexpr auto const c_max = dango::integer::MAX_VAL<tp_int>;

    bool const a_lp = (a_lhs > tp_int(0));
    bool const a_rp = (a_rhs > tp_int(0));

    if(a_lp && a_rp)
    {
      return a_lhs <= tp_int(c_max / a_rhs);
    }

    if(a_lp)
    {
      return a_rhs >= tp_int(c_min / a_lhs);
    }

    if(a_rp)
    {
      return a_lhs >= tp_int(c_min / a_rhs);
    }

    if(a_lhs == c_min || a_rhs == c_min)
    {
      return false;
    }

    return (-a_lhs) <= tp_int(c_max / (-a_rhs));
  }

  template
  <dango::is_sint tp_int>
  constexpr auto
  safe_div_test
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->bool
  {
    if(a_rhs == tp_int(0))
    {
      return false;
    }

    if((a_lhs == dango::integer::MIN_VAL<tp_int>) && a_rhs == tp_int(-1))
    {
      return false;
    }

    return true;
  }
}

/*** unsigned safe arithmetic tests ***/

namespace
dango::integer
{
  template
  <dango::is_uint tp_int>
  constexpr auto
  safe_add_test
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->bool
  {
    return a_lhs <= tp_int(dango::integer::MAX_VAL<tp_int> - a_rhs);
  }

  template
  <dango::is_uint tp_int>
  constexpr auto
  safe_sub_test
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->bool
  {
    return a_lhs >= a_rhs;
  }

  template
  <dango::is_uint tp_int>
  constexpr auto
  safe_mul_test
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->bool
  {
    if(a_rhs == tp_int(0))
    {
      return true;
    }

    return a_lhs <= tp_int(dango::integer::MAX_VAL<tp_int> / a_rhs);
  }

  template
  <dango::is_uint tp_int>
  constexpr auto
  safe_div_test
  (tp_int const, tp_int const a_rhs)noexcept->bool
  {
    return a_rhs != tp_int(0);
  }
}

/*** safe arithmetic ***/

namespace
dango::integer
{
  template
  <dango::is_int tp_int>
  constexpr auto
  safe_add
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->tp_int
  {
    dango_assert(dango::integer::safe_add_test(a_lhs, a_rhs));

    return tp_int(a_lhs + a_rhs);
  }

  template
  <dango::is_int tp_int>
  constexpr auto
  safe_sub
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->tp_int
  {
    dango_assert(dango::integer::safe_sub_test(a_lhs, a_rhs));

    return tp_int(a_lhs - a_rhs);
  }

  template
  <dango::is_int tp_int>
  constexpr auto
  safe_mul
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->tp_int
  {
    dango_assert(dango::integer::safe_mul_test(a_lhs, a_rhs));

    return tp_int(a_lhs * a_rhs);
  }

  template
  <dango::is_int tp_int>
  constexpr auto
  safe_div
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->tp_int
  {
    dango_assert(dango::integer::safe_div_test(a_lhs, a_rhs));

    return tp_int(a_lhs / a_rhs);
  }
}

#endif // DANGO_INT_HPP_INCLUDED

