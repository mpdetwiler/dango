#ifndef __DANGO_INTRUSIVE_LIST_HPP__
#define __DANGO_INTRUSIVE_LIST_HPP__ 1

#include "dango-traits.hpp"
#include "dango-assert.hpp"

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
