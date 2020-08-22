#ifndef DANGO_ALLOCATOR_HPP_INCLUDED
#define DANGO_ALLOCATOR_HPP_INCLUDED

/*** is_allocator ***/

namespace
dango
{
  template
  <typename tp_alloc>
  concept is_handle_based_allocator =
    dango::is_class_or_union<tp_alloc> &&
    requires{ typename tp_alloc::handle_type; } &&
    dango::is_object_exclude_array<typename tp_alloc::handle_type> &&
    !dango::is_const_or_volatile<typename tp_alloc::handle_type> &&
    dango::is_nullable<typename tp_alloc::handle_type> &&
    dango::is_noexcept_copy_constructible<typename tp_alloc::handle_type> &&
    dango::is_noexcept_move_constructible<typename tp_alloc::handle_type> &&
    dango::is_noexcept_copy_assignable<typename tp_alloc::handle_type> &&
    dango::is_noexcept_move_assignable<typename tp_alloc::handle_type> &&
    requires
    (
      typename tp_alloc::handle_type& a_handle_m,
      typename tp_alloc::handle_type const& a_handle_c,
      void const volatile* const a_voidp,
      dango::usize const a_usize
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
    };

  template
  <typename tp_alloc>
  concept is_noexcept_handle_based_allocator =
    dango::is_handle_based_allocator<tp_alloc> &&
    requires
    (
      typename tp_alloc::handle_type& a_handle_m,
      typename tp_alloc::handle_type const& a_handle_c,
      void const volatile* const a_voidp,
      dango::usize const a_usize
    )
    {
      { a_handle_m->alloc(a_usize, a_usize) }noexcept->dango::is_same<void*>;
      { a_handle_c->alloc(a_usize, a_usize) }noexcept->dango::is_same<void*>;
      { dango::move(a_handle_m)->alloc(a_usize, a_usize) }noexcept->dango::is_same<void*>;
      { dango::move(a_handle_c)->alloc(a_usize, a_usize) }noexcept->dango::is_same<void*>;
    };

  template
  <typename tp_alloc>
  concept is_nohandle_allocator =
    dango::is_class_or_union<tp_alloc> &&
    !requires{ typename tp_alloc::handle_type; } &&
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
  static auto
  alloc
  (dango::usize const a_size, dango::usize const a_align)dango_new_noexcept->void*
  {
    return dango::operator_new(a_size, a_align);
  }

  static void
  dealloc
  (void const volatile* const a_ptr, dango::usize const a_size, dango::usize const a_align)noexcept
  {
    dango::operator_delete(a_ptr, a_size, a_align);
  }

  DANGO_UNINSTANTIABLE(basic_allocator)
};

static_assert(dango::is_allocator<dango::basic_allocator>);
static_assert(dango::is_nohandle_allocator<dango::basic_allocator>);

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
  using mem_resource_ptr = typename dango::polymorphic_allocator<tp_noexcept>::mem_resource_ptr;
}

template
<bool tp_noexcept>
struct
dango::
polymorphic_allocator
final
{
public:
  class mem_resource;
  class mem_resource_ptr;
  using handle_type = mem_resource_ptr;
  template
  <typename tp_mr>
  class mem_resource_storage;
  template
  <typename tp_mr>
  class mem_resource_storage_constinit;
public:
#ifndef DANGO_NO_DEBUG
  template
  <dango::is_derived_from<dango::mem_resource<tp_noexcept>> tp_mr, typename... tp_args>
  requires(!dango::is_const_or_volatile<tp_mr> && dango::is_brace_constructible<tp_mr, tp_args...> && dango::is_noexcept_destructible<tp_mr>)
  static auto
  make
  (tp_args&&... a_args)
  dango_new_noexcept_and(dango::is_noexcept_brace_constructible<tp_mr, tp_args...>)->mem_resource_storage<tp_mr>
  {
    return mem_resource_storage<tp_mr>{ dango::forward<tp_args>(a_args)... };
  }
#else
  template
  <dango::is_derived_from<dango::mem_resource<tp_noexcept>> tp_mr, typename... tp_args>
  requires(!dango::is_const_or_volatile<tp_mr> && dango::is_brace_constructible<tp_mr, tp_args...> && dango::is_noexcept_destructible<tp_mr>)
  static constexpr auto
  make
  (tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<tp_mr, tp_args...>)->mem_resource_storage<tp_mr>
  {
    return mem_resource_storage<tp_mr>{ dango::forward<tp_args>(a_args)... };
  }
#endif
  template
  <dango::is_derived_from<dango::mem_resource<tp_noexcept>> tp_mr, typename... tp_args>
  requires(!dango::is_const_or_volatile<tp_mr> && dango::is_noexcept_brace_constructible<tp_mr, tp_args...> && dango::is_trivial_destructible<tp_mr>)
  static constexpr auto
  make_constinit
  (tp_args&&... a_args)noexcept->mem_resource_storage_constinit<tp_mr>
  {
    return mem_resource_storage_constinit<tp_mr>{ dango::forward<tp_args>(a_args)... };
  }
private:
  class control_base;
  template
  <typename tp_mr>
  class control;
#ifndef DANGO_NODEBUG
  template
  <typename tp_mr>
  class control_debug;
#endif
public:
  DANGO_UNINSTANTIABLE(polymorphic_allocator)
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
  ~mem_resource()noexcept = default;
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
  DANGO_IMMOBILE(mem_resource)
};

template
<bool tp_noexcept>
class
dango::
polymorphic_allocator<tp_noexcept>::
control_base
{
protected:
  explicit constexpr
  control_base(mem_resource* const a_resource)noexcept:
  m_resource{ a_resource }
  { }
  ~control_base()noexcept = default;
public:
  constexpr auto get_resource_ptr()const noexcept->auto{ return m_resource; }
#ifndef DANGO_NO_DEBUG
protected:
  void set_resource_ptr(mem_resource* const a_resource)noexcept{ m_resource = a_resource; }
public:
  virtual void weak_increment()noexcept{ }
  virtual auto try_strong_increment()noexcept->bool{ return true; }
  virtual auto weak_decrement()noexcept->bool{ return false; }
  virtual auto strong_decrement()noexcept->bool{ return false; }
private:
  mem_resource* m_resource;
#else
private:
  mem_resource* const m_resource;
#endif
public:
  DANGO_DELETE_DEFAULT(control_base)
  DANGO_IMMOBILE(control_base)
};

template
<bool tp_noexcept>
template
<typename tp_mr>
class
dango::
polymorphic_allocator<tp_noexcept>::
control
final:
public control_base
{
private:
  using super_type = control_base;
  using resource_type = tp_mr;
public:
  template
  <typename... tp_args>
  explicit constexpr
  control(tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<resource_type, tp_args...>):
  super_type{ &m_resource },
  m_resource{ dango::forward<tp_args>(a_args)... }
  { }
  ~control()noexcept = default;
private:
  resource_type m_resource;
public:
  DANGO_DELETE_DEFAULT(control)
  DANGO_IMMOBILE(control)
};

#ifndef DANGO_NO_DEBUG
template
<bool tp_noexcept>
template
<typename tp_mr>
class
dango::
polymorphic_allocator<tp_noexcept>::
control_debug
final:
public control_base
{
private:
  using super_type = control_base;
  using resource_type = tp_mr;
  using storage_type = dango::aligned_union<resource_type, dango::cache_align_type>;
public:
  static auto
  operator new
  (dango::usize const a_size)dango_new_noexcept->void*
  {
    return dango::operator_new(a_size, alignof(control_debug));
  }
  static void
  operator delete
  (void* const a_ptr, dango::usize const a_size)noexcept
  {
    dango::operator_delete(a_ptr, a_size, alignof(control_debug));
  }
public:
  template
  <typename... tp_args>
  explicit constexpr
  control_debug(tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<resource_type, tp_args...>):
  super_type{ dango::null },
  m_storage{ },
  m_count{ }
  {
    auto const a_resource =
      dango_placement_new(m_storage.get(), resource_type, { dango::forward<tp_args>(a_args)... });

    super_type::set_resource_ptr(a_resource);
  }
  ~control_debug()noexcept = default;
public:
  virtual void weak_increment()noexcept override{ m_count.weak_increment(); }
  virtual auto try_strong_increment()noexcept->bool override{ return m_count.try_strong_increment(); }
  virtual auto weak_decrement()noexcept->bool override{ return m_count.weak_decrement(); }
  virtual auto
  strong_decrement()noexcept->bool override
  {
    auto const a_dispose =
     [this]()noexcept->void{ dango::destructor(super_type::get_resource_ptr()); };

    return m_count.strong_decrement(a_dispose);
  }
private:
  storage_type m_storage;
  dango::atomic_ref_count_ws m_count;
public:
  DANGO_DELETE_DEFAULT(control_debug)
  DANGO_IMMOBILE(control_debug)
};
#endif

#ifndef DANGO_NO_DEBUG
template
<bool tp_noexcept>
template
<typename tp_mr>
class
dango::
polymorphic_allocator<tp_noexcept>::
mem_resource_storage
final
{
  template
  <dango::is_derived_from<dango::mem_resource<tp_noexcept>> tp_tp_mr, typename... tp_args>
  requires(!dango::is_const_or_volatile<tp_tp_mr> && dango::is_brace_constructible<tp_tp_mr, tp_args...> && dango::is_noexcept_destructible<tp_tp_mr>)
  friend constexpr auto
  dango::polymorphic_allocator<tp_noexcept>::make(tp_args&&...)
  dango_new_noexcept_and(dango::is_noexcept_brace_constructible<tp_tp_mr, tp_args...>)->mem_resource_storage<tp_tp_mr>;
private:
  using resource_type = tp_mr;
  using control_type = control_debug<resource_type>;
public:
  template
  <typename... tp_args>
  explicit
  mem_resource_storage
  (tp_args&&... a_args)
  dango_new_noexcept_and(dango::is_noexcept_brace_constructible<resource_type, tp_args...>):
  m_control{ new control_type{ dango::forward<tp_args>(a_args)... } }
  { }
public:
  ~mem_resource_storage()noexcept
  {
    if(m_control->strong_decrement())
    {
      delete m_control;
    }
  }
public:
  constexpr auto get_ptr()noexcept->dango::mem_resource_ptr<tp_noexcept>;
private:
  control_type* const m_control;
public:
  DANGO_DELETE_DEFAULT(mem_resource_storage)
  DANGO_IMMOBILE(mem_resource_storage)
};
#else
template
<bool tp_noexcept>
template
<typename tp_mr>
class
dango::
polymorphic_allocator<tp_noexcept>::
mem_resource_storage
final
{
  template
  <dango::is_derived_from<dango::mem_resource<tp_noexcept>> tp_tp_mr, typename... tp_args>
  requires(!dango::is_const_or_volatile<tp_tp_mr> && dango::is_brace_constructible<tp_tp_mr, tp_args...> && dango::is_noexcept_destructible<tp_tp_mr>)
  friend constexpr auto
  dango::polymorphic_allocator<tp_noexcept>::make(tp_args&&...)
  noexcept(dango::is_noexcept_brace_constructible<tp_tp_mr, tp_args...>)->mem_resource_storage<tp_tp_mr>;
private:
  using resource_type = tp_mr;
  using control_type = control<resource_type>;
private:
  template
  <typename... tp_args>
  explicit constexpr
  mem_resource_storage
  (tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<resource_type, tp_args...>):
  m_control{ dango::forward<tp_args>(a_args)... }
  { }
public:
  ~mem_resource_storage()noexcept = default;
public:
  constexpr auto get_ptr()noexcept->dango::mem_resource_ptr<tp_noexcept>;
private:
  control_type m_control;
public:
  DANGO_DELETE_DEFAULT(mem_resource_storage)
  DANGO_IMMOBILE(mem_resource_storage)
};
#endif

template
<bool tp_noexcept>
template
<typename tp_mr>
class
dango::
polymorphic_allocator<tp_noexcept>::
mem_resource_storage_constinit
final
{
  template
  <dango::is_derived_from<dango::mem_resource<tp_noexcept>> tp_tp_mr, typename... tp_args>
  requires(!dango::is_const_or_volatile<tp_tp_mr> && dango::is_noexcept_brace_constructible<tp_tp_mr, tp_args...> && dango::is_trivial_destructible<tp_tp_mr>)
  friend constexpr auto
  dango::polymorphic_allocator<tp_noexcept>::make_constinit(tp_args&&...)noexcept->mem_resource_storage_constinit<tp_tp_mr>;
private:
  using resource_type = tp_mr;
  using control_type = control<resource_type>;
private:
  template
  <typename... tp_args>
  explicit constexpr
  mem_resource_storage_constinit
  (tp_args&&... a_args)noexcept:
  m_control{ dango::forward<tp_args>(a_args)... }
  { }
public:
  ~mem_resource_storage_constinit()noexcept = default;
public:
  constexpr auto get_ptr()noexcept->dango::mem_resource_ptr<tp_noexcept>;
private:
  control_type m_control;
public:
  DANGO_DELETE_DEFAULT(mem_resource_storage_constinit)
  DANGO_IMMOBILE(mem_resource_storage_constinit)
};



template
<bool tp_noexcept>
class
dango::
polymorphic_allocator<tp_noexcept>::
mem_resource_ptr
final
{

};

/*** basic_memory_resource ***/

namespace
dango
{

}

#endif // DANGO_ALLOCATOR_HPP_INCLUDED

