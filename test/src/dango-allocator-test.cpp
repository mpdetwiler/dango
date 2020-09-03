#include "dango-allocator.hpp"
#include "dango-test-print.hpp"

namespace
{

DANGO_UNIT_TEST_BEGIN(allocator_test1)
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
}
DANGO_UNIT_TEST_END

}
