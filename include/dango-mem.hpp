#ifndef __DANGO_MEM_HPP__
#define __DANGO_MEM_HPP__

/*** placement new ***/

namespace
dango
{
  struct
  placement_tag
  final
  {
    DANGO_TAG_TYPE(placement_tag)
  };

  inline constexpr dango::placement_tag const placement{ };
}

[[nodiscard]] constexpr auto
operator new
(dango::usize, dango::placement_tag, void*)noexcept->void*;

[[nodiscard]] constexpr auto
operator new[]
(dango::usize, dango::placement_tag, void*)noexcept->void*;

constexpr void
operator delete
(void*, dango::placement_tag, void*)noexcept;

constexpr void
operator delete[]
(void*, dango::placement_tag, void*)noexcept;

constexpr auto
operator new
(
  dango::usize const,
  dango::placement_tag const,
  void* const a_addr
)
noexcept->void*
{
  return a_addr;
}

constexpr auto
operator new[]
(
  dango::usize const,
  dango::placement_tag const,
  void* const a_addr
)
noexcept->void*
{
  return a_addr;
}

constexpr void
operator delete
(
  void* const,
  dango::placement_tag const,
  void* const
)
noexcept
{

}

constexpr void
operator delete[]
(
  void* const,
  dango::placement_tag const,
  void* const
)
noexcept
{

}

/*** destructor ***/

namespace
dango
{
  template
  <typename tp_type>
  auto
  destructor
  (tp_type const volatile*)
  noexcept(dango::is_noexcept_destructible<tp_type>)->
  dango::enable_if<dango::is_destructible<tp_type>, void>;
}

template
<typename tp_type>
auto
dango::
destructor
(tp_type const volatile* const a_ptr)
noexcept(dango::is_noexcept_destructible<tp_type>)->
dango::enable_if<dango::is_destructible<tp_type>, void>
{
  dango_assert(a_ptr != nullptr);

  a_ptr->~tp_type();
}

/*** dango_new_noexcept ***/

namespace
dango
{
#ifndef DANGO_NEW_NOEXCEPT
  inline constexpr bool const c_operator_new_noexcept = false;
#else
  inline constexpr bool const c_operator_new_noexcept = true;
#endif
}

namespace
dango::detail
{
  constexpr auto true_bool()noexcept->bool;

  template
  <typename tp_type>
  constexpr auto
  true_bool
  (tp_type&&)noexcept->
  dango::enable_if<dango::is_constructible<bool, tp_type>, bool>;
}

constexpr auto
dango::
detail::
true_bool
()noexcept->bool
{
  return true;
}

template
<typename tp_type>
constexpr auto
dango::
detail::
true_bool
(tp_type&& a_arg)noexcept->
dango::enable_if<dango::is_constructible<bool, tp_type>, bool>
{
  return bool(dango::forward<tp_type>(a_arg));
}

#define dango_new_noexcept(cond) \
noexcept(dango::c_operator_new_noexcept && dango::detail::true_bool(cond))

/*** launder ***/

namespace
dango
{
  template
  <typename tp_type>
  [[nodiscard]] constexpr auto
  launder
  (tp_type*)noexcept->
  dango::enable_if<dango::is_object<tp_type>, tp_type*>;
}

template
<typename tp_type>
constexpr auto
dango::
launder
(tp_type* const a_arg)noexcept->
dango::enable_if<dango::is_object<tp_type>, tp_type*>
{
  return __builtin_launder(a_arg);
}

/*** address_of ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr auto
  address_of
  (tp_type&)noexcept->
  dango::enable_if<dango::is_object<tp_type>, tp_type*>;
}

template
<typename tp_type>
constexpr auto
dango::
address_of
(tp_type& a_arg)noexcept->
dango::enable_if<dango::is_object<tp_type>, tp_type*>
{
  using cast_type = dango::preserve_cv<tp_type, dango::byte>;

  auto& a_ref = reinterpret_cast<cast_type&>(a_arg);

  return reinterpret_cast<tp_type*>(&a_ref);
}

/*** is_aligned ***/

namespace
dango
{
  constexpr auto is_aligned(void const volatile*, dango::usize)noexcept->bool;
}

constexpr auto
dango::
is_aligned
(void const volatile* const a_ptr, dango::usize const a_align)noexcept->bool
{
  dango_assert(dango::is_pow_two(a_align));

  auto const a_int = reinterpret_cast<dango::uintptr>(a_ptr);

  return (a_int % dango::uintptr(a_align)) == dango::uintptr(0);
}

/*** operator_new operator_delete ***/

namespace
dango
{
  class heap_ptr;

  [[nodiscard]] auto
  operator_new
  (dango::usize, dango::usize)dango_new_noexcept(true)->dango::heap_ptr;

  void operator_delete(void*, dango::usize, dango::usize)noexcept;
}

/*** heap_ptr ***/

namespace
dango
{
  class heap_ptr;
}

class
dango::
heap_ptr
final
{
  friend auto dango::operator_new(dango::usize, dango::usize)dango_new_noexcept()->dango::heap_ptr;
private:
  using value_type = void*;
private:
  constexpr heap_ptr(value_type, dango::usize, dango::usize)noexcept;
public:
  ~heap_ptr()noexcept;
  auto get()const noexcept->value_type;
  auto size()const noexcept->dango::usize;
  auto align()const noexcept->dango::usize;
  auto dismiss()noexcept->value_type;
private:
  value_type m_ptr;
  dango::usize const m_size;
  dango::usize const m_align;
public:
  DANGO_DELETE_DEFAULT(heap_ptr)
  DANGO_IMMOBILE(heap_ptr)
};

constexpr
dango::
heap_ptr::
heap_ptr
(
  value_type const a_ptr,
  dango::usize const a_size,
  dango::usize const a_align
)
noexcept:
m_ptr{ a_ptr },
m_size{ a_size },
m_align{ a_align }
{

}

inline
dango::
heap_ptr::
~heap_ptr
()noexcept
{
  if(m_ptr)
  {
    dango::operator_delete(m_ptr, m_size, m_align);
  }
}

inline auto
dango::
heap_ptr::
get
()const noexcept->value_type
{
  return m_ptr;
}

inline auto
dango::
heap_ptr::
size
()const noexcept->dango::usize
{
  return m_size;
}

inline auto
dango::
heap_ptr::
align
()const noexcept->dango::usize
{
  return m_align;
}

inline auto
dango::
heap_ptr::
dismiss
()noexcept->value_type
{
  auto const a_ptr = m_ptr;

  m_ptr = nullptr;

  return a_ptr;
}

/*** mem_copy ***/

namespace
dango
{
  constexpr auto mem_copy(void*, void const*, dango::usize)noexcept->void*;
}

constexpr auto
dango::
mem_copy
(
  void* const a_dest,
  void const* const a_source,
  dango::usize const a_count
)
noexcept->void*
{
  return __builtin_memcpy(a_dest, a_source, a_count);
}

/*** aligned_storage ***/

namespace
dango::detail
{
  template
  <dango::usize tp_size, dango::usize tp_align>
  using aligned_storage_enable_spec =
    dango::enable_if<!dango::is_equal(tp_size, dango::usize(0)) && dango::is_pow_two(tp_align)>;
}

namespace
dango
{
  template
  <
    dango::usize tp_size,
    dango::usize tp_align = alignof(dango::max_align_type),
    typename tp_enabled = dango::enable_tag
  >
  class aligned_storage;

  template
  <dango::usize tp_size, dango::usize tp_align>
  class
  aligned_storage
  <tp_size, tp_align, dango::detail::aligned_storage_enable_spec<tp_size, tp_align>>;
}

template
<dango::usize tp_size, dango::usize tp_align>
class
dango::
aligned_storage
<tp_size, tp_align, dango::detail::aligned_storage_enable_spec<tp_size, tp_align>>
{
public:
  static constexpr dango::usize const c_size = dango::next_multiple(tp_size, tp_align);
  static constexpr dango::usize const c_align = tp_align;
public:
  constexpr aligned_storage()noexcept = default;

  ~aligned_storage()noexcept = default;

  constexpr auto get()noexcept->void*;
  constexpr auto get()const noexcept->void const*;
private:
  alignas(c_align) dango::byte m_storage[c_size];
public:
  DANGO_IMMOBILE(aligned_storage)
};

template
<dango::usize tp_size, dango::usize tp_align>
constexpr auto
dango::
aligned_storage
<tp_size, tp_align, dango::detail::aligned_storage_enable_spec<tp_size, tp_align>>::
get
()noexcept->void*
{
  return m_storage;
}

template
<dango::usize tp_size, dango::usize tp_align>
constexpr auto
dango::
aligned_storage
<tp_size, tp_align, dango::detail::aligned_storage_enable_spec<tp_size, tp_align>>::
get
()const noexcept->void const*
{
  return m_storage;
}

#endif

