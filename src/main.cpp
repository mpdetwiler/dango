#include "dango.hpp"

#include <cstdio>
#include <tuple>
#include <string>
#include <memory>

thread_local char t_string[] = "abcdefghijklmnopqrstuvwxyzABCEDFGHIJKLMNOPQRSTUVWXYZ0123456789";
thread_local char t_string2[] = "12345";

struct
string_accessor
final
{
  string_accessor()noexcept
  {
    printf("ctor: string=%p\n", (void const*)(&t_string[0]));
    printf("ctor: %s\n", t_string);
  }

  ~string_accessor()noexcept
  {
    dango::thread::create_daemon([]()noexcept->void{ printf("dtor thread\n"); }).join();

    printf("dtor: string=%p string2=%p\n", (void const*)(&t_string[0]), (void const*)(&t_string2[0]));

    printf("dtor string2: %s\n", t_string2);

    t_string2[0] = 'X';
    t_string2[1] = 'Y';
    t_string2[2] = 'Z';

    printf("dtor string2: %s\n", t_string2);

    printf("dtor: %s\n", t_string);

    t_string2[20] = '$';

    printf("dtor: %s\n", t_string);
  }
};


static string_accessor const s_access{ };

auto
main
()noexcept(false)->dango::s_int
{
  auto const a_guard = dango::thread::main_join_finally();

  /*{
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
      [&a_mutex](dango::uint32 const a_id, dango::uint64 const a_start_tick)noexcept->void
      {
        dango_assert(!dango::thread::self().is_daemon());

        fprintf
        (
          stderr,
          "thread[%u] start tick: %llu\n",
          dango::u_int(a_id),
          dango::u_cent(a_start_tick)
        );

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
      },
      a_count,
      dango::get_tick_count()
    );

    dango_assert(!a_thread.is_daemon());

    ++a_count;
  }*/

  /*for(auto const& a_thread:a_threads)
  {
    a_thread.join();

    dango::invoker<decltype(&dango::thread::is_alive)> a_inv{ &dango::thread::is_alive };

    dango_assert(!a_inv(&a_thread));
  }*/

  return 0;
}

