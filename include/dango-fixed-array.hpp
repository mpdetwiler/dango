#ifndef DANGO_FIXED_ARRAY_HPP_INCLUDED
#define DANGO_FIXED_ARRAY_HPP_INCLUDED

#include "dango-allocator.hpp"
#include "dango-array.hpp"
#include "dango-auto-ptr.hpp"
#include "dango-util.hpp"

namespace
dango::detail
{
  template
  <dango::is_class tp_header, dango::is_object_exclude_array tp_elem, dango::usize tp_align = alignof(tp_elem)>
  requires
  (
    !dango::is_const_or_volatile<tp_header> && !dango::is_const_or_volatile<tp_elem> &&
    dango::is_noexcept_destructible<tp_header> && dango::is_noexcept_destructible<tp_elem> &&
    dango::is_pow_two(tp_align)
  )
  class flex_array;

  template
  <dango::is_class tp_header, dango::is_object_exclude_array tp_elem>
  requires
  (
    !dango::is_const_or_volatile<tp_header> && !dango::is_const_or_volatile<tp_elem> &&
    dango::is_noexcept_destructible<tp_header> && dango::is_noexcept_destructible<tp_elem>
  )
  class flex_array_constexpr;
}

template
<dango::is_class tp_header, dango::is_object_exclude_array tp_elem, dango::usize tp_align>
requires
(
  !dango::is_const_or_volatile<tp_header> && !dango::is_const_or_volatile<tp_elem> &&
  dango::is_noexcept_destructible<tp_header> && dango::is_noexcept_destructible<tp_elem> &&
  dango::is_pow_two(tp_align)
)
class alignas(dango::aligned_union<tp_elem, dango::aligned_storage<tp_align>>)
dango::
detail::
flex_array
final:
public tp_header
{
private:
  using super_type = tp_header;
public:
  using elem_type = tp_elem;
  using ptr_type = tp_elem*;
  using ptr_cref = ptr_type const&;
  using size_type = dango::usize;
public:
  template
  <dango::is_nohandle_allocator tp_alloc, typename... tp_args>
  requires(dango::is_brace_constructible<super_type, ptr_cref, ptr_cref, ptr_cref, tp_args...>)
  static auto
  allocate
  (size_type const a_requested, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_nohandle_allocator<tp_alloc> &&
    dango::is_noexcept_brace_constructible<super_type, ptr_cref, ptr_cref, ptr_cref, tp_args...>
  )->flex_array*
  {
    constexpr auto const c_align = alignof(flex_array);

    auto const a_size =
      dango::next_multiple(sizeof(flex_array) + a_requested * sizeof(elem_type), c_align);

    auto const a_capacity = (a_size - sizeof(flex_array)) / sizeof(elem_type);

    auto a_void = dango::make_auto_ptr<void, tp_alloc>(a_size, c_align);

    auto const a_bytes = dango_placement_new_array(a_void.get(), dango::byte, a_size);

    auto const a_begin = reinterpret_cast<ptr_type>(a_bytes + sizeof(flex_array));

    auto const a_ptr =
      dango_placement_new(a_bytes, flex_array, { a_begin, a_begin + a_requested, a_begin + a_capacity, dango::forward<tp_args>(a_args)... });

    a_void.release();

    return a_ptr;
  }

  template
  <dango::is_nohandle_allocator tp_alloc>
  static void
  deallocate
  (super_type const volatile* const a_ptr, size_type const a_requested_or_capacity)noexcept
  {
    dango_assert_nonnull(a_ptr);

    constexpr auto const c_align = alignof(flex_array);

    auto const a_size =
      dango::next_multiple(sizeof(flex_array) + a_requested_or_capacity * sizeof(elem_type), c_align);

    dango::destructor_as<flex_array>(static_cast<flex_array const volatile*>(a_ptr));

    tp_alloc::dealloc(a_ptr, a_size, c_align);
  }

  template
  <dango::is_handle_based_allocator tp_alloc, typename... tp_args>
  requires(dango::is_brace_constructible<super_type, ptr_cref, ptr_cref, ptr_cref, tp_args...>)
  static auto
  allocate
  (dango::allocator_handle_type<tp_alloc> const& a_handle, size_type const a_requested, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_handle_based_allocator<tp_alloc> &&
    dango::is_noexcept_brace_constructible<super_type, ptr_cref, ptr_cref, ptr_cref, tp_args...>
  )->flex_array*
  {
    dango_assert_nonnull(a_handle);

    constexpr auto const c_align = alignof(flex_array);

    auto const a_size =
      dango::next_multiple(sizeof(flex_array) + a_requested * sizeof(elem_type), c_align);

    auto const a_capacity = (a_size - sizeof(flex_array)) / sizeof(elem_type);

    auto a_void = dango::make_auto_ptr<void, tp_alloc>(a_handle, a_size, c_align);

    auto const a_bytes = dango_placement_new_array(a_void.get(), dango::byte, a_size);

    auto const a_begin = reinterpret_cast<ptr_type>(a_bytes + sizeof(flex_array));

    auto const a_result =
      dango_placement_new(a_bytes, flex_array, { a_begin, a_begin + a_requested, a_begin + a_capacity, dango::forward<tp_args>(a_args)... });

    a_void.release();

    return a_result;
  }

  template
  <dango::is_handle_based_allocator tp_alloc>
  static void
  deallocate
  (dango::allocator_handle_type<tp_alloc> const& a_handle, super_type const volatile* const a_ptr, size_type const a_requested_or_capacity)noexcept
  {
    dango_assert_nonnull(a_handle);
    dango_assert_nonnull(a_ptr);

    constexpr auto const c_align = alignof(flex_array);

    auto const a_size =
      dango::next_multiple(sizeof(flex_array) + a_requested_or_capacity * sizeof(elem_type), c_align);

    dango::destructor_as<flex_array>(static_cast<flex_array const volatile*>(a_ptr));

    a_handle->dealloc(a_ptr, a_size, c_align);
  }
private:
  template
  <typename... tp_args>
  requires(dango::is_brace_constructible<super_type, ptr_cref, ptr_cref, ptr_cref, tp_args...>)
  explicit
  flex_array
  (ptr_type const a_begin, ptr_type const a_requested, ptr_type const a_capacity, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<super_type, ptr_cref, ptr_cref, ptr_cref, tp_args...>):
  super_type
  {
    a_begin,
    a_requested,
    a_capacity,
    dango::forward<tp_args>(a_args)...
  }
  { }
public:
  ~flex_array()noexcept = default;
public:
  DANGO_DELETE_DEFAULT(flex_array)
  DANGO_UNMOVEABLE(flex_array)
};

template
<dango::is_class tp_header, dango::is_object_exclude_array tp_elem>
requires
(
  !dango::is_const_or_volatile<tp_header> && !dango::is_const_or_volatile<tp_elem> &&
  dango::is_noexcept_destructible<tp_header> && dango::is_noexcept_destructible<tp_elem>
)
class
dango::
detail::
flex_array_constexpr
final:
public tp_header
{
private:
  using super_type = tp_header;
public:
  using elem_type = tp_elem;
  using ptr_type = tp_elem*;
  using ptr_cref = ptr_type const&;
  using size_type = dango::usize;
public:
  template
  <typename... tp_args>
  requires(dango::is_brace_constructible<super_type, ptr_cref, ptr_cref, ptr_cref, tp_args...>)
  static constexpr auto
  allocate
  (ptr_type const a_array, size_type const a_requested, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<super_type, ptr_cref, ptr_cref, ptr_cref, tp_args...>)->super_type*
  {
    auto a_guard =
      dango::make_guard([a_array]()noexcept->void{ delete[] a_array; });

    auto const a_ptr = new flex_array_constexpr{ a_array, a_requested, dango::forward<tp_args>(a_args)... };

    a_guard.dismiss();

    return a_ptr;
  }

  static constexpr void
  deallocate
  (super_type const volatile* const a_ptr)
  {
    dango_assert_nonnull(a_ptr);

    delete static_cast<flex_array_constexpr const volatile*>(a_ptr);
  }
private:
  template
  <typename... tp_args>
  requires(dango::is_brace_constructible<super_type, ptr_cref, ptr_cref, ptr_cref, tp_args...>)
  explicit constexpr
  flex_array_constexpr
  (ptr_type const a_array, size_type const a_requested, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<super_type, ptr_cref, ptr_cref, ptr_cref, tp_args...>):
  super_type
  {
    a_array,
    dango::as_const(a_array + a_requested),
    dango::as_const(a_array + a_requested),
    dango::forward<tp_args>(a_args)...
  },
  m_array{ a_array }
  { }
public:
  constexpr
  ~flex_array_constexpr()noexcept
  {
    delete[] m_array;
  }
private:
  ptr_type const m_array;
public:
  DANGO_DELETE_DEFAULT(flex_array_constexpr)
  DANGO_UNMOVEABLE(flex_array_constexpr)
};

namespace
dango::detail
{
  template
  <typename tp_elem, typename tp_allocator DANGO_GCC_BUG_81043_WORKAROUND>
  class fixed_array_header;

  template
  <typename tp_elem, dango::is_nohandle_allocator tp_allocator>
  class fixed_array_header<tp_elem, tp_allocator DANGO_GCC_BUG_81043_WORKAROUND_ID(0, tp_elem, tp_allocator)>;

  template
  <typename tp_elem, dango::is_handle_based_allocator tp_allocator>
  class fixed_array_header<tp_elem, tp_allocator DANGO_GCC_BUG_81043_WORKAROUND_ID(1, tp_elem, tp_allocator)>;
}

template
<typename tp_elem, dango::is_nohandle_allocator tp_allocator>
class
dango::
detail::
fixed_array_header<tp_elem, tp_allocator DANGO_GCC_BUG_81043_WORKAROUND_ID(0, tp_elem, tp_allocator)>
{
private:
  using elem_type = tp_elem;
  using ptr_type = tp_elem*;
public:
  explicit constexpr
  fixed_array_header
  (
    ptr_type const a_begin,
    ptr_type const a_end,
    ptr_type const
  )
  noexcept:
  m_begin{ a_begin },
  m_end{ a_end }
  { }

  constexpr ~fixed_array_header()noexcept = default;
public:
  constexpr void launder()noexcept{ m_begin = dango::launder(m_begin); }
  constexpr auto begin()const noexcept->ptr_type{ return m_begin; }
  constexpr auto end()const noexcept->ptr_type{ return m_end; }
private:
  ptr_type m_begin;
  ptr_type m_end;
public:
  DANGO_DELETE_DEFAULT(fixed_array_header)
  DANGO_UNMOVEABLE(fixed_array_header)
};

template
<typename tp_elem, dango::is_handle_based_allocator tp_allocator>
class
dango::
detail::
fixed_array_header<tp_elem, tp_allocator DANGO_GCC_BUG_81043_WORKAROUND_ID(1, tp_elem, tp_allocator)>
{
private:
  using elem_type = tp_elem;
  using ptr_type = tp_elem*;
  using handle_type = typename tp_allocator::handle_type;
public:
  explicit constexpr
  fixed_array_header
  (
    ptr_type const a_begin,
    ptr_type const a_end,
    ptr_type const,
    handle_type&& a_alloc_ptr
  )
  noexcept:
  m_begin{ a_begin },
  m_end{ a_end },
  m_alloc_ptr{ dango::move(a_alloc_ptr) }
  { }

  constexpr ~fixed_array_header()noexcept = default;
public:
  constexpr void launder()noexcept{ m_begin = dango::launder(m_begin); }
  constexpr auto begin()const noexcept->ptr_type{ return m_begin; }
  constexpr auto end()const noexcept->ptr_type{ return m_end; }

  auto
  allocator_handle()noexcept->handle_type&
  {
    return m_alloc_ptr;
  }
private:
  ptr_type m_begin;
  ptr_type m_end;
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
  using header = dango::detail::fixed_array_header<elem_type_intern, allocator_type>;
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

        return array_type_constexpr::allocate(a_array, size_type(0));
      }
      else
      {
        return array_type::template allocate<allocator_type>(size_type(0));
      }
    }
    else
    {
      if(dango::in_constexpr_context())
      {
        auto const a_array = new elem_type_intern[size_type(0)];

        return array_type_constexpr::allocate(a_array, size_type(0), allocator_handle_type{ dango::null });
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
public:
  constexpr auto dango_operator_is_null()const noexcept->bool{ return dango::is_null(m_header); }

public:
  constexpr auto size()const noexcept->size_type{ dango_assert_nonnull(*this); return size_type(m_header->end() - m_header->begin()); }
  constexpr auto is_empty()const noexcept->bool{ dango_assert_nonnull(*this); return m_header->begin() == m_header->end(); }
private:
  header_ptr m_header;
};

#endif
