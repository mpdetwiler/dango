#ifndef DANGO_ALLOCATOR_HPP_INCLUDED
#define DANGO_ALLOCATOR_HPP_INCLUDED

/*** allocator classes ***/

namespace
dango
{
  template
  <bool tp_noexcept>
  class allocator_ptr;

  template
  <bool to_noexcept>
  class static_allocator;

  template
  <bool tp_noexcept>
  class local_allocator;
}

namespace
dango::detail
{
  class
  allocator_common
  {
  public:
    explicit constexpr allocator_common()noexcept;
    ~allocator_common()noexcept = default;
  public:
    virtual auto is_noexcept()const noexcept->bool = 0;
  public:
    DANGO_IMMOBILE(allocator_common)
  };

  template
  <bool tp_noexcept>
  class allocator_base;
}

template
<bool tp_noexcept>
class
dango::
detail::
allocator_base:
public dango::detail::allocator_common
{
  friend dango::allocator_ptr<tp_noexcept>;
private:
  using super_type = dango::detail::allocator_common;
  using count_type = dango::usize;
public:
  using size_type = dango::usize;
protected:
  explicit constexpr allocator_base()noexcept;
  ~allocator_base()noexcept = default;
public:
  auto is_noexcept()const noexcept->bool final{ return tp_noexcept; }
protected:
  virtual auto virtual_alloc(size_type, size_type)noexcept(tp_noexcept)->void*;
  virtual void virtual_free(void const volatile*, size_type, size_type)noexcept;
protected:
  void destructor_check()noexcept;
private:
  auto alloc(size_type, size_type)noexcept(tp_noexcept)->void*;
  void free(void const volatile*, size_type, size_type)noexcept;
private:
  dango::spin_mutex m_lock;
  count_type m_alloc_count;
public:
  DANGO_IMMOBILE(allocator_base)
};

template
<bool tp_noexcept>
constexpr
dango::
detail::
allocator_base<tp_noexcept>::
allocator_base
()noexcept:
super_type{ },
m_lock{ },
m_alloc_count{ count_type(0) }
{

}

template
<bool tp_noexcept>
auto
dango::
detail::
allocator_base<tp_noexcept>::
virtual_alloc
(size_type const a_size, size_type const a_align)noexcept(tp_noexcept)->void*
{
  return dango::operator_new(a_size, a_align);
}

template
<bool tp_noexcept>
void
dango::
detail::
allocator_base<tp_noexcept>::
virtual_free
(void const volatile* const a_ptr, size_type const a_size, size_type const a_align)noexcept
{
  dango::operator_delete(a_ptr, a_size, a_align);
}

template
<bool tp_noexcept>
auto
dango::
detail::
allocator_base<tp_noexcept>::
alloc
(size_type const a_size, size_type const a_align)noexcept(tp_noexcept)->void*
{
  auto const a_ptr = virtual_alloc(a_size, a_align);

  dango_crit(m_lock)
  {
    ++m_alloc_count;
  }

  return a_ptr;
}

template
<bool tp_noexcept>
void
dango::
detail::
allocator_base<tp_noexcept>::
free
(void const volatile* const a_ptr, size_type const a_size, size_type const a_align)noexcept
{
  dango_crit(m_lock)
  {
    dango_assert_terminate_msg(m_alloc_count != count_type(0), u8"allocator: double free detected");

    --m_alloc_count;
  }

  virtual_free(a_ptr, a_size, a_align);
}

template
<bool tp_noexcept>
void
dango::
detail::
allocator_base<tp_noexcept>::
destructor_check
()noexcept
{
  dango_crit(m_lock)
  {
    dango_assert_terminate_msg(m_alloc_count == count_type(0), u8"allocator: attempt to destroy with unfreed allocations");
  }
}

/******************************/

template
<bool tp_noexcept>
class
dango::
static_allocator:
public dango::detail::allocator_base<tp_noexcept>
{
private:
  using super_type = dango::detail::allocator_base<tp_noexcept>;
public:
  explicit constexpr static_allocator()noexcept;
  ~static_allocator()noexcept = default;
public:
  DANGO_IMMOBILE(static_allocator)
};

template
<bool tp_noexcept>
constexpr
dango::
static_allocator<tp_noexcept>::
static_allocator
()noexcept:
super_type{ }
{

}

/******************************/

template
<bool tp_noexcept>
class
dango::
local_allocator:
public dango::detail::allocator_base<tp_noexcept>
{
private:
  using super_type = dango::detail::allocator_base<tp_noexcept>;
public:
  explicit constexpr local_allocator()noexcept;
  virtual ~local_allocator()noexcept;
public:
  DANGO_IMMOBILE(local_allocator)
};

template
<bool tp_noexcept>
constexpr
dango::
local_allocator<tp_noexcept>::
local_allocator
()noexcept:
super_type{ }
{

}

template
<bool tp_noexcept>
dango::
local_allocator<tp_noexcept>::
~local_allocator
()noexcept
{
  super_type::destructor_check();
}

/************************************************************/

namespace
dango::detail
{
  class allocator_control;

  template
  <typename tp_allocator>
  class allocator_control_static;

  template
  <typename tp_allocator>
  class allocator_control_local;
}

class alignas(dango::cache_align_type)
dango::
detail::
allocator_control
{
private:
  using alloc_ptr_type = dango::detail::allocator_common*;
  using count_type = dango::usize;
protected:
  using delete_func = void(*)(allocator_control*)noexcept;
public:
  static void strong_increment(allocator_control*)noexcept;
  static auto try_strong_increment(allocator_control*)noexcept->bool;
  static void strong_decrement(allocator_control*)noexcept;
  static void weak_increment(allocator_control*)noexcept;
  static void weak_decrement(allocator_control*)noexcept;
protected:
  explicit constexpr allocator_control()noexcept;
  ~allocator_control()noexcept = default;
protected:
  void set_alloc(alloc_ptr_type const a_alloc)noexcept{ m_alloc = a_alloc; }
private:
  virtual void destruct_alloc()const noexcept = 0;
  virtual auto get_delete()const noexcept->delete_func = 0;
private:
  alloc_ptr_type m_alloc;
  dango::spin_mutex m_lock;
  count_type m_strong_count;
  count_type m_weak_count;
public:
  DANGO_IMMOBILE(allocator_control)
};

constexpr
dango::
detail::
allocator_control::
allocator_control
()noexcept:
m_alloc{ dango::null },
m_lock{ },
m_strong_count{ count_type(1) },
m_weak_count{ count_type(1) }
{

}

inline void
dango::
detail::
allocator_control::
strong_increment
(allocator_control* const a_this)noexcept
{
  dango_assert(a_this != dango::null);

  dango_crit(a_this->m_lock)
  {
    ++a_this->m_strong_count;
    ++a_this->m_weak_count;
  }
}

inline auto
dango::
detail::
allocator_control::
try_strong_increment
(allocator_control* const a_this)noexcept->bool
{
  dango_assert(a_this != dango::null);

  dango_crit(a_this->m_lock)
  {
    if(a_this->m_strong_count == count_type(0))
    {
      return false;
    }

    ++a_this->m_strong_count;
    ++a_this->m_weak_count;
  }

  return true;
}

inline void
dango::
detail::
allocator_control::
strong_decrement
(allocator_control* const a_this)noexcept
{
  dango_assert(a_this != dango::null);

  dango_crit(a_this->m_lock)
  {
    if(--a_this->m_strong_count != count_type(0))
    {
      --a_this->m_weak_count;

      return;
    }
  }

  a_this->destruct_alloc();

  weak_decrement(a_this);
}

inline void
dango::
detail::
allocator_control::
weak_increment
(allocator_control* const a_this)noexcept
{
  dango_assert(a_this != dango::null);

  dango_crit(a_this->m_lock)
  {
    ++a_this->m_weak_count;
  }
}

inline void
dango::
detail::
allocator_control::
weak_decrement
(allocator_control* const a_this)noexcept
{
  dango_assert(a_this != dango::null);

  dango_crit(a_this->m_lock)
  {
    if(--a_this->m_weak_count != count_type(0))
    {
      return;
    }
  }

  auto const a_delete = a_this->get_delete();

  a_delete(a_this);
}

/******************************/

template
<typename tp_allocator>
class
dango::
detail::
allocator_control_static
final:
public dango::detail::allocator_control
{

};

/******************************/

template
<typename tp_allocator>
class
dango::
detail::
allocator_control_local
final:
public dango::detail::allocator_control
{

};

/************************************************************/

namespace
dango
{
  template
  <typename tp_type, bool tp_noexcept>
  concept static_allocator_storage_constraint_spec =
    dango::is_class<tp_type> && !dango::is_const<tp_type> && !dango::is_volatile<tp_type> &&
    dango::is_trivial_destructible<tp_type> &&
    dango::is_derived_from<tp_type, dango::static_allocator<tp_noexcept>>;

  template
  <typename tp_type, bool tp_noexcept>
  concept local_allocator_storage_constraint_spec =
    dango::is_class<tp_type> && !dango::is_const<tp_type> && !dango::is_volatile<tp_type> &&
    dango::is_noexcept_destructible<tp_type> &&
    dango::is_derived_from<tp_type, dango::local_allocator<tp_noexcept>>;

  template
  <typename tp_type, bool tp_noexcept>
  requires(dango::static_allocator_storage_constraint_spec<tp_type, tp_noexcept>)
  class static_allocator_storage;

  template
  <typename tp_type, bool tp_noexcept>
  requires(dango::local_allocator_storage_constraint_spec<tp_type, tp_noexcept>)
  class local_allocator_storage;
}

template
<typename tp_type, bool tp_noexcept>
requires(dango::static_allocator_storage_constraint_spec<tp_type, tp_noexcept>)
class
dango::
static_allocator_storage
final
{

};

template
<typename tp_type, bool tp_noexcept>
requires(dango::local_allocator_storage_constraint_spec<tp_type, tp_noexcept>)
class
dango::
local_allocator_storage
final
{

};


#endif // DANGO_ALLOCATOR_HPP_INCLUDED

