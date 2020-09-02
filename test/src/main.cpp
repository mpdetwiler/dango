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
  auto a_deleter =
   [](int const volatile* const a_ptr)noexcept->void
   {
     test_print("deleting: %p\n", static_cast<void const volatile*>(a_ptr));

     delete a_ptr;
   };

  auto a_ptr_int1 = dango::make_auto_ptr<int>(1);
  auto a_ptr_int2 = dango::auto_ptr{ new int{ 2 } };
  auto a_ptr_int3 = dango::auto_ptr<int const volatile>{ new int{ 3 }, dango::plain_delete };
  auto a_ptr_int4 = dango::auto_ptr<int, decltype(a_deleter)>{ new int{ 4 } };
  auto a_ptr_int5 = dango::auto_ptr{ new int{ 5 }, dango::move(a_deleter) };
  auto a_ptr_int6 = dango::make_auto_ptr<int, dango::basic_allocator>(6);
  auto a_ptr_int7 = dango::make_auto_ptr<int const volatile, dango::polymorphic_allocator<>>(dango::default_mem_resource_ptr(), 7);

  a_ptr_int1.get_deleter();
  a_ptr_int2.get_deleter();
  a_ptr_int3.get_deleter();
  a_ptr_int4.get_deleter();
  a_ptr_int5.get_deleter();
  a_ptr_int7.get_allocator_handle();

  static_assert(sizeof(a_ptr_int1) == sizeof(int*));
  static_assert(sizeof(a_ptr_int2) == sizeof(int*));
  static_assert(sizeof(a_ptr_int3) == sizeof(int*));
  static_assert(sizeof(a_ptr_int4) == sizeof(int*));
  static_assert(sizeof(a_ptr_int5) == sizeof(int*));
  static_assert(sizeof(a_ptr_int6) == 2 * sizeof(int*));
  static_assert(sizeof(a_ptr_int7) == 2 * sizeof(int*));

  auto a_ptr_void1 = dango::make_auto_ptr<void>(64, 64);
  auto a_ptr_void2 = dango::make_auto_ptr<void const, dango::polymorphic_allocator<>>(dango::default_mem_resource_ptr(), 64, 64);

  a_ptr_void2.get_allocator_handle();

  static_assert(sizeof(a_ptr_void1) == 3 * sizeof(void*));
  static_assert(sizeof(a_ptr_void2) == 4 * sizeof(void*));

  dango_assert_terminate(*a_ptr_int1 == 1);
  dango_assert_terminate(*a_ptr_int2 == 2);
  dango_assert_terminate(*a_ptr_int3 == 3);
  dango_assert_terminate(*a_ptr_int4 == 4);
  dango_assert_terminate(*a_ptr_int5 == 5);
  dango_assert_terminate(*a_ptr_int6 == 6);
  dango_assert_terminate(*a_ptr_int7 == 7);
}
DANGO_UNIT_TEST_END

}
