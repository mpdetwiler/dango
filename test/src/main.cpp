#include "dango.hpp"

#include <cstdio>
#include <string>
#include <memory>

static_assert(dango::is_nullable<dango::null_tag>);
static_assert(dango::is_nullable<dango::thread>);

auto
main
()noexcept(false)->dango::s_int
{
  auto const a_guard = dango::thread::main_join_finally();

  {
    auto const a_tup = dango::tuple<int, int, char const[6]>{ 1, 2, "hello" };

    auto& [a, b, c] = a_tup;

    static_assert(dango::is_same<decltype(a), int const>);
    static_assert(dango::is_same<decltype(b), int const>);
    static_assert(dango::is_same<decltype(c), char const(&)[6]>);

    dango_assert(a == 1);
    dango_assert(b == 2);

    //a = 4;

    //dango_assert(a_tup.first() == 4);

    int array[5];

    int const* x = &array[0];

    dango::atomic_fetch_add(&x, 4);

    dango_assert(x == &array[4]);
  }

  {
    int a_arr[] = { 1, 2, 3 };

    dango::tuple<int&&, int&, int, std::unique_ptr<double>> a_tup{ dango::move(a_arr[0]), a_arr[1], a_arr[2], nullptr };

    dango::tuple<int&&, int, int&&, std::unique_ptr<double>> a_tup2{ dango::move(a_tup) };

    a_tup2->*[](int const a, int const b, int const c, auto const&)noexcept->void{ printf("{ %i, %i, %i }\n", a, b, c); };
  }

  dango_assert(dango::thread::self().is_daemon());

  dango::thread a_threads[10];

  dango::mutex a_mutex{ };

  auto a_count = dango::uint32(0);

  for(auto& a_thread:a_threads)
  {
    a_thread =
    dango::thread::create
    (
      [&a_mutex](dango::uint32 const a_id)noexcept->void
      {
        dango_assert(!dango::thread::self().is_daemon());

        auto a_tick = dango::get_tick_count();
        auto a_tick_sa = dango::get_tick_count_sa();

        fprintf
        (
          stderr,
          "thread[%u] sleep_rel_hr... (%llu) (%llu)\n",
          dango::u_int(a_id),
          dango::u_cent(a_tick),
          dango::u_cent(a_tick_sa)
        );

        dango::thread::sleep_rel_hr(30'000);

        a_tick = dango::get_tick_count();
        a_tick_sa = dango::get_tick_count_sa();

        fprintf
        (
          stderr,
          "thread[%u] sleep_rel_hr_sa... (%llu) (%llu)\n",
          dango::u_int(a_id),
          dango::u_cent(a_tick),
          dango::u_cent(a_tick_sa)
        );

        dango::thread::sleep_rel_hr_sa(30'000);

        a_tick = dango::get_tick_count();
        a_tick_sa = dango::get_tick_count_sa();

        fprintf
        (
          stderr,
          "thread[%u] serial sleep_rel... (%llu) (%llu)\n",
          dango::u_int(a_id),
          dango::u_cent(a_tick),
          dango::u_cent(a_tick_sa)
        );

        dango_crit(a_mutex)
        {
          dango::thread::sleep_rel(5'000);
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
      },
      a_count
    );

    dango_assert(!a_thread.is_daemon());

    ++a_count;
  }

  /*for(auto const& a_thread:a_threads)
  {
    a_thread.join();

    dango::invoker<decltype(&dango::thread::is_alive)> a_inv{ &dango::thread::is_alive };

    dango_assert(!a_inv(&a_thread));
  }*/

  return 0;
}

