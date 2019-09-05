#ifndef __DANGO_ASSERT_HPP__
#define __DANGO_ASSERT_HPP__

/*** source_location ***/

namespace
dango
{
  class source_location;
}

class
dango::
source_location
final
{
public:
#ifdef __clang__
  static constexpr auto
  current
  (
    dango::bchar const* = u8"TODO: clang++ FILE",
    dango::bchar const* = u8"TODO: clang++ FUNCTION",
    dango::builtin::sint = dango::builtin::sint(0)
  )
  noexcept->source_location;
#else
  static constexpr auto
  current
  (
    dango::bchar const* = __builtin_FILE(),
    dango::bchar const* = __builtin_FUNCTION(),
    dango::builtin::sint = __builtin_LINE()
  )
  noexcept->source_location;
#endif
private:
  constexpr
  source_location
  (
    dango::bchar const*,
    dango::bchar const*,
    dango::uint
  )
  noexcept;
public:
  ~source_location()noexcept = default;

  constexpr auto file()const noexcept->dango::bchar const*;
  constexpr auto function()const noexcept->dango::bchar const*;
  constexpr auto line()const noexcept->dango::uint;
private:
  dango::bchar const* const m_file;
  dango::bchar const* const m_func;
  dango::uint const m_line;
public:
  DANGO_DELETE_DEFAULT(source_location)
  DANGO_IMMOBILE(source_location)
};

constexpr
dango::
source_location::
source_location
(
  dango::bchar const* const a_file,
  dango::bchar const* const a_func,
  dango::uint const a_line
)
noexcept:
m_file{ a_file },
m_func{ a_func },
m_line{ a_line }
{

}

constexpr auto
dango::
source_location::
file
()const noexcept->dango::bchar const*
{
  return m_file;
}

constexpr auto
dango::
source_location::
function
()const noexcept->dango::bchar const*
{
  return m_func;
}

constexpr auto
dango::
source_location::
line
()const noexcept->dango::uint
{
  return m_line;
}

constexpr auto
dango::
source_location::
current
(
  dango::bchar const* const a_file,
  dango::bchar const* const a_func,
  dango::builtin::sint const a_line
)
noexcept->source_location
{
  return source_location{ a_file, a_func, dango::uint(a_line) };
}

/*** likely unlikely ***/

namespace
dango
{
  constexpr auto likely(bool)noexcept->bool;
  constexpr auto unlikely(bool)noexcept->bool;
}

constexpr auto
dango::
likely
(bool const a_cond)noexcept->bool
{
  return bool(__builtin_expect(a_cond, true));
}

constexpr auto
dango::
unlikely
(bool const a_cond)noexcept->bool
{
  return bool(__builtin_expect(a_cond, false));
}

/*** assume ***/

namespace
dango
{
  constexpr void assume(bool)noexcept;
}

constexpr void
dango::
assume
(bool const a_cond)noexcept
{
  if(dango::unlikely(!a_cond))
  {
    __builtin_unreachable();
  }
}

/*** infinite_loop ***/

namespace
dango
{
  [[noreturn]] void infinite_loop()noexcept;
}

/*** trap_instruction ***/

namespace
dango
{
  [[noreturn]] void trap_instruction()noexcept;
}

inline void
dango::
trap_instruction()noexcept
{
  __builtin_trap();

  dango::infinite_loop();
}

/*** dango_assert ***/

namespace
dango
{
  using assert_log_handler =
    void(*)(dango::bchar const*, dango::bchar const*, dango::source_location const&)noexcept;

  auto set_assert_log_handler(dango::assert_log_handler)noexcept->dango::assert_log_handler;
  auto get_assert_log_handler()noexcept->dango::assert_log_handler;
}

namespace
dango::detail
{
  void
  assert_fail_log
  (
    dango::bchar const*,
    dango::bchar const*,
    dango::source_location const&
  )
  noexcept;

  void assert_fail_once()noexcept;

  void
  default_assert_log_handler
  (
    dango::bchar const*,
    dango::bchar const*,
    dango::source_location const&
  )
  noexcept;
}

inline void
dango::
detail::
assert_fail_log
(
  dango::bchar const* const a_expr,
  dango::bchar const* const a_msg,
  dango::source_location const& a_loc
)
noexcept
{
  detail::assert_fail_once();

  auto const a_handler = dango::get_assert_log_handler();

  if(a_handler)
  {
    a_handler(a_expr, a_msg, a_loc);
  }
  else
  {
    detail::default_assert_log_handler(a_expr, a_msg, a_loc);
  }
}

namespace
dango::detail
{
  constexpr void
  assert_func
  (
    bool,
    dango::bchar const*,
    dango::bchar const*,
    dango::source_location const& = dango::source_location::current()
  )
  noexcept;
}

constexpr void
dango::
detail::
assert_func
(
  bool const a_cond,
  dango::bchar const* const a_expr,
  dango::bchar const* const a_msg,
  dango::source_location const& a_loc
)
noexcept
{
  if(dango::unlikely(!a_cond))
  {
    detail::assert_fail_log(a_expr, a_msg, a_loc);

    dango::trap_instruction();
  }
}

#ifndef DANGO_NO_DEBUG
#define dango_assert(cond) dango::detail::assert_func(bool(cond), #cond, dango::null)
#define dango_assert_loc(cond, loc) dango::detail::assert_func(bool(cond), #cond, dango::null, loc)
#define dango_assert_msg(cond, msg) dango::detail::assert_func(bool(cond), #cond, msg)
#define dango_assert_msg_loc(cond, msg, loc) dango::detail::assert_func(bool(cond), #cond, msg, loc)
#else
#define dango_assert(cond) dango::assume(bool(cond))
#define dango_assert_loc(cond, loc) dango_assert(cond)
#define dango_assert_msg(cond, msg) dango_assert(cond)
#define dango_assert_msg_loc(cond, msg, loc) dango_assert(cond)
#endif

/*** dango_unreachable ***/

namespace
dango::detail
{
  [[noreturn]] void
  unreachable_func
  (
    dango::bchar const* const,
    dango::source_location const& = dango::source_location::current()
  )
  noexcept;

  extern dango::bchar const* const unreachable_expr;
  extern dango::bchar const* const unreachable_message;
}

inline void
dango::
detail::
unreachable_func
(
  dango::bchar const* const a_msg,
  dango::source_location const& a_loc
)
noexcept
{
  detail::assert_fail_log(detail::unreachable_expr, a_msg, a_loc);

  dango::trap_instruction();
}

#ifndef DANGO_NO_DEBUG
#define dango_unreachable dango::detail::unreachable_func(dango::detail::unreachable_message)
#define dango_unreachable_msg(msg) dango::detail::unreachable_func(msg)
#else
#define dango_unreachable do{ __builtin_unreachable(); }while(false)
#define dango_unreachable_msg(msg) dango_unreachable
#endif

/*** source location arg ***/

#ifndef DANGO_NO_DEBUG

#define DANGO_SRC_LOC_ARG_DEFAULT(name) \
dango::source_location const& name = dango::source_location::current()

#define DANGO_SRC_LOC_ARG(name) dango::source_location const& name

#define DANGO_SRC_LOC_ARG_FORWARD(name) name

#else

namespace
dango::detail
{
  struct
  assert_dummy_tag
  final
  {
    DANGO_TAG_TYPE(assert_dummy_tag)
  };

  inline constexpr assert_dummy_tag const assert_dummy_val{ };
}


#define DANGO_SRC_LOC_ARG_DEFAULT(name) \
dango::detail::assert_dummy_tag = dango::detail::assert_dummy_val

#define DANGO_SRC_LOC_ARG(name) dango::detail::assert_dummy_tag const

#define DANGO_SRC_LOC_ARG_FORWARD(name) dango::detail::assert_dummy_val

#endif

/*** terminate get_terminate set_terminate ***/

namespace
dango
{
  using terminate_handler = void(*)()noexcept(false);

  auto set_terminate(terminate_handler)noexcept->dango::terminate_handler;
  auto get_terminate()noexcept->dango::terminate_handler;

  [[noreturn]] void terminate()noexcept;
}

#endif

