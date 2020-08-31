#ifndef DANGO_AUTO_PTR_HPP_INCLUDED
#define DANGO_AUTO_PTR_HPP_INCLUDED

#include "dango-allocator.hpp"
#include "dango-tuple.hpp"

namespace
dango
{
  template
  <typename tp_deleter, typename tp_type>
  concept is_auto_ptr_deleter =
    !dango::is_const_or_volatile<tp_deleter> &&
    (dango::is_object_exclude_array<tp_deleter> || dango::is_func<tp_deleter>) &&
    dango::is_noexcept_destructible<dango::decay<tp_deleter>> &&
    dango::is_object_exclude_array<tp_type> &&
    dango::is_noexcept_callable_ret<void, dango::decay<tp_deleter>&, tp_type* const&> &&
    !dango::is_allocator<tp_deleter>;

  template
  <typename tp_type, typename tp_deleter>
  concept auto_ptr_constraint_spec =
    (dango::is_object_exclude_array<tp_type> || dango::is_void<tp_type>) &&
    (dango::is_auto_ptr_deleter<tp_deleter, tp_type> || dango::is_allocator<tp_deleter>);

  inline constexpr auto const plain_delete =
    []<dango::is_object_exclude_array tp_type>
    (tp_type const volatile* const a_ptr)constexpr noexcept->void
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
  struct auto_ptr_make_tag{ DANGO_TAG_TYPE(auto_ptr_make_tag) };
}

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

  template
  <typename tp_tp_type, typename tp_tp_deleter>
  requires(dango::auto_ptr_constraint_spec<tp_tp_type, tp_tp_deleter>)
  friend class dango::auto_ptr;
private:
  using make_tag = dango::detail::auto_ptr_make_tag;
public:
  using value_type = tp_type;
  using ptr_type = value_type*;
  using ref_type = dango::remove_cv<dango::add_lvalue_ref<value_type>>;
  using allocator_type = tp_deleter;
  using deleter_type = dango::present_if<dango::is_auto_ptr_deleter<tp_deleter, value_type>, dango::decay<tp_deleter>, dango::uint(0)>;
  using allocator_handle_type = dango::allocator_handle_type_or<allocator_type, dango::present_if_type<dango::uint(1)>>;
  using size_type = dango::present_if<dango::is_void<value_type>, dango::usize, dango::uint(2)>;
  using align_type = dango::present_if<dango::is_void<value_type>, dango::usize, dango::uint(3)>;
  using release_type = dango::conditional<dango::is_void<value_type>, dango::tuple<ptr_type, size_type, align_type>, ptr_type>;
private:
  template
  <typename tp_del>
  explicit constexpr
  auto_ptr
  (make_tag const, ptr_type const a_ptr, tp_del&& a_del)
  noexcept(dango::is_noexcept_brace_constructible<deleter_type, tp_del>)
  requires
  (
    !dango::is_void<value_type> &&
    dango::is_auto_ptr_deleter<tp_deleter, value_type> &&
    dango::is_brace_constructible<deleter_type, tp_del>
  ):
  m_ptr{ a_ptr },
  m_deleter{ dango::forward<tp_del>(a_del) },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }

  template
  <typename tp_alloc>
  explicit constexpr
  auto_ptr
  (make_tag const, ptr_type const a_ptr, tp_alloc&& a_alloc_ptr)
  noexcept(dango::is_noexcept_brace_constructible<allocator_handle_type, tp_alloc>)
  requires
  (
    !dango::is_void<value_type> &&
    dango::is_handle_based_allocator<allocator_type> &&
    dango::is_brace_constructible<allocator_handle_type, tp_alloc>
  ):
  m_ptr{ a_ptr },
  m_deleter{ },
  m_alloc_ptr{ dango::forward<tp_alloc>(a_alloc_ptr) },
  m_size{ },
  m_align{ }
  { }

  explicit constexpr
  auto_ptr
  (make_tag const, ptr_type const a_ptr)noexcept
  requires
  (
    !dango::is_void<value_type> &&
    dango::is_nohandle_allocator<allocator_type>
  ):
  m_ptr{ a_ptr },
  m_deleter{ },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }

  template
  <typename tp_alloc>
  explicit constexpr
  auto_ptr
  (
    make_tag const,
    ptr_type const a_ptr,
    tp_alloc&& a_alloc_ptr,
    size_type const a_size,
    align_type const a_align
  )
  noexcept(dango::is_noexcept_brace_constructible<allocator_handle_type, tp_alloc>)
  requires
  (
    dango::is_void<value_type> &&
    dango::is_handle_based_allocator<allocator_type> &&
    dango::is_brace_constructible<allocator_handle_type, tp_alloc>
  ):
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
    ptr_type const a_ptr,
    size_type const a_size,
    align_type const a_align
  )noexcept
  requires
  (
    dango::is_void<value_type> &&
    dango::is_nohandle_allocator<allocator_type>
  ):
  m_ptr{ a_ptr },
  m_deleter{ },
  m_alloc_ptr{ },
  m_size{ a_size },
  m_align{ a_align }
  { }
public:
  constexpr
  auto_ptr
  (dango::null_tag const)
  noexcept(is_noexcept_default_constructible<deleter_type>)
  requires
  (
    dango::is_auto_ptr_deleter<tp_deleter, value_type> &&
    dango::is_default_constructible<deleter_type>
  ):
  m_ptr{ dango::null },
  m_deleter{ },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }

  constexpr
  auto_ptr
  (dango::null_tag const)noexcept
  requires
  (dango::is_handle_based_allocator<allocator_type>):
  m_ptr{ dango::null },
  m_deleter{ },
  m_alloc_ptr{ dango::null },
  m_size{ },
  m_align{ }
  { }

  constexpr
  auto_ptr
  (dango::null_tag const)noexcept
  requires(dango::is_nohandle_allocator<allocator_type>):
  m_ptr{ dango::null },
  m_deleter{ },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }

  explicit constexpr auto_ptr()noexcept = delete;

  explicit constexpr
  auto_ptr()
  noexcept(dango::is_noexcept_default_constructible<deleter_type>)
  requires
  (
    !dango::is_auto_ptr_deleter<tp_deleter, value_type> ||
    dango::is_default_constructible<deleter_type>
  ):
  auto_ptr{ dango::null }
  { }

  constexpr auto_ptr(auto_ptr const&)noexcept = delete;

  constexpr auto_ptr(auto_ptr&&)noexcept = delete;

  constexpr
  auto_ptr
  (auto_ptr&& a_ptr)
  noexcept(dango::is_noexcept_move_constructible<deleter_type>)
  requires
  (
    dango::is_auto_ptr_deleter<tp_deleter, value_type> &&
    dango::is_move_constructible<deleter_type>
  ):
  m_ptr{ dango::move(a_ptr).m_ptr },
  m_deleter{ dango::move(a_ptr).m_deleter },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  {
    a_ptr.m_ptr = dango::null;
  }

  constexpr
  auto_ptr
  (auto_ptr&& a_ptr)noexcept
  requires(dango::is_handle_based_allocator<allocator_type>):
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
  requires(dango::is_nohandle_allocator<allocator_type>):
  m_ptr{ dango::move(a_ptr).m_ptr },
  m_deleter{ },
  m_alloc_ptr{ },
  m_size{ dango::move(a_ptr).m_size },
  m_align{ dango::move(a_ptr).m_align }
  {
    a_ptr.m_ptr = dango::null;
  }
#ifdef DANGO_USING_CLANG // clang currently doesnt have full support for P0848R3 (destructors in particular)
  constexpr
  ~auto_ptr()noexcept
  {
    if(m_ptr)
    {
      if constexpr(dango::is_auto_ptr_deleter<tp_deleter, value_type>)
      {
        m_deleter(dango::as_const(m_ptr));
      }
      else
      {
        if constexpr(dango::is_void<value_type>)
        {
          auto const a_size = m_size;
          auto const a_align = m_align;

          if constexpr(dango::is_handle_based_allocator<allocator_type>)
          {
            m_alloc_ptr->dealloc(dango::as_const(m_ptr), a_size, a_align);
          }
          else
          {
            allocator_type::dealloc(dango::as_const(m_ptr), a_size, a_align);
          }
        }
        else
        {
          constexpr auto const c_size = sizeof(value_type);
          constexpr auto const c_align = alignof(value_type);

          dango::destructor_as<value_type>(m_ptr);

          if constexpr(dango::is_handle_based_allocator<allocator_type>)
          {
            m_alloc_ptr->dealloc(dango::as_const(m_ptr), c_size, c_align);
          }
          else
          {
            allocator_type::dealloc(dango::as_const(m_ptr), c_size, c_align);
          }
        }
      }
    }
  }
#else
  constexpr ~auto_ptr()noexcept = delete;

  constexpr
  ~auto_ptr()noexcept
  requires(dango::is_auto_ptr_deleter<tp_deleter, value_type>)
  {
    static_assert(dango::is_noexcept_destructible<value_type>);

    if(m_ptr)
    {
      m_deleter(dango::as_const(m_ptr));
    }
  }

  constexpr
  ~auto_ptr()noexcept
  requires(dango::is_handle_based_allocator<allocator_type>)
  {
    static_assert(dango::is_void<value_type> || dango::is_noexcept_destructible<value_type>);

    if(m_ptr)
    {
      if constexpr(dango::is_void<value_type>)
      {
        auto const a_size = m_size;
        auto const a_align = m_align;

        m_alloc_ptr->dealloc(dango::as_const(m_ptr), a_size, a_align);
      }
      else
      {
        constexpr auto const c_size = sizeof(value_type);
        constexpr auto const c_align = alignof(value_type);

        dango::destructor_as<value_type>(m_ptr);

        m_alloc_ptr->dealloc(dango::as_const(m_ptr), c_size, c_align);
      }
    }
  }

  constexpr
  ~auto_ptr()noexcept
  requires(dango::is_nohandle_allocator<allocator_type>)
  {
    static_assert(dango::is_void<value_type> || dango::is_noexcept_destructible<value_type>);

    if(m_ptr)
    {
      if constexpr(dango::is_void<value_type>)
      {
        auto const a_size = m_size;
        auto const a_align = m_align;

        allocator_type::dealloc(dango::as_const(m_ptr), a_size, a_align);
      }
      else
      {
        constexpr auto const c_size = sizeof(value_type);
        constexpr auto const c_align = alignof(value_type);

        dango::destructor_as<value_type>(m_ptr);

        allocator_type::dealloc(dango::as_const(m_ptr), c_size, c_align);
      }
    }
  }
#endif
  constexpr auto
  operator =
  (dango::null_tag const)&
  noexcept(dango::is_noexcept_default_constructible<deleter_type> && dango::is_noexcept_swappable<deleter_type&, deleter_type&>)->auto_ptr&
  requires
  (
    !dango::is_auto_ptr_deleter<tp_deleter, value_type> ||
    (dango::is_default_constructible<deleter_type> && dango::is_swappable<deleter_type&, deleter_type&>)
  )
  {
    auto_ptr a_temp{ dango::null };

    dango::swap(*this, a_temp);

    return *this;
  }

  constexpr auto operator = (auto_ptr const&)& noexcept->auto_ptr& = delete;

  constexpr auto operator = (auto_ptr&&)& noexcept->auto_ptr& = delete;

  constexpr auto
  operator =
  (auto_ptr&& a_ptr)&
  noexcept(dango::is_noexcept_move_constructible<deleter_type> && dango::is_noexcept_swappable<deleter_type&, deleter_type&>)->auto_ptr&
  requires
  (
    !dango::is_auto_ptr_deleter<tp_deleter, value_type> ||
    (dango::is_move_constructible<deleter_type> && dango::is_swappable<deleter_type&, deleter_type&>)
  )
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(*this, a_temp);

    return *this;
  }
public:
  template
  <typename tp_val>
  constexpr
  auto_ptr
  (dango::auto_ptr<tp_val, tp_deleter>&& a_ptr)
  noexcept(dango::is_noexcept_move_constructible<deleter_type>)
  requires
  (
    dango::is_auto_ptr_deleter<tp_deleter, value_type> &&
    dango::is_move_constructible<deleter_type> &&
    !dango::is_same<value_type, tp_val> &&
    (dango::is_same_ignore_cv<value_type, tp_val> || (dango::is_base_of<value_type, tp_val> && dango::is_virtual_destructible<value_type>)) &&
    dango::is_constructible<ptr_type, tp_val*&&>
  ):
  m_ptr{ dango::move(a_ptr).m_ptr },
  m_deleter{ dango::move(a_ptr).m_deleter },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  {
    a_ptr.m_ptr = dango::null;
  }

  template
  <typename tp_val>
  constexpr
  auto_ptr
  (dango::auto_ptr<tp_val, tp_deleter>&& a_ptr)noexcept
  requires
  (
    dango::is_handle_based_allocator<allocator_type> &&
    !dango::is_same<value_type, tp_val> &&
    dango::is_same_ignore_cv<value_type, tp_val> &&
    dango::is_constructible<ptr_type, tp_val*&&>
  ):
  m_ptr{ dango::move(a_ptr).m_ptr },
  m_deleter{ },
  m_alloc_ptr{ dango::move(a_ptr).m_alloc_ptr },
  m_size{ dango::move(a_ptr).m_size },
  m_align{ dango::move(a_ptr).m_size }
  {
    a_ptr.m_ptr = dango::null;
  }

  template
  <typename tp_val>
  constexpr
  auto_ptr
  (dango::auto_ptr<tp_val, tp_deleter>&& a_ptr)noexcept
  requires
  (
    dango::is_nohandle_allocator<allocator_type> &&
    !dango::is_same<value_type, tp_val> &&
    dango::is_same_ignore_cv<value_type, tp_val> &&
    dango::is_constructible<ptr_type, tp_val*&&>
  ):
  m_ptr{ dango::move(a_ptr).m_ptr },
  m_deleter{ },
  m_alloc_ptr{ },
  m_size{ dango::move(a_ptr).m_size },
  m_align{ dango::move(a_ptr).m_size }
  {
    a_ptr.m_ptr = dango::null;
  }

  template
  <typename tp_val>
  constexpr auto
  operator =
  (dango::auto_ptr<tp_val, tp_deleter>&& a_ptr)&
  noexcept(dango::is_noexcept_move_constructible<deleter_type> && dango::is_noexcept_swappable<deleter_type&, deleter_type&>)->auto_ptr&
  requires
  (
    dango::is_auto_ptr_deleter<tp_deleter, value_type> &&
    (dango::is_move_constructible<deleter_type> && dango::is_swappable<deleter_type&, deleter_type&>) &&
    !dango::is_same<value_type, tp_val> &&
    (dango::is_same_ignore_cv<value_type, tp_val> || (dango::is_base_of<value_type, tp_val> && dango::is_virtual_destructible<value_type>)) &&
    dango::is_constructible<ptr_type, tp_val*&&> && dango::is_assignable<ptr_type&, tp_val*&&>
  )
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(*this, a_temp);

    return *this;
  }

  template
  <typename tp_val>
  constexpr auto
  operator =
  (dango::auto_ptr<tp_val, tp_deleter>&& a_ptr)& noexcept->auto_ptr&
  requires
  (
    dango::is_allocator<allocator_type> &&
    !dango::is_same<value_type, tp_val> &&
    dango::is_same_ignore_cv<value_type, tp_val> &&
    dango::is_constructible<ptr_type, tp_val*&&> && dango::is_assignable<ptr_type&, tp_val*&&>
  )
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(*this, a_temp);

    return *this;
  }
public:
  constexpr auto get()const noexcept->ptr_type{ return m_ptr; }

  explicit constexpr operator bool()const noexcept{ return !dango::is_null(*this); }

  explicit constexpr operator ptr_type()const noexcept{ return get(); }

  constexpr auto
  operator -> ()const noexcept->ptr_type
  requires(!dango::is_void<value_type>)
  {
    dango_assert_nonull(*this);

    return get();
  }

  constexpr auto
  operator * ()const noexcept->ref_type
  requires(!dango::is_void<value_type>)
  {
    dango_assert_nonnull(*this);

    return *get();
  }

  constexpr auto
  size()const noexcept->size_type
  requires(dango::is_void<value_type>)
  {
    dango_assert_nonnull(*this);

    return m_size;
  }

  constexpr auto
  align()const noexcept->align_type
  requires(dango::is_void<value_type>)
  {
    dango_assert_nonnull(*this);

    return m_align;
  }

  constexpr auto
  release()const noexcept->release_type
  {
    dango_assert_nonnull(*this);

    auto const a_ptr = get();

    m_ptr = dango::null;

    if constexpr(dango::is_handle_based_allocator<allocator_type>)
    {
      m_alloc_ptr = dango::null;
    }

    if constexpr(dango::is_void<value_type>)
    {
      return release_type{ a_ptr, size(), align() };
    }
    else
    {
      return a_ptr;
    }
  }

  constexpr auto
  get_deleter()& noexcept->deleter_type const&
  requires(dango::is_auto_ptr_deleter<tp_deleter, value_type>)
  {
    return m_deleter;
  }
  constexpr auto
  get_deleter()const& noexcept->deleter_type const&
  requires(dango::is_auto_ptr_deleter<tp_deleter, value_type>)
  {
    return m_deleter;
  }
  constexpr auto
  get_deleter()&& noexcept
  requires(dango::is_auto_ptr_deleter<tp_deleter, value_type>) = delete;
  constexpr auto
  get_deleter()const&& noexcept
  requires(dango::is_auto_ptr_deleter<tp_deleter, value_type>) = delete;

  constexpr auto
  get_allocator_handle()& noexcept->allocator_handle_type const&
  requires(dango::is_handle_based_allocator<allocator_type>)
  {
    return m_alloc_ptr;
  }
  constexpr auto
  get_allocator_handle()const& noexcept->allocator_handle_type const&
  requires(dango::is_handle_based_allocator<allocator_type>)
  {
    return m_alloc_ptr;
  }
  constexpr auto
  get_allocator_handle()&& noexcept
  requires(dango::is_handle_based_allocator<allocator_type>) = delete;
  constexpr auto
  get_allocator_handle()const&& noexcept
  requires(dango::is_handle_based_allocator<allocator_type>) = delete;
public:
  constexpr auto dango_operator_is_null()const noexcept->bool{ return dango::is_null(m_ptr); }

  template
  <typename tp_val, typename tp_del>
  constexpr auto
  dango_operator_equals
  (dango::auto_ptr<tp_val, tp_del> const& a_ptr)const noexcept->bool
  requires(dango::is_noexcept_equatable<ptr_type const&, tp_val* const&>)
  {
    return dango::equals(m_ptr, a_ptr.m_ptr);
  }

  template
  <typename tp_val, typename tp_del>
  constexpr auto
  dango_operator_compare
  (dango::auto_ptr<tp_val, tp_del> const& a_ptr)const noexcept->auto
  requires(dango::is_noexcept_comparable<ptr_type const&, tp_val* const&>)
  {
    return dango::compare(m_ptr, a_ptr.m_ptr);
  }

  auto dango_operator_hash()const noexcept->dango::hash_val{ return dango::hash(m_ptr); }

  template
  <typename tp_val>
  constexpr void
  dango_operator_swap
  (dango::auto_ptr<tp_val, tp_deleter>& a_ptr)&
  noexcept(dango::is_noexcept_swappable<deleter_type&, deleter_type&>)
  requires
  (
    !dango::is_auto_ptr_deleter<tp_deleter, value_type> ||
    (dango::is_swappable<ptr_type&, tp_val*&> && dango::is_swappable<deleter_type&, deleter_type&>)
  )
  {
    dango::swap(m_ptr, a_ptr.m_ptr);

    if constexpr(dango::is_auto_ptr_deleter<tp_deleter, value_type>)
    {
      dango::swap(m_deleter, a_ptr.m_deleter);
    }

    if constexpr(dango::is_handle_based_allocator<allocator_type>)
    {
      dango::swap(m_alloc_ptr, a_ptr.m_alloc_ptr);
    }

    if constexpr(dango::is_void<value_type>)
    {
      dango::swap(m_size, a_ptr.m_size);
      dango::swap(m_align, a_ptr.m_align);
    }
  }
private:
  ptr_type m_ptr;
  deleter_type m_deleter [[no_unique_address]];
  allocator_handle_type m_alloc_ptr [[no_unique_address]];
  size_type m_size [[no_unique_address]];
  align_type m_align [[no_unique_address]];
};

struct
dango::
detail::
auto_ptr_make
final
{
private:
  using make_tag = dango::detail::auto_ptr_make_tag;
public:
  template
  <typename tp_type, typename tp_deleter, typename tp_del, typename... tp_args>
  requires
  (
    dango::is_object_exclude_array<tp_type> &&
    dango::is_auto_ptr_deleter<tp_deleter, tp_type> &&
    dango::is_noexcept_destructible<tp_type> &&
    requires(tp_type* const a_ptr)
    {
      { new tp_type{ dango::declval<tp_args>()... } };
      { dango::auto_ptr<tp_type, tp_deleter>{ make_tag{ }, a_ptr, dango::declval<tp_del>() } };
    }
  )
  static constexpr auto
  make
  (dango::priority_tag<dango::uint(4)> const, tp_del&& a_del, tp_args&&... a_args)
  noexcept
  (
    requires(tp_type* const a_ptr)
    {
      { new tp_type{ dango::declval<tp_args>()... } }noexcept;
      { dango::auto_ptr<tp_type, tp_deleter>{ make_tag{ }, a_ptr, dango::declval<tp_del>() } }noexcept;
    }
  )->auto
  {
    auto const a_ptr = new tp_type{ dango::forward<tp_args>(a_args)... };

    return dango::auto_ptr<tp_type, tp_deleter>{ make_tag{ }, a_ptr, dango::forward<tp_del>(a_del) };
  }

  template
  <typename tp_type, typename tp_allocator, typename tp_alloc, typename... tp_args>
  requires
  (
    dango::is_object_exclude_array<tp_type> &&
    dango::is_handle_based_allocator<tp_allocator> &&
    dango::is_brace_constructible<typename tp_allocator::handle_type, tp_alloc> &&
    dango::is_brace_constructible<tp_type, tp_args...> &&
    dango::is_noexcept_destructible<tp_type> &&
    requires(tp_type* const a_ptr)
    { { dango::auto_ptr<tp_type, tp_allocator>{ make_tag{ }, a_ptr, dango::declval<typename tp_allocator::handle_type&&>() } }; }
  )
  static constexpr auto
  make
  (dango::priority_tag<dango::uint(3)> const, tp_alloc&& a_alloc, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_handle_based_allocator<tp_allocator> &&
    dango::is_noexcept_brace_constructible<typename tp_allocator::handle_type, tp_alloc> &&
    dango::is_noexcept_brace_constructible<tp_type, tp_args...> &&
    requires(tp_type* const a_ptr)
    { { dango::auto_ptr<tp_type, tp_allocator>{ make_tag{ }, a_ptr, dango::declval<typename tp_allocator::handle_type&&>() } }noexcept; }
  )->auto
  {
    typename tp_allocator::handle_type a_alloc_ptr{ dango::forward<tp_alloc>(a_alloc) };

    auto const a_addr = a_alloc_ptr->alloc(sizeof(tp_type), alignof(tp_type));

    auto a_guard =
      dango::make_guard([&a_alloc_ptr, a_addr]()noexcept->void{ a_alloc_ptr->dealloc(a_addr, sizeof(tp_type), alignof(tp_type)); });

    auto const a_ptr = dango_placement_new(a_addr, tp_type, { dango::forward<tp_args>(a_args)... });

    a_guard.dismiss();

    return dango::auto_ptr<tp_type, tp_allocator>{ make_tag{ }, a_ptr, dango::move(a_alloc_ptr) };
  }

  template
  <typename tp_type, typename tp_allocator, typename... tp_args>
  requires
  (
    dango::is_object_exclude_array<tp_type> &&
    dango::is_nohandle_allocator<tp_allocator> &&
    dango::is_brace_constructible<tp_type, tp_args...> &&
    dango::is_noexcept_destructible<tp_type> &&
    requires(tp_type* const a_ptr)
    { { dango::auto_ptr<tp_type, tp_allocator>{ make_tag{ }, a_ptr } }; }
  )
  static constexpr auto
  make
  (dango::priority_tag<dango::uint(2)> const, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_nohandle_allocator<tp_allocator> &&
    dango::is_noexcept_brace_constructible<tp_type, tp_args...> &&
    requires(tp_type* const a_ptr)
    { { dango::auto_ptr<tp_type, tp_allocator>{ make_tag{ }, a_ptr } }noexcept; }
  )->auto
  {
    auto const a_addr = tp_allocator::alloc(sizeof(tp_type), alignof(tp_type));

    auto a_guard =
      dango::make_guard([a_addr]()noexcept->void{ tp_allocator::dealloc(a_addr, sizeof(tp_type), alignof(tp_type)); });

    auto const a_ptr = dango_placement_new(a_addr, tp_type, { dango::forward<tp_args>(a_args)... });

    a_guard.dismiss();

    return dango::auto_ptr<tp_type, tp_allocator>{ make_tag{ }, a_ptr };
  }

  template
  <typename tp_type, typename tp_allocator, typename tp_alloc>
  requires
  (
    dango::is_void<tp_type> &&
    dango::is_handle_based_allocator<tp_allocator> &&
    dango::is_brace_constructible<typename tp_allocator::handle_type, tp_alloc> &&
    requires(tp_type* const a_ptr, dango::usize const a_usize)
    { { dango::auto_ptr<tp_type, tp_allocator>{ make_tag{ }, a_ptr, dango::declval<typename tp_allocator::handle_type&&>(), a_usize, a_usize } }; }
  )
  static constexpr auto
  make
  (dango::priority_tag<dango::uint(1)> const, tp_alloc&& a_alloc, dango::usize const a_size, dango::usize const a_align)
  noexcept
  (
    dango::is_noexcept_handle_based_allocator<tp_allocator> &&
    dango::is_noexcept_brace_constructible<typename tp_allocator::handle_type, tp_alloc> &&
    requires(tp_type* const a_ptr, dango::usize const a_usize)
    { { dango::auto_ptr<tp_type, tp_allocator>{ make_tag{ }, a_ptr, dango::declval<typename tp_allocator::handle_type&&>(), a_usize, a_usize } }noexcept; }
  )->auto
  {
    typename tp_allocator::handle_type a_alloc_ptr{ dango::forward<tp_alloc>(a_alloc) };

    auto const a_ptr = a_alloc_ptr->alloc(a_size, a_align);

    return dango::auto_ptr<tp_type, tp_allocator>{ make_tag{ }, a_ptr, dango::move(a_alloc_ptr), a_size, a_align };
  }

  template
  <typename tp_type, typename tp_allocator>
  requires
  (
    dango::is_void<tp_type> &&
    dango::is_nohandle_allocator<tp_allocator> &&
    requires(tp_type* const a_ptr, dango::usize const a_usize)
    { { dango::auto_ptr<tp_type, tp_allocator>{ make_tag{ }, a_ptr, a_usize, a_usize } }; }
  )
  static constexpr auto
  make
  (dango::priority_tag<dango::uint(0)> const, dango::usize const a_size, dango::usize const a_align)
  noexcept
  (
    dango::is_noexcept_nohandle_allocator<tp_allocator> &&
    requires(tp_type* const a_ptr, dango::usize const a_usize)
    { { dango::auto_ptr<tp_type, tp_allocator>{ make_tag{ }, a_ptr, a_usize, a_usize } }noexcept; }
  )->auto
  {
    auto const a_ptr = tp_allocator::alloc(a_size, a_align);

    return dango::auto_ptr<tp_type, tp_allocator>{ make_tag{ }, a_ptr, a_size, a_align };
  }
public:
  DANGO_UNCONSTRUCTIBLE(auto_ptr_make)
};

#endif // DANGO_AUTO_PTR_HPP_INCLUDED
