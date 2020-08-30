#include "dango.hpp"
#include "dango-global-test.hpp"
#include "dango-test-print.hpp"

auto
main
()noexcept(false)->int
{
  test_print("main entered\n");

  test_print("main exiting\n");

  return 0;
}

namespace
{

DANGO_UNIT_TEST_BEGIN(main_test)
{
  auto a_rs = dango::polymorphic_allocator<>::make<dango::basic_mem_resource>();

  auto a_ptr_mut = a_rs.get_ptr();

  dango::swap(a_ptr_mut, a_ptr_mut);

  auto const a_ptr = a_ptr_mut;

  dango_assert_nonnull_terminate(a_ptr);
  dango_assert_terminate(!(a_ptr  == null));
  dango_assert_terminate(  a_ptr  != null);
  dango_assert_terminate(!(null   == a_ptr));
  dango_assert_terminate(  null   != a_ptr);
  dango_assert_terminate(  a_ptr  == a_ptr);
  dango_assert_terminate(!(a_ptr  != a_ptr));
  dango_assert_terminate(!(a_ptr  >  a_ptr));
  dango_assert_terminate(  a_ptr  >= a_ptr);
  dango_assert_terminate(!(a_ptr  <  a_ptr));
  dango_assert_terminate(  a_ptr  <= a_ptr);
  dango_assert_terminate( (a_ptr <=> a_ptr).is_eq());
  dango::hash(a_ptr);

  dango_assert_terminate(dango::max(a_ptr, a_ptr, a_ptr, a_ptr) == a_ptr);
  dango_assert_terminate(dango::min(a_ptr, a_ptr, a_ptr, a_ptr) == a_ptr);

  auto const a_mem = a_ptr->alloc(16, 16);

  a_ptr->dealloc(a_mem, 16, 16);

  dango::auto_ptr<void> a_void_ptr = null;
  dango::auto_ptr<bool> a_bool_ptr = null;

  a_void_ptr = null;
  a_void_ptr = dango::move(a_void_ptr);

  a_bool_ptr = null;
  a_bool_ptr = dango::move(a_bool_ptr);

  a_bool_ptr.get_deleter();

  dango_assert_terminate(a_bool_ptr == a_bool_ptr);

  dango::auto_ptr<bool const volatile> a_bool_const{ dango::move(a_bool_ptr) };
  dango::auto_ptr<void const volatile> a_void_const{ dango::move(a_void_ptr) };

  a_bool_const = dango::move(a_bool_ptr);
  a_void_const = dango::move(a_void_ptr);
}
DANGO_UNIT_TEST_END

}
