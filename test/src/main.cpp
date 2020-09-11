#include "dango.hpp"
#include "dango-global-test.hpp"
#include "dango-test-print.hpp"

auto
main
()noexcept(false)->int
{
  test_print("main entered\n");

  print_tls_test();

  void const* const a_addr = &(dango::detail::current_alloc<dango::polymorphic_allocator<true>>::value());

  test_print("inline print_tls_test: %p\n", a_addr);

  test_print("main exiting\n");

  return 0;
}
