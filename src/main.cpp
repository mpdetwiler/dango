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

  constexpr auto dango_operator_is_null()const noexcept->bool{ return false; }
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

  dango_assert(dango::thread::self().is_alive());

  auto const a_thread =
    dango::thread::create([]()noexcept(false)->void{ fprintf(stderr, "thread print\n"); });

  dango_assert(a_thread != null);
  dango_assert(a_thread != dango::thread::self());

  a_thread.join();

  dango_assert(!a_thread.is_alive());

  dango_assert(a_thread == a_thread);

  auto const a_deadline = dango::make_deadline_rel(dango::uint64(1'000));

  printf
  (
    "tick count: %llu\n",
    dango::u_cent(dango::get_tick_count() / 1000)
  );

  printf
  (
    "tick count (suspend-aware): %llu\n",
    dango::u_cent(dango::get_tick_count(dango::suspend_aware) / 1000)
  );

  dango_crit_full(a_cond, a_crit)
  {
    while(!a_deadline.has_passed())
    {
      a_crit.wait(a_deadline);

      printf("wake\n");
    }
  }

  printf
  (
    "tick count: %llu\n",
    dango::u_cent(dango::get_tick_count() / 1000)
  );

  printf
  (
    "tick count (suspend-aware): %llu\n",
    dango::u_cent(dango::get_tick_count(dango::suspend_aware) / 1000)
  );

  dango::thread::sleep(5'00);

  for(auto a_i = uint32(0); a_i < uint32(100); ++a_i)
  {
    printf("tick count: %llu\n", dango::u_cent(dango::get_tick_count()));

    dango::thread::sleep(1);
  }

  return 0;
}

