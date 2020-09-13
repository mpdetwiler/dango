#ifndef DANGO_AUTO_PTR_HPP_INCLUDED
#define DANGO_AUTO_PTR_HPP_INCLUDED

#include "dango-allocator.hpp"
#include "dango-tuple.hpp"

namespace
dango
{
  template
  <typename tp_deleter, typename tp_elem>
  concept is_deleter =
    !dango::is_const_or_volatile<tp_deleter> &&
    (dango::is_object_exclude_array<tp_deleter> || dango::is_func<tp_deleter>) &&
    dango::is_noexcept_destructible<dango::decay<tp_deleter>> &&
    dango::is_object_exclude_array<tp_elem> &&
    dango::is_noexcept_callable_ret<void, dango::decay<tp_deleter>&, tp_elem* const&> &&
    !dango::is_allocator<tp_deleter>;

  inline constexpr auto const plain_delete =
    []<dango::is_object_exclude_array tp_elem>
    (tp_elem const volatile* const a_ptr)constexpr noexcept->void
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
  <typename tp_deleter, typename tp_elem DANGO_GCC_BUG_81043_WORKAROUND>
  struct deleter_traits;

  template
  <typename tp_elem>
  requires(dango::is_deleter<dango::plain_delete_type, tp_elem>)
  struct
  deleter_traits<dango::plain_delete_type, tp_elem>
  final
  {
    static inline constexpr bool const polymorphic_requires_virtual_destruct = true;

    static constexpr auto get_default_instance()noexcept->auto const&{ return dango::plain_delete; }
    static constexpr auto get_null_instance()noexcept->auto const&{ return dango::plain_delete; }

    DANGO_UNCONSTRUCTIBLE(deleter_traits)
  };

  template
  <dango::is_noexcept_default_constructible tp_deleter, typename tp_elem>
  requires(dango::is_deleter<tp_deleter, tp_elem> && !dango::is_func_ptr<tp_deleter>)
  struct
  deleter_traits<tp_deleter, tp_elem DANGO_GCC_BUG_81043_WORKAROUND_ID(0, tp_deleter, tp_elem)>
  {
    static inline constexpr bool const polymorphic_requires_virtual_destruct = true;

    static constexpr auto get_default_instance()noexcept->tp_deleter{ return tp_deleter{ }; }
    static constexpr auto get_null_instance()noexcept->tp_deleter{ return tp_deleter{ }; }

    DANGO_UNCONSTRUCTIBLE(deleter_traits)
  };

  template
  <dango::is_func_ptr tp_deleter, typename tp_elem>
  requires(dango::is_deleter<tp_deleter, tp_elem>)
  struct
  deleter_traits<tp_deleter, tp_elem DANGO_GCC_BUG_81043_WORKAROUND_ID(1, tp_deleter, tp_elem)>
  {
    static inline constexpr bool const polymorphic_requires_virtual_destruct = true;

    static constexpr auto get_null_instance()noexcept->tp_deleter{ return dango::null; }

    DANGO_UNCONSTRUCTIBLE(deleter_traits)
  };
}

namespace
dango::detail
{
  template
  <typename tp_deleter, typename tp_elem>
  concept auto_ptr_deleter_prvd_exists =
    requires{ { dango::constexpr_check<bool, dango::custom::deleter_traits<tp_deleter, tp_elem>::polymorphic_requires_virtual_destruct>{ } }noexcept; };

  template
  <typename tp_deleter, typename tp_elem>
  concept auto_ptr_deleter_prvd_value =
    dango::detail::auto_ptr_deleter_prvd_exists<tp_deleter, tp_elem> &&
    bool(dango::custom::deleter_traits<tp_deleter, tp_elem>::polymorphic_requires_virtual_destruct);

  template
  <typename tp_deleter, typename tp_lhs, typename tp_rhs>
  concept auto_ptr_deleter_prvd_test =
    dango::is_same_ignore_cv<tp_lhs, tp_rhs> ||
    !dango::detail::auto_ptr_deleter_prvd_value<tp_deleter, tp_lhs> ||
    (dango::is_base_of<tp_lhs, tp_rhs> && dango::is_virtual_destructible<tp_lhs>);

  template
  <typename tp_deleter, typename tp_elem>
  concept auto_ptr_deleter_default_instance_exists =
    requires{ { dango::custom::deleter_traits<tp_deleter, tp_elem>::get_default_instance() }noexcept->dango::is_noexcept_convertible_ret<tp_deleter>; };

  template
  <typename tp_deleter, typename tp_elem>
  concept auto_ptr_deleter_null_instance_exists =
    requires{ { dango::custom::deleter_traits<tp_deleter, tp_elem>::get_null_instance() }noexcept->dango::is_noexcept_convertible_ret<tp_deleter>; };

  template
  <typename tp_lhs, typename tp_rhs>
  concept auto_ptr_valid_conversion =
    (dango::is_same_ignore_cv<tp_lhs, tp_rhs> || dango::is_base_of<tp_lhs, tp_rhs>) &&
    dango::is_noexcept_destructible<tp_lhs> &&
    dango::is_noexcept_brace_constructible<tp_lhs*, tp_rhs* const&> &&
    dango::is_noexcept_brace_constructible<tp_lhs*, tp_rhs*&&> &&
    dango::is_noexcept_convertible_arg<tp_rhs* const&, tp_lhs*> &&
    dango::is_noexcept_convertible_arg<tp_rhs*&&, tp_lhs*>;

  template
  <typename tp_lhs, typename tp_rhs>
  concept auto_ptr_valid_conversion_void =
    dango::is_void<tp_rhs> &&
    dango::is_noexcept_brace_constructible<tp_lhs*, tp_rhs* const&> &&
    dango::is_noexcept_brace_constructible<tp_lhs*, tp_rhs*&&> &&
    dango::is_noexcept_convertible_arg<tp_rhs* const&, tp_lhs*> &&
    dango::is_noexcept_convertible_arg<tp_rhs*&&, tp_lhs*>;

  template
  <typename tp_elem, typename... tp_args>
  concept auto_ptr_deferred_construct_test =
    dango::is_brace_constructible<tp_elem, tp_args...> &&
    dango::is_noexcept_destructible<tp_elem>;

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
  m_object{ dango::forward<tp_args>(a_args)... }
  { }
  constexpr ~auto_ptr_control_nohandle()noexcept = default;
public:
  constexpr auto get_object_ptr()noexcept->auto{ return dango::addressof(m_object); }
private:
  tp_type m_object;
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
  super_type{ &delete_this, dango::addressof(m_alloc_ptr) },
  m_alloc_ptr{ dango::move(a_alloc) },
  m_object{ dango::forward<tp_args>(a_args)... }
  { }
  constexpr ~auto_ptr_control_handle_based()noexcept = default;
public:
  constexpr auto get_object_ptr()noexcept->auto{ return dango::addressof(m_object); }
private:
  allocator_handle_type m_alloc_ptr;
  tp_type m_object;
public:
  DANGO_DELETE_DEFAULT(auto_ptr_control_handle_based)
  DANGO_UNMOVEABLE(auto_ptr_control_handle_based)
};

namespace
dango
{
  template
  <typename tp_type, typename tp_deleter_or_allocator = dango::auto_ptr_default_deleter<tp_type> DANGO_GCC_BUG_81043_WORKAROUND>
  class auto_ptr;

  template
  <dango::is_object_exclude_array tp_elem_type, dango::is_deleter<tp_elem_type> tp_deleter>
  class auto_ptr<tp_elem_type, tp_deleter DANGO_GCC_BUG_81043_WORKAROUND_ID(0, tp_elem_type, tp_deleter)>;

  template
  <dango::is_object_exclude_array tp_elem_type, dango::is_allocator tp_allocator>
  class auto_ptr<tp_elem_type, tp_allocator DANGO_GCC_BUG_81043_WORKAROUND_ID(1, tp_elem_type, tp_allocator)>;

  template
  <dango::is_void tp_elem_type, dango::is_nohandle_allocator tp_allocator>
  class auto_ptr<tp_elem_type, tp_allocator DANGO_GCC_BUG_81043_WORKAROUND_ID(2, tp_elem_type, tp_allocator)>;

  template
  <dango::is_void tp_elem_type, dango::is_handle_based_allocator tp_allocator>
  class auto_ptr<tp_elem_type, tp_allocator DANGO_GCC_BUG_81043_WORKAROUND_ID(3, tp_elem_type, tp_allocator)>;

  struct
  auto_ptr_construct_tag
  final
  {
    DANGO_TAG_TYPE(auto_ptr_construct_tag)
  };

  inline constexpr auto_ptr_construct_tag const auto_ptr_construct{ };
}

template
<dango::is_object_exclude_array tp_elem_type, dango::is_deleter<tp_elem_type> tp_deleter>
class
dango::
auto_ptr
<tp_elem_type, tp_deleter DANGO_GCC_BUG_81043_WORKAROUND_ID(0, tp_elem_type, tp_deleter)>
final
{
  template
  <typename tp_first, typename tp_second DANGO_GCC_BUG_81043_WORKAROUND_ND>
  friend class dango::auto_ptr;
public:
  using elem_type = tp_elem_type;
  using ptr_type = elem_type*;
  using ref_type = elem_type&;
  using deleter_type = dango::decay<tp_deleter>;
public:
  template
  <typename tp_elem>
  requires
  (
    dango::detail::auto_ptr_deleter_prvd_exists<deleter_type, elem_type> &&
    dango::detail::auto_ptr_deleter_default_instance_exists<deleter_type, elem_type> &&
    dango::detail::auto_ptr_valid_conversion<elem_type, tp_elem> &&
    dango::detail::auto_ptr_deleter_prvd_test<deleter_type, elem_type, tp_elem>
  )
  explicit constexpr
  auto_ptr
  (tp_elem* const a_ptr)noexcept:
  m_ptr{ a_ptr },
  m_deleter{ dango::custom::deleter_traits<deleter_type, elem_type>::get_default_instance() }
  { }

  template
  <typename tp_elem, typename tp_deleter_arg>
  requires
  (
    dango::detail::auto_ptr_deleter_prvd_exists<deleter_type, elem_type> &&
    dango::detail::auto_ptr_valid_conversion<elem_type, tp_elem> &&
    dango::detail::auto_ptr_deleter_prvd_test<deleter_type, elem_type, tp_elem> &&
    dango::is_noexcept_brace_constructible<deleter_type, tp_deleter_arg>
  )
  explicit constexpr
  auto_ptr
  (tp_elem* const a_ptr, tp_deleter_arg&& a_deleter)noexcept:
  m_ptr{ a_ptr },
  m_deleter{ dango::forward<tp_deleter_arg>(a_deleter) }
  { }
public:
  constexpr
  auto_ptr
  (dango::null_tag const)noexcept
  requires(dango::detail::auto_ptr_deleter_null_instance_exists<deleter_type, elem_type>):
  m_ptr{ dango::null },
  m_deleter{ dango::custom::deleter_traits<deleter_type, elem_type>::get_null_instance() }
  { }

  explicit constexpr auto_ptr()noexcept = delete;

  explicit constexpr
  auto_ptr()noexcept
  requires(dango::detail::auto_ptr_deleter_null_instance_exists<deleter_type, elem_type>):
  auto_ptr{ dango::null }
  { }

  constexpr auto_ptr(auto_ptr const&)noexcept = delete;

  constexpr auto_ptr(auto_ptr&&)noexcept = delete;

  constexpr
  auto_ptr
  (auto_ptr&& a_ptr)noexcept
  requires(dango::is_noexcept_move_constructible<deleter_type>):
  m_ptr{ dango::exchange(a_ptr.m_ptr, dango::null) },
  m_deleter{ dango::move(a_ptr).m_deleter }
  { }

  constexpr
  ~auto_ptr()noexcept
  {
    if(m_ptr)
    {
      m_deleter(dango::as_const(m_ptr));
    }
  }

  constexpr auto
  operator =
  (dango::null_tag const)& noexcept->auto_ptr&
  requires
  (
    dango::detail::auto_ptr_deleter_null_instance_exists<deleter_type, elem_type> &&
    dango::is_noexcept_swappable<deleter_type&>
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
  (auto_ptr&& a_ptr)& noexcept->auto_ptr&
  requires
  (
    dango::is_noexcept_move_constructible<deleter_type> &&
    dango::is_noexcept_swappable<deleter_type&>
  )
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(*this, a_temp);

    return *this;
  }

  explicit constexpr operator bool()const noexcept{ return !dango::is_null(*this); }

  explicit constexpr operator ptr_type()const noexcept{ return get(); }

  constexpr auto
  operator -> ()const noexcept->ptr_type
  {
    dango_assert_nonnull(*this);

    return get();
  }

  constexpr auto
  operator * ()const noexcept->ref_type
  {
    dango_assert_nonnull(*this);

    return *get();
  }
public:
  template
  <typename tp_elem, typename tp_del>
  requires
  (
    dango::is_noexcept_move_constructible<deleter_type> &&
    dango::detail::auto_ptr_deleter_prvd_exists<deleter_type, elem_type> &&
    !(dango::is_same<elem_type, tp_elem> && dango::is_same<deleter_type, tp_del>) &&
    dango::is_same<deleter_type, dango::decay<tp_del>> &&
    dango::detail::auto_ptr_valid_conversion<elem_type, tp_elem> &&
    dango::detail::auto_ptr_deleter_prvd_test<deleter_type, elem_type, tp_elem>
  )
  constexpr
  auto_ptr
  (dango::auto_ptr<tp_elem, tp_del>&& a_ptr)noexcept:
  m_ptr{ dango::exchange(a_ptr.m_ptr, dango::null) },
  m_deleter{ dango::move(a_ptr).m_deleter }
  { }

  template
  <typename tp_elem, typename tp_del>
  requires
  (
    dango::is_noexcept_move_constructible<deleter_type> &&
    dango::is_noexcept_swappable<deleter_type&> &&
    dango::detail::auto_ptr_deleter_prvd_exists<deleter_type, elem_type> &&
    !(dango::is_same<elem_type, tp_elem> && dango::is_same<deleter_type, tp_del>) &&
    dango::is_same<deleter_type, dango::decay<tp_del>> &&
    dango::detail::auto_ptr_valid_conversion<elem_type, tp_elem> &&
    dango::detail::auto_ptr_deleter_prvd_test<deleter_type, elem_type, tp_elem>
  )
  constexpr auto
  operator =
  (dango::auto_ptr<tp_elem, tp_del>&& a_ptr)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(*this, a_temp);

    return *this;
  }
public:
  constexpr auto dango_operator_is_null()const noexcept->bool{ return dango::is_null(m_ptr); }

  template
  <typename tp_elem, typename tp_deleter_or_allocator>
  constexpr auto
  dango_operator_equals
  (dango::auto_ptr<tp_elem, tp_deleter_or_allocator> const& a_ptr)const noexcept->bool
  requires(dango::is_noexcept_equatable<ptr_type const&, tp_elem* const&>)
  {
    return dango::equals(m_ptr, a_ptr.m_ptr);
  }

  template
  <typename tp_elem, typename tp_deleter_or_allocator>
  constexpr auto
  dango_operator_compare
  (dango::auto_ptr<tp_elem, tp_deleter_or_allocator> const& a_ptr)const noexcept->auto
  requires(dango::is_noexcept_comparable<ptr_type const&, tp_elem* const&>)
  {
    return dango::compare(m_ptr, a_ptr.m_ptr);
  }

  auto dango_operator_hash()const noexcept->dango::hash_val{ return dango::hash(m_ptr); }

  template
  <typename tp_del>
  requires
  (
    dango::is_noexcept_swappable<deleter_type&> &&
    dango::is_same<deleter_type, dango::decay<tp_del>>
  )
  constexpr void
  dango_operator_swap
  (dango::auto_ptr<elem_type, tp_del>& a_ptr)& noexcept
  {
    dango::swap(m_ptr, a_ptr.m_ptr);
    dango::swap(m_deleter, a_ptr.m_deleter);
  }
public:
  constexpr auto get()const noexcept->ptr_type{ return m_ptr; }

  [[nodiscard]] constexpr auto
  release()noexcept->ptr_type
  {
    dango_assert_nonnull(*this);

    return dango::exchange(m_ptr, dango::null);
  }

  constexpr auto
  get_deleter()& noexcept->deleter_type const&
  {
    return m_deleter;
  }

  constexpr auto
  get_deleter()const& noexcept->deleter_type const&
  {
    return m_deleter;
  }

  constexpr auto
  get_deleter()&&
  noexcept(dango::is_noexcept_copy_constructible<deleter_type>)->deleter_type
  requires(dango::is_copy_constructible<deleter_type>)
  {
    if(m_ptr)
    {
      return m_deleter;
    }

    return dango::move(m_deleter);
  }

  constexpr auto
  get_deleter()const&&
  noexcept(dango::is_noexcept_copy_constructible<deleter_type>)->deleter_type
  requires(dango::is_copy_constructible<deleter_type>)
  {
    return m_deleter;
  }
private:
  ptr_type m_ptr;
  deleter_type m_deleter [[no_unique_address]];
};

template
<dango::is_object_exclude_array tp_elem_type, dango::is_allocator tp_allocator>
class
dango::
auto_ptr
<tp_elem_type, tp_allocator DANGO_GCC_BUG_81043_WORKAROUND_ID(1, tp_elem_type, tp_allocator)>
{
  template
  <typename tp_first, typename tp_second DANGO_GCC_BUG_81043_WORKAROUND_ND>
  friend class dango::auto_ptr;
public:
  using elem_type = tp_elem_type;
  using ptr_type = elem_type*;
  using ref_type = elem_type&;
  using allocator_type = tp_allocator;
  using allocator_handle_type = dango::allocator_handle_type<allocator_type, void>;
private:
  using control_ptr = dango::detail::auto_ptr_control*;
public:
  template
  <typename... tp_args>
  requires
  (
    dango::is_nohandle_allocator<allocator_type> &&
    dango::detail::auto_ptr_deferred_construct_test<elem_type, tp_args...>
  )
  explicit
  auto_ptr
  (dango::auto_ptr_construct_tag const, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_nohandle_allocator<allocator_type> &&
    dango::is_noexcept_brace_constructible<elem_type, tp_args...>
  )
  {
    using control_type = dango::detail::auto_ptr_control_nohandle<elem_type, allocator_type>;
    constexpr auto const c_size = sizeof(control_type);
    constexpr auto const c_align = alignof(control_type);

    auto const a_addr = allocator_type::alloc(c_size, c_align);

    auto a_guard =
      dango::make_guard([a_addr]()noexcept->void{ allocator_type::dealloc(a_addr, c_size, c_align); });

    auto const a_control = dango_placement_new(a_addr, control_type, { dango::forward<tp_args>(a_args)... });

    a_guard.dismiss();

    m_ptr = a_control->get_object_ptr();
    m_control = a_control;
  }

  template
  <dango::is_same_ignore_cvref<allocator_handle_type> tp_handle, typename... tp_args>
  requires
  (
    dango::is_handle_based_allocator<allocator_type> &&
    dango::detail::auto_ptr_deferred_construct_test<elem_type, tp_args...>
  )
  explicit
  auto_ptr
  (
    dango::auto_ptr_construct_tag const,
    dango::allocator_arg_tag const,
    tp_handle&& a_handle,
    tp_args&&... a_args
  )
  noexcept
  (
    dango::is_noexcept_handle_based_allocator<allocator_type> &&
    dango::is_noexcept_brace_constructible<elem_type, tp_args...>
  )
  {
    using control_type = dango::detail::auto_ptr_control_handle_based<elem_type, allocator_type>;
    constexpr auto const c_size = sizeof(control_type);
    constexpr auto const c_align = alignof(control_type);

    dango_assert_nonnull(a_handle);

    allocator_handle_type a_alloc_ptr{ dango::forward<tp_handle>(a_handle) };

    auto const a_addr = a_alloc_ptr->alloc(c_size, c_align);

    auto const a_push_guard =
      dango::push_allocator_if_user<allocator_type, elem_type>(a_alloc_ptr);

    if constexpr(dango::is_noexcept_brace_constructible<elem_type, tp_args...>)
    {
      auto const a_control =
        dango_placement_new(a_addr, control_type, { dango::move(a_alloc_ptr), dango::forward<tp_args>(a_args)... });

      m_ptr = a_control->get_object_ptr();
      m_control = a_control;
    }
    else
    {
      auto a_guard =
        dango::make_guard([a_alloc_ptr, a_addr]()noexcept->void{ a_alloc_ptr->dealloc(a_addr, c_size, c_align); });

      auto const a_control =
        dango_placement_new(a_addr, control_type, { dango::move(a_alloc_ptr), dango::forward<tp_args>(a_args)... });

      a_guard.dismiss();

      m_ptr = a_control->get_object_ptr();
      m_control = a_control;
    }
  }

  template
  <typename... tp_args>
  requires
  (
    dango::is_handle_based_allocator<allocator_type> &&
    dango::allocator_has_default_handle<allocator_type> &&
    dango::detail::auto_ptr_deferred_construct_test<elem_type, tp_args...>
  )
  explicit
  auto_ptr
  (dango::auto_ptr_construct_tag const, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_handle_based_allocator<allocator_type> &&
    dango::is_noexcept_brace_constructible<elem_type, tp_args...>
  ):
  auto_ptr
  {
    dango::auto_ptr_construct,
    dango::allocator_arg,
    dango::current_allocator_or_default<allocator_type>(),
    dango::forward<tp_args>(a_args)...
  }
  { }
public:
  constexpr
  auto_ptr
  (dango::null_tag const)noexcept:
  m_ptr{ dango::null },
  m_control{ dango::null }
  { }

  explicit constexpr
  auto_ptr()noexcept:
  auto_ptr{ dango::null }
  { }

  constexpr auto_ptr(auto_ptr const&)noexcept = delete;

  constexpr
  auto_ptr
  (auto_ptr&& a_ptr)noexcept:
  m_ptr{ dango::exchange(a_ptr.m_ptr, dango::null) },
  m_control{ dango::exchange(a_ptr.m_control, dango::null) }
  { }

  constexpr
  ~auto_ptr()noexcept
  {
    if(m_ptr)
    {
      dango_assert_nonnull(m_control);

      auto const a_delete_func = m_control->get_delete_func();

      a_delete_func(m_control);
    }
  }

  constexpr auto
  operator =
  (dango::null_tag const)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::null };

    dango::swap(*this, a_temp);

    return *this;
  }

  constexpr auto operator = (auto_ptr const&)& noexcept->auto_ptr& = delete;

  constexpr auto
  operator =
  (auto_ptr&& a_ptr)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(*this, a_temp);

    return *this;
  }

  explicit constexpr operator bool()const noexcept{ return !dango::is_null(*this); }

  explicit constexpr operator ptr_type()const noexcept{ return get(); }

  constexpr auto
  operator -> ()const noexcept->ptr_type
  {
    dango_assert_nonnull(*this);

    return get();
  }

  constexpr auto
  operator * ()const noexcept->ref_type
  {
    dango_assert_nonnull(*this);

    return *get();
  }
public:
  template
  <typename tp_elem>
  requires
  (!dango::is_same<elem_type, tp_elem> && dango::detail::auto_ptr_valid_conversion<elem_type, tp_elem>)
  constexpr
  auto_ptr
  (dango::auto_ptr<tp_elem, allocator_type>&& a_ptr)noexcept:
  m_ptr{ dango::exchange(a_ptr.m_ptr, dango::null) },
  m_control{ dango::exchange(a_ptr.m_control, dango::null) }
  { }

  template
  <typename tp_elem>
  requires(!dango::is_same<elem_type, tp_elem> && dango::detail::auto_ptr_valid_conversion<elem_type, tp_elem>)
  constexpr auto
  operator =
  (dango::auto_ptr<tp_elem, allocator_type>&& a_ptr)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(*this, a_temp);

    return *this;
  }
public:
  constexpr auto dango_operator_is_null()const noexcept->bool{ return dango::is_null(m_ptr); }

  template
  <typename tp_elem, typename tp_deleter_or_allocator>
  constexpr auto
  dango_operator_equals
  (dango::auto_ptr<tp_elem, tp_deleter_or_allocator> const& a_ptr)const noexcept->bool
  requires(dango::is_noexcept_equatable<ptr_type const&, tp_elem* const&>)
  {
    return dango::equals(m_ptr, a_ptr.m_ptr);
  }

  template
  <typename tp_elem, typename tp_deleter_or_allocator>
  constexpr auto
  dango_operator_compare
  (dango::auto_ptr<tp_elem, tp_deleter_or_allocator> const& a_ptr)const noexcept->auto
  requires(dango::is_noexcept_comparable<ptr_type const&, tp_elem* const&>)
  {
    return dango::compare(m_ptr, a_ptr.m_ptr);
  }

  auto dango_operator_hash()const noexcept->dango::hash_val{ return dango::hash(m_ptr); }

  constexpr void
  dango_operator_swap
  (auto_ptr& a_ptr)& noexcept
  {
    dango::swap(m_ptr, a_ptr.m_ptr);
    dango::swap(m_control, a_ptr.m_control);
  }
public:
  constexpr auto get()const noexcept->ptr_type{ return m_ptr; }

  constexpr auto
  get_allocator_handle()const noexcept->allocator_handle_type
  requires(!dango::is_void<allocator_handle_type>)
  {
    if constexpr(!dango::is_void<allocator_handle_type>)
    {
      if(m_ptr)
      {
        dango_assert_nonnull(m_control);

        auto const a_void_ptr = m_control->get_handle();

        dango_assert_nonnull(a_void_ptr);

        return allocator_handle_type{ *static_cast<allocator_handle_type const*>(a_void_ptr) };
      }
      else
      {
        return allocator_handle_type{ dango::null };
      }
    }
  }
private:
  ptr_type m_ptr;
  control_ptr m_control;
};

template
<dango::is_void tp_elem_type, dango::is_nohandle_allocator tp_allocator>
class
dango::
auto_ptr
<tp_elem_type, tp_allocator DANGO_GCC_BUG_81043_WORKAROUND_ID(2, tp_elem_type, tp_allocator)>
{
  template
  <typename tp_first, typename tp_second DANGO_GCC_BUG_81043_WORKAROUND_ND>
  friend class dango::auto_ptr;
public:
  using elem_type = tp_elem_type;
  using ptr_type = elem_type*;
  using allocator_type = tp_allocator;
  using size_type = dango::usize;
  using release_type = dango::tuple<ptr_type, size_type, size_type>;
public:
  explicit
  auto_ptr
  (size_type const a_size, size_type const a_align)
  noexcept(dango::is_noexcept_nohandle_allocator<allocator_type>):
  m_ptr{ allocator_type::alloc(a_size, a_align) },
  m_size{ a_size },
  m_align{ a_align }
  { }
public:
  constexpr
  auto_ptr
  (dango::null_tag const)noexcept:
  m_ptr{ dango::null },
  m_size{ size_type(0) },
  m_align{ size_type(0) }
  { }

  explicit constexpr
  auto_ptr()noexcept:
  auto_ptr{ dango::null }
  { }

  constexpr auto_ptr(auto_ptr const&)noexcept = delete;

  constexpr
  auto_ptr
  (auto_ptr&& a_ptr)noexcept:
  m_ptr{ dango::exchange(a_ptr.m_ptr, dango::null) },
  m_size{ dango::exchange(a_ptr.m_size, size_type(0)) },
  m_align{ dango::exchange(a_ptr.m_align, size_type(0)) }
  { }

  constexpr
  ~auto_ptr()noexcept
  {
    if(m_ptr)
    {
      auto const a_size = m_size;
      auto const a_align = m_align;

      allocator_type::dealloc(dango::as_const(m_ptr), a_size, a_align);
    }
  }

  constexpr auto
  operator =
  (dango::null_tag const)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::null };

    dango::swap(*this, a_temp);

    return *this;
  }

  constexpr auto operator = (auto_ptr const&)& noexcept->auto_ptr& = delete;

  constexpr auto
  operator =
  (auto_ptr&& a_ptr)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(*this, a_temp);

    return *this;
  }

  explicit constexpr operator bool()const noexcept{ return !dango::is_null(*this); }

  explicit constexpr operator ptr_type()const noexcept{ return get(); }
public:
  template
  <typename tp_elem>
  requires
  (!dango::is_same<elem_type, tp_elem> && dango::detail::auto_ptr_valid_conversion_void<elem_type, tp_elem>)
  constexpr
  auto_ptr
  (dango::auto_ptr<tp_elem, allocator_type>&& a_ptr)noexcept:
  m_ptr{ dango::exchange(a_ptr.m_ptr, dango::null) },
  m_size{ dango::exchange(a_ptr.m_size, size_type(0)) },
  m_align{ dango::exchange(a_ptr.m_align, size_type(0)) }
  { }

  template
  <typename tp_elem>
  requires(!dango::is_same<elem_type, tp_elem> && dango::detail::auto_ptr_valid_conversion_void<elem_type, tp_elem>)
  constexpr auto
  operator =
  (dango::auto_ptr<tp_elem, allocator_type>&& a_ptr)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(*this, a_temp);

    return *this;
  }
public:
  constexpr auto dango_operator_is_null()const noexcept->bool{ return dango::is_null(m_ptr); }

  template
  <typename tp_elem, typename tp_deleter_or_allocator>
  constexpr auto
  dango_operator_equals
  (dango::auto_ptr<tp_elem, tp_deleter_or_allocator> const& a_ptr)const noexcept->bool
  requires(dango::is_noexcept_equatable<ptr_type const&, tp_elem* const&>)
  {
    return dango::equals(m_ptr, a_ptr.m_ptr);
  }

  template
  <typename tp_elem, typename tp_deleter_or_allocator>
  constexpr auto
  dango_operator_compare
  (dango::auto_ptr<tp_elem, tp_deleter_or_allocator> const& a_ptr)const noexcept->auto
  requires(dango::is_noexcept_comparable<ptr_type const&, tp_elem* const&>)
  {
    return dango::compare(m_ptr, a_ptr.m_ptr);
  }

  auto dango_operator_hash()const noexcept->dango::hash_val{ return dango::hash(m_ptr); }

  constexpr void
  dango_operator_swap
  (auto_ptr& a_ptr)& noexcept
  {
    dango::swap(m_ptr, a_ptr.m_ptr);
    dango::swap(m_size, a_ptr.m_size);
    dango::swap(m_align, a_ptr.m_align);
  }
public:
  constexpr auto get()const noexcept->ptr_type{ return m_ptr; }

  [[nodiscard]] constexpr auto
  release()noexcept->release_type
  {
    dango_assert_nonnull(*this);

    return
      release_type
      {
        dango::exchange(m_ptr, dango::null),
        dango::exchange(m_size, size_type(0)),
        dango::exchange(m_align, size_type(0))
      };
  }

  constexpr auto size()const noexcept->size_type{ return m_size; }
  constexpr auto align()const noexcept->size_type{ return m_align; }
private:
  ptr_type m_ptr;
  size_type m_size;
  size_type m_align;
};

template
<dango::is_void tp_elem_type, dango::is_handle_based_allocator tp_allocator>
class
dango::
auto_ptr
<tp_elem_type, tp_allocator DANGO_GCC_BUG_81043_WORKAROUND_ID(3, tp_elem_type, tp_allocator)>
{
  template
  <typename tp_first, typename tp_second DANGO_GCC_BUG_81043_WORKAROUND_ND>
  friend class dango::auto_ptr;
public:
  using elem_type = tp_elem_type;
  using ptr_type = elem_type*;
  using allocator_type = tp_allocator;
  using allocator_handle_type = typename allocator_type::handle_type;
  using size_type = dango::usize;
  using release_type = dango::tuple<allocator_handle_type, ptr_type, size_type, size_type>;
public:
  explicit
  auto_ptr
  (size_type const a_size, size_type const a_align)
  noexcept(dango::is_noexcept_handle_based_allocator<allocator_type>)
  requires(dango::allocator_has_default_handle<allocator_type>):
  m_alloc_ptr{ dango::current_allocator_or_default<allocator_type>() },
  m_size{ a_size },
  m_align{ a_align }
  {
    m_ptr = m_alloc_ptr->alloc(a_size, a_align);
  }

  template
  <dango::is_same_ignore_cvref<allocator_handle_type> tp_handle>
  explicit
  auto_ptr
  (
    dango::allocator_arg_tag const,
    tp_handle&& a_handle,
    size_type const a_size,
    size_type const a_align
  )
  noexcept(dango::is_noexcept_handle_based_allocator<allocator_type>):
  m_alloc_ptr{ dango::forward<tp_handle>(a_handle) },
  m_size{ a_size },
  m_align{ a_align }
  {
    dango_assert_nonnull(m_alloc_ptr);

    m_ptr = m_alloc_ptr->alloc(a_size, a_align);
  }
public:
  constexpr
  auto_ptr
  (dango::null_tag const)noexcept:
  m_ptr{ dango::null },
  m_alloc_ptr{ dango::null },
  m_size{ size_type(0) },
  m_align{ size_type(0) }
  { }

  explicit constexpr
  auto_ptr()noexcept:
  auto_ptr{ dango::null }
  { }

  constexpr auto_ptr(auto_ptr const&)noexcept = delete;

  constexpr
  auto_ptr
  (auto_ptr&& a_ptr)noexcept:
  m_ptr{ dango::exchange(a_ptr.m_ptr, dango::null) },
  m_alloc_ptr{ dango::move(a_ptr).m_alloc_ptr },
  m_size{ dango::exchange(a_ptr.m_size, size_type(0)) },
  m_align{ dango::exchange(a_ptr.m_align, size_type(0)) }
  { }

  constexpr
  ~auto_ptr()noexcept
  {
    if(m_ptr)
    {
      dango_assert_nonnull(m_alloc_ptr);

      auto const a_size = m_size;
      auto const a_align = m_align;

      m_alloc_ptr->dealloc(dango::as_const(m_ptr), a_size, a_align);
    }
  }

  constexpr auto
  operator =
  (dango::null_tag const)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::null };

    dango::swap(*this, a_temp);

    return *this;
  }

  constexpr auto operator = (auto_ptr const&)& noexcept->auto_ptr& = delete;

  constexpr auto
  operator =
  (auto_ptr&& a_ptr)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(*this, a_temp);

    return *this;
  }

  explicit constexpr operator bool()const noexcept{ return !dango::is_null(*this); }

  explicit constexpr operator ptr_type()const noexcept{ return get(); }
public:
  template
  <typename tp_elem>
  requires
  (!dango::is_same<elem_type, tp_elem> && dango::detail::auto_ptr_valid_conversion_void<elem_type, tp_elem>)
  constexpr
  auto_ptr
  (dango::auto_ptr<tp_elem, allocator_type>&& a_ptr)noexcept:
  m_ptr{ dango::exchange(a_ptr.m_ptr, dango::null) },
  m_alloc_ptr{ dango::move(a_ptr).m_alloc_ptr },
  m_size{ dango::exchange(a_ptr.m_size, size_type(0)) },
  m_align{ dango::exchange(a_ptr.m_align, size_type(0)) }
  { }

  template
  <typename tp_elem>
  requires(!dango::is_same<elem_type, tp_elem> && dango::detail::auto_ptr_valid_conversion_void<elem_type, tp_elem>)
  constexpr auto
  operator =
  (dango::auto_ptr<tp_elem, allocator_type>&& a_ptr)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(*this, a_temp);

    return *this;
  }
public:
  constexpr auto dango_operator_is_null()const noexcept->bool{ return dango::is_null(m_ptr); }

  template
  <typename tp_elem, typename tp_deleter_or_allocator>
  constexpr auto
  dango_operator_equals
  (dango::auto_ptr<tp_elem, tp_deleter_or_allocator> const& a_ptr)const noexcept->bool
  requires(dango::is_noexcept_equatable<ptr_type const&, tp_elem* const&>)
  {
    return dango::equals(m_ptr, a_ptr.m_ptr);
  }

  template
  <typename tp_elem, typename tp_deleter_or_allocator>
  constexpr auto
  dango_operator_compare
  (dango::auto_ptr<tp_elem, tp_deleter_or_allocator> const& a_ptr)const noexcept->auto
  requires(dango::is_noexcept_comparable<ptr_type const&, tp_elem* const&>)
  {
    return dango::compare(m_ptr, a_ptr.m_ptr);
  }

  auto dango_operator_hash()const noexcept->dango::hash_val{ return dango::hash(m_ptr); }

  constexpr void
  dango_operator_swap
  (auto_ptr& a_ptr)& noexcept
  {
    dango::swap(m_ptr, a_ptr.m_ptr);
    dango::swap(m_alloc_ptr, a_ptr.m_alloc_ptr);
    dango::swap(m_size, a_ptr.m_size);
    dango::swap(m_align, a_ptr.m_align);
  }
public:
  constexpr auto get()const noexcept->ptr_type{ return m_ptr; }

  [[nodiscard]] constexpr auto
  release()noexcept->release_type
  {
    dango_assert_nonnull(*this);

    return
      release_type
      {
        dango::move(m_alloc_ptr),
        dango::exchange(m_ptr, dango::null),
        dango::exchange(m_size, size_type(0)),
        dango::exchange(m_align, size_type(0))
      };
  }

  constexpr auto
  get_allocator_handle()const& noexcept->allocator_handle_type const&
  {
    return m_alloc_ptr;
  }

  constexpr auto
  get_allocator_handle()& noexcept->allocator_handle_type const&
  {
    return m_alloc_ptr;
  }

  constexpr auto
  get_allocator_handle()const&& noexcept->allocator_handle_type
  {
    return m_alloc_ptr;
  }

  constexpr auto
  get_allocator_handle()&& noexcept->allocator_handle_type
  {
    if(m_ptr)
    {
      return m_alloc_ptr;
    }

    return dango::move(m_alloc_ptr);
  }

  constexpr auto size()const noexcept->size_type{ return m_size; }
  constexpr auto align()const noexcept->size_type{ return m_align; }
private:
  ptr_type m_ptr;
  allocator_handle_type m_alloc_ptr;
  size_type m_size;
  size_type m_align;
};

namespace
dango::custom
{
  template
  <typename tp_type, dango::is_allocator tp_alloc>
  requires(dango::is_void<tp_type> || dango::is_object_exclude_array<tp_type>)
  struct
  allocator_user<dango::auto_ptr<tp_type, tp_alloc>>
  final
  {
    using allocator_type = tp_alloc;

    DANGO_UNCONSTRUCTIBLE(allocator_user)
  };
}

namespace
dango
{
  template
  <dango::is_object_exclude_array tp_elem>
  explicit auto_ptr(tp_elem*)->auto_ptr<tp_elem, dango::plain_delete_type>;

  template
  <dango::is_object_exclude_array tp_elem, typename tp_del>
  explicit auto_ptr(tp_elem*, tp_del&&)->auto_ptr<tp_elem, dango::decay<tp_del>>;

  template
  <dango::is_emplacer tp_emplacer>
  explicit auto_ptr(dango::auto_ptr_construct_tag, tp_emplacer const&)->auto_ptr<dango::emplacer_return_type<tp_emplacer>, dango::polymorphic_allocator<>>;

  template
  <dango::is_same_ignore_cvref<dango::mem_resource_ptr<false>> tp_handle, dango::is_emplacer tp_emplacer>
  explicit auto_ptr(dango::auto_ptr_construct_tag, dango::allocator_arg_tag, tp_handle&&, tp_emplacer const&)->auto_ptr<dango::emplacer_return_type<tp_emplacer>, dango::polymorphic_allocator<false>>;
  template
  <dango::is_same_ignore_cvref<dango::mem_resource_ptr<true>> tp_handle, dango::is_emplacer tp_emplacer>
  explicit auto_ptr(dango::auto_ptr_construct_tag, dango::allocator_arg_tag, tp_handle&&, tp_emplacer const&)->auto_ptr<dango::emplacer_return_type<tp_emplacer>, dango::polymorphic_allocator<true>>;

  explicit auto_ptr(dango::usize, dango::usize)->auto_ptr<void, dango::basic_allocator>;

  template
  <dango::is_same_ignore_cvref<dango::mem_resource_ptr<false>> tp_handle>
  explicit auto_ptr(dango::allocator_arg_tag, tp_handle&&, dango::usize, dango::usize)->auto_ptr<void, dango::polymorphic_allocator<false>>;
  template
  <dango::is_same_ignore_cvref<dango::mem_resource_ptr<true>> tp_handle>
  explicit auto_ptr(dango::allocator_arg_tag, tp_handle&&, dango::usize, dango::usize)->auto_ptr<void, dango::polymorphic_allocator<true>>;

  template
  <dango::is_object_exclude_array tp_elem, typename... tp_args>
  requires
  (
    requires{ { new tp_elem{ dango::declval<tp_args>()... } }; } &&
    dango::is_noexcept_brace_constructible<dango::auto_ptr<tp_elem, dango::plain_delete_type>, tp_elem* const&>
  )
  constexpr auto
  make_auto_ptr
  (tp_args&&... a_args)
  noexcept(requires{ { new tp_elem{ dango::declval<tp_args>()... } }noexcept; })->auto
  {
    auto const a_ptr = new tp_elem{ dango::forward<tp_args>(a_args)... };

    return dango::auto_ptr<tp_elem, dango::plain_delete_type>{ a_ptr };
  }

  template
  <dango::is_void tp_elem>
  auto
  make_auto_ptr
  (dango::usize const a_size, dango::usize const a_align)dango_new_noexcept->auto
  {
    return dango::auto_ptr<tp_elem, dango::basic_allocator>{ a_size, a_align };
  }

  template
  <dango::is_object_exclude_array tp_elem, dango::is_allocator tp_allocator, typename... tp_args>
  requires(dango::is_brace_constructible<dango::auto_ptr<tp_elem, tp_allocator>, dango::auto_ptr_construct_tag const&, tp_args...>)
  constexpr auto
  make_auto_ptr
  (tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<dango::auto_ptr<tp_elem, tp_allocator>, dango::auto_ptr_construct_tag const&, tp_args...>)->auto
  {
    return dango::auto_ptr<tp_elem, tp_allocator>{ dango::auto_ptr_construct, dango::forward<tp_args>(a_args)... };
  }

  template
  <dango::is_object_exclude_array tp_elem, dango::is_handle_based_allocator tp_allocator, typename... tp_args>
  requires(dango::is_brace_constructible<dango::auto_ptr<tp_elem, tp_allocator>, dango::auto_ptr_construct_tag const&, dango::allocator_arg_tag const&, tp_args...>)
  constexpr auto
  make_auto_ptr
  (dango::allocator_arg_tag const, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<dango::auto_ptr<tp_elem, tp_allocator>, dango::auto_ptr_construct_tag const&, dango::allocator_arg_tag const&, tp_args...>)->auto
  {
    return dango::auto_ptr<tp_elem, tp_allocator>{ dango::auto_ptr_construct, dango::allocator_arg, dango::forward<tp_args>(a_args)... };
  }

  template
  <dango::is_void tp_elem, dango::is_allocator tp_allocator>
  constexpr auto
  make_auto_ptr
  (dango::usize const a_size, dango::usize const a_align)
  noexcept(dango::is_noexcept_allocator<tp_allocator>)->auto
  {
    return dango::auto_ptr<tp_elem, tp_allocator>{ a_size, a_align };
  }

  template
  <dango::is_void tp_elem, dango::is_handle_based_allocator tp_allocator, dango::is_same_ignore_cvref<typename tp_allocator::handle_type> tp_handle>
  constexpr auto
  make_auto_ptr
  (dango::allocator_arg_tag const, tp_handle&& a_handle, dango::usize const a_size, dango::usize const a_align)
  noexcept(dango::is_noexcept_handle_based_allocator<tp_allocator>)->auto
  {
    return dango::auto_ptr<tp_elem, tp_allocator>{ dango::allocator_arg, dango::forward<tp_handle>(a_handle), a_size, a_align };
  }
}

#endif // DANGO_AUTO_PTR_HPP_INCLUDED
