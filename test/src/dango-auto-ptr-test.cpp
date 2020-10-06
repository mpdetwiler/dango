#include "dango-auto-ptr.hpp"
#include "dango-test-print.hpp"

template class dango::auto_ptr<double, dango::plain_delete_type>;
template class dango::auto_ptr<double, dango::basic_allocator>;
template class dango::auto_ptr<double, dango::polymorphic_allocator<>>;
template class dango::auto_ptr<void, dango::basic_allocator>;
template class dango::auto_ptr<void, dango::polymorphic_allocator<>>;

namespace
{

struct incomplete;

using incomplete_ptr = dango::auto_ptr<incomplete>;

static_assert(dango::is_same<incomplete_ptr::elem_type, incomplete>);
static_assert(dango::is_same<incomplete_ptr::ptr_type, incomplete*>);
static_assert(dango::is_same<incomplete_ptr::ref_type, incomplete&>);
static_assert(dango::is_same<incomplete_ptr::deleter_type, dango::plain_delete_type>);
static_assert(dango::is_noexcept_default_constructible<incomplete_ptr>);
static_assert(dango::is_nullable<incomplete_ptr>);
static_assert(dango::is_noexcept_destructible<incomplete_ptr>);
static_assert(dango::is_deleter<dango::plain_delete_type, incomplete>);

using incomplete_ptr2 = dango::auto_ptr<incomplete, dango::basic_allocator>;

static_assert(dango::is_same<incomplete_ptr2::elem_type, incomplete>);
static_assert(dango::is_same<incomplete_ptr2::ptr_type, incomplete*>);
static_assert(dango::is_same<incomplete_ptr2::ref_type, incomplete&>);
static_assert(dango::is_same<incomplete_ptr2::allocator_type, dango::basic_allocator>);
static_assert(dango::is_same<incomplete_ptr2::allocator_handle_type, void>);
static_assert(dango::is_noexcept_default_constructible<incomplete_ptr2>);
static_assert(dango::is_nullable<incomplete_ptr2>);
static_assert(dango::is_noexcept_destructible<incomplete_ptr2>);

using incomplete_ptr3 = dango::auto_ptr<incomplete, dango::polymorphic_allocator<>>;

static_assert(dango::is_same<incomplete_ptr3::elem_type, incomplete>);
static_assert(dango::is_same<incomplete_ptr3::ptr_type, incomplete*>);
static_assert(dango::is_same<incomplete_ptr3::ref_type, incomplete&>);
static_assert(dango::is_same<incomplete_ptr3::allocator_type, dango::polymorphic_allocator<>>);
static_assert(dango::is_same<incomplete_ptr3::allocator_handle_type, dango::mem_resource_ptr<>>);
static_assert(dango::is_noexcept_default_constructible<incomplete_ptr3>);
static_assert(dango::is_nullable<incomplete_ptr3>);
static_assert(dango::is_noexcept_destructible<incomplete_ptr3>);

struct incomplete
{
  incomplete_ptr m_ptr;
  incomplete_ptr2 m_ptr2;
  incomplete_ptr2 m_ptr3;
};

static_assert(dango::is_noexcept_destructible<incomplete>);

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
  auto a_ptr_int5 = dango::auto_ptr{ new int{ 5 }, a_deleter };
  auto a_ptr_int7 = dango::make_auto_ptr<int, dango::basic_allocator>(7);
  auto a_ptr_int8 = dango::make_auto_ptr<int const volatile, dango::polymorphic_allocator<>>(dango::default_mem_resource_ptr(), 8);
  auto a_ptr_int9 = dango::make_auto_ptr<int const, dango::polymorphic_allocator<>>(9);

  auto a_ptr_ptr_int =
    dango::make_auto_ptr<dango::auto_ptr<int, dango::polymorphic_allocator<>>, dango::polymorphic_allocator<>>
    (
      dango::make_auto_ptr<int, dango::polymorphic_allocator<>>(9)
    );

  a_ptr_int1.get_deleter();
  a_ptr_int2.get_deleter();
  a_ptr_int3.get_deleter();
  a_ptr_int4.get_deleter();
  a_ptr_int5.get_deleter();

  dango_assert_terminate(a_ptr_int8.get_allocator_handle() == a_ptr_int9.get_allocator_handle());
  dango_assert_terminate(a_ptr_ptr_int.get_allocator_handle() == a_ptr_ptr_int->get_allocator_handle());

  static_assert(sizeof(a_ptr_int1) == sizeof(int*));
  static_assert(sizeof(a_ptr_int2) == sizeof(int*));
  static_assert(sizeof(a_ptr_int3) == sizeof(int*));
  static_assert(sizeof(a_ptr_int4) == sizeof(int*));
  static_assert(sizeof(a_ptr_int5) == sizeof(int*));
  static_assert(sizeof(a_ptr_int7) == 2 * sizeof(int*));
  static_assert(sizeof(a_ptr_int8) == 2 * sizeof(int*));
  static_assert(sizeof(a_ptr_int9) == 2 * sizeof(int*));
  static_assert(sizeof(a_ptr_ptr_int) == 2 * sizeof(int*));

  auto a_ptr_void1 = dango::make_auto_ptr<void>(64, 64);
  auto a_ptr_void2 = dango::make_auto_ptr<void const, dango::polymorphic_allocator<>>(dango::default_mem_resource_ptr(), 64, 64);
  auto a_ptr_void3 = dango::make_auto_ptr<void volatile, dango::polymorphic_allocator<>>(64, 64);
  auto a_ptr_void4 = dango::auto_ptr{ 64, 64 };
  auto a_ptr_void5 = dango::auto_ptr{ dango::default_mem_resource_ptr(), 64, 64 };

  dango_assert_terminate(a_ptr_void2.get_allocator_handle() == a_ptr_void3.get_allocator_handle());
  dango_assert_terminate(a_ptr_void2.get_allocator_handle() == a_ptr_void5.get_allocator_handle());

  static_assert(sizeof(a_ptr_void1) == 3 * sizeof(void*));
  static_assert(sizeof(a_ptr_void2) == 4 * sizeof(void*));

  dango_assert_terminate(*a_ptr_int1 == 1);
  dango_assert_terminate(*a_ptr_int2 == 2);
  dango_assert_terminate(*a_ptr_int3 == 3);
  dango_assert_terminate(*a_ptr_int4 == 4);
  dango_assert_terminate(*a_ptr_int5 == 5);
  dango_assert_terminate(*a_ptr_int7 == 7);
  dango_assert_terminate(*a_ptr_int8 == 8);
  dango_assert_terminate(*a_ptr_int9 == 9);

  dango::auto_ptr a_array_ptr{ new int[3], dango::array_delete };
}
DANGO_UNIT_TEST_END

#ifdef DANGO_USING_CLANG
constexpr auto
auto_ptr_contstexpr_test(int const a_val)noexcept(false)->int
{
  dango::auto_ptr a_ptr{ new int{ a_val } };

  dango_assert(*a_ptr == a_val);

  auto a_ptr2{ dango::move(a_ptr) };

  dango_assert(a_ptr == null);
  dango_assert(*a_ptr2 == a_val);

  auto a_ptr3 = dango::make_auto_ptr<int, dango::polymorphic_allocator<>>(*a_ptr2);

  dango_assert(a_ptr2 != null);

  auto a_ptr4 = dango::make_auto_ptr<int const, dango::basic_allocator>(*a_ptr3);

  dango::mem_resource_ptr<> a_null = null;

  auto a_ptr5 = dango::make_auto_ptr<int, dango::polymorphic_allocator<>>(a_null, *a_ptr4);

  dango::auto_ptr a_array_ptr{ new int[3], dango::array_delete };

  return *a_ptr5;
}

static_assert(auto_ptr_contstexpr_test(42) == 42);
#endif

struct poly_base_nv
{

};

struct poly_derived_nv:poly_base_nv
{

};

static_assert( dango::is_brace_constructible<dango::auto_ptr<poly_base_nv>, poly_base_nv* const&>);
static_assert( dango::is_brace_constructible<dango::auto_ptr<poly_base_nv>, poly_base_nv*&&>);
static_assert(!dango::is_brace_constructible<dango::auto_ptr<poly_base_nv>, dango::auto_ptr<poly_base_nv> const&>);
static_assert( dango::is_brace_constructible<dango::auto_ptr<poly_base_nv>, dango::auto_ptr<poly_base_nv>&&>);
static_assert(!dango::is_assignable<dango::auto_ptr<poly_base_nv>&, dango::auto_ptr<poly_base_nv> const&>);
static_assert( dango::is_assignable<dango::auto_ptr<poly_base_nv>&, dango::auto_ptr<poly_base_nv>&&>);

static_assert(!dango::is_brace_constructible<dango::auto_ptr<poly_base_nv>, poly_derived_nv* const&>);
static_assert(!dango::is_brace_constructible<dango::auto_ptr<poly_base_nv>, poly_derived_nv*&&>);
static_assert(!dango::is_brace_constructible<dango::auto_ptr<poly_base_nv>, dango::auto_ptr<poly_derived_nv> const&>);
static_assert(!dango::is_brace_constructible<dango::auto_ptr<poly_base_nv>, dango::auto_ptr<poly_derived_nv>&&>);
static_assert(!dango::is_assignable<dango::auto_ptr<poly_base_nv>&, dango::auto_ptr<poly_derived_nv> const&>);
static_assert(!dango::is_assignable<dango::auto_ptr<poly_base_nv>&, dango::auto_ptr<poly_derived_nv>&&>);

struct poly_base
{
  virtual ~poly_base()noexcept
  {
    test_print("~poly_base()\n");
  }
};

struct poly_derived:poly_base
{
  ~poly_derived()noexcept override
  {
    test_print("~poly_derived()\n");
  }
};

static_assert( dango::is_brace_constructible<dango::auto_ptr<poly_base>, poly_base* const&>);
static_assert( dango::is_brace_constructible<dango::auto_ptr<poly_base>, poly_base*&&>);
static_assert(!dango::is_brace_constructible<dango::auto_ptr<poly_base>, dango::auto_ptr<poly_base> const&>);
static_assert( dango::is_brace_constructible<dango::auto_ptr<poly_base>, dango::auto_ptr<poly_base>&&>);
static_assert(!dango::is_assignable<dango::auto_ptr<poly_base>&, dango::auto_ptr<poly_base> const&>);
static_assert( dango::is_assignable<dango::auto_ptr<poly_base>&, dango::auto_ptr<poly_base>&&>);

static_assert( dango::is_brace_constructible<dango::auto_ptr<poly_base>, poly_derived* const&>);
static_assert( dango::is_brace_constructible<dango::auto_ptr<poly_base>, poly_derived*&&>);
static_assert(!dango::is_brace_constructible<dango::auto_ptr<poly_base>, dango::auto_ptr<poly_derived> const&>);
static_assert( dango::is_brace_constructible<dango::auto_ptr<poly_base>, dango::auto_ptr<poly_derived>&&>);
static_assert(!dango::is_assignable<dango::auto_ptr<poly_base>&, dango::auto_ptr<poly_derived> const&>);
static_assert( dango::is_assignable<dango::auto_ptr<poly_base>&, dango::auto_ptr<poly_derived>&&>);

static_assert( dango::is_brace_constructible<dango::auto_ptr<poly_base, dango::array_delete_type>, poly_base* const&>);
static_assert( dango::is_brace_constructible<dango::auto_ptr<poly_base, dango::array_delete_type>, poly_base*&&>);
static_assert(!dango::is_brace_constructible<dango::auto_ptr<poly_base, dango::array_delete_type>, dango::auto_ptr<poly_base, dango::array_delete_type> const&>);
static_assert( dango::is_brace_constructible<dango::auto_ptr<poly_base, dango::array_delete_type>, dango::auto_ptr<poly_base, dango::array_delete_type>&&>);
static_assert(!dango::is_assignable<dango::auto_ptr<poly_base, dango::array_delete_type>&, dango::auto_ptr<poly_base, dango::array_delete_type> const&>);
static_assert( dango::is_assignable<dango::auto_ptr<poly_base, dango::array_delete_type>&, dango::auto_ptr<poly_base, dango::array_delete_type>&&>);

static_assert(!dango::is_brace_constructible<dango::auto_ptr<poly_base, dango::array_delete_type>, poly_derived* const&>);
static_assert(!dango::is_brace_constructible<dango::auto_ptr<poly_base, dango::array_delete_type>, poly_derived*&&>);
static_assert(!dango::is_brace_constructible<dango::auto_ptr<poly_base, dango::array_delete_type>, dango::auto_ptr<poly_derived, dango::array_delete_type> const&>);
static_assert(!dango::is_brace_constructible<dango::auto_ptr<poly_base, dango::array_delete_type>, dango::auto_ptr<poly_derived, dango::array_delete_type>&&>);
static_assert(!dango::is_assignable<dango::auto_ptr<poly_base, dango::array_delete_type>&, dango::auto_ptr<poly_derived, dango::array_delete_type> const&>);
static_assert(!dango::is_assignable<dango::auto_ptr<poly_base, dango::array_delete_type>&, dango::auto_ptr<poly_derived, dango::array_delete_type>&&>);

DANGO_UNIT_TEST_BEGIN(auto_ptr_polymorphic_test)
{
  dango::auto_ptr<poly_base> a_ptr = null;

  a_ptr = dango::auto_ptr{ new poly_derived{ } };
}
DANGO_UNIT_TEST_END

}
