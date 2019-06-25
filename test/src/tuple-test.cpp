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

#define DANGO_TEST_TYPE immobile

static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag>);

static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

#undef DANGO_TEST_TYPE

namespace
{
  struct
  immobile_throwing
  final
  {
    immobile_throwing()noexcept(false){ throw 0; }
    ~immobile_throwing()noexcept = default;
    DANGO_IMMOBILE(immobile_throwing)
  };
}

#define DANGO_TEST_TYPE immobile_throwing

static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(!dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(!dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(!dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(!dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(!dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(!dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(!dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(!dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag>);

static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

#undef DANGO_TEST_TYPE

namespace
{
  struct
  move_construct_only
  final
  {
    constexpr move_construct_only()noexcept = default;
    ~move_construct_only()noexcept = default;
    constexpr move_construct_only(move_construct_only const&)noexcept = delete;
    constexpr move_construct_only(move_construct_only&&)noexcept = default;
    constexpr auto operator = (move_construct_only const&)& noexcept->move_construct_only& = delete;
    constexpr auto operator = (move_construct_only&&)& noexcept->move_construct_only& = delete;
  };
}

#define DANGO_TEST_TYPE move_construct_only

static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag>);

static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

#undef DANGO_TEST_TYPE

namespace
{
  struct
  move_construct_only_throwing
  final
  {
    constexpr move_construct_only_throwing()noexcept = default;
    ~move_construct_only_throwing()noexcept = default;
    constexpr move_construct_only_throwing(move_construct_only_throwing const&)noexcept = delete;
    move_construct_only_throwing(move_construct_only_throwing&&)noexcept(false){ throw 0; }
    constexpr auto operator = (move_construct_only_throwing const&)& noexcept->move_construct_only_throwing& = delete;
    constexpr auto operator = (move_construct_only_throwing&&)& noexcept->move_construct_only_throwing& = delete;
  };
}

#define DANGO_TEST_TYPE move_construct_only_throwing

static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag>);

static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

#undef DANGO_TEST_TYPE

namespace
{
  struct
  move_only
  final
  {
    constexpr move_only()noexcept = default;
    ~move_only()noexcept = default;
    constexpr move_only(move_only const&)noexcept = delete;
    constexpr move_only(move_only&&)noexcept = default;
    constexpr auto operator = (move_only const&)& noexcept->move_only& = delete;
    constexpr auto operator = (move_only&&)& noexcept->move_only& = default;
  };
}

#define DANGO_TEST_TYPE move_only

static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag>);

static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

#undef DANGO_TEST_TYPE

namespace
{
  struct
  move_only_throwing
  final
  {
    constexpr move_only_throwing()noexcept = default;
    ~move_only_throwing()noexcept = default;
    constexpr move_only_throwing(move_only_throwing const&)noexcept = delete;
    move_only_throwing(move_only_throwing&&)noexcept(false){ throw 0; }
    constexpr auto operator = (move_only_throwing const&)& noexcept->move_only_throwing& = delete;
    auto operator = (move_only_throwing&&)& noexcept(false)->move_only_throwing&{ throw 0; };
  };
}

#define DANGO_TEST_TYPE move_only_throwing

static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag>);

static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

#undef DANGO_TEST_TYPE

namespace
{
  struct
  copy_construct_only
  final
  {
    constexpr copy_construct_only()noexcept = default;
    ~copy_construct_only()noexcept = default;
    constexpr copy_construct_only(copy_construct_only const&)noexcept = default;
    constexpr copy_construct_only(copy_construct_only&&)noexcept = default;
    constexpr auto operator = (copy_construct_only const&)& noexcept->copy_construct_only& = delete;
    constexpr auto operator = (copy_construct_only&&)& noexcept->copy_construct_only& = delete;
  };
}

#define DANGO_TEST_TYPE copy_construct_only

static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag>);

static_assert(dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_copy_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_noexcept_copy_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

#undef DANGO_TEST_TYPE

namespace
{
  struct
  copy_construct_only_throwing
  final
  {
    constexpr copy_construct_only_throwing()noexcept = default;
    ~copy_construct_only_throwing()noexcept = default;
    copy_construct_only_throwing(copy_construct_only_throwing const&)noexcept(false){ throw 0; }
    copy_construct_only_throwing(copy_construct_only_throwing&&)noexcept(false){ throw 0; }
    constexpr auto operator = (copy_construct_only_throwing const&)& noexcept->copy_construct_only_throwing& = delete;
    constexpr auto operator = (copy_construct_only_throwing&&)& noexcept->copy_construct_only_throwing& = delete;
  };
}

#define DANGO_TEST_TYPE copy_construct_only_throwing

static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag>);

static_assert(dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_noexcept_copy_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_noexcept_copy_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

#undef DANGO_TEST_TYPE

namespace
{
  struct
  copyable
  final
  {
    constexpr copyable()noexcept = default;
    ~copyable()noexcept = default;
    constexpr copyable(copyable const&)noexcept = default;
    constexpr copyable(copyable&&)noexcept = default;
    constexpr auto operator = (copyable const&)& noexcept->copyable& = default;
    constexpr auto operator = (copyable&&)& noexcept->copyable& = default;
  };
}

#define DANGO_TEST_TYPE copyable

static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag>);

static_assert(dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_copy_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_copy_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

#undef DANGO_TEST_TYPE

namespace
{
  struct
  copyable_throwing
  final
  {
    constexpr copyable_throwing()noexcept = default;
    ~copyable_throwing()noexcept = default;
    copyable_throwing(copyable_throwing const&)noexcept(false){ throw 0; }
    copyable_throwing(copyable_throwing&&)noexcept(false){ throw 0; }
    auto operator = (copyable_throwing const&)& noexcept(false)->copyable_throwing&{ throw 0; }
    auto operator = (copyable_throwing&&)& noexcept(false)->copyable_throwing&{ throw 0; }
  };
}

#define DANGO_TEST_TYPE copyable_throwing

static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_default_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::value_init_tag const&>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::value_init_tag, dango::value_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE>, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::skip_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::value_init_tag, dango::skip_init_tag>);
static_assert(dango::is_noexcept_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag, dango::value_init_tag, dango::skip_init_tag>);

static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_noexcept_destructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>, dango::skip_init_tag>);

static_assert(dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_noexcept_copy_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_copy_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_move_constructible<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_noexcept_copy_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_copy_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(dango::is_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

static_assert(!dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);
static_assert(!dango::is_noexcept_move_assignable<dango::tuple<DANGO_TEST_TYPE, DANGO_TEST_TYPE, DANGO_TEST_TYPE>>);

#undef DANGO_TEST_TYPE

static_assert(dango::is_constructible<dango::tuple<int const&, float const&, bool const&>, dango::tuple<int&, float&, bool&> const&>);
static_assert(dango::is_constructible<dango::tuple<int const&, float const&, bool const&>, dango::tuple<int&, float&, bool&>&&>);
static_assert(!dango::is_assignable<dango::tuple<int const&, float const&, bool const&>, dango::tuple<int&, float&, bool&> const&>);
static_assert(!dango::is_assignable<dango::tuple<int const&, float const&, bool const&>, dango::tuple<int&, float&, bool&> &&>);
static_assert(!dango::is_assignable<dango::tuple<int&, float&, bool&>, dango::tuple<int&, float&, bool&> const&>);
static_assert(!dango::is_assignable<dango::tuple<int&, float&, bool&>, dango::tuple<int&, float&, bool&>&&>);
static_assert(!dango::is_assignable<dango::tuple<int&, float&, bool&>, dango::tuple<int, float, bool> const&>);
static_assert(!dango::is_assignable<dango::tuple<int&&, float&&, bool&&>, dango::tuple<int, float, bool>&&>);
static_assert(dango::is_constructible<dango::tuple<int&&, float&&, bool&&>, dango::tuple<int, float, bool>&&>);

