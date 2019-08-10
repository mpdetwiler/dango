#ifndef __DANGO_UTIL_HPP__
#define __DANGO_UTIL_HPP__

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

  template
  <typename tp_int, tp_int tp_len>
  using make_integer_seq = dango::integer_seq<tp_int, __integer_pack(tp_len)...>;

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

/*** swap ***/

namespace
dango
{
  template
  <typename tp_type1, typename tp_type2>
  constexpr auto
  swap
  (tp_type1&, tp_type2&)noexcept->
  dango::enable_if
  <
    !dango::is_const<tp_type1> &&
    !dango::is_const<tp_type2> &&
    dango::is_same<dango::remove_volatile<tp_type1>, dango::remove_volatile<tp_type2>> &&
    dango::is_scalar<tp_type1>,
    void
  >;
}


template
<typename tp_type1, typename tp_type2>
constexpr auto
dango::
swap
(tp_type1& a_arg1, tp_type2& a_arg2)noexcept->
dango::enable_if
<
  !dango::is_const<tp_type1> &&
  !dango::is_const<tp_type2> &&
  dango::is_same<dango::remove_volatile<tp_type1>, dango::remove_volatile<tp_type2>> &&
  dango::is_scalar<tp_type1>,
  void
>
{
  dango::remove_volatile<tp_type1> a_temp{ dango::move(a_arg1) };

  a_arg1 = dango::move(a_arg2);

  a_arg2 = dango::move(a_temp);
}

/*** is_pow_two ***/

namespace
dango
{
  template
  <typename tp_int>
  constexpr auto
  is_pow_two
  (tp_int)noexcept->
  dango::enable_if<dango::is_int<tp_int>, bool>;
}

template
<typename tp_int>
constexpr auto
dango::
is_pow_two
(tp_int const a_arg)noexcept->
dango::enable_if<dango::is_int<tp_int>, bool>
{
  if(a_arg <= tp_int(0))
  {
    return false;
  }

  return tp_int(a_arg & (a_arg - tp_int(1))) == tp_int(0);
}

/*** is_equal is_lesser is_greater is_lequal is_gequal ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr auto
  is_equal
  (tp_type, tp_type)noexcept->
  enable_if<dango::is_arithmetic<tp_type>, bool>;

  template
  <typename tp_type>
  constexpr auto
  is_lesser
  (tp_type, tp_type)noexcept->
  enable_if<dango::is_nonbool_arithmetic<tp_type>, bool>;

  template
  <typename tp_type>
  constexpr auto
  is_greater
  (tp_type, tp_type)noexcept->
  enable_if<dango::is_nonbool_arithmetic<tp_type>, bool>;

  template
  <typename tp_type>
  constexpr auto
  is_lequal
  (tp_type, tp_type)noexcept->
  enable_if<dango::is_nonbool_arithmetic<tp_type>, bool>;

  template
  <typename tp_type>
  constexpr auto
  is_gequal
  (tp_type, tp_type)noexcept->
  enable_if<dango::is_nonbool_arithmetic<tp_type>, bool>;
}

template
<typename tp_type>
constexpr auto
dango::
is_equal
(tp_type const a_lhs, tp_type const a_rhs)noexcept->
enable_if<dango::is_arithmetic<tp_type>, bool>
{
  return a_lhs == a_rhs;
}

#define DANGO_DEFINE_COMPARISON_FUNC(name, oper) \
template \
<typename tp_type> \
constexpr auto \
dango:: \
is_##name \
(tp_type const a_lhs, tp_type const a_rhs)noexcept-> \
enable_if<dango::is_nonbool_arithmetic<tp_type>, bool> \
{ \
  return a_lhs oper a_rhs; \
}

DANGO_DEFINE_COMPARISON_FUNC(lesser, < )
DANGO_DEFINE_COMPARISON_FUNC(greater, > )
DANGO_DEFINE_COMPARISON_FUNC(lequal, <= )
DANGO_DEFINE_COMPARISON_FUNC(gequal, >= )

#undef DANGO_DEFINE_COMPARISON_FUNC

/*** min max ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr auto
  min
  (tp_type)noexcept->
  dango::enable_if
  <
    dango::is_nonbool_arithmetic<tp_type> || dango::is_object_ptr<tp_type>,
    dango::remove_cv<tp_type>
  >;

  template
  <typename tp_type>
  constexpr auto
  min
  (tp_type, tp_type)noexcept->
  dango::enable_if
  <
    dango::is_nonbool_arithmetic<tp_type> || dango::is_object_ptr<tp_type>,
    dango::remove_cv<tp_type>
  >;

  template
  <typename tp_type, typename... tp_types>
  constexpr auto
  min
  (tp_type, tp_type, tp_types...)
  noexcept->
  dango::enable_if
  <
    !dango::is_equal(sizeof...(tp_types), dango::usize(0)) &&
    (dango::is_nonbool_arithmetic<tp_type> || dango::is_object_ptr<tp_type>) &&
    (... && dango::is_same<tp_types, dango::remove_cv<tp_type>>),
    dango::remove_cv<tp_type>
  >;

  template
  <typename tp_type>
  constexpr auto
  max
  (tp_type)noexcept->
  dango::enable_if
  <
    dango::is_nonbool_arithmetic<tp_type> || dango::is_object_ptr<tp_type>,
    dango::remove_cv<tp_type>
  >;

  template
  <typename tp_type>
  constexpr auto
  max
  (tp_type, tp_type)noexcept->
  dango::enable_if
  <
    dango::is_nonbool_arithmetic<tp_type> || dango::is_object_ptr<tp_type>,
    dango::remove_cv<tp_type>
  >;

  template
  <typename tp_type, typename... tp_types>
  constexpr auto
  max
  (tp_type, tp_type, tp_types...)
  noexcept->
  dango::enable_if
  <
    !dango::is_equal(sizeof...(tp_types), dango::usize(0)) &&
    (dango::is_nonbool_arithmetic<tp_type> || dango::is_object_ptr<tp_type>) &&
    (... && dango::is_same<tp_types, dango::remove_cv<tp_type>>),
    dango::remove_cv<tp_type>
  >;
}

template
<typename tp_type>
constexpr auto
dango::
min
(tp_type const a_arg)noexcept->
dango::enable_if
<
  dango::is_nonbool_arithmetic<tp_type> || dango::is_object_ptr<tp_type>,
  dango::remove_cv<tp_type>
>
{
  return a_arg;
}

template
<typename tp_type>
constexpr auto
dango::
min
(tp_type const a_arg1, tp_type const a_arg2)noexcept->
dango::enable_if
<
  dango::is_nonbool_arithmetic<tp_type> || dango::is_object_ptr<tp_type>,
  dango::remove_cv<tp_type>
>
{
  return a_arg1 < a_arg2 ? a_arg1 : a_arg2;
}

template
<typename tp_type, typename... tp_types>
constexpr auto
dango::
min
(
  tp_type const a_arg1,
  tp_type const a_arg2,
  tp_types... a_args
)
noexcept->
dango::enable_if
<
  !dango::is_equal(sizeof...(tp_types), dango::usize(0)) &&
  (dango::is_nonbool_arithmetic<tp_type> || dango::is_object_ptr<tp_type>) &&
  (... && dango::is_same<tp_types, dango::remove_cv<tp_type>>),
  dango::remove_cv<tp_type>
>
{
  return dango::min(a_arg1, dango::min(a_arg2, a_args...));
}

template
<typename tp_type>
constexpr auto
dango::
max
(tp_type const a_arg)noexcept->
dango::enable_if
<
  dango::is_nonbool_arithmetic<tp_type> || dango::is_object_ptr<tp_type>,
  dango::remove_cv<tp_type>
>
{
  return a_arg;
}

template
<typename tp_type>
constexpr auto
dango::
max
(tp_type const a_arg1, tp_type const a_arg2)noexcept->
dango::enable_if
<
  dango::is_nonbool_arithmetic<tp_type> || dango::is_object_ptr<tp_type>,
  dango::remove_cv<tp_type>
>
{
  return a_arg1 > a_arg2 ? a_arg1 : a_arg2;
}

template
<typename tp_type, typename... tp_types>
constexpr auto
dango::
max
(
  tp_type const a_arg1,
  tp_type const a_arg2,
  tp_types... a_args
)
noexcept->
dango::enable_if
<
  !dango::is_equal(sizeof...(tp_types), dango::usize(0)) &&
  (dango::is_nonbool_arithmetic<tp_type> || dango::is_object_ptr<tp_type>) &&
  (... && dango::is_same<tp_types, dango::remove_cv<tp_type>>),
  dango::remove_cv<tp_type>
>
{
  return dango::max(a_arg1, dango::max(a_arg2, a_args...));
}

/*** logical ***/

namespace
dango
{
  constexpr auto logical_implication(bool, bool)noexcept->bool;
  constexpr auto logical_equivalence(bool, bool)noexcept->bool;
  constexpr auto logical_and(bool, bool)noexcept->bool;
  constexpr auto logical_or(bool, bool)noexcept->bool;
  constexpr auto logical_xor(bool, bool)noexcept->bool;
  constexpr auto logical_not(bool)noexcept->bool;
}

constexpr auto
dango::
logical_implication
(bool const a_lhs, bool const a_rhs)noexcept->bool
{
  return !a_lhs || a_rhs;
}

constexpr auto
dango::
logical_equivalence
(bool const a_lhs, bool const a_rhs)noexcept->bool
{
  return a_lhs == a_rhs;
}

constexpr auto
dango::
logical_and
(bool const a_lhs, bool const a_rhs)noexcept->bool
{
  return a_lhs && a_rhs;
}

constexpr auto
dango::
logical_or
(bool const a_lhs, bool const a_rhs)noexcept->bool
{
  return a_lhs || a_rhs;
}

constexpr auto
dango::
logical_xor
(bool const a_lhs, bool const a_rhs)noexcept->bool
{
  return a_lhs != a_rhs;
}

constexpr auto
dango::
logical_not
(bool const a_arg)noexcept->bool
{
  return !a_arg;
}

/*** next_multiple ***/

namespace
dango
{
  template
  <typename tp_uint>
  constexpr auto
  next_multiple
  (tp_uint, tp_uint)noexcept->
  dango::enable_if<dango::is_uint<tp_uint>, dango::remove_cv<tp_uint>>;
}

template
<typename tp_uint>
constexpr auto
dango::
next_multiple
(tp_uint const a_arg1, tp_uint const a_arg2)noexcept->
dango::enable_if<dango::is_uint<tp_uint>, dango::remove_cv<tp_uint>>
{
  tp_uint const a_div = a_arg1 / a_arg2;
  tp_uint const a_mod = a_arg1 % a_arg2;

  return (a_mod != tp_uint(0)) ? (a_arg2 * (a_div + tp_uint(1))) : a_arg1;
}

/*** make_guard ***/

namespace
dango::detail
{
  template
  <typename tp_func>
  class scope_guard;
}

template
<typename tp_func>
class
dango::
detail::
scope_guard
{
private:
  using func_type = dango::decay<tp_func>;
  static_assert(dango::is_same<func_type, tp_func>);
public:
  template
  <typename tp_func_type, dango::enable_if<dango::is_constructible<func_type, tp_func_type>> = dango::enable_val>
  explicit constexpr
  scope_guard
  (tp_func_type&& a_func)noexcept(dango::is_noexcept_constructible<func_type, tp_func_type>):
  m_func{ dango::forward<tp_func_type>(a_func) },
  m_dismissed{ false }
  {

  }

  ~scope_guard()noexcept;
  void dismiss()noexcept;
private:
  func_type m_func;
  bool m_dismissed;
public:
  DANGO_DELETE_DEFAULT(scope_guard)
  DANGO_IMMOBILE(scope_guard)
};

template
<typename tp_func>
dango::
detail::
scope_guard<tp_func>::
~scope_guard
()noexcept
{
  static_assert(noexcept(m_func()));
  static_assert(dango::is_noexcept_destructible<func_type>);

  if(m_dismissed)
  {
    return;
  }

  m_func();
}

template
<typename tp_func>
void
dango::
detail::
scope_guard<tp_func>::
dismiss
()noexcept
{
  m_dismissed = true;
}

namespace
dango
{
  template
  <
    typename tp_func,
    typename tp_ret = dango::detail::scope_guard<dango::decay<tp_func>>,
    typename tp_enabled = decltype(tp_ret{ dango::declval<tp_func>() })
  >
  [[nodiscard]] auto
  make_guard
  (tp_func&&)noexcept(noexcept(tp_ret{ dango::declval<tp_func>() }))->
  dango::enable_if
  <
    dango::is_noexcept_callable_ret<void, dango::decay<tp_func>&> &&
    dango::is_noexcept_destructible<dango::decay<tp_func>>,
    tp_ret
  >;
}

template
<
  typename tp_func,
  typename tp_ret,
  typename tp_enabled
>
auto
dango::
make_guard
(tp_func&& a_func)noexcept(noexcept(tp_ret{ dango::declval<tp_func>() }))->
dango::enable_if
<
  dango::is_noexcept_callable_ret<void, dango::decay<tp_func>&> &&
  dango::is_noexcept_destructible<dango::decay<tp_func>>,
  tp_ret
>
{
  return tp_ret{ dango::forward<tp_func>(a_func) };
}

/*** make_finally ***/

namespace
dango::detail
{
  template
  <typename tp_func>
  class finally_guard;
}

template
<typename tp_func>
class
dango::
detail::
finally_guard
{
private:
  using func_type = dango::decay<tp_func>;
  static_assert(dango::is_same<func_type, tp_func>);
public:
  template
  <typename tp_func_type, dango::enable_if<dango::is_constructible<func_type, tp_func_type>> = dango::enable_val>
  explicit constexpr
  finally_guard
  (tp_func_type&& a_func)noexcept(dango::is_noexcept_constructible<func_type, tp_func_type>):
  m_func{ dango::forward<tp_func_type>(a_func) }
  {

  }

  ~finally_guard()noexcept;
private:
  func_type m_func;
public:
  DANGO_DELETE_DEFAULT(finally_guard)
  DANGO_IMMOBILE(finally_guard)
};

template
<typename tp_func>
dango::
detail::
finally_guard<tp_func>::
~finally_guard
()noexcept
{
  static_assert(noexcept(m_func()));
  static_assert(dango::is_noexcept_destructible<func_type>);

  m_func();
}

namespace
dango
{
  template
  <
    typename tp_func,
    typename tp_ret = dango::detail::finally_guard<dango::decay<tp_func>>,
    typename tp_enabled = decltype(tp_ret{ dango::declval<tp_func>() })
  >
  [[nodiscard]] auto
  make_finally
  (tp_func&&)noexcept(noexcept(tp_ret{ dango::declval<tp_func>() }))->
  dango::enable_if
  <
    dango::is_noexcept_callable_ret<void, dango::decay<tp_func>&> &&
    dango::is_noexcept_destructible<dango::decay<tp_func>>,
    tp_ret
  >;
}

template
<
  typename tp_func,
  typename tp_ret,
  typename tp_enabled
>
auto
dango::
make_finally
(tp_func&& a_func)noexcept(noexcept(tp_ret{ dango::declval<tp_func>() }))->
dango::enable_if
<
  dango::is_noexcept_callable_ret<void, dango::decay<tp_func>&> &&
  dango::is_noexcept_destructible<dango::decay<tp_func>>,
  tp_ret
>
{
  return tp_ret{ dango::forward<tp_func>(a_func) };
}

/*** invoker ***/

namespace
dango::detail
{
  template
  <
    typename tp_type,
    typename tp_nocv = dango::remove_cv<tp_type>,
    typename tp_enabled = dango::enable_tag
  >
  struct invoker_help;

  template
  <typename tp_type, typename tp_func, typename tp_class>
  struct
  invoker_help
  <tp_type, tp_func tp_class::*, dango::enable_if<dango::is_func<tp_func>>>;
}

namespace
dango
{
  template
  <typename tp_type>
  using invoker = typename detail::invoker_help<tp_type>::type;
}

template
<
  typename tp_type,
  typename tp_nocv,
  typename tp_enabled
>
struct
dango::
detail::
invoker_help
final
{
  using type = tp_type;

  DANGO_UNINSTANTIABLE(invoker_help)
};

template
<typename tp_type, typename tp_func, typename tp_class>
struct
dango::
detail::
invoker_help
<tp_type, tp_func tp_class::*, dango::enable_if<dango::is_func<tp_func>>>
final
{
public:
  using type = invoker_help;
private:
  using value_type = tp_func tp_class::*;
public:
  explicit constexpr invoker_help(value_type const a_method)noexcept:m_method{ a_method }{ }
  constexpr invoker_help(invoker_help const&)noexcept = default;
  constexpr invoker_help(invoker_help&&)noexcept = default;
  ~invoker_help()noexcept = default;
  constexpr auto operator = (invoker_help const&)&noexcept->invoker_help& = default;
  constexpr auto operator = (invoker_help&&)&noexcept->invoker_help& = default;

  template
  <
    typename tp_first,
    typename... tp_args,
    typename tp_enabled =
      decltype(((dango::declval<tp_first>()).*(dango::declval<value_type const&>()))(dango::declval<tp_args>()...))
  >
  constexpr auto
  operator ()
  (tp_first&& a_class, tp_args&&... a_args)const
  noexcept(noexcept(((dango::declval<tp_first>()).*(dango::declval<value_type const&>()))(dango::declval<tp_args>()...)))->
  decltype(auto)
  {
    return ((dango::forward<tp_first>(a_class)).*m_method)(dango::forward<tp_args>(a_args)...);
  }

  template
  <
    typename tp_first,
    typename... tp_args,
    typename tp_enabled =
      decltype(((dango::declval<tp_first* const&>())->*(dango::declval<value_type const&>()))(dango::declval<tp_args>()...))
  >
  constexpr auto
  operator ()
  (tp_first* const a_class, tp_args&&... a_args)const
  noexcept(noexcept(((dango::declval<tp_first* const&>())->*(dango::declval<value_type const&>()))(dango::declval<tp_args>()...)))->
  decltype(auto)
  {
    return (a_class->*m_method)(dango::forward<tp_args>(a_args)...);
  }
private:
  value_type m_method;
public:
  DANGO_DELETE_DEFAULT(invoker_help)
};

#endif

