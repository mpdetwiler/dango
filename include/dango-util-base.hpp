#ifndef DANGO_UTIL_BASE_HPP_INCLUDED
#define DANGO_UTIL_BASE_HPP_INCLUDED

/*** priority_tag ***/

namespace
dango
{
  template
  <dango::uint tp_prio>
  struct priority_tag;

  template<>
  struct priority_tag<dango::uint(0)>;
}

template
<dango::uint tp_prio>
struct
dango::
priority_tag:
dango::priority_tag<tp_prio - dango::uint(1)>
{
  DANGO_TAG_TYPE(priority_tag)
};

template<>
struct
dango::
priority_tag<dango::uint(0)>
{
  DANGO_TAG_TYPE(priority_tag)
};

/*** integer_seq ***/

namespace
dango
{
  template
  <typename tp_int, tp_int... tp_integers>
  struct
  integer_seq
  final
  {
    DANGO_TAG_TYPE(integer_seq)
  };

#ifdef __clang__
  template
  <typename tp_int, tp_int tp_len>
  using make_integer_seq = __make_integer_seq<dango::integer_seq, tp_int, tp_len>;
#else
  template
  <typename tp_int, tp_int tp_len>
  using make_integer_seq = dango::integer_seq<tp_int, __integer_pack(tp_len)...>;
#endif

  template
  <dango::usize... tp_indices>
  using index_seq = dango::integer_seq<dango::usize, tp_indices...>;

  template
  <dango::usize tp_len>
  using make_index_seq = dango::make_integer_seq<dango::usize, tp_len>;
}

/*** move forward ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr auto move(tp_type&&)noexcept->dango::remove_ref<tp_type>&&;

  template
  <typename tp_type>
  constexpr auto forward(dango::remove_ref<tp_type>&)noexcept->tp_type&&;

  template
  <typename tp_type>
  constexpr auto forward(dango::remove_ref<tp_type>&&)noexcept->tp_type&&;
}

template
<typename tp_type>
constexpr auto
dango::
move
(tp_type&& a_arg)noexcept->dango::remove_ref<tp_type>&&
{
  using ret_type = dango::remove_ref<tp_type>&&;

  return static_cast<ret_type>(a_arg);
}

template
<typename tp_type>
constexpr auto
dango::
forward
(dango::remove_ref<tp_type>& a_arg)noexcept->tp_type&&
{
  return static_cast<tp_type&&>(a_arg);
}

template
<typename tp_type>
constexpr auto
dango::
forward
(dango::remove_ref<tp_type>&& a_arg)noexcept->tp_type&&
{
  return static_cast<tp_type&&>(a_arg);
}

/*** likely unlikely ***/

namespace
dango
{
  constexpr auto likely(bool)noexcept->bool;
  constexpr auto unlikely(bool)noexcept->bool;
}

constexpr auto
dango::
likely
(bool const a_cond)noexcept->bool
{
  return bool(__builtin_expect(a_cond, true));
}

constexpr auto
dango::
unlikely
(bool const a_cond)noexcept->bool
{
  return bool(__builtin_expect(a_cond, false));
}

/*** assume ***/

namespace
dango
{
  constexpr void assume(bool)noexcept;
}

constexpr void
dango::
assume
(bool const a_cond)noexcept
{
  if(dango::unlikely(!a_cond))
  {
    __builtin_unreachable();
  }
}

/*** infinite_loop ***/

namespace
dango
{
  [[noreturn]] DANGO_EXPORT void infinite_loop()noexcept;
}

/*** trap_instruction ***/

namespace
dango
{
  [[noreturn]] void trap_instruction()noexcept;
}

inline void
dango::
trap_instruction()noexcept
{
  __builtin_trap();

  dango::infinite_loop();
}

/*** bchar_as_char char_as_bchar ***/

namespace
dango
{
  template
  <dango::is_same_ignore_cv<dango::bchar> tp_char>
  constexpr auto
  bchar_as_char
  (tp_char* const a_ptr)noexcept->auto
  {
    using ret_type = dango::copy_cv<tp_char, char>*;

    if(a_ptr)
    {
      return reinterpret_cast<ret_type>(a_ptr);
    }
    else
    {
      return static_cast<ret_type>(nullptr);
    }
  }

  template
  <dango::is_same_ignore_cv<char> tp_char>
  constexpr auto
  char_as_bchar
  (tp_char* const a_ptr)noexcept->auto
  {
    using ret_type = dango::copy_cv<tp_char, dango::bchar>*;

    if(a_ptr)
    {
      return reinterpret_cast<ret_type>(a_ptr);
    }
    else
    {
      return static_cast<ret_type>(nullptr);
    }
  }
}

/*** is_pow_two ***/

namespace
dango
{
  template
  <dango::is_int tp_int>
  constexpr auto
  is_pow_two
  (tp_int const a_arg)noexcept->bool
  {
    if(a_arg <= tp_int(0))
    {
      return false;
    }

    return tp_int(a_arg & (a_arg - tp_int(1))) == tp_int(0);
  }
}

/*** logic ***/

namespace
dango
{
  constexpr auto logic_implies(bool const a_lhs, bool const a_rhs)noexcept->bool{ return !a_lhs || a_rhs; }
  constexpr auto logic_equivalent(bool const a_lhs, bool const a_rhs)noexcept->bool{ return a_lhs == a_rhs; }
  constexpr auto logic_and(bool const a_lhs, bool const a_rhs)noexcept->bool{ return a_lhs && a_rhs; }
  constexpr auto logic_or(bool const a_lhs, bool const a_rhs)noexcept->bool{ return a_lhs || a_rhs; }
  constexpr auto logic_xor(bool const a_lhs, bool const a_rhs)noexcept->bool{ return a_lhs != a_rhs; }
  constexpr auto logic_not(bool const a_arg)noexcept->bool{ return !a_arg; }
}

/*** next_multiple ***/

namespace
dango
{
  template
  <dango::is_uint tp_uint>
  constexpr auto
  next_multiple
  (tp_uint const a_arg1, tp_uint const a_arg2)noexcept->tp_uint
  {
    tp_uint const a_div = a_arg1 / a_arg2;
    tp_uint const a_mod = a_arg1 % a_arg2;

    return (a_mod != tp_uint(0)) ? (a_arg2 * (a_div + tp_uint(1))) : a_arg1;
  }
}

/*** destructor ***/

namespace
dango
{
  template
  <dango::is_destructible tp_type>
  constexpr void
  destructor
  (tp_type const volatile* a_ptr)noexcept(dango::is_noexcept_destructible<tp_type>)
  {
    if(a_ptr)
    {
      a_ptr->~tp_type();
    }
  }

  constexpr void destructor(dango::null_tag const)noexcept{ }
}

/*** launder ***/

namespace
dango
{
  template
  <dango::is_object tp_type>
  [[nodiscard]] constexpr auto
  launder(tp_type* const a_ptr)noexcept->tp_type*
  {
    return __builtin_launder(a_ptr);
  }
}

/*** address_of ***/

namespace
dango
{
  template
  <typename tp_type>
  requires(dango::is_object<dango::remove_ref<tp_type>> && dango::is_lvalue_ref<tp_type>)
  constexpr auto
  address_of
  (tp_type&& a_arg)noexcept->dango::remove_ref<tp_type>*
  {
    return __builtin_addressof(a_arg);
  }

  template
  <typename tp_type>
  requires(dango::is_object<dango::remove_ref<tp_type>> && !dango::is_lvalue_ref<tp_type>)
  constexpr auto
  address_of
  (tp_type&&)noexcept = delete;
}

/*** ptr_as_uint ptr_as_sint ***/

namespace
dango
{
  constexpr auto ptr_as_uint(void const volatile*)noexcept->dango::uptr;
  constexpr auto ptr_as_sint(void const volatile*)noexcept->dango::sptr;
}

constexpr auto
dango::
ptr_as_uint
(void const volatile* const a_ptr)noexcept->dango::uptr
{
  using ret_type = dango::uptr;

  if(a_ptr)
  {
    return reinterpret_cast<ret_type>(a_ptr);
  }
  else
  {
    return ret_type(0);
  }
}

constexpr auto
dango::
ptr_as_sint
(void const volatile* const a_ptr)noexcept->dango::sptr
{
  using ret_type = dango::sptr;

  if(a_ptr)
  {
    return reinterpret_cast<ret_type>(a_ptr);
  }
  else
  {
    return ret_type(0);
  }
}

/*** volatile_load volatile_store ***/

namespace
dango
{
  template
  <dango::is_scalar tp_type>
  constexpr auto
  volatile_load
  (tp_type const volatile* const a_addr)noexcept->tp_type
  {
    return *a_addr;
  }

  template
  <dango::is_scalar tp_type, typename tp_arg>
  requires
  (
    !dango::is_const<tp_type> &&
    dango::is_brace_constructible<tp_type, tp_arg> &&
    dango::is_convertible<tp_arg, tp_type>
  )
  constexpr void
  volatile_store
  (tp_type volatile* const a_addr, tp_arg&& a_arg)
  noexcept(dango::is_noexcept_brace_constructible<tp_type, tp_arg>)
  {
    *a_addr = tp_type{ dango::forward<tp_arg>(a_arg) };
  }
}

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
  class compare_val;
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
  requires(dango::is_convertible<tp_arg, super_type> && dango::is_constructible<super_type, tp_arg const&>)
  explicit constexpr
  compare_val_base(tp_arg const a_arg)noexcept:
  super_type{ a_arg }
  { }
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
compare_val
final:
public dango::detail::compare_val_base<std::strong_ordering>
{
private:
  using super_type = dango::detail::compare_val_base<std::strong_ordering>;
public:
  using category_type = category_strong;
public:
  static compare_val const equal;
  static compare_val const equivalent;
  static compare_val const less;
  static compare_val const greater;
public:
  explicit constexpr compare_val()noexcept:super_type{ 0 }{ }

  template
  <dango::is_sint tp_int>
  constexpr compare_val(tp_int const a_int)noexcept:super_type{ a_int }{ }

  constexpr compare_val(category_strong const a_arg)noexcept:super_type{ a_arg }{ }

  template
  <dango::is_sint tp_int>
  explicit constexpr operator tp_int()const noexcept{ return tp_int(*this > 0) - tp_int(*this < 0); }

  constexpr operator dango::compare_val_weak()const noexcept;
  constexpr operator dango::compare_val_partial()const noexcept;
public:
  constexpr compare_val(compare_val const&)noexcept = default;
  constexpr compare_val(compare_val&&)noexcept = default;
  constexpr auto operator = (compare_val const&)& noexcept->compare_val& = default;
  constexpr auto operator = (compare_val&&)& noexcept->compare_val& = default;
public:
  constexpr auto as_int()const noexcept->int_type{ return int_type(*this); }
  constexpr auto mirror()const noexcept->compare_val{ return compare_val{ -(as_int()) }; }
};

inline constexpr dango::compare_val const dango::compare_val::equal = compare_val{ category_type::equal };
inline constexpr dango::compare_val const dango::compare_val::equivalent = compare_val{ category_type::equivalent };
inline constexpr dango::compare_val const dango::compare_val::less = compare_val{ category_type::less };
inline constexpr dango::compare_val const dango::compare_val::greater = compare_val{ category_type::greater };

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
  explicit constexpr operator tp_int()const noexcept{ return tp_int(*this > 0) - tp_int(*this < 0); }

  constexpr operator dango::compare_val_partial()const noexcept;
public:
  constexpr compare_val_weak(compare_val_weak const&)noexcept = default;
  constexpr compare_val_weak(compare_val_weak&&)noexcept = default;
  constexpr auto operator = (compare_val_weak const&)& noexcept->compare_val_weak& = default;
  constexpr auto operator = (compare_val_weak&&)& noexcept->compare_val_weak& = default;
public:
  constexpr auto as_int()const noexcept->int_type{ return int_type(*this); }
  constexpr auto mirror()const noexcept->compare_val_weak{ return compare_val_weak{ -(as_int()) }; }
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
  if(*this == unordered)
  {
    return unordered;
  }

  return compare_val_partial{ int_type(*this > 0) - int_type(*this < 0) };
}

constexpr
dango::
compare_val::
operator dango::compare_val_weak
()const noexcept
{
  return dango::compare_val_weak{ static_cast<category_type const&>(*this) };
}

constexpr
dango::
compare_val::
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
dango
{
  template
  <typename tp_type>
  concept is_compare_val =
    dango::is_same_ignore_cv<tp_type, dango::compare_val> ||
    dango::is_same_ignore_cv<tp_type, dango::compare_val_weak> ||
    dango::is_same_ignore_cv<tp_type, dango::compare_val_partial>;
}

namespace
dango::detail
{
  constexpr auto strongest_comparison_help(dango::priority_tag<dango::uint(2)> const, dango::compare_val const a_val)noexcept->auto{ return a_val; }
  constexpr auto strongest_comparison_help(dango::priority_tag<dango::uint(1)> const, dango::compare_val_weak const a_val)noexcept->auto{ return a_val; }
  constexpr auto strongest_comparison_help(dango::priority_tag<dango::uint(0)> const, dango::compare_val_partial const a_val)noexcept->auto{ return a_val; }
}

namespace
dango::comparison
{
  template
  <typename tp_type>
  concept is_convertible =
    dango::is_object_exclude_array<dango::remove_ref<tp_type>> &&
    requires{ { dango::detail::strongest_comparison_help(dango::priority_tag<dango::uint(2)>{ }, dango::declval<tp_type>()) }; };

  template
  <typename tp_type>
  concept is_noexcept_convertible =
    dango::comparison::is_convertible<tp_type> &&
    requires{ { dango::detail::strongest_comparison_help(dango::priority_tag<dango::uint(2)>{ }, dango::declval<tp_type>()) }noexcept; };

  template
  <dango::comparison::is_convertible tp_arg>
  constexpr auto
  strongest(tp_arg&& a_arg)
  noexcept(dango::comparison::is_noexcept_convertible<tp_arg>)->auto
  {
    return dango::detail::strongest_comparison_help(dango::priority_tag<dango::uint(2)>{ }, dango::forward<tp_arg>(a_arg));
  }

  template
  <dango::is_compare_val... tp_types>
  using common_type =
    decltype(dango::comparison::strongest(dango::declval<std::common_comparison_category_t<typename tp_types::category_type...>>()));

  constexpr auto is_eq(dango::compare_val_partial const a_val)noexcept->bool{ return a_val.is_eq(); }
  constexpr auto is_neq(dango::compare_val_partial const a_val)noexcept->bool{ return a_val.is_neq(); }
  constexpr auto is_lt(dango::compare_val_partial const a_val)noexcept->bool{ return a_val.is_lt(); }
  constexpr auto is_lteq(dango::compare_val_partial const a_val)noexcept->bool{ return a_val.is_lteq(); }
  constexpr auto is_gt(dango::compare_val_partial const a_val)noexcept->bool{ return a_val.is_gt(); }
  constexpr auto is_gteq(dango::compare_val_partial const a_val)noexcept->bool{ return a_val.is_gteq(); }

  constexpr auto mirror(dango::compare_val const a_val)noexcept->auto{ return a_val.mirror(); }
  constexpr auto mirror(dango::compare_val_weak const a_val)noexcept->auto{ return a_val.mirror(); }
  constexpr auto mirror(dango::compare_val_partial const a_val)noexcept->auto{ return a_val.mirror(); }
}

static_assert(dango::is_convertible<dango::compare_val, std::strong_ordering>);
static_assert(dango::is_convertible<dango::compare_val, std::weak_ordering>);
static_assert(dango::is_convertible<dango::compare_val, std::partial_ordering>);
static_assert(dango::is_convertible<dango::compare_val, dango::compare_val>);
static_assert(dango::is_convertible<dango::compare_val, dango::compare_val_weak>);
static_assert(dango::is_convertible<dango::compare_val, dango::compare_val_partial>);

static_assert(!dango::is_convertible<dango::compare_val_weak, std::strong_ordering>);
static_assert(dango::is_convertible<dango::compare_val_weak, std::weak_ordering>);
static_assert(dango::is_convertible<dango::compare_val_weak, std::partial_ordering>);
static_assert(!dango::is_convertible<dango::compare_val_weak, dango::compare_val>);
static_assert(dango::is_convertible<dango::compare_val_weak, dango::compare_val_weak>);
static_assert(dango::is_convertible<dango::compare_val_weak, dango::compare_val_partial>);

static_assert(!dango::is_convertible<dango::compare_val_partial, std::strong_ordering>);
static_assert(!dango::is_convertible<dango::compare_val_partial, std::weak_ordering>);
static_assert(dango::is_convertible<dango::compare_val_partial, std::partial_ordering>);
static_assert(!dango::is_convertible<dango::compare_val_partial, dango::compare_val>);
static_assert(!dango::is_convertible<dango::compare_val_partial, dango::compare_val_weak>);
static_assert(dango::is_convertible<dango::compare_val_partial, dango::compare_val_partial>);

static_assert(dango::is_convertible<std::strong_ordering, dango::compare_val>);
static_assert(dango::is_convertible<std::strong_ordering, dango::compare_val_weak>);
static_assert(dango::is_convertible<std::strong_ordering, dango::compare_val_partial>);

static_assert(!dango::is_convertible<std::weak_ordering, dango::compare_val>);
static_assert(dango::is_convertible<std::weak_ordering, dango::compare_val_weak>);
static_assert(dango::is_convertible<std::weak_ordering, dango::compare_val_partial>);

static_assert(!dango::is_convertible<std::partial_ordering, dango::compare_val>);
static_assert(!dango::is_convertible<std::partial_ordering, dango::compare_val_weak>);
static_assert(dango::is_convertible<std::partial_ordering, dango::compare_val_partial>);

/*** swap ***/

namespace
dango::custom
{
  template
  <typename tp_lhs, typename tp_rhs>
  struct operator_swap;
}

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept has_operator_swap =
    dango::is_object<dango::remove_ref<tp_lhs>> && dango::is_object<dango::remove_ref<tp_rhs>> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::custom::operator_swap<dango::remove_cvref<tp_lhs>, dango::remove_cvref<tp_rhs>>::swap(dango::forward<tp_lhs>(a_lhs), dango::forward<tp_rhs>(a_rhs)) }->dango::is_convertible_ret<void>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_operator_swap =
    dango::has_operator_swap<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::custom::operator_swap<dango::remove_cvref<tp_lhs>, dango::remove_cvref<tp_rhs>>::swap(dango::forward<tp_lhs>(a_lhs), dango::forward<tp_rhs>(a_rhs)) }noexcept->dango::is_noexcept_convertible_ret<void>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_dango_operator_swap =
    dango::is_class_or_union<dango::remove_ref<tp_lhs>> && dango::is_object<dango::remove_ref<tp_rhs>> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::forward<tp_lhs>(a_lhs).dango_operator_swap(dango::forward<tp_rhs>(a_rhs)) }->dango::is_convertible_ret<void>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_dango_operator_swap =
    dango::has_dango_operator_swap<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::forward<tp_lhs>(a_lhs).dango_operator_swap(dango::forward<tp_rhs>(a_rhs)) }noexcept->dango::is_noexcept_convertible_ret<void>; };
}

namespace
dango::detail
{
  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_swap<tp_lhs&, tp_rhs&>)
  constexpr void
  swap_help
  (dango::priority_tag<dango::uint(4)> const, tp_lhs& a_lhs, tp_rhs& a_rhs)
  noexcept(dango::has_noexcept_operator_swap<tp_lhs&, tp_rhs&>)
  {
    dango::custom::operator_swap<dango::remove_cv<tp_lhs>, dango::remove_cv<tp_rhs>>::swap(a_lhs, a_rhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_swap<tp_rhs&, tp_lhs&>)
  constexpr void
  swap_help
  (dango::priority_tag<dango::uint(3)> const, tp_lhs& a_lhs, tp_rhs& a_rhs)
  noexcept(dango::has_noexcept_operator_swap<tp_rhs&, tp_lhs&>)
  {
    dango::custom::operator_swap<dango::remove_cv<tp_rhs>, dango::remove_cv<tp_lhs>>::swap(a_rhs, a_lhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_dango_operator_swap<tp_lhs&, tp_rhs&>)
  constexpr void
  swap_help
  (dango::priority_tag<dango::uint(2)> const, tp_lhs& a_lhs, tp_rhs& a_rhs)
  noexcept(dango::has_noexcept_dango_operator_swap<tp_lhs&, tp_rhs&>)
  {
    a_lhs.dango_operator_swap(a_rhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_dango_operator_swap<tp_rhs&, tp_lhs&>)
  constexpr void
  swap_help
  (dango::priority_tag<dango::uint(1)> const, tp_lhs& a_lhs, tp_rhs& a_rhs)
  noexcept(dango::has_noexcept_dango_operator_swap<tp_rhs&, tp_lhs&>)
  {
    a_rhs.dango_operator_swap(a_lhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires
  (
    !dango::is_const<tp_lhs> && !dango::is_const<tp_rhs> &&
    dango::is_brace_constructible<dango::remove_volatile<tp_lhs>, tp_lhs&&> &&
    dango::is_assignable<tp_lhs&, tp_rhs&&> &&
    dango::is_assignable<tp_rhs&, dango::remove_volatile<tp_lhs>&&>
  )
  constexpr void
  swap_help
  (dango::priority_tag<dango::uint(0)> const, tp_lhs& a_lhs, tp_rhs& a_rhs)
  noexcept
  (
    dango::is_noexcept_brace_constructible<dango::remove_volatile<tp_lhs>, tp_lhs&&> &&
    dango::is_noexcept_assignable<tp_lhs&, tp_rhs&&> &&
    dango::is_noexcept_assignable<tp_rhs&, dango::remove_volatile<tp_lhs>&&>
  )
  {
    dango::remove_volatile<tp_lhs> a_temp{ dango::move(a_lhs) };

    a_lhs = dango::move(a_rhs);

    a_rhs = dango::move(a_temp);
  }
}

/*** swap is_swappable is_noexcept_swappable ***/

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept is_swappable =
    dango::is_object<dango::remove_ref<tp_lhs>> && dango::is_object<dango::remove_ref<tp_rhs>> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::detail::swap_help(dango::priority_tag<dango::uint(4)>{ }, dango::forward<tp_lhs>(a_lhs), dango::forward<tp_rhs>(a_rhs)) }->dango::is_convertible_ret<void>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept is_noexcept_swappable =
    dango::is_swappable<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::detail::swap_help(dango::priority_tag<dango::uint(4)>{ }, dango::forward<tp_lhs>(a_lhs), dango::forward<tp_rhs>(a_rhs)) }noexcept->dango::is_noexcept_convertible_ret<void>; };

  inline constexpr auto const swap =
    []<typename tp_lhs, typename tp_rhs>
    (tp_lhs& a_lhs, tp_rhs& a_rhs)constexpr
    noexcept(dango::is_noexcept_swappable<tp_lhs&, tp_rhs&>)->void
    requires(dango::is_swappable<tp_lhs&, tp_rhs&>)
    {
      dango::detail::swap_help(dango::priority_tag<dango::uint(4)>{ }, a_lhs, a_rhs);
    };
}

/*** swap for arrays of same dim ***/

namespace
dango::custom
{
  template
  <typename tp_lhs, typename tp_rhs, dango::usize tp_size>
  requires(dango::is_swappable<tp_lhs&, tp_rhs&>)
  struct
  operator_swap
  <tp_lhs[tp_size], tp_rhs[tp_size]>
  final
  {
    static constexpr void
    swap(tp_lhs(& a_lhs)[tp_size], tp_rhs(& a_rhs)[tp_size])noexcept(dango::is_noexcept_swappable<tp_lhs&, tp_rhs&>)
    {
      for(auto a_i = dango::usize(0); a_i < tp_size; ++a_i)
      {
        dango::swap(a_lhs[a_i], a_rhs[a_i]);
      }
    }

    DANGO_UNINSTANTIABLE(operator_swap)
  };
}

/*** equals ***/

namespace
dango::custom
{
  template
  <typename type>
  struct operator_is_null;

  template
  <typename tp_lhs, typename tp_rhs>
  struct operator_equals;

  template
  <typename tp_lhs, typename tp_rhs>
  struct operator_compare;
}

namespace
dango
{
  template
  <typename tp_type>
  concept has_operator_is_null =
    dango::is_referenceable<dango::remove_ref<tp_type>> &&
    requires(tp_type a_arg)
    { { dango::custom::operator_is_null<dango::remove_cvref<tp_type>>::is_null(dango::forward<tp_type>(a_arg)) }->dango::is_convertible_ret<bool>; };

  template
  <typename tp_type>
  concept has_noexcept_operator_is_null =
    dango::has_operator_is_null<tp_type> &&
    requires(tp_type a_arg)
    { { dango::custom::operator_is_null<dango::remove_cvref<tp_type>>::is_null(dango::forward<tp_type>(a_arg)) }noexcept->dango::is_noexcept_convertible_ret<bool>; };

  template
  <typename tp_type>
  concept has_dango_operator_is_null =
    dango::is_class_or_union<dango::remove_ref<tp_type>> &&
    requires(tp_type a_arg){ { dango::forward<tp_type>(a_arg).dango_operator_is_null() }->dango::is_convertible_ret<bool>; };

  template
  <typename tp_type>
  concept has_noexcept_dango_operator_is_null =
    dango::has_dango_operator_is_null<tp_type> &&
    requires(tp_type a_arg){ { dango::forward<tp_type>(a_arg).dango_operator_is_null() }noexcept->dango::is_noexcept_convertible_ret<bool>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_operator_equals =
    dango::is_referenceable<dango::remove_ref<tp_lhs>> && dango::is_referenceable<dango::remove_ref<tp_rhs>> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    {
      {
        dango::custom::operator_equals<dango::remove_cvref<tp_lhs>, dango::remove_cvref<tp_rhs>>::equals(dango::forward<tp_lhs>(a_lhs), dango::forward<tp_rhs>(a_rhs))
      }->dango::is_convertible_ret<bool>;
    };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_operator_equals =
    dango::has_operator_equals<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    {
      {
        dango::custom::operator_equals<dango::remove_cvref<tp_lhs>, dango::remove_cvref<tp_rhs>>::equals(dango::forward<tp_lhs>(a_lhs), dango::forward<tp_rhs>(a_rhs))
      }noexcept->dango::is_noexcept_convertible_ret<bool>;
    };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_dango_operator_equals =
    dango::is_class_or_union<dango::remove_ref<tp_lhs>> && dango::is_referenceable<dango::remove_ref<tp_rhs>> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::forward<tp_lhs>(a_lhs).dango_operator_equals(dango::forward<tp_rhs>(a_rhs)) }->dango::is_convertible_ret<bool>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_dango_operator_equals =
    dango::has_dango_operator_equals<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::forward<tp_lhs>(a_lhs).dango_operator_equals(dango::forward<tp_rhs>(a_rhs)) }noexcept->dango::is_noexcept_convertible_ret<bool>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_operator_compare =
    dango::is_referenceable<dango::remove_ref<tp_lhs>> && dango::is_referenceable<dango::remove_ref<tp_rhs>> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    {
      {
        dango::custom::operator_compare<dango::remove_cvref<tp_lhs>, dango::remove_cvref<tp_rhs>>::compare(dango::forward<tp_lhs>(a_lhs), dango::forward<tp_rhs>(a_rhs))
      }->dango::comparison::is_convertible;
    };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_operator_compare =
    dango::has_operator_compare<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    {
      {
        dango::custom::operator_compare<dango::remove_cvref<tp_lhs>, dango::remove_cvref<tp_rhs>>::compare(dango::forward<tp_lhs>(a_lhs), dango::forward<tp_rhs>(a_rhs))
      }noexcept->dango::comparison::is_noexcept_convertible;
    };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_dango_operator_compare =
    dango::is_class_or_union<dango::remove_ref<tp_lhs>> && dango::is_referenceable<dango::remove_ref<tp_rhs>> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::forward<tp_lhs>(a_lhs).dango_operator_compare(dango::forward<tp_rhs>(a_rhs)) }->dango::comparison::is_convertible; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept has_noexcept_dango_operator_compare =
    dango::has_dango_operator_compare<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::forward<tp_lhs>(a_lhs).dango_operator_compare(dango::forward<tp_rhs>(a_rhs)) }noexcept->dango::comparison::is_noexcept_convertible; };
}

namespace
dango::detail
{
  template
  <typename tp_lhs>
  requires(dango::has_operator_is_null<tp_lhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(11)> const, tp_lhs const& a_lhs, dango::null_tag const)
  noexcept(dango::has_noexcept_operator_is_null<tp_lhs const&>)->bool
  {
    return dango::custom::operator_is_null<dango::remove_cv<tp_lhs>>::is_null(a_lhs);
  }

  template
  <typename tp_rhs>
  requires(dango::has_operator_is_null<tp_rhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(10)> const, dango::null_tag const, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_is_null<tp_rhs const&>)->bool
  {
    return dango::custom::operator_is_null<dango::remove_cv<tp_rhs>>::is_null(a_rhs);
  }

  template
  <typename tp_lhs>
  requires(dango::has_dango_operator_is_null<tp_lhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(9)> const, tp_lhs const& a_lhs, dango::null_tag const)
  noexcept(dango::has_noexcept_dango_operator_is_null<tp_lhs const&>)->bool
  {
    return a_lhs.dango_operator_is_null();
  }

  template
  <typename tp_rhs>
  requires(dango::has_dango_operator_is_null<tp_rhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(8)> const, dango::null_tag const, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_dango_operator_is_null<tp_rhs const&>)->bool
  {
    return a_rhs.dango_operator_is_null();
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_equals<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(7)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_equals<tp_lhs const&, tp_rhs const&>)->bool
  {
    return dango::custom::operator_equals<dango::remove_cv<tp_lhs>, dango::remove_cv<tp_rhs>>::equals(a_lhs, a_rhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_equals<tp_rhs const&, tp_lhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(6)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_equals<tp_rhs const&, tp_lhs const&>)->bool
  {
    return dango::custom::operator_equals<dango::remove_cv<tp_rhs>, dango::remove_cv<tp_lhs>>::equals(a_rhs, a_lhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_dango_operator_equals<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(5)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_dango_operator_equals<tp_lhs const&, tp_rhs const&>)->bool
  {
    return a_lhs.dango_operator_equals(a_rhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_dango_operator_equals<tp_rhs const&, tp_lhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(4)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_dango_operator_equals<tp_rhs const&, tp_lhs const&>)->bool
  {
    return a_rhs.dango_operator_equals(a_lhs);
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_compare<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(3)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_compare<tp_lhs const&, tp_rhs const&>)->bool
  {
    return dango::comparison::strongest(dango::custom::operator_compare<dango::remove_cv<tp_lhs>, dango::remove_cv<tp_rhs>>::compare(a_lhs, a_rhs)).is_eq();
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_compare<tp_rhs const&, tp_lhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(2)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_compare<tp_rhs const&, tp_lhs const&>)->bool
  {
    return dango::comparison::strongest(dango::custom::operator_compare<dango::remove_cv<tp_lhs>, dango::remove_cv<tp_rhs>>::compare(a_lhs, a_rhs)).mirror().is_eq();
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_dango_operator_compare<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(1)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_dango_operator_compare<tp_lhs const&, tp_rhs const&>)->bool
  {
    return dango::comparison::strongest(a_lhs.dango_operator_compare(a_rhs)).is_eq();
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_dango_operator_compare<tp_rhs const&, tp_lhs const&>)
  constexpr auto
  equals_help
  (dango::priority_tag<dango::uint(0)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_dango_operator_compare<tp_rhs const&, tp_lhs const&>)->bool
  {
    return dango::comparison::strongest(a_rhs.dango_operator_compare(a_lhs)).mirror().is_eq();
  }

  template
  <typename tp_lhs, typename tp_rhs, dango::uint tp_prio = dango::uint(11)>
  concept has_equals_help =
    dango::is_referenceable<dango::remove_ref<tp_lhs>> && dango::is_referenceable<dango::remove_ref<tp_rhs>> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::detail::equals_help(dango::priority_tag<tp_prio>{ }, dango::forward<tp_lhs>(a_lhs), dango::forward<tp_rhs>(a_rhs)) }->dango::is_convertible_ret<bool>; };

  template
  <typename tp_lhs, typename tp_rhs, dango::uint tp_prio = dango::uint(11)>
  concept has_noexcept_equals_help =
    dango::detail::has_equals_help<tp_lhs, tp_rhs, tp_prio> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    { { dango::detail::equals_help(dango::priority_tag<tp_prio>{ }, dango::forward<tp_lhs>(a_lhs), dango::forward<tp_rhs>(a_rhs)) }noexcept->dango::is_noexcept_convertible_ret<bool>; };
}

template
<typename tp_lhs, typename tp_rhs>
requires(dango::detail::has_equals_help<tp_lhs const&, tp_rhs const&>)
constexpr auto
operator ==
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)
noexcept(dango::detail::has_noexcept_equals_help<tp_lhs const&, tp_rhs const&>)->bool
{
  return dango::detail::equals_help(dango::priority_tag<dango::uint(11)>{ }, a_lhs, a_rhs);
}

template
<typename tp_lhs, typename tp_rhs>
requires(dango::detail::has_equals_help<tp_lhs const&, tp_rhs const&>)
constexpr auto
operator !=
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)
noexcept(dango::detail::has_noexcept_equals_help<tp_lhs const&, tp_rhs const&>)->bool
{
  return !dango::detail::equals_help(dango::priority_tag<dango::uint(11)>{ }, a_lhs, a_rhs);
}

/*** is_equatable ***/

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept is_equatable =
    dango::is_referenceable<dango::remove_ref<tp_lhs>> && dango::is_referenceable<dango::remove_ref<tp_rhs>> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    {
      { dango::forward<tp_lhs>(a_lhs) == dango::forward<tp_rhs>(a_rhs) }->dango::is_convertible_ret<bool>;
      { dango::forward<tp_lhs>(a_lhs) != dango::forward<tp_rhs>(a_rhs) }->dango::is_convertible_ret<bool>;
    };

  template
  <typename tp_lhs, typename tp_rhs>
  concept is_noexcept_equatable =
    dango::is_equatable<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    {
      { dango::forward<tp_lhs>(a_lhs) == dango::forward<tp_rhs>(a_rhs) }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { dango::forward<tp_lhs>(a_lhs) != dango::forward<tp_rhs>(a_rhs) }noexcept->dango::is_noexcept_convertible_ret<bool>;
    };


  inline constexpr auto const equals =
    []<typename tp_lhs, typename tp_rhs>
    (tp_lhs const& a_lhs, tp_rhs const& a_rhs)constexpr
    noexcept(dango::is_noexcept_equatable<tp_lhs const&, tp_rhs const&>)->bool
    requires(dango::is_equatable<tp_lhs const&, tp_rhs const&>)
    {
      return a_lhs == a_rhs;
    };
}

/*** is_nullable ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_null_equatable =
    dango::is_equatable<tp_type, dango::null_tag const&> && dango::is_equatable<tp_type, dango::null_tag&&> &&
    dango::is_equatable<dango::null_tag const&, tp_type> && dango::is_equatable<dango::null_tag&&, tp_type>;

  template
  <typename tp_type>
  concept is_noexcept_null_equatable =
    dango::is_null_equatable<tp_type> &&
    dango::is_noexcept_equatable<tp_type, dango::null_tag const&> && dango::is_noexcept_equatable<tp_type, dango::null_tag&&> &&
    dango::is_noexcept_equatable<dango::null_tag const&, tp_type> && dango::is_noexcept_equatable<dango::null_tag&&, tp_type>;

  template
  <typename tp_type>
  concept is_nullable =
    dango::is_object_exclude_array<tp_type> &&
    dango::is_null_equatable<dango::remove_cv<tp_type> const&> && dango::is_null_equatable<dango::remove_cv<tp_type>&&> &&
    dango::is_constructible<dango::remove_cv<tp_type>, dango::null_tag const&> && dango::is_constructible<dango::remove_cv<tp_type>, dango::null_tag&&> &&
    dango::is_convertible<dango::null_tag const&, dango::remove_cv<tp_type>> && dango::is_convertible<dango::null_tag&&, dango::remove_cv<tp_type>> &&
    dango::is_assignable<dango::remove_cv<tp_type>&, dango::null_tag const&> && dango::is_assignable<dango::remove_cv<tp_type>&, dango::null_tag&&>;
}

/*** compare ***/

namespace
dango::detail
{
  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_compare<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  compare_help
  (dango::priority_tag<dango::uint(3)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_compare<tp_lhs const&, tp_rhs const&>)->auto
  {
    return dango::comparison::strongest(dango::custom::operator_compare<dango::remove_cv<tp_lhs>, dango::remove_cv<tp_rhs>>::compare(a_lhs, a_rhs));
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_operator_compare<tp_rhs const&, tp_lhs const&>)
  constexpr auto
  compare_help
  (dango::priority_tag<dango::uint(2)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_operator_compare<tp_rhs const&, tp_lhs const&>)->auto
  {
    return dango::comparison::strongest(dango::custom::operator_compare<dango::remove_cv<tp_rhs>, dango::remove_cv<tp_lhs>>::compare(a_rhs, a_lhs)).mirror();
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_dango_operator_compare<tp_lhs const&, tp_rhs const&>)
  constexpr auto
  compare_help
  (dango::priority_tag<dango::uint(1)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_dango_operator_compare<tp_lhs const&, tp_rhs const&>)->auto
  {
    return dango::comparison::strongest(a_lhs.dango_operator_compare(a_rhs));
  }

  template
  <typename tp_lhs, typename tp_rhs>
  requires(dango::has_dango_operator_compare<tp_rhs const&, tp_lhs const&>)
  constexpr auto
  compare_help
  (dango::priority_tag<dango::uint(0)> const, tp_lhs const& a_lhs, tp_rhs const& a_rhs)
  noexcept(dango::has_noexcept_dango_operator_compare<tp_rhs const&, tp_lhs const&>)->auto
  {
    return dango::comparison::strongest(a_rhs.dango_operator_compare(a_lhs)).mirror();
  }

  template
  <typename tp_lhs, typename tp_rhs, dango::uint tp_prio = dango::uint(3)>
  concept has_compare_help =
    dango::is_referenceable<dango::remove_ref<tp_lhs>> && dango::is_referenceable<dango::remove_ref<tp_rhs>> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    {
      {
        dango::detail::compare_help(dango::priority_tag<tp_prio>{ }, dango::forward<tp_lhs>(a_lhs), dango::forward<tp_rhs>(a_rhs))
        }->dango::comparison::is_convertible;
    };

  template
  <typename tp_lhs, typename tp_rhs, dango::uint tp_prio = dango::uint(3)>
  concept has_noexcept_compare_help =
    dango::detail::has_compare_help<tp_lhs, tp_rhs, tp_prio> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    {
      {
        dango::detail::compare_help(dango::priority_tag<tp_prio>{ }, dango::forward<tp_lhs>(a_lhs), dango::forward<tp_rhs>(a_rhs))
      }noexcept->dango::comparison::is_noexcept_convertible;
    };
}

template
<typename tp_lhs, typename tp_rhs>
requires(dango::detail::has_compare_help<tp_lhs const&, tp_rhs const&>)
constexpr auto
operator <
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::detail::has_noexcept_compare_help<tp_lhs const&, tp_rhs const&>)->bool
{
  return dango::detail::compare_help(dango::priority_tag<dango::uint(3)>{ }, a_lhs, a_rhs).is_lt();
}

template
<typename tp_lhs, typename tp_rhs>
requires(dango::detail::has_compare_help<tp_lhs const&, tp_rhs const&>)
constexpr auto
operator <=
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::detail::has_noexcept_compare_help<tp_lhs const&, tp_rhs const&>)->bool
{
  return dango::detail::compare_help(dango::priority_tag<dango::uint(3)>{ }, a_lhs, a_rhs).is_lteq();
}

template
<typename tp_lhs, typename tp_rhs>
requires(dango::detail::has_compare_help<tp_lhs const&, tp_rhs const&>)
constexpr auto
operator >
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::detail::has_noexcept_compare_help<tp_lhs const&, tp_rhs const&>)->bool
{
  return dango::detail::compare_help(dango::priority_tag<dango::uint(3)>{ }, a_lhs, a_rhs).is_gt();
}

template
<typename tp_lhs, typename tp_rhs>
requires(dango::detail::has_compare_help<tp_lhs const&, tp_rhs const&>)
constexpr auto
operator >=
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::detail::has_noexcept_compare_help<tp_lhs const&, tp_rhs const&>)->bool
{
  return dango::detail::compare_help(dango::priority_tag<dango::uint(3)>{ }, a_lhs, a_rhs).is_gteq();
}

template
<typename tp_lhs, typename tp_rhs>
requires(dango::detail::has_compare_help<tp_lhs const&, tp_rhs const&>)
constexpr auto
operator <=>
(tp_lhs const& a_lhs, tp_rhs const& a_rhs)noexcept(dango::detail::has_noexcept_compare_help<tp_lhs const&, tp_rhs const&>)->auto
{
  return dango::detail::compare_help(dango::priority_tag<dango::uint(3)>{ }, a_lhs, a_rhs);
}

/*** is_comparable is_comparable_spaceship ***/

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept is_comparable =
    dango::is_equatable<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    {
      { dango::forward<tp_lhs>(a_lhs) <  dango::forward<tp_rhs>(a_rhs) }->dango::is_convertible_ret<bool>;
      { dango::forward<tp_lhs>(a_lhs) <= dango::forward<tp_rhs>(a_rhs) }->dango::is_convertible_ret<bool>;
      { dango::forward<tp_lhs>(a_lhs) >  dango::forward<tp_rhs>(a_rhs) }->dango::is_convertible_ret<bool>;
      { dango::forward<tp_lhs>(a_lhs) >= dango::forward<tp_rhs>(a_rhs) }->dango::is_convertible_ret<bool>;
    };

  template
  <typename tp_lhs, typename tp_rhs>
  concept is_noexcept_comparable =
    dango::is_comparable<tp_lhs, tp_rhs> &&
    dango::is_noexcept_equatable<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs)
    {
      { dango::forward<tp_lhs>(a_lhs) <  dango::forward<tp_rhs>(a_rhs) }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { dango::forward<tp_lhs>(a_lhs) <= dango::forward<tp_rhs>(a_rhs) }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { dango::forward<tp_lhs>(a_lhs) >  dango::forward<tp_rhs>(a_rhs) }noexcept->dango::is_noexcept_convertible_ret<bool>;
      { dango::forward<tp_lhs>(a_lhs) >= dango::forward<tp_rhs>(a_rhs) }noexcept->dango::is_noexcept_convertible_ret<bool>;
    };
}

namespace
dango::detail
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept is_comparable_spaceship_help1 =
    dango::is_comparable<tp_lhs, tp_rhs> &&
    requires(tp_lhs a_lhs, tp_rhs a_rhs){ { dango::forward<tp_lhs>(a_lhs) <=> dango::forward<tp_rhs>(a_rhs) }->dango::comparison::is_convertible; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept is_comparable_spaceship_help2 =
    !dango::is_noexcept_comparable<tp_lhs, tp_rhs> ||
    requires(tp_lhs a_lhs, tp_rhs a_rhs){ { dango::forward<tp_lhs>(a_lhs) <=> dango::forward<tp_rhs>(a_rhs) }noexcept->dango::comparison::is_noexcept_convertible; };
}

namespace
dango
{
  template
  <typename tp_lhs, typename tp_rhs>
  concept is_comparable_spaceship =
    dango::detail::is_comparable_spaceship_help1<tp_lhs, tp_rhs> &&
    dango::detail::is_comparable_spaceship_help2<tp_lhs, tp_rhs>;

  namespace
  detail
  {
    template
    <typename tp_lhs, typename tp_rhs>
    concept is_ptr_comparable_help =
      dango::is_ptr<dango::decay<tp_lhs>> && dango::is_ptr<dango::decay<tp_rhs>> &&
      dango::is_convertible<tp_lhs, void const volatile*> && dango::is_convertible<tp_rhs, void const volatile*> &&
      dango::is_comparable_spaceship<dango::decay<tp_lhs> const&, dango::decay<tp_rhs> const&>;
  }

  inline constexpr auto compare =
    []<typename tp_lhs, typename tp_rhs>
    (tp_lhs const& a_lhs, tp_rhs const& a_rhs)
    noexcept(dango::is_noexcept_comparable<tp_lhs const&, tp_rhs const&>)->auto
    requires(dango::is_comparable<tp_lhs const&, tp_rhs const&>)
    {
      if constexpr(dango::detail::is_ptr_comparable_help<tp_lhs const&, tp_rhs const&>)
      {
        auto const a_lhs_p = static_cast<void const volatile*>(a_lhs);
        auto const a_rhs_p = static_cast<void const volatile*>(a_rhs);

        if constexpr(dango::in_constexpr_context())
        {
          return dango::comparison::strongest(a_lhs_p <=> a_rhs_p);
        }
        else
        {
          return dango::comparison::strongest(dango::ptr_as_uint(a_lhs_p) <=> dango::ptr_as_uint(a_rhs_p));
        }
      }
      else
      {
        if constexpr(dango::is_comparable_spaceship<tp_lhs const&, tp_rhs const&>)
        {
          return dango::comparison::strongest(a_lhs <=> a_rhs);
        }
        else
        {
          return dango::compare_val{ dango::sint(a_lhs > a_rhs) - dango::sint(a_lhs < a_rhs) };
        }
      }
    };
}

/*** min max ***/

namespace
dango
{
  constexpr void min()noexcept{ }

  template
  <dango::is_scalar tp_arg>
  constexpr auto
  min
  (tp_arg const a_arg)noexcept->tp_arg
  {
    return a_arg;
  }

  template
  <dango::is_scalar tp_arg>
  requires(dango::is_comparable<tp_arg const&, tp_arg const&>)
  constexpr auto
  min
  (tp_arg const a_arg1, tp_arg const a_arg2)noexcept(dango::is_noexcept_comparable<tp_arg const&, tp_arg const&>)->tp_arg
  {
    return a_arg1 < a_arg2 ? a_arg1 : a_arg2;
  }

  template
  <dango::is_scalar tp_arg, dango::is_same<tp_arg>... tp_args>
  requires(sizeof...(tp_args) >= dango::usize(2) && dango::is_comparable<tp_arg const&, tp_arg const&>)
  constexpr auto
  min
  (tp_arg const a_arg, tp_args... a_args)noexcept(dango::is_noexcept_comparable<tp_arg const&, tp_arg const&>)->tp_arg
  {
    return dango::min(a_arg, dango::min(a_args...));
  }
}

namespace
dango
{
  constexpr void max()noexcept{ }

  template
  <dango::is_scalar tp_arg>
  constexpr auto
  max
  (tp_arg const a_arg)noexcept->tp_arg
  {
    return a_arg;
  }

  template
  <dango::is_scalar tp_arg>
  requires(dango::is_comparable<tp_arg const&, tp_arg const&>)
  constexpr auto
  max
  (tp_arg const a_arg1, tp_arg const a_arg2)noexcept(dango::is_noexcept_comparable<tp_arg const&, tp_arg const&>)->tp_arg
  {
    return a_arg1 > a_arg2 ? a_arg1 : a_arg2;
  }

  template
  <dango::is_scalar tp_arg, dango::is_same<tp_arg>... tp_args>
  requires(sizeof...(tp_args) >= dango::usize(2) && dango::is_comparable<tp_arg const&, tp_arg const&>)
  constexpr auto
  max
  (tp_arg const a_arg, tp_args... a_args)noexcept(dango::is_noexcept_comparable<tp_arg const&, tp_arg const&>)->tp_arg
  {
    return dango::max(a_arg, dango::max(a_args...));
  }
}

namespace
dango::detail
{
  template
  <typename... tp_args>
  struct min_max_help;
}

namespace
dango
{
  template
  <typename tp_arg>
  requires
  (dango::is_class_or_union<dango::remove_ref<tp_arg>> && dango::is_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg>)
  constexpr auto
  min
  (tp_arg&& a_arg)
  noexcept(dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg>)->dango::remove_cvref<tp_arg>
  {
    return dango::remove_cvref<tp_arg>{ dango::forward<tp_arg>(a_arg) };
  }

  template
  <typename tp_arg1, typename tp_arg2>
  requires
  (
    dango::is_class_or_union<dango::remove_ref<tp_arg1>> && dango::is_same_ignore_cvref<tp_arg2, tp_arg1> &&
    dango::is_comparable<dango::remove_ref<tp_arg1> const&, dango::remove_ref<tp_arg1> const&> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg1>, tp_arg1> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg1>, tp_arg2>
  )
  constexpr auto
  min
  (tp_arg1&& a_arg1, tp_arg2&& a_arg2)
  noexcept
  (
    dango::is_noexcept_comparable<dango::remove_ref<tp_arg1> const&, dango::remove_ref<tp_arg1> const&> &&
    dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg1>, tp_arg1> &&
    dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg1>, tp_arg2>
  )->dango::remove_cvref<tp_arg1>
  {
    using ret_type = dango::remove_cvref<tp_arg1>;

    if(dango::compare(a_arg1, a_arg2).is_lt())
    {
      return ret_type{ dango::forward<tp_arg1>(a_arg1) };
    }

    return ret_type{ dango::forward<tp_arg2>(a_arg2) };
  }

  template
  <typename tp_arg, typename... tp_args>
  requires
  (
    (sizeof...(tp_args) >= dango::usize(2)) &&
    (dango::is_class_or_union<dango::remove_ref<tp_arg>> && ... && dango::is_same_ignore_cvref<tp_args, tp_arg>) &&
    requires
    {
      { dango::detail::min_max_help<tp_args&&...>::min(dango::declval<tp_args>()...) }->dango::is_same<dango::remove_cvref<tp_arg>>;
      { dango::min(dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }->dango::is_same<dango::remove_cvref<tp_arg>>;
    }
  )
  constexpr auto
  min
  (tp_arg&& a_arg, tp_args&&... a_args)
  noexcept
  (
    requires
    {
      { dango::detail::min_max_help<tp_args&&...>::min(dango::declval<tp_args>()...) }noexcept;
      { dango::min(dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }noexcept;
    }
  )->dango::remove_cvref<tp_arg>
  {
    return dango::min(dango::forward<tp_arg>(a_arg), dango::detail::min_max_help<tp_args&&...>::min(dango::forward<tp_args>(a_args)...));
  }
}

namespace
dango
{
  template
  <typename tp_arg>
  requires
  (dango::is_class_or_union<dango::remove_ref<tp_arg>> && dango::is_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg>)
  constexpr auto
  max
  (tp_arg&& a_arg)
  noexcept(dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg>)->dango::remove_cvref<tp_arg>
  {
    return dango::remove_cvref<tp_arg>{ dango::forward<tp_arg>(a_arg) };
  }

  template
  <typename tp_arg1, typename tp_arg2>
  requires
  (
    dango::is_class_or_union<dango::remove_ref<tp_arg1>> && dango::is_same_ignore_cvref<tp_arg2, tp_arg1> &&
    dango::is_comparable<dango::remove_ref<tp_arg1> const&, dango::remove_ref<tp_arg1> const&> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg1>, tp_arg1> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg1>, tp_arg2>
  )
  constexpr auto
  max
  (tp_arg1&& a_arg1, tp_arg2&& a_arg2)
  noexcept
  (
    dango::is_noexcept_comparable<dango::remove_ref<tp_arg1> const&, dango::remove_ref<tp_arg1> const&> &&
    dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg1>, tp_arg1> &&
    dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg1>, tp_arg2>
  )->dango::remove_cvref<tp_arg1>
  {
    using ret_type = dango::remove_cvref<tp_arg1>;

    if(dango::compare(a_arg1, a_arg2).is_gt())
    {
      return ret_type{ dango::forward<tp_arg1>(a_arg1) };
    }

    return ret_type{ dango::forward<tp_arg2>(a_arg2) };
  }

  template
  <typename tp_arg, typename... tp_args>
  requires
  (
    (sizeof...(tp_args) >= dango::usize(2)) &&
    (dango::is_class_or_union<dango::remove_ref<tp_arg>> && ... && dango::is_same_ignore_cvref<tp_args, tp_arg>) &&
    requires
    {
      { dango::detail::min_max_help<tp_args&&...>::max(dango::declval<tp_args>()...) }->dango::is_same<dango::remove_cvref<tp_arg>>;
      { dango::max(dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }->dango::is_same<dango::remove_cvref<tp_arg>>;
    }
  )
  constexpr auto
  max
  (tp_arg&& a_arg, tp_args&&... a_args)
  noexcept
  (
    requires
    {
      { dango::detail::min_max_help<tp_args&&...>::max(dango::declval<tp_args>()...) }noexcept;
      { dango::max(dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }noexcept;
    }
  )->dango::remove_cvref<tp_arg>
  {
    return dango::max(dango::forward<tp_arg>(a_arg), dango::detail::min_max_help<tp_args&&...>::max(dango::forward<tp_args>(a_args)...));
  }
}

template
<typename... tp_args>
struct
dango::
detail::
min_max_help
final
{
  static constexpr auto
  min(tp_args... a_args)noexcept(requires{ { dango::min(dango::declval<tp_args>()...) }noexcept; })->decltype(auto)
  requires(requires{ { dango::min(dango::declval<tp_args>()...) }; })
  {
    return dango::min(dango::forward<tp_args>(a_args)...);
  }

  static constexpr auto
  max(tp_args... a_args)noexcept(requires{ { dango::max(dango::declval<tp_args>()...) }noexcept; })->decltype(auto)
  requires(requires{ { dango::max(dango::declval<tp_args>()...) }; })
  {
    return dango::max(dango::forward<tp_args>(a_args)...);
  }

  DANGO_UNINSTANTIABLE(min_max_help)
};

/*** aligned_storage ***/

namespace
dango::detail
{
  template
  <dango::usize tp_size, dango::usize tp_align>
  concept aligned_storage_constraint_spec =
    (tp_size != dango::usize(0)) && dango::is_pow_two(tp_align);
}

namespace
dango
{
  template
  <dango::usize tp_size, dango::usize tp_align = alignof(dango::max_align_type)>
  requires(dango::detail::aligned_storage_constraint_spec<tp_size, tp_align>)
  class aligned_storage;

  template
  <typename... tp_types>
  requires(( ... && dango::is_object<tp_types>))
  using aligned_union =
    dango::aligned_storage
    <dango::max(dango::usize(1), sizeof(tp_types)...), dango::max(dango::usize(1), alignof(tp_types)...)>;
}

template
<dango::usize tp_size, dango::usize tp_align>
requires(dango::detail::aligned_storage_constraint_spec<tp_size, tp_align>)
class
dango::
aligned_storage
{
public:
  static inline constexpr dango::usize const c_size = dango::next_multiple(tp_size, tp_align);
  static inline constexpr dango::usize const c_align = tp_align;
public:
  explicit constexpr aligned_storage()noexcept = default;
  ~aligned_storage()noexcept = default;
public:
  constexpr auto get()const noexcept->void*{ return m_storage; }
  template
  <dango::is_object tp_type>
  constexpr auto
  launder_get()const noexcept->tp_type*{ return dango::launder(static_cast<tp_type*>(get())); }
private:
  alignas(c_align) mutable dango::byte m_storage[c_size];
public:
  DANGO_IMMOBILE(aligned_storage)
};

#endif

