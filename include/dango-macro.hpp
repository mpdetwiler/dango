#ifndef DANGO_MACRO_HPP_INCLUDED
#define DANGO_MACRO_HPP_INCLUDED

#ifndef __GNUG__
static_assert(false, "dango requires GCC or clang to compile");
#endif

#ifdef __clang__
#define DANGO_USING_CLANG
#else
#define DANGO_USING_GCC
#endif

#ifdef DANGO_BUILDING_DANGO
#ifndef DANGO_BUILDING_LIB
#define DANGO_BUILDING_LIB
#endif
#endif

#ifdef _WIN32
#define DANGO_PLATFORM_WINDOWS
#endif

#ifdef __linux__
#define DANGO_PLATFORM_LINUX
#define DANGO_PLATFORM_LINUX_OR_APPLE
#endif

#ifdef __APPLE__
#define DANGO_PLATFORM_APPLE
#define DANGO_PLATFORM_LINUX_OR_APPLE
#endif

#ifdef DANGO_PLATFORM_WINDOWS

#ifdef DANGO_BUILDING_LIB
#define DANGO_EXPORT [[gnu::dllexport]]
#define DANGO_EXPORT_ONLY DANGO_EXPORT
#else
#define DANGO_EXPORT [[gnu::dllimport]]
#define DANGO_EXPORT_ONLY
#endif

#endif // DANGO_PLATFORM_WINDOWS

#ifdef DANGO_PLATFORM_LINUX_OR_APPLE

#ifdef DANGO_BUILDING_LIB
#define DANGO_EXPORT [[gnu::visibility("default")]]
#define DANGO_EXPORT_ONLY DANGO_EXPORT
#else
#define DANGO_EXPORT
#define DANGO_EXPORT_ONLY
#endif

#endif // DANGO_PLATFORM_LINUX_OR_APPLE

#define DANGO_UNCOPYABLE(name) \
constexpr name(name const&)noexcept = delete; \
constexpr auto operator = (name const&)& noexcept->name& = delete;

#define DANGO_IMMOBILE(name) \
DANGO_UNCOPYABLE(name) \
constexpr name(name&&)noexcept = delete; \
constexpr auto operator = (name&&)& noexcept->name& = delete;

#define DANGO_DELETE_DEFAULT(name) \
explicit constexpr name()noexcept = delete;

#define DANGO_UNINSTANTIABLE(name) \
DANGO_IMMOBILE(name) \
DANGO_DELETE_DEFAULT(name) \
~name()noexcept = delete;

#define DANGO_UNASSIGNABLE(name) \
constexpr auto operator = (name const&)& noexcept->name& = delete; \
constexpr auto operator = (name&&)& noexcept->name& = delete;

#define DANGO_EMPTY_TYPE(name, ...) \
explicit(bool(__VA_ARGS__)) constexpr name()noexcept = default; \
constexpr name(name const&)noexcept = default; \
constexpr name(name&&)noexcept = default; \
~name()noexcept = default; \
constexpr auto operator = (name const&)& noexcept->name& = default; \
constexpr auto operator = (name&&)& noexcept->name& = default;

#define DANGO_TAG_TYPE(name) DANGO_EMPTY_TYPE(name, true)

#define DANGO_TOKEN_CONCAT_IMPL(x, y) x##y
#define DANGO_TOKEN_CONCAT(x, y) DANGO_TOKEN_CONCAT_IMPL(x, y)
#define DANGO_APPEND_LINE(x) DANGO_TOKEN_CONCAT(x, __LINE__)

/*** DANGO_PARENTH_TYPE ***/

#define DANGO_PARENTH_TYPE(...) dango::parenth_type<void(__VA_ARGS__)noexcept>

namespace
dango::detail
{
  template
  <typename tp_type>
  struct parenth_type_help;

  template
  <typename tp_ret, typename tp_type, bool tp_noexcept>
  struct parenth_type_help<tp_ret(tp_type)noexcept(tp_noexcept)>;
}

namespace
dango
{
  template
  <typename tp_type>
  using parenth_type = typename dango::detail::parenth_type_help<tp_type>::type;
}

template
<typename tp_ret, typename tp_type, bool tp_noexcept>
struct
dango::
detail::
parenth_type_help<tp_ret(tp_type)noexcept(tp_noexcept)>
final
{
  using type = tp_type;

  DANGO_UNINSTANTIABLE(parenth_type_help)
};

/*** DANGO_UNIT_TEST_BEGIN DANGO_UNIT_TEST_END ***/

/*  usage:

DANGO_UNIT_TEST_BEGIN(my_test1)
{
  testing code goes here
}
DANGO_UNIT_TEST_END

namespace my_namespace
{

DANGO_UNIT_TEST_BEGIN(my_test2)
{
  testing code goes here
}
DANGO_UNIT_TEST_END

}

- can only be placed at global or namespace scope
- if DANGO_ENABLE_UNIT_TESTS is defined, a global inline constant boolean variable called s_<my test name>_executor will be introduced into the enclosing scope
- each unit test will be run at some point before control enters main, in no particular order (google "static initialization order fiasco" for more info on the order)
- can be placed in a header or source file
- if you place a unit test in a header file and enclose it in the anon namespace it will be run potentially multiple times, once in every source file that includes the header
- if DANGO_ENABLE_UNIT_TESTS is not defined, no global variable is introduced. the test body is still parsed and syntax-validated but not executed. zero runtime overhead
- test body is the body of a function that returns void. if an execption is thrown during execution of a unit test, std::terminate will be called

*/

namespace
dango::detail
{
  DANGO_EXPORT void unit_test_exec(char const*, char const*, int, void(*)()noexcept(false))noexcept;
}

#ifdef DANGO_ENABLE_UNIT_TESTS

#ifdef DANGO_BUILDING_LIB
static_assert(false, "unit tests should only be enabled when building executables");
#endif

#define DANGO_UNIT_TEST_BEGIN(name) \
  [[maybe_unused]] inline bool const s_##name##_executor_bool = \
  (dango::detail::unit_test_exec(#name, __FILE__, __LINE__, []()noexcept(false)->void

#define DANGO_UNIT_TEST_END \
  ), false);

#else

#define DANGO_UNIT_TEST_BEGIN(name) \
  static_assert(true || noexcept([]()noexcept->void

#define DANGO_UNIT_TEST_END \
  ));

#endif

#endif // DANGO_MACRO_HPP_INCLUDED

