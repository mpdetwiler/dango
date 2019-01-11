#include "dango.hpp"

#include <cstdio>

struct
accessor
{
  accessor()noexcept;
  ~accessor()noexcept;
  DANGO_IMMOBILE(accessor)
};

struct
printer
{
  printer()noexcept;
  ~printer()noexcept;

  void print(char const* const a_str)const noexcept
  {
    printf("%p: %s\n", static_cast<void const*>(this), a_str);
  }

  DANGO_IMMOBILE(printer)
};


DANGO_DEFINE_GLOBAL_INLINE(s_accessor, accessor const, )

namespace
test
{
  DANGO_DEFINE_GLOBAL_INLINE(s_printer, printer const, )
}

printer::
printer
()noexcept
{
  printf("printer constructor\n");
}

printer::
~printer
()noexcept
{
  printf("printer destructor\n");
}

accessor::
accessor
()noexcept
{
  printf("accessor constructor\n");
  printf("accessing...\n");

  dango_access_global(test::s_printer, a_printer)
  {
    a_printer->print("hello");
  }

  printf("accessed\n");
}

accessor::
~accessor
()noexcept
{
  printf("accessor destructor\n");
  printf("accessing...\n");

  dango_access_global(test::s_printer, a_printer)
  {
    a_printer->print("hey");
  }

  printf("accessed\n");
}

auto
main
()noexcept->dango::s_int
{
  dango_access_global(test::s_printer, a_printer)
  {
    a_printer->print("hi");
  }

  test::s_printer_fast.print("fast");

  return 0;
}

