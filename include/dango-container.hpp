#ifndef DANGO_CONTAINER_HPP_INCLUDED
#define DANGO_CONTAINER_HPP_INCLUDED

#include "dango-allocator.hpp"
#include "dango-auto-ptr.hpp"
#include "dango-util.hpp"

namespace
dango
{
  template
  <dango::is_object_exclude_array tp_first, dango::is_object_exclude_array... tp_next>
  struct
  struct_of_array
  final
  {
    DANGO_TAG_TYPE(struct_of_array)
  };
}

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

#endif
