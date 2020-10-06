#ifndef DANGO_CONTAINER_HPP_INCLUDED
#define DANGO_CONTAINER_HPP_INCLUDED

#include "dango-allocator.hpp"
#include "dango-auto-ptr.hpp"
#include "dango-tuple.hpp"
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
  <typename tp_header, typename tp_elem, dango::usize tp_align>
  class flex_array;

  template
  <dango::is_class tp_header, typename tp_first, typename... tp_next, dango::usize tp_align>
  requires
  (
    !dango::is_const_or_volatile<tp_header> &&
    (!dango::is_const_or_volatile<tp_first> && ... && !dango::is_const_or_volatile<tp_next>) &&
    dango::is_pow_two(tp_align)
  )
  class flex_array<tp_header, dango::struct_of_array<tp_first, tp_next...>, tp_align>;

  template
  <typename tp_header, typename tp_elem>
  class flex_array_constexpr;

  template
  <dango::is_class tp_header, typename tp_first, typename... tp_next>
  requires
  (
    !dango::is_const_or_volatile<tp_header> &&
    (!dango::is_const_or_volatile<tp_first> && ... && !dango::is_const_or_volatile<tp_next>)
  )
  class flex_array_constexpr<tp_header, dango::struct_of_array<tp_first, tp_next...>>;
}

template
<dango::is_class tp_header, typename tp_first, typename... tp_next, dango::usize tp_align>
requires
(
  !dango::is_const_or_volatile<tp_header> &&
  (!dango::is_const_or_volatile<tp_first> && ... && !dango::is_const_or_volatile<tp_next>) &&
  dango::is_pow_two(tp_align)
)
class alignas(dango::aligned_union<dango::aligned_storage<tp_align>, tp_first, tp_next...>)
dango::
detail::
flex_array<tp_header, dango::struct_of_array<tp_first, tp_next...>, tp_align>
final:
public tp_header
{
private:
  using super_type = tp_header;
public:
  using elem_type = tp_first;
  using ptr_type = elem_type*;
  using tuple_type = dango::tuple<tp_next*...>;
  using size_type = dango::usize;
private:
  template
  <bool tp_noexcept, typename tp_func, typename... tp_args>
  static auto
  alloc_impl
  (tp_func&& a_func, size_type const a_size_init, size_type const a_requested, tp_args&&... a_args)noexcept(tp_noexcept)->super_type*
  {
    dango_assert(a_size_init <= a_requested);

    constexpr auto const c_align = alignof(flex_array);

    auto const a_size =
      dango::next_multiple(sizeof(flex_array) + a_requested * sizeof(elem_type), c_align);

    auto const a_capacity = (a_size - sizeof(flex_array)) / sizeof(elem_type);

    dango_assert(a_capacity >= a_requested);

    auto a_void = dango::forward<tp_func>(a_func)(a_size, c_align);

    auto const a_bytes = dango_placement_new_array(a_void.get(), dango::byte, a_size);

    auto const a_array = reinterpret_cast<ptr_type>(a_bytes + sizeof(flex_array));

    auto a_void_tuple =
      dango::make_tuple(dango::forward<tp_func>(a_func)(a_capacity * sizeof(tp_next), dango::max(tp_align, alignof(tp_next)))...);

    auto const a_array_tuple =
      a_void_tuple->*[](auto&... a_arg)noexcept->auto
      {
        return dango::make_tuple(reinterpret_cast<tp_next*>(dango_placement_new_array(a_arg.get(), dango::byte, a_arg.size()))...);
      };

    auto const a_ptr =
      dango_placement_new(a_bytes, flex_array, { a_array, a_size_init, a_capacity, a_array_tuple, dango::forward<tp_args>(a_args)... });

    a_void_tuple->*[](auto&... a_arg)noexcept->void{ (void(a_arg.release()) , ... ); };

    a_void.release();

    return a_ptr;
  }

  template
  <typename tp_func>
  static void
  dealloc_impl
  (tp_func&& a_func, super_type const volatile* const a_super)noexcept
  {
    dango_assert_nonnull(a_super);

    constexpr auto const c_align = alignof(flex_array);

    auto const a_array = static_cast<flex_array const*>(const_cast<super_type const*>(a_super));

    auto const a_capacity = a_array->capacity();

    auto const a_size =
      dango::next_multiple(sizeof(flex_array) + a_capacity * sizeof(elem_type), c_align);

    auto const& a_tuple = a_array->m_next;

    a_tuple->*[&a_func, a_capacity](auto const... a_arg)noexcept->void
    {
      (void(dango::forward<tp_func>(a_func)(a_arg, a_capacity * sizeof(tp_next), dango::max(tp_align, alignof(tp_next)))) , ... );
    };

    dango::destructor(a_array);

    dango::forward<tp_func>(a_func)(a_array, a_size, c_align);
  }
public:
  template
  <dango::is_nohandle_allocator tp_alloc, typename... tp_args>
  requires(dango::is_brace_constructible<super_type, tp_args...>)
  static auto
  allocate
  (size_type const a_size_init, size_type const a_requested, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_nohandle_allocator<tp_alloc> &&
    dango::is_noexcept_brace_constructible<super_type, tp_args...>
  )->super_type*
  {
    constexpr bool const c_noexcept =
      dango::is_noexcept_nohandle_allocator<tp_alloc> &&
      dango::is_noexcept_brace_constructible<super_type, tp_args...>;

    constexpr auto const a_alloc =
    [](size_type const a_size, size_type const a_align)
    noexcept(dango::is_noexcept_nohandle_allocator<tp_alloc>)->dango::auto_ptr<void, tp_alloc>
    {
      return dango::make_auto_ptr<void, tp_alloc>(a_size, a_align);
    };

    return alloc_impl<c_noexcept>(a_alloc, a_size_init, a_requested, dango::forward<tp_args>(a_args)...);
  }

  template
  <dango::is_handle_based_allocator tp_alloc, typename... tp_args>
  requires(dango::is_brace_constructible<super_type, tp_args...>)
  static auto
  allocate
  (dango::allocator_handle_type<tp_alloc> const& a_handle, size_type const a_size_init, size_type const a_requested, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_handle_based_allocator<tp_alloc> &&
    dango::is_noexcept_brace_constructible<super_type, tp_args...>
  )->super_type*
  {
    constexpr bool const c_noexcept =
      dango::is_noexcept_handle_based_allocator<tp_alloc> &&
      dango::is_noexcept_brace_constructible<super_type, tp_args...>;

    auto const a_alloc =
    [&a_handle](size_type const a_size, size_type const a_align)
    noexcept(dango::is_noexcept_handle_based_allocator<tp_alloc>)->dango::auto_ptr<void, tp_alloc>
    {
      return dango::make_auto_ptr<void, tp_alloc>(a_handle, a_size, a_align);
    };

    return alloc_impl<c_noexcept>(a_alloc, a_size_init, a_requested, dango::forward<tp_args>(a_args)...);
  }

  template
  <dango::is_nohandle_allocator tp_alloc>
  static void
  deallocate
  (super_type const volatile* const a_super)noexcept
  {
    constexpr auto const a_dealloc =
     [](void const volatile* const a_ptr, size_type const a_size, size_type const a_align)noexcept->void
     {
       tp_alloc::dealloc(a_ptr, a_size, a_align);
     };

     dealloc_impl(a_dealloc, a_super);
  }

  template
  <dango::is_handle_based_allocator tp_alloc>
  static void
  deallocate
  (dango::allocator_handle_type<tp_alloc> const& a_handle, super_type const volatile* const a_super)noexcept
  {
    dango_assert_nonnull(a_handle);

    auto const a_dealloc =
     [&a_handle](void const volatile* const a_ptr, size_type const a_size, size_type const a_align)noexcept->void
     {
       a_handle->dealloc(a_ptr, a_size, a_align);
     };

    dealloc_impl(a_dealloc, a_super);
  }
private:
  template
  <typename... tp_args>
  requires(dango::is_brace_constructible<super_type, tp_args...>)
  explicit constexpr
  flex_array
  (
    ptr_type const a_array,
    size_type const a_size,
    size_type const a_capacity,
    tuple_type const& a_next,
    tp_args&&... a_args
  )
  noexcept(dango::is_noexcept_brace_constructible<super_type, tp_args...>):
  super_type{ dango::forward<tp_args>(a_args)... },
  m_array{ a_array },
  m_capacity{ a_array + a_capacity },
  m_size{ a_array + a_size },
  m_next{ a_next }
  { }
public:
  constexpr ~flex_array()noexcept = default;
public:
  constexpr auto begin()const noexcept->ptr_type{ return m_array; }
  constexpr auto end()const noexcept->ptr_type{ return m_size; }
  constexpr auto capacity_end()const noexcept->ptr_type{ return m_capacity; }
  constexpr auto size()const noexcept->size_type{ return size_type(end() - begin()); }
  constexpr auto capacity()const noexcept->size_type{ return size_type(capacity_end() - begin()); }
  constexpr auto is_empty()const noexcept->bool{ return begin() == end(); }

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

  template
  <size_type tp_index>
  requires(tp_index < (sizeof...(tp_next) + size_type(1)))
  constexpr auto
  capacity_end_at()const noexcept->auto
  {
    if constexpr(tp_index == size_type(0))
    {
      return capacity_end();
    }
    else
    {
      return m_next.template at<tp_index - size_type(1)>() + capacity();
    }
  }
private:
  ptr_type const m_array;
  ptr_type const m_capacity;
  ptr_type m_size;
  tuple_type const m_next [[no_unique_address]];
public:
  DANGO_DELETE_DEFAULT(flex_array)
  DANGO_UNMOVEABLE(flex_array)
};

template
<dango::is_class tp_header, typename tp_first, typename... tp_next>
requires
(
  !dango::is_const_or_volatile<tp_header> &&
  (!dango::is_const_or_volatile<tp_first> && ... && !dango::is_const_or_volatile<tp_next>)
)
class
dango::
detail::
flex_array_constexpr<tp_header, dango::struct_of_array<tp_first, tp_next...>>
final:
public tp_header
{
private:
  using super_type = tp_header;
public:
  using elem_type = tp_first;
  using ptr_type = elem_type*;
  using tuple_type = dango::tuple<tp_next*...>;
  using size_type = dango::usize;
public:
  template
  <typename... tp_args>
  requires((sizeof...(tp_next) != size_type(0)) && dango::is_brace_constructible<super_type, tp_args...>)
  static constexpr auto
  allocate
  (ptr_type const a_array, size_type const a_init_size, size_type const a_requested, tuple_type const& a_next, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<super_type, tp_args...>)->super_type*
  {
    dango_assert(a_init_size <= a_requested);

    auto const a_ptr =
      new flex_array_constexpr{ a_array, a_init_size, a_requested, a_next, dango::forward<tp_args>(a_args)... };

    return a_ptr;
  }

  template
  <typename... tp_args>
  requires((sizeof...(tp_next) == size_type(0)) && dango::is_brace_constructible<super_type, tp_args...>)
  static constexpr auto
  allocate
  (ptr_type const a_array, size_type const a_init_size, size_type const a_requested, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<super_type, tp_args...>)->super_type*
  {
    dango_assert(a_init_size <= a_requested);

    auto const a_ptr =
      new flex_array_constexpr{ a_array, a_init_size, a_requested, tuple_type{ }, dango::forward<tp_args>(a_args)... };

    return a_ptr;
  }

  static constexpr void
  deallocate
  (super_type const volatile* const a_super)
  {
    dango_assert_nonnull(a_super);

    delete static_cast<flex_array_constexpr const volatile*>(a_super);
  }
private:
  template
  <typename... tp_args>
  requires(dango::is_brace_constructible<super_type, tp_args...>)
  explicit constexpr
  flex_array_constexpr
  (
    ptr_type const a_array,
    size_type const a_size,
    size_type const a_capacity,
    tuple_type const& a_next,
    tp_args&&... a_args
  )
  noexcept(dango::is_noexcept_brace_constructible<super_type, tp_args...>):
  super_type{ dango::forward<tp_args>(a_args)... },
  m_array{ a_array },
  m_capacity{ a_array + a_capacity },
  m_size{ a_array + a_size },
  m_next{ a_next }
  { }
public:
  constexpr
  ~flex_array_constexpr()noexcept
  {
    m_next->*[](auto const... a_arg)noexcept->void
    {
      (void(delete[] a_arg) , ... );
    };

    delete[] m_array;
  }
public:
  constexpr auto begin()const noexcept->ptr_type{ return m_array; }
  constexpr auto end()const noexcept->ptr_type{ return m_size; }
  constexpr auto capacity_end()const noexcept->ptr_type{ return m_capacity; }
  constexpr auto size()const noexcept->size_type{ return size_type(end() - begin()); }
  constexpr auto capacity()const noexcept->size_type{ return size_type(capacity_end() - begin()); }
  constexpr auto is_empty()const noexcept->bool{ return begin() == end(); }

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

  template
  <size_type tp_index>
  requires(tp_index < (sizeof...(tp_next) + size_type(1)))
  constexpr auto
  capacity_end_at()const noexcept->auto
  {
    if constexpr(tp_index == size_type(0))
    {
      return capacity_end();
    }
    else
    {
      return m_next.template at<tp_index - size_type(1)>() + capacity();
    }
  }
private:
  ptr_type const m_array;
  ptr_type const m_capacity;
  ptr_type const m_size;
  tuple_type const m_next [[no_unique_address]];
public:
  DANGO_DELETE_DEFAULT(flex_array_constexpr)
  DANGO_UNMOVEABLE(flex_array_constexpr)
};

#endif
