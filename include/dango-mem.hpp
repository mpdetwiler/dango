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
dango::detail
{
  [[nodiscard, gnu::malloc, gnu::alloc_size(1)]] DANGO_EXPORT auto operator_new_help(dango::usize, dango::usize)dango_new_noexcept->void*;

  DANGO_EXPORT void operator_delete_help(void const volatile*, dango::usize, dango::usize)noexcept;
}

namespace
dango
{
  [[nodiscard]] inline auto
  operator_new
  (dango::usize a_size, dango::usize const a_align)dango_new_noexcept->void*
  {
    dango_assert(dango::is_pow_two(a_align));

    a_size = dango::next_multiple(dango::max(dango::usize(1), a_size), a_align);

    return dango::detail::operator_new_help(a_size, a_align);
  }

  inline void
  operator_delete
  (void const volatile* const a_ptr, dango::usize a_size, dango::usize const a_align)noexcept
  {
    dango_assert_nonnull(a_ptr);
    dango_assert(dango::is_pow_two(a_align));

    a_size = dango::next_multiple(dango::max(dango::usize(1), a_size), a_align);

    dango::detail::operator_delete_help(a_ptr, a_size, a_align);
  }
}

/*** placement new ***/

namespace
dango
{
  struct
  placement_new_tag
  final
  {
    DANGO_TAG_TYPE(placement_new_tag)
  };

  inline constexpr dango::placement_new_tag const placement_new{ };
}

template
<dango::is_object_exclude_array tp_type>
requires(dango::is_noexcept_destructible<tp_type>)
[[nodiscard]] constexpr auto
operator new
(
  dango::usize const a_size,
  dango::placement_new_tag const,
  void* const a_addr,
  dango::type_tag<tp_type> const
)
noexcept->void*
{
  dango_assert_nonnull(a_addr);
  dango_assert(dango::is_aligned(a_addr, alignof(tp_type)));
  dango_assert(a_size == sizeof(tp_type));

  return a_addr;
}

template
<dango::is_object_exclude_array tp_type>
requires(dango::is_noexcept_destructible<tp_type>)
constexpr void
operator delete
(
  void* const,
  dango::placement_new_tag const,
  void* const,
  dango::type_tag<tp_type> const
)
noexcept
{

}

template
<dango::is_object_exclude_array tp_type>
requires(dango::is_trivial_destructible<tp_type>)
[[nodiscard]] constexpr auto
operator new[]
(
  dango::usize const a_size,
  dango::placement_new_tag const,
  void* const a_addr,
  dango::type_tag<tp_type> const,
  dango::usize const a_count
)
noexcept->void*
{
  dango_assert(a_addr != dango::null);
  dango_assert(dango::is_aligned(a_addr, alignof(tp_type)));
  dango_assert(a_size == a_count * sizeof(tp_type));

  return a_addr;
}

template
<dango::is_object_exclude_array tp_type>
requires(dango::is_trivial_destructible<tp_type>)
constexpr void
operator delete[]
(
  void* const,
  dango::placement_new_tag const,
  void* const,
  dango::type_tag<tp_type> const,
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
    dango_assert_nonnull(a_array);

    if constexpr(!dango::is_trivial_destructible<tp_type>)
    {
      if(a_count == dango::usize(0))
      {
        return;
      }

      auto const a_end = a_array + a_count;

      for(auto a_cur = a_array; a_cur != a_end; ++a_cur)
      {
        dango::qualified_destructor(a_cur);
      }
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
