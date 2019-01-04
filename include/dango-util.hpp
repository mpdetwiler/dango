#ifndef __DANGO_UTIL_HPP__
#define __DANGO_UTIL_HPP__ 1

#include "dango-traits.hpp"

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

#endif

