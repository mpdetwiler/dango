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

  dango_assert_terminate(a_void_const   == a_void_ptr);
  dango_assert_terminate((a_void_const <=> a_void_ptr).is_eq());
  dango_assert_terminate(a_bool_const   == a_bool_ptr);
  dango_assert_terminate((a_bool_const <=> a_bool_ptr).is_eq());

  delete new node{ 5, null };
}
DANGO_UNIT_TEST_END

DANGO_UNIT_TEST_BEGIN(main_test2)
{
  using p = dango::priority_tag<4>;

  auto a_ptr_int = dango::detail::auto_ptr_make::make<int, dango::plain_delete_type>(p{ }, dango::plain_delete, 1);
  auto a_ptr_int2 = dango::detail::auto_ptr_make::make<int, dango::polymorphic_allocator<>>(p{ }, dango::default_mem_resource_ptr(), 2);
  auto a_ptr_int3 = dango::detail::auto_ptr_make::make<int, dango::basic_allocator>(p{ }, 3);
  auto a_ptr_void = dango::detail::auto_ptr_make::make<void, dango::polymorphic_allocator<>>(p{ }, dango::default_mem_resource_ptr(), 16, 16);
  auto a_ptr_void2 = dango::detail::auto_ptr_make::make<void, dango::basic_allocator>(p{ }, 16, 16);

  dango_assert_terminate(*a_ptr_int == 1);
  dango_assert_terminate(*a_ptr_int2 == 2);
  dango_assert_terminate(*a_ptr_int3 == 3);
}
DANGO_UNIT_TEST_END

}
