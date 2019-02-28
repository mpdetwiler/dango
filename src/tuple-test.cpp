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

static_assert(dango::is_noexcept_default_constructible<dango::tuple<immobile>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<immobile, immobile>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<immobile, immobile, immobile>>);

static_assert(dango::is_noexcept_constructible<dango::tuple<immobile>, dango::value_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<immobile, immobile>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<immobile, immobile, immobile>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<immobile>, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<immobile, immobile>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<immobile, immobile, immobile>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<immobile, immobile>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<immobile, immobile, immobile>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_destructible<dango::tuple<immobile>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<immobile, immobile>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<immobile, immobile, immobile>>);

static_assert(!dango::is_constructible<dango::tuple<immobile, immobile, immobile>, dango::skip_init_tag>);

static_assert(!dango::is_copy_constructible<dango::tuple<immobile>>);
static_assert(!dango::is_copy_constructible<dango::tuple<immobile, immobile>>);
static_assert(!dango::is_copy_constructible<dango::tuple<immobile, immobile, immobile>>);

static_assert(!dango::is_move_constructible<dango::tuple<immobile>>);
static_assert(!dango::is_move_constructible<dango::tuple<immobile, immobile>>);
static_assert(!dango::is_move_constructible<dango::tuple<immobile, immobile, immobile>>);

static_assert(!dango::is_copy_assignable<dango::tuple<immobile>>);
static_assert(!dango::is_copy_assignable<dango::tuple<immobile, immobile>>);
static_assert(!dango::is_copy_assignable<dango::tuple<immobile, immobile, immobile>>);

static_assert(!dango::is_move_assignable<dango::tuple<immobile>>);
static_assert(!dango::is_move_assignable<dango::tuple<immobile, immobile>>);
static_assert(!dango::is_move_assignable<dango::tuple<immobile, immobile, immobile>>);

