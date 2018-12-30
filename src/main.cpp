#include "dango-def.hpp"
#include "dango-int.hpp"
#include "dango-macro.hpp"

void func()noexcept;

template
<
  typename... tp_bool,
  dango::enable_if<(... && dango::is_same<dango::remove_cv<tp_bool>, bool>)> = dango::enable_val
>
constexpr auto
conjunction
(tp_bool const... args)noexcept->bool
{
  return (... && args);
}

static_assert(conjunction(true, true, true));

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

