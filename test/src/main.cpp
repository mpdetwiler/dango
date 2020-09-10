#include "dango.hpp"
#include "dango-global-test.hpp"
#include "dango-test-print.hpp"

auto
main
()noexcept(false)->int
{
  test_print("main entered\n");

  print_tls_test();

  test_print("inline print_tls_test: %p\n", static_cast<void const*>(&dango::detail::current_allocator_handle<dango::polymorphic_allocator<>>()));

  test_print("main exiting\n");

  return 0;
}
