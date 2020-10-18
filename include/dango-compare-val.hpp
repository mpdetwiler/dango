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
  class compare_val_s;
  class compare_val_w;
  class compare_val_p;
}

template
<dango::detail::is_std_comparison_category tp_cat>
class
dango::
detail::
compare_val_base:
public tp_cat
{
public:
  using int_type = dango::ssize;
protected:
  using super_type = tp_cat;
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
  constexpr auto as_integer()const noexcept->int_type{ return tp_int(is_gt()) - tp_int(is_lt()); }
public:
  constexpr compare_val_base(compare_val_base const&)noexcept = default;
  constexpr compare_val_base(compare_val_base&&)noexcept = default;
  constexpr auto operator = (compare_val_base const&)& noexcept->compare_val_base& = default;
  constexpr auto operator = (compare_val_base&&)& noexcept->compare_val_base& = default;
public:
  constexpr auto is_eq  ()const noexcept->bool{ return *this == 0; }
  constexpr auto is_neq ()const noexcept->bool{ return *this != 0; }
  constexpr auto is_lt  ()const noexcept->bool{ return *this <  0; }
  constexpr auto is_lteq()const noexcept->bool{ return *this <= 0; }
  constexpr auto is_gt  ()const noexcept->bool{ return *this >  0; }
  constexpr auto is_gteq()const noexcept->bool{ return *this >= 0; }
public:
  explicit constexpr compare_val_base()noexcept = delete;
};

class
dango::
compare_val_s
final:
public dango::detail::compare_val_base<std::strong_ordering>
{
private:
  using super_type = dango::detail::compare_val_base<std::strong_ordering>;
public:
  using category_type = category_strong;
public:
  static compare_val_s const equal;
  static compare_val_s const equivalent;
  static compare_val_s const less;
  static compare_val_s const greater;
public:
  explicit constexpr compare_val_s()noexcept:super_type{ 0 }{ }

  template
  <dango::is_sint tp_int>
  constexpr compare_val_s(tp_int const a_int)noexcept:super_type{ a_int }{ }

  constexpr compare_val_s(category_strong const a_arg)noexcept:super_type{ a_arg }{ }

  template
  <dango::is_sint tp_int>
  explicit constexpr operator tp_int()const noexcept{ return as_integer<tp_int>(); }

  constexpr operator dango::compare_val_w()const noexcept;
  constexpr operator dango::compare_val_p()const noexcept;
public:
  constexpr compare_val_s(compare_val_s const&)noexcept = default;
  constexpr compare_val_s(compare_val_s&&)noexcept = default;
  constexpr auto operator = (compare_val_s const&)& noexcept->compare_val_s& = default;
  constexpr auto operator = (compare_val_s&&)& noexcept->compare_val_s& = default;
public:
  constexpr auto as_int()const noexcept->int_type{ return as_integer(); }
  constexpr auto mirror()const noexcept->compare_val_s{ return compare_val_s{ -(as_integer()) }; }
};

inline constexpr dango::compare_val_s const dango::compare_val_s::equal      = compare_val_s{ category_type::equal };
inline constexpr dango::compare_val_s const dango::compare_val_s::equivalent = compare_val_s{ category_type::equivalent };
inline constexpr dango::compare_val_s const dango::compare_val_s::less       = compare_val_s{ category_type::less };
inline constexpr dango::compare_val_s const dango::compare_val_s::greater    = compare_val_s{ category_type::greater };

class
dango::
compare_val_w
final:
public dango::detail::compare_val_base<std::weak_ordering>
{
private:
  using super_type = dango::detail::compare_val_base<std::weak_ordering>;
public:
  using category_type = category_weak;
public:
  static compare_val_w const equivalent;
  static compare_val_w const less;
  static compare_val_w const greater;
public:
  explicit constexpr compare_val_w()noexcept:super_type{ 0 }{ }

  template
  <dango::is_sint tp_int>
  constexpr compare_val_w(tp_int const a_int)noexcept:super_type{ a_int }{ }

  constexpr compare_val_w(category_strong const a_arg)noexcept:super_type{ a_arg }{ }
  constexpr compare_val_w(category_weak const a_arg)noexcept:super_type{ a_arg }{ }

  template
  <dango::is_sint tp_int>
  explicit constexpr operator tp_int()const noexcept{ return as_integer<tp_int>(); }

  constexpr operator dango::compare_val_p()const noexcept;
public:
  constexpr compare_val_w(compare_val_w const&)noexcept = default;
  constexpr compare_val_w(compare_val_w&&)noexcept = default;
  constexpr auto operator = (compare_val_w const&)& noexcept->compare_val_w& = default;
  constexpr auto operator = (compare_val_w&&)& noexcept->compare_val_w& = default;
public:
  constexpr auto as_int()const noexcept->int_type{ return as_integer(); }
  constexpr auto mirror()const noexcept->compare_val_w{ return compare_val_w{ -(as_integer()) }; }
};

inline constexpr dango::compare_val_w const dango::compare_val_w::equivalent = compare_val_w{ category_type::equivalent };
inline constexpr dango::compare_val_w const dango::compare_val_w::less       = compare_val_w{ category_type::less };
inline constexpr dango::compare_val_w const dango::compare_val_w::greater    = compare_val_w{ category_type::greater };

class
dango::
compare_val_p
final:
public dango::detail::compare_val_base<std::partial_ordering>
{
private:
  using super_type = dango::detail::compare_val_base<std::partial_ordering>;
public:
  using category_type = category_partial;
public:
  static compare_val_p const equivalent;
  static compare_val_p const less;
  static compare_val_p const greater;
  static compare_val_p const unordered;
public:
  explicit constexpr compare_val_p()noexcept:super_type{ 0 }{ }

  template
  <dango::is_sint tp_int>
  constexpr compare_val_p(tp_int const a_int)noexcept:super_type{ a_int }{ }

  constexpr compare_val_p(category_strong const a_arg)noexcept:super_type{ a_arg }{ }
  constexpr compare_val_p(category_weak const a_arg)noexcept:super_type{ a_arg }{ }
  constexpr compare_val_p(category_partial const a_arg)noexcept:super_type{ a_arg }{ }
public:
  constexpr compare_val_p(compare_val_p const&)noexcept = default;
  constexpr compare_val_p(compare_val_p&&)noexcept = default;
  constexpr auto operator = (compare_val_p const&)& noexcept->compare_val_p& = default;
  constexpr auto operator = (compare_val_p&&)& noexcept->compare_val_p& = default;
public:
  constexpr auto mirror()const noexcept->compare_val_p;
};

inline constexpr dango::compare_val_p const dango::compare_val_p::equivalent = compare_val_p{ category_type::equivalent };
inline constexpr dango::compare_val_p const dango::compare_val_p::less       = compare_val_p{ category_type::less };
inline constexpr dango::compare_val_p const dango::compare_val_p::greater    = compare_val_p{ category_type::greater };
inline constexpr dango::compare_val_p const dango::compare_val_p::unordered  = compare_val_p{ category_type::unordered };

constexpr auto
dango::
compare_val_p::
mirror
()const noexcept->compare_val_p
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

  return compare_val_p{ -(as_integer()) };
}

constexpr
dango::
compare_val_s::
operator dango::compare_val_w
()const noexcept
{
  return dango::compare_val_w{ static_cast<category_type const&>(*this) };
}

constexpr
dango::
compare_val_s::
operator dango::compare_val_p
()const noexcept
{
  return dango::compare_val_p{ static_cast<category_type const&>(*this) };
}

constexpr
dango::
compare_val_w::
operator dango::compare_val_p
()const noexcept
{
  return dango::compare_val_p{ static_cast<category_type const&>(*this) };
}

namespace
dango::custom
{
  template<>
  struct
  common_type<dango::compare_val_s, dango::compare_val_w>
  final
  {
    using type = dango::compare_val_w;

    DANGO_UNCONSTRUCTIBLE(common_type)
  };

  template<>
  struct
  common_type<dango::compare_val_s, dango::compare_val_p>
  final
  {
    using type = dango::compare_val_p;

    DANGO_UNCONSTRUCTIBLE(common_type)
  };

  template<>
  struct
  common_type<dango::compare_val_w, dango::compare_val_p>
  final
  {
    using type = dango::compare_val_p;

    DANGO_UNCONSTRUCTIBLE(common_type)
  };
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_compare_val =
    dango::is_same_ignore_cv<tp_type, dango::compare_val_s> ||
    dango::is_same_ignore_cv<tp_type, dango::compare_val_w> ||
    dango::is_same_ignore_cv<tp_type, dango::compare_val_p>;
}

namespace
dango::detail
{
  using strongest_comparison_prio = dango::priority_tag<dango::uint(2)>;

  constexpr auto strongest_comparison_help(dango::priority_tag<dango::uint(2)> const, dango::compare_val_s const a_val)noexcept->auto{ return a_val; }
  constexpr auto strongest_comparison_help(dango::priority_tag<dango::uint(1)> const, dango::compare_val_w const a_val)noexcept->auto{ return a_val; }
  constexpr auto strongest_comparison_help(dango::priority_tag<dango::uint(0)> const, dango::compare_val_p const a_val)noexcept->auto{ return a_val; }
}

namespace
dango::comparison
{
  template
  <typename tp_type>
  concept is_convertible =
    dango::is_object_exclude_array_ignore_ref<tp_type> &&
    requires{ { dango::detail::strongest_comparison_help(dango::detail::strongest_comparison_prio{ }, dango::declval<tp_type>()) }; };

  template
  <typename tp_type>
  concept is_noexcept_convertible =
    dango::comparison::is_convertible<tp_type> &&
    requires{ { dango::detail::strongest_comparison_help(dango::detail::strongest_comparison_prio{ }, dango::declval<tp_type>()) }noexcept; };

  template
  <typename tp_arg>
  requires(dango::comparison::is_convertible<tp_arg>)
  constexpr auto
  strongest(tp_arg&& a_arg)
  noexcept(dango::comparison::is_noexcept_convertible<tp_arg>)->auto
  {
    return dango::detail::strongest_comparison_help(dango::detail::strongest_comparison_prio{ }, dango::forward<tp_arg>(a_arg));
  }

  template
  <dango::is_compare_val... tp_types>
  using common_type =
    dango::common_type<dango::compare_val_s, dango::remove_cv<tp_types>...>;

  constexpr auto is_eq  (dango::compare_val_p const a_val)noexcept->bool{ return a_val.is_eq();   }
  constexpr auto is_neq (dango::compare_val_p const a_val)noexcept->bool{ return a_val.is_neq();  }
  constexpr auto is_lt  (dango::compare_val_p const a_val)noexcept->bool{ return a_val.is_lt();   }
  constexpr auto is_lteq(dango::compare_val_p const a_val)noexcept->bool{ return a_val.is_lteq(); }
  constexpr auto is_gt  (dango::compare_val_p const a_val)noexcept->bool{ return a_val.is_gt();   }
  constexpr auto is_gteq(dango::compare_val_p const a_val)noexcept->bool{ return a_val.is_gteq(); }

  constexpr auto mirror(dango::compare_val_s const a_val)noexcept->auto{ return a_val.mirror(); }
  constexpr auto mirror(dango::compare_val_w const a_val)noexcept->auto{ return a_val.mirror(); }
  constexpr auto mirror(dango::compare_val_p const a_val)noexcept->auto{ return a_val.mirror(); }
}

#endif // DANGO_COMPARE_VAL_HPP_INCLUDED
