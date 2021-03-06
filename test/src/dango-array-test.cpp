#include "dango-array.hpp"
#include "dango-container.hpp"
#include "dango-fixed-array.hpp"
#include "dango-test-print.hpp"

template class dango::array<float, 4>;
template class dango::array<float, 0>;

static_assert(dango::array{ "hello" } == dango::array{ "hello" });
static_assert(dango::array{ "hello" } != dango::array{ "hi" });
static_assert(dango::array{ "hello" } != dango::array{ "hella" });
static_assert((dango::array{ "hello" } <=> dango::array{ "hello" }).is_eq());
static_assert((dango::array{ "hello" } <=> dango::array{ "hella" }).is_gt());
static_assert((dango::array{ "hello" } <=> dango::array{ "hellu" }).is_lt());
static_assert(dango::tuple_get<4>(dango::array{ "hello" }) == 'o');
static_assert(dango::array{ 1, 2, 3 } == dango::array{ 1, 2, 3 });
static_assert(dango::array{ 1, 2, 3 } < dango::array{ 1, 2, 4 });
static_assert(dango::array{ 1, 2, 3LL, 4U } < dango::array{ 1, 2, 4 });
static_assert(dango::array{ "hello" }.size() == 6);
static_assert(sizeof(decltype(dango::array{ "hello" })) == 6);

static_assert( dango::tuple_size<dango::array<int, 0>> == 0);
static_assert(!dango::has_tuple_elem<dango::array<int, 0>, 0>);
static_assert(!dango::has_tuple_get<dango::array<int, 0>, 0>);
static_assert( dango::is_tuple_like<dango::array<int, 0>>);

static_assert(dango::tuple_size<dango::array<int, 4>> == 4);
static_assert(dango::has_tuple_elem<dango::array<int, 4>, 0>);
static_assert(dango::has_tuple_get<dango::array<int, 4>, 0>);
static_assert(dango::is_tuple_like<dango::array<int, 4>>);
static_assert(dango::is_same<dango::tuple_elem<0, dango::array<int, 4>>, int>);
static_assert(dango::is_same<dango::tuple_elem<0, dango::array<int, 4> const>, int const>);
static_assert(dango::is_same<dango::tuple_elem<0, dango::array<int volatile, 4> const>, int const volatile>);
static_assert(dango::is_same<typename std::tuple_element<0, dango::array<int, 4> const>::type, int const>);

namespace
{

DANGO_UNIT_TEST_BEGIN(array_test1)
{
  for(auto& a_val : dango::array{ "hello" })
  {
    test_print("%c", a_val);
  }

  test_print("\n");

  for(auto& a_val : dango::array<char, 0>{ })
  {
    test_print("%c", a_val);
  }

  dango::array<char, 6> a_array{ "hello" };

  dango::swap(a_array, a_array);

  auto const& [a, b, c, d, e, f] = a_array;

  dango_assert_terminate(a == 'h');
  dango_assert_terminate(b == 'e');
  dango_assert_terminate(c == 'l');
  dango_assert_terminate(d == 'l');
  dango_assert_terminate(e == 'o');
  dango_assert_terminate(f == '\0');
}
DANGO_UNIT_TEST_END

static_assert(dango::fixed_array<float, dango::basic_allocator>{ }.size() == 0);
static_assert(dango::fixed_array<float, dango::basic_allocator>{ }.is_empty());
static_assert(dango::fixed_array<float, dango::polymorphic_allocator<>>{ }.size() == 0);
static_assert(dango::fixed_array<float, dango::polymorphic_allocator<>>{ }.is_empty());

DANGO_UNIT_TEST_BEGIN(fixed_array_test1)
{
  dango::fixed_array<float, dango::basic_allocator> a_test1{ };

  test_print("size: %u\n", dango::integer::u_int(a_test1.size()));

  dango::fixed_array<float, dango::polymorphic_allocator<>> a_test2{ };

  test_print("size: %u\n", dango::integer::u_int(a_test2.size()));
}
DANGO_UNIT_TEST_END

struct
node
{
  float m_data;
  dango::fixed_array<node> m_children;
};

static_assert(dango::is_noexcept_destructible<node>);
static_assert(dango::is_noexcept_move_constructible<node>);
static_assert(dango::is_noexcept_move_assignable<node>);
static_assert(!dango::is_copy_constructible<node>);
static_assert(!dango::is_copy_assignable<node>);

DANGO_UNIT_TEST_BEGIN(fixed_array_test2)
{
  node a_node1;
  node a_node2{ 1.0f, { } };
  node a_node3{ 1.0f, null };
}
DANGO_UNIT_TEST_END

struct nondefault
{
  explicit constexpr nondefault()noexcept = delete;
};

[[maybe_unused]]
constexpr auto
nondefault_test(int const a_ret)noexcept->int
{
  dango::fixed_array<nondefault> a_array{ };

  return a_ret;
}

// uncommenting the following line should cause a compilation error:
// static_assert(nondefault_test(5) == 5);

// the runtime version should compile:

DANGO_UNIT_TEST_BEGIN(fixed_array_test3)
{
  dango_assert(nondefault_test(5) == 5);
}
DANGO_UNIT_TEST_END

DANGO_UNIT_TEST_BEGIN(fixed_array_test4)
{
  using array_type = dango::fixed_array<float, dango::basic_allocator>;
  using array_allocator = array_type::array_allocator;

  array_allocator::allocate_init(dango::forward_as_tuple(dango::forward_as_tuple(1.0f)), dango::usize(1));
}
DANGO_UNIT_TEST_END


}
