#include "dango-concurrent.hpp"

/*** tick_count ***/

namespace
{
  auto tick_count_suspend_bias_help()noexcept->dango::tick_count_pair;
}

auto
dango::
current_tick_and_suspend_bias
()noexcept->dango::tick_count_pair
{
  static auto const s_init = tick_count_suspend_bias_help();
  static thread_local auto t_last = s_init;

  auto a_current = tick_count_suspend_bias_help();
  auto& [a_tick, a_bias] = a_current;

  a_tick = dango::max(a_tick, t_last.first());
  a_bias = dango::max(a_bias, t_last.second());
  t_last = a_current;

  a_tick -= s_init.first();
  a_bias -= s_init.second();

  return a_current;
}

#include "dango-concurrent-private.hpp"

#ifdef DANGO_PLATFORM_LINUX_OR_APPLE

/*** tick_count_suspend_bias_help ***/

namespace
{
  auto
  tick_count_suspend_bias_help
  ()noexcept->dango::tick_count_pair
  {
    using dango::tick::to_milli;
    using dango::tick::from_milli;
    using dango::tick::zero;
    using tc64 = dango::tick_count_type;

    tc64 a_mono;
    tc64 a_boot;

    {
      tc64 a_temp;

      do
      {
        a_temp = dango::detail::tick_count_boottime();

        a_mono = dango::detail::tick_count_monotonic();

        a_boot = dango::detail::tick_count_boottime();
      }
      while(to_milli(a_boot - a_temp) != dango::slong(0));
    }

    auto const a_bias = to_milli(dango::max(zero, a_boot - a_mono));

    return dango::tick_count_pair{ a_mono , from_milli(a_bias) };
  }
}

#endif // DANGO_PLATFORM_LINUX_OR_APPLE

#ifdef DANGO_PLATFORM_WINDOWS

namespace
{
  auto
  tick_count_suspend_bias_help
  ()noexcept->dango::tick_count_pair
  {
    auto a_current = dango::detail::perf_count_suspend_bias();
    auto& [a_tick, a_bias] = a_current;

    dango_assert(a_tick >= a_bias);

    a_tick -= a_bias;

    return a_current;
  }
}

#endif // DANGO_PLATFORM_WINDOWS
