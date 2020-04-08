#ifndef DANGO_UTIL_BASE_HPP_INCLUDED
#define DANGO_UTIL_BASE_HPP_INCLUDED

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

/*** in_constexpr_context ***/

namespace dango
{
  constexpr auto in_constexpr_context()noexcept->bool;
}

constexpr auto
dango::
in_constexpr_context
()noexcept->bool
{
  return __builtin_is_constant_evaluated();
}

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

/*** swap ***/

namespace
dango
{
  template
  <typename tp_type1, typename tp_type2>
  requires
  (
    !dango::is_const<tp_type1> &&
    !dango::is_const<tp_type2> &&
    dango::is_same_ignore_cv<tp_type1, tp_type2> &&
    dango::is_scalar<tp_type1>
  )
  constexpr void
  swap
  (tp_type1& a_arg1, tp_type2& a_arg2)noexcept
  {
    dango::remove_volatile<tp_type1> a_temp{ dango::move(a_arg1) };

    a_arg1 = dango::move(a_arg2);

    a_arg2 = dango::move(a_temp);
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

/*** min ***/

namespace
dango
{
  constexpr void min()noexcept{ }

  template
  <typename tp_type>
  requires(dango::is_arithmetic_exclude_bool<tp_type> || dango::is_object_ptr<tp_type>)
  constexpr auto
  min
  (tp_type const a_arg)noexcept->tp_type
  {
    return a_arg;
  }

  template
  <typename tp_type>
  requires(dango::is_arithmetic_exclude_bool<tp_type> || dango::is_object_ptr<tp_type>)
  constexpr auto
  min
  (tp_type const a_arg1, tp_type const a_arg2)noexcept->tp_type
  {
    return a_arg1 < a_arg2 ? a_arg1 : a_arg2;
  }

  template
  <typename tp_type, typename... tp_types>
  requires
  (
    (sizeof...(tp_types) >= dango::usize(2)) &&
    (dango::is_arithmetic_exclude_bool<tp_type> || dango::is_object_ptr<tp_type>) &&
    ( ... && dango::is_same<tp_types, tp_type>)
  )
  constexpr auto
  min
  (tp_type const a_arg, tp_types... a_args)noexcept->tp_type
  {
    return dango::min(a_arg, dango::min(a_args...));
  }
}

/*** max ***/

namespace
dango
{
  constexpr void max()noexcept{ }

  template
  <typename tp_type>
  requires(dango::is_arithmetic_exclude_bool<tp_type> || dango::is_object_ptr<tp_type>)
  constexpr auto
  max
  (tp_type const a_arg)noexcept->tp_type
  {
    return a_arg;
  }

  template
  <typename tp_type>
  requires(dango::is_arithmetic_exclude_bool<tp_type> || dango::is_object_ptr<tp_type>)
  constexpr auto
  max
  (tp_type const a_arg1, tp_type const a_arg2)noexcept->tp_type
  {
    return a_arg1 > a_arg2 ? a_arg1 : a_arg2;
  }

  template
  <typename tp_type, typename... tp_types>
  requires
  (
    (sizeof...(tp_types) >= dango::usize(2)) &&
    (dango::is_arithmetic_exclude_bool<tp_type> || dango::is_object_ptr<tp_type>) &&
    ( ... && dango::is_same<tp_types, tp_type>)
  )
  constexpr auto
  max
  (tp_type const a_arg, tp_types... a_args)noexcept->tp_type
  {
    return dango::max(a_arg, dango::max(a_args...));
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
    return DANGO_MAGIC_CONST_FOLD(reinterpret_cast<ret_type>(static_cast<void*>(nullptr)));
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
    return DANGO_MAGIC_CONST_FOLD(reinterpret_cast<ret_type>(static_cast<void*>(nullptr)));
  }
}

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

