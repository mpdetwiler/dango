#include "dango-global-test.hpp"
#include "dango-test-print.hpp"

test::
global_printer::
global_printer
()noexcept
{
  test_print("global_printer::global_printer()\n");
}

test::
global_printer::
~global_printer
()noexcept
{
  test_print("global_printer::~global_printer()\n");
}

void
test::
global_printer::
print
(char const* const a_str)const noexcept
{
  test_print("global_printer::print(): %s\n", a_str);
}

namespace
global_test
{

DANGO_UNIT_TEST_BEGIN(global_access_test)
{
  dango_access_global(test::s_global_printer, a_printer)
  {
    a_printer.print("global_access_test");
  }
}
DANGO_UNIT_TEST_END

#ifndef DANGO_NO_DEBUG
DANGO_UNIT_TEST_BEGIN(inline_global_address_test)
{
  auto const a_extern = dango::detail::test::inline_global_address_extern();
  auto const a_inline = dango::detail::test::inline_global_address_inline();

  test_print("inline_global_address_extern: %p\n", a_extern);
  test_print("inline_global_address_inline: %p\n", a_inline);

#ifndef DANGO_PLATFORM_WINDOWS
  dango_assert_terminate(a_extern == a_inline);
#endif
}
DANGO_UNIT_TEST_END
#endif

}

namespace
{
  struct incomplete;
}

namespace ns
{
  DANGO_DECLARE_NAMESPACE_SCOPE_THREAD_LOCAL_EXTERN(incomplete, t_incomplete)

  DANGO_DEFINE_NAMESPACE_SCOPE_THREAD_LOCAL_INLINE(float, t_float, { 1.0f })

  struct
  tls_class_test
  {
    DANGO_DEFINE_CLASS_SCOPE_THREAD_LOCAL_INLINE(double, t_double, { 4.0 })
  };
}

namespace
{
  struct incomplete
  {
    DANGO_TAG_TYPE(incomplete)
  };
}

namespace
global_test
{

DANGO_UNIT_TEST_BEGIN(thread_local_test)
{
  DANGO_DEFINE_FUNC_SCOPE_THREAD_LOCAL(dango::ulong, t_integer, { 10 })

  dango_assert_nonnull_terminate(t_integer_try_access());

  test_print("%u\n", dango::integer::u_int(t_integer()));

  t_integer() = 5;

  dango_assert_nonnull_terminate(ns::t_incomplete_try_access());

  incomplete a_incomplete{ };

  ns::t_incomplete() = a_incomplete;

  dango_assert_nonnull_terminate(ns::t_float_try_access());

  ns::t_float() = 2.0f;

  dango_assert_nonnull_terminate(ns::tls_class_test::t_double_try_access());

  ns::tls_class_test::t_double() = 7.0;
}
DANGO_UNIT_TEST_END

}

namespace ns
{
  DANGO_DEFINE_NAMESPACE_SCOPE_THREAD_LOCAL_EXTERN(t_incomplete, { })
}
