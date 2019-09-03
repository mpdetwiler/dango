#ifndef __DANGO_INT_HPP__
#define __DANGO_INT_HPP__

namespace
dango::detail
{
  template
  <typename tp_integer>
  constexpr dango::bad_variable_template<tp_integer> max_val_help;

  template
  <typename tp_integer>
  constexpr dango::bad_variable_template<tp_integer> min_val_help;

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

//constexpr auto const bad_inst = dango::detail::max_val_help<float>;

namespace
dango::integer
{
  template
  <typename tp_integer, typename tp_enabled = dango::enable_tag>
  constexpr dango::bad_variable_template<tp_integer, tp_enabled> MAX_VAL;

  template
  <typename tp_integer>
  constexpr dango::remove_cv<tp_integer> const
  MAX_VAL<tp_integer, dango::enable_if<dango::is_int<tp_integer>>> =
    dango::detail::max_val_help<dango::remove_cv<tp_integer>>;

  template
  <typename tp_integer, typename tp_enabled = dango::enable_tag>
  constexpr dango::bad_variable_template<tp_integer, tp_enabled> MIN_VAL;

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

/*** signed safe arithmetic tests ***/

namespace
dango::integer
{
  template
  <typename tp_int, dango::enable_if<dango::is_sint<tp_int>> = dango::enable_val>
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
  <typename tp_int, dango::enable_if<dango::is_sint<tp_int>> = dango::enable_val>
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
  <typename tp_int, dango::enable_if<dango::is_sint<tp_int>> = dango::enable_val>
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
  <typename tp_int, dango::enable_if<dango::is_sint<tp_int>> = dango::enable_val>
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
  <typename tp_int, dango::enable_if<dango::is_uint<tp_int>> = dango::enable_val>
  constexpr auto
  safe_add_test
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->bool
  {
    return a_lhs <= tp_int(dango::integer::MAX_VAL<tp_int> - a_rhs);
  }

  template
  <typename tp_int, dango::enable_if<dango::is_uint<tp_int>> = dango::enable_val>
  constexpr auto
  safe_sub_test
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->bool
  {
    return a_lhs >= a_rhs;
  }

  template
  <typename tp_int, dango::enable_if<dango::is_uint<tp_int>> = dango::enable_val>
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
  <typename tp_int, dango::enable_if<dango::is_uint<tp_int>> = dango::enable_val>
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
  <typename tp_int, dango::enable_if<dango::is_int<tp_int>> = dango::enable_val>
  constexpr auto
  safe_add
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->tp_int
  {
    dango_assert(dango::integer::safe_add_test(a_lhs, a_rhs));

    return tp_int(a_lhs + a_rhs);
  }

  template
  <typename tp_int, dango::enable_if<dango::is_int<tp_int>> = dango::enable_val>
  constexpr auto
  safe_sub
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->tp_int
  {
    dango_assert(dango::integer::safe_sub_test(a_lhs, a_rhs));

    return tp_int(a_lhs - a_rhs);
  }

  template
  <typename tp_int, dango::enable_if<dango::is_int<tp_int>> = dango::enable_val>
  constexpr auto
  safe_mul
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->tp_int
  {
    dango_assert(dango::integer::safe_mul_test(a_lhs, a_rhs));

    return tp_int(a_lhs * a_rhs);
  }

  template
  <typename tp_int, dango::enable_if<dango::is_int<tp_int>> = dango::enable_val>
  constexpr auto
  safe_div
  (tp_int const a_lhs, tp_int const a_rhs)noexcept->tp_int
  {
    dango_assert(dango::integer::safe_div_test(a_lhs, a_rhs));

    return tp_int(a_lhs / a_rhs);
  }
}

#endif

