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


DANGO_DEFINE_GLOBAL_INLINE_CV(s_accessor, const, accessor{ })

namespace
test
{
  DANGO_DEFINE_GLOBAL_INLINE_CV(s_printer, const, printer{ })
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

#include <tuple>

DANGO_DEFINE_GLOBAL_INLINE(s_x, std::make_tuple(1, true, 2u, 3.5f))

auto
main
()noexcept->dango::s_int
{
  auto const a_finally =
  dango::make_finally
  (
    []()noexcept->void
    {
      printf("finally\n");
    }
  );

  dango_access_global(test::s_printer, a_printer)
  {
    a_printer->print("hi");
  }

  test::s_printer_fast.print("fast");

  dango_access_global(s_x, a_x)
  {
    std::get<0>(*a_x) = 5;
  }

  dango::mutex a_lock{ };

  dango::cond_var_mutex a_cond{ a_lock };

  dango_try_crit_full(a_cond, a_crit)
  {
    a_crit.notify();
  }

  dango_assert(dango::thread::self().is_running());

  auto const a_thread =
    dango::thread::create([]()noexcept(false)->void{ printf("thread print\n"); });

  dango_assert(a_thread != null);

  a_thread.join();

  dango_assert(!a_thread.is_running());

  dango_assert(a_thread == a_thread);

  return 0;
}

