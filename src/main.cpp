#include "dango-def.hpp"
#include "dango-int.hpp"
#include "dango-macro.hpp"
#include "dango-traits.hpp"
#include "dango-util.hpp"
#include "dango-assert.hpp"

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

  func();

  return 0;
}

