#include "dango-util-base.hpp"
#include "dango-assert.hpp"
#include "dango-test-print.hpp"

static_assert( dango::is_convertible_arg<dango::compare_val_s, std::strong_ordering>);
static_assert( dango::is_convertible_arg<dango::compare_val_s, std::weak_ordering>);
static_assert( dango::is_convertible_arg<dango::compare_val_s, std::partial_ordering>);
static_assert( dango::is_convertible_arg<dango::compare_val_s, dango::compare_val_s>);
static_assert( dango::is_convertible_arg<dango::compare_val_s, dango::compare_val_w>);
static_assert( dango::is_convertible_arg<dango::compare_val_s, dango::compare_val_p>);

static_assert(!dango::is_convertible_arg<dango::compare_val_w, std::strong_ordering>);
static_assert( dango::is_convertible_arg<dango::compare_val_w, std::weak_ordering>);
static_assert( dango::is_convertible_arg<dango::compare_val_w, std::partial_ordering>);
static_assert(!dango::is_convertible_arg<dango::compare_val_w, dango::compare_val_s>);
static_assert( dango::is_convertible_arg<dango::compare_val_w, dango::compare_val_w>);
static_assert( dango::is_convertible_arg<dango::compare_val_w, dango::compare_val_p>);

static_assert(!dango::is_convertible_arg<dango::compare_val_p, std::strong_ordering>);
static_assert(!dango::is_convertible_arg<dango::compare_val_p, std::weak_ordering>);
static_assert( dango::is_convertible_arg<dango::compare_val_p, std::partial_ordering>);
static_assert(!dango::is_convertible_arg<dango::compare_val_p, dango::compare_val_s>);
static_assert(!dango::is_convertible_arg<dango::compare_val_p, dango::compare_val_w>);
static_assert( dango::is_convertible_arg<dango::compare_val_p, dango::compare_val_p>);

static_assert( dango::is_convertible_arg<std::strong_ordering, dango::compare_val_s>);
static_assert( dango::is_convertible_arg<std::strong_ordering, dango::compare_val_w>);
static_assert( dango::is_convertible_arg<std::strong_ordering, dango::compare_val_p>);

static_assert(!dango::is_convertible_arg<std::weak_ordering, dango::compare_val_s>);
static_assert( dango::is_convertible_arg<std::weak_ordering, dango::compare_val_w>);
static_assert( dango::is_convertible_arg<std::weak_ordering, dango::compare_val_p>);

static_assert(!dango::is_convertible_arg<std::partial_ordering, dango::compare_val_s>);
static_assert(!dango::is_convertible_arg<std::partial_ordering, dango::compare_val_w>);
static_assert( dango::is_convertible_arg<std::partial_ordering, dango::compare_val_p>);


static_assert(dango::equals("hello", "hello"));
static_assert(!dango::equals("hello", "hi"));
static_assert(dango::compare("hello", "hello") == 0);
static_assert(dango::compare("hello", "hi") < 0);
static_assert(dango::compare("hey", "hello") > 0);
static_assert(dango::min(16.0f) == 16.0f);
static_assert(dango::max(16.0f) == 16.0f);
static_assert(dango::min(16.0, 32.0) == 16.0);
static_assert(dango::max(16.0, 32.0) == 32.0);
static_assert(dango::min(5, 4, 3, 2, 1, 1, 1, 2, 3, 4, 5) == 1);
static_assert(dango::max(1, 2, 3, 4, 5, 5, 5, 4, 3, 2, 1) == 5);

static_assert(dango::tuple_get<0>("hello") == 'h');
static_assert(dango::tuple_get<1>("hello") == 'e');
static_assert(dango::tuple_get<2>("hello") == 'l');
static_assert(dango::tuple_get<3>("hello") == 'l');
static_assert(dango::tuple_get<4>("hello") == 'o');
static_assert(dango::tuple_get<5>("hello") == '\0');

static_assert(dango::tuple_size<int[4]> == 4);
static_assert(dango::is_same<dango::tuple_elem<0, int[4]>, int>);
static_assert(dango::is_same<dango::tuple_elem<1, int const[4]>, int const>);
static_assert(dango::is_tuple_like<int[4]>);

namespace ns
{
  struct
  ranged_for_test
  {
    dango::uint data[4];
  };
}

namespace ns
{
  DANGO_USING_RANGE_BASED_FOR_OPERATORS
}

namespace
dango::custom
{
  template<>
  struct
  operator_iter<ns::ranged_for_test>
  final
  {
    static constexpr auto
    begin(auto& a_arg)noexcept->auto
    {
      return dango::iter_begin(a_arg.data);
    }

    static constexpr auto
    end(auto& a_arg)noexcept->auto
    {
      return dango::iter_end(a_arg.data);
    }
  };
}

static_assert(dango::is_iterable<ns::ranged_for_test&>);
static_assert(dango::is_iterable<ns::ranged_for_test const&>);
static_assert(dango::is_noexcept_iterable<ns::ranged_for_test&>);
static_assert(dango::is_noexcept_iterable<ns::ranged_for_test const&>);

namespace
{

DANGO_UNIT_TEST_BEGIN(util_ranged_for_test)
{
  ns::ranged_for_test const a_test{ { 1, 2, 3, 4 } };
  ns::ranged_for_test a_test2{ { 4, 3, 2, 1 } };

  for(auto& a_val : a_test)
  {
    test_print("%u\n", dango::integer::u_int(a_val));
  }

  for(auto& a_val : a_test2)
  {
    test_print("%u\n", dango::integer::u_int(a_val));
  }
}
DANGO_UNIT_TEST_END

}
