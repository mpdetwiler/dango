#ifndef DANGO_MEM_HPP_INCLUDED
#define DANGO_MEM_HPP_INCLUDED

#include "dango-util.hpp"

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

#define dango_new_noexcept_and(cond, ...) \
  noexcept(dango::c_operator_new_noexcept && bool(cond __VA_OPT__(, __VA_ARGS__)))

#define dango_new_noexcept_or(cond, ...) \
  noexcept(dango::c_operator_new_noexcept || bool(cond __VA_OPT__(, __VA_ARGS__)))

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

  if(dango::in_constexpr_context())
  {
    return a_ptr != dango::null;
  }
  else
  {
    auto const a_int = dango::ptr_as_uint(a_ptr);

    auto const a_mod = a_int % dango::uptr(a_align);

    return (a_ptr != dango::null) && (a_mod == dango::uptr(0));
  }
}

/*** assume_aligned ***/

namespace
dango
{
  template
  <dango::usize tp_align, typename tp_type>
  requires(dango::is_pow_two(tp_align) && !dango::is_func<tp_type>)
  [[nodiscard]] constexpr auto
  assume_aligned
  (tp_type* const a_ptr)noexcept->tp_type*
  {
    dango_assert(dango::is_aligned(a_ptr, tp_align));

    if(dango::in_constexpr_context())
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
}

/*** operator_new operator_delete ***/

namespace
dango
{
  [[nodiscard, gnu::malloc, gnu::alloc_size(1)]] DANGO_EXPORT auto operator_new(dango::usize, dango::usize)dango_new_noexcept->void*;

  DANGO_EXPORT void operator_delete(void const volatile*, dango::usize, dango::usize)noexcept;
}

#define DANGO_DEFINE_CLASS_OPERATOR_NEW_DELETE(name) \
  static auto operator new(std::size_t const a_size)dango_new_noexcept->void* \
  { dango_assert(a_size == sizeof(name)); return dango::operator_new(sizeof(name), alignof(name)); } \
  static void operator delete(void* const a_ptr)noexcept \
  { dango::operator_delete(a_ptr, sizeof(name), alignof(name)); }

#define DANGO_DELETE_CLASS_OPERATOR_NEW_DELETE \
  static auto operator new(std::size_t)noexcept->void* = delete; \
  static void operator delete(void*)noexcept = delete;

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

#define dango_placement_new(addr, type, ...) \
  ::new (dango::placement, static_cast<void*>(addr), sizeof(type), alignof(type)) type __VA_OPT__(__VA_ARGS__)

#define dango_placement_new_array(addr, type, count) \
  ::new (dango::placement, static_cast<void*>(addr), sizeof(type), alignof(type)) type[dango::usize(count)]

[[nodiscard]] constexpr auto
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

[[nodiscard]] constexpr auto
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

/*** array_destroy array_copy array_move array_relocate array_shift ***/

namespace
dango
{
  template
  <dango::is_noexcept_destructible tp_type>
  constexpr auto
  array_destroy
  (tp_type const volatile* const a_array, dango::usize const a_count)noexcept->void
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
  requires
  (!dango::is_volatile<tp_type> && dango::is_copy_constructible<tp_type> && dango::is_noexcept_destructible<tp_type>)
  constexpr auto
  array_copy
  (void* const dango_restrict a_dest, tp_type const* const a_array, dango::usize const a_count)
  noexcept(dango::is_noexcept_copy_constructible<tp_type>)->tp_type*
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
      dango_placement_new(a_dst, tp_type, { *a_src });

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
  requires(!dango::is_const<tp_type> && !dango::is_volatile<tp_type> && dango::is_noexcept_move_constructible<tp_type>)
  constexpr auto
  array_move
  (void* const dango_restrict a_dest, tp_type* const a_array, dango::usize const a_count)noexcept->tp_type*
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
      dango_placement_new(a_dst, tp_type, { dango::move(*a_src) });

      ++a_src;
      ++a_dst;
    }
    while(a_src != a_end);

    return a_result;
  }

  template
  <typename tp_type>
  requires(!dango::is_const<tp_type> && !dango::is_volatile<tp_type> && dango::is_noexcept_move_constructible<tp_type>)
  constexpr auto
  array_relocate
  (void* const dango_restrict a_dest, tp_type* const a_array, dango::usize const a_count)noexcept->tp_type*
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
      dango_placement_new(a_dst, tp_type, { dango::move(*a_src) });

      dango::destructor(a_src);

      ++a_src;
      ++a_dst;
    }
    while(a_src != a_end);

    return a_result;
  }

  template
  <typename tp_type>
  requires(!dango::is_const<tp_type> && !dango::is_volatile<tp_type> && dango::is_noexcept_move_constructible<tp_type>)
  constexpr auto
  array_shift
  (dango::ssize const a_shift, tp_type* const a_array, dango::usize const a_count)noexcept->tp_type*
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
        dango_placement_new(a_dst, tp_type, { dango::move(*a_src) });

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

        dango_placement_new(a_dst, tp_type, { dango::move(*a_src) });

        dango::destructor(a_src);
      }
      while(a_src != a_end);
    }

    return a_result;
  }
}

#endif // DANGO_MEM_HPP_INCLUDED
