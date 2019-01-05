#include "dango-def.hpp"
#include "dango-int.hpp"
#include "dango-macro.hpp"
#include "dango-traits.hpp"
#include "dango-util.hpp"
#include "dango-assert.hpp"
#include "dango-atomic.hpp"

void func()noexcept;

template
<
  typename... tp_bool,
  dango::enable_if<(true && ... && dango::is_same<dango::remove_cv<tp_bool>, bool>)> = dango::enable_val
>
constexpr auto
conjunction
(tp_bool const... args)noexcept->bool
{
  return (true && ... && args);
}

static_assert(conjunction(true, true, true, true));

#include <cstdio>

auto
main
()noexcept->dango::s_int
{
  printf("%lli\n", dango::s_cent(dango::integer::MIN_VAL<sint8>));
  printf("%lli\n", dango::s_cent(dango::integer::MIN_VAL<sint16>));
  printf("%lli\n", dango::s_cent(dango::integer::MIN_VAL<sint32>));
  printf("%lli\n", dango::s_cent(dango::integer::MIN_VAL<sint64>));

  printf
  (
    "%s %s %u\n",
    dango::source_location::current().file(),
    dango::source_location::current().function(),
    dango::source_location::current().line()
  );

  func();

  dango::atomic<int> a_test{ 5 };

  a_test.load<dango::mem_order::acquire>();
  a_test.sub_fetch(4);

  int a_x = 5;

  dango::atomic<int const*> a_ax{ &a_x };

  dango::atomic<void(*)()noexcept> a_afunc{ &func };

  a_ax.fetch_add<dango::mem_order::acq_rel>(5);

  return 0;
}

template class dango::atomic<int(*)(int)noexcept>;

