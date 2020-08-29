#ifndef DANGO_AUTO_PTR_HPP_INCLUDED
#define DANGO_AUTO_PTR_HPP_INCLUDED

#include "dango-allocator.hpp"

namespace
dango
{
  template
  <typename tp_deleter, typename tp_type>
  concept is_auto_ptr_deleter =
    (dango::is_object_exclude_array<tp_deleter> || dango::is_func<tp_deleter>) &&
    !dango::is_const_or_volatile<tp_deleter> && dango::is_object_exclude_array<tp_type> &&
    dango::is_noexcept_callable_ret<void, dango::decay<tp_deleter>&, tp_type* const&> &&
    !dango::is_allocator<tp_deleter>;

  template
  <typename tp_type, typename tp_deleter>
  concept auto_ptr_constraint_spec =
    (dango::is_object_exclude_array<tp_type> || dango::is_void<tp_type>) &&
    (dango::is_auto_ptr_deleter<tp_deleter, tp_type> || dango::is_allocator<tp_deleter>);

  inline constexpr auto const plain_delete =
    []<dango::is_object_exclude_array tp_type>
    (tp_type* const a_ptr)noexcept->void
    {
      delete a_ptr;
    };

  using plain_delete_type =
    dango::remove_const<decltype(dango::plain_delete)>;

  template
  <typename tp_type>
  using auto_ptr_default_deleter =
    dango::conditional<dango::is_void<tp_type>, dango::basic_allocator, dango::plain_delete_type>;
}

namespace
dango::detail
{
  struct auto_ptr_make;
}

struct
dango::
detail::
auto_ptr_make
final
{
public:
  struct make_tag{ DANGO_TAG_TYPE(make_tag) };
public:

public:
  DANGO_UNCONSTRUCTIBLE(auto_ptr_make)
};

namespace
dango
{
  template
  <typename tp_type, typename tp_deleter = dango::auto_ptr_default_deleter<tp_type>>
  requires(dango::auto_ptr_constraint_spec<tp_type, tp_deleter>)
  class auto_ptr;
}

template
<typename tp_type, typename tp_deleter>
requires(dango::auto_ptr_constraint_spec<tp_type, tp_deleter>)
class
dango::
auto_ptr
{
  friend dango::detail::auto_ptr_make;
private:
  using make_tag = dango::detail::auto_ptr_make::make_tag;
public:
  using value_type = tp_type;
  using ptr_type = value_type*;
  using ref_type = value_type&;
  using allocator_type = tp_deleter;
  using deleter_type = dango::present_if<dango::is_auto_ptr_deleter<tp_deleter, value_type>, dango::decay<tp_deleter>, dango::uint(0)>;
  using allocator_handle_type = dango::allocator_handle_type_or<allocator_type, dango::present_if_type<dango::uint(1)>>;
  using size_type = dango::present_if<dango::is_void<value_type>, dango::usize, dango::uint(2)>;
  using align_type = dango::present_if<dango::is_void<value_type>, dango::usize, dango::uint(3)>;
private:
  template
  <typename tp_del>
  requires
  (
    !dango::is_void<value_type> &&
    dango::is_auto_ptr_deleter<tp_deleter, value_type> &&
    dango::is_noexcept_constructible<deleter_type, tp_del>
  )
  explicit constexpr
  auto_ptr
  (make_tag const, value_type const a_ptr, tp_del&& a_del)noexcept:
  m_ptr{ a_ptr },
  m_deleter{ dango::forward<tp_del>(a_del) },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }

  template
  <typename tp_alloc>
  requires
  (
    !dango::is_void<value_type> &&
    dango::is_handle_based_allocator<tp_deleter> &&
    dango::is_noexcept_constructible<allocator_handle_type, tp_alloc>
  )
  explicit constexpr
  auto_ptr
  (make_tag const, value_type const a_ptr, tp_alloc&& a_alloc_ptr)noexcept:
  m_ptr{ a_ptr },
  m_deleter{ },
  m_alloc_ptr{ dango::forward<tp_alloc>(a_alloc_ptr) },
  m_size{ },
  m_align{ }
  { }

  explicit constexpr
  auto_ptr
  (make_tag const, value_type const a_ptr)noexcept
  requires(!dango::is_void<value_type> && dango::is_nohandle_allocator<tp_deleter>):
  m_ptr{ a_ptr },
  m_deleter{ },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }

  template
  <typename tp_alloc>
  requires
  (
    dango::is_void<value_type> &&
    dango::is_handle_based_allocator<tp_deleter> &&
    dango::is_noexcept_constructible<allocator_handle_type, tp_alloc>
  )
  explicit constexpr
  auto_ptr
  (
    make_tag const,
    value_type const a_ptr,
    tp_alloc&& a_alloc_ptr,
    size_type const a_size,
    align_type const a_align
  )noexcept:
  m_ptr{ a_ptr },
  m_deleter{ },
  m_alloc_ptr{ dango::forward<tp_alloc>(a_alloc_ptr) },
  m_size{ a_size },
  m_align{ a_align }
  { }

  explicit constexpr
  auto_ptr
  (
    make_tag const,
    value_type const a_ptr,
    size_type const a_size,
    align_type const a_align
  )noexcept
  requires(dango::is_void<value_type> && dango::is_nohandle_allocator<tp_deleter>):
  m_ptr{ a_ptr },
  m_deleter{ },
  m_alloc_ptr{ },
  m_size{ a_size },
  m_align{ a_align }
  { }
public:
  explicit constexpr auto_ptr()noexcept = delete;

  explicit constexpr
  auto_ptr()noexcept
  requires(dango::is_auto_ptr_deleter<tp_deleter, value_type> && dango::is_noexcept_default_constructible<deleter_type>):
  m_ptr{ dango::null },
  m_deleter{ },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }

  explicit constexpr
  auto_ptr()noexcept
  requires(dango::is_handle_based_allocator<tp_deleter>):
  m_ptr{ dango::null },
  m_deleter{ },
  m_alloc_ptr{ dango::null },
  m_size{ },
  m_align{ }
  { }

  explicit constexpr
  auto_ptr()noexcept
  requires(dango::is_nohandle_allocator<tp_deleter>):
  m_ptr{ dango::null },
  m_deleter{ },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }

  explicit constexpr
  auto_ptr
  (dango::null_tag const)noexcept
  requires(dango::is_auto_ptr_deleter<tp_deleter, value_type> && dango::is_noexcept_default_constructible<deleter_type>):
  m_ptr{ dango::null },
  m_deleter{ },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }

  explicit constexpr
  auto_ptr
  (dango::null_tag const)noexcept
  requires(dango::is_handle_based_allocator<tp_deleter>):
  m_ptr{ dango::null },
  m_deleter{ },
  m_alloc_ptr{ dango::null },
  m_size{ },
  m_align{ }
  { }

  explicit constexpr
  auto_ptr
  (dango::null_tag const)noexcept
  requires(dango::is_nohandle_allocator<tp_deleter>):
  m_ptr{ dango::null },
  m_deleter{ },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }

  constexpr auto_ptr(auto_ptr const&)noexcept = delete;

  constexpr auto_ptr(auto_ptr&&)noexcept = delete;

  constexpr
  auto_ptr
  (auto_ptr&& a_ptr)noexcept
  requires(dango::is_auto_ptr_deleter<tp_deleter, value_type> && dango::is_noexcept_move_constructible<deleter_type>):
  m_ptr{ dango::move(a_ptr).m_ptr },
  m_deleter{ dango::move(a_ptr).m_deleter },
  m_alloc_ptr{ },
  m_size{ dango::move(a_ptr).m_size },
  m_align{ dango::move(a_ptr).m_align }
  {
    a_ptr.m_ptr = dango::null;
  }

  constexpr
  auto_ptr
  (auto_ptr&& a_ptr)noexcept
  requires(dango::is_handle_based_allocator<tp_deleter>):
  m_ptr{ dango::move(a_ptr).m_ptr },
  m_deleter{ },
  m_alloc_ptr{ dango::move(a_ptr).m_alloc_ptr },
  m_size{ dango::move(a_ptr).m_size },
  m_align{ dango::move(a_ptr).m_align }
  {
    a_ptr.m_ptr = dango::null;
  }

  constexpr
  auto_ptr
  (auto_ptr&& a_ptr)noexcept
  requires(dango::is_nohandle_allocator<tp_deleter>):
  m_ptr{ dango::move(a_ptr).m_ptr },
  m_deleter{ },
  m_alloc_ptr{ },
  m_size{ dango::move(a_ptr).m_size },
  m_align{ dango::move(a_ptr).m_align }
  {
    a_ptr.m_ptr = dango::null;
  }

  constexpr
  ~auto_ptr
  ()noexcept
  {
    auto const a_ptr = m_ptr;

    if(a_ptr)
    {
      if constexpr(dango::is_auto_ptr_deleter<tp_deleter, value_type>)
      {
        m_deleter(a_ptr);
      }
      else
      {
        if constexpr(dango::is_void<value_type>)
        {
          auto const a_size = m_size;
          auto const a_align = m_align;

          if constexpr(dango::is_handle_based_allocator<tp_deleter>)
          {
            m_alloc_ptr->delloc(a_ptr, a_size, a_align);
          }
          else
          {
            allocator_type::dealloc(a_ptr, a_size, a_align);
          }
        }
        else
        {
          dango::destructor(a_ptr);

          if constexpr(dango::is_handle_based_allocator<tp_deleter>)
          {
            m_alloc_ptr->dealloc(a_ptr, sizeof(value_type), alignof(value_type));
          }
          else
          {
            allocator_type::dealloc(a_ptr, sizeof(value_type), alignof(value_type));
          }
        }
      }
    }
  }
private:
  ptr_type m_ptr;
  deleter_type m_deleter [[no_unique_address]];
  allocator_handle_type m_alloc_ptr [[no_unique_address]];
  size_type m_size [[no_unique_address]];
  align_type m_align [[no_unique_address]];
};

#endif // DANGO_AUTO_PTR_HPP_INCLUDED
