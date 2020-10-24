#include "dango-allocator.hpp"
#include "dango-mem.hpp"
#include "dango-global.hpp"

#include <new>

auto
dango::
detail::
operator_new_help
(dango::usize const a_size, dango::usize const a_align)dango_new_noexcept->void*
{
  if constexpr(dango::c_operator_new_noexcept)
  {
    try
    {
      return ::operator new(a_size, std::align_val_t{ a_align });
    }
    catch(...)
    {
      dango_unreachable_terminate_msg(u8"memory allocation failed");
    }
  }
  else
  {
    return ::operator new(a_size, std::align_val_t{ a_align });
  }
}

void
dango::
detail::
operator_delete_help
(void const volatile* const a_ptr, dango::usize const a_size, dango::usize const a_align)noexcept
{
  ::operator delete(const_cast<void*>(a_ptr), a_size, std::align_val_t{ a_align });
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
