#ifndef __DANGO_MEM_HPP__
#define __DANGO_MEM_HPP__

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
  dango_assert(a_ptr != dango::null);

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

#define dango_new_noexcept noexcept(dango::c_operator_new_noexcept)

#define dango_new_noexcept_and(...) noexcept(dango::c_operator_new_noexcept && bool(__VA_ARGS__))

#define dango_new_noexcept_or(...) noexcept(dango::c_operator_new_noexcept || bool(__VA_ARGS__))

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
  requires(dango::is_object<dango::remove_ref<tp_type>> && dango::is_lvalue_ref<tp_type>)
  constexpr auto
  address_of
  (tp_type&& a_arg)noexcept->dango::remove_ref<tp_type>*
  {
    return __builtin_addressof(a_arg);
  }

  template
  <typename tp_type>
  requires(dango::is_object<dango::remove_ref<tp_type>> && !dango::is_lvalue_ref<tp_type>)
  constexpr auto
  address_of
  (tp_type&&)noexcept = delete;
}

/*** ptr_as_uint ptr_as_sint ***/

namespace
dango
{
  constexpr auto ptr_as_uint(void const volatile*)noexcept->dango::uptr;
  constexpr auto ptr_as_sint(void const volatile*)noexcept->dango::sptr;
}

constexpr auto
dango::
ptr_as_uint
(void const volatile* const a_ptr)noexcept->dango::uptr
{
  using ret_type = dango::uptr;

  if(a_ptr)
  {
    return reinterpret_cast<ret_type>(a_ptr);
  }
  else
  {
    return DANGO_MAGIC_CONST_FOLD(reinterpret_cast<ret_type>(static_cast<void const*>(nullptr)));
  }
}

constexpr auto
dango::
ptr_as_sint
(void const volatile* const a_ptr)noexcept->dango::sptr
{
  using ret_type = dango::sptr;

  if(a_ptr)
  {
    return reinterpret_cast<ret_type>(a_ptr);
  }
  else
  {
    return DANGO_MAGIC_CONST_FOLD(reinterpret_cast<ret_type>(static_cast<void const*>(nullptr)));
  }
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

  if constexpr(dango::in_constexpr_context())
  {
    return a_ptr != nullptr;
  }
  else
  {
    auto const a_int = dango::ptr_as_uint(a_ptr);

    auto const a_mod = a_int % dango::uptr(a_align);

    return (a_ptr != nullptr) && (a_mod == dango::uptr(0));
  }
}

/*** assume_aligned ***/

namespace
dango
{
  template
  <dango::usize tp_align, typename tp_type>
  [[nodiscard]] constexpr auto
  assume_aligned
  (tp_type*)noexcept->
  dango::enable_if<dango::is_pow_two(tp_align) && !dango::is_func<tp_type>, tp_type*>;
}

template
<dango::usize tp_align, typename tp_type>
[[nodiscard]] constexpr auto
dango::
assume_aligned
(tp_type* const a_ptr)noexcept->
dango::enable_if<dango::is_pow_two(tp_align) && !dango::is_func<tp_type>, tp_type*>
{
  dango_assert(dango::is_aligned(a_ptr, tp_align));

  if constexpr(dango::in_constexpr_context())
  {
    return a_ptr;
  }
  else
  {
    auto const a_result =
      __builtin_assume_aligned(const_cast<dango::remove_cv<tp_type> const*>(a_ptr), tp_align);

    return static_cast<tp_type*>(a_result);
  }
}

/*** operator_new operator_delete ***/

namespace
dango
{
  [[nodiscard]] auto
  operator_new
  (dango::usize, dango::usize)dango_new_noexcept->void*;

  void operator_delete(void const volatile*, dango::usize, dango::usize)noexcept;
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
  dango_assert(a_dest != dango::null);
  dango_assert(a_source != dango::null);

  return __builtin_memcpy(a_dest, a_source, a_count);
}

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
(dango::usize, dango::placement_tag, void*, dango::usize, dango::usize)noexcept->void*;

[[nodiscard]] constexpr auto
operator new[]
(dango::usize, dango::placement_tag, void*, dango::usize, dango::usize, dango::usize)noexcept->void*;

constexpr void
operator delete
(void*, dango::placement_tag, void*, dango::usize, dango::usize)noexcept;

constexpr void
operator delete[]
(void*, dango::placement_tag, void*, dango::usize, dango::usize, dango::usize)noexcept;

namespace
dango
{
  template
  <typename tp_type, typename... tp_args>
  requires(dango::is_object<tp_type> && !dango::is_array<tp_type> && dango::is_constructible<tp_type, tp_args...>)
  constexpr auto
  placement_new
  (void*, tp_args&&...)noexcept(dango::is_noexcept_constructible<tp_type, tp_args...>)->tp_type*;

  template
  <typename tp_type>
  requires(dango::is_object<tp_type> && !dango::is_array<tp_type> && dango::is_trivial_default_constructible<tp_type>)
  constexpr auto
  placement_new_trivial
  (void*)noexcept->tp_type*;

  template
  <typename tp_type>
  requires(dango::is_object<tp_type> && !dango::is_array<tp_type> && dango::is_trivial_default_constructible<tp_type>)
  constexpr auto
  placement_new_trivial_array
  (void*, dango::usize)noexcept->tp_type*;
}

constexpr auto
operator new
(
  dango::usize const a_size,
  dango::placement_tag const,
  void* const a_addr,
  dango::usize const a_sizeof,
  dango::usize const a_alignof
)
noexcept->void*
{
  dango_assert(a_addr != dango::null);
  dango_assert(dango::is_aligned(a_addr, a_alignof));
  dango_assert(a_size == a_sizeof);

  return a_addr;
}

constexpr auto
operator new[]
(
  dango::usize const a_size,
  dango::placement_tag const,
  void* const a_addr,
  dango::usize const a_sizeof,
  dango::usize const a_alignof,
  dango::usize const a_count
)
noexcept->void*
{
  dango_assert(a_addr != dango::null);
  dango_assert(dango::is_aligned(a_addr, a_alignof));
  dango_assert(a_size == a_sizeof * a_count);

  return a_addr;
}

constexpr void
operator delete
(
  void* const,
  dango::placement_tag const,
  void* const,
  dango::usize const,
  dango::usize const
)
noexcept
{

}

constexpr void
operator delete[]
(
  void* const,
  dango::placement_tag const,
  void* const,
  dango::usize const,
  dango::usize const,
  dango::usize const
)
noexcept
{

}

template
<typename tp_type, typename... tp_args>
requires(dango::is_object<tp_type> && !dango::is_array<tp_type> && dango::is_constructible<tp_type, tp_args...>)
constexpr auto
dango::
placement_new
(void* const a_addr, tp_args&&... a_args)noexcept(dango::is_noexcept_constructible<tp_type, tp_args...>)->tp_type*
{
  return ::new (dango::placement, a_addr, sizeof(tp_type), alignof(tp_type)) tp_type(dango::forward<tp_args>(a_args)...);
}

template
<typename tp_type>
requires(dango::is_object<tp_type> && !dango::is_array<tp_type> && dango::is_trivial_default_constructible<tp_type>)
constexpr auto
dango::
placement_new_trivial
(void* const a_addr)noexcept->tp_type*
{
  return ::new (dango::placement, a_addr, sizeof(tp_type), alignof(tp_type)) tp_type;
}

template
<typename tp_type>
requires(dango::is_object<tp_type> && !dango::is_array<tp_type> && dango::is_trivial_default_constructible<tp_type>)
constexpr auto
dango::
placement_new_trivial_array
(void* const a_addr, dango::usize const a_count)noexcept->tp_type*
{
  return ::new (dango::placement, a_addr, sizeof(tp_type), alignof(tp_type), a_count) tp_type[a_count];
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

static_assert(dango::is_trivial_default_constructible<dango::aligned_storage<dango::usize(4), dango::usize(4)>>);

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

/*** array_destroy array_copy array_move array_relocate array_shift ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr auto
  array_destroy
  (tp_type const volatile*, dango::usize)noexcept->
  dango::enable_if<dango::is_noexcept_destructible<tp_type>, void>;

  template
  <typename tp_type>
  constexpr auto
  array_copy
  (void* dango_restrict, tp_type const*, dango::usize)
  noexcept(dango::is_noexcept_copy_constructible<tp_type>)->
  dango::enable_if
  <
    !dango::is_volatile<tp_type> &&
    dango::is_copy_constructible<tp_type> &&
    dango::is_noexcept_destructible<tp_type>,
    tp_type*
  >;

  template
  <typename tp_type>
  constexpr auto
  array_move
  (void* dango_restrict, tp_type*, dango::usize)
  noexcept->
  dango::enable_if
  <
    !dango::is_const<tp_type> &&
    !dango::is_volatile<tp_type> &&
    dango::is_noexcept_move_constructible<tp_type>,
    tp_type*
  >;

  template
  <typename tp_type>
  constexpr auto
  array_relocate
  (void* dango_restrict, tp_type*, dango::usize)
  noexcept->
  dango::enable_if
  <
    !dango::is_const<tp_type> &&
    !dango::is_volatile<tp_type> &&
    dango::is_noexcept_move_constructible<tp_type> &&
    dango::is_noexcept_destructible<tp_type>,
    tp_type*
  >;

  template
  <typename tp_type>
  constexpr auto
  array_shift
  (dango::ssize, tp_type*, dango::usize)
  noexcept->
  dango::enable_if
  <
    !dango::is_const<tp_type> &&
    !dango::is_volatile<tp_type> &&
    dango::is_noexcept_move_constructible<tp_type> &&
    dango::is_noexcept_destructible<tp_type>,
    tp_type*
  >;
}

template
<typename tp_type>
constexpr auto
dango::
array_destroy
(tp_type const volatile* const a_array, dango::usize const a_count)noexcept->
dango::enable_if<dango::is_noexcept_destructible<tp_type>, void>
{
  dango_assert(a_array != dango::null);

  if(a_count == dango::usize(0))
  {
    return;
  }

  auto const a_end = a_array + a_count;
  auto a_cur = a_array;

  while(a_cur != a_end)
  {
    dango::destructor(a_cur++);
  }
}

template
<typename tp_type>
constexpr auto
dango::
array_copy
(void* const dango_restrict a_dest, tp_type const* const a_array, dango::usize const a_count)
noexcept(dango::is_noexcept_copy_constructible<tp_type>)->
dango::enable_if
<
  !dango::is_volatile<tp_type> &&
  dango::is_copy_constructible<tp_type> &&
  dango::is_noexcept_destructible<tp_type>,
  tp_type*
>
{
  dango_assert(a_dest != dango::null);
  dango_assert(a_array != dango::null);

  if(a_count == dango::usize(0))
  {
    return dango::null;
  }

  auto const a_end = a_array + a_count;
  auto a_src = a_array;
  auto const a_result = dango::assume_aligned<alignof(tp_type)>(static_cast<tp_type*>(a_dest));
  auto a_dst = a_result;

  auto a_guard =
  dango::make_guard
  (
    [a_result, &a_dst]()noexcept->void
    {
      dango::array_destroy(a_result, dango::usize(a_dst - a_result));
    }
  );

  do
  {
    dango::placement_new<tp_type>(*a_src);

    dango::destructor(a_src);

    ++a_src;
    ++a_dst;
  }
  while(a_src != a_end);

  a_guard.dismiss();

  return a_result;
}

template
<typename tp_type>
constexpr auto
dango::
array_move
(void* const dango_restrict a_dest, tp_type* const a_array, dango::usize const a_count)
noexcept->
dango::enable_if
<
  !dango::is_const<tp_type> &&
  !dango::is_volatile<tp_type> &&
  dango::is_noexcept_move_constructible<tp_type>,
  tp_type*
>
{
  dango_assert(a_dest != dango::null);
  dango_assert(a_array != dango::null);

  if(a_count == dango::usize(0))
  {
    return dango::null;
  }

  tp_type const* const a_end = a_array + a_count;
  auto a_src = a_array;
  auto const a_result = dango::assume_aligned<alignof(tp_type)>(static_cast<tp_type*>(a_dest));
  auto a_dst = a_result;

  do
  {
    dango::placement_new<tp_type>(dango::move(*a_src));

    ++a_src;
    ++a_dst;
  }
  while(a_src != a_end);

  return a_result;
}

template
<typename tp_type>
constexpr auto
dango::
array_relocate
(void* const dango_restrict a_dest, tp_type* const a_array, dango::usize const a_count)
noexcept->
dango::enable_if
<
  !dango::is_const<tp_type> &&
  !dango::is_volatile<tp_type> &&
  dango::is_noexcept_move_constructible<tp_type> &&
  dango::is_noexcept_destructible<tp_type>,
  tp_type*
>
{
  dango_assert(a_dest != dango::null);
  dango_assert(a_array != dango::null);

  if(a_count == dango::usize(0))
  {
    return dango::null;
  }

  tp_type const* const a_end = a_array + a_count;
  auto a_src = a_array;
  auto const a_result = dango::assume_aligned<alignof(tp_type)>(static_cast<tp_type*>(a_dest));
  auto a_dst = a_result;

  do
  {
    dango::placement_new<tp_type>(dango::move(*a_src));

    dango::destructor(a_src);

    ++a_src;
    ++a_dst;
  }
  while(a_src != a_end);

  return a_result;
}

template
<typename tp_type>
constexpr auto
dango::
array_shift
(dango::ssize const a_shift, tp_type* const a_array, dango::usize const a_count)
noexcept->
dango::enable_if
<
  !dango::is_const<tp_type> &&
  !dango::is_volatile<tp_type> &&
  dango::is_noexcept_move_constructible<tp_type> &&
  dango::is_noexcept_destructible<tp_type>,
  tp_type*
>
{
  dango_assert(a_array != dango::null);

  if(a_count == dango::usize(0))
  {
    return dango::null;
  }

  auto const a_result = a_array + a_shift;

  if(a_shift < dango::ssize(0))
  {
    tp_type const* const a_end = a_array + a_count;
    auto a_src = a_array;
    auto a_dst = a_result;

    do
    {
      dango::placement_new<tp_type>(dango::move(*a_src));

      dango::destructor(a_src);

      ++a_src;
      ++a_dst;
    }
    while(a_src != a_end);
  }

  if(a_shift > dango::ssize(0))
  {
    tp_type const* const a_end = a_array;
    auto a_src = a_array + a_count;
    auto a_dst = a_result + a_count;

    do
    {
      --a_src;
      --a_dst;

      dango::placement_new<tp_type>(dango::move(*a_src));

      dango::destructor(a_src);
    }
    while(a_src != a_end);
  }

  return a_result;
}

#endif

