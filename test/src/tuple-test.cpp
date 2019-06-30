#include "dango.hpp"

namespace
{
  struct
  size_test_struct
  final
  {
    float m_0;
    dango::uint32 m_1;
    dango::uint64 m_2;
    double m_3;
  };

  using size_test_tuple = dango::tuple<float, dango::uint32, dango::uint64, double>;
}

static_assert(sizeof(size_test_struct) == sizeof(size_test_tuple));
static_assert(alignof(size_test_struct) == alignof(size_test_tuple));

namespace
{
  struct
  immobile
  final
  {
    constexpr immobile()noexcept = default;
    ~immobile()noexcept = default;
    DANGO_IMMOBILE(immobile)
  };
}

