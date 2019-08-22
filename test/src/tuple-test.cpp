#include "dango.hpp"

namespace
{
  struct
  size_test_struct
  final
  {
    float m_0;
    dango::uint m_1;
    dango::ulong m_2;
    double m_3;
  };

  using size_test_tuple = dango::tuple<float, dango::uint, dango::ulong, double>;
}

static_assert(sizeof(size_test_struct) == sizeof(size_test_tuple));
static_assert(alignof(size_test_struct) == alignof(size_test_tuple));

