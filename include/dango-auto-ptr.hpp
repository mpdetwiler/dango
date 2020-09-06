#ifndef DANGO_AUTO_PTR_HPP_INCLUDED
#define DANGO_AUTO_PTR_HPP_INCLUDED

#include "dango-allocator.hpp"
#include "dango-tuple.hpp"

namespace
dango
{
  template
  <typename tp_deleter, typename tp_type>
  concept is_deleter =
    !dango::is_const_or_volatile<tp_deleter> &&
    (dango::is_object_exclude_array<tp_deleter> || dango::is_func<tp_deleter>) &&
    (!dango::is_move_constructible<dango::decay<tp_deleter>> || dango::is_noexcept_move_constructible<dango::decay<tp_deleter>>) &&
    (!dango::is_swappable<dango::decay<tp_deleter>&> || dango::is_noexcept_swappable<dango::decay<tp_deleter>&>) &&
    dango::is_object_exclude_array<tp_type> &&
    dango::is_noexcept_callable_ret<void, dango::decay<tp_deleter>&, tp_type* const&> &&
    !dango::is_allocator<tp_deleter>;

  template
  <typename tp_type, typename tp_deleter>
  concept auto_ptr_constraint_spec =
    (dango::is_object_exclude_array<tp_type> || dango::is_void<tp_type>) &&
    (dango::is_deleter<tp_deleter, tp_type> || dango::is_allocator<tp_deleter>);

  inline constexpr auto const plain_delete =
    []<dango::is_object_exclude_array tp_type>
    (tp_type const volatile* const a_ptr)constexpr noexcept->void
    {
      static_assert(requires{ { delete a_ptr }noexcept; });

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
dango::custom
{
  template
  <typename tp_deleter>
  struct deleter_traits;

  template
  <dango::is_noexcept_default_constructible tp_deleter>
  struct
  deleter_traits<tp_deleter>
  {
    static inline constexpr bool const polymorphic_requires_virtual_destruct = true;

    static constexpr auto get_default_instance()noexcept->tp_deleter{ return tp_deleter{ }; }
    static constexpr auto get_null_instance()noexcept->tp_deleter{ return tp_deleter{ }; }

    DANGO_UNCONSTRUCTIBLE(deleter_traits)
  };

  template
  <typename tp_ret, typename tp_type>
  struct
  deleter_traits<tp_ret(*)(tp_type*)noexcept>
  {
    using type = tp_ret(*)(tp_type*)noexcept;

    static inline constexpr bool const polymorphic_requires_virtual_destruct = true;

    static constexpr auto get_null_instance()noexcept->type{ return dango::null; }

    DANGO_UNCONSTRUCTIBLE(deleter_traits)
  };

  template<>
  struct
  deleter_traits<dango::plain_delete_type>
  final
  {
    static inline constexpr bool const polymorphic_requires_virtual_destruct = true;

    static constexpr auto get_default_instance()noexcept->auto const&{ return dango::plain_delete; }
    static constexpr auto get_null_instance()noexcept->auto const&{ return dango::plain_delete; }

    DANGO_UNCONSTRUCTIBLE(deleter_traits)
  };
}

namespace
dango::detail
{
  template
  <typename tp_deleter>
  concept auto_ptr_deleter_prvd_check =
    requires{ { dango::constexpr_check<bool, dango::custom::deleter_traits<tp_deleter>::polymorphic_requires_virtual_destruct>{ } }noexcept; };

  template
  <typename tp_deleter>
  concept auto_ptr_deleter_prvd_value =
    dango::detail::auto_ptr_deleter_prvd_check<tp_deleter> &&
    bool(dango::custom::deleter_traits<tp_deleter>::polymorphic_requires_virtual_destruct);

  template
  <typename tp_deleter, typename tp_lhs, typename tp_rhs>
#ifdef DANGO_USING_CLANG
  inline constexpr bool const auto_ptr_deleter_prvd =
#else
  concept auto_ptr_deleter_prvd =
#endif
    dango::is_same_ignore_cv<tp_lhs, tp_rhs> ||
    !dango::detail::auto_ptr_deleter_prvd_value<tp_deleter> ||
    (dango::is_base_of<tp_lhs, tp_rhs> && dango::is_virtual_destructible<tp_lhs>);

  template
  <typename tp_deleter>
  concept auto_ptr_deleter_default_instance_check =
    requires{ { dango::custom::deleter_traits<tp_deleter>::get_default_instance() }noexcept->dango::is_convertible_ret<tp_deleter>; };

  template
  <typename tp_deleter>
  concept auto_ptr_deleter_null_instance_check =
    requires{ { dango::custom::deleter_traits<tp_deleter>::get_null_instance() }noexcept->dango::is_convertible_ret<tp_deleter>; };

  template
  <typename tp_lhs, typename tp_rhs>
#ifdef DANGO_USING_CLANG
  inline constexpr bool const auto_ptr_valid_conversion =
#else
  concept auto_ptr_valid_conversion =
#endif
    (dango::is_same_ignore_cv<tp_lhs, tp_rhs> || dango::is_base_of<tp_lhs, tp_rhs>) &&
    dango::is_noexcept_destructible<tp_lhs> &&
    dango::is_noexcept_brace_constructible<tp_lhs*, tp_rhs* const&> &&
    dango::is_noexcept_brace_constructible<tp_lhs*, tp_rhs*&&> &&
    dango::is_noexcept_convertible<tp_rhs* const&, tp_lhs*> &&
    dango::is_noexcept_convertible<tp_rhs*&&, tp_lhs*>;

    template
    <typename tp_lhs, typename tp_rhs>
  #ifdef DANGO_USING_CLANG
    inline constexpr bool const auto_ptr_valid_conversion_void =
  #else
    concept auto_ptr_valid_conversion_void =
  #endif
      dango::is_void<tp_rhs> &&
      dango::is_noexcept_brace_constructible<tp_lhs*, tp_rhs* const&> &&
      dango::is_noexcept_brace_constructible<tp_lhs*, tp_rhs*&&> &&
      dango::is_noexcept_convertible<tp_rhs* const&, tp_lhs*> &&
      dango::is_noexcept_convertible<tp_rhs*&&, tp_lhs*>;

  struct auto_ptr_make;

  struct
  auto_ptr_make_tag
  final
  {
    DANGO_TAG_TYPE(auto_ptr_make_tag)
  };

  using auto_ptr_make_prio = dango::priority_tag<dango::uint(4)>;

  class auto_ptr_control;

  template
  <dango::is_noexcept_destructible tp_type, dango::is_nohandle_allocator tp_alloc>
  class auto_ptr_control_nohandle;

  template
  <dango::is_noexcept_destructible tp_type, dango::is_handle_based_allocator tp_alloc>
  class auto_ptr_control_handle_based;
}

class
dango::
detail::
auto_ptr_control
{
public:
  using delete_func = void(*)(auto_ptr_control*)noexcept;
protected:
  explicit constexpr
  auto_ptr_control
  (delete_func const a_delete_func, void const* const a_handle)noexcept:
  m_delete_func{ a_delete_func },
  m_handle{ a_handle }
  { }
  constexpr ~auto_ptr_control()noexcept = default;
public:
  constexpr auto get_delete_func()const noexcept->delete_func{ return m_delete_func; }
  constexpr auto get_handle()const noexcept->void const*{ return m_handle; }
private:
  delete_func const m_delete_func;
  void const* const m_handle;
public:
  DANGO_DELETE_DEFAULT(auto_ptr_control)
  DANGO_UNMOVEABLE(auto_ptr_control)
};

template
<dango::is_noexcept_destructible tp_type, dango::is_nohandle_allocator tp_alloc>
class
dango::
detail::
auto_ptr_control_nohandle
final:
public dango::detail::auto_ptr_control
{
public:
  using super_type = dango::detail::auto_ptr_control;
  using delete_func = super_type::delete_func;
private:
  static void
  delete_this
  (super_type* const a_super)noexcept
  {
    using this_type = auto_ptr_control_nohandle;
    constexpr auto const c_size = sizeof(this_type);
    constexpr auto const c_align = alignof(this_type);

    auto const a_this = static_cast<this_type*>(a_super);

    dango::destructor_as<this_type>(a_this);

    tp_alloc::dealloc(a_this, c_size, c_align);
  }
public:
  template
  <typename... tp_args>
  requires(dango::is_brace_constructible<tp_type, tp_args...>)
  explicit constexpr
  auto_ptr_control_nohandle
  (tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<tp_type, tp_args...>):
  super_type{ &delete_this, dango::null },
  m_data{ dango::forward<tp_args>(a_args)... }
  { }
  constexpr ~auto_ptr_control_nohandle()noexcept = default;
public:
  constexpr auto get_object_ptr()noexcept->auto{ return &m_data; }
private:
  tp_type m_data;
public:
  DANGO_UNMOVEABLE(auto_ptr_control_nohandle)
};

template
<dango::is_noexcept_destructible tp_type, dango::is_handle_based_allocator tp_alloc>
class
dango::
detail::
auto_ptr_control_handle_based
final:
public dango::detail::auto_ptr_control
{
public:
  using super_type = dango::detail::auto_ptr_control;
  using delete_func = super_type::delete_func;
  using allocator_handle_type = typename tp_alloc::handle_type;
private:
  static void
  delete_this
  (super_type* const a_super)noexcept
  {
    using this_type = auto_ptr_control_handle_based;
    constexpr auto const c_size = sizeof(this_type);
    constexpr auto const c_align = alignof(this_type);

    auto const a_this = static_cast<this_type*>(a_super);

    auto const a_alloc_ptr = dango::move(a_this->m_alloc_ptr);

    dango::destructor_as<this_type>(a_this);

    a_alloc_ptr->dealloc(a_this, c_size, c_align);
  }
public:
  template
  <typename... tp_args>
  requires(dango::is_brace_constructible<tp_type, tp_args...>)
  explicit constexpr
  auto_ptr_control_handle_based
  (allocator_handle_type&& a_alloc, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<tp_type, tp_args...>):
  super_type{ &delete_this, &m_alloc_ptr },
  m_alloc_ptr{ dango::move(a_alloc) },
  m_data{ dango::forward<tp_args>(a_args)... }
  { }
  constexpr ~auto_ptr_control_handle_based()noexcept = default;
public:
  constexpr auto get_object_ptr()noexcept->auto{ return &m_data; }
private:
  allocator_handle_type m_alloc_ptr;
  tp_type m_data;
public:
  DANGO_DELETE_DEFAULT(auto_ptr_control_handle_based)
  DANGO_UNMOVEABLE(auto_ptr_control_handle_based)
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

  template
  <typename tp_tp_type, typename tp_tp_deleter>
  requires(dango::auto_ptr_constraint_spec<tp_tp_type, tp_tp_deleter>)
  friend class dango::auto_ptr;
private:
  using make_tag = dango::detail::auto_ptr_make_tag;
public:
  using value_type = tp_type;
  using ptr_type = value_type*;
  using ref_type = dango::conditional<dango::is_void<value_type>, void, dango::add_lvalue_ref<value_type>>;
  using size_type = dango::usize;
  using deleter_type = dango::conditional<dango::is_deleter<tp_deleter, value_type>, dango::decay<tp_deleter>, void>;
  using allocator_type = dango::conditional<dango::is_allocator<tp_deleter>, tp_deleter, void>;
  using allocator_handle_type = dango::allocator_handle_type_or<allocator_type, void>;
  using release_type = dango::conditional<dango::is_void<value_type>, dango::tuple<ptr_type, size_type, size_type>, ptr_type>;
private:
  using deleter_member_type = dango::present_if<!dango::is_void<deleter_type>, deleter_type, dango::uint(0)>;
  using control_member_type = dango::present_if<!dango::is_void<value_type> && !dango::is_void<allocator_type>, dango::detail::auto_ptr_control*, dango::uint(1)>;
  using allocator_handle_member_type = dango::present_if<dango::is_void<value_type> && !dango::is_void<allocator_handle_type>, allocator_handle_type, dango::uint(2)>;
  using size_member_type = dango::present_if<dango::is_void<value_type>, size_type, dango::uint(3)>;
  using align_member_type = dango::present_if<dango::is_void<value_type>, size_type, dango::uint(4)>;
  using deleter_ref_type = dango::add_lvalue_ref<deleter_type>;
private:
  template
  <typename tp_del>
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<deleter_type>
  )
  explicit constexpr
  auto_ptr
  (make_tag const, ptr_type const a_ptr, tp_del&& a_del)noexcept
  requires(dango::is_noexcept_brace_constructible<deleter_type, tp_del>):
  m_ptr{ a_ptr },
  m_deleter{ dango::forward<tp_del>(a_del) },
  m_control{ },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }

  explicit constexpr
  auto_ptr
  (make_tag const, ptr_type const a_ptr, control_member_type const a_control)noexcept
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<allocator_type>
  ):
  m_ptr{ a_ptr },
  m_deleter{ },
  m_control{ a_control },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }

  template
  <typename tp_alloc>
  requires
  (
    dango::is_void<value_type> &&
    !dango::is_void<allocator_type> &&
    !dango::is_void<allocator_handle_type>
  )
  explicit constexpr
  auto_ptr
  (
    make_tag const,
    ptr_type const a_ptr,
    tp_alloc&& a_alloc_ptr,
    dango::usize const a_size,
    dango::usize const a_align
  )
  noexcept
  requires(dango::is_noexcept_brace_constructible<allocator_handle_type, tp_alloc>):
  m_ptr{ a_ptr },
  m_deleter{ },
  m_control{ },
  m_alloc_ptr{ dango::forward<tp_alloc>(a_alloc_ptr) },
  m_size{ a_size },
  m_align{ a_align }
  { }

  explicit constexpr
  auto_ptr
  (
    make_tag const,
    ptr_type const a_ptr,
    dango::usize const a_size,
    dango::usize const a_align
  )
  noexcept
  requires
  (
    dango::is_void<value_type> &&
    !dango::is_void<allocator_type> &&
    dango::is_void<allocator_handle_type>
  ):
  m_ptr{ a_ptr },
  m_deleter{ },
  m_control{ },
  m_alloc_ptr{ },
  m_size{ a_size },
  m_align{ a_align }
  { }
public:
  template
  <typename tp_val>
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<deleter_type> &&
    dango::detail::auto_ptr_deleter_prvd_check<deleter_type> &&
    dango::detail::auto_ptr_deleter_default_instance_check<deleter_type>
  )
  explicit constexpr
  auto_ptr
  (tp_val* const a_ptr)noexcept
  requires
  (
    dango::detail::auto_ptr_valid_conversion<value_type, tp_val> &&
    dango::detail::auto_ptr_deleter_prvd<deleter_type, value_type, tp_val>
  ):
  m_ptr{ a_ptr },
  m_deleter{ dango::custom::deleter_traits<deleter_type>::get_default_instance() },
  m_control{ },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }

  template
  <typename tp_val, typename tp_del>
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<deleter_type> &&
    dango::detail::auto_ptr_deleter_prvd_check<deleter_type>
  )
  explicit constexpr
  auto_ptr
  (tp_val* const a_ptr, tp_del&& a_deleter)noexcept
  requires
  (
    dango::detail::auto_ptr_valid_conversion<value_type, tp_val> &&
    dango::is_noexcept_brace_constructible<deleter_type, tp_del> &&
    dango::detail::auto_ptr_deleter_prvd<deleter_type, value_type, tp_val>
  ):
  m_ptr{ a_ptr },
  m_deleter{ dango::forward<tp_del>(a_deleter) },
  m_control{ },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }
public:
  constexpr
  auto_ptr
  (dango::null_tag const)noexcept
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<deleter_type> &&
    dango::detail::auto_ptr_deleter_null_instance_check<deleter_type>
  ):
  m_ptr{ dango::null },
  m_deleter{ dango::custom::deleter_traits<deleter_type>::get_null_instance() },
  m_control{ },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }

  constexpr
  auto_ptr
  (dango::null_tag const)noexcept
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<allocator_type>
  ):
  m_ptr{ dango::null },
  m_deleter{ },
  m_control{ dango::null },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }

  constexpr
  auto_ptr
  (dango::null_tag const)noexcept
  requires
  (
    dango::is_void<value_type> &&
    !dango::is_void<allocator_type> &&
    !dango::is_void<allocator_handle_type>
  ):
  m_ptr{ dango::null },
  m_deleter{ },
  m_control{ },
  m_alloc_ptr{ dango::null },
  m_size{ },
  m_align{ }
  { }

  constexpr
  auto_ptr
  (dango::null_tag const)noexcept
  requires
  (
    dango::is_void<value_type> &&
    !dango::is_void<allocator_type> &&
    dango::is_void<allocator_handle_type>
  ):
  m_ptr{ dango::null },
  m_deleter{ },
  m_control{ },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  { }

  explicit constexpr auto_ptr()noexcept = delete;

  explicit constexpr
  auto_ptr()noexcept
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<deleter_type> &&
    dango::detail::auto_ptr_deleter_null_instance_check<deleter_type>
  ):
  auto_ptr{ dango::null }
  { }

  explicit constexpr
  auto_ptr()noexcept
  requires(!dango::is_void<allocator_type>):
  auto_ptr{ dango::null }
  { }

  constexpr auto_ptr(auto_ptr const&)noexcept = delete;

  constexpr auto_ptr(auto_ptr&&)noexcept = delete;

  constexpr
  auto_ptr
  (auto_ptr&& a_ptr)noexcept
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<deleter_type> &&
    dango::is_noexcept_move_constructible<deleter_type>
  ):
  m_ptr{ dango::move(a_ptr).m_ptr },
  m_deleter{ dango::move(a_ptr).m_deleter },
  m_control{ },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  {
    a_ptr.m_ptr = dango::null;
  }

  constexpr
  auto_ptr
  (auto_ptr&& a_ptr)noexcept
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<allocator_type>
  ):
  m_ptr{ dango::move(a_ptr).m_ptr },
  m_deleter{ },
  m_control{ dango::move(a_ptr).m_control },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  {
    a_ptr.m_ptr = dango::null;
    a_ptr.m_control = dango::null;
  }

  constexpr
  auto_ptr
  (auto_ptr&& a_ptr)noexcept
  requires
  (
    dango::is_void<value_type> &&
    !dango::is_void<allocator_type> &&
    !dango::is_void<allocator_handle_type>
  ):
  m_ptr{ dango::move(a_ptr).m_ptr },
  m_deleter{ },
  m_control{ },
  m_alloc_ptr{ dango::move(a_ptr).m_alloc_ptr },
  m_size{ dango::move(a_ptr).m_size },
  m_align{ dango::move(a_ptr).m_align }
  {
    a_ptr.m_ptr = dango::null;
    a_ptr.m_size = size_type(0);
    a_ptr.m_align = size_type(0);
  }

  constexpr
  auto_ptr
  (auto_ptr&& a_ptr)noexcept
  requires
  (
    dango::is_void<value_type> &&
    !dango::is_void<allocator_type> &&
    dango::is_void<allocator_handle_type>
  ):
  m_ptr{ dango::move(a_ptr).m_ptr },
  m_deleter{ },
  m_control{ },
  m_alloc_ptr{ },
  m_size{ dango::move(a_ptr).m_size },
  m_align{ dango::move(a_ptr).m_align }
  {
    a_ptr.m_ptr = dango::null;
    a_ptr.m_size = size_type(0);
    a_ptr.m_align = size_type(0);
  }

  constexpr
  ~auto_ptr()noexcept
  {
    if constexpr(!dango::is_void<value_type> && !dango::is_void<deleter_type>)
    {
      if(m_ptr)
      {
        m_deleter(dango::as_const(m_ptr));
      }
    }

    if constexpr(!dango::is_void<value_type> && !dango::is_void<allocator_type>)
    {
      if(m_ptr)
      {
        auto const a_delete_func = m_control->get_delete_func();

        a_delete_func(m_control);
      }
    }

    if constexpr(dango::is_void<value_type> && !dango::is_void<allocator_type>)
    {
      if(m_ptr)
      {
        auto const a_size = m_size;
        auto const a_align = m_align;

        if constexpr(!dango::is_void<allocator_handle_type>)
        {
          m_alloc_ptr->dealloc(dango::as_const(m_ptr), a_size, a_align);
        }
        else
        {
          allocator_type::dealloc(dango::as_const(m_ptr), a_size, a_align);
        }
      }
    }
  }

  constexpr auto
  operator =
  (dango::null_tag const)& noexcept->auto_ptr&
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<deleter_type> &&
    dango::detail::auto_ptr_deleter_null_instance_check<deleter_type> &&
    dango::is_noexcept_swappable<deleter_ref_type>
  )
  {
    auto_ptr a_temp{ dango::null };

    dango::swap(*this, a_temp);

    return *this;
  }

  constexpr auto
  operator =
  (dango::null_tag const)& noexcept->auto_ptr&
  requires(!dango::is_void<allocator_type>)
  {
    auto_ptr a_temp{ dango::null };

    dango::swap(*this, a_temp);

    return *this;
  }

  constexpr auto operator = (auto_ptr const&)& noexcept->auto_ptr& = delete;

  constexpr auto operator = (auto_ptr&&)& noexcept->auto_ptr& = delete;

  constexpr auto
  operator =
  (auto_ptr&& a_ptr)& noexcept->auto_ptr&
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<deleter_type> &&
    dango::is_noexcept_move_constructible<deleter_type> &&
    dango::is_noexcept_swappable<deleter_ref_type>
  )
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(*this, a_temp);

    return *this;
  }

  constexpr auto
  operator =
  (auto_ptr&& a_ptr)& noexcept->auto_ptr&
  requires(!dango::is_void<allocator_type>)
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(*this, a_temp);

    return *this;
  }
public:
  template
  <typename tp_val, typename tp_del>
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<deleter_type> &&
    dango::is_noexcept_move_constructible<deleter_type> &&
    dango::detail::auto_ptr_deleter_prvd_check<deleter_type>
  )
  constexpr
  auto_ptr
  (dango::auto_ptr<tp_val, tp_del>&& a_ptr)noexcept
  requires
  (
    !dango::is_same<value_type, tp_val> &&
    dango::is_same<deleter_type, dango::decay<tp_del>> &&
    dango::detail::auto_ptr_valid_conversion<value_type, tp_val> &&
    dango::detail::auto_ptr_deleter_prvd<deleter_type, value_type, tp_val>
  ):
  m_ptr{ dango::move(a_ptr).m_ptr },
  m_deleter{ dango::move(a_ptr).m_deleter },
  m_control{ },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  {
    a_ptr.m_ptr = dango::null;
  }

  template
  <typename tp_val>
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<allocator_type>
  )
  constexpr
  auto_ptr
  (dango::auto_ptr<tp_val, allocator_type>&& a_ptr)noexcept
  requires
  (
    !dango::is_same<value_type, tp_val> &&
    dango::detail::auto_ptr_valid_conversion<value_type, tp_val>
  ):
  m_ptr{ dango::move(a_ptr).m_ptr },
  m_deleter{ },
  m_control{ dango::move(a_ptr).m_control },
  m_alloc_ptr{ },
  m_size{ },
  m_align{ }
  {
    a_ptr.m_ptr = dango::null;
    a_ptr.m_control = dango::null;
  }

  template
  <typename tp_val>
  requires
  (
    dango::is_void<value_type> &&
    !dango::is_void<allocator_type> &&
    !dango::is_void<allocator_handle_type>
  )
  constexpr
  auto_ptr
  (dango::auto_ptr<tp_val, allocator_type>&& a_ptr)noexcept
  requires
  (
    !dango::is_same<value_type, tp_val> &&
    dango::detail::auto_ptr_valid_conversion_void<value_type, tp_val>
  ):
  m_ptr{ dango::move(a_ptr).m_ptr },
  m_deleter{ },
  m_control{ },
  m_alloc_ptr{ dango::move(a_ptr).m_alloc_ptr },
  m_size{ dango::move(a_ptr).m_size },
  m_align{ dango::move(a_ptr).m_size }
  {
    a_ptr.m_ptr = dango::null;
    a_ptr.m_size = size_type(0);
    a_ptr.m_align = size_type(0);
  }

  template
  <typename tp_val>
  requires
  (
    dango::is_void<value_type> &&
    !dango::is_void<allocator_type> &&
    dango::is_void<allocator_handle_type>
  )
  constexpr
  auto_ptr
  (dango::auto_ptr<tp_val, allocator_type>&& a_ptr)noexcept
  requires
  (
    !dango::is_same<value_type, tp_val> &&
    dango::detail::auto_ptr_valid_conversion_void<value_type, tp_val>
  ):
  m_ptr{ dango::move(a_ptr).m_ptr },
  m_deleter{ },
  m_control{ },
  m_alloc_ptr{ },
  m_size{ dango::move(a_ptr).m_size },
  m_align{ dango::move(a_ptr).m_size }
  {
    a_ptr.m_ptr = dango::null;
    a_ptr.m_size = size_type(0);
    a_ptr.m_align = size_type(0);
  }

  template
  <typename tp_val, typename tp_del>
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<deleter_type> &&\
    dango::is_noexcept_move_constructible<deleter_type> &&
    dango::is_noexcept_swappable<deleter_ref_type> &&
    dango::detail::auto_ptr_deleter_prvd_check<deleter_type>
  )
  constexpr auto
  operator =
  (dango::auto_ptr<tp_val, tp_del>&& a_ptr)& noexcept->auto_ptr&
  requires
  (
    !dango::is_same<value_type, tp_val> &&
    dango::is_same<deleter_type, dango::decay<tp_del>> &&
    dango::detail::auto_ptr_valid_conversion<value_type, tp_val> &&
    dango::detail::auto_ptr_deleter_prvd<deleter_type, value_type, tp_val>
  )
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(*this, a_temp);

    return *this;
  }

  template
  <typename tp_val>
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<allocator_type>
  )
  constexpr auto
  operator =
  (dango::auto_ptr<tp_val, allocator_type>&& a_ptr)& noexcept->auto_ptr&
  requires
  (
    !dango::is_same<value_type, tp_val> &&
    dango::detail::auto_ptr_valid_conversion<value_type, tp_val>
  )
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(*this, a_temp);

    return *this;
  }

  template
  <typename tp_val>
  requires
  (
    dango::is_void<value_type> &&
    !dango::is_void<allocator_type> &&
    !dango::is_void<allocator_handle_type>
  )
  constexpr auto
  operator =
  (dango::auto_ptr<tp_val, allocator_type>&& a_ptr)& noexcept->auto_ptr&
  requires
  (
    !dango::is_same<value_type, tp_val> &&
    dango::detail::auto_ptr_valid_conversion_void<value_type, tp_val>
  )
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(*this, a_temp);

    return *this;
  }

  template
  <typename tp_val>
  requires
  (
    dango::is_void<value_type> &&
    !dango::is_void<allocator_type> &&
    dango::is_void<allocator_handle_type>
  )
  constexpr auto
  operator =
  (dango::auto_ptr<tp_val, allocator_type>&& a_ptr)& noexcept->auto_ptr&
  requires
  (
    !dango::is_same<value_type, tp_val> &&
    dango::detail::auto_ptr_valid_conversion_void<value_type, tp_val>
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
    dango_assert_nonnull(*this);

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
  align()const noexcept->size_type
  requires(dango::is_void<value_type>)
  {
    dango_assert_nonnull(*this);

    return m_align;
  }

  constexpr auto
  release()const noexcept->release_type
  requires
  (
    (!dango::is_void<value_type> && !dango::is_void<deleter_type>) ||
    (dango::is_void<value_type> && !dango::is_void<allocator_type>)
  )
  {
    dango_assert_nonnull(*this);

    auto const a_ptr = get();

    m_ptr = dango::null;

    if constexpr(dango::is_void<value_type>)
    {
      if constexpr(!dango::is_void<allocator_handle_type>)
      {
        m_alloc_ptr = dango::null;
      }

      return release_type{ a_ptr, size(), align() };
    }
    else
    {
      return a_ptr;
    }
  }

  constexpr auto
  get_deleter()& noexcept->auto const&
  requires(!dango::is_void<deleter_type>)
  {
    dango_assert_nonnull(*this);

    return m_deleter;
  }
  constexpr auto
  get_deleter()const& noexcept->auto const&
  requires(!dango::is_void<deleter_type>)
  {
    dango_assert_nonnull(*this);

    return m_deleter;
  }
  constexpr auto
  get_deleter()&& noexcept
  requires(!dango::is_void<deleter_type>) = delete;
  constexpr auto
  get_deleter()const&& noexcept
  requires(!dango::is_void<deleter_type>) = delete;

  constexpr auto
  get_allocator_handle()& noexcept->auto const&
  requires(!dango::is_void<allocator_handle_type>)
  {
    return static_cast<auto_ptr const&>(*this).get_allocator_handle();
  }
  constexpr auto
  get_allocator_handle()const& noexcept->auto const&
  requires(!dango::is_void<allocator_handle_type>)
  {
    dango_assert_nonnull(*this);

    if constexpr(dango::is_void<value_type>)
    {
      return m_alloc_ptr;
    }
    else
    {
      auto const a_handle = m_control->get_handle();

      dango_assert_nonnull(a_handle);

      return *static_cast<allocator_handle_type const*>(a_handle);
    }
  }
  constexpr auto
  get_allocator_handle()&& noexcept
  requires(!dango::is_void<allocator_handle_type>) = delete;
  constexpr auto
  get_allocator_handle()const&& noexcept
  requires(!dango::is_void<allocator_handle_type>) = delete;
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
  <typename tp_del>
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<deleter_type> &&
    dango::is_noexcept_swappable<deleter_ref_type>
  )
  constexpr void
  dango_operator_swap
  (dango::auto_ptr<value_type, tp_del>& a_ptr)& noexcept
  requires(dango::is_same<deleter_type, dango::decay<tp_del>>)
  {
    dango::swap(m_ptr, a_ptr.m_ptr);
    dango::swap(m_deleter, a_ptr.m_deleter);
  }

  constexpr void
  dango_operator_swap
  (auto_ptr& a_ptr)& noexcept
  requires
  (
    !dango::is_void<value_type> &&
    !dango::is_void<allocator_type>
  )
  {
    dango::swap(m_ptr, a_ptr.m_ptr);
    dango::swap(m_control, a_ptr.m_control);
  }

  constexpr void
  dango_operator_swap
  (auto_ptr& a_ptr)& noexcept
  requires
  (
    dango::is_void<value_type> &&
    !dango::is_void<allocator_type>
  )
  {
    dango::swap(m_ptr, a_ptr.m_ptr);

    if constexpr(!dango::is_void<allocator_handle_type>)
    {
      dango::swap(m_alloc_ptr, a_ptr.m_alloc_ptr);
    }

    dango::swap(m_size, a_ptr.m_size);
    dango::swap(m_align, a_ptr.m_align);
  }
private:
  ptr_type m_ptr;
  deleter_member_type m_deleter [[no_unique_address]];
  control_member_type m_control [[no_unique_address]];
  allocator_handle_member_type m_alloc_ptr [[no_unique_address]];
  size_member_type m_size [[no_unique_address]];
  align_member_type m_align [[no_unique_address]];
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
    dango::is_noexcept_destructible<tp_type> &&
    dango::is_deleter<tp_deleter, tp_type> &&
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
    dango::is_noexcept_destructible<tp_type> &&
    dango::is_handle_based_allocator<tp_allocator> &&
    dango::is_brace_constructible<typename tp_allocator::handle_type, tp_alloc> &&
    dango::is_brace_constructible<tp_type, tp_args...>
  )
  static constexpr auto
  make
  (dango::priority_tag<dango::uint(3)> const, tp_alloc&& a_alloc, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_handle_based_allocator<tp_allocator> &&
    dango::is_noexcept_brace_constructible<typename tp_allocator::handle_type, tp_alloc> &&
    dango::is_noexcept_brace_constructible<tp_type, tp_args...>
  )->auto
  {
    using control_type = dango::detail::auto_ptr_control_handle_based<tp_type, tp_allocator>;
    constexpr auto const c_size = sizeof(control_type);
    constexpr auto const c_align = alignof(control_type);

    typename tp_allocator::handle_type a_alloc_ptr{ dango::forward<tp_alloc>(a_alloc) };

    auto const a_addr = a_alloc_ptr->alloc(c_size, c_align);

    auto a_guard =
      dango::make_guard([a_alloc_ptr, a_addr]()noexcept->void{ a_alloc_ptr->dealloc(a_addr, c_size, c_align); });

    auto const a_control = dango_placement_new(a_addr, control_type, { dango::move(a_alloc_ptr), dango::forward<tp_args>(a_args)... });

    a_guard.dismiss();

    auto const a_ptr = a_control->get_object_ptr();

    return dango::auto_ptr<tp_type, tp_allocator>{ make_tag{ }, a_ptr, a_control };
  }

  template
  <typename tp_type, typename tp_allocator, typename... tp_args>
  requires
  (
    dango::is_object_exclude_array<tp_type> &&
    dango::is_noexcept_destructible<tp_type> &&
    dango::is_nohandle_allocator<tp_allocator> &&
    dango::is_brace_constructible<tp_type, tp_args...>
  )
  static constexpr auto
  make
  (dango::priority_tag<dango::uint(2)> const, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_nohandle_allocator<tp_allocator> &&
    dango::is_noexcept_brace_constructible<tp_type, tp_args...>
  )->auto
  {
    using control_type = dango::detail::auto_ptr_control_nohandle<tp_type, tp_allocator>;
    constexpr auto const c_size = sizeof(control_type);
    constexpr auto const c_align = alignof(control_type);

    auto const a_addr = tp_allocator::alloc(c_size, c_align);

    auto a_guard =
      dango::make_guard([a_addr]()noexcept->void{ tp_allocator::dealloc(a_addr, c_size, c_align); });

    auto const a_control = dango_placement_new(a_addr, control_type, { dango::forward<tp_args>(a_args)... });

    a_guard.dismiss();

    auto const a_ptr = a_control->get_object_ptr();

    return dango::auto_ptr<tp_type, tp_allocator>{ make_tag{ }, a_ptr, a_control };
  }

  template
  <typename tp_type, typename tp_allocator, typename tp_alloc>
  requires
  (
    dango::is_void<tp_type> &&
    dango::is_handle_based_allocator<tp_allocator> &&
    dango::is_brace_constructible<typename tp_allocator::handle_type, tp_alloc>
  )
  static constexpr auto
  make
  (dango::priority_tag<dango::uint(1)> const, tp_alloc&& a_alloc, dango::usize const a_size, dango::usize const a_align)
  noexcept
  (
    dango::is_noexcept_handle_based_allocator<tp_allocator> &&
    dango::is_noexcept_brace_constructible<typename tp_allocator::handle_type, tp_alloc>
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
    dango::is_nohandle_allocator<tp_allocator>
  )
  static constexpr auto
  make
  (dango::priority_tag<dango::uint(0)> const, dango::usize const a_size, dango::usize const a_align)
  noexcept(dango::is_noexcept_nohandle_allocator<tp_allocator>)->auto
  {
    auto const a_ptr = tp_allocator::alloc(a_size, a_align);

    return dango::auto_ptr<tp_type, tp_allocator>{ make_tag{ }, a_ptr, a_size, a_align };
  }
public:
  DANGO_UNCONSTRUCTIBLE(auto_ptr_make)
};

namespace
dango
{
  template
  <typename tp_type>
  explicit auto_ptr(tp_type*)->auto_ptr<tp_type, dango::plain_delete_type>;

  template
  <typename tp_type, typename tp_del>
  explicit auto_ptr(tp_type*, tp_del&&)->auto_ptr<tp_type, dango::decay<tp_del>>;

  template
  <dango::is_object_exclude_array tp_type, typename... tp_args>
  requires
  (requires{ { dango::detail::auto_ptr_make::make<tp_type, dango::plain_delete_type>(dango::detail::auto_ptr_make_prio{ }, dango::plain_delete, dango::declval<tp_args>()...) }; })
  constexpr auto
  make_auto_ptr
  (tp_args&&... a_args)
  noexcept
  (requires{ { dango::detail::auto_ptr_make::make<tp_type, dango::plain_delete_type>(dango::detail::auto_ptr_make_prio{ }, dango::plain_delete, dango::declval<tp_args>()...) }noexcept; })->auto
  {
    return dango::detail::auto_ptr_make::make<tp_type, dango::plain_delete_type>(dango::detail::auto_ptr_make_prio{ }, dango::plain_delete, dango::forward<tp_args>(a_args)...);
  }

  template
  <dango::is_object_exclude_array tp_type, dango::is_nohandle_allocator tp_allocator, typename... tp_args>
  requires
  (requires{ { dango::detail::auto_ptr_make::make<tp_type, tp_allocator>(dango::detail::auto_ptr_make_prio{ }, dango::declval<tp_args>()...) }; })
  constexpr auto
  make_auto_ptr
  (tp_args&&... a_args)
  noexcept
  (requires{ { dango::detail::auto_ptr_make::make<tp_type, tp_allocator>(dango::detail::auto_ptr_make_prio{ }, dango::declval<tp_args>()...) }noexcept; })->auto
  {
    return dango::detail::auto_ptr_make::make<tp_type, tp_allocator>(dango::detail::auto_ptr_make_prio{ }, dango::forward<tp_args>(a_args)...);
  }

  template
  <dango::is_object_exclude_array tp_type, dango::is_handle_based_allocator tp_allocator, typename tp_handle, typename... tp_args>
  requires
  (requires{ { dango::detail::auto_ptr_make::make<tp_type, tp_allocator>(dango::detail::auto_ptr_make_prio{ }, dango::declval<tp_handle>(), dango::declval<tp_args>()...) }; })
  constexpr auto
  make_auto_ptr
  (tp_handle&& a_handle, tp_args&&... a_args)
  noexcept
  (requires{ { dango::detail::auto_ptr_make::make<tp_type, tp_allocator>(dango::detail::auto_ptr_make_prio{ }, dango::declval<tp_handle>(), dango::declval<tp_args>()...) }noexcept; })->auto
  {
    return dango::detail::auto_ptr_make::make<tp_type, tp_allocator>(dango::detail::auto_ptr_make_prio{ }, dango::forward<tp_handle>(a_handle), dango::forward<tp_args>(a_args)...);
  }

  template
  <dango::is_void tp_type, dango::is_nohandle_allocator tp_allocator = dango::basic_allocator>
  requires
  (requires(dango::usize const a_usize){ { dango::detail::auto_ptr_make::make<tp_type, tp_allocator>(dango::detail::auto_ptr_make_prio{ }, a_usize, a_usize) }; })
  constexpr auto
  make_auto_ptr
  (dango::usize const a_size, dango::usize const a_align)
  noexcept
  (requires(dango::usize const a_usize){ { dango::detail::auto_ptr_make::make<tp_type, tp_allocator>(dango::detail::auto_ptr_make_prio{ }, a_usize, a_usize) }noexcept; })->auto
  {
    return dango::detail::auto_ptr_make::make<tp_type, tp_allocator>(dango::detail::auto_ptr_make_prio{ }, a_size, a_align);
  }

  template
  <dango::is_void tp_type, dango::is_handle_based_allocator tp_allocator, typename tp_handle>
  requires
  (requires(dango::usize const a_usize){ { dango::detail::auto_ptr_make::make<tp_type, tp_allocator>(dango::detail::auto_ptr_make_prio{ }, dango::declval<tp_handle>(), a_usize, a_usize) }; })
  constexpr auto
  make_auto_ptr
  (tp_handle&& a_handle, dango::usize const a_size, dango::usize const a_align)
  noexcept
  (requires(dango::usize const a_usize){ { dango::detail::auto_ptr_make::make<tp_type, tp_allocator>(dango::detail::auto_ptr_make_prio{ }, dango::declval<tp_handle>(), a_usize, a_usize) }noexcept; })->auto
  {
    return dango::detail::auto_ptr_make::make<tp_type, tp_allocator>(dango::detail::auto_ptr_make_prio{ }, dango::forward<tp_handle>(a_handle), a_size, a_align);
  }
}

#endif // DANGO_AUTO_PTR_HPP_INCLUDED
