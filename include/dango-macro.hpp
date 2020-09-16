#ifndef DANGO_MACRO_HPP_INCLUDED
#define DANGO_MACRO_HPP_INCLUDED

#include "dango-standard-headers.hpp"

#ifndef __GNUG__
static_assert(false, u8"dango requires GCC or clang to compile");
#endif

#define dango_restrict __restrict__

#ifndef DANGO_NO_RESTRICT_KEYWORD
#ifndef restrict
#define restrict dango_restrict
#endif
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
#define DANGO_EXPORT_LEGACY __declspec(dllexport)
#define DANGO_EXPORT_ONLY DANGO_EXPORT
#define DANGO_EXPORT_ONLY_LEGACY DANGO_EXPORT_LEGACY
#else
#define DANGO_EXPORT [[gnu::dllimport]]
#define DANGO_EXPORT_LEGACY __declspec(dllimport)
#define DANGO_EXPORT_ONLY
#define DANGO_EXPORT_ONLY_LEGACY
#endif

#endif // DANGO_PLATFORM_WINDOWS

#ifdef DANGO_PLATFORM_LINUX_OR_APPLE

#ifdef DANGO_BUILDING_LIB
#define DANGO_EXPORT [[gnu::visibility("default")]]
#define DANGO_EXPORT_LEGACY __attribute((visibility("default")))
#define DANGO_EXPORT_ONLY DANGO_EXPORT
#define DANGO_EXPORT_ONLY_LEGACY DANGO_EXPORT_LEGACY
#else
#define DANGO_EXPORT
#define DANGO_EXPORT_LEGACY
#define DANGO_EXPORT_ONLY
#define DANGO_EXPORT_ONLY_LEGACY
#endif

#endif // DANGO_PLATFORM_LINUX_OR_APPLE

/*** member generators ***/

#define DANGO_DELETE_DEFAULT(name) \
  explicit constexpr name()noexcept = delete;

#define DANGO_UNCOPYABLE(name) \
  constexpr name(name const&)noexcept = delete; \
  constexpr auto operator = (name const&)& noexcept->name& = delete;

#define DANGO_UNMOVEABLE(name) \
  DANGO_UNCOPYABLE(name) \
  constexpr name(name&&)noexcept = delete; \
  constexpr auto operator = (name&&)& noexcept->name& = delete;

#define DANGO_UNCONSTRUCTIBLE(name) \
  DANGO_DELETE_DEFAULT(name) \
  DANGO_UNMOVEABLE(name) \
  constexpr ~name()noexcept = delete;

#define DANGO_UNASSIGNABLE(name) \
  constexpr auto operator = (name const&)& noexcept->name& = delete; \
  constexpr auto operator = (name&&)& noexcept->name& = delete;

#define DANGO_ALL_DEFAULT_CONSTEXPR_NOEXCEPT(name, expl) \
  explicit(expl) constexpr name()noexcept = default; \
  constexpr name(name const&)noexcept = default; \
  constexpr name(name&&)noexcept = default; \
  constexpr ~name()noexcept = default; \
  constexpr auto operator = (name const&)& noexcept->name& = default; \
  constexpr auto operator = (name&&)& noexcept->name& = default;

#define DANGO_TAG_TYPE(name) DANGO_ALL_DEFAULT_CONSTEXPR_NOEXCEPT(name, true)

#define DANGO_DEFINE_NULL_SWAP_ASSIGN(name, cexpr, noex) \
  cexpr auto operator = (dango::null_tag const)& noexcept(noex)->name& \
  { name a_##name##_temp{ dango::null }; dango::swap(*this, a_##name##_temp); return *this; }

#define DANGO_DEFINE_COPY_SWAP_ASSIGN(name, cexpr, noex) \
  cexpr auto operator = (name const& a_##name##_arg)& noexcept(noex)->name& \
  { name a_##name##_temp{ a_##name##_arg }; dango::swap(*this, a_##name##_temp); return *this; }

#define DANGO_DEFINE_MOVE_SWAP_ASSIGN(name, cexpr, noex) \
  cexpr auto operator = (name&& a_##name##_arg)& noexcept(noex)->name& \
  { name a_##name##_temp{ dango::move(a_##name##_arg) }; dango::swap(*this, a_##name##_temp); return *this; }

#define DANGO_DEFINE_NULLABLE_OPERATOR_BOOL(cexpr, noex) \
  cexpr explicit operator bool()const noexcept(noex){ return !dango::is_null(*this); }

/*** append line number to an identifier ***/

#define DANGO_TOKEN_CONCAT_HELP(x, y) x##y
#define DANGO_TOKEN_CONCAT(x, y) DANGO_TOKEN_CONCAT_HELP(x, y)
#define DANGO_APPEND_LINE(x) DANGO_TOKEN_CONCAT(x, __LINE__)

/*** default CACHE_LINE_SIZE ***/

#ifndef DANGO_CACHE_LINE_SIZE
#define DANGO_CACHE_LINE_SIZE 64
#endif

/*** DANGO_CACHE_LINE_START (include dango-def.hpp before use) ***/

#define DANGO_CACHE_LINE_START alignas(dango::cache_align_type)

/*** type name with commas in parentheses to not confuse the preprocessor (include dango-traits.hpp before use) ***/

#define DANGO_PARENTH_TYPE(...) dango::parenth_type<void(__VA_ARGS__)noexcept>

/*** DANGO_USING_*_OPERATORS (include dango-util-base.hpp before use) ***/

#define DANGO_USING_EQUALITY_OPERATORS  \
  using dango::operators::operator == ; \
  using dango::operators::operator != ;

#define DANGO_USING_COMPARISON_OPERATORS \
  DANGO_USING_EQUALITY_OPERATORS         \
  using dango::operators::operator <   ; \
  using dango::operators::operator <=  ; \
  using dango::operators::operator >   ; \
  using dango::operators::operator >=  ; \
  using dango::operators::operator <=> ;

#define DANGO_USING_RANGE_BASED_FOR_OPERATORS \
  using dango::operators::begin;              \
  using dango::operators::end;

#define DANGO_USING_ALL_OPERATORS       \
  DANGO_USING_COMPARISON_OPERATORS      \
  DANGO_USING_RANGE_BASED_FOR_OPERATORS

/*** DANGO_UNIT_TEST_BEGIN DANGO_UNIT_TEST_END (include dango-assert.hpp before use) ***/

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

#ifdef DANGO_ENABLE_UNIT_TESTS

#ifdef DANGO_BUILDING_LIB
static_assert(false, u8"unit tests should only be enabled when building executables");
#endif

#define DANGO_UNIT_TEST_BEGIN(name) \
  [[maybe_unused]] inline bool const s_##name##_executor_bool = \
  (dango::detail::unit_test_exec(#name, __FILE__, __LINE__, []()noexcept(false)->void

#define DANGO_UNIT_TEST_END \
  ), false);

#else // !DANGO_ENABLE_UNIT_TESTS

#define DANGO_UNIT_TEST_BEGIN(name) \
  static_assert(true || (void([]()noexcept->void

#define DANGO_UNIT_TEST_END \
  ), true));

#endif // DANGO_ENABLE_UNIT_TESTS

/*** dango_assert (include dango-assert.hpp before use) ***/

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

/*** dango_assert_nonnull (include dango-assert.hpp before use) ***/

#define dango_assert_nonnull(arg) dango_assert(!dango::is_null(arg))
#define dango_assert_nonnull_loc(arg, loc) dango_assert_loc(!dango::is_null(arg), loc)
#define dango_assert_nonnull_msg(arg, msg) dango_assert_msg(!dango::is_null(arg), msg)
#define dango_assert_nonnull_msg_loc(arg, msg, loc) dango_assert_msg_loc(!dango::is_null(arg), msg, loc)

/*** dango_assert_noassume (include dango-assert.hpp before use) ***/

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

/*** dango_assert_nonnull_noassume (include dango-assert.hpp before use) ***/

#define dango_assert_nonnull_noassume(arg) dango_assert_noassume(!dango::is_null(arg))
#define dango_assert_nonnull_noassume_loc(arg, loc) dango_assert_noassume_loc(!dango::is_null(arg), loc)
#define dango_assert_nonnull_noassume_msg(arg, msg) dango_assert_noassume_msg(!dango::is_null(arg), msg)
#define dango_assert_nonnull_noassume_msg_loc(arg, msg, loc) dango_assert_noassume_msg_loc(!dango::is_null(arg), msg, loc)

/*** dango_assert_terminate (include dango-assert.hpp before use) ***/

#ifndef DANGO_NO_DEBUG
#define dango_assert_terminate(cond) dango_assert(cond)
#define dango_assert_terminate_loc(cond, loc) dango_assert_loc(cond, loc)
#define dango_assert_terminate_msg(cond, msg) dango_assert_msg(cond, msg)
#define dango_assert_terminate_msg_loc(cond, msg, loc) dango_assert_msg_loc(cond, msg, loc)
#else
#define dango_assert_terminate(cond) do{ if(dango::unlikely(!bool(cond))){ dango::terminate(); } }while(false)
#define dango_assert_terminate_loc(cond, loc) dango_assert_terminate(cond)
#define dango_assert_terminate_msg(cond, msg) dango_assert_terminate(cond)
#define dango_assert_terminate_msg_loc(cond, msg, loc) dango_assert_terminate(cond)
#endif

/*** dango_assert_nonnull_terminate (include dango-assert.hpp before use) ***/

#define dango_assert_nonnull_terminate(arg) dango_assert_terminate(!dango::is_null(arg))
#define dango_assert_nonnull_terminate_loc(arg, loc) dango_assert_terminate_loc(!dango::is_null(arg), loc)
#define dango_assert_nonnull_terminate_msg(arg, msg) dango_assert_terminate_msg(!dango::is_null(arg), msg)
#define dango_assert_nonnull_terminate_msg_loc(arg, msg, loc) dango_assert_terminate_msg_loc(!dango::is_null(arg), msg, loc)

/*** dango_unreachable (include dango-assert.hpp before use) ***/

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

/*** dango_unreachable_terminate (include dango-assert.hpp before use) ***/

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

/*** source location arg (include dango-assert.hpp before use) ***/

#ifndef DANGO_NO_DEBUG
#define DANGO_SRC_LOC_ARG_DEFAULT(name) dango::source_location const& name = dango::source_location::current()
#define DANGO_SRC_LOC_ARG(name) dango::source_location const& name
#define DANGO_SRC_LOC_ARG_FORWARD(name) name
#else
#define DANGO_SRC_LOC_ARG_DEFAULT(name) dango::detail::assert_dummy_tag const = dango::detail::assert_dummy_val
#define DANGO_SRC_LOC_ARG(name) dango::detail::assert_dummy_tag const
#define DANGO_SRC_LOC_ARG_FORWARD(name) dango::detail::assert_dummy_val
#endif

/*** dango_new_noexcept (include dango-mem.hpp before use) ***/

#define dango_new_noexcept noexcept(dango::c_operator_new_noexcept)

#define dango_new_noexcept_and(cond, ...) \
  noexcept(dango::c_operator_new_noexcept && bool(cond __VA_OPT__(, __VA_ARGS__)))

#define dango_new_noexcept_or(cond, ...) \
  noexcept(dango::c_operator_new_noexcept || bool(cond __VA_OPT__(, __VA_ARGS__)))

/*** DANGO_<DEFINE/DELETE>_CLASS_OPERATOR_NEW_DELETE (include dango-mem.hpp before use) ***/

#define DANGO_DEFINE_CLASS_OPERATOR_NEW_DELETE(name) \
  static auto operator new(std::size_t const a_size)dango_new_noexcept->void* \
  { dango_assert(a_size == sizeof(name)); return dango::operator_new(sizeof(name), alignof(name)); } \
  static void operator delete(void* const a_ptr)noexcept \
  { dango::operator_delete(a_ptr, sizeof(name), alignof(name)); }

#define DANGO_DELETE_CLASS_OPERATOR_NEW_DELETE \
  static auto operator new(std::size_t)noexcept->void* = delete; \
  static void operator delete(void*)noexcept = delete;

/*** dango_placement_new (include dango-mem.hpp before use) ***/

#define dango_placement_new(addr, type, ...) \
  ::new (dango::placement, static_cast<void*>(addr), sizeof(type), alignof(type)) type __VA_OPT__(__VA_ARGS__)

#define dango_placement_new_array(addr, type, count) \
  ::new (dango::placement, static_cast<void*>(addr), sizeof(type), alignof(type)) type[dango::usize(count)]

/*** dango_crit ***/

/*

for use with:
- dango::spin_mutex (include dango-concurrent-base.hpp)
- dango::mutex, dango::cond_var (include dango-concurrent.hpp)
- other lockables

useage:

dango::mutex mut{ };

dango_crit(mut)
{
  this scope is a critical section about mut
  a local guard variable called dango_crit_guard_<LINE_NUMBER> is declared in
  this scope where <LINE_NUMBER> is the line number (__LINE__) in the source
  file at which 'dango_crit(mut)' appears (an effort to avoid collisions with
  other local variables)
}

dango_try_crit(mut)
{
  same as above but code here is only executed if (mut).try_lock() was
  successful
}

sometimes you need to specify the name of the guard variable
to use it (the guard returned by dango::cond_var for example):

dango::cond_var cond{ mut };

dango_crit_full(cond, cond_guard)
{
  while(!some_condition)
  {
    cond_guard.wait();
  }
}

dango_try_crit_full(cond, cond_guard)
{
  if(some_condition)
  {
    cond_guard.notify();
  }
}

*/

#define dango_crit(lockable) \
dango_crit_full(lockable, DANGO_APPEND_LINE(dango_crit_guard_))

#define dango_crit_full(lockable, local_name) \
if constexpr(auto const local_name = (lockable).lock(); true)

#define dango_try_crit(lockable) \
dango_try_crit_full(lockable, DANGO_APPEND_LINE(dango_crit_guard_))

#define dango_try_crit_full(lockable, local_name) \
if(auto const local_name = (lockable).try_lock(); local_name)

/*** DANGO_<DECLARE/DEFINE>_GLOBAL (include dango-global.hpp before use) ***/

/*

usage: // TODO

*/

#ifndef DANGO_BUILDING_LIB

#define DANGO_GLOBAL_DEFINE_STATIC_INC(name) static name##_strong_type const name##_strong{ };
#define DANGO_GLOBAL_DEFINE_INLINE_INC(name) namespace name##_namespace{ inline bool const name##_bool{ (void(name()), false) }; }

#define DANGO_GLOBAL_DEFINE_ACCESS(name) \
[[nodiscard]] inline auto \
name(DANGO_SRC_LOC_ARG_DEFAULT(a_loc))noexcept->name##_namespace::name##_weak_type \
{ static name##_namespace::name##_strong_type const name##_strong_func{ }; \
  return name##_namespace::name##_weak_type{ DANGO_SRC_LOC_ARG_FORWARD(a_loc) }; }

#define DANGO_GLOBAL_DEFINE_ACCESS_LIB(name)

#else

#define DANGO_GLOBAL_DEFINE_STATIC_INC(name)
#define DANGO_GLOBAL_DEFINE_INLINE_INC(name)
#define DANGO_GLOBAL_DEFINE_ACCESS(name)

#define DANGO_GLOBAL_DEFINE_ACCESS_LIB(name) \
[[nodiscard]] inline auto \
name##_lib(DANGO_SRC_LOC_ARG_DEFAULT(a_loc))noexcept->name##_namespace::name##_weak_type \
{ return name##_namespace::name##_weak_type{ DANGO_SRC_LOC_ARG_FORWARD(a_loc) }; }

#endif // DANGO_BUILDING_LIB

/*** extern globals ***/

#define DANGO_DECLARE_GLOBAL_EXTERN(type_name, name) \
namespace name##_namespace \
{ \
  using name##_value_type = type_name; \
  using name##_return_type = dango::remove_cv<name##_value_type>; \
  DANGO_EXPORT auto name##_construct()noexcept->name##_return_type; \
  using name##_storage_type = dango::detail::global_storage<name##_value_type, name##_construct>; \
  DANGO_EXPORT_ONLY extern name##_storage_type name##_storage; \
  using name##_strong_type = name##_storage_type::strong_incrementer<name##_storage>; \
  using name##_weak_type = name##_storage_type::weak_incrementer<name##_storage>; \
  DANGO_GLOBAL_DEFINE_STATIC_INC(name) \
} \
DANGO_GLOBAL_DEFINE_ACCESS(name) \
DANGO_GLOBAL_DEFINE_INLINE_INC(name) \
DANGO_GLOBAL_DEFINE_ACCESS_LIB(name)

#define DANGO_DEFINE_GLOBAL_EXTERN(type_name, name, ...) \
namespace name##_namespace \
{ \
  static_assert(dango::detail::global_storage_constraint_spec<name##_value_type>); \
  auto name##_construct()noexcept->name##_return_type \
  { try{ return name##_return_type __VA_ARGS__ ; }catch(...) \
    { dango_unreachable_terminate_msg(u8"constructor of extern global \"name\" threw exception"); } } \
  constinit name##_storage_type name##_storage{ }; \
}

/*** inline globals ***/

#ifndef DANGO_BUILDING_LIB

#define DANGO_DEFINE_GLOBAL_INLINE(type_name, name, ...) \
namespace name##_namespace \
{ \
  using name##_value_type = type_name; \
  static_assert(dango::detail::global_storage_constraint_spec<name##_value_type>); \
  using name##_return_type = dango::remove_cv<name##_value_type>; \
  inline auto name##_construct()noexcept->name##_return_type \
  { try{ return name##_return_type __VA_ARGS__ ; }catch(...) \
    { dango_unreachable_terminate_msg(u8"constructor of inline global \"name\" threw exception"); } } \
  using name##_storage_type = dango::detail::global_storage<name##_value_type, name##_construct>; \
  inline constinit name##_storage_type name##_storage{ }; \
  using name##_strong_type = name##_storage_type::strong_incrementer<name##_storage>; \
  using name##_weak_type = name##_storage_type::weak_incrementer<name##_storage>; \
  DANGO_GLOBAL_DEFINE_STATIC_INC(name) \
} \
DANGO_GLOBAL_DEFINE_ACCESS(name) \
DANGO_GLOBAL_DEFINE_INLINE_INC(name)

#else

#define DANGO_DEFINE_GLOBAL_INLINE(type_name, name, ...)

#endif // DANGO_BUILDING_LIB

/*** access ***/

#define dango_access_global(global_name, local_name) \
if constexpr(auto const local_name = global_name(); true)

/*** DANGO_<DECLARE/DEFINE>_CURRENT_ALLOC_EXPLICIT_INSTANTIATION (include dango-allocator.hpp before use) ***/

/*

for use only if you are writing a library (not an application) that uses dango and
you have written a custom handle-based allocator type that you want to be compatible
with the following:
- dango::current_allocator<your_handle_based_allocator>(default_handle)
- dango::current_allocator_or_default<your_handle_based_allocator>()
- dango::push_allocator<your_handle_based_allocator>(handle)
- dango::push_allocator_if_no_current<your_handle_based_allocator>(handle)
- dango::push_allocator_if_user<your_handle_based_allocator, potential_users...>(handle)
- dango::push_allocator_if_user_and_no_current<your_handle_based_allocator, potential_users...>(handle)
and only if your library calls one of these functions internally with your_handle_based_allocator

these functions (may) access a thread_local pointer to the top of what is effectively a
stack of allocator handles, however it is parameterized on the allocator type, so
effectively every handle-based allocator of distinct type has its own independent
handle stack. this means the thread_local pointer is behind a template. if you want
a particular thread to access the same instance of the pointer accross shared/dynamic
library boundaries (this is almost certainly what you want) then the template must
be explicitly instantiated and exported

usage:

your_handle_based_allocator should be a complete type and the concept
dango::is_handle_based_allocator<your_handle_based_allocator> should be satisfied

this goes in a header file of your library:

DANGO_DECLARE_CURRENT_ALLOC_EXPLICIT_INSTANTIATION(your_handle_based_allocator)

this goes in a source file of your library:

DANGO_DEFINE_CURRENT_ALLOC_EXPLICIT_INSTANTIATION(your_handle_based_allocator)

and thats it.

*/

#define DANGO_DECLARE_CURRENT_ALLOC_EXPLICIT_INSTANTIATION(...) \
  extern template struct DANGO_EXPORT_LEGACY dango::detail::current_alloc<__VA_ARGS__>;

#define DANGO_DEFINE_CURRENT_ALLOC_EXPLICIT_INSTANTIATION(...) \
  template struct dango::detail::current_alloc<__VA_ARGS__>;

/*** workaround for GCC bug 81043 (include dango-traits.hpp before use) ***/

#ifdef DANGO_USING_GCC
#define DANGO_GCC_BUG_81043_WORKAROUND \
  , dango::is_same<void> = void
#define DANGO_GCC_BUG_81043_WORKAROUND_ND \
  , dango::is_same<void>
#define DANGO_GCC_BUG_81043_WORKAROUND_ID(idno, ...) \
  , dango::detail::spec_id<dango::uint(idno), __VA_ARGS__>
#else
#define DANGO_GCC_BUG_81043_WORKAROUND
#define DANGO_GCC_BUG_81043_WORKAROUND_ND
#define DANGO_GCC_BUG_81043_WORKAROUND_ID(idno, ...)
#endif

#endif // DANGO_MACRO_HPP_INCLUDED
