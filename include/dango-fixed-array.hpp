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

  explicit constexpr
  fixed_array_header
  ()noexcept:
  fixed_array_header{ handle_type{ dango::null } }
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
  DANGO_UNMOVEABLE(fixed_array_header)
};

namespace
dango
{
  template
  <typename tp_elem>
  concept fixed_array_constraint_spec =
    dango::is_object_exclude_array<tp_elem> &&
    !dango::is_bad_struct_of_array<tp_elem> &&
    !dango::is_bad_stable_adaptor<tp_elem> &&
    !dango::is_bad_ref_adaptor<tp_elem> &&
    !dango::is_bad_stable_ref_adaptor<tp_elem>;

  template
  <dango::fixed_array_constraint_spec tp_elem, dango::is_allocator tp_allocator = dango::polymorphic_allocator<>, dango::usize tp_align = dango::usize(16)>
  requires(dango::is_pow_two(tp_align))
  class fixed_array;

  template
  <typename tp_elem>
  concept fixed_array_constraint_spec_soa =
    dango::fixed_array_constraint_spec<tp_elem> &&
    !dango::is_struct_of_array<tp_elem>;

  template
  <dango::fixed_array_constraint_spec_soa tp_first, dango::fixed_array_constraint_spec_soa... tp_next, dango::is_allocator tp_allocator, dango::usize tp_align>
  class fixed_array<dango::struct_of_array<tp_first, tp_next...>, tp_allocator, tp_align>;
}

template
<dango::fixed_array_constraint_spec tp_elem, dango::is_allocator tp_allocator, dango::usize tp_align>
requires(dango::is_pow_two(tp_align))
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
  using header = dango::detail::fixed_array_header<allocator_type>;
  using header_ptr = header*;
  using elem_type_adaptor = dango::detail::container_elem_type<elem_type, tp_align>;
  using array_type = dango::detail::flex_array<header, tp_align, dango::struct_of_array<elem_type_adaptor>>;
  using array_type_constexpr = dango::detail::flex_array_constexpr<header, tp_align, dango::struct_of_array<elem_type_adaptor>>;
  using array_allocator = typename array_type::template allocator<allocator_type>;
  using array_allocator_constexpr = typename array_type_constexpr::allocator;
  using dispatcher_type = dango::detail::flex_array_dispatcher<header, array_type, array_type_constexpr>;
private:
  static constexpr auto
  default_construct_help()
  noexcept(dango::is_noexcept_allocator<allocator_type>)->header_ptr
  {
    if(dango::in_constexpr_context())
    {
      return array_allocator_constexpr::allocate_n(size_type(0), size_type(0));
    }
    else
    {
      if constexpr(dango::is_void<allocator_handle_type>)
      {
        return array_allocator::allocate_n(size_type(0), size_type(0));
      }
      else
      {
        auto a_alloc_ptr = dango::current_allocator_or_default<allocator_type>();

        return array_allocator::allocate_n(a_alloc_ptr, size_type(0), size_type(0), dango::move(a_alloc_ptr));
      }
    }
  }
public:
  constexpr
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
    auto const a_header = m_header;

    if(!a_header)
    {
      return;
    }

    auto const a_begin = dispatcher_type::begin(a_header);
    auto const a_end = dispatcher_type::end(a_header);

    auto const a_runtime =
    [a_header, a_begin, a_end]()noexcept->void
    {
      if constexpr(dango::is_void<allocator_handle_type>)
      {
        for(auto a_current = a_begin; a_current != a_end; ++a_current)
        {
          elem_type_adaptor::template destroy<allocator_type>(*a_current);
        }

        array_allocator::deallocate(a_header);
      }
      else
      {
        auto const a_alloc_ptr = dango::move(a_header->allocator_handle());

        auto const a_guard = allocator_type::lock(a_alloc_ptr);

        for(auto a_current = a_begin; a_current != a_end; ++a_current)
        {
          elem_type_adaptor::template destroy<allocator_type>(a_guard, *a_current);
        }

        array_allocator::deallocate(a_alloc_ptr, a_header);
      }
    };

    if(dango::in_constexpr_context())
    {
      for(auto a_current = a_begin; a_current != a_end; ++a_current)
      {
        elem_type_adaptor::destroy_constexpr(*a_current);
      }

      array_allocator_constexpr::deallocate(a_header);
    }
    else
    {
      a_runtime();
    }
  }

  DANGO_DEFINE_NULL_SWAP_ASSIGN(fixed_array, constexpr, true)

  constexpr auto operator = (fixed_array const&)& noexcept = delete;

  DANGO_DEFINE_MOVE_SWAP_ASSIGN(fixed_array, constexpr, true)
public:
  constexpr auto dango_operator_is_null()const noexcept->bool{ return dango::is_null(m_header); }
  constexpr void dango_operator_swap(fixed_array& a_array)& noexcept{ dango::swap(m_header, a_array.m_header); }
public:
  constexpr auto size()const noexcept->size_type{ return dispatcher_type::size(m_header); }
  constexpr auto is_empty()const noexcept->bool{ return dispatcher_type::is_empty(m_header); }
private:
  header_ptr m_header;
};

#endif
