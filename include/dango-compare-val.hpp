#ifndef DANGO_COMPARE_VAL_HPP_INCLUDED
#define DANGO_COMPARE_VAL_HPP_INCLUDED

#include "dango-traits.hpp"

/*** compare_val ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  concept is_std_comparison_category =
    dango::is_same<tp_type, std::strong_ordering> ||
    dango::is_same<tp_type, std::weak_ordering> ||
    dango::is_same<tp_type, std::partial_ordering>;

  template
  <dango::detail::is_std_comparison_category tp_cat>
  class compare_val_base;
}

namespace
dango
{
  class compare_val_strong;
  class compare_val_weak;
  class compare_val_partial;
}

template
<dango::detail::is_std_comparison_category tp_cat>
class
dango::
detail::
compare_val_base:
public tp_cat
{
protected:
  using super_type = tp_cat;
  using int_type = dango::ssize;
  using category_strong = std::strong_ordering;
  using category_weak = std::weak_ordering;
  using category_partial = std::partial_ordering;
private:
  static inline constexpr super_type const c_map[dango::usize(3)] =
  {
    super_type::less,
    super_type::equivalent,
    super_type::greater
  };
protected:
  template
  <dango::is_sint tp_int>
  explicit constexpr
  compare_val_base(tp_int const a_int)noexcept:
  super_type{ c_map[int_type(1) + int_type(a_int > tp_int(0)) - int_type(a_int < tp_int(0))] }
  { }
  template
  <dango::detail::is_std_comparison_category tp_arg>
  requires(dango::is_noexcept_convertible_arg<tp_arg, super_type> && dango::is_noexcept_brace_constructible<super_type, tp_arg const&>)
  explicit constexpr
  compare_val_base(tp_arg const a_arg)noexcept:
  super_type{ a_arg }
  { }
protected:
  template
  <dango::is_sint tp_int = int_type>
  constexpr auto as_integer()const noexcept->int_type{ return tp_int(*this > 0) - tp_int(*this < 0); }
public:
  constexpr compare_val_base(compare_val_base const&)noexcept = default;
  constexpr compare_val_base(compare_val_base&&)noexcept = default;
  constexpr auto operator = (compare_val_base const&)& noexcept->compare_val_base& = default;
  constexpr auto operator = (compare_val_base&&)& noexcept->compare_val_base& = default;
public:
  constexpr auto is_eq()const noexcept->bool{ return   *this == 0; }
  constexpr auto is_neq()const noexcept->bool{ return  *this != 0; }
  constexpr auto is_lt()const noexcept->bool{ return   *this <  0; }
  constexpr auto is_lteq()const noexcept->bool{ return *this <= 0; }
  constexpr auto is_gt()const noexcept->bool{ return   *this >  0; }
  constexpr auto is_gteq()const noexcept->bool{ return *this >= 0; }
public:
  explicit constexpr compare_val_base()noexcept = delete;
};

class
dango::
compare_val_strong
final:
public dango::detail::compare_val_base<std::strong_ordering>
{
private:
  using super_type = dango::detail::compare_val_base<std::strong_ordering>;
public:
  using category_type = category_strong;
public:
  static compare_val_strong const equal;
  static compare_val_strong const equivalent;
  static compare_val_strong const less;
  static compare_val_strong const greater;
public:
  explicit constexpr compare_val_strong()noexcept:super_type{ 0 }{ }

  template
  <dango::is_sint tp_int>
  constexpr compare_val_strong(tp_int const a_int)noexcept:super_type{ a_int }{ }

  constexpr compare_val_strong(category_strong const a_arg)noexcept:super_type{ a_arg }{ }

  template
  <dango::is_sint tp_int>
  explicit constexpr operator tp_int()const noexcept{ return as_integer<tp_int>(); }

  constexpr operator dango::compare_val_weak()const noexcept;
  constexpr operator dango::compare_val_partial()const noexcept;
public:
  constexpr compare_val_strong(compare_val_strong const&)noexcept = default;
  constexpr compare_val_strong(compare_val_strong&&)noexcept = default;
  constexpr auto operator = (compare_val_strong const&)& noexcept->compare_val_strong& = default;
  constexpr auto operator = (compare_val_strong&&)& noexcept->compare_val_strong& = default;
public:
  constexpr auto as_int()const noexcept->int_type{ return as_integer(); }
  constexpr auto mirror()const noexcept->compare_val_strong{ return compare_val_strong{ -(as_integer()) }; }
};

inline constexpr dango::compare_val_strong const dango::compare_val_strong::equal = compare_val_strong{ category_type::equal };
inline constexpr dango::compare_val_strong const dango::compare_val_strong::equivalent = compare_val_strong{ category_type::equivalent };
inline constexpr dango::compare_val_strong const dango::compare_val_strong::less = compare_val_strong{ category_type::less };
inline constexpr dango::compare_val_strong const dango::compare_val_strong::greater = compare_val_strong{ category_type::greater };

class
dango::
compare_val_weak
final:
public dango::detail::compare_val_base<std::weak_ordering>
{
private:
  using super_type = dango::detail::compare_val_base<std::weak_ordering>;
public:
  using category_type = category_weak;
public:
  static compare_val_weak const equivalent;
  static compare_val_weak const less;
  static compare_val_weak const greater;
public:
  explicit constexpr compare_val_weak()noexcept:super_type{ 0 }{ }

  template
  <dango::is_sint tp_int>
  constexpr compare_val_weak(tp_int const a_int)noexcept:super_type{ a_int }{ }

  constexpr compare_val_weak(category_strong const a_arg)noexcept:super_type{ a_arg }{ }
  constexpr compare_val_weak(category_weak const a_arg)noexcept:super_type{ a_arg }{ }

  template
  <dango::is_sint tp_int>
  explicit constexpr operator tp_int()const noexcept{ return as_integer<tp_int>(); }

  constexpr operator dango::compare_val_partial()const noexcept;
public:
  constexpr compare_val_weak(compare_val_weak const&)noexcept = default;
  constexpr compare_val_weak(compare_val_weak&&)noexcept = default;
  constexpr auto operator = (compare_val_weak const&)& noexcept->compare_val_weak& = default;
  constexpr auto operator = (compare_val_weak&&)& noexcept->compare_val_weak& = default;
public:
  constexpr auto as_int()const noexcept->int_type{ return as_integer(); }
  constexpr auto mirror()const noexcept->compare_val_weak{ return compare_val_weak{ -(as_integer()) }; }
};

inline constexpr dango::compare_val_weak const dango::compare_val_weak::equivalent = compare_val_weak{ category_type::equivalent };
inline constexpr dango::compare_val_weak const dango::compare_val_weak::less = compare_val_weak{ category_type::less };
inline constexpr dango::compare_val_weak const dango::compare_val_weak::greater = compare_val_weak{ category_type::greater };

class
dango::
compare_val_partial
final:
public dango::detail::compare_val_base<std::partial_ordering>
{
private:
  using super_type = dango::detail::compare_val_base<std::partial_ordering>;
public:
  using category_type = category_partial;
public:
  static compare_val_partial const equivalent;
  static compare_val_partial const less;
  static compare_val_partial const greater;
  static compare_val_partial const unordered;
public:
  explicit constexpr compare_val_partial()noexcept:super_type{ 0 }{ }

  template
  <dango::is_sint tp_int>
  constexpr compare_val_partial(tp_int const a_int)noexcept:super_type{ a_int }{ }

  constexpr compare_val_partial(category_strong const a_arg)noexcept:super_type{ a_arg }{ }
  constexpr compare_val_partial(category_weak const a_arg)noexcept:super_type{ a_arg }{ }
  constexpr compare_val_partial(category_partial const a_arg)noexcept:super_type{ a_arg }{ }
public:
  constexpr compare_val_partial(compare_val_partial const&)noexcept = default;
  constexpr compare_val_partial(compare_val_partial&&)noexcept = default;
  constexpr auto operator = (compare_val_partial const&)& noexcept->compare_val_partial& = default;
  constexpr auto operator = (compare_val_partial&&)& noexcept->compare_val_partial& = default;
public:
  constexpr auto mirror()const noexcept->compare_val_partial;
};

inline constexpr dango::compare_val_partial const dango::compare_val_partial::equivalent = compare_val_partial{ category_type::equivalent };
inline constexpr dango::compare_val_partial const dango::compare_val_partial::less = compare_val_partial{ category_type::less };
inline constexpr dango::compare_val_partial const dango::compare_val_partial::greater = compare_val_partial{ category_type::greater };
inline constexpr dango::compare_val_partial const dango::compare_val_partial::unordered = compare_val_partial{ category_type::unordered };

constexpr auto
dango::
compare_val_partial::
mirror
()const noexcept->compare_val_partial
{
#ifdef DANGO_USING_CLANG
  if(!is_lt() && !is_eq() && !is_gt())
  {
    return *this;
  }
#else
  if(*this == unordered)
  {
    return *this;
  }
#endif

  return compare_val_partial{ -(as_integer()) };
}

constexpr
dango::
compare_val_strong::
operator dango::compare_val_weak
()const noexcept
{
  return dango::compare_val_weak{ static_cast<category_type const&>(*this) };
}

constexpr
dango::
compare_val_strong::
operator dango::compare_val_partial
()const noexcept
{
  return dango::compare_val_partial{ static_cast<category_type const&>(*this) };
}

constexpr
dango::
compare_val_weak::
operator dango::compare_val_partial
()const noexcept
{
  return dango::compare_val_partial{ static_cast<category_type const&>(*this) };
}

namespace
dango::custom
{
  template<>
  struct
  common_type<dango::compare_val_strong, dango::compare_val_weak>
  final
  {
    using type = dango::compare_val_weak;

    DANGO_UNCONSTRUCTIBLE(common_type)
  };

  template<>
  struct
  common_type<dango::compare_val_strong, dango::compare_val_partial>
  final
  {
    using type = dango::compare_val_partial;

    DANGO_UNCONSTRUCTIBLE(common_type)
  };

  template<>
  struct
  common_type<dango::compare_val_weak, dango::compare_val_partial>
  final
  {
    using type = dango::compare_val_partial;

    DANGO_UNCONSTRUCTIBLE(common_type)
  };
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_compare_val =
    dango::is_same_ignore_cv<tp_type, dango::compare_val_strong> ||
    dango::is_same_ignore_cv<tp_type, dango::compare_val_weak> ||
    dango::is_same_ignore_cv<tp_type, dango::compare_val_partial>;
}

namespace
dango::detail
{
  using strongest_comparison_prio = dango::priority_tag<dango::uint(2)>;

  constexpr auto strongest_comparison_help(dango::priority_tag<dango::uint(2)> const, dango::compare_val_strong const a_val)noexcept->auto{ return a_val; }
  constexpr auto strongest_comparison_help(dango::priority_tag<dango::uint(1)> const, dango::compare_val_weak const a_val)noexcept->auto{ return a_val; }
  constexpr auto strongest_comparison_help(dango::priority_tag<dango::uint(0)> const, dango::compare_val_partial const a_val)noexcept->auto{ return a_val; }
}

namespace
dango::comparison
{
  template
  <typename tp_type>
  concept is_convertible =
    dango::is_object_exclude_array_ignore_ref<tp_type> &&
    requires{ { dango::detail::strongest_comparison_help(dango::detail::strongest_comparison_prio{ }, dango::declval<tp_type>()) }noexcept; };

  template
  <dango::comparison::is_convertible tp_arg>
  constexpr auto
  strongest(tp_arg&& a_arg)noexcept->auto
  {
    return dango::detail::strongest_comparison_help(dango::detail::strongest_comparison_prio{ }, dango::forward<tp_arg>(a_arg));
  }

  template
  <dango::is_compare_val... tp_types>
  using common_type =
    dango::common_type<dango::compare_val_strong, dango::remove_cv<tp_types>...>;

  constexpr auto is_eq(dango::compare_val_partial const a_val)noexcept->bool{ return a_val.is_eq(); }
  constexpr auto is_neq(dango::compare_val_partial const a_val)noexcept->bool{ return a_val.is_neq(); }
  constexpr auto is_lt(dango::compare_val_partial const a_val)noexcept->bool{ return a_val.is_lt(); }
  constexpr auto is_lteq(dango::compare_val_partial const a_val)noexcept->bool{ return a_val.is_lteq(); }
  constexpr auto is_gt(dango::compare_val_partial const a_val)noexcept->bool{ return a_val.is_gt(); }
  constexpr auto is_gteq(dango::compare_val_partial const a_val)noexcept->bool{ return a_val.is_gteq(); }

  constexpr auto mirror(dango::compare_val_strong const a_val)noexcept->auto{ return a_val.mirror(); }
  constexpr auto mirror(dango::compare_val_weak const a_val)noexcept->auto{ return a_val.mirror(); }
  constexpr auto mirror(dango::compare_val_partial const a_val)noexcept->auto{ return a_val.mirror(); }
}

namespace
dango
{
  using compare_val = dango::compare_val_strong;
}

#endif // DANGO_COMPARE_VAL_HPP_INCLUDED
