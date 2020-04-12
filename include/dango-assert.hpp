#ifndef DANGO_ASSERT_HPP_INCLUDED
#define DANGO_ASSERT_HPP_INCLUDED

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
private:
  using char_type = char;
  using int_type = dango::builtin::sint;
public:
  static constexpr auto
  current
  (
    char_type const* = __builtin_FILE(),
    char_type const* = __builtin_FUNCTION(),
    int_type = __builtin_LINE()
  )
  noexcept->source_location;
private:
  constexpr
  source_location
  (
    char_type const* const a_file,
    char_type const* const a_func,
    int_type const a_line
  )
  noexcept:
  m_file{ a_file },
  m_func{ a_func },
  m_line{ a_line }
  { }
public:
  ~source_location()noexcept = default;

  constexpr auto file_as_char()const noexcept->char_type const*{ return m_file; }
  auto file()const noexcept->dango::bchar const*{ return dango::char_as_bchar(file_as_char()); }

  constexpr auto function_as_char()const noexcept->char_type const*{ return m_func; }
  auto function()const noexcept->dango::bchar const*{ return dango::char_as_bchar(function_as_char()); }

  constexpr auto line()const noexcept->dango::uint{ return dango::uint(m_line); }
private:
  char_type const* const m_file;
  char_type const* const m_func;
  int_type const m_line;
public:
  DANGO_DELETE_DEFAULT(source_location)
  DANGO_IMMOBILE(source_location)
};

constexpr auto
dango::
source_location::
current
(
  char_type const* const a_file,
  char_type const* const a_func,
  int_type const a_line
)
noexcept->source_location
{
  return source_location{ a_file, a_func, a_line };
}

/*** dango_assert ***/

namespace
dango
{
  using assert_log_func =
    void(*)(dango::bchar const*, dango::bchar const*, dango::source_location const&)noexcept;

  DANGO_EXPORT auto set_assert_logger(dango::assert_log_func)noexcept->dango::assert_log_func;
  DANGO_EXPORT auto get_assert_logger()noexcept->dango::assert_log_func;
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

  DANGO_EXPORT void assert_fail_once()noexcept;

  DANGO_EXPORT void
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
  dango::detail::assert_fail_once();

  auto const a_handler = dango::get_assert_logger();

  if(a_handler)
  {
    a_handler(a_expr, a_msg, a_loc);
  }
  else
  {
    dango::detail::default_assert_log_handler(a_expr, a_msg, a_loc);
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
    dango::detail::assert_fail_log(a_expr, a_msg, a_loc);

    dango::trap_instruction();
  }
}

#ifndef DANGO_NO_DEBUG
#define dango_assert(cond) dango::detail::assert_func(bool(cond), u8###cond, dango::null)
#define dango_assert_loc(cond, loc) dango::detail::assert_func(bool(cond), u8###cond, dango::null, loc)
#define dango_assert_msg(cond, msg) dango::detail::assert_func(bool(cond), u8###cond, msg)
#define dango_assert_msg_loc(cond, msg, loc) dango::detail::assert_func(bool(cond), u8###cond, msg, loc)
#else
#define dango_assert(cond) dango::assume(bool(cond))
#define dango_assert_loc(cond, loc) dango_assert(cond)
#define dango_assert_msg(cond, msg) dango_assert(cond)
#define dango_assert_msg_loc(cond, msg, loc) dango_assert(cond)
#endif

/*** dango_assert_noassume ***/

#ifndef DANGO_NO_DEBUG
#define dango_assert_noassume(cond) dango_assert(cond)
#define dango_assert_noassume_loc(cond, loc) dango_assert_loc(cond, loc)
#define dango_assert_noassume_msg(cond, msg) dango_assert_msg(cond, msg)
#define dango_assert_noassume_msg_loc(cond, msg, loc) dango_assert_msg_loc(cond, msg, loc)
#else
#define dango_assert_noassume(cond) void(noexcept(bool(cond)))
#define dango_assert_noassume_loc(cond, loc) dango_assert_noassume(cond)
#define dango_assert_noassume_msg(cond, msg) dango_assert_noassume(cond)
#define dango_assert_noassume_msg_loc(cond, msg, loc) dango_assert_noassume(cond)
#endif

/*** dango_unreachable ***/

namespace
dango::detail
{
  [[noreturn]] void
  unreachable_func(dango::bchar const* const, dango::source_location const& = dango::source_location::current())noexcept;

  inline constexpr dango::bchar const unreachable_expr[]{ u8"unreachable" };
  inline constexpr dango::bchar const unreachable_message[]{ u8"unreachable statement reached" };
}

inline void
dango::
detail::
unreachable_func
(dango::bchar const* const a_msg, dango::source_location const& a_loc)noexcept
{
  dango::detail::assert_fail_log(dango::detail::unreachable_expr, a_msg, a_loc);

  dango::trap_instruction();
}

#ifndef DANGO_NO_DEBUG
#define dango_unreachable dango::detail::unreachable_func(dango::detail::unreachable_message)
#define dango_unreachable_loc(loc) dango::detail::unreachable_func(dango::detail::unreachable_message, loc)
#define dango_unreachable_msg(msg) dango::detail::unreachable_func(msg)
#define dango_unreachable_msg_loc(msg, loc) dango::detail::unreachable_func(msg, loc)
#else
#define dango_unreachable do{ __builtin_unreachable(); }while(false)
#define dango_unreachable_loc(loc) dango_unreachable
#define dango_unreachable_msg(msg) dango_unreachable
#define dango_unreachable_msg_loc(msg, loc) dango_unreachable
#endif

/*** terminate get_terminate set_terminate ***/

namespace
dango
{
  using terminate_func = void(*)()noexcept(false);

  DANGO_EXPORT auto set_terminate(dango::terminate_func)noexcept->dango::terminate_func;
  DANGO_EXPORT auto get_terminate()noexcept->dango::terminate_func;

  [[noreturn]] DANGO_EXPORT void terminate()noexcept;
}

/*** dango_assert_terminate ***/

#ifndef DANGO_NO_DEBUG
#define dango_assert_terminate(cond) dango_assert(cond)
#define dango_assert_terminate_loc(cond, loc) dango_assert_loc(cond, loc)
#define dango_assert_terminate_msg(cond, msg) dango_assert_msg(cond, msg)
#define dango_assert_terminate_msg_loc(cond, msg, loc) dango_assert_msg_loc(cond, msg, loc)
#else
#define dango_assert_terminate(cond) do{ if(dango::unlikely(!bool(cond)){ dango::terminate(); } }while(false)
#define dango_assert_terminate_loc(cond, loc) dango_assert_terminate(cond)
#define dango_assert_terminate_msg(cond, msg) dango_assert_terminate(cond)
#define dango_assert_terminate_msg_loc(cond, msg, loc) dango_assert_terminate(cond)
#endif

/*** dango_unreachable_terminate ***/

#ifndef DANGO_NO_DEBUG
#define dango_unreachable_terminate dango_unreachable
#define dango_unreachable_terminate_loc(loc) dango_unreachable_loc(loc)
#define dango_unreachable_terminate_msg(msg) dango_unreachable_msg(msg)
#define dango_unreachable_terminate_msg_loc(msg, loc) dango_unreachable_msg_loc(msg, loc)
#else
#define dango_unreachable_terminate dango::terminate()
#define dango_unreachable_terminate_loc(loc) dango_unreachable_terminate
#define dango_unreachable_terminate_msg(msg) dango_unreachable_terminate
#define dango_unreachable_terminate_msg_loc(msg, loc) dango_unreachable_terminate
#endif

/*** source location arg ***/

#ifndef DANGO_NO_DEBUG
#define DANGO_SRC_LOC_ARG_DEFAULT(name) dango::source_location const& name = dango::source_location::current()
#define DANGO_SRC_LOC_ARG(name) dango::source_location const& name
#define DANGO_SRC_LOC_ARG_FORWARD(name) name
#else
#define DANGO_SRC_LOC_ARG_DEFAULT(name) dango::detail::assert_dummy_tag const = dango::detail::assert_dummy_val
#define DANGO_SRC_LOC_ARG(name) dango::detail::assert_dummy_tag const
#define DANGO_SRC_LOC_ARG_FORWARD(name) dango::detail::assert_dummy_val

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

#endif

#endif // DANGO_ASSERT_HPP_INCLUDED

