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


static_assert(dango::is_same<decltype(dango::tuple<int, float>{ } <=> dango::tuple<int, float, dango::bchar>{ }), dango::compare_val_partial>);
static_assert(dango::is_same<decltype(dango::tuple<int, dango::wchar>{ } <=> dango::tuple<int, dango::wchar, dango::bchar>{ }), dango::compare_val>);

static_assert(dango::tuple<char8_t, char8_t, char8_t>{ u8'a', u8'b', u8'c' } < dango::tuple<char8_t, char8_t>{ u8'a', u8'c' });
static_assert(dango::tuple<char8_t, char8_t>{ u8'a', u8'c' } > dango::tuple<char8_t, char8_t, char8_t>{ u8'a', u8'b', u8'c' });
