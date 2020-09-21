#include "dango-allocator.hpp"
#include "dango-mem.hpp"
#include "dango-global.hpp"

#include <new>

auto
dango::
operator_new
(
  dango::usize const a_size_arg,
  dango::usize const a_align_arg
)
dango_new_noexcept->void*
{
  dango_assert(a_size_arg != dango::usize(0));
  dango_assert(dango::is_pow_two(a_align_arg));

  auto const a_size = dango::next_multiple(a_size_arg, a_align_arg);
  auto const a_align = std::align_val_t{ a_align_arg };

  if constexpr(dango::c_operator_new_noexcept)
  {
    try
    {
      return ::operator new(a_size, a_align);
    }
    catch(...)
    {
      dango_unreachable_terminate_msg(u8"memory allocation failed");
    }
  }
  else
  {
    return ::operator new(a_size, a_align);
  }
}

void
dango::
operator_delete
(
  void const volatile* const a_ptr,
  dango::usize const a_size_arg,
  dango::usize const a_align_arg
)
noexcept
{
  dango_assert(a_ptr != dango::null);
  dango_assert(a_size_arg != dango::usize(0));
  dango_assert(dango::is_pow_two(a_align_arg));

  auto const a_size = dango::next_multiple(a_size_arg, a_align_arg);
  auto const a_align = std::align_val_t{ a_align_arg };

  ::operator delete(const_cast<void*>(a_ptr), a_size, a_align);
}

constinit dango::default_mem_resource_storage_type
dango::
s_default_mem_resource =
  dango::polymorphic_allocator<>::make_static<dango::basic_mem_resource>();

DANGO_DEFINE_CURRENT_ALLOC_EXPLICIT_INSTANTIATION(dango::polymorphic_allocator<false>)
DANGO_DEFINE_CURRENT_ALLOC_EXPLICIT_INSTANTIATION(dango::polymorphic_allocator<true>)

#ifndef DANGO_NO_DEBUG
auto
dango::
detail::
test::
current_alloc_address_extern()noexcept->void const*
{
  return dango::addressof(dango::detail::current_alloc<dango::polymorphic_allocator<>>::value());
}
#endif

#ifndef DANGO_NO_DEBUG
auto
dango::
detail::
test::
inline_global_address_extern()noexcept->void const*
{
#ifdef DANGO_PLATFORM_WINDOWS
  return dango::null;
#else
  return dango::addressof(*dango::detail::test::inline_global_address_test_lib());
#endif
}
#endif
