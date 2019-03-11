#ifndef __DANGO_UTIL_HPP__
#define __DANGO_UTIL_HPP__ 1

#include "dango-traits.hpp"
#include "dango-assert.hpp"

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
  <typename tp_type>
  constexpr auto
  swap
  (tp_type&, tp_type&)
  noexcept
  (
    dango::is_noexcept_move_constructible<tp_type> &&
    dango::is_noexcept_move_assignable<tp_type>
  )->
  dango::enable_if
  <
    dango::is_move_constructible<tp_type> &&
    dango::is_move_assignable<tp_type>,
    void
  >;
}


template
<typename tp_type>
constexpr auto
dango::
swap
(tp_type& a_arg1, tp_type& a_arg2)
noexcept
(
  dango::is_noexcept_move_constructible<tp_type> &&
  dango::is_noexcept_move_assignable<tp_type>
)->
dango::enable_if
<
  dango::is_move_constructible<tp_type> &&
  dango::is_move_assignable<tp_type>,
  void
>
{
  tp_type a_temp{ dango::move(a_arg1) };

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
    dango::is_arithmetic<tp_type>,
    dango::remove_cv<tp_type>
  >;

  template
  <typename tp_type>
  constexpr auto
  min
  (tp_type, tp_type)noexcept->
  dango::enable_if
  <
    dango::is_arithmetic<tp_type>,
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
    (dango::is_arithmetic<tp_type> && ... && dango::is_same<tp_types, dango::remove_cv<tp_type>>),
    dango::remove_cv<tp_type>
  >;

  template
  <typename tp_type>
  constexpr auto
  max
  (tp_type)noexcept->
  dango::enable_if
  <
    dango::is_arithmetic<tp_type>,
    dango::remove_cv<tp_type>
  >;

  template
  <typename tp_type>
  constexpr auto
  max
  (tp_type, tp_type)noexcept->
  dango::enable_if
  <
    dango::is_arithmetic<tp_type>,
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
    (dango::is_arithmetic<tp_type> && ... && dango::is_same<tp_types, dango::remove_cv<tp_type>>),
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
  dango::is_arithmetic<tp_type>,
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
  dango::is_arithmetic<tp_type>,
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
  (dango::is_arithmetic<tp_type> && ... && dango::is_same<tp_types, dango::remove_cv<tp_type>>),
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
  dango::is_arithmetic<tp_type>,
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
  dango::is_arithmetic<tp_type>,
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
  (dango::is_arithmetic<tp_type> && ... && dango::is_same<tp_types, dango::remove_cv<tp_type>>),
  dango::remove_cv<tp_type>
>
{
  return dango::max(a_arg1, dango::max(a_arg2, a_args...));
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
  enable_if<dango::is_arithmetic<tp_type>, bool>;

  template
  <typename tp_type>
  constexpr auto
  is_greater
  (tp_type, tp_type)noexcept->
  enable_if<dango::is_arithmetic<tp_type>, bool>;

  template
  <typename tp_type>
  constexpr auto
  is_lequal
  (tp_type, tp_type)noexcept->
  enable_if<dango::is_arithmetic<tp_type>, bool>;

  template
  <typename tp_type>
  constexpr auto
  is_gequal
  (tp_type, tp_type)noexcept->
  enable_if<dango::is_arithmetic<tp_type>, bool>;
}

#define DANGO_DEFINE_COMPARISON_FUNC(name, oper) \
template \
<typename tp_type> \
constexpr auto \
dango:: \
is_##name \
(tp_type const a_lhs, tp_type const a_rhs)noexcept-> \
enable_if<dango::is_arithmetic<tp_type>, bool> \
{ \
  return a_lhs oper a_rhs; \
}

DANGO_DEFINE_COMPARISON_FUNC(equal, == )
DANGO_DEFINE_COMPARISON_FUNC(lesser, < )
DANGO_DEFINE_COMPARISON_FUNC(greater, > )
DANGO_DEFINE_COMPARISON_FUNC(lequal, <= )
DANGO_DEFINE_COMPARISON_FUNC(gequal, >= )

#undef DANGO_DEFINE_COMPARISON_FUNC

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

/*** intrusive_list ***/

#define DANGO_INTRUSIVE_LIST_ELEM_ENABLE_SPEC(tp_elem) \
dango::enable_if \
< \
  !dango::is_const<tp_elem> && \
  !dango::is_volatile<tp_elem> && \
  dango::is_class<tp_elem> \
>

#define DANGO_INTRUSIVE_LIST_ENABLE_SPEC(tp_elem) \
dango::enable_if \
< \
  !dango::is_const<tp_elem> && \
  !dango::is_volatile<tp_elem> && \
  dango::is_class<tp_elem> && \
  dango::is_base_of<dango::intrusive_list_elem<tp_elem>, tp_elem> \
>

namespace
dango
{
  template
  <typename tp_elem, typename tp_enabled = dango::enable_tag>
  class intrusive_list_elem;

  template
  <typename tp_elem>
  class
  intrusive_list_elem<tp_elem, DANGO_INTRUSIVE_LIST_ELEM_ENABLE_SPEC(tp_elem)>;

  template
  <typename tp_elem, typename tp_enabled = dango::enable_tag>
  class intrusive_list;

  template
  <typename tp_elem>
  class
  intrusive_list<tp_elem, DANGO_INTRUSIVE_LIST_ENABLE_SPEC(tp_elem)>;
}

namespace
dango::detail
{
  template
  <typename tp_elem>
  class intrusive_list_sentinel;
}

template
<typename tp_elem>
class
dango::
intrusive_list_elem<tp_elem, DANGO_INTRUSIVE_LIST_ELEM_ENABLE_SPEC(tp_elem)>
{
  friend dango::intrusive_list<tp_elem>;
private:
  using elem_type = intrusive_list_elem;
private:
  elem_type* m_prev;
  elem_type* m_next;
protected:
  constexpr
  intrusive_list_elem()noexcept:
  m_prev{ nullptr },
  m_next{ nullptr }
  {

  }
  ~intrusive_list_elem()noexcept = default;
public:
  DANGO_IMMOBILE(intrusive_list_elem)
};

template
<typename tp_elem>
class
dango::
detail::
intrusive_list_sentinel
final:
public dango::intrusive_list_elem<tp_elem>
{
private:
  using super_type = dango::intrusive_list_elem<tp_elem>;
public:
  constexpr
  intrusive_list_sentinel()noexcept:
  super_type{ }
  {

  }
  ~intrusive_list_sentinel()noexcept = default;
public:
  DANGO_IMMOBILE(intrusive_list_sentinel)
};

template
<typename tp_elem>
class
dango::
intrusive_list<tp_elem, DANGO_INTRUSIVE_LIST_ENABLE_SPEC(tp_elem)>
final
{
private:
  using elem_ptr = tp_elem*;
  using elem_const_ptr = tp_elem const*;
  using elem_type = dango::intrusive_list_elem<tp_elem>;
  using sentinel_type = dango::detail::intrusive_list_sentinel<tp_elem>;
private:
  sentinel_type m_head;
  sentinel_type m_tail;
public:
  static constexpr void
  remove(elem_ptr const a_element)noexcept
  {
    elem_type* const a_elem = a_element;

    dango_assert(a_elem != nullptr);

    elem_type* const a_prev = a_elem->m_prev;
    elem_type* const a_next = a_elem->m_next;

    dango_assert(a_prev != nullptr);
    dango_assert(a_next != nullptr);
    dango_assert(a_prev->m_next == a_elem);
    dango_assert(a_next->m_prev == a_elem);

    a_next->m_prev = a_prev;
    a_prev->m_next = a_next;
    a_elem->m_prev = nullptr;
    a_elem->m_next = nullptr;
  }
public:
  constexpr void
  clear()noexcept
  {
    m_head.m_next = &m_tail;
    m_tail.m_prev = &m_head;
  }

  constexpr
  intrusive_list()noexcept:
  m_head{ },
  m_tail{ }
  {
    clear();
  }

  ~intrusive_list()noexcept = default;

  constexpr auto
  is_empty()const noexcept->bool
  {
    return m_head.m_next == &m_tail;
  }

  constexpr auto
  first()noexcept->elem_ptr
  {
    if(is_empty())
    {
      return nullptr;
    }

    return static_cast<elem_ptr>(m_head.m_next);
  }

  constexpr auto
  last()noexcept->elem_ptr
  {
    if(is_empty())
    {
      return nullptr;
    }

    return static_cast<elem_ptr>(m_tail.m_prev);
  }

  constexpr auto
  first()const noexcept->elem_const_ptr
  {
    if(is_empty())
    {
      return nullptr;
    }

    return static_cast<elem_const_ptr>(m_head.m_next);
  }

  constexpr auto
  last()const noexcept->elem_const_ptr
  {
    if(is_empty())
    {
      return nullptr;
    }

    return static_cast<elem_const_ptr>(m_tail.m_prev);
  }

  constexpr void
  add_first(elem_ptr const a_element)noexcept
  {
    elem_type* const a_elem = a_element;

    dango_assert(a_elem != nullptr);
    dango_assert(a_elem->m_prev == nullptr);
    dango_assert(a_elem->m_next == nullptr);

    elem_type* const a_prev = &m_head;
    elem_type* const a_next = m_head.m_next;

    dango_assert(a_prev->m_next == a_next);
    dango_assert(a_next->m_prev == a_prev);

    a_elem->m_prev = a_prev;
    a_elem->m_next = a_next;
    a_next->m_prev = a_elem;
    a_prev->m_next = a_elem;
  }

  constexpr void
  add_last(elem_ptr const a_element)noexcept
  {
    elem_type* const a_elem = a_element;

    dango_assert(a_elem != nullptr);
    dango_assert(a_elem->m_prev == nullptr);
    dango_assert(a_elem->m_next == nullptr);

    elem_type* const a_prev = m_tail.m_prev;
    elem_type* const a_next = &m_tail;

    dango_assert(a_prev->m_next == a_next);
    dango_assert(a_next->m_prev == a_prev);

    a_elem->m_prev = a_prev;
    a_elem->m_next = a_next;
    a_next->m_prev = a_elem;
    a_prev->m_next = a_elem;
  }

  constexpr auto
  remove_first()noexcept->elem_ptr
  {
    if(is_empty())
    {
      return nullptr;
    }

    auto const a_result = first();

    remove(a_result);

    return a_result;
  }

  constexpr auto
  remove_last()noexcept->elem_ptr
  {
    if(is_empty())
    {
      return nullptr;
    }

    auto const a_result = last();

    remove(a_result);

    return a_result;
  }

  template
  <typename tp_func>
  constexpr auto
  for_each
  (tp_func&& a_func)
  noexcept(dango::is_noexcept_callable_ret<void, dango::remove_ref<tp_func>&, elem_ptr const&>)->
  dango::enable_if<is_callable_ret<void, dango::remove_ref<tp_func>&, elem_ptr const&>, void>
  {
    elem_type* a_current = m_head.m_next;
    elem_type const* const a_end = &m_tail;

    while(a_current != a_end)
    {
      elem_type* const a_next = a_current->m_next;

      auto const a_elem = static_cast<elem_ptr>(a_current);

      a_func(a_elem);

      a_current = a_next;
    }
  }

  template
  <typename tp_func>
  constexpr auto
  for_each
  (tp_func&& a_func)const
  noexcept(dango::is_noexcept_callable_ret<void, dango::remove_ref<tp_func>&, elem_const_ptr const&>)->
  dango::enable_if<is_callable_ret<void, dango::remove_ref<tp_func>&, elem_const_ptr const&>, void>
  {
    elem_type const* a_current = m_head.m_next;
    elem_type const* const a_end = &m_tail;

    while(a_current != a_end)
    {
      elem_type const* const a_next = a_current->m_next;

      auto const a_elem = static_cast<elem_const_ptr>(a_current);

      a_func(a_elem);

      a_current = a_next;
    }
  }
public:
  DANGO_IMMOBILE(intrusive_list)
};

#undef DANGO_INTRUSIVE_LIST_ELEM_ENABLE_SPEC

#undef DANGO_INTRUSIVE_LIST_ENABLE_SPEC

#endif

