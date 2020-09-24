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
class alignas(dango::max(alignof(tp_elem), tp_align))
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
  using size_type = dango::usize;
public:
  template
  <dango::is_nohandle_allocator tp_alloc, typename... tp_args>
  requires(dango::is_brace_constructible<super_type, ptr_type const&, ptr_type const&, tp_args...>)
  static auto
  allocate
  (size_type const a_count, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_nohandle_allocator<tp_alloc> &&
    dango::is_noexcept_brace_constructible<super_type, ptr_type const&, ptr_type const&, tp_args...>
  )->flex_array*
  {
    constexpr auto const c_align = alignof(flex_array);

    auto const a_size =
      dango::next_multiple(sizeof(flex_array) + a_count * sizeof(elem_type), c_align);

    auto const a_capacity = (a_size - sizeof(flex_array)) / sizeof(elem_type);

    auto const a_void = dango::make_auto_ptr<void, tp_alloc>(a_size, c_align);

    auto const a_ptr =
      dango_placement_new(a_void.get(), flex_array, { a_capacity, dango::forward<tp_args>(a_args)... });

    a_void.release();

    return a_ptr;
  }

  template
  <dango::is_nohandle_allocator tp_alloc>
  static void
  deallocate
  (flex_array const volatile* const a_ptr, size_type const a_count)noexcept
  {
    dango_assert_nonnull(a_ptr);

    constexpr auto const c_align = alignof(flex_array);

    auto const a_size =
      dango::next_multiple(sizeof(flex_array) + a_count * sizeof(elem_type), c_align);

    dango::destructor_as<flex_array>(a_ptr);

    tp_alloc::dealloc(a_ptr, a_size, c_align);
  }

  template
  <dango::is_handle_based_allocator tp_alloc, typename... tp_args>
  requires(dango::is_brace_constructible<super_type, ptr_type const&, ptr_type const&, tp_args...>)
  static auto
  allocate
  (dango::allocator_handle_type<tp_alloc> const& a_handle, size_type const a_count, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_handle_based_allocator<tp_alloc> &&
    dango::is_noexcept_brace_constructible<super_type, ptr_type const&, ptr_type const&, tp_args...>
  )->flex_array*
  {
    dango_assert_nonnull(a_handle);

    constexpr auto const c_align = alignof(flex_array);

    auto const a_size =
      dango::next_multiple(sizeof(flex_array) + a_count * sizeof(elem_type), c_align);

    auto const a_capacity = (a_size - sizeof(flex_array)) / sizeof(elem_type);

    auto const a_void = dango::make_auto_ptr<void, tp_alloc>(a_handle, a_size, c_align);

    auto const a_result =
      dango_placement_new(a_void.get(), flex_array, { a_capacity, dango::forward<tp_args>(a_args)... });

    a_void.release();

    return a_result;
  }

  template
  <dango::is_handle_based_allocator tp_alloc>
  static void
  deallocate
  (dango::allocator_handle_type<tp_alloc> const& a_handle, flex_array const volatile* const a_ptr, size_type const a_count)noexcept
  {
    dango_assert_nonnull(a_handle);
    dango_assert_nonnull(a_ptr);

    constexpr auto const c_align = alignof(flex_array);

    auto const a_size =
      dango::next_multiple(sizeof(flex_array) + a_count * sizeof(elem_type), c_align);

    dango::destructor_as<flex_array>(a_ptr);

    a_handle->dealloc(a_ptr, a_size, c_align);
  }
private:
  static auto
  get_begin
  (flex_array* const a_this)noexcept->ptr_type
  {
    void* const a_void = a_this + size_type(1);

    return static_cast<ptr_type>(a_void);
  }

  static auto
  get_end
  (flex_array* const a_this, size_type const a_capacity)noexcept->ptr_type
  {
    return get_begin(a_this) + a_capacity;
  }
private:
  template
  <typename... tp_args>
  requires(dango::is_brace_constructible<super_type, ptr_type const&, ptr_type const&, tp_args...>)
  explicit
  flex_array
  (size_type const a_capacity, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<super_type, ptr_type const&, ptr_type const&, tp_args...>):
  super_type{ dango::as_const(get_begin(this)), dango::as_const(get_end(this, a_capacity)), dango::forward<tp_args>(a_args)... }
  { }

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
  using size_type = dango::usize;
public:
  template
  <typename... tp_args>
  requires
  (
    dango::is_default_constructible<tp_elem> &&
    dango::is_brace_constructible<super_type, ptr_type const&, ptr_type const&, tp_args...>
  )
  static constexpr auto
  allocate
  (size_type const a_count, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_default_constructible<tp_elem> &&
    dango::is_noexcept_brace_constructible<super_type, ptr_type const&, ptr_type const&, tp_args...>
  )->flex_array_constexpr*
  {
    auto const a_array = new tp_elem[a_count];

    auto a_guard =
      dango::make_guard([a_array]()noexcept->void{ delete[] a_array; });

    auto const a_ptr = new flex_array_constexpr{ a_count, a_array, dango::forward<tp_args>(a_args)... };

    a_guard.dismiss();

    return a_ptr;
  }

  static constexpr void
  deallocate
  (flex_array_constexpr const volatile* const a_ptr)
  {
    dango_assert_nonnull(a_ptr);

    delete a_ptr;
  }
private:
  template
  <typename... tp_args>
  requires(dango::is_brace_constructible<super_type, ptr_type const&, ptr_type const&, tp_args...>)
  explicit constexpr
  flex_array_constexpr
  (size_type const a_capacity, ptr_type const a_array, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<super_type, ptr_type const&, ptr_type const&, tp_args...>):
  super_type{ a_array, a_array + a_capacity, dango::forward<tp_args>(a_args)... },
  m_array{ a_array }
  { }

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
dango
{
  template
  <dango::is_object_exclude_array tp_elem, dango::is_allocator tp_alloc = dango::polymorphic_allocator<>>
  class fixed_array;


}

template
<dango::is_object_exclude_array tp_elem, dango::is_allocator tp_alloc>
class
dango::
fixed_array
final
{

public:
  DANGO_DELETE_DEFAULT(fixed_array)
};

#endif
