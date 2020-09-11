#include "dango.hpp"

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

template struct dango::detail::current_alloc<dango::polymorphic_allocator<false>>;
template struct dango::detail::current_alloc<dango::polymorphic_allocator<true>>;

#include <cstdio>

void
print_tls_test()noexcept
{
  void const* const a_addr = &(dango::detail::current_alloc<dango::polymorphic_allocator<true>>::value());

  std::fprintf(stderr, "export print_tls_test: %p\n", a_addr);
}
