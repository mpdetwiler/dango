#ifndef __DANGO_UTIL_HPP__
#define __DANGO_UTIL_HPP__ 1

#include "dango-traits.hpp"

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
  tp_type a_temp{dango::move(a_arg1)};

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

#define DANGO_DEFINE_COMPARISON_FUNC(name, oper)          \
template                                                  \
<typename tp_type>                                        \
constexpr auto                                            \
dango::                                                   \
is_##name                                                 \
(tp_type const a_lhs, tp_type const a_rhs)noexcept->      \
enable_if<dango::is_arithmetic<tp_type>, bool>            \
{                                                         \
  return a_lhs oper a_rhs;                                \
}

DANGO_DEFINE_COMPARISON_FUNC(equal, == )
DANGO_DEFINE_COMPARISON_FUNC(lesser, < )
DANGO_DEFINE_COMPARISON_FUNC(greater, > )
DANGO_DEFINE_COMPARISON_FUNC(lequal, <= )
DANGO_DEFINE_COMPARISON_FUNC(gequal, >= )

#undef DANGO_DEFINE_COMPARISON_FUNC

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

#endif

