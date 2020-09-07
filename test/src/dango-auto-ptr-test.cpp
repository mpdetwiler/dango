#include "dango-auto-ptr.hpp"
#include "dango-test-print.hpp"

namespace
{

struct incomplete;

static_assert(dango::auto_ptr_constraint_spec<incomplete, dango::plain_delete_type>);

using incomplete_ptr = dango::auto_ptr<incomplete>;

static_assert(dango::is_same<incomplete_ptr::value_type, incomplete>);
static_assert(dango::is_same<incomplete_ptr::ptr_type, incomplete*>);
static_assert(dango::is_same<incomplete_ptr::ref_type, incomplete&>);
static_assert(dango::is_same<incomplete_ptr::deleter_type, dango::plain_delete_type>);
static_assert(dango::is_same<incomplete_ptr::allocator_type, void>);
static_assert(dango::is_same<incomplete_ptr::allocator_handle_type, void>);
static_assert(dango::is_noexcept_default_constructible<incomplete_ptr>);
static_assert(dango::is_nullable<incomplete_ptr>);
static_assert(dango::is_noexcept_destructible<incomplete_ptr>);
static_assert(dango::is_deleter<dango::plain_delete_type, incomplete>);

struct incomplete
{
  incomplete_ptr m_ptr;
};

static_assert(dango::is_destructible<incomplete>);

struct node
{
  int m_data;
  dango::auto_ptr<node> m_next;
};

static_assert(dango::is_noexcept_destructible<node>);
static_assert(dango::is_noexcept_move_constructible<node>);
static_assert(dango::is_noexcept_move_assignable<node>);
static_assert(!dango::is_copy_constructible<node>);
static_assert(!dango::is_copy_assignable<node>);

DANGO_UNIT_TEST_BEGIN(auto_ptr_incomplete_test)
{
  using dango::auto_ptr;

  auto a_list =
    auto_ptr{ new node{ 1, auto_ptr{ new node{ 2, auto_ptr{ new node{ 3, auto_ptr{ new node{ 4, null } } } } } } } };

  dango_assert_terminate(a_list->m_data == 1);
  dango_assert_terminate(a_list->m_next->m_data == 2);
  dango_assert_terminate(a_list->m_next->m_next->m_data == 3);
  dango_assert_terminate(a_list->m_next->m_next->m_next->m_data == 4);
}
DANGO_UNIT_TEST_END

DANGO_UNIT_TEST_BEGIN(auto_ptr_test1)
{
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
}
DANGO_UNIT_TEST_END

DANGO_UNIT_TEST_BEGIN(auto_ptr_test2)
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

#ifdef DANGO_USING_CLANG
constexpr auto
auto_ptr_contstexpr_test()noexcept(false)->bool
{
  dango::auto_ptr a_ptr{ new int{ 5 } };

  auto a_ptr2{ dango::move(a_ptr) };

  return (*a_ptr2 == 5);
}

static_assert(auto_ptr_contstexpr_test());
#endif

}
