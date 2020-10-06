#ifndef DANGO_FIXED_ARRAY_HPP_INCLUDED
#define DANGO_FIXED_ARRAY_HPP_INCLUDED

#include "dango-array.hpp"
#include "dango-container.hpp"

namespace
dango::detail
{
  template
  <typename tp_allocator, typename tp_elem DANGO_GCC_BUG_81043_WORKAROUND>
  class fixed_array_header_base;

  template
  <dango::is_nohandle_allocator tp_allocator, typename tp_elem>
  class fixed_array_header_base<tp_allocator, tp_elem DANGO_GCC_BUG_81043_WORKAROUND_ID(0, tp_elem, tp_allocator)>;

  template
  <dango::is_handle_based_allocator tp_allocator, typename tp_elem>
  class fixed_array_header_base<tp_allocator, tp_elem DANGO_GCC_BUG_81043_WORKAROUND_ID(1, tp_elem, tp_allocator)>;

  template
  <typename tp_allocator, typename tp_elem>
  class fixed_array_header;

  template
  <dango::is_allocator tp_allocator, typename tp_first, typename... tp_next>
  class fixed_array_header<tp_allocator, dango::struct_of_array<tp_first, tp_next...>>;
}

template
<dango::is_nohandle_allocator tp_allocator, typename tp_elem>
class
dango::
detail::
fixed_array_header_base<tp_allocator, tp_elem DANGO_GCC_BUG_81043_WORKAROUND_ID(0, tp_elem, tp_allocator)>
{
public:
  using elem_type = tp_elem;
  using ptr_type = elem_type*;
  using size_type = dango::usize;
public:
  explicit constexpr
  fixed_array_header_base
  (
    ptr_type const a_begin,
    ptr_type const a_end,
    dango::null_tag const
  )
  noexcept:
  m_begin{ a_begin },
  m_end{ a_end }
  { }

  constexpr ~fixed_array_header_base()noexcept = default;
public:
  constexpr auto begin()const noexcept->ptr_type{ return m_begin; }
  constexpr auto end()const noexcept->ptr_type{ return m_end; }
  constexpr auto size()const noexcept->size_type{ return size_type(end() - begin()); }
  constexpr auto is_empty()const noexcept->bool{ return begin() == end(); }
private:
  ptr_type const m_begin;
  ptr_type const m_end;
public:
  DANGO_DELETE_DEFAULT(fixed_array_header_base)
  DANGO_UNMOVEABLE(fixed_array_header_base)
};

template
<dango::is_handle_based_allocator tp_allocator, typename tp_elem>
class
dango::
detail::
fixed_array_header_base<tp_allocator, tp_elem DANGO_GCC_BUG_81043_WORKAROUND_ID(1, tp_elem, tp_allocator)>
{
public:
  using elem_type = tp_elem;
  using ptr_type = elem_type*;
  using handle_type = typename tp_allocator::handle_type;
  using size_type = dango::usize;
public:
  explicit constexpr
  fixed_array_header_base
  (
    ptr_type const a_begin,
    ptr_type const a_end,
    handle_type&& a_alloc_ptr
  )
  noexcept:
  m_begin{ a_begin },
  m_end{ a_end },
  m_alloc_ptr{ dango::move(a_alloc_ptr) }
  { }

  constexpr ~fixed_array_header_base()noexcept = default;
public:
  constexpr auto begin()const noexcept->ptr_type{ return m_begin; }
  constexpr auto end()const noexcept->ptr_type{ return m_end; }
  constexpr auto size()const noexcept->size_type{ return size_type(end() - begin()); }
  constexpr auto is_empty()const noexcept->bool{ return begin() == end(); }

  auto
  allocator_handle()noexcept->handle_type&
  {
    return m_alloc_ptr;
  }
private:
  ptr_type const m_begin;
  ptr_type const m_end;
  handle_type m_alloc_ptr;
public:
  DANGO_DELETE_DEFAULT(fixed_array_header_base)
  DANGO_UNMOVEABLE(fixed_array_header_base)
};

template
<dango::is_allocator tp_allocator, typename tp_first, typename... tp_next>
class
dango::
detail::
fixed_array_header<tp_allocator, dango::struct_of_array<tp_first, tp_next...>>:
public dango::detail::fixed_array_header_base<tp_allocator, tp_first>
{
public:
  using super_type = dango::detail::fixed_array_header_base<tp_allocator, tp_first>;
  using ptr_type = typename super_type::ptr_type;
  using size_type = typename super_type::size_type;
  using tuple_type = dango::tuple<tp_next*...>;
public:
  using super_type::begin;
  using super_type::end;
  using super_type::size;
public:
  explicit constexpr
  fixed_array_header
  (
    ptr_type const a_begin,
    ptr_type const a_end,
    ptr_type const,
    dango::allocator_handle_type<tp_allocator, dango::null_tag const&>&& a_alloc_ptr
  )
  noexcept
  requires(sizeof...(tp_next) == size_type(0)):
  super_type{ a_begin, a_end, dango::move(a_alloc_ptr) },
  m_next{ }
  { }

  explicit constexpr
  fixed_array_header
  (
    ptr_type const a_begin,
    ptr_type const a_end,
    ptr_type const,
    dango::allocator_handle_type<tp_allocator, dango::null_tag const&>&& a_alloc_ptr,
    tuple_type const& a_next
  )
  noexcept
  requires(sizeof...(tp_next) != size_type(0)):
  super_type{ a_begin, a_end, dango::move(a_alloc_ptr) },
  m_next{ a_next }
  { }

  constexpr ~fixed_array_header()noexcept = default;
public:
  template
  <size_type tp_index>
  requires(tp_index < (sizeof...(tp_next) + size_type(1)))
  constexpr auto
  begin_at()const noexcept->auto
  {
    if constexpr(tp_index == size_type(0))
    {
      return begin();
    }
    else
    {
      return m_next.template at<tp_index - size_type(1)>();
    }
  }

  template
  <size_type tp_index>
  requires(tp_index < (sizeof...(tp_next) + size_type(1)))
  constexpr auto
  end_at()const noexcept->auto
  {
    if constexpr(tp_index == size_type(0))
    {
      return end();
    }
    else
    {
      return m_next.template at<tp_index - size_type(1)>() + size();
    }
  }
private:
  tuple_type const m_next [[no_unique_address]];
public:
  DANGO_DELETE_DEFAULT(fixed_array_header)
  DANGO_UNMOVEABLE(fixed_array_header)
};

namespace
dango
{
  template
  <dango::is_object_exclude_array tp_elem, dango::is_allocator tp_allocator = dango::polymorphic_allocator<>>
  class fixed_array;
}

template
<dango::is_object_exclude_array tp_elem, dango::is_allocator tp_allocator>
class
dango::
fixed_array
final
{
public:
  using elem_type = tp_elem;
  using ptr_type = elem_type*;
  using cptr_type = elem_type const*;
  using ref_type = elem_type&;
  using cref_type = elem_type const&;
  using allocator_type = tp_allocator;
  using allocator_handle_type = dango::allocator_handle_type<allocator_type, void>;
  using size_type = dango::usize;
private:
  using elem_type_intern = dango::remove_cv<elem_type>;
  using ptr_type_intern = elem_type_intern*;
  using header = dango::detail::fixed_array_header<allocator_type, dango::struct_of_array<elem_type_intern>>;
  using header_ptr = header*;
  using array_type = dango::detail::flex_array<header, elem_type_intern, alignof(dango::cache_align_type)>;
  using array_type_constexpr = dango::detail::flex_array_constexpr<header, elem_type_intern>;
private:
  static constexpr auto
  default_construct_help()
  noexcept(dango::is_noexcept_allocator<allocator_type>)->header_ptr
  {
    if constexpr(dango::is_void<allocator_handle_type>)
    {
      if(dango::in_constexpr_context())
      {
        auto const a_array = new elem_type_intern[size_type(0)];

        return array_type_constexpr::allocate(a_array, size_type(0), dango::null);
      }
      else
      {
        return array_type::template allocate<allocator_type>(size_type(0), dango::null);
      }
    }
    else
    {
      if(dango::in_constexpr_context())
      {
        auto const a_array = new elem_type_intern[size_type(0)];

        return array_type_constexpr::allocate(a_array, size_type(0), dango::null);
      }
      else
      {
        auto a_alloc_ptr = dango::current_allocator_or_default<allocator_type>();

        return array_type::template allocate<allocator_type>(a_alloc_ptr, size_type(0), dango::move(a_alloc_ptr));
      }
    }
  }
public:
  explicit constexpr
  fixed_array()
  noexcept(dango::is_noexcept_allocator<allocator_type>)
  requires(dango::is_void<allocator_handle_type> || dango::allocator_has_default_handle<allocator_type>):
  m_header{ default_construct_help() }
  { }

  constexpr
  fixed_array
  (dango::null_tag const)noexcept:
  m_header{ dango::null }
  { }

  constexpr fixed_array(fixed_array const&)noexcept = delete;

  constexpr
  fixed_array
  (fixed_array&& a_array)noexcept:
  m_header{ dango::exchange(a_array.m_header, dango::null) }
  { }

  constexpr
  ~fixed_array()noexcept
  {
    if(!m_header)
    {
      return;
    }

    if(dango::in_constexpr_context())
    {
      array_type_constexpr::deallocate(m_header);
    }
    else
    {
      dango::array_destroy(m_header->begin(), size());

      if constexpr(dango::is_void<allocator_handle_type>)
      {
        array_type::template deallocate<allocator_type>(m_header, size());
      }
      else
      {
        auto const a_alloc_ptr = dango::move(m_header->allocator_handle());

        array_type::template deallocate<allocator_type>(a_alloc_ptr, m_header, size());
      }
    }
  }

  DANGO_DEFINE_NULL_SWAP_ASSIGN(fixed_array, constexpr, true)

  constexpr auto operator = (fixed_array const&)& noexcept = delete;

  DANGO_DEFINE_MOVE_SWAP_ASSIGN(fixed_array, constexpr, true)
public:
  constexpr auto dango_operator_is_null()const noexcept->bool{ return dango::is_null(m_header); }

  constexpr void dango_operator_swap(fixed_array& a_array)& noexcept{ dango::swap(m_header, a_array.m_header); }
public:
  constexpr auto size()const noexcept->size_type{ dango_assert_nonnull(*this); return m_header->size(); }
  constexpr auto is_empty()const noexcept->bool{ dango_assert_nonnull(*this); return m_header->is_empty(); }
private:
  header_ptr m_header;
};

#endif
