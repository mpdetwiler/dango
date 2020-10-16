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
  <dango::is_object_exclude_array tp_type, dango::usize tp_align>
  struct container_elem_type;

  template
  <dango::is_object_exclude_array tp_type, dango::usize tp_align>
  struct container_elem_type<dango::stable_adaptor<tp_type>, tp_align>;

  template
  <dango::is_object_exclude_array tp_type, dango::usize tp_align>
  struct container_elem_type<dango::ref_adaptor<tp_type>, tp_align>;

  template
  <dango::is_object_exclude_array tp_type, dango::usize tp_align>
  struct container_elem_type<dango::stable_ref_adaptor<tp_type>, tp_align>;
}


template
<dango::is_object_exclude_array tp_type, dango::usize tp_align>
struct
dango::
detail::
container_elem_type
final
{
  using elem_type = tp_type;
  using elem_type_intern = dango::remove_cv<elem_type>;
  using elem_type_intern_constexpr =
    dango::conditional<dango::is_trivial_default_constructible<elem_type_intern>, elem_type_intern, elem_type_intern*>;
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
  (typename tp_allocator::handle_type const&, size_type const, tp_args&&... a_args)
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
  noexcept(dango::is_noexcept_brace_constructible<elem_type_intern, tp_args...>)->elem_type_intern_constexpr
  {
    if constexpr(!dango::is_trivial_default_constructible<elem_type_intern>)
    {
      return new elem_type_intern{ dango::forward<tp_args>(a_args)... };
    }
    else
    {
      return elem_type_intern{ dango::forward<tp_args>(a_args)... };
    }
  }

  static constexpr void
  destroy_constexpr
  (elem_type_intern_constexpr& a_elem)noexcept
  {
    if constexpr(!dango::is_trivial_default_constructible<elem_type_intern>)
    {
      delete a_elem;
    }
  }

  DANGO_UNCONSTRUCTIBLE(container_elem_type)
};

template
<dango::is_object_exclude_array tp_type, dango::usize tp_align>
struct
dango::
detail::
container_elem_type<dango::stable_adaptor<tp_type>, tp_align>
final
{
  using elem_type = tp_type;
  using elem_storage = dango::detail::stable_element<dango::remove_cv<elem_type>, tp_align>;
  using elem_type_intern = elem_storage*;
  using elem_type_intern_constexpr = elem_type_intern;
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
  (typename tp_allocator::handle_type const& a_handle, size_type const a_index, tp_args&&... a_args)
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
  noexcept(dango::is_noexcept_brace_constructible<elem_type_intern, tp_args...>)->elem_type_intern_constexpr
  {
    return new elem_storage{ a_index, dango::forward<tp_args>(a_args)... };
  }

  static constexpr void
  destroy_constexpr
  (elem_type_intern_constexpr& a_elem)noexcept
  {
    delete a_elem;
  }

  DANGO_UNCONSTRUCTIBLE(container_elem_type)
};

template
<dango::is_object_exclude_array tp_type, dango::usize tp_align>
struct
dango::
detail::
container_elem_type<dango::ref_adaptor<tp_type>, tp_align>
final
{
  using elem_type = tp_type;
  using elem_type_intern = elem_type*;
  using elem_type_intern_constexpr = elem_type_intern;
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
  (typename tp_allocator::handle_type const&, size_type const, elem_type& a_arg)noexcept->elem_type_intern
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
  (size_type const, elem_type& a_arg)noexcept->elem_type_intern_constexpr
  {
    return elem_type_intern{ dango::addressof(a_arg) };
  }

  static constexpr void
  destroy_constexpr
  (elem_type_intern_constexpr&)noexcept
  {

  }

  DANGO_UNCONSTRUCTIBLE(container_elem_type)
};

template
<dango::is_object_exclude_array tp_type, dango::usize tp_align>
struct
dango::
detail::
container_elem_type<dango::stable_ref_adaptor<tp_type>, tp_align>
final
{
  using elem_type = tp_type;
  using elem_storage = dango::detail::stable_element<elem_type* const, tp_align>;
  using elem_type_intern = elem_storage*;
  using elem_type_intern_constexpr = elem_type_intern;
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
  (typename tp_allocator::handle_type const& a_handle, size_type const a_index, elem_type& a_arg)
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
  (size_type const a_index, elem_type& a_arg)noexcept->elem_type_intern_constexpr
  {
    return new elem_storage{ a_index, dango::addressof(a_arg) };
  }

  static constexpr void
  destroy_constexpr
  (elem_type_intern_constexpr& a_elem)noexcept
  {
    delete a_elem;
  }

  DANGO_UNCONSTRUCTIBLE(container_elem_type)
};

namespace
dango::detail
{
  template
  <typename tp_type, dango::usize tp_align>
  inline constexpr bool const is_container_elem_type_help = false;
  template
  <typename tp_type, dango::usize tp_align>
  inline constexpr bool const is_container_elem_type_help<dango::detail::container_elem_type<tp_type, tp_align>, tp_align> = true;

  template
  <typename tp_type, dango::usize tp_align>
  concept is_container_elem_type =
    dango::detail::is_container_elem_type_help<tp_type, tp_align>;
}

namespace
dango::detail
{
  template
  <typename tp_header, dango::usize tp_align, typename tp_elem>
  class flex_array;

  template
  <dango::is_class tp_header, dango::usize tp_align, dango::detail::is_container_elem_type<tp_align> tp_first, dango::detail::is_container_elem_type<tp_align>... tp_next>
  requires(!dango::is_const_or_volatile<tp_header> && dango::is_pow_two(tp_align))
  class flex_array<tp_header, tp_align, dango::struct_of_array<tp_first, tp_next...>>;

  template
  <typename tp_header, dango::usize tp_align, typename tp_elem>
  class flex_array_constexpr;

  template
  <dango::is_class tp_header, dango::usize tp_align, dango::detail::is_container_elem_type<tp_align> tp_first, dango::detail::is_container_elem_type<tp_align>... tp_next>
  requires(!dango::is_const_or_volatile<tp_header> && dango::is_pow_two(tp_align))
  class flex_array_constexpr<tp_header, tp_align, dango::struct_of_array<tp_first, tp_next...>>;
}

template
<dango::is_class tp_header, dango::usize tp_align, dango::detail::is_container_elem_type<tp_align> tp_first, dango::detail::is_container_elem_type<tp_align>... tp_next>
requires(!dango::is_const_or_volatile<tp_header> && dango::is_pow_two(tp_align))
class alignas(dango::aligned_union<dango::aligned_storage<tp_align>, typename tp_first::elem_type_intern>)
dango::
detail::
flex_array<tp_header, tp_align, dango::struct_of_array<tp_first, tp_next...>>
final:
public tp_header
{
private:
  using super_type = tp_header;
public:
  using elem_type = typename tp_first::elem_type_intern;
  using ptr_type = elem_type*;
  using tuple_type = dango::tuple<typename tp_next::elem_type_intern*...>;
  using size_type = dango::usize;
public:
  template
  <dango::is_allocator tp_allocator>
  struct allocator;
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
<dango::is_class tp_header, dango::usize tp_align, dango::detail::is_container_elem_type<tp_align> tp_first, dango::detail::is_container_elem_type<tp_align>... tp_next>
requires(!dango::is_const_or_volatile<tp_header> && dango::is_pow_two(tp_align))
template
<dango::is_allocator tp_allocator>
struct
dango::
detail::
flex_array<tp_header, tp_align, dango::struct_of_array<tp_first, tp_next...>>::
allocator
final
{
private:
  using outer_index_seq = dango::make_index_seq<size_type(1) + sizeof...(tp_next)>;
private:
  template
  <typename... tp_elems>
  requires(sizeof...(tp_elems) == (size_type(1) + sizeof...(tp_next)))
  struct
  allocate_init_help
  final
  {
  private:
    template
    <typename tp_elem, size_type tp_index, size_type... tp_indices, typename... tp_tuples, typename tp_construct, typename tp_destroy>
    requires(sizeof...(tp_indices) == sizeof...(tp_tuples))
    static constexpr auto
    inner
    (
      dango::index_seq<tp_indices...> const,
      dango::tuple<tp_tuples...> const& a_init,
      flex_array const* const a_mem,
      tp_construct const& a_construct,
      tp_destroy const& a_destroy
    )
    noexcept(requires{ { ( ... , void(a_construct(dango::type_tag<tp_elem>{ }, tp_indices, dango::tuple_get<tp_index>(dango::tuple_get<tp_indices>(a_init))))) }noexcept; })->auto
    requires(requires{ { ( ... , void(a_construct(dango::type_tag<tp_elem>{ }, tp_indices, dango::tuple_get<tp_index>(dango::tuple_get<tp_indices>(a_init))))) }; })
    {
      using intern_type = typename tp_elem::elem_type_intern;

      constexpr auto const c_size = sizeof...(tp_tuples);

      auto const a_deleter =
        [&a_destroy](intern_type* const a_array)noexcept->void
        {
          for(auto a_i = c_size; a_i != size_type(0); --a_i)
          {
            a_destroy(dango::type_tag<tp_elem>{ }, a_array[a_i - size_type(1)]);
          }
        };

      intern_type* const a_begin = a_mem->begin_at<tp_index>();

      auto a_constructed = size_type(0);

      auto a_guard =
      dango::make_guard
      (
        [&a_destroy, a_begin, &a_constructed]()noexcept->void
        {
          for(auto a_i = a_constructed; a_i != size_type(0); --a_i)
          {
            a_destroy(dango::type_tag<tp_elem>{ }, a_begin[a_i - size_type(1)]);
          }
        }
      );

      ( ... , void((void(dango_placement_new(a_begin + tp_indices, intern_type, { a_construct(dango::type_tag<tp_elem>{ }, tp_indices, dango::tuple_get<tp_index>(dango::tuple_get<tp_indices>(a_init))) })), ++a_constructed)));

      a_guard.dismiss();

      return dango::auto_ptr{ a_begin, a_deleter };
    }
  public:
    template
    <size_type... tp_indices, dango::is_ref... tp_tuples, typename tp_construct, typename tp_destroy>
    requires
    (
      (sizeof...(tp_elems) == sizeof...(tp_indices)) &&
      ( ... && dango::is_tuple<dango::remove_cvref<tp_tuples>>) &&
      ( ... && (sizeof...(tp_elems) == dango::tuple_size<dango::remove_cvref<tp_tuples>>))
    )
    static constexpr void
    outer
    (
      dango::index_seq<tp_indices...> const,
      dango::tuple<tp_tuples...> const& a_init,
      flex_array const* const a_mem,
      tp_construct const& a_construct,
      tp_destroy const& a_destroy
    )
    noexcept(requires{ { ( ... , void(inner<tp_elems, tp_indices>(dango::make_index_seq<sizeof...(tp_tuples)>{ }, a_init, a_mem, a_construct, a_destroy))) }noexcept; })
    requires(requires{ { ( ... , void(inner<tp_elems, tp_indices>(dango::make_index_seq<sizeof...(tp_tuples)>{ }, a_init, a_mem, a_construct, a_destroy))) }; })
    {
      auto a_tuple =
        dango::make_tuple(inner<tp_elems, tp_indices>(dango::make_index_seq<sizeof...(tp_tuples)>{ }, a_init, a_mem, a_construct, a_destroy)...);

      a_tuple->*[](auto&... a_ptr)noexcept->void{ (void(a_ptr.release()) , ... ); };
    }
  public:
    DANGO_UNCONSTRUCTIBLE(allocate_init_help)
  };
private:
  static inline constexpr auto const construct_test_nohandle =
    []<typename tp_elem, typename tp_arg>
    (dango::type_tag<tp_elem> const, size_type const a_index, tp_arg&& a_arg)
    noexcept(requires{ { tp_elem::template construct<tp_allocator>(a_index, dango::forward<tp_arg>(a_arg)) }noexcept; })->void
    requires(requires{ { tp_elem::template construct<tp_allocator>(a_index, dango::forward<tp_arg>(a_arg)) }; })
    { };

  static inline constexpr auto const construct_test_handle_based =
    []<typename tp_elem, typename tp_arg>
    (dango::type_tag<tp_elem> const, size_type const a_index, tp_arg&& a_arg)
    noexcept(requires{ { tp_elem::template construct<tp_allocator>(dango::declval<typename tp_allocator::handle_type const&>(), a_index, dango::forward<tp_arg>(a_arg)) }noexcept; })->void
    requires(requires{ { tp_elem::template construct<tp_allocator>(dango::declval<typename tp_allocator::handle_type const&>(), a_index, dango::forward<tp_arg>(a_arg)) }; })
    { };

  static inline constexpr auto const destroy_test_nohandle =
    []<typename tp_elem, typename tp_arg>
    (dango::type_tag<tp_elem> const, typename tp_elem::elem_type_intern& a_elem)
    noexcept(requires{ { tp_elem::template destroy<tp_allocator>(a_elem) }noexcept; })->void
    requires(requires{ { tp_elem::template destroy<tp_allocator>(a_elem) }; })
    { };

  static inline constexpr auto const destroy_test_handle_based =
    []<typename tp_elem, typename tp_arg>
    (dango::type_tag<tp_elem> const, typename tp_elem::elem_type_intern& a_elem)
    noexcept(requires{ { tp_elem::template destroy<tp_allocator>(dango::declval<typename tp_allocator::handle_type const&>(), a_elem) }noexcept; })->void
    requires(requires{ { tp_elem::template destroy<tp_allocator>(dango::declval<typename tp_allocator::handle_type const&>(), a_elem) }; })
    { };
private:
  template
  <bool tp_noexcept, typename tp_alloc, typename... tp_args>
  static auto
  alloc_impl
  (tp_alloc const& a_alloc, size_type const a_size_init, size_type const a_requested, tp_args&&... a_args)noexcept(tp_noexcept)->flex_array*
  {
    dango_assert(a_size_init <= a_requested);

    constexpr auto const c_align = alignof(flex_array);

    auto const a_size =
      dango::next_multiple(sizeof(flex_array) + a_requested * sizeof(elem_type), c_align);

    auto const a_capacity = (a_size - sizeof(flex_array)) / sizeof(elem_type);

    dango_assert(a_capacity >= a_requested);

    auto a_void = a_alloc(a_size, c_align);

    auto const a_bytes = dango_placement_new_array(a_void.get(), dango::byte, a_size);

    auto const a_array = reinterpret_cast<ptr_type>(a_bytes + sizeof(flex_array));

    auto a_void_tuple =
      dango::make_tuple(a_alloc(a_capacity * sizeof(typename tp_next::elem_type_intern), dango::max(tp_align, alignof(typename tp_next::elem_type_intern)))...);

    auto const a_next =
      a_void_tuple->*[](auto const&... a_ptr)noexcept->auto
      {
        return dango::make_tuple(reinterpret_cast<typename tp_next::elem_type_intern*>(dango_placement_new_array(a_ptr.get(), dango::byte, a_ptr.size()))...);
      };

    auto const a_mem =
      dango_placement_new(a_bytes, flex_array, { a_array, a_next, a_size_init, a_capacity, dango::forward<tp_args>(a_args)... });

    a_void_tuple->*[](auto&... a_ptr)noexcept->void{ (void(a_ptr.release()) , ... ); };

    a_void.release();

    return a_mem;
  }

  template
  <typename tp_dealloc>
  static void
  dealloc_impl
  (tp_dealloc const& a_dealloc, super_type const* const a_super)noexcept
  {
    dango_assert_nonnull(a_super);

    constexpr auto const c_align = alignof(flex_array);

    auto const a_mem = static_cast<flex_array const*>(a_super);

    auto const a_capacity = a_mem->capacity();

    auto const a_size =
      dango::next_multiple(sizeof(flex_array) + a_capacity * sizeof(elem_type), c_align);

    auto const& a_next = a_mem->m_next;

    a_next->*[&a_dealloc, a_capacity](auto const... a_arg)noexcept->void
    {
      (void(a_dealloc(a_arg, a_capacity * sizeof(typename tp_next::elem_type_intern), dango::max(tp_align, alignof(typename tp_next::elem_type_intern)))) , ... );
    };

    dango::destructor(a_mem);

    a_dealloc(a_mem, a_size, c_align);
  }
public:
  template
  <dango::is_same<tp_allocator> tp_alloc = tp_allocator, typename... tp_args>
  requires(dango::is_nohandle_allocator<tp_alloc> && dango::is_brace_constructible<super_type, tp_args...>)
  static auto
  allocate_n
  (size_type const a_size_init, size_type const a_requested, tp_args&&... a_args)
  noexcept(dango::is_noexcept_nohandle_allocator<tp_alloc> && dango::is_noexcept_brace_constructible<super_type, tp_args...>)->flex_array*
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
  <dango::is_same<tp_allocator> tp_alloc = tp_allocator, typename... tp_args>
  requires(dango::is_handle_based_allocator<tp_alloc> && dango::is_brace_constructible<super_type, tp_args...>)
  static auto
  allocate_n
  (typename tp_alloc::handle_type const& a_handle, size_type const a_size_init, size_type const a_requested, tp_args&&... a_args)
  noexcept(dango::is_noexcept_handle_based_allocator<tp_alloc> && dango::is_noexcept_brace_constructible<super_type, tp_args...>)->flex_array*
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
  <dango::is_same<tp_allocator> tp_alloc = tp_allocator, typename... tp_tuples, typename... tp_args>
  requires(dango::is_nohandle_allocator<tp_alloc>)
  static auto
  allocate_init
  (dango::tuple<tp_tuples...> const& a_init, size_type const a_requested, tp_args&&... a_args)
  noexcept
  (
    requires
    {
      { allocate_n(dango::declval<size_type const&>(), dango::declval<size_type const&>(), dango::declval<tp_args>()...) }noexcept;
      { allocate_init_help<tp_first, tp_next...>::outer(outer_index_seq{ }, a_init, dango::null, construct_test_nohandle, destroy_test_nohandle) }noexcept;
    }
  )->flex_array*
  requires
  (
    requires
    {
      { allocate_n(dango::declval<size_type const&>(), dango::declval<size_type const&>(), dango::declval<tp_args>()...) };
      { allocate_init_help<tp_first, tp_next...>::outer(outer_index_seq{ }, a_init, dango::null, construct_test_nohandle, destroy_test_nohandle) };
    }
  )
  {
    constexpr auto const c_size_init = sizeof...(tp_tuples);

    dango_assert(c_size_init <= a_requested);

    auto const a_mem = allocate_n(c_size_init, a_requested, dango::forward<tp_args>(a_args)...);

    auto a_guard = dango::make_guard([a_mem]()noexcept->void{ deallocate(a_mem); });

    constexpr auto const c_construct =
      []<typename tp_elem, typename tp_arg>
      (dango::type_tag<tp_elem> const, size_type const a_index, tp_arg&& a_arg)
      noexcept(requires{ { tp_elem::template construct<tp_alloc>(a_index, dango::forward<tp_arg>(a_arg)) }noexcept; })->typename tp_elem::elem_type_intern
      {
        return tp_elem::template construct<tp_alloc>(a_index, dango::forward<tp_arg>(a_arg));
      };

    constexpr auto const c_destroy =
      []<typename tp_elem>
      (dango::type_tag<tp_elem> const, typename tp_elem::elem_type_intern& a_elem)noexcept->void
      {
        tp_elem::template destroy<tp_alloc>(a_elem);
      };

    allocate_init_help<tp_first, tp_next...>::outer(outer_index_seq{ }, a_init, a_mem, c_construct, c_destroy);

    a_guard.dismiss();

    return a_mem;
  }

  template
  <dango::is_same<tp_allocator> tp_alloc = tp_allocator, typename... tp_tuples, typename... tp_args>
  requires(dango::is_handle_based_allocator<tp_alloc>)
  static auto
  allocate_init
  (typename tp_alloc::handle_type const& a_handle, dango::tuple<tp_tuples...> const& a_init, size_type const a_requested, tp_args&&... a_args)
  noexcept
  (
    requires
    {
      { allocate_n(a_handle, dango::declval<size_type const&>(), dango::declval<size_type const&>(), dango::declval<tp_args>()...) }noexcept;
      { allocate_init_help<tp_first, tp_next...>::outer(outer_index_seq{ }, a_init, dango::null, construct_test_handle_based, destroy_test_handle_based) }noexcept;
    }
  )->flex_array*
  requires
  (
    requires
    {
      { allocate_n(a_handle, dango::declval<size_type const&>(), dango::declval<size_type const&>(), dango::declval<tp_args>()...) };
      { allocate_init_help<tp_first, tp_next...>::outer(outer_index_seq{ }, a_init, dango::null, construct_test_handle_based, destroy_test_handle_based) };
    }
  )
  {
    constexpr auto const c_size_init = sizeof...(tp_tuples);

    dango_assert(c_size_init <= a_requested);

    auto const a_mem = allocate_n(c_size_init, a_requested, dango::forward<tp_args>(a_args)...);

    auto a_guard = dango::make_guard([&a_handle, a_mem]()noexcept->void{ deallocate(a_handle, a_mem); });

    auto const a_construct =
      [&a_handle]<typename tp_elem, typename tp_arg>
      (dango::type_tag<tp_elem> const, size_type const a_index, tp_arg&& a_arg)
      noexcept(requires{ { tp_elem::template construct<tp_alloc>(a_index, dango::forward<tp_arg>(a_arg)) }noexcept; })->typename tp_elem::elem_type_intern
      {
        return tp_elem::template construct<tp_alloc>(a_handle, a_index, dango::forward<tp_arg>(a_arg));
      };

    auto const a_destroy =
      [&a_handle]<typename tp_elem>
      (dango::type_tag<tp_elem> const, typename tp_elem::elem_type_intern& a_elem)noexcept->void
      {
        tp_elem::template destroy<tp_alloc>(a_handle, a_elem);
      };

    allocate_init_help<tp_first, tp_next...>::outer(outer_index_seq{ }, a_init, a_mem, a_construct, a_destroy);

    a_guard.dismiss();

    return a_mem;
  }

  template
  <dango::is_same<tp_allocator> tp_alloc = tp_allocator, typename... tp_init, typename... tp_args>
  requires((sizeof...(tp_next) == size_type(0)) && dango::is_nohandle_allocator<tp_alloc>)
  static auto
  allocate_init_single_array
  (dango::tuple<tp_init...> const& a_init, size_type const a_requested, tp_args&&... a_args)
  noexcept(requires{ { allocate_init(dango::declval<dango::tuple<dango::tuple<tp_init> const&...> const&>(), a_requested, dango::declval<tp_args>()...) }noexcept; })->flex_array*
  requires(requires{ { allocate_init(dango::declval<dango::tuple<dango::tuple<tp_init> const&...> const&>(), a_requested, dango::declval<tp_args>()...) }; })
  {
    auto const a_tuple =
      a_init->*[]<typename... tp_init_args>
      (tp_init_args&&... a_init_args)noexcept->auto
      {
        return dango::make_tuple(dango::forward_as_tuple(dango::forward<tp_init_args>(a_init_args))...);
      };

    auto const a_ref_tuple =
      a_tuple->*[]<typename... tp_tuples>
      (tp_tuples const&... a_tuples)noexcept->auto
      {
        return dango::tie_as_tuple(a_tuples...);
      };

    return allocate_init(a_ref_tuple, a_requested, dango::forward<tp_args>(a_args)...);
  }

  template
  <dango::is_same<tp_allocator> tp_alloc = tp_allocator, typename... tp_init, typename... tp_args>
  requires((sizeof...(tp_next) == size_type(0)) && dango::is_handle_based_allocator<tp_alloc>)
  static auto
  allocate_init_single_array
  (typename tp_alloc::handle_type const& a_handle, dango::tuple<tp_init...> const& a_init, size_type const a_requested, tp_args&&... a_args)
  noexcept(requires{ { allocate_init(a_handle, dango::declval<dango::tuple<dango::tuple<tp_init> const&...> const&>(), a_requested, dango::declval<tp_args>()...) }noexcept; })->flex_array*
  requires(requires{ { allocate_init(a_handle, dango::declval<dango::tuple<dango::tuple<tp_init> const&...> const&>(), a_requested, dango::declval<tp_args>()...) }; })
  {
    auto const a_tuple =
      a_init->*[]<typename... tp_init_args>
      (tp_init_args&&... a_init_args)noexcept->auto
      {
        return dango::make_tuple(dango::forward_as_tuple(dango::forward<tp_init_args>(a_init_args))...);
      };

    auto const a_ref_tuple =
      a_tuple->*[]<typename... tp_tuples>
      (tp_tuples const&... a_tuples)noexcept->auto
      {
        return dango::tie_as_tuple(a_tuples...);
      };

    return allocate_init(a_handle, a_ref_tuple, a_requested, dango::forward<tp_args>(a_args)...);
  }

  template
  <dango::is_same<tp_allocator> tp_alloc = tp_allocator>
  requires(dango::is_nohandle_allocator<tp_alloc>)
  static void
  deallocate
  (super_type const volatile* const a_super)noexcept
  {
    constexpr auto const a_dealloc =
     [](void const volatile* const a_ptr, size_type const a_size, size_type const a_align)noexcept->void
     {
       tp_allocator::dealloc(a_ptr, a_size, a_align);
     };

     dealloc_impl(a_dealloc, const_cast<super_type const*>(a_super));
  }

  template
  <dango::is_same<tp_allocator> tp_alloc = tp_allocator>
  requires(dango::is_handle_based_allocator<tp_alloc>)
  static void
  deallocate
  (typename tp_alloc::handle_type const& a_handle, super_type const volatile* const a_super)noexcept
  {
    dango_assert_nonnull(a_handle);

    auto const a_dealloc =
     [&a_handle](void const volatile* const a_ptr, size_type const a_size, size_type const a_align)noexcept->void
     {
       a_handle->dealloc(a_ptr, a_size, a_align);
     };

    dealloc_impl(a_dealloc, const_cast<super_type const*>(a_super));
  }
public:
  DANGO_UNCONSTRUCTIBLE(allocator)
};

template
<dango::is_class tp_header, dango::usize tp_align, dango::detail::is_container_elem_type<tp_align> tp_first, dango::detail::is_container_elem_type<tp_align>... tp_next>
requires(!dango::is_const_or_volatile<tp_header> && dango::is_pow_two(tp_align))
class
dango::
detail::
flex_array_constexpr<tp_header, tp_align, dango::struct_of_array<tp_first, tp_next...>>
final:
public tp_header
{
private:
  using super_type = tp_header;
public:
  using elem_type = typename tp_first::elem_type_intern_constexpr;
  using ptr_type = elem_type*;
  using tuple_type = dango::tuple<typename tp_next::elem_type_intern_constexpr*...>;
  using size_type = dango::usize;
public:
  struct allocator;
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

template
<dango::is_class tp_header, dango::usize tp_align, dango::detail::is_container_elem_type<tp_align> tp_first, dango::detail::is_container_elem_type<tp_align>... tp_next>
requires(!dango::is_const_or_volatile<tp_header> && dango::is_pow_two(tp_align))
struct
dango::
detail::
flex_array_constexpr<tp_header, tp_align, dango::struct_of_array<tp_first, tp_next...>>::
allocator
final
{
private:
  using outer_index_seq = dango::make_index_seq<size_type(1) + sizeof...(tp_next)>;
private:
  template
  <typename... tp_elems>
  requires(sizeof...(tp_elems) == (size_type(1) + sizeof...(tp_next)))
  struct
  allocate_init_help
  final
  {
  private:
    template
    <typename tp_elem, size_type tp_index, size_type... tp_indices, typename... tp_tuples>
    requires(sizeof...(tp_indices) == sizeof...(tp_tuples))
    static constexpr auto
    inner
    (dango::index_seq<tp_indices...> const, dango::tuple<tp_tuples...> const& a_init, size_type const a_capacity)
    noexcept(requires{ { ( ... , void(tp_elem::construct_constexpr(tp_indices, dango::tuple_get<tp_index>(dango::tuple_get<tp_indices>(a_init))))) }noexcept; })->auto
    requires(requires{ { ( ... , void(tp_elem::construct_constexpr(tp_indices, dango::tuple_get<tp_index>(dango::tuple_get<tp_indices>(a_init))))) }; })
    {
      using intern_type = typename tp_elem::elem_type_intern_constexpr;

      constexpr auto const c_size = sizeof...(tp_tuples);

      constexpr auto const c_deleter =
        [](intern_type* const a_array)noexcept->void
        {
          for(auto a_i = c_size; a_i != size_type(0); --a_i)
          {
            tp_elem::destroy_constexpr(a_array[a_i - size_type(1)]);
          }

          delete[] a_array;
        };

      auto const a_begin =
        new intern_type[a_capacity]{ tp_elem::construct_constexpr(tp_indices, dango::tuple_get<tp_index>(dango::tuple_get<tp_indices>(a_init)))... };

      return dango::auto_ptr{ a_begin, c_deleter };
    }
  public:
    template
    <size_type... tp_indices, dango::is_ref... tp_tuples>
    requires
    (
      (sizeof...(tp_elems) == sizeof...(tp_indices)) &&
      ( ... && dango::is_tuple<dango::remove_cvref<tp_tuples>>) &&
      ( ... && (sizeof...(tp_elems) == dango::tuple_size<dango::remove_cvref<tp_tuples>>))
    )
    static constexpr auto
    outer
    (dango::index_seq<tp_indices...> const, dango::tuple<tp_tuples...> const& a_init, size_type const a_capacity)
    noexcept(requires{ { ( ... , void(inner<tp_elems, tp_indices>(dango::make_index_seq<sizeof...(tp_tuples)>{ }, a_init, a_capacity))) }noexcept; })->auto
    requires(requires{ { ( ... , void(inner<tp_elems, tp_indices>(dango::make_index_seq<sizeof...(tp_tuples)>{ }, a_init, a_capacity))) }; })
    {
      return dango::make_tuple(inner<tp_elems, tp_indices>(dango::make_index_seq<sizeof...(tp_tuples)>{ }, a_init, a_capacity)...);
    }
  public:
    DANGO_UNCONSTRUCTIBLE(allocate_init_help)
  };
public:
  template
  <typename... tp_args>
  requires(dango::is_brace_constructible<super_type, tp_args...>)
  static constexpr auto
  allocate_n
  (size_type const a_init_size, size_type const a_capacity, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<super_type, tp_args...>)->flex_array_constexpr*
  {
    dango_assert(a_init_size <= a_capacity);

    dango::auto_ptr a_array{ new elem_type[a_capacity], dango::array_delete };

    auto a_array_tuple =
      dango::make_tuple(dango::auto_ptr{ new typename tp_next::elem_type_intern_constexpr[a_capacity], dango::array_delete }...);

    auto const a_next =
      a_array_tuple->*[](auto const&... a_ptr)noexcept->tuple_type
      {
        return tuple_type{ a_ptr.get()... };
      };

    auto const a_mem =
      new flex_array_constexpr{ a_array.get(), a_next, a_init_size, a_capacity, dango::forward<tp_args>(a_args)... };

    a_array_tuple->*[](auto&... a_ptr)noexcept->void{ (void(a_ptr.release()) , ... ); };

    a_array.release();

    return a_mem;
  }

  template
  <typename... tp_tuples, typename... tp_args>
  static constexpr auto
  allocate_init
  (dango::tuple<tp_tuples...> const& a_init, size_type const a_capacity, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_brace_constructible<super_type, tp_args...> &&
    requires{ { allocate_init_help<tp_first, tp_next...>::outer(outer_index_seq{ }, a_init, a_capacity) }noexcept; }
  )->flex_array_constexpr*
  requires
  (
    dango::is_brace_constructible<super_type, tp_args...> &&
    requires{ { allocate_init_help<tp_first, tp_next...>::outer(outer_index_seq{ }, a_init, a_capacity) }; }
  )
  {
    constexpr auto const c_size = sizeof...(tp_tuples);

    dango_assert(c_size <= a_capacity);

    constexpr auto const c_next =
      []<size_type... tp_indices>
      (dango::index_seq<tp_indices...> const, auto const& a_tup)noexcept->tuple_type
      {
        return tuple_type{ dango::tuple_get<tp_indices + size_type(1)>(a_tup).get()... };
      };

    auto a_tuple = allocate_init_help<tp_first, tp_next...>::outer(outer_index_seq{ }, a_init, a_capacity);

    auto const a_next = c_next(dango::make_index_seq<sizeof...(tp_next)>{ }, a_tuple);

    auto const a_mem =
      new flex_array_constexpr{ a_tuple.first().get(), a_next, c_size, a_capacity, dango::forward<tp_args>(a_args)... };

    a_tuple->*[](auto&... a_ptr)noexcept->void{ (void(a_ptr.release()) , ... ); };

    return a_mem;
  }

  template
  <typename... tp_init, typename... tp_args>
  requires(sizeof...(tp_next) == size_type(0))
  static auto
  allocate_init_single_array
  (dango::tuple<tp_init...> const& a_init, size_type const a_capacity, tp_args&&... a_args)
  noexcept(requires{ { allocate_init(dango::declval<dango::tuple<dango::tuple<tp_init> const&...> const&>(), a_capacity, dango::declval<tp_args>()...) }noexcept; })->flex_array_constexpr*
  requires(requires{ { allocate_init(dango::declval<dango::tuple<dango::tuple<tp_init> const&...> const&>(), a_capacity, dango::declval<tp_args>()...) }; })
  {
    auto const a_tuple =
      a_init->*[]<typename... tp_init_args>
      (tp_init_args&&... a_init_args)noexcept->auto
      {
        return dango::make_tuple(dango::forward_as_tuple(dango::forward<tp_init_args>(a_init_args))...);
      };

    auto const a_ref_tuple =
      a_tuple->*[]<typename... tp_tuples>
      (tp_tuples const&... a_tuples)noexcept->auto
      {
        return dango::tie_as_tuple(a_tuples...);
      };

    return allocate_init(a_ref_tuple, a_capacity, dango::forward<tp_args>(a_args)...);
  }

  static constexpr void
  deallocate
  (super_type const volatile* const a_super)
  {
    dango_assert_nonnull(a_super);

    delete static_cast<flex_array_constexpr const volatile*>(a_super);
  }
public:
  DANGO_UNCONSTRUCTIBLE(allocator)
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
