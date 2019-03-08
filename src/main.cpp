#include "dango.hpp"

#include <cstdio>
#include <tuple>
#include <string>

auto
main
()noexcept(false)->dango::s_int
{
  {
    dango::tuple<int, int, int> a_tup{ };

    dango::tuple<int const, int const, int const> a_tup2{ 1, 2, 3 };

    a_tup = a_tup2;

    a_tup->*[](int const a, int const b, int const c)noexcept->void{ printf("{ %i, %i, %i }\n", a, b, c); };
  }

  /*dango::thread a_threads[10];

  dango::mutex a_mutex{ };

  auto a_id = dango::uint32(0);

  for(auto& a_thread:a_threads)
  {
    a_thread =
    dango::thread::create
    (
      [a_id, &a_mutex]()noexcept->void
      {
        auto a_tick = dango::get_tick_count();
        auto a_tick_sa = dango::get_tick_count_sa();

        fprintf
        (
          stderr,
          "thread[%u] sleep_hr... (%llu) (%llu)\n",
          dango::u_int(a_id),
          dango::u_cent(a_tick),
          dango::u_cent(a_tick_sa)
        );

        dango::thread::sleep_hr(30'000);

        a_tick = dango::get_tick_count();
        a_tick_sa = dango::get_tick_count_sa();

        fprintf
        (
          stderr,
          "thread[%u] sleep_hr_sa... (%llu) (%llu)\n",
          dango::u_int(a_id),
          dango::u_cent(a_tick),
          dango::u_cent(a_tick_sa)
        );

        dango::thread::sleep_hr_sa(30'000);

        a_tick = dango::get_tick_count();
        a_tick_sa = dango::get_tick_count_sa();

        fprintf
        (
          stderr,
          "thread[%u] serial sleep... (%llu) (%llu)\n",
          dango::u_int(a_id),
          dango::u_cent(a_tick),
          dango::u_cent(a_tick_sa)
        );

        dango_crit(a_mutex)
        {
          dango::thread::sleep(5'000);
        }

        a_tick = dango::get_tick_count();
        a_tick_sa = dango::get_tick_count_sa();

        fprintf
        (
          stderr,
          "thread[%u] done (%llu) (%llu)\n",
          dango::u_int(a_id),
          dango::u_cent(a_tick),
          dango::u_cent(a_tick_sa)
        );
      }
    );

    dango::invoker<decltype(&dango::thread::is_alive)> a_inv{ &dango::thread::is_alive };

    static_assert(dango::is_noexcept_callable_ret<bool, decltype(a_inv), dango::thread const&>);
    static_assert(dango::is_noexcept_callable_ret<bool, decltype(a_inv), dango::thread const*>);

    dango_assert(a_inv(a_thread));

    ++a_id;
  }

  for(auto const& a_thread:a_threads)
  {
    a_thread.join();

    dango::invoker<decltype(&dango::thread::is_alive)> a_inv{ &dango::thread::is_alive };

    dango_assert(!a_inv(&a_thread));
  }*/

  return 0;
}

auto
test_func
(dango::tuple<dango::uint32, dango::uint32, dango::uint64>& a_tup)noexcept->dango::uint32
{
  auto a_copy = dango::move(a_tup);

  a_copy = a_copy;

  a_copy.get<0>() += a_copy.get<1>();
  a_copy.get<1>() += a_tup.get<0>();
  a_tup.get<2>() = a_copy.get<1>();

  return a_tup.get<0>();
}

