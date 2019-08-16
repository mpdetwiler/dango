#ifndef __DANGO_ALLOCATOR_HPP__
#define __DANGO_ALLOCATOR_HPP__

/*** allocator classes ***/

namespace
dango
{
  class allocator;
  class static_allocator;
  class local_allocator;
}

namespace
dango::detail
{
  class allocator_base;
}

class
dango::
detail::
allocator_base
{
  friend dango::allocator;
protected:
  constexpr allocator_base()noexcept;
  ~allocator_base()noexcept = default;
protected:
  virtual auto alloc(dango::usize, dango::usize)dango_new_noexcept(true)->void*;
  virtual void free(void const volatile*, dango::usize, dango::usize)noexcept;
protected:
  void destructor_check()noexcept;
private:
  auto checked_alloc(dango::usize, dango::usize)dango_new_noexcept(true)->void*;
  void checked_free(void const volatile*, dango::usize, dango::usize)noexcept;
private:
  dango::atomic<dango::usize> m_alloc_count;
public:
  DANGO_IMMOBILE(allocator_base)
};

constexpr
dango::
detail::
allocator_base::
allocator_base
()noexcept:
m_alloc_count{ dango::usize(0) }
{

}

inline auto
dango::
detail::
allocator_base::
alloc
(dango::usize const a_size, dango::usize const a_align)dango_new_noexcept(true)->void*
{
  return dango::operator_new(a_size, a_align);
}

inline void
dango::
detail::
allocator_base::
free
(void const volatile* const a_ptr, dango::usize const a_size, dango::usize const a_align)noexcept
{
  dango::operator_delete(a_ptr, a_size, a_align);
}

inline auto
dango::
detail::
allocator_base::
checked_alloc
(dango::usize const a_size, dango::usize const a_align)dango_new_noexcept(true)->void*
{
  auto const a_ptr = alloc(a_size, a_align);

  m_alloc_count.add_fetch<dango::mem_order::acquire>(dango::usize(1));

  return a_ptr;
}

inline void
dango::
detail::
allocator_base::
checked_free
(void const volatile* const a_ptr, dango::usize const a_size, dango::usize const a_align)noexcept
{
  if(m_alloc_count.fetch_sub<dango::mem_order::release>(dango::usize(1)) != dango::usize(0))
  {
    free(a_ptr, a_size, a_align);

    return;
  }
#ifndef DANGO_NO_DEBUG
  dango_unreachable_msg("allocator: double free detected");
#else
  dango::terminate();
#endif
}

inline void
dango::
detail::
allocator_base::
destructor_check
()noexcept
{
  if(m_alloc_count.load<dango::mem_order::acquire>() == dango::usize(0))
  {
    return;
  }
#ifndef DANGO_NO_DEBUG
  dango_unreachable_msg("allocator: attempt to destroy with unfreed allocations");
#else
  dango::terminate();
#endif
}

/*****************************/

class
dango::
static_allocator:
public dango::detail::allocator_base
{
private:
  using super_type = dango::detail::allocator_base;
public:
  constexpr static_allocator()noexcept;
  ~static_allocator()noexcept = default;
public:
  DANGO_IMMOBILE(static_allocator)
};

constexpr
dango::
static_allocator::
static_allocator
()noexcept:
super_type{ }
{

}

/*****************************/

class
dango::
local_allocator:
public dango::detail::allocator_base
{
private:
  using super_type = dango::detail::allocator_base;
public:
  constexpr local_allocator()noexcept;
  virtual ~local_allocator()noexcept;
public:
  DANGO_IMMOBILE(local_allocator)
};

constexpr
dango::
local_allocator::
local_allocator
()noexcept:
super_type{ }
{

}

inline
dango::
local_allocator::
~local_allocator
()noexcept
{
  super_type::destructor_check();
}

/*************************************************************/

class
dango::
allocator
final
{
private:
  class control_base;
  template
  <typename tp_alloc>
  class static_control_block;
  template
  <typename tp_alloc>
  class local_control_block;
public:
  template
  <typename tp_alloc>
  class static_allocator_storage;
public:
  template
  <typename tp_alloc, typename... tp_args>
  static constexpr auto
  make_static_storage
  (tp_args&&... a_args)noexcept->
  dango::enable_if
  <
    !dango::is_const<tp_alloc> &&
    !dango::is_volatile<tp_alloc> &&
    dango::is_class<tp_alloc> &&
    dango::is_base_of<dango::static_allocator, tp_alloc> &&
    dango::is_trivial_destructible<tp_alloc> &&
    dango::is_noexcept_constructible<tp_alloc, tp_args...>,
    static_allocator_storage<tp_alloc>
  >
  {
    return static_allocator_storage<tp_alloc>{ dango::forward<tp_args>(a_args)... };
  }

  template
  <typename tp_alloc>
  static auto
  make_static
  (static_allocator_storage<tp_alloc>& a_storage)noexcept->allocator
  {
    return allocator{ a_storage.get_control_block() };
  }

  template
  <typename tp_alloc, typename... tp_args>
  static auto
  make_local
  (tp_args&&... a_args)dango_new_noexcept(dango::is_noexcept_constructible<tp_alloc, tp_args...>)->
  dango::enable_if
  <
    !dango::is_const<tp_alloc> &&
    !dango::is_volatile<tp_alloc> &&
    dango::is_class<tp_alloc> &&
    dango::is_base_of<dango::local_allocator, tp_alloc> &&
    dango::is_noexcept_destructible<tp_alloc> &&
    dango::is_constructible<tp_alloc, tp_args...>,
    allocator
  >
  {
    return allocator{ new local_control_block<tp_alloc>{ dango::forward<tp_args>(a_args)... } };
  }
private:
  constexpr explicit allocator(control_base*)noexcept;
public:
  constexpr allocator(dango::null_tag const)noexcept:m_control{ nullptr }{ };
  allocator(allocator const&)noexcept;
  constexpr allocator(allocator&&)noexcept;
  ~allocator()noexcept;
  auto operator = (dango::null_tag)& noexcept->allocator&;
  auto operator = (allocator const&)& noexcept->allocator&;
  auto operator = (allocator&&)& noexcept->allocator&;
  constexpr explicit operator bool()const noexcept{ return m_control != nullptr; }
public:
  auto alloc(dango::usize, dango::usize)const dango_new_noexcept(true)->void*;
  void free(void const volatile*, dango::usize, dango::usize)const noexcept;
public:
  constexpr auto dango_operator_is_null()const noexcept->bool{ return m_control == nullptr; }
private:
  control_base* m_control;
};

/******************************/

class alignas(dango::cache_align_type)
dango::
allocator::
control_base
{
public:
  static auto operator new(dango::usize)noexcept->void* = delete;
  static void operator delete(void* const, dango::usize const)noexcept{ dango_unreachable; }
protected:
  constexpr explicit
  control_base
  (dango::detail::allocator_base* const a_alloc_ptr)noexcept:
  m_alloc_ptr{ a_alloc_ptr }
  { }
public:
  virtual ~control_base()noexcept = default;
public:
  virtual void increment()noexcept = 0;
  virtual auto decrement()noexcept->bool = 0;
public:
  auto alloc(dango::usize, dango::usize)dango_new_noexcept(true)->void*;
  void free(void const volatile*, dango::usize, dango::usize)noexcept;
private:
  dango::detail::allocator_base* const m_alloc_ptr;
public:
  DANGO_DELETE_DEFAULT(control_base)
  DANGO_IMMOBILE(control_base)
};

inline auto
dango::
allocator::
control_base::
alloc
(dango::usize const a_size, dango::usize const a_align)dango_new_noexcept(true)->void*
{
  return m_alloc_ptr->checked_alloc(a_size, a_align);
}

inline void
dango::
allocator::
control_base::
free
(void const volatile* const a_ptr, dango::usize const a_size, dango::usize const a_align)noexcept
{
  m_alloc_ptr->checked_free(a_ptr, a_size, a_align);
}

/*****************************/

template
<typename tp_alloc>
class
dango::
allocator::
static_control_block
final:
public dango::allocator::control_base
{
private:
  using super_type = dango::allocator::control_base;
public:
  template
  <typename... tp_args>
  constexpr explicit
  static_control_block
  (tp_args&&... a_args)noexcept:
  super_type{ &m_alloc },
  m_alloc{ dango::forward<tp_args>(a_args)... }
  { }
  virtual ~static_control_block()noexcept override final = default;
public:
  virtual void increment()noexcept override final;
  virtual auto decrement()noexcept->bool override final;
private:
  tp_alloc m_alloc;
public:
  DANGO_IMMOBILE(static_control_block)
};

template
<typename tp_alloc>
void
dango::
allocator::
static_control_block<tp_alloc>::
increment
()noexcept
{

}

template
<typename tp_alloc>
auto
dango::
allocator::
static_control_block<tp_alloc>::
decrement
()noexcept->bool
{
  return false;
}

/*****************************/

template
<typename tp_alloc>
class
dango::
allocator::
local_control_block
final:
public dango::allocator::control_base
{
private:
  using super_type = dango::allocator::control_base;
public:
  static auto operator new(dango::usize const a_size)dango_new_noexcept(true)->void*{ return dango::operator_new(a_size, alignof(local_control_block)); }
  static void operator delete(void* const a_ptr, dango::usize const a_size)noexcept{ dango::operator_delete(a_ptr, a_size, alignof(local_control_block)); }
public:
  template
  <typename... tp_args>
  explicit
  local_control_block
  (tp_args&&... a_args)noexcept:
  super_type{ &m_alloc },
  m_ref_count{ dango::usize(1) },
  m_alloc{ dango::forward<tp_args>(a_args)... }
  { }
  virtual ~local_control_block()noexcept override final = default;
public:
  virtual void increment()noexcept override final;
  virtual auto decrement()noexcept->bool override final;
private:
  dango::atomic<dango::usize> m_ref_count;
  tp_alloc m_alloc;
public:
  DANGO_IMMOBILE(local_control_block)
};

template
<typename tp_alloc>
void
dango::
allocator::
local_control_block<tp_alloc>::
increment
()noexcept
{
  m_ref_count.add_fetch<dango::mem_order::acquire>(dango::usize(1));
}

template
<typename tp_alloc>
auto
dango::
allocator::
local_control_block<tp_alloc>::
decrement
()noexcept->bool
{
  auto const a_prev = m_ref_count.fetch_sub<dango::mem_order::release>(dango::usize(1));

  dango_assert(a_prev != dango::usize(0));

  return a_prev == dango::usize(1);
}

/*****************************/

template
<typename tp_alloc>
class
dango::
allocator::
static_allocator_storage
final
{
  static_assert(!dango::is_const<tp_alloc>);
  static_assert(!dango::is_volatile<tp_alloc>);
  static_assert(dango::is_class<tp_alloc>);
  static_assert(dango::is_base_of<dango::static_allocator, tp_alloc>);
  static_assert(dango::is_trivial_destructible<tp_alloc>);

  friend dango::allocator;
private:
  using control_base = dango::allocator::control_base;
  using control_block = dango::allocator::static_control_block<tp_alloc>;
private:
  template
  <typename... tp_args>
  constexpr explicit
  static_allocator_storage
  (tp_args&&... a_args)noexcept:
  m_control_block{ dango::forward<tp_args>(a_args)... }
  { }
public:
  ~static_allocator_storage()noexcept = default;
private:
  constexpr auto get_control_block()noexcept->control_base*{ return &m_control_block; }
private:
  control_block m_control_block;
public:
  DANGO_IMMOBILE(static_allocator_storage)
};

/*****************************/

constexpr
dango::
allocator::
allocator
(control_base* const a_control)noexcept:
m_control{ a_control }
{
  dango_assert(m_control != nullptr);
}

inline
dango::
allocator::
allocator
(allocator const& a_alloc)noexcept:
m_control{ a_alloc.m_control }
{
  if(m_control)
  {
    m_control->increment();
  }
}

constexpr
dango::
allocator::
allocator
(allocator&& a_alloc)noexcept:
m_control{ a_alloc.m_control }
{
  a_alloc.m_control = nullptr;
}

inline
dango::
allocator::
~allocator
()noexcept
{
  if(m_control && m_control->decrement())
  {
    delete m_control;
  }
}

inline auto
dango::
allocator::
operator =
(dango::null_tag const)& noexcept->allocator&
{
  allocator a_temp{ dango::null };

  dango::swap(m_control, a_temp.m_control);

  return *this;
}

inline auto
dango::
allocator::
operator =
(allocator const& a_alloc)& noexcept->allocator&
{
  allocator a_temp{ a_alloc };

  dango::swap(m_control, a_temp.m_control);

  return *this;
}

inline auto
dango::
allocator::
operator =
(allocator&& a_alloc)& noexcept->allocator&
{
  allocator a_temp{ dango::move(a_alloc) };

  dango::swap(m_control, a_temp.m_control);

  return *this;
}

inline auto
dango::
allocator::
alloc
(dango::usize const a_size, dango::usize const a_align)const dango_new_noexcept(true)->void*
{
  dango_assert(m_control != nullptr);

  return m_control->alloc(a_size, a_align);
}

inline void
dango::
allocator::
free
(void const volatile* const a_ptr, dango::usize const a_size, dango::usize const a_align)const noexcept
{
  dango_assert(m_control != nullptr);

  m_control->free(a_ptr, a_size, a_align);
}

/*** allocator_tree ***/

namespace
dango
{
  class allocator_tree;
}

class
dango::
allocator_tree
final
{
private:
  class node_base;
  template
  <dango::usize tp_size, typename tp_enabled = dango::enable_tag>
  class node;
  template
  <dango::usize tp_size>
  class node<tp_size, dango::enable_if<dango::is_equal(tp_size, dango::usize(0))>>;
  template
  <dango::usize tp_size>
  class node<tp_size, dango::enable_if<!dango::is_equal(tp_size, dango::usize(0))>>;
private:
  template
  <typename tp_alloc, typename... tp_children>
  static auto make_node(tp_alloc&&, tp_children&&...)dango_new_noexcept(true)->node_base*;
public:
  template
  <
    typename tp_alloc,
    typename... tp_children,
    dango::enable_if
    <(dango::is_constructible<dango::allocator, tp_alloc> && ... && dango::is_constructible<dango::allocator_tree, tp_children>)>
      = dango::enable_val
  >
  explicit
  allocator_tree
  (tp_alloc&&, tp_children&&...)dango_new_noexcept(true);
public:
  constexpr allocator_tree(dango::null_tag const)noexcept:m_node{ nullptr }{ }
  allocator_tree(allocator_tree const&)noexcept;
  constexpr allocator_tree(allocator_tree&&)noexcept;
  ~allocator_tree()noexcept;
  auto operator = (dango::null_tag)& noexcept->allocator_tree&;
  auto operator = (allocator_tree const&)& noexcept->allocator_tree&;
  auto operator = (allocator_tree&&)& noexcept->allocator_tree&;
  constexpr explicit operator bool()const noexcept{ return m_node != nullptr; }
public:
  auto get_allocator()const noexcept->dango::allocator const&;
  auto get_child(dango::usize)const noexcept->allocator_tree const&;
public:
  constexpr auto dango_operator_is_null()const noexcept->bool{ return m_node == nullptr; }
private:
  node_base* m_node;
public:
  DANGO_DELETE_DEFAULT(allocator_tree)
};

/*****************************/

class alignas(dango::cache_align_type)
dango::
allocator_tree::
node_base
{
public:
  using destructor_func = void(*)(node_base*)noexcept;
public:
  static void increment(node_base*)noexcept;
  static void decrement(node_base*)noexcept;
protected:
  template
  <typename tp_alloc>
  explicit
  node_base
  (destructor_func const a_destructor, tp_alloc&& a_alloc)noexcept:
  m_ref_count{ dango::usize(1) },
  m_destructor{ a_destructor },
  m_alloc{ dango::forward<tp_alloc>(a_alloc) }
  { }
  ~node_base()noexcept = default;
public:
  virtual auto get_child(dango::usize)const noexcept->dango::allocator_tree const* = 0;
public:
  auto get_allocator()const noexcept->dango::allocator const&{ return m_alloc; }
protected:
  auto get_allocator_mutable()noexcept->dango::allocator&{ return m_alloc; }
private:
  dango::atomic<dango::usize> m_ref_count;
  destructor_func const m_destructor;
  dango::allocator m_alloc;
public:
  DANGO_DELETE_DEFAULT(node_base)
  DANGO_IMMOBILE(node_base)
};

inline void
dango::
allocator_tree::
node_base::
increment
(node_base* const a_node)noexcept
{
  dango_assert(a_node != nullptr);

  auto const a_prev = a_node->m_ref_count.fetch_add<dango::mem_order::acquire>(dango::usize(1));

  dango_assert(a_prev != dango::usize(0));
}

inline void
dango::
allocator_tree::
node_base::
decrement
(node_base* const a_node)noexcept
{
  dango_assert(a_node != nullptr);

  auto const a_prev = a_node->m_ref_count.fetch_sub<dango::mem_order::release>(dango::usize(1));

  dango_assert(a_prev != dango::usize(0));

  if(a_prev != dango::usize(1))
  {
    return;
  }

  auto const a_destructor = a_node->m_destructor;

  a_destructor(a_node);
}

/*****************************/

template
<dango::usize tp_size>
class
dango::
allocator_tree::
node<tp_size, dango::enable_if<dango::is_equal(tp_size, dango::usize(0))>>
final:
public dango::allocator_tree::node_base
{
private:
  using super_type = dango::allocator_tree::node_base;
  using super_type::destructor_func;
private:
  static void destructor(super_type*)noexcept;
public:
  template
  <typename tp_alloc>
  explicit
  node
  (tp_alloc&& a_alloc)noexcept:
  super_type{ destructor, dango::forward<tp_alloc>(a_alloc) }
  { }
  ~node()noexcept = default;
public:
  virtual auto get_child(dango::usize const)const noexcept->dango::allocator_tree const* override final{ return nullptr; }
public:
  DANGO_DELETE_DEFAULT(node)
  DANGO_IMMOBILE(node)
};

template
<dango::usize tp_size>
void
dango::
allocator_tree::
node<tp_size, dango::enable_if<dango::is_equal(tp_size, dango::usize(0))>>::
destructor
(super_type* const a_node)noexcept
{
  auto const a_this = static_cast<node*>(a_node);

  auto const a_alloc = dango::move(a_this->get_allocator_mutable());

  dango::destructor(a_this);

  a_alloc.free(a_this, sizeof(node), alignof(node));
}

/*****************************/

template
<dango::usize tp_size>
class
dango::
allocator_tree::
node<tp_size, dango::enable_if<!dango::is_equal(tp_size, dango::usize(0))>>
final:
public dango::allocator_tree::node_base
{
private:
  using super_type = dango::allocator_tree::node_base;
  using super_type::destructor_func;
private:
  static void destructor(super_type*)noexcept;
public:
  template
  <typename tp_alloc, typename... tp_children>
  explicit
  node
  (tp_alloc&& a_alloc, tp_children&&... a_children)noexcept:
  super_type{ destructor, dango::forward<tp_alloc>(a_alloc) },
  m_child{ dango::forward<tp_children>(a_children)... }
  { }
  ~node()noexcept = default;
public:
  virtual auto get_child(dango::usize)const noexcept->dango::allocator_tree const* override final;
private:
  dango::allocator_tree const m_child[tp_size];
public:
  DANGO_DELETE_DEFAULT(node)
  DANGO_IMMOBILE(node)
};

template
<dango::usize tp_size>
void
dango::
allocator_tree::
node<tp_size, dango::enable_if<!dango::is_equal(tp_size, dango::usize(0))>>::
destructor
(super_type* const a_node)noexcept
{
  auto const a_this = static_cast<node*>(a_node);

  auto const a_alloc = dango::move(a_this->get_allocator_mutable());

  dango::destructor(a_this);

  a_alloc.free(a_this, sizeof(node), alignof(node));
}

template
<dango::usize tp_size>
auto
dango::
allocator_tree::
node<tp_size, dango::enable_if<!dango::is_equal(tp_size, dango::usize(0))>>::
get_child
(dango::usize const a_index)const noexcept->dango::allocator_tree const*
{
  if(a_index >= tp_size)
  {
    return nullptr;
  }

  auto const& a_child = m_child[a_index];

  if(a_child == dango::null)
  {
    return nullptr;
  }

  return &a_child;
}

/*****************************/

template
<typename tp_alloc, typename... tp_children>
auto
dango::
allocator_tree::
make_node
(tp_alloc&& a_alloc, tp_children&&... a_children)dango_new_noexcept(true)->node_base*
{
  using node_type = node<sizeof...(tp_children)>;

  dango::allocator a_allocator{ dango::forward<tp_alloc>(a_alloc) };

  dango_assert(a_allocator != dango::null);

  auto const a_ptr = a_allocator.alloc(sizeof(node_type), alignof(node_type));

  return ::new (dango::placement, a_ptr) node_type{ dango::move(a_allocator), dango::forward<tp_children>(a_children)... };
}

template
<
  typename tp_alloc,
  typename... tp_children,
  dango::enable_if
  <(dango::is_constructible<dango::allocator, tp_alloc> && ... && dango::is_constructible<dango::allocator_tree, tp_children>)>
    = dango::enable_val
>
dango::
allocator_tree::
allocator_tree
(tp_alloc&& a_alloc, tp_children&&... a_children)dango_new_noexcept(true):
m_node{ make_node(dango::forward<tp_alloc>(a_alloc), dango::forward<tp_children>(a_children)...) }
{

}

inline
dango::
allocator_tree::
allocator_tree(allocator_tree const& a_tree)noexcept:
m_node{ a_tree.m_node }
{
  if(m_node)
  {
    node_base::increment(m_node);
  }
}

constexpr
dango::
allocator_tree::
allocator_tree(allocator_tree&& a_tree)noexcept:
m_node{ a_tree.m_node }
{
  a_tree.m_node = nullptr;
}

inline
dango::
allocator_tree::
~allocator_tree
()noexcept
{
  if(m_node)
  {
    node_base::decrement(m_node);
  }
}

inline auto
dango::
allocator_tree::
operator =
(dango::null_tag const)& noexcept->allocator_tree&
{
  allocator_tree a_temp{ dango::null };

  dango::swap(m_node, a_temp.m_node);

  return *this;
}

inline auto
dango::
allocator_tree::
operator =
(allocator_tree const& a_tree)& noexcept->allocator_tree&
{
  allocator_tree a_temp{ a_tree };

  dango::swap(m_node, a_temp.m_node);

  return *this;
}

inline auto
dango::
allocator_tree::
operator =
(allocator_tree&& a_tree)& noexcept->allocator_tree&
{
  allocator_tree a_temp{ dango::move(a_tree) };

  dango::swap(m_node, a_temp.m_node);

  return *this;
}

inline auto
dango::
allocator_tree::
get_allocator
()const noexcept->dango::allocator const&
{
  dango_assert(m_node != nullptr);

  return m_node->get_allocator();
}

inline auto
dango::
allocator_tree::
get_child
(dango::usize const a_index)const noexcept->dango::allocator_tree const&
{
  dango_assert(m_node != nullptr);

  auto const a_child = m_node->get_child(a_index);

  if(a_child)
  {
    return *a_child;
  }

  return *this;
}

/*************************************************************/

namespace
dango
{
  inline auto default_static_allocator_storage = dango::allocator::make_static_storage<dango::static_allocator>();

  auto get_default_allocator()noexcept->dango::allocator;
}

inline auto
dango::
get_default_allocator
()noexcept->dango::allocator
{
  return dango::allocator::make_static(dango::default_static_allocator_storage);
}

#endif

