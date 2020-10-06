#ifndef DANGO_FIXED_ARRAY_HPP_INCLUDED
#define DANGO_FIXED_ARRAY_HPP_INCLUDED

#include "dango-array.hpp"
#include "dango-container.hpp"

namespace
dango::detail
{
  template
  <typename tp_allocator DANGO_GCC_BUG_81043_WORKAROUND>
  class fixed_array_header;

  template
  <dango::is_nohandle_allocator tp_allocator>
  class fixed_array_header<tp_allocator DANGO_GCC_BUG_81043_WORKAROUND_ID(0, tp_allocator)>;

  template
  <dango::is_handle_based_allocator tp_allocator>
  class fixed_array_header<tp_allocator DANGO_GCC_BUG_81043_WORKAROUND_ID(1, tp_allocator)>;
}

template
<dango::is_nohandle_allocator tp_allocator>
class
dango::
detail::
fixed_array_header<tp_allocator DANGO_GCC_BUG_81043_WORKAROUND_ID(0, tp_allocator)>
{
public:
  explicit constexpr fixed_array_header()noexcept = default;
  constexpr ~fixed_array_header()noexcept = default;
public:
  DANGO_UNMOVEABLE(fixed_array_header)
};

template
<dango::is_handle_based_allocator tp_allocator>
class
dango::
detail::
fixed_array_header<tp_allocator DANGO_GCC_BUG_81043_WORKAROUND_ID(1, tp_allocator)>
{
public:
  using handle_type = typename tp_allocator::handle_type;
public:
  explicit constexpr fixed_array_header(handle_type&& a_alloc_ptr)noexcept:
  m_alloc_ptr{ dango::move(a_alloc_ptr) }
  { }
  constexpr ~fixed_array_header()noexcept = default;
public:
  auto
  allocator_handle()noexcept->handle_type&
  {
    return m_alloc_ptr;
  }
private:
  handle_type m_alloc_ptr;
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
  using header = dango::detail::fixed_array_header<allocator_type>;
  using header_ptr = header*;
  using array_type = dango::detail::flex_array<header, dango::struct_of_array<elem_type_intern>, dango::usize(16)>;
  using array_type_constexpr = dango::detail::flex_array_constexpr<header, dango::struct_of_array<elem_type_intern>>;
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

        return array_type_constexpr::allocate(a_array, size_type(0), size_type(0));
      }
      else
      {
        return array_type::template allocate<allocator_type>(size_type(0), size_type(0));
      }
    }
    else
    {
      if(dango::in_constexpr_context())
      {
        auto const a_array = new elem_type_intern[size_type(0)];

        return array_type_constexpr::allocate(a_array, size_type(0), size_type(0), dango::null);
      }
      else
      {
        auto a_alloc_ptr = dango::current_allocator_or_default<allocator_type>();

        return array_type::template allocate<allocator_type>(a_alloc_ptr, size_type(0), size_type(0), dango::move(a_alloc_ptr));
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
      dango::array_destroy(static_cast<array_type*>(m_header)->begin(), size());

      if constexpr(dango::is_void<allocator_handle_type>)
      {
        array_type::template deallocate<allocator_type>(m_header);
      }
      else
      {
        auto const a_alloc_ptr = dango::move(m_header->allocator_handle());

        array_type::template deallocate<allocator_type>(a_alloc_ptr, m_header);
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
  constexpr auto
  size()const noexcept->size_type
  {
    dango_assert_nonnull(*this);

    if(dango::in_constexpr_context())
    {
      return static_cast<array_type_constexpr*>(m_header)->size();
    }
    else
    {
      return static_cast<array_type*>(m_header)->size();
    }
  }

  constexpr auto
  is_empty()const noexcept->bool
  {
    dango_assert_nonnull(*this);

    if(dango::in_constexpr_context())
    {
      return static_cast<array_type_constexpr*>(m_header)->is_empty();
    }
    else
    {
      return static_cast<array_type*>(m_header)->is_empty();
    }
  }
private:
  header_ptr m_header;
};

#endif
