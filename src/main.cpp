#include "dango.hpp"

#include <cstdio>
#include <tuple>

struct
immobile
final
{
public:
  constexpr immobile()noexcept:m_int{ 42 }{ }
  constexpr explicit immobile(int const a_int)noexcept(false):m_int{ a_int }{ if(a_int != 42){ throw a_int; } }
  ~immobile()noexcept = default;
private:
  int const m_int;
public:
  DANGO_IMMOBILE(immobile)
};

struct
thing
{
  int m_int;
  float m_float;
  double m_double;
};

#include<memory>

auto
main
()noexcept(false)->dango::s_int
{
  {
    dango::tuple<int, int> a_tup{ };

    auto a_copy = a_tup;
  }

  /*try
  {
    dango::tuple<int&&, immobile, int, float> const a_tup{ 5, 123, dango::skip_init, dango::value_init };

    static_assert(dango::is_same<decltype(a_tup.get<0>()), int&&>);
    static_assert(dango::is_same<decltype(a_tup.get<1>()), immobile const&>);
  }
  catch(int const& a_int)
  {
    printf("caught %i\n", a_int);
  }*/

  try
  {
    using ptr_type = std::unique_ptr<float>;

    dango::tuple<ptr_type, ptr_type> a_tup{ nullptr, nullptr };

    auto a_copy{ dango::move(a_tup) };

    dango::tuple<ptr_type const, ptr_type const> a_copy2( dango::move(a_tup) );

    dango::tuple<ptr_type> a_tup2{ nullptr };

    dango::tuple<dango::tuple<ptr_type const>> { dango::move(a_tup2) };
  }
  catch(int const& a_int)
  {
    printf("caught %i\n", a_int);
  }

  /*{
    dango::tuple<dango::tuple<int&&>> a_tup{ 42 };

    dango::tuple<dango::tuple<int>> a_copy{ a_tup };
  }*/

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

