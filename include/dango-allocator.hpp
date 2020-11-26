#ifndef DANGO_ALLOCATOR_HPP_INCLUDED
#define DANGO_ALLOCATOR_HPP_INCLUDED

#include "dango-concepts.hpp"
#include "dango-concurrent.hpp"
#include "dango-hash.hpp"
#include "dango-mem.hpp"

/*** is_allocator ***/

namespace
dango
{
  template
  <typename tp_alloc>
  concept is_handle_based_allocator =
    dango::is_class<tp_alloc> && !dango::is_const_or_volatile<tp_alloc> &&
    requires{ typename tp_alloc::handle_type; typename tp_alloc::guard_type; } &&
    dango::is_object_exclude_array<typename tp_alloc::handle_type> && dango::is_object_exclude_array<typename tp_alloc::guard_type> &&
    !dango::is_const_or_volatile<typename tp_alloc::handle_type> && !dango::is_const_or_volatile<typename tp_alloc::guard_type> &&
    dango::is_noexcept_destructible<typename tp_alloc::guard_type> &&
    dango::is_nullable<typename tp_alloc::handle_type> &&
    dango::is_noexcept_copy_constructible<typename tp_alloc::handle_type> &&
    dango::is_noexcept_move_constructible<typename tp_alloc::handle_type> &&
    dango::is_noexcept_copy_assignable<typename tp_alloc::handle_type> &&
    dango::is_noexcept_move_assignable<typename tp_alloc::handle_type> &&
    dango::is_noexcept_equatable<typename tp_alloc::handle_type const&> &&
    dango::is_noexcept_swappable<typename tp_alloc::handle_type&> &&
    requires
    (
      typename tp_alloc::handle_type& a_handle_m,
      typename tp_alloc::handle_type const& a_handle_c,
      void const volatile* const a_voidp,
      dango::usize const a_usize,
      typename tp_alloc::guard_type& a_guard_m,
      typename tp_alloc::guard_type const& a_guard_c
    )
    {
      { a_handle_m->alloc(a_usize, a_usize) }->dango::is_same<void*>;
      { a_handle_c->alloc(a_usize, a_usize) }->dango::is_same<void*>;
      { dango::move(a_handle_m)->alloc(a_usize, a_usize) }->dango::is_same<void*>;
      { dango::move(a_handle_c)->alloc(a_usize, a_usize) }->dango::is_same<void*>;

      { a_handle_m->dealloc(a_voidp, a_usize, a_usize) }noexcept->dango::is_same<void>;
      { a_handle_c->dealloc(a_voidp, a_usize, a_usize) }noexcept->dango::is_same<void>;
      { dango::move(a_handle_m)->dealloc(a_voidp, a_usize, a_usize) }noexcept->dango::is_same<void>;
      { dango::move(a_handle_c)->dealloc(a_voidp, a_usize, a_usize) }noexcept->dango::is_same<void>;

      { tp_alloc::lock(a_handle_m) }noexcept->dango::is_same<typename tp_alloc::guard_type>;
      { tp_alloc::lock(a_handle_c) }noexcept->dango::is_same<typename tp_alloc::guard_type>;
      { tp_alloc::lock(dango::move(a_handle_m)) }noexcept->dango::is_same<typename tp_alloc::guard_type>;
      { tp_alloc::lock(dango::move(a_handle_c)) }noexcept->dango::is_same<typename tp_alloc::guard_type>;

      { a_guard_m->alloc(a_usize, a_usize) }->dango::is_same<void*>;
      { a_guard_c->alloc(a_usize, a_usize) }->dango::is_same<void*>;
      { dango::move(a_guard_m)->alloc(a_usize, a_usize) }->dango::is_same<void*>;
      { dango::move(a_guard_c)->alloc(a_usize, a_usize) }->dango::is_same<void*>;

      { a_guard_m->dealloc(a_voidp, a_usize, a_usize) }noexcept->dango::is_same<void>;
      { a_guard_c->dealloc(a_voidp, a_usize, a_usize) }noexcept->dango::is_same<void>;
      { dango::move(a_guard_m)->dealloc(a_voidp, a_usize, a_usize) }noexcept->dango::is_same<void>;
      { dango::move(a_guard_c)->dealloc(a_voidp, a_usize, a_usize) }noexcept->dango::is_same<void>;
    };

  template
  <typename tp_alloc>
  concept is_noexcept_handle_based_allocator =
    dango::is_handle_based_allocator<tp_alloc> &&
    requires
    (
      typename tp_alloc::handle_type& a_handle_m,
      typename tp_alloc::handle_type const& a_handle_c,
      dango::usize const a_usize,
      typename tp_alloc::guard_type& a_guard_m,
      typename tp_alloc::guard_type const& a_guard_c
    )
    {
      { a_handle_m->alloc(a_usize, a_usize) }noexcept->dango::is_same<void*>;
      { a_handle_c->alloc(a_usize, a_usize) }noexcept->dango::is_same<void*>;
      { dango::move(a_handle_m)->alloc(a_usize, a_usize) }noexcept->dango::is_same<void*>;
      { dango::move(a_handle_c)->alloc(a_usize, a_usize) }noexcept->dango::is_same<void*>;

      { a_guard_m->alloc(a_usize, a_usize) }noexcept->dango::is_same<void*>;
      { a_guard_c->alloc(a_usize, a_usize) }noexcept->dango::is_same<void*>;
      { dango::move(a_guard_m)->alloc(a_usize, a_usize) }noexcept->dango::is_same<void*>;
      { dango::move(a_guard_c)->alloc(a_usize, a_usize) }noexcept->dango::is_same<void*>;
    };

  template
  <typename tp_alloc>
  concept is_nohandle_allocator =
    dango::is_class<tp_alloc> && !dango::is_const_or_volatile<tp_alloc> &&
    !requires{ typename tp_alloc::handle_type; } && !requires{ typename tp_alloc::guard_type; } &&
    requires(void const volatile* const a_voidp, dango::usize const a_usize)
    {
      { tp_alloc::alloc(a_usize, a_usize) }->dango::is_same<void*>;
      { tp_alloc::dealloc(a_voidp, a_usize, a_usize) }noexcept->dango::is_same<void>;
    };

  template
  <typename tp_alloc>
  concept is_noexcept_nohandle_allocator =
    dango::is_nohandle_allocator<tp_alloc> &&
    requires(void const volatile* const a_voidp, dango::usize const a_usize)
    {
      { tp_alloc::alloc(a_usize, a_usize) }noexcept->dango::is_same<void*>;
    };

  template
  <typename tp_alloc>
  concept is_allocator =
    dango::is_handle_based_allocator<tp_alloc> ||
    dango::is_nohandle_allocator<tp_alloc>;

  template
  <typename tp_alloc>
  concept is_noexcept_allocator =
    dango::is_allocator<tp_alloc> &&
    (dango::is_noexcept_handle_based_allocator<tp_alloc> || dango::is_noexcept_nohandle_allocator<tp_alloc>);
}

namespace
dango::detail
{
  template
  <typename tp_alloc, typename tp_default>
  struct
  allocator_handle_type_help
  final
  {
    using type = tp_default;

    DANGO_UNCONSTRUCTIBLE(allocator_handle_type_help)
  };

  template
  <dango::is_handle_based_allocator tp_alloc, typename tp_default>
  struct
  allocator_handle_type_help<tp_alloc, tp_default>
  final
  {
    using type = typename tp_alloc::handle_type;

    DANGO_UNCONSTRUCTIBLE(allocator_handle_type_help)
  };
}

namespace
dango
{
  template
  <typename tp_alloc, typename tp_default = tp_alloc>
  using allocator_handle_type =
    typename dango::detail::allocator_handle_type_help<tp_alloc, tp_default>::type;
}

namespace
dango::detail
{
  template
  <typename tp_alloc, typename tp_default>
  struct
  allocator_guard_type_help
  final
  {
    using type = tp_default;

    DANGO_UNCONSTRUCTIBLE(allocator_guard_type_help)
  };

  template
  <dango::is_handle_based_allocator tp_alloc, typename tp_default>
  struct
  allocator_guard_type_help<tp_alloc, tp_default>
  final
  {
    using type = typename tp_alloc::guard_type;

    DANGO_UNCONSTRUCTIBLE(allocator_guard_type_help)
  };
}

namespace
dango
{
  template
  <typename tp_alloc, typename tp_default = tp_alloc>
  using allocator_guard_type =
    typename dango::detail::allocator_guard_type_help<tp_alloc, tp_default>::type;
}

/*** allocator_traits ***/

namespace
dango::custom
{
  template
  <typename tp_alloc>
  struct allocator_traits;
}

namespace
dango
{
  template
  <typename tp_alloc>
  concept allocator_has_default_handle =
    dango::is_handle_based_allocator<tp_alloc> &&
    requires{ { dango::custom::allocator_traits<tp_alloc>::get_default_handle() }noexcept->dango::is_same_ignore_cvref<typename tp_alloc::handle_type>; };
}

/*** allocator_user ***/

namespace
dango::custom
{
  template
  <typename tp_type>
  struct allocator_user;
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_allocator_user =
    dango::is_class_or_union<tp_type> &&
    requires{ typename dango::custom::allocator_user<dango::remove_cv<tp_type>>::allocator_type; } &&
    dango::is_allocator<typename dango::custom::allocator_user<dango::remove_cv<tp_type>>::allocator_type>;

  template
  <typename tp_type, typename tp_alloc>
  concept is_allocator_user_allocator_same =
    dango::is_allocator_user<tp_type> &&
    dango::is_same<tp_alloc, typename dango::custom::allocator_user<dango::remove_cv<tp_type>>::allocator_type>;

  template
  <dango::is_allocator_user tp_type>
  using allocator_user_allocator_type =
    typename dango::custom::allocator_user<dango::remove_cv<tp_type>>::allocator_type;
}

/*** basic_allocator ***/

namespace
dango
{
  struct basic_allocator;
}

struct
dango::
basic_allocator
{
#ifndef DANGO_NO_DEBUG
private:
  using count_type = dango::usize;
  static inline constinit dango::spin_mutex s_lock{ };
  static inline constinit count_type s_count{ 0 };
#endif
public:
  static auto
  alloc
  (dango::usize const a_size, dango::usize const a_align)dango_new_noexcept->void*
  {
#ifndef DANGO_NO_DEBUG
    dango_crit(s_lock)
    {
      ++s_count;
    }
#endif
    return dango::operator_new(a_size, a_align);
  }

  static void
  dealloc
  (void const volatile* const a_ptr, dango::usize const a_size, dango::usize const a_align)noexcept
  {
#ifndef DANGO_NO_DEBUG
    dango_crit(s_lock)
    {
      dango_assert_msg(s_count != count_type(0), u8"basic_allocator: extra dealloc detected");

      --s_count;
    }
#endif
    dango::operator_delete(a_ptr, a_size, a_align);
  }
public:
  DANGO_UNCONSTRUCTIBLE(basic_allocator)
};

/*** allocator_traits for basic_allocator ***/

namespace
dango::custom
{
  template<>
  struct
  allocator_traits<dango::basic_allocator>
  final
  {
    DANGO_UNCONSTRUCTIBLE(allocator_traits)
  };
}

/*** polymorphic_allocator ***/

namespace
dango
{
  template
  <bool tp_noexcept = dango::c_operator_new_noexcept>
  struct polymorphic_allocator;

  template
  <bool tp_noexcept = dango::c_operator_new_noexcept>
  using mem_resource = typename dango::polymorphic_allocator<tp_noexcept>::mem_resource;

  template
  <bool tp_noexcept = dango::c_operator_new_noexcept>
  using mem_resource_ptr = typename dango::polymorphic_allocator<tp_noexcept>::handle_type;

  template
  <bool tp_noexcept = dango::c_operator_new_noexcept>
  using mem_resource_guard = typename dango::polymorphic_allocator<tp_noexcept>::guard_type;

  template
  <typename tp_mr, bool tp_noexcept>
  concept mem_resource_storage_constraint_spec =
    dango::is_class<tp_mr> && !dango::is_const_or_volatile<tp_mr> &&
    dango::is_base_of<dango::mem_resource<tp_noexcept>, tp_mr> &&
    dango::is_convertible_arg<tp_mr*, dango::mem_resource<tp_noexcept>*> &&
    dango::is_noexcept_destructible<tp_mr>;
}

template
<bool tp_noexcept>
struct
dango::
polymorphic_allocator
final
{
private:
  struct
  privacy_tag
  final
  {
    DANGO_TAG_TYPE(privacy_tag)
  };
public:
  class mem_resource;
  class mem_resource_ptr;
  using handle_type = mem_resource_ptr;
#ifndef DANGO_NO_DEBUG
  class mem_resource_guard;
  using guard_type = mem_resource_guard;
#else
  using guard_type = mem_resource*;
#endif
  template
  <dango::mem_resource_storage_constraint_spec<tp_noexcept> tp_mr>
  class mem_resource_storage;

  template
  <dango::mem_resource_storage_constraint_spec<tp_noexcept> tp_mr>
  requires(dango::is_trivial_destructible<tp_mr>)
  class mem_resource_storage_static;
public:
  static auto lock(handle_type const&)noexcept->guard_type;

  template
  <dango::mem_resource_storage_constraint_spec<tp_noexcept> tp_mr, typename... tp_args>
  requires(dango::is_brace_constructible<tp_mr, tp_args...>)
  static auto
  make
  (tp_args&&... a_args)
  dango_new_noexcept_and(dango::is_noexcept_brace_constructible<tp_mr, tp_args...>)->mem_resource_storage<tp_mr>
  {
    return mem_resource_storage<tp_mr>{ privacy_tag{ }, dango::forward<tp_args>(a_args)... };
  }

  template
  <dango::mem_resource_storage_constraint_spec<tp_noexcept> tp_mr, typename... tp_args>
  requires(dango::is_trivial_destructible<tp_mr> && dango::is_noexcept_brace_constructible<tp_mr, tp_args...>)
  static constexpr auto
  make_static
  (tp_args&&... a_args)noexcept->mem_resource_storage_static<tp_mr>
  {
    return mem_resource_storage_static<tp_mr>{ privacy_tag{ }, dango::forward<tp_args>(a_args)... };
  }
private:
  class control_base;

  template
  <dango::mem_resource_storage_constraint_spec<tp_noexcept> tp_mr>
  class control_dynamic;

  template
  <dango::mem_resource_storage_constraint_spec<tp_noexcept> tp_mr>
  requires(dango::is_trivial_destructible<tp_mr>)
  class control_static;
public:
  DANGO_UNCONSTRUCTIBLE(polymorphic_allocator)
};

template
<bool tp_noexcept>
class
dango::
polymorphic_allocator<tp_noexcept>::
mem_resource
{
#ifndef DANGO_NO_DEBUG
private:
  using count_type = dango::usize;
#endif
protected:
#ifndef DANGO_NO_DEBUG
  explicit constexpr
  mem_resource()noexcept:
  m_lock{ },
  m_count{ count_type(0) }
  { }
#else
  explicit constexpr mem_resource()noexcept{ }
#endif
  constexpr ~mem_resource()noexcept = default;
public:
  auto
  alloc
  (dango::usize const a_size, dango::usize const a_align)noexcept(tp_noexcept)->void*
  {
#ifndef DANGO_NO_DEBUG
    dango_crit(m_lock)
    {
      ++m_count;
    }
#endif
    return override_alloc(a_size, a_align);
  }

  void
  dealloc
  (void const volatile* const a_ptr, dango::usize const a_size, dango::usize const a_align)noexcept
  {
#ifndef DANGO_NO_DEBUG
    dango_crit(m_lock)
    {
      dango_assert_msg(m_count != count_type(0), u8"mem_resource: extra dealloc detected");

      --m_count;
    }
#endif
    override_dealloc(a_ptr, a_size, a_align);
  }
private:
  virtual auto override_alloc(dango::usize, dango::usize)noexcept(tp_noexcept)->void* = 0;
  virtual void override_dealloc(void const volatile*, dango::usize, dango::usize)noexcept(tp_noexcept) = 0;
#ifndef DANGO_NO_DEBUG
private:
  dango::spin_mutex m_lock;
  count_type m_count;
#endif
public:
  DANGO_UNMOVEABLE(mem_resource)
};

template
<bool tp_noexcept>
class
dango::
polymorphic_allocator<tp_noexcept>::
control_base
{
#ifndef DANGO_NO_DEBUG
public:
  using destroy_func = void(*)(control_base*)noexcept;
#endif
public:
  DANGO_DELETE_CLASS_OPERATOR_NEW_DELETE
protected:
  explicit constexpr
  control_base(mem_resource* const a_resource)noexcept:
  m_resource{ a_resource }
  { }

  constexpr ~control_base()noexcept = default;
public:
  constexpr auto get_resource_ptr()const noexcept->auto{ return dango::launder(m_resource); }
#ifndef DANGO_NO_DEBUG
public:
  virtual void weak_increment()noexcept{ }
  virtual auto try_strong_increment()noexcept->bool{ return true; }
  virtual auto weak_decrement()noexcept->bool{ return false; }
  virtual auto strong_decrement()noexcept->bool{ return false; }
  virtual auto get_destroy_func()const noexcept->destroy_func{ return dango::null; }
#endif
private:
  mem_resource* const m_resource;
public:
  DANGO_DELETE_DEFAULT(control_base)
  DANGO_UNMOVEABLE(control_base)
};

template
<bool tp_noexcept>
template
<dango::mem_resource_storage_constraint_spec<tp_noexcept> tp_mr>
class
dango::
polymorphic_allocator<tp_noexcept>::
control_dynamic
final:
public control_base
{
private:
  using super_type = control_base;
  using resource_type = tp_mr;
#ifndef DANGO_NO_DEBUG
  using storage_type = dango::aligned_union<resource_type, dango::cache_align_type>;
  using destroy_func = typename super_type::destroy_func;
#endif
public:
  DANGO_DEFINE_CLASS_OPERATOR_NEW_DELETE(control_dynamic)
public:
  template
  <typename... tp_args>
  explicit
  control_dynamic(privacy_tag const, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<resource_type, tp_args...>):
#ifndef DANGO_NO_DEBUG
  super_type{ dango::aligned_storage_get_as<resource_type>(m_storage) },
  m_storage{ },
  m_count{ }
  {
    dango_placement_new(m_storage.get(), resource_type, { dango::forward<tp_args>(a_args)... });
  }
#else
  super_type{ dango::addressof(m_resource) },
  m_resource{ dango::forward<tp_args>(a_args)... }
  { }
#endif

  ~control_dynamic()noexcept = default;
#ifndef DANGO_NO_DEBUG
public:
  virtual void weak_increment()noexcept override{ m_count.weak_increment(); }
  virtual auto try_strong_increment()noexcept->bool override{ return m_count.try_strong_increment(); }
  virtual auto weak_decrement()noexcept->bool override{ return m_count.weak_decrement(); }

  virtual auto
  strong_decrement()noexcept->bool override
  {
    auto const a_resource = m_storage.template launder_get<resource_type>();

    auto const a_dispose =
      [a_resource]()noexcept->void
      {
        dango::qualified_destructor(a_resource);
      };

    return m_count.strong_decrement(a_dispose);
  }

  virtual auto
  get_destroy_func()const noexcept->destroy_func override
  {
    static constexpr auto const c_destroy =
      [](control_base* const a_control)noexcept->void
      {
        delete static_cast<control_dynamic*>(a_control);
      };

    return c_destroy;
  }
#endif
private:
#ifndef DANGO_NO_DEBUG
  storage_type m_storage;
  dango::atomic_ref_count_ws m_count;
#else
  resource_type m_resource;
#endif
public:
  DANGO_DELETE_DEFAULT(control_dynamic)
  DANGO_UNMOVEABLE(control_dynamic)
};

template
<bool tp_noexcept>
template
<dango::mem_resource_storage_constraint_spec<tp_noexcept> tp_mr>
requires(dango::is_trivial_destructible<tp_mr>)
class
dango::
polymorphic_allocator<tp_noexcept>::
control_static
final:
public control_base
{
private:
  using super_type = control_base;
  using resource_type = tp_mr;
public:
  DANGO_DELETE_CLASS_OPERATOR_NEW_DELETE
public:
  template
  <typename... tp_args>
  explicit constexpr
  control_static
  (privacy_tag const, tp_args&&... a_args)noexcept:
  super_type{ dango::addressof(m_resource) },
  m_resource{ dango::forward<tp_args>(a_args)... }
  { }

  constexpr ~control_static()noexcept = default;
private:
  resource_type m_resource;
public:
  DANGO_DELETE_DEFAULT(control_static)
  DANGO_UNMOVEABLE(control_static)
};

template
<bool tp_noexcept>
template
<dango::mem_resource_storage_constraint_spec<tp_noexcept> tp_mr>
class
dango::
polymorphic_allocator<tp_noexcept>::
mem_resource_storage
final
{
private:
  using resource_type = tp_mr;
  using control_type = control_dynamic<resource_type>;
public:
  template
  <typename... tp_args>
  explicit
  mem_resource_storage
  (privacy_tag const a_tag, tp_args&&... a_args)
  dango_new_noexcept_and(dango::is_noexcept_brace_constructible<resource_type, tp_args...>):
  m_control{ new control_type{ a_tag, dango::forward<tp_args>(a_args)... } }
  { }

  ~mem_resource_storage()noexcept
  {
#ifndef DANGO_NO_DEBUG
    if(!m_control->strong_decrement())
    {
      return;
    }
#endif
    delete m_control;
  }
public:
  auto
  get_ptr()noexcept->dango::mem_resource_ptr<tp_noexcept>
  {
    return handle_type{ privacy_tag{ }, m_control };
  }
private:
  control_type* const m_control;
public:
  DANGO_DELETE_DEFAULT(mem_resource_storage)
  DANGO_UNMOVEABLE(mem_resource_storage)
};

template
<bool tp_noexcept>
template
<dango::mem_resource_storage_constraint_spec<tp_noexcept> tp_mr>
requires(dango::is_trivial_destructible<tp_mr>)
class
dango::
polymorphic_allocator<tp_noexcept>::
mem_resource_storage_static
final
{
private:
  using resource_type = tp_mr;
  using control_type = control_static<resource_type>;
public:
  template
  <typename... tp_args>
  explicit constexpr
  mem_resource_storage_static
  (privacy_tag const a_tag, tp_args&&... a_args)noexcept:
  m_control{ a_tag, dango::forward<tp_args>(a_args)... }
  { }

  constexpr ~mem_resource_storage_static()noexcept = default;
public:
  auto
  get_ptr()noexcept->dango::mem_resource_ptr<tp_noexcept>
  {
    return handle_type{ privacy_tag{ }, &m_control };
  }
private:
  control_type m_control;
public:
  DANGO_DELETE_DEFAULT(mem_resource_storage_static)
  DANGO_UNMOVEABLE(mem_resource_storage_static)
};

template
<bool tp_noexcept>
class
dango::
polymorphic_allocator<tp_noexcept>::
mem_resource_ptr
final
{
private:
  using control_type = control_base;
public:
  explicit
  mem_resource_ptr
  (privacy_tag const, control_type* const a_control)noexcept:
  m_control{ a_control }
  {
    dango_assert(a_control != dango::null);
#ifndef DANGO_NO_DEBUG
    a_control->weak_increment();
#endif
  }

  explicit constexpr
  mem_resource_ptr()noexcept:
  m_control{ dango::null }
  { }

  constexpr
  mem_resource_ptr
  (dango::null_tag const)noexcept:
  m_control{ dango::null }
  { }

#ifndef DANGO_NO_DEBUG
  constexpr
  mem_resource_ptr
  (mem_resource_ptr const& a_arg)noexcept:
  m_control{ a_arg.m_control }
  {
    if(m_control)
    {
      m_control->weak_increment();
    }
  }
#else
  constexpr mem_resource_ptr(mem_resource_ptr const&)noexcept = default;
#endif

  constexpr
  mem_resource_ptr
  (mem_resource_ptr&& a_arg)noexcept:
  m_control{ dango::exchange(a_arg.m_control, dango::null) }
  { }

#ifndef DANGO_NO_DEBUG
  constexpr
  ~mem_resource_ptr()noexcept
  {
    auto const a_control = m_control;

    if(a_control && a_control->weak_decrement())
    {
      auto const a_destroy_func = a_control->get_destroy_func();

      dango_assert_nonnull(a_destroy_func);

      a_destroy_func(a_control);
    }
  }
#else
  constexpr ~mem_resource_ptr()noexcept = default;
#endif

  DANGO_DEFINE_NULL_SWAP_ASSIGN(mem_resource_ptr, constexpr, true)
#ifndef DANGO_NO_DEBUG
  DANGO_DEFINE_COPY_SWAP_ASSIGN(mem_resource_ptr, constexpr, true)
#else
  constexpr auto operator = (mem_resource_ptr const&)& noexcept->mem_resource_ptr& = default;
#endif
  DANGO_DEFINE_MOVE_SWAP_ASSIGN(mem_resource_ptr, constexpr, true)

  explicit constexpr operator bool()const noexcept{ return !dango::is_null(*this); }
  auto operator -> ()const noexcept->dango::mem_resource_guard<tp_noexcept>;
public:
  constexpr auto
  dango_operator_is_null()const noexcept->bool
  {
    return dango::is_null(m_control);
  }

  constexpr auto
  dango_operator_equals
  (mem_resource_ptr const& a_arg)const noexcept->bool
  {
    return dango::equals(m_control, a_arg.m_control);
  }

  constexpr auto
  dango_operator_compare
  (mem_resource_ptr const& a_arg)const noexcept->auto
  {
    return dango::compare(m_control, a_arg.m_control);
  }

  auto
  dango_operator_hash()const noexcept->auto
  {
    return dango::hash(m_control);
  }

  constexpr void
  dango_operator_swap
  (mem_resource_ptr& a_arg)& noexcept
  {
    dango::swap(m_control, a_arg.m_control);
  }
private:
  control_type* m_control;
};

#ifndef DANGO_NO_DEBUG
template
<bool tp_noexcept>
class
dango::
polymorphic_allocator<tp_noexcept>::
mem_resource_guard
final
{
public:
  using control_type = control_base;
public:
  explicit constexpr
  mem_resource_guard
  (privacy_tag const, control_type* const a_control)noexcept:
  m_control{ a_control }
  {
    dango_assert_nonnull(a_control);
  }

  constexpr
  ~mem_resource_guard()noexcept
  {
    auto const a_control = m_control;

    if(a_control->strong_decrement())
    {
      auto const a_destroy_func = a_control->get_destroy_func();

      dango_assert_nonnull(a_destroy_func);

      a_destroy_func(a_control);
    }
  }
public:
  auto
  operator ->
  ()const noexcept->dango::mem_resource<tp_noexcept>*
  {
    return m_control->get_resource_ptr();
  }
private:
  control_type* const m_control;
public:
  DANGO_DELETE_DEFAULT(mem_resource_guard)
  DANGO_UNMOVEABLE(mem_resource_guard)
};

template
<bool tp_noexcept>
auto
dango::
polymorphic_allocator<tp_noexcept>::
mem_resource_ptr::
operator ->
()const noexcept->dango::mem_resource_guard<tp_noexcept>
{
  dango_assert_nonnull(*this);

  bool const a_alive = m_control->try_strong_increment();

  dango_assert_msg(a_alive, u8"mem_resource_ptr: detected use of memory resource after its destruction");

  return guard_type{ privacy_tag{ }, m_control };
}
#else
template
<bool tp_noexcept>
auto
dango::
polymorphic_allocator<tp_noexcept>::
mem_resource_ptr::
operator ->
()const noexcept->dango::mem_resource_guard<tp_noexcept>
{
  dango_assert_nonnull(*this);

  return m_control->get_resource_ptr();
}
#endif

template
<bool tp_noexcept>
auto
dango::
polymorphic_allocator<tp_noexcept>::
lock
(handle_type const& a_ptr)noexcept->guard_type
{
  return a_ptr.operator -> ();
}

/*** basic_memory_resource ***/

namespace
dango
{
  class basic_mem_resource;
}

class
dango::
basic_mem_resource:
public dango::mem_resource<dango::c_operator_new_noexcept>
{
private:
  using super_type = dango::mem_resource<dango::c_operator_new_noexcept>;
public:
  explicit constexpr basic_mem_resource()noexcept:super_type{ }{ }
  constexpr ~basic_mem_resource()noexcept = default;
public:
  auto
  override_alloc
  (dango::usize const a_size, dango::usize const a_align)dango_new_noexcept->void* override
  {
    return dango::operator_new(a_size, a_align);
  }

  void
  override_dealloc
  (void const volatile* const a_ptr, dango::usize const a_size, dango::usize const a_align)noexcept override
  {
    dango::operator_delete(a_ptr, a_size, a_align);
  }
public:
  DANGO_UNMOVEABLE(basic_mem_resource)
};

namespace
dango
{
  using default_mem_resource_storage_type =
    dango::polymorphic_allocator<dango::c_operator_new_noexcept>::mem_resource_storage_static<dango::basic_mem_resource>;

  DANGO_EXPORT_ONLY extern dango::default_mem_resource_storage_type s_default_mem_resource;

  inline auto
  default_mem_resource_ptr
  ()noexcept->auto
  {
    return s_default_mem_resource.get_ptr();
  }
}

/*** allocator_traits for polymorphic_allocator ***/

namespace
dango::custom
{
  template<>
  struct
  allocator_traits<dango::polymorphic_allocator<dango::c_operator_new_noexcept>>
  final
  {
    static auto
    get_default_handle()noexcept->auto
    {
      return dango::default_mem_resource_ptr();
    }

    DANGO_UNCONSTRUCTIBLE(allocator_traits)
  };

  template<>
  struct
  allocator_traits<dango::polymorphic_allocator<!dango::c_operator_new_noexcept>>
  final
  {
    DANGO_UNCONSTRUCTIBLE(allocator_traits)
  };
}

/*** push_allocator ***/

namespace
dango::detail
{
  template
  <dango::is_handle_based_allocator tp_alloc>
  struct
  current_alloc
  final
  {
    using handle_ptr = typename tp_alloc::handle_type*;

    static auto value()noexcept->handle_ptr&;

    DANGO_UNCONSTRUCTIBLE(current_alloc)
  };
}

template
<dango::is_handle_based_allocator tp_alloc>
auto
dango::
detail::
current_alloc<tp_alloc>::
value()noexcept->handle_ptr&
{
  thread_local constinit handle_ptr t_current = dango::null;

  return t_current;
}

DANGO_DECLARE_CURRENT_ALLOC_EXPLICIT_INSTANTIATION(dango::polymorphic_allocator<false>)
DANGO_DECLARE_CURRENT_ALLOC_EXPLICIT_INSTANTIATION(dango::polymorphic_allocator<true>)

#ifndef DANGO_NO_DEBUG
namespace
dango::detail::test
{
  DANGO_EXPORT auto current_alloc_address_extern()noexcept->void const*;

  inline auto
  current_alloc_address_inline()noexcept->void const*
  {
    return dango::addressof(dango::detail::current_alloc<dango::polymorphic_allocator<>>::value());
  }
}
#endif

namespace
dango::detail
{
  template
  <dango::is_handle_based_allocator tp_alloc>
  class allocator_pusher;
}

template
<dango::is_handle_based_allocator tp_alloc>
class
dango::
detail::
allocator_pusher
final
{
private:
  using handle_type = typename tp_alloc::handle_type;
  using current_alloc = dango::detail::current_alloc<tp_alloc>;
public:
  template
  <dango::is_same_ignore_cvref<handle_type> tp_handle>
  explicit
  allocator_pusher
  (tp_handle&& a_handle)noexcept:
  m_current{ dango::forward<tp_handle>(a_handle) },
  m_prev{ current_alloc::value() },
  m_active{ true }
  {
    current_alloc::value() = dango::addressof(m_current);
  }

  explicit
  allocator_pusher
  (dango::null_tag const)noexcept:
  m_current{ dango::null },
  m_prev{ dango::null },
  m_active{ false }
  { }

  ~allocator_pusher()noexcept{ pop(); }
public:
  void
  pop()noexcept
  {
    if(m_active)
    {
      current_alloc::value() = m_prev;

      m_active = false;
    }
  }
private:
  handle_type m_current;
  handle_type* m_prev;
  bool m_active;
public:
  DANGO_DELETE_DEFAULT(allocator_pusher)
  DANGO_UNMOVEABLE(allocator_pusher)
};

namespace
dango
{
  template
  <dango::is_handle_based_allocator tp_alloc, dango::is_same_ignore_cvref<typename tp_alloc::handle_type> tp_handle>
  auto
  current_allocator
  (tp_handle&& a_default)noexcept->auto
  {
    using return_type = typename tp_alloc::handle_type;
    using current_alloc = dango::detail::current_alloc<tp_alloc>;

    dango_assert_nonnull(a_default);

    auto const a_current = current_alloc::value();

    if(dango::is_null(a_current))
    {
      return return_type{ dango::forward<tp_handle>(a_default) };
    }

    return return_type{ *a_current };
  }

  template
  <dango::allocator_has_default_handle tp_alloc>
  auto
  current_allocator_or_default()noexcept->auto
  {
    return dango::current_allocator<tp_alloc>(dango::custom::allocator_traits<tp_alloc>::get_default_handle());
  }

  template
  <dango::is_handle_based_allocator tp_alloc, dango::is_same_ignore_cvref<typename tp_alloc::handle_type> tp_handle>
  [[nodiscard]] auto
  push_allocator
  (tp_handle&& a_handle)noexcept->auto
  {
    using return_type = dango::detail::allocator_pusher<tp_alloc>;

    dango_assert_nonnull(a_handle);

    return return_type{ dango::forward<tp_handle>(a_handle) };
  }

  template
  <dango::is_handle_based_allocator tp_alloc, dango::is_same_ignore_cvref<typename tp_alloc::handle_type> tp_handle>
  [[nodiscard]] auto
  push_allocator_if_no_current
  (tp_handle&& a_handle)noexcept->auto
  {
    using return_type = dango::detail::allocator_pusher<tp_alloc>;
    using current_alloc = dango::detail::current_alloc<tp_alloc>;

    dango_assert_nonnull(a_handle);

    auto const a_current = current_alloc::value();

    if(dango::is_null(a_current))
    {
      return return_type{ dango::forward<tp_handle>(a_handle) };
    }

    return return_type{ dango::null };
  }

  template
  <dango::is_handle_based_allocator tp_alloc, typename... tp_users, dango::is_same_ignore_cvref<typename tp_alloc::handle_type> tp_handle>
  [[nodiscard]] auto
  push_allocator_if_user
  (tp_handle&& a_handle)noexcept->auto
  {
    using return_type = dango::detail::allocator_pusher<tp_alloc>;

    if constexpr(( ... || dango::is_allocator_user_allocator_same<tp_users, tp_alloc>))
    {
      return dango::push_allocator<tp_alloc>(dango::forward<tp_handle>(a_handle));
    }
    else
    {
      return return_type{ dango::null };
    }
  }

  template
  <dango::is_handle_based_allocator tp_alloc, typename... tp_users, dango::is_same_ignore_cvref<typename tp_alloc::handle_type> tp_handle>
  [[nodiscard]] auto
  push_allocator_if_user_and_no_current
  (tp_handle&& a_handle)noexcept->auto
  {
    using return_type = dango::detail::allocator_pusher<tp_alloc>;

    if constexpr(( ... || dango::is_allocator_user_allocator_same<tp_users, tp_alloc>))
    {
      return dango::push_allocator_if_no_current<tp_alloc>(dango::forward<tp_handle>(a_handle));
    }
    else
    {
      return return_type{ dango::null };
    }
  }
}

/*** allocator_alloc allocator_dealloc ***/

namespace
dango::detail
{
  template
  <dango::is_nohandle_allocator tp_alloc>
  [[nodiscard, gnu::malloc, gnu::alloc_size(1)]] auto
  allocator_alloc_help
  (dango::usize const a_size, dango::usize const a_align)
  noexcept(dango::is_noexcept_nohandle_allocator<tp_alloc>)->void*
  {
    return tp_alloc::alloc(a_size, a_align);
  }

  template
  <dango::is_nohandle_allocator tp_alloc>
  void
  allocator_dealloc_help
  (void const volatile* const a_ptr, dango::usize const a_size, dango::usize const a_align)noexcept
  {
    return tp_alloc::dealloc(a_ptr, a_size, a_align);
  }

  template
  <dango::is_handle_based_allocator tp_alloc, typename tp_handle>
  requires(dango::is_same<tp_handle, typename tp_alloc::handle_type> && !dango::is_same<tp_handle, typename tp_alloc::guard_type>)
  [[nodiscard, gnu::malloc, gnu::alloc_size(2)]] auto
  allocator_alloc_help
  (tp_handle const& a_handle, dango::usize const a_size, dango::usize const a_align)
  noexcept(dango::is_noexcept_handle_based_allocator<tp_alloc>)->void*
  {
    dango_assert_nonnull(a_handle);

    return a_handle->alloc(a_size, a_align);
  }

  template
  <dango::is_handle_based_allocator tp_alloc, typename tp_handle>
  requires(dango::is_same<tp_handle, typename tp_alloc::handle_type> && !dango::is_same<tp_handle, typename tp_alloc::guard_type>)
  void
  allocator_dealloc_help
  (tp_handle const& a_handle, void const volatile* const a_ptr, dango::usize const a_size, dango::usize const a_align)noexcept
  {
    dango_assert_nonnull(a_handle);

    return a_handle->dealloc(a_ptr, a_size, a_align);
  }

  template
  <dango::is_handle_based_allocator tp_alloc, typename tp_guard>
  requires(dango::is_same<tp_guard, typename tp_alloc::guard_type>)
  [[nodiscard, gnu::malloc, gnu::alloc_size(2)]] auto
  allocator_alloc_help
  (tp_guard const& a_guard, dango::usize const a_size, dango::usize const a_align)
  noexcept(dango::is_noexcept_handle_based_allocator<tp_alloc>)->void*
  {
    return a_guard->alloc(a_size, a_align);
  }

  template
  <dango::is_handle_based_allocator tp_alloc, typename tp_guard>
  requires(dango::is_same<tp_guard, typename tp_alloc::guard_type>)
  void
  allocator_dealloc_help
  (tp_guard const& a_guard, void const volatile* const a_ptr, dango::usize const a_size, dango::usize const a_align)noexcept
  {
    return a_guard->dealloc(a_ptr, a_size, a_align);
  }
}

namespace
dango
{
  template
  <typename tp_alloc>
  [[nodiscard]] auto
  allocator_alloc
  (dango::usize a_size, dango::usize const a_align)
  noexcept(requires{ { dango::detail::allocator_alloc_help<tp_alloc>(a_size, a_align) }noexcept; })->void*
  requires(requires{ { dango::detail::allocator_alloc_help<tp_alloc>(a_size, a_align) }; })
  {
    dango_assert(dango::is_pow_two(a_align));

    a_size = dango::next_multiple(dango::max(dango::usize(1), a_size), a_align);

    return dango::detail::allocator_alloc_help<tp_alloc>(a_size, a_align);
  }

  template
  <typename tp_alloc>
  void
  allocator_dealloc
  (void const volatile* const a_ptr, dango::usize a_size, dango::usize const a_align)
  noexcept(requires{ { dango::detail::allocator_dealloc_help<tp_alloc>(a_ptr, a_size, a_align) }noexcept; })
  requires(requires{ { dango::detail::allocator_dealloc_help<tp_alloc>(a_ptr, a_size, a_align) }; })
  {
    dango_assert_nonnull(a_ptr);
    dango_assert(dango::is_pow_two(a_align));

    a_size = dango::next_multiple(dango::max(dango::usize(1), a_size), a_align);

    dango::detail::allocator_dealloc_help<tp_alloc>(a_ptr, a_size, a_align);
  }

  template
  <typename tp_alloc, typename tp_handle>
  [[nodiscard]] auto
  allocator_alloc
  (tp_handle const& a_handle, dango::usize a_size, dango::usize const a_align)
  noexcept(requires{ { dango::detail::allocator_alloc_help<tp_alloc>(a_handle, a_size, a_align) }noexcept; })->void*
  requires(requires{ { dango::detail::allocator_alloc_help<tp_alloc>(a_handle, a_size, a_align) }; })
  {
    dango_assert(dango::is_pow_two(a_align));

    a_size = dango::next_multiple(dango::max(dango::usize(1), a_size), a_align);

    return dango::detail::allocator_alloc_help<tp_alloc>(a_handle, a_size, a_align);
  }

  template
  <typename tp_alloc, typename tp_handle>
  void
  allocator_dealloc
  (tp_handle const& a_handle, void const volatile* const a_ptr, dango::usize a_size, dango::usize const a_align)
  noexcept(requires{ { dango::detail::allocator_dealloc_help<tp_alloc>(a_handle, a_ptr, a_size, a_align) }noexcept; })
  requires(requires{ { dango::detail::allocator_dealloc_help<tp_alloc>(a_handle, a_ptr, a_size, a_align) }; })
  {
    dango_assert_nonnull(a_ptr);
    dango_assert(dango::is_pow_two(a_align));

    a_size = dango::next_multiple(dango::max(dango::usize(1), a_size), a_align);

    dango::detail::allocator_dealloc_help<tp_alloc>(a_handle, a_ptr, a_size, a_align);
  }
}

/*** allocator_new allocator_delete ***/

namespace
dango
{
  template
  <dango::is_allocator tp_alloc>
  struct
  allocator_new_tag
  final
  {
    DANGO_TAG_TYPE(allocator_new_tag)
  };

  template
  <dango::is_allocator tp_alloc>
  inline constexpr dango::allocator_new_tag<tp_alloc> const allocator_new{ };
}

template
<dango::is_nohandle_allocator tp_alloc, dango::is_object_exclude_array tp_type>
[[nodiscard]] auto
operator new
(
  dango::usize const a_size,
  dango::allocator_new_tag<tp_alloc> const,
  dango::type_tag<tp_type> const,
  dango::usize const a_align = alignof(tp_type)
)
noexcept(dango::is_noexcept_nohandle_allocator<tp_alloc>)->void*
requires(dango::is_noexcept_destructible<tp_type>)
{
  dango_assert(a_size == sizeof(tp_type));
  dango_assert(dango::is_pow_two(a_align));

  return dango::allocator_alloc<tp_alloc>(a_size, dango::max(alignof(tp_type), a_align));
}

template
<dango::is_nohandle_allocator tp_alloc, dango::is_object_exclude_array tp_type>
void
operator delete
(
  void* const a_ptr,
  dango::allocator_new_tag<tp_alloc> const,
  dango::type_tag<tp_type> const,
  dango::usize const a_align
)
noexcept
requires(dango::is_noexcept_destructible<tp_type>)
{
  dango_assert(dango::is_pow_two(a_align));

  dango::allocator_dealloc<tp_alloc>(a_ptr, sizeof(tp_type), dango::max(alignof(tp_type), a_align));
}

template
<dango::is_handle_based_allocator tp_alloc, typename tp_handle, dango::is_object_exclude_array tp_type>
requires(dango::is_same<tp_handle, typename tp_alloc::handle_type> && !dango::is_same<tp_handle, typename tp_alloc::guard_type>)
[[nodiscard]] auto
operator new
(
  dango::usize const a_size,
  dango::allocator_new_tag<tp_alloc> const,
  tp_handle const& a_handle,
  dango::type_tag<tp_type> const,
  dango::usize const a_align = alignof(tp_type)
)
noexcept(dango::is_noexcept_handle_based_allocator<tp_alloc>)->void*
requires(dango::is_noexcept_destructible<tp_type>)
{
  dango_assert(a_size == sizeof(tp_type));
  dango_assert(dango::is_pow_two(a_align));

  return dango::allocator_alloc<tp_alloc>(a_handle, a_size, dango::max(alignof(tp_type), a_align));
}

template
<dango::is_handle_based_allocator tp_alloc, typename tp_handle, dango::is_object_exclude_array tp_type>
requires(dango::is_same<tp_handle, typename tp_alloc::handle_type> && !dango::is_same<tp_handle, typename tp_alloc::guard_type>)
void
operator delete
(
  void* const a_ptr,
  dango::allocator_new_tag<tp_alloc> const,
  tp_handle const& a_handle,
  dango::type_tag<tp_type> const,
  dango::usize const a_align
)
noexcept
requires(dango::is_noexcept_destructible<tp_type>)
{
  dango_assert(dango::is_pow_two(a_align));

  dango::allocator_dealloc<tp_alloc>(a_handle, a_ptr, sizeof(tp_type), dango::max(alignof(tp_type), a_align));
}

template
<dango::is_handle_based_allocator tp_alloc, typename tp_guard, dango::is_object_exclude_array tp_type>
requires(dango::is_same<tp_guard, typename tp_alloc::guard_type>)
[[nodiscard]] auto
operator new
(
  dango::usize const a_size,
  dango::allocator_new_tag<tp_alloc> const,
  tp_guard const& a_guard,
  dango::type_tag<tp_type> const,
  dango::usize const a_align = alignof(tp_type)
)
noexcept(dango::is_noexcept_handle_based_allocator<tp_alloc>)->void*
requires(dango::is_noexcept_destructible<tp_type>)
{
  dango_assert(a_size == sizeof(tp_type));
  dango_assert(dango::is_pow_two(a_align));

  return dango::allocator_alloc<tp_alloc>(a_guard, a_size, dango::max(alignof(tp_type), a_align));
}

template
<dango::is_handle_based_allocator tp_alloc, typename tp_guard, dango::is_object_exclude_array tp_type>
requires(dango::is_same<tp_guard, typename tp_alloc::guard_type>)
void
operator delete
(
  void* const a_ptr,
  dango::allocator_new_tag<tp_alloc> const,
  tp_guard const& a_guard,
  dango::type_tag<tp_type> const,
  dango::usize const a_align
)
noexcept
requires(dango::is_noexcept_destructible<tp_type>)
{
  dango_assert(dango::is_pow_two(a_align));

  dango::allocator_dealloc<tp_alloc>(a_guard, a_ptr, sizeof(tp_type), dango::max(alignof(tp_type), a_align));
}

template
<dango::is_nohandle_allocator tp_alloc, dango::is_object_exclude_array tp_type>
[[nodiscard]] auto
operator new[]
(
  dango::usize const a_size,
  dango::allocator_new_tag<tp_alloc> const,
  dango::type_tag<tp_type> const,
  dango::usize const a_count,
  dango::usize const a_align = alignof(tp_type)
)
noexcept(dango::is_noexcept_nohandle_allocator<tp_alloc>)->void*
requires(dango::is_trivial_destructible<tp_type>)
{
  dango_assert(a_size == a_count * sizeof(tp_type));
  dango_assert(dango::is_pow_two(a_align));

  return dango::allocator_alloc<tp_alloc>(a_size, dango::max(alignof(tp_type), a_align));
}

template
<dango::is_nohandle_allocator tp_alloc, dango::is_object_exclude_array tp_type>
void
operator delete[]
(
  void* const a_ptr,
  dango::allocator_new_tag<tp_alloc> const,
  dango::type_tag<tp_type> const,
  dango::usize const a_count,
  dango::usize const a_align
)
noexcept
requires(dango::is_trivial_destructible<tp_type>)
{
  dango_assert(dango::is_pow_two(a_align));

  dango::allocator_dealloc<tp_alloc>(a_ptr, a_count * sizeof(tp_type), dango::max(alignof(tp_type), a_align));
}

template
<dango::is_handle_based_allocator tp_alloc, typename tp_handle, dango::is_object_exclude_array tp_type>
requires(dango::is_same<tp_handle, typename tp_alloc::handle_type> && !dango::is_same<tp_handle, typename tp_alloc::guard_type>)
[[nodiscard]] auto
operator new[]
(
  dango::usize const a_size,
  dango::allocator_new_tag<tp_alloc> const,
  tp_handle const& a_handle,
  dango::type_tag<tp_type> const,
  dango::usize const a_count,
  dango::usize const a_align = alignof(tp_type)
)
noexcept(dango::is_noexcept_handle_based_allocator<tp_alloc>)->void*
requires(dango::is_trivial_destructible<tp_type>)
{
  dango_assert(a_size == a_count * sizeof(tp_type));
  dango_assert(dango::is_pow_two(a_align));

  return dango::allocator_alloc<tp_alloc>(a_handle, a_size, dango::max(alignof(tp_type), a_align));
}

template
<dango::is_handle_based_allocator tp_alloc, typename tp_handle, dango::is_object_exclude_array tp_type>
requires(dango::is_same<tp_handle, typename tp_alloc::handle_type> && !dango::is_same<tp_handle, typename tp_alloc::guard_type>)
void
operator delete[]
(
  void* const a_ptr,
  dango::allocator_new_tag<tp_alloc> const,
  tp_handle const& a_handle,
  dango::type_tag<tp_type> const,
  dango::usize const a_count,
  dango::usize const a_align
)
noexcept
requires(dango::is_trivial_destructible<tp_type>)
{
  dango_assert(dango::is_pow_two(a_align));

  dango::allocator_dealloc<tp_alloc>(a_handle, a_ptr, a_count * sizeof(tp_type), dango::max(alignof(tp_type), a_align));
}

template
<dango::is_handle_based_allocator tp_alloc, typename tp_guard, dango::is_object_exclude_array tp_type>
requires(dango::is_same<tp_guard, typename tp_alloc::guard_type>)
[[nodiscard]] auto
operator new[]
(
  dango::usize const a_size,
  dango::allocator_new_tag<tp_alloc> const,
  tp_guard const& a_guard,
  dango::type_tag<tp_type> const,
  dango::usize const a_count,
  dango::usize const a_align = alignof(tp_type)
)
noexcept(dango::is_noexcept_handle_based_allocator<tp_alloc>)->void*
requires(dango::is_trivial_destructible<tp_type>)
{
  dango_assert(a_size == a_count * sizeof(tp_type));
  dango_assert(dango::is_pow_two(a_align));

  return dango::allocator_alloc<tp_alloc>(a_guard, a_size, dango::max(alignof(tp_type), a_align));
}

template
<dango::is_handle_based_allocator tp_alloc, typename tp_guard, dango::is_object_exclude_array tp_type>
requires(dango::is_same<tp_guard, typename tp_alloc::guard_type>)
void
operator delete[]
(
  void* const a_ptr,
  dango::allocator_new_tag<tp_alloc> const,
  tp_guard const& a_guard,
  dango::type_tag<tp_type> const,
  dango::usize const a_count,
  dango::usize const a_align
)
noexcept
requires(dango::is_trivial_destructible<tp_type>)
{
  dango_assert(dango::is_pow_two(a_align));

  dango::allocator_dealloc<tp_alloc>(a_guard, a_ptr, a_count * sizeof(tp_type), dango::max(alignof(tp_type), a_align));
}

namespace
dango
{
  template
  <dango::is_nohandle_allocator tp_alloc, dango::is_noexcept_destructible tp_type>
  void
  allocator_delete
  (tp_type const volatile* const a_ptr, dango::usize const a_align = alignof(tp_type))noexcept
  {
    dango_assert_nonnull(a_ptr);
    dango_assert(dango::is_pow_two(a_align));

    dango::qualified_destructor(a_ptr);

    dango::allocator_dealloc<tp_alloc>(a_ptr, sizeof(tp_type), dango::max(alignof(tp_type), a_align));
  }

  template
  <dango::is_handle_based_allocator tp_alloc, typename tp_handle, dango::is_noexcept_destructible tp_type>
  requires(dango::is_same<tp_handle, typename tp_alloc::handle_type> || dango::is_same<tp_handle, typename tp_alloc::guard_type>)
  void
  allocator_delete
  (tp_handle const& a_handle, tp_type const volatile* const a_ptr, dango::usize const a_align = alignof(tp_type))noexcept
  {
    dango_assert_nonnull(a_ptr);
    dango_assert(dango::is_pow_two(a_align));

    dango::qualified_destructor(a_ptr);

    dango::allocator_dealloc<tp_alloc>(a_handle, a_ptr, sizeof(tp_type), dango::max(alignof(tp_type), a_align));
  }

  template
  <dango::is_nohandle_allocator tp_alloc, dango::is_trivial_destructible tp_type>
  void
  allocator_delete_array
  (tp_type const volatile* const a_ptr, dango::usize const a_count, dango::usize const a_align = alignof(tp_type))noexcept
  {
    dango_assert_nonnull(a_ptr);
    dango_assert(dango::is_pow_two(a_align));

    dango::allocator_dealloc<tp_alloc>(a_ptr, a_count * sizeof(tp_type), dango::max(alignof(tp_type), a_align));
  }

  template
  <dango::is_handle_based_allocator tp_alloc, typename tp_handle, dango::is_trivial_destructible tp_type>
  requires(dango::is_same<tp_handle, typename tp_alloc::handle_type> || dango::is_same<tp_handle, typename tp_alloc::guard_type>)
  void
  allocator_delete_array
  (tp_handle const& a_handle, tp_type const volatile* const a_ptr, dango::usize const a_count, dango::usize const a_align = alignof(tp_type))noexcept
  {
    dango_assert_nonnull(a_ptr);
    dango_assert(dango::is_pow_two(a_align));

    dango::allocator_dealloc<tp_alloc>(a_handle, a_ptr, a_count * sizeof(tp_type), dango::max(alignof(tp_type), a_align));
  }
}

#endif // DANGO_ALLOCATOR_HPP_INCLUDED
