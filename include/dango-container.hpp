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

  template
  <dango::is_object_exclude_array tp_type>
  struct
  stable_adaptor
  final
  {
    DANGO_TAG_TYPE(stable_adaptor)
  };

  template
  <dango::is_object_exclude_array tp_type>
  struct
  ref_adaptor
  final
  {
    DANGO_TAG_TYPE(ref_adaptor)
  };

  template
  <dango::is_object_exclude_array tp_type>
  struct
  stable_ref_adaptor
  final
  {
    DANGO_TAG_TYPE(stable_ref_adaptor)
  };
}

namespace
dango::detail
{
  template
  <typename tp_type>
  inline constexpr bool const is_struct_of_array_help = false;
  template
  <typename... tp_types>
  inline constexpr bool const is_struct_of_array_help<dango::struct_of_array<tp_types...>> = true;

  template
  <typename tp_type>
  inline constexpr bool const is_stable_adaptor_help = false;
  template
  <typename tp_type>
  inline constexpr bool const is_stable_adaptor_help<dango::stable_adaptor<tp_type>> = true;

  template
  <typename tp_type>
  inline constexpr bool const is_ref_adaptor_help = false;
  template
  <typename tp_type>
  inline constexpr bool const is_ref_adaptor_help<dango::ref_adaptor<tp_type>> = true;

  template
  <typename tp_type>
  inline constexpr bool const is_stable_ref_adaptor_help = false;
  template
  <typename tp_type>
  inline constexpr bool const is_stable_ref_adaptor_help<dango::stable_ref_adaptor<tp_type>> = true;
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_struct_of_array =
    !dango::is_const_or_volatile<tp_type> && dango::detail::is_struct_of_array_help<tp_type>;
  template
  <typename tp_type>
  concept is_bad_struct_of_array =
    dango::is_const_or_volatile<tp_type> &&
    dango::detail::is_struct_of_array_help<dango::remove_cv<tp_type>>;

  template
  <typename tp_type>
  concept is_stable_adaptor =
    !dango::is_const_or_volatile<tp_type> && dango::detail::is_stable_adaptor_help<tp_type>;
  template
  <typename tp_type>
  concept is_bad_stable_adaptor =
    dango::is_const_or_volatile<tp_type> &&
    dango::detail::is_stable_adaptor_help<dango::remove_cv<tp_type>>;

  template
  <typename tp_type>
  concept is_ref_adaptor =
    !dango::is_const_or_volatile<tp_type> && dango::detail::is_ref_adaptor_help<tp_type>;
  template
  <typename tp_type>
  concept is_bad_ref_adaptor =
    dango::is_const_or_volatile<tp_type> &&
    dango::detail::is_ref_adaptor_help<dango::remove_cv<tp_type>>;

  template
  <typename tp_type>
  concept is_stable_ref_adaptor =
    !dango::is_const_or_volatile<tp_type> && dango::detail::is_stable_ref_adaptor_help<tp_type>;
  template
  <typename tp_type>
  concept is_bad_stable_ref_adaptor =
    dango::is_const_or_volatile<tp_type> &&
    dango::detail::is_stable_ref_adaptor_help<dango::remove_cv<tp_type>>;
}

namespace
dango::detail
{
  template
  <typename tp_elem, dango::usize tp_align>
  struct alignas(tp_align)
  stable_element
  final
  {
  public:
    using elem_type = tp_elem;
    using size_type = dango::usize;
  public:
    template
    <typename... tp_args>
    requires(dango::is_brace_constructible<elem_type, tp_args...>)
    constexpr
    stable_element
    (size_type const a_index, tp_args&&... a_args)
    noexcept(dango::is_noexcept_brace_constructible<elem_type, tp_args...>):
    m_elem{ dango::forward<tp_args>(a_args)... },
    m_index{ a_index }
    { }
    constexpr ~stable_element()noexcept = default;
  public:
    elem_type m_elem;
    size_type m_index;
  public:
    DANGO_UNMOVEABLE(stable_element)
  };

  template
  <typename tp_type, dango::usize tp_align>
  struct container_elem_type;

  template
  <typename tp_type, dango::usize tp_align>
  struct container_elem_type<dango::stable_adaptor<tp_type>, tp_align>;

  template
  <typename tp_type, dango::usize tp_align>
  struct container_elem_type<dango::ref_adaptor<tp_type>, tp_align>;

  template
  <typename tp_type, dango::usize tp_align>
  struct container_elem_type<dango::stable_ref_adaptor<tp_type>, tp_align>;
}


template
<typename tp_type, dango::usize tp_align>
struct
dango::
detail::
container_elem_type
final
{
  using elem_type = tp_type;
  using elem_type_intern = dango::remove_cv<elem_type>;
  using size_type = dango::usize;

  template
  <dango::is_nohandle_allocator tp_allocator, typename... tp_args>
  requires(dango::is_brace_constructible<elem_type_intern, tp_args...>)
  static constexpr auto
  construct
  (size_type const, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<elem_type_intern, tp_args...>)->elem_type_intern
  {
    return elem_type_intern{ dango::forward<tp_args>(a_args)... };
  }

  template
  <dango::is_handle_based_allocator tp_allocator, typename... tp_args>
  requires(dango::is_brace_constructible<elem_type_intern, tp_args...>)
  static constexpr auto
  construct
  (size_type const, dango::allocator_handle_type<tp_allocator> const&, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<elem_type_intern, tp_args...>)->elem_type_intern
  {
    return elem_type_intern{ dango::forward<tp_args>(a_args)... };
  }

  template
  <dango::is_nohandle_allocator tp_allocator>
  static constexpr void
  destroy
  (elem_type_intern& a_elem)noexcept
  {
    dango::destructor_as<elem_type_intern>(&a_elem);
  }

  template
  <dango::is_handle_based_allocator tp_allocator>
  static constexpr void
  destroy
  (dango::allocator_handle_type<tp_allocator> const&, elem_type_intern& a_elem)noexcept
  {
    dango::destructor_as<elem_type_intern>(&a_elem);
  }

  template
  <typename... tp_args>
  requires(dango::is_brace_constructible<elem_type_intern, tp_args...>)
  static constexpr auto
  construct_constexpr
  (size_type const, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<elem_type_intern, tp_args...>)->elem_type_intern
  {
    return elem_type_intern{ dango::forward<tp_args>(a_args)... };
  }

  static constexpr void
  destroy_constexpr
  (elem_type_intern&)noexcept
  {

  }

  DANGO_UNCONSTRUCTIBLE(container_elem_type)
};

template
<typename tp_type, dango::usize tp_align>
struct
dango::
detail::
container_elem_type<dango::stable_adaptor<tp_type>, tp_align>
final
{
  using elem_type = tp_type;
  using elem_storage = dango::detail::stable_element<dango::remove_cv<elem_type>, tp_align>;
  using elem_type_intern = elem_storage*;
  using size_type = dango::usize;

  template
  <dango::is_nohandle_allocator tp_allocator, typename... tp_args>
  requires(dango::is_brace_constructible<elem_type_intern, tp_args...>)
  static constexpr auto
  construct
  (size_type const a_index, tp_args&&... a_args)
  noexcept(dango::is_noexcept_nohandle_allocator<tp_allocator> && dango::is_noexcept_brace_constructible<elem_type_intern, tp_args...>)->elem_type_intern
  {
    auto a_void = dango::make_auto_ptr<void, tp_allocator>(sizeof(elem_storage), alignof(elem_storage));

    auto const a_elem = dango_placement_new(a_void.get(), elem_storage, { a_index, dango::forward<tp_args>(a_args)... });

    a_void.release();

    return a_elem;
  }

  template
  <dango::is_handle_based_allocator tp_allocator, typename... tp_args>
  requires(dango::is_brace_constructible<elem_type_intern, tp_args...>)
  static constexpr auto
  construct
  (size_type const a_index, dango::allocator_handle_type<tp_allocator> const& a_handle, tp_args&&... a_args)
  noexcept(dango::is_noexcept_handle_based_allocator<tp_allocator> && dango::is_noexcept_brace_constructible<elem_type_intern, tp_args...>)->elem_type_intern
  {
    auto a_void = dango::make_auto_ptr<void, tp_allocator>(a_handle, sizeof(elem_storage), alignof(elem_storage));

    auto const a_elem = dango_placement_new(a_void.get(), elem_storage, { a_index, dango::forward<tp_args>(a_args)... });

    a_void.release();

    return a_elem;
  }

  template
  <dango::is_nohandle_allocator tp_allocator>
  static constexpr void
  destroy
  (elem_type_intern& a_elem)noexcept
  {
    dango::destructor_as<elem_storage>(a_elem);

    tp_allocator::dealloc(a_elem, sizeof(elem_storage), alignof(elem_storage));

    dango::destructor_as<elem_type_intern>(&a_elem);
  }

  template
  <dango::is_handle_based_allocator tp_allocator>
  static constexpr void
  destroy
  (dango::allocator_handle_type<tp_allocator> const& a_handle, elem_type_intern& a_elem)noexcept
  {
    dango::destructor_as<elem_storage>(a_elem);

    a_handle->dealloc(a_elem, sizeof(elem_storage), alignof(elem_storage));

    dango::destructor_as<elem_type_intern>(&a_elem);
  }

  template
  <typename... tp_args>
  requires(dango::is_brace_constructible<elem_type_intern, tp_args...>)
  static constexpr auto
  construct_constexpr
  (size_type const a_index, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<elem_type_intern, tp_args...>)->elem_type_intern
  {
    return new elem_storage{ a_index, dango::forward<tp_args>(a_args)... };
  }

  static constexpr void
  destroy_constexpr
  (elem_type_intern& a_elem)noexcept
  {
    delete a_elem;
  }

  DANGO_UNCONSTRUCTIBLE(container_elem_type)
};

template
<typename tp_type, dango::usize tp_align>
struct
dango::
detail::
container_elem_type<dango::ref_adaptor<tp_type>, tp_align>
final
{
  using elem_type = tp_type;
  using elem_type_intern = elem_type*;
  using size_type = dango::usize;

  template
  <dango::is_nohandle_allocator tp_allocator>
  static constexpr auto
  construct
  (size_type const, elem_type& a_arg)noexcept->elem_type_intern
  {
    return elem_type_intern{ dango::addressof(a_arg) };
  }

  template
  <dango::is_handle_based_allocator tp_allocator>
  static constexpr auto
  construct
  (size_type const, dango::allocator_handle_type<tp_allocator> const&, elem_type& a_arg)noexcept->elem_type_intern
  {
    return elem_type_intern{ dango::addressof(a_arg) };
  }

  template
  <dango::is_nohandle_allocator tp_allocator>
  static constexpr void
  destroy
  (elem_type_intern& a_elem)noexcept
  {
    dango::destructor_as<elem_type_intern>(&a_elem);
  }

  template
  <dango::is_handle_based_allocator tp_allocator>
  static constexpr void
  destroy
  (dango::allocator_handle_type<tp_allocator> const&, elem_type_intern& a_elem)noexcept
  {
    dango::destructor_as<elem_type_intern>(&a_elem);
  }

  static constexpr auto
  construct_constexpr
  (size_type const, elem_type& a_arg)noexcept->elem_type_intern
  {
    return elem_type_intern{ dango::addressof(a_arg) };
  }

  static constexpr void
  destroy_constexpr
  (elem_type_intern&)noexcept
  {

  }

  DANGO_UNCONSTRUCTIBLE(container_elem_type)
};

template
<typename tp_type, dango::usize tp_align>
struct
dango::
detail::
container_elem_type<dango::stable_ref_adaptor<tp_type>, tp_align>
final
{
  using elem_type = tp_type;
  using elem_storage = dango::detail::stable_element<elem_type* const, tp_align>;
  using elem_type_intern = elem_storage*;
  using size_type = dango::usize;

  template
  <dango::is_nohandle_allocator tp_allocator>
  static constexpr auto
  construct
  (size_type const a_index, elem_type& a_arg)
  noexcept(dango::is_noexcept_nohandle_allocator<tp_allocator>)->elem_type_intern
  {
    auto a_void = dango::make_auto_ptr<void, tp_allocator>(sizeof(elem_storage), alignof(elem_storage));

    auto const a_elem = dango_placement_new(a_void.get(), elem_storage, { a_index, dango::addressof(a_arg) });

    a_void.release();

    return a_elem;
  }

  template
  <dango::is_handle_based_allocator tp_allocator>
  static constexpr auto
  construct
  (size_type const a_index, dango::allocator_handle_type<tp_allocator> const& a_handle, elem_type& a_arg)
  noexcept(dango::is_noexcept_handle_based_allocator<tp_allocator>)->elem_type_intern
  {
    auto a_void = dango::make_auto_ptr<void, tp_allocator>(a_handle, sizeof(elem_storage), alignof(elem_storage));

    auto const a_elem = dango_placement_new(a_void.get(), elem_storage, { a_index, dango::addressof(a_arg) });

    a_void.release();

    return a_elem;
  }

  template
  <dango::is_nohandle_allocator tp_allocator>
  static constexpr void
  destroy
  (elem_type_intern& a_elem)noexcept
  {
    dango::destructor_as<elem_storage>(a_elem);

    tp_allocator::dealloc(a_elem, sizeof(elem_storage), alignof(elem_storage));

    dango::destructor_as<elem_type_intern>(&a_elem);
  }

  template
  <dango::is_handle_based_allocator tp_allocator>
  static constexpr void
  destroy
  (dango::allocator_handle_type<tp_allocator> const& a_handle, elem_type_intern& a_elem)noexcept
  {
    dango::destructor_as<elem_storage>(a_elem);

    a_handle->dealloc(a_elem, sizeof(elem_storage), alignof(elem_storage));

    dango::destructor_as<elem_type_intern>(&a_elem);
  }

  static constexpr auto
  construct_constexpr
  (size_type const a_index, elem_type& a_arg)noexcept->elem_type_intern
  {
    return new elem_storage{ a_index, dango::addressof(a_arg) };
  }

  static constexpr void
  destroy_constexpr
  (elem_type_intern& a_elem)noexcept
  {
    delete a_elem;
  }

  DANGO_UNCONSTRUCTIBLE(container_elem_type)
};

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
      a_void_tuple->*[](auto const&... a_arg)noexcept->auto
      {
        return dango::make_tuple(reinterpret_cast<tp_next*>(dango_placement_new_array(a_arg.get(), dango::byte, a_arg.size()))...);
      };

    auto const a_ptr =
      dango_placement_new(a_bytes, flex_array, { a_array, a_array_tuple, a_size_init, a_capacity, dango::forward<tp_args>(a_args)... });

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
    tuple_type const& a_next,
    size_type const a_size,
    size_type const a_capacity,
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
  requires
  (
    (dango::is_default_constructible<elem_type> && ... && dango::is_default_constructible<tp_next>) &&
    dango::is_brace_constructible<super_type, tp_args...>
  )
  static constexpr auto
  allocate_n
  (size_type const a_init_size, size_type const a_capacity, tp_args&&... a_args)
  noexcept
  (
    (dango::is_noexcept_default_constructible<elem_type> && ... && dango::is_noexcept_default_constructible<tp_next>) &&
    dango::is_noexcept_brace_constructible<super_type, tp_args...>
  )->super_type*
  {
    dango_assert(a_init_size <= a_capacity);

    dango::auto_ptr a_array{ new elem_type[a_capacity], dango::array_delete };

    auto a_array_tuple =
      dango::make_tuple(dango::auto_ptr{ new tp_next[a_capacity], dango::array_delete }...);

    auto const a_next =
      a_array_tuple->*[](auto const&... a_arg)noexcept->tuple_type
      {
        return tuple_type{ a_arg.get()... };
      };

    auto const a_ptr =
      new flex_array_constexpr{ a_array.get(), a_next, a_init_size, a_capacity, dango::forward<tp_args>(a_args)... };

    a_array_tuple->*[](auto&... a_arg)noexcept->void{ (void(a_arg.release()) , ... ); };

    a_array.release();

    return a_ptr;
  }

  template
  <typename... tp_init, typename... tp_args>
  requires
  (
    (sizeof...(tp_next) == size_type(0)) &&
    ( ... && dango::is_ref<tp_init>) &&
    ( ... && dango::is_brace_constructible<elem_type, tp_init>) &&
    dango::is_brace_constructible<super_type, tp_args...>
  )
  static constexpr auto
  allocate_init
  (dango::tuple<tp_init...> const& a_init, size_type const a_capacity, tp_args&&... a_args)
  noexcept
  (
    ( ... && dango::is_noexcept_brace_constructible<elem_type, tp_init>) &&
    dango::is_noexcept_brace_constructible<super_type, tp_args...>
  )->super_type*
  {
    constexpr auto const c_init_size = sizeof...(tp_init);

    dango_assert(c_init_size <= a_capacity);

    auto a_array =
      a_init->*[a_capacity]<typename... tp_init_arg>
      (tp_init_arg&&... a_arg)
      noexcept(( ... && dango::is_noexcept_brace_constructible<elem_type, tp_init>))->auto
      {
        return dango::auto_ptr{ new elem_type[a_capacity]{ dango::forward<tp_init_arg>(a_arg)... }, dango::array_delete };
      };

    auto const a_ptr =
      new flex_array_constexpr{ a_array.get(), tuple_type{ }, c_init_size, a_capacity, dango::forward<tp_args>(a_args)... };

    a_array.release();

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
    tuple_type const& a_next,
    size_type const a_size,
    size_type const a_capacity,
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
  ptr_type m_size;
  tuple_type const m_next [[no_unique_address]];
public:
  DANGO_DELETE_DEFAULT(flex_array_constexpr)
  DANGO_UNMOVEABLE(flex_array_constexpr)
};

namespace
dango::detail
{
  template
  <dango::is_class tp_header, dango::is_derived_from<tp_header> tp_array, dango::is_derived_from<tp_header> tp_array_constexpr>
  class flex_array_dispatcher;
}

template
<dango::is_class tp_header, dango::is_derived_from<tp_header> tp_array, dango::is_derived_from<tp_header> tp_array_constexpr>
class
dango::
detail::
flex_array_dispatcher
final
{
public:
  using header_type = tp_header;
  using array_type = tp_array;
  using array_type_constexpr = tp_array_constexpr;
public:

#define DANGO_CONTAINER_DEFINE_DISPATCH(name) \
  static constexpr auto \
  name \
  (header_type const* const a_header)noexcept->auto \
  { \
    dango_assert_nonnull(a_header); \
    if(dango::in_constexpr_context()) \
    { \
      return static_cast<array_type_constexpr const*>(a_header)->name(); \
    } \
    else \
    { \
      return static_cast<array_type const*>(a_header)->name(); \
    } \
  }

  DANGO_CONTAINER_DEFINE_DISPATCH(begin)
  DANGO_CONTAINER_DEFINE_DISPATCH(end)
  DANGO_CONTAINER_DEFINE_DISPATCH(capacity_end)
  DANGO_CONTAINER_DEFINE_DISPATCH(size)
  DANGO_CONTAINER_DEFINE_DISPATCH(capacity)
  DANGO_CONTAINER_DEFINE_DISPATCH(is_empty)

#undef DANGO_CONTAINER_DEFINE_DISPATCH

#define DANGO_CONTAINER_DEFINE_DISPATCH(name) \
  template \
  <dango::usize tp_index> \
  static constexpr auto \
  name \
  (header_type const* const a_header)noexcept->auto \
  { \
    dango_assert_nonnull(a_header); \
    if(dango::in_constexpr_context()) \
    { \
      return static_cast<array_type_constexpr const*>(a_header)->template name<tp_index>(); \
    } \
    else \
    { \
      return static_cast<array_type const*>(a_header)->template name<tp_index>(); \
    } \
  }

  DANGO_CONTAINER_DEFINE_DISPATCH(begin_at)
  DANGO_CONTAINER_DEFINE_DISPATCH(end_at)
  DANGO_CONTAINER_DEFINE_DISPATCH(capacity_end_at)

#undef DANGO_CONTAINER_DEFINE_DISPATCH

public:
  DANGO_UNCONSTRUCTIBLE(flex_array_dispatcher)
};

#endif
