#ifndef DANGO_INTRUSIVE_LIST_HPP_INCLUDED
#define DANGO_INTRUSIVE_LIST_HPP_INCLUDED

/*** intrusive_list ***/

namespace
dango
{
  template
  <dango::is_class tp_elem>
  requires(!dango::is_const<tp_elem> && !dango::is_volatile<tp_elem>)
  class intrusive_list_elem;

  template
  <typename tp_elem>
  concept intrusive_list_constraint_spec =
    dango::is_class<tp_elem> &&
    dango::is_derived_from<tp_elem, dango::intrusive_list_elem<tp_elem>> &&
    !dango::is_const<tp_elem> &&
    !dango::is_volatile<tp_elem>;

  template
  <dango::intrusive_list_constraint_spec tp_elem>
  class intrusive_list;
}

namespace
dango::detail
{
  template
  <typename tp_elem>
  class intrusive_list_sentinel;
}

template
<dango::is_class tp_elem>
requires(!dango::is_const<tp_elem> && !dango::is_volatile<tp_elem>)
class
dango::
intrusive_list_elem
{
  template
  <dango::intrusive_list_constraint_spec>
  friend class intrusive_list;
private:
  using elem_type = intrusive_list_elem;
private:
  elem_type* m_prev;
  elem_type* m_next;
protected:
  constexpr
  intrusive_list_elem()noexcept:
  m_prev{ dango::null },
  m_next{ dango::null }
  {

  }
  ~intrusive_list_elem()noexcept = default;
public:
  DANGO_UNMOVEABLE(intrusive_list_elem)
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
  DANGO_UNMOVEABLE(intrusive_list_sentinel)
};

template
<dango::intrusive_list_constraint_spec tp_elem>
class
dango::
intrusive_list
final
{
private:
  using elem_ptr = tp_elem*;
  using elem_const_ptr = tp_elem const*;
  using elem_type = dango::intrusive_list_elem<tp_elem>;
  using sentinel_type = dango::detail::intrusive_list_sentinel<tp_elem>;
public:
  static constexpr void remove(elem_ptr const a_element)noexcept;
public:
  constexpr intrusive_list()noexcept;
  ~intrusive_list()noexcept = default;
public:
  constexpr void clear()noexcept;
  constexpr auto is_empty()const noexcept->bool;
  constexpr auto first()noexcept->elem_ptr;
  constexpr auto last()noexcept->elem_ptr;
  constexpr auto first()const noexcept->elem_const_ptr;
  constexpr auto last()const noexcept->elem_const_ptr;
  constexpr void add_first(elem_ptr const a_element)noexcept;
  constexpr void add_last(elem_ptr const a_element)noexcept;
  constexpr auto remove_first()noexcept->elem_ptr;
  constexpr auto remove_last()noexcept->elem_ptr;
public:
  template
  <typename tp_func>
  requires(dango::is_callable_ret<void, tp_func, elem_ptr const&>)
  constexpr void
  for_each
  (tp_func&& a_func)noexcept(dango::is_noexcept_callable_ret<void, tp_func, elem_ptr const&>)
  {
    elem_type* a_current = m_head.m_next;
    elem_type const* const a_end = &m_tail;

    while(a_current != a_end)
    {
      elem_type* const a_next = a_current->m_next;

      auto const a_elem = static_cast<elem_ptr>(a_current);

      dango::forward<tp_func>(a_func)(a_elem);

      a_current = a_next;
    }
  }

  template
  <typename tp_func>
  requires(dango::is_callable_ret<void, tp_func, elem_const_ptr const&>)
  constexpr void
  for_each
  (tp_func&& a_func)const noexcept(dango::is_noexcept_callable_ret<void, tp_func, elem_const_ptr const&>)
  {
    elem_type const* a_current = m_head.m_next;
    elem_type const* const a_end = &m_tail;

    while(a_current != a_end)
    {
      elem_type const* const a_next = a_current->m_next;

      auto const a_elem = static_cast<elem_const_ptr>(a_current);

      dango::forward<tp_func>(a_func)(a_elem);

      a_current = a_next;
    }
  }
private:
  sentinel_type m_head;
  sentinel_type m_tail;
public:
  DANGO_UNMOVEABLE(intrusive_list)
};

template
<dango::intrusive_list_constraint_spec tp_elem>
constexpr void
dango::
intrusive_list<tp_elem>::
remove(elem_ptr const a_element)noexcept
{
  elem_type* const a_elem = a_element;

  dango_assert(a_elem != dango::null);

  elem_type* const a_prev = a_elem->m_prev;
  elem_type* const a_next = a_elem->m_next;

  dango_assert(a_prev != dango::null);
  dango_assert(a_next != dango::null);
  dango_assert(a_prev->m_next == a_elem);
  dango_assert(a_next->m_prev == a_elem);

  a_next->m_prev = a_prev;
  a_prev->m_next = a_next;
  a_elem->m_prev = dango::null;
  a_elem->m_next = dango::null;
}

template
<dango::intrusive_list_constraint_spec tp_elem>
constexpr
dango::
intrusive_list<tp_elem>::
intrusive_list()noexcept:
m_head{ },
m_tail{ }
{
  clear();
}

template
<dango::intrusive_list_constraint_spec tp_elem>
constexpr void
dango::
intrusive_list<tp_elem>::
clear()noexcept
{
  m_head.m_next = &m_tail;
  m_tail.m_prev = &m_head;
}

template
<dango::intrusive_list_constraint_spec tp_elem>
constexpr auto
dango::
intrusive_list<tp_elem>::
is_empty()const noexcept->bool
{
  return m_head.m_next == &m_tail;
}

template
<dango::intrusive_list_constraint_spec tp_elem>
constexpr auto
dango::
intrusive_list<tp_elem>::
first()noexcept->elem_ptr
{
  if(is_empty())
  {
    return dango::null;
  }

  return static_cast<elem_ptr>(m_head.m_next);
}

template
<dango::intrusive_list_constraint_spec tp_elem>
constexpr auto
dango::
intrusive_list<tp_elem>::
last()noexcept->elem_ptr
{
  if(is_empty())
  {
    return dango::null;
  }

  return static_cast<elem_ptr>(m_tail.m_prev);
}

template
<dango::intrusive_list_constraint_spec tp_elem>
constexpr auto
dango::
intrusive_list<tp_elem>::
first()const noexcept->elem_const_ptr
{
  if(is_empty())
  {
    return dango::null;
  }

  return static_cast<elem_const_ptr>(m_head.m_next);
}

template
<dango::intrusive_list_constraint_spec tp_elem>
constexpr auto
dango::
intrusive_list<tp_elem>::
last()const noexcept->elem_const_ptr
{
  if(is_empty())
  {
    return dango::null;
  }

  return static_cast<elem_const_ptr>(m_tail.m_prev);
}

template
<dango::intrusive_list_constraint_spec tp_elem>
constexpr void
dango::
intrusive_list<tp_elem>::
add_first(elem_ptr const a_element)noexcept
{
  elem_type* const a_elem = a_element;

  dango_assert(a_elem != dango::null);
  dango_assert(a_elem->m_prev == dango::null);
  dango_assert(a_elem->m_next == dango::null);

  elem_type* const a_prev = &m_head;
  elem_type* const a_next = m_head.m_next;

  dango_assert(a_prev->m_next == a_next);
  dango_assert(a_next->m_prev == a_prev);

  a_elem->m_prev = a_prev;
  a_elem->m_next = a_next;
  a_next->m_prev = a_elem;
  a_prev->m_next = a_elem;
}

template
<dango::intrusive_list_constraint_spec tp_elem>
constexpr void
dango::
intrusive_list<tp_elem>::
add_last(elem_ptr const a_element)noexcept
{
  elem_type* const a_elem = a_element;

  dango_assert(a_elem != dango::null);
  dango_assert(a_elem->m_prev == dango::null);
  dango_assert(a_elem->m_next == dango::null);

  elem_type* const a_prev = m_tail.m_prev;
  elem_type* const a_next = &m_tail;

  dango_assert(a_prev->m_next == a_next);
  dango_assert(a_next->m_prev == a_prev);

  a_elem->m_prev = a_prev;
  a_elem->m_next = a_next;
  a_next->m_prev = a_elem;
  a_prev->m_next = a_elem;
}

template
<dango::intrusive_list_constraint_spec tp_elem>
constexpr auto
dango::
intrusive_list<tp_elem>::
remove_first()noexcept->elem_ptr
{
  if(is_empty())
  {
    return dango::null;
  }

  auto const a_result = first();

  remove(a_result);

  return a_result;
}

template
<dango::intrusive_list_constraint_spec tp_elem>
constexpr auto
dango::
intrusive_list<tp_elem>::
remove_last()noexcept->elem_ptr
{
  if(is_empty())
  {
    return dango::null;
  }

  auto const a_result = last();

  remove(a_result);

  return a_result;
}

#endif // DANGO_INTRUSIVE_LIST_HPP_INCLUDED

