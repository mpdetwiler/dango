#ifndef DANGO_FIXED_ARRAY_HPP_INCLUDED
#define DANGO_FIXED_ARRAY_HPP_INCLUDED

#include "dango-allocator.hpp"
#include "dango-util.hpp"

namespace
dango::detail
{
  template
  <dango::is_class tp_header, dango::is_object_exclude_array tp_elem, dango::usize tp_align = alignof(tp_elem)>
  requires
  (
    !dango::is_const_or_volatile<tp_header> &&
    !dango::is_const_or_volatile<tp_elem> &&
    dango::is_pow_two(tp_align)
  )
  class flex_array;
}

template
<dango::is_class tp_header, dango::is_object_exclude_array tp_elem, dango::usize tp_align>
requires
(
  !dango::is_const_or_volatile<tp_header> &&
  !dango::is_const_or_volatile<tp_elem> &&
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

    auto const a_new_count = (a_size - sizeof(flex_array)) / sizeof(elem_type);

    auto const a_void = tp_alloc::alloc(a_size, c_align);

    auto a_guard =
      dango::make_guard([a_size, a_void]()noexcept->void{ tp_alloc::dealloc(a_void, a_size, c_align); });

    auto const a_result =
      dango_placement_new(a_void, flex_array, { a_new_count, dango::forward<tp_args>(a_args)... });

    a_guard.dismiss();

    return a_result;
  }

  template
  <dango::is_handle_based_allocator tp_alloc, dango::is_same_ignore_cv<dango::allocator_handle_type<tp_alloc>> tp_handle, typename... tp_args>
  requires(dango::is_brace_constructible<super_type, ptr_type const&, ptr_type const&, tp_args...>)
  static auto
  allocate
  (size_type const a_count, tp_handle const& a_handle, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_handle_based_allocator<tp_alloc> &&
    dango::is_noexcept_brace_constructible<super_type, ptr_type const&, ptr_type const&, tp_args...>
  )->flex_array*
  {
    constexpr auto const c_align = alignof(flex_array);

    auto const a_size =
      dango::next_multiple(sizeof(flex_array) + a_count * sizeof(elem_type), c_align);

    auto const a_new_count = (a_size - sizeof(flex_array)) / sizeof(elem_type);

    auto const a_void = a_handle->alloc(a_size, c_align);

    auto a_guard =
      dango::make_guard([&a_handle, a_size, a_void]()noexcept->void{ a_handle->dealloc(a_void, a_size, c_align); });

    auto const a_result =
      dango_placement_new(a_void, flex_array, { a_new_count, dango::forward<tp_args>(a_args)... });

    a_guard.dismiss();

    return a_result;
  }
private:
  static auto
  get_begin
  (flex_array* const a_this)noexcept->elem_type*
  {
    void* const a_void = a_this + size_type(1);

    return static_cast<elem_type*>(a_void);
  }

  static auto
  get_end
  (flex_array* const a_this, size_type a_count)noexcept->elem_type*
  {
    return get_begin(a_this) + a_count;
  }
private:
  template
  <typename... tp_args>
  requires(dango::is_brace_constructible<super_type, ptr_type const&, ptr_type const&, tp_args...>)
  explicit
  flex_array
  (size_type a_count, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<super_type, ptr_type const&, ptr_type const&, tp_args...>):
  super_type{ dango::as_const(get_begin(this)), dango::as_const(get_end(this, a_count)), dango::forward<tp_args>(a_args)... }
  { }

  ~flex_array()noexcept = default;
public:
  DANGO_DELETE_DEFAULT(flex_array)
  DANGO_UNMOVEABLE(flex_array)
};

#endif
