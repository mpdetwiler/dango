#ifndef __DANGO_ALLOCATOR_HPP__
#define __DANGO_ALLOCATOR_HPP__

/*** allocator classes ***/

namespace
dango
{
  class allocator_base;
  class allocator;
  class allocator_tree;
}

class
dango::
allocator_base
{
  friend dango::allocator;
public:
  constexpr allocator_base()noexcept;
  virtual ~allocator_base()noexcept;
protected:
  virtual auto alloc(dango::usize, dango::usize)dango_new_noexcept(true)->void*;
  virtual void free(void const volatile*, dango::usize, dango::usize)noexcept;
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
allocator_base::
allocator_base
()noexcept:
m_alloc_count{ dango::usize(0) }
{

}

inline
dango::
allocator_base::
~allocator_base
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

inline auto
dango::
allocator_base::
alloc
(dango::usize const a_size, dango::usize const a_align)dango_new_noexcept(true)->void*
{
  return dango::operator_new(a_size, a_align).dismiss();
}

inline void
dango::
allocator_base::
free
(void const volatile* const a_ptr, dango::usize const a_size, dango::usize const a_align)noexcept
{
  dango::operator_delete(a_ptr, a_size, a_align);
}

inline auto
dango::
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

///////////////////////////////////////////////////////////////

class
dango::
allocator
final
{
  friend dango::allocator_tree;
private:
  class control_base;
  template
  <typename tp_alloc>
  class control_block;
public:
  template
  <typename tp_alloc, typename... tp_args>
  static auto
  make
  (tp_args&&...)dango_new_noexcept(dango::is_noexcept_constructible<tp_alloc, tp_args...>)->
  dango::enable_if
  <
    !dango::is_const<tp_alloc> &&
    !dango::is_volatile<tp_alloc> &&
    dango::is_class<tp_alloc> &&
    dango::is_base_of<dango::allocator_base, tp_alloc> &&
    dango::is_noexcept_destructible<tp_alloc> &&
    dango::is_constructible<tp_alloc, tp_args...>,
    dango::allocator
  >;
private:
  explicit allocator(control_base*)noexcept;
public:
  constexpr allocator(dango::null_tag const)noexcept:m_control{ nullptr }{ };
  allocator(allocator const&)noexcept;
  allocator(allocator&&)noexcept;
  ~allocator()noexcept;
  auto operator = (dango::null_tag)& noexcept->allocator&;
  auto operator = (allocator const&)& noexcept->allocator&;
  auto operator = (allocator&&)& noexcept->allocator&;
public:
  constexpr auto dango_operator_is_null()const noexcept->bool{ return m_control == nullptr; }
public:
  auto alloc(dango::usize, dango::usize)const dango_new_noexcept(true)->void*;
  void free(void const volatile*, dango::usize, dango::usize)const noexcept;
  auto as_tree()const noexcept->dango::allocator_tree;
private:
  control_base* m_control;
};

class
dango::
allocator_tree
final
{
  template
  <typename tp_alloc>
  friend class dango::allocator::control_block;
private:
  class node_base;
  class weak_node;
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
  static auto make_node(tp_alloc&&, tp_children&&...)dango_new_noexcept(true)->node<sizeof...(tp_children)>*;
private:
  explicit allocator_tree(weak_node*)noexcept;
public:
  template
  <
    typename tp_alloc,
    typename... tp_children,
    dango::enable_if
    <(dango::is_same<dango::remove_cvref<tp_alloc>, dango::allocator> && ... && dango::is_same<dango::remove_cvref<tp_children>, dango::allocator_tree>)>
      = dango::enable_val
  >
  explicit
  allocator_tree
  (tp_alloc&&, tp_children&&...)dango_new_noexcept(true);
public:
  constexpr allocator_tree(dango::null_tag const)noexcept:m_node{ nullptr }{ };
  allocator_tree(allocator_tree const&)noexcept;
  allocator_tree(allocator_tree&&)noexcept;
  ~allocator_tree()noexcept;
  auto operator = (dango::null_tag)& noexcept->allocator_tree&;
  auto operator = (allocator_tree const&)& noexcept->allocator_tree&;
  auto operator = (allocator_tree&&)& noexcept->allocator_tree&;
  constexpr auto dango_operator_is_null()const noexcept->bool{ return m_node == nullptr; }
public:
  auto get_allocator()const noexcept->dango::allocator const&;
  auto get_child(dango::usize)const noexcept->allocator_tree const&;
private:
  node_base* m_node;
public:
  DANGO_DELETE_DEFAULT(allocator_tree)
};

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
  static auto increment_weak(weak_node*)noexcept->bool;
protected:
  template
  <typename tp_alloc>
  constexpr
  node_base
  (dango::usize const a_init, tp_alloc&& a_alloc)noexcept:
  m_ref_count{ a_init },
  m_alloc{ dango::forward<tp_alloc>(a_alloc) }
  { }
  ~node_base()noexcept = default;
public:
  auto get_allocator()const noexcept->dango::allocator const&{ return m_alloc; }
  virtual auto get_child(dango::usize)const noexcept->dango::allocator_tree const* = 0;
protected:
  auto get_allocator_mutable()noexcept->dango::allocator&{ return m_alloc; }
private:
  virtual auto get_destructor()const noexcept->destructor_func = 0;
private:
  dango::atomic<dango::usize> m_ref_count;
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

  auto const a_destructor = a_node->get_destructor();

  a_destructor(a_node);
}

class
dango::
allocator_tree::
weak_node
final:
public dango::allocator_tree::node_base
{
private:
  using super_type = dango::allocator_tree::node_base;
  using super_type::destructor_func;
  using control_base = dango::allocator::control_base;
public:
  static auto make(control_base*)dango_new_noexcept(true)->weak_node*;
  static void destroy(control_base*, weak_node const*)noexcept;
private:
  static void destructor(super_type*)noexcept;
public:
  constexpr explicit
  weak_node
  (control_base* const a_control)noexcept:
  super_type{ dango::usize(0), dango::null },
  m_control{ a_control }
  { }
  ~weak_node()noexcept = default;
public:
  void strengthen()noexcept;
  virtual auto get_child(dango::usize const)const noexcept->dango::allocator_tree const* override final{ return nullptr; }
private:
  virtual auto get_destructor()const noexcept->destructor_func override final{ return destructor; };
private:
  control_base* const m_control;
public:
  DANGO_DELETE_DEFAULT(weak_node)
  DANGO_IMMOBILE(weak_node)
};

inline auto
dango::
allocator_tree::
node_base::
increment_weak
(weak_node* const a_node)noexcept->bool
{
  dango_assert(a_node != nullptr);

  auto const a_prev = a_node->m_ref_count.fetch_add<dango::mem_order::acquire>(dango::usize(1));

  return a_prev == dango::usize(0);
}

inline void
dango::
allocator_tree::
weak_node::
destructor
(super_type* const a_node)noexcept
{
  auto const a_this = static_cast<weak_node*>(a_node);

  auto& a_alloc = a_this->get_allocator_mutable();

  dango_assert(a_alloc != dango::null);

  a_alloc = dango::null;
}

inline void
dango::
allocator_tree::
weak_node::
strengthen
()noexcept
{
  if(!super_type::increment_weak(this))
  {
    return;
  }

  auto& a_alloc = get_allocator_mutable();

  dango_assert(a_alloc == dango::null);

  a_alloc = dango::allocator{ m_control };
}

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
  super_type{ dango::usize(1), dango::forward<tp_alloc>(a_alloc) }
  { }
  ~node()noexcept = default;
public:
  virtual auto get_child(dango::usize const)const noexcept->dango::allocator_tree const* override final{ return nullptr; }
private:
  virtual auto get_destructor()const noexcept->destructor_func override final{ return destructor; };
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
  super_type{ dango::usize(1), dango::forward<tp_alloc>(a_alloc) },
  m_child{ dango::forward<tp_children>(a_children)... }
  { }
  ~node()noexcept = default;
public:
  virtual auto get_child(dango::usize)const noexcept->dango::allocator_tree const* override final;
private:
  virtual auto get_destructor()const noexcept->destructor_func override final{ return destructor; }
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

class alignas(dango::cache_align_type)
dango::
allocator::
control_base
{
public:
  static auto operator new(dango::usize)noexcept->void* = delete;
  static void operator delete(void* const, dango::usize const)noexcept{ dango_unreachable; }
protected:
  constexpr
  control_base
  (dango::allocator_base* const a_alloc_ptr)noexcept:
  m_ref_count{ dango::usize(0) },
  m_alloc_ptr{ a_alloc_ptr }
  { }
public:
  virtual ~control_base()noexcept = default;
public:
  void increment()noexcept;
  auto decrement()noexcept->bool;
  auto alloc(dango::usize, dango::usize)dango_new_noexcept(true)->void*;
  void free(void const volatile*, dango::usize, dango::usize)noexcept;
public:
  virtual auto get_cached_tree()noexcept->dango::allocator_tree = 0;
private:
  dango::atomic<dango::usize> m_ref_count;
  dango::allocator_base* const m_alloc_ptr;
public:
  DANGO_DELETE_DEFAULT(control_base)
  DANGO_IMMOBILE(control_base)
};

inline void
dango::
allocator::
control_base::
increment
()noexcept
{
  m_ref_count.add_fetch<dango::mem_order::acquire>(dango::usize(1));
}

inline auto
dango::
allocator::
control_base::
decrement
()noexcept->bool
{
  auto const a_prev = m_ref_count.fetch_sub<dango::mem_order::release>(dango::usize(1));

  dango_assert(a_prev != dango::usize(0));

  return a_prev == dango::usize(1);
}

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

inline auto
dango::
allocator_tree::
weak_node::
make
(control_base* const a_control)dango_new_noexcept(true)->weak_node*
{
  auto const a_ptr = a_control->alloc(sizeof(weak_node), alignof(weak_node));

  return ::new (dango::placement, a_ptr) weak_node{ a_control };
}

inline void
dango::
allocator_tree::
weak_node::
destroy
(control_base* const a_control, weak_node const* const a_this)noexcept
{
  dango::destructor(a_this);

  a_control->free(a_this, sizeof(weak_node), alignof(weak_node));
}

template
<typename tp_alloc>
class
dango::
allocator::
control_block
final:
public dango::allocator::control_base
{
private:
  using super_type = dango::allocator::control_base;
  using weak_node = dango::allocator_tree::weak_node;
public:
  static auto operator new(dango::usize const a_size)dango_new_noexcept(true)->void*{ return dango::operator_new(a_size, alignof(control_block)).dismiss(); }
  static void operator delete(void* const a_ptr, dango::usize const a_size)noexcept{ dango::operator_delete(a_ptr, a_size, alignof(control_block)); }
public:
  template
  <typename... tp_args>
  explicit
  control_block
  (tp_args&&... a_args)noexcept:
  super_type{ &m_alloc },
  m_alloc{ dango::forward<tp_args>(a_args)... },
  m_tree{ weak_node::make(this) }
  { }
  virtual ~control_block()noexcept override final{ weak_node::destroy(this, m_tree); }
public:
  virtual auto get_cached_tree()noexcept->dango::allocator_tree override final{ return dango::allocator_tree{ m_tree }; }
private:
  tp_alloc m_alloc;
  weak_node* const m_tree;
public:
  DANGO_IMMOBILE(control_block)
};

inline
dango::
allocator::
allocator
(control_base* const a_control)noexcept:
m_control{ a_control }
{
  dango_assert(m_control != nullptr);

  m_control->increment();
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

inline
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

inline auto
dango::
allocator::
as_tree
()const noexcept->dango::allocator_tree
{
  dango_assert(m_control != nullptr);

  return m_control->get_cached_tree();
}

template
<typename tp_alloc, typename... tp_args>
auto
dango::
allocator::
make
(tp_args&&... a_args)dango_new_noexcept(dango::is_noexcept_constructible<tp_alloc, tp_args...>)->
dango::enable_if
<
  !dango::is_const<tp_alloc> &&
  !dango::is_volatile<tp_alloc> &&
  dango::is_class<tp_alloc> &&
  dango::is_base_of<dango::allocator_base, tp_alloc> &&
  dango::is_noexcept_destructible<tp_alloc> &&
  dango::is_constructible<tp_alloc, tp_args...>,
  dango::allocator
>
{
  return dango::allocator{ new control_block<tp_alloc>{ dango::forward<tp_args>(a_args)... } };
}

template
<typename tp_alloc, typename... tp_children>
auto
dango::
allocator_tree::
make_node
(tp_alloc&& a_alloc, tp_children&&... a_children)dango_new_noexcept(true)->node<sizeof...(tp_children)>*
{
  using node_type = node<sizeof...(tp_children)>;

  auto const a_ptr = a_alloc.alloc(sizeof(node_type), alignof(node_type));

  return ::new (dango::placement, a_ptr) node_type{ dango::forward<tp_alloc>(a_alloc), dango::forward<tp_children>(a_children)... };
}

inline
dango::
allocator_tree::
allocator_tree
(weak_node* const a_node)noexcept:
m_node{ a_node }
{
  a_node->strengthen();
}

template
<
  typename tp_alloc,
  typename... tp_children,
  dango::enable_if
  <(dango::is_same<dango::remove_cvref<tp_alloc>, dango::allocator> && ... && dango::is_same<dango::remove_cvref<tp_children>, dango::allocator_tree>)>
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

inline
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

#endif

