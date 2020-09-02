#include "dango.hpp"
#include "dango-global-test.hpp"
#include "dango-test-print.hpp"

static_assert(dango::is_same<decltype(*dango::declval<int*&>()), int&>);

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

struct node
{
  int m_data;
  dango::auto_ptr<node> m_next;
  ~node()noexcept = default;
};

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

  dango::auto_ptr<void> a_void_ptr;
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

  dango_assert_terminate(a_void_const   == a_void_ptr);
  dango_assert_terminate((a_void_const <=> a_void_ptr).is_eq());
  dango_assert_terminate(a_bool_const   == a_bool_ptr);
  dango_assert_terminate((a_bool_const <=> a_bool_ptr).is_eq());

  delete new node{ 5, null };
}
DANGO_UNIT_TEST_END

DANGO_UNIT_TEST_BEGIN(main_test2)
{
  auto a_ptr_int = dango::make_auto_ptr<int>(42);
  auto a_ptr_void = dango::make_auto_ptr<void>(64, 64);

  a_ptr_int.get_deleter();

  static_assert(sizeof(a_ptr_int) == sizeof(int*));
  static_assert(sizeof(a_ptr_void) == 3 * sizeof(void*));

  auto a_ptr_void2 = dango::make_auto_ptr<void, dango::polymorphic_allocator<>>(dango::default_mem_resource_ptr(), 64, 64);

  a_ptr_void2.get_allocator_handle();

  static_assert(sizeof(a_ptr_void2) == 4 * sizeof(void*));

  dango_assert_terminate(*a_ptr_int == 42);
}
DANGO_UNIT_TEST_END

}
