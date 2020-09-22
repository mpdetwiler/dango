#ifndef DANGO_ARRAY_HPP_INCLUDED
#define DANGO_ARRAY_HPP_INCLUDED

#include "dango-assert.hpp"
#include "dango-util-base.hpp"
#include "dango-hash.hpp"

namespace
dango
{
  template
  <typename tp_elem>
  concept array_constraint_spec =
    dango::is_object_exclude_array<tp_elem> &&
    dango::is_noexcept_destructible<tp_elem>;

  template
  <dango::array_constraint_spec tp_elem, dango::usize tp_size>
  class array;

  template
  <dango::array_constraint_spec tp_elem>
  class array<tp_elem, dango::usize(0)>;
}

template
<dango::array_constraint_spec tp_elem, dango::usize tp_size>
class
dango::
array
{
public:
  using elem_type = tp_elem;
  using ptr_type = elem_type*;
  using cptr_type = elem_type const*;
  using ref_type = elem_type&;
  using cref_type = elem_type const&;
  using rref_type = elem_type&&;
  using crref_type = elem_type const&&;
  using size_type = dango::usize;
public:
  constexpr auto operator [] (size_type const a_index)& noexcept->ref_type{ dango_assert(a_index < size()); return m_array[a_index]; }
  constexpr auto operator [] (size_type const a_index)const& noexcept->cref_type{ dango_assert(a_index < size()); return m_array[a_index]; }
  constexpr auto operator [] (size_type const a_index)&& noexcept->rref_type{ return dango::move((*this)[a_index]); }
  constexpr auto operator [] (size_type const a_index)const&& noexcept->crref_type{ return dango::move((*this)[a_index]); }

  explicit constexpr operator ptr_type()noexcept{ return data(); }
  explicit constexpr operator cptr_type()const noexcept{ return data(); }
public:
  constexpr auto size()const noexcept->size_type{ return tp_size; }
  constexpr auto is_empty()const noexcept->bool{ return false; }

  constexpr auto first()& noexcept->ref_type{ return m_array[size_type(0)]; }
  constexpr auto first()const& noexcept->cref_type{ return m_array[size_type(0)]; }
  constexpr auto first()&& noexcept->rref_type{ return dango::move(first()); }
  constexpr auto first()const&& noexcept->crref_type{ return dango::move(first()); }

  constexpr auto last()& noexcept->ref_type{ return m_array[size() - size_type(1)]; }
  constexpr auto last()const& noexcept->cref_type{ return m_array[size() - size_type(1)]; }
  constexpr auto last()&& noexcept->rref_type{ return dango::move(last()); }
  constexpr auto last()const&& noexcept->crref_type{ return dango::move(last()); }

  constexpr auto data()noexcept->ptr_type{ return m_array; }
  constexpr auto data()const noexcept->cptr_type{ return m_array; }
public:
  elem_type m_array[tp_size];
};

template
<dango::array_constraint_spec tp_elem>
class
dango::
array<tp_elem, dango::usize(0)>
{
public:
  using elem_type = tp_elem;
  using ptr_type = elem_type*;
  using cptr_type = elem_type const*;
  using ref_type = elem_type&;
  using cref_type = elem_type const&;
  using rref_type = elem_type&&;
  using crref_type = elem_type const&&;
  using size_type = dango::usize;
public:
  constexpr auto operator [] (size_type const)& noexcept->ref_type{ dango_unreachable; }
  constexpr auto operator [] (size_type const)const& noexcept->cref_type{ dango_unreachable; }
  constexpr auto operator [] (size_type const)&& noexcept->rref_type{ dango_unreachable; }
  constexpr auto operator [] (size_type const)const&& noexcept->crref_type{ dango_unreachable; }

  explicit constexpr operator ptr_type()noexcept{ return data(); }
  explicit constexpr operator cptr_type()const noexcept{ return data(); }
public:
  constexpr auto size()const noexcept->size_type{ return size_type(0); }
  constexpr auto is_empty()const noexcept->bool{ return true; }

  constexpr auto first()& noexcept->ref_type{ dango_unreachable; }
  constexpr auto first()const& noexcept->cref_type{ dango_unreachable; }
  constexpr auto first()&& noexcept->rref_type{ dango_unreachable; }
  constexpr auto first()const&& noexcept->crref_type{ dango_unreachable; }

  constexpr auto last()& noexcept->ref_type{ dango_unreachable; }
  constexpr auto last()const& noexcept->cref_type{ dango_unreachable; }
  constexpr auto last()&& noexcept->rref_type{ dango_unreachable; }
  constexpr auto last()const&& noexcept->crref_type{ dango_unreachable; }

  constexpr auto data()noexcept->ptr_type{ return dango::null; }
  constexpr auto data()const noexcept->cptr_type{ return dango::null; }
};

namespace
dango
{
  template
  <typename... tp_args>
  requires(requires{ typename dango::common_type<tp_args...>; })
  array(tp_args&&...)->array<dango::common_type<tp_args...>, sizeof...(tp_args)>;

  template
  <typename tp_elem, dango::usize tp_size>
  array(tp_elem(&)[tp_size])->array<tp_elem, tp_size>;
}

namespace
dango::custom
{
  template
  <typename tp_lhs, dango::usize tp_lsz, typename tp_rhs, dango::usize tp_rsz>
  requires(dango::is_equatable<tp_lhs const&, tp_rhs const&>)
  struct
  operator_equals<dango::array<tp_lhs, tp_lsz>, dango::array<tp_rhs, tp_rsz>>;

  template
  <typename tp_lhs, dango::usize tp_lsz, typename tp_rhs, dango::usize tp_rsz>
  requires(dango::is_comparable<tp_lhs const&, tp_rhs const&>)
  struct
  operator_compare<dango::array<tp_lhs, tp_lsz>, dango::array<tp_rhs, tp_rsz>>;

  template
  <typename tp_elem, dango::usize tp_size>
  requires(dango::is_hashable<tp_elem const&>)
  struct
  operator_hash<dango::array<tp_elem, tp_size>>;

  template
  <typename tp_lhs, typename tp_rhs, dango::usize tp_size>
  requires(dango::is_swappable<tp_lhs&, tp_rhs&>)
  struct
  operator_swap<dango::array<tp_lhs, tp_size>, dango::array<tp_rhs, tp_size>>;

  template
  <typename tp_elem, dango::usize tp_size>
  struct
  operator_get<dango::array<tp_elem, tp_size>>;

  template
  <typename tp_elem, dango::usize tp_size>
  struct
  operator_iter<dango::array<tp_elem, tp_size>>;
}

template
<typename tp_lhs, dango::usize tp_lsz, typename tp_rhs, dango::usize tp_rsz>
requires(dango::is_equatable<tp_lhs const&, tp_rhs const&>)
struct
dango::
custom::
operator_equals<dango::array<tp_lhs, tp_lsz>, dango::array<tp_rhs, tp_rsz>>
final
{
  using array_lhs = dango::array<tp_lhs, tp_lsz>;
  using array_rhs = dango::array<tp_rhs, tp_rsz>;

  static constexpr auto
  equals
  (array_lhs const& a_lhs, array_rhs const& a_rhs)
  noexcept
  (
    (tp_lsz != tp_rsz || tp_lsz == dango::usize(0)) ||
    dango::is_noexcept_equatable<tp_lhs const&, tp_rhs const&>
  )->bool
  {
    if constexpr(tp_lsz != tp_rsz)
    {
      return false;
    }
    else
    {
      if constexpr(tp_lsz == dango::usize(0))
      {
        return true;
      }
      else
      {
        return dango::equals(a_lhs.m_array, a_rhs.m_array);
      }
    }
  }

  DANGO_UNCONSTRUCTIBLE(operator_equals)
};

template
<typename tp_lhs, dango::usize tp_lsz, typename tp_rhs, dango::usize tp_rsz>
requires(dango::is_comparable<tp_lhs const&, tp_rhs const&>)
struct
dango::
custom::
operator_compare<dango::array<tp_lhs, tp_lsz>, dango::array<tp_rhs, tp_rsz>>
final
{
  using array_lhs = dango::array<tp_lhs, tp_lsz>;
  using array_rhs = dango::array<tp_rhs, tp_rsz>;

  using ret_type =
    decltype(dango::compare(dango::declval<tp_lhs const&>(), dango::declval<tp_rhs const&>()));

  static constexpr auto
  compare
  (array_lhs const& a_lhs, array_rhs const& a_rhs)
  noexcept
  (
    (tp_lsz == dango::usize(0) || tp_rsz == dango::usize(0)) ||
    dango::is_noexcept_comparable<tp_lhs const&, tp_rhs const&>
  )->ret_type
  {
    if constexpr(tp_lsz == dango::usize(0) || tp_rsz == dango::usize(0))
    {
      return dango::compare(tp_lsz, tp_rsz);
    }
    else
    {
      return dango::compare(a_lhs.m_array, a_rhs.m_array);
    }
  }

  DANGO_UNCONSTRUCTIBLE(operator_compare)
};

template
<typename tp_elem, dango::usize tp_size>
requires(dango::is_hashable<tp_elem const&>)
struct
dango::
custom::
operator_hash<dango::array<tp_elem, tp_size>>
final
{
  using array_type = dango::array<tp_elem, tp_size>;

  static constexpr auto
  hash
  (array_type const& a_array)
  noexcept((tp_size == dango::usize(0)) || dango::has_noexcept_hash<tp_elem const&>)->dango::hash_val
  {
    if constexpr(tp_size == dango::usize(0))
    {
      return dango::hash_val{ dango::usize(0) };
    }
    else
    {
      return dango::hash(a_array.m_array);
    }
  }

  DANGO_UNCONSTRUCTIBLE(operator_hash)
};

template
<typename tp_lhs, typename tp_rhs, dango::usize tp_size>
requires(dango::is_swappable<tp_lhs&, tp_rhs&>)
struct
dango::
custom::
operator_swap<dango::array<tp_lhs, tp_size>, dango::array<tp_rhs, tp_size>>
final
{
  using array_lhs = dango::array<tp_lhs, tp_size>;
  using array_rhs = dango::array<tp_rhs, tp_size>;

  static constexpr void
  swap
  (array_lhs& a_lhs, array_rhs& a_rhs)
  noexcept((tp_size == dango::usize(0)) || dango::is_noexcept_swappable<tp_lhs&, tp_rhs&>)
  {
    if constexpr(tp_size != dango::usize(0))
    {
      dango::swap(a_lhs.m_array, a_rhs.m_array);
    }
  }

  DANGO_UNCONSTRUCTIBLE(operator_swap)
};

template
<typename tp_elem, dango::usize tp_size>
struct
dango::
custom::
operator_get<dango::array<tp_elem, tp_size>>
final
{
  using array_type = dango::array<tp_elem, tp_size>;

  template
  <dango::usize tp_index, dango::is_same_ignore_cvref<array_type> tp_arg>
  requires(tp_index < tp_size)
  static constexpr auto
  get
  (tp_arg&& a_array)noexcept->decltype(auto)
  {
    return dango::forward<tp_arg>(a_array)[tp_index];
  }

  DANGO_UNCONSTRUCTIBLE(operator_get)
};

template
<typename tp_elem, dango::usize tp_size>
struct
dango::
custom::
operator_iter<dango::array<tp_elem, tp_size>>
final
{
  using array_type = dango::array<tp_elem, tp_size>;

  template
  <dango::is_same_ignore_cvref<array_type> tp_arg>
  static constexpr auto
  begin
  (tp_arg&& a_array)noexcept->auto
  {
    return a_array.data();
  }

  static constexpr auto
  end
  (array_type const& a_array)noexcept->void const volatile*
  {
    if constexpr(tp_size != dango::usize(0))
    {
      return a_array.data() + tp_size;
    }
    else
    {
      return dango::null;
    }
  }

  DANGO_UNCONSTRUCTIBLE(operator_iter)
};

#endif // DANGO_ARRAY_HPP_INCLUDED
