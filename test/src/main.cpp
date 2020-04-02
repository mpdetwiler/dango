#include "dango.hpp"

#include <cstdio>
#include <string>
#include <memory>

static_assert(dango::is_nullable<dango::null_tag>);
static_assert(dango::is_nullable<dango::thread>);
/*
template
<typename tp_void, typename tp_enabled = dango::enable_tag>
struct test_config;

template
<typename tp_void>
struct
test_config<tp_void, dango::enable_if<dango::is_void<tp_void>>>
final:
public dango::auto_ptr_default_config<tp_void>
{
  static constexpr bool const store_size = false;
  static constexpr bool const store_align = false;
  static constexpr bool const destroy_size = false;
  static constexpr bool const destroy_align = false;
  static constexpr dango::usize const alignment = dango::usize(16);

  struct destroyer
  {
    static void
    destroy(tp_void* const)noexcept
    {

    }
  };
};*/

struct test_base{ virtual ~test_base()noexcept = default; };

struct test_derived:test_base{ ~test_derived()noexcept override = default; };

static_assert(dango::is_constructible<void*, int* const&>);
static_assert(dango::is_brace_constructible<void*, int* const&>);
static_assert(dango::is_constructible<test_base*, test_derived* const&>);
static_assert(dango::is_brace_constructible<test_base*, test_derived* const&>);
static_assert(dango::is_constructible<test_base&, test_derived&>);
static_assert(dango::is_brace_constructible<test_base&, test_derived&>);
static_assert(!dango::is_constructible<test_derived&, test_base&>);
static_assert(!dango::is_brace_constructible<test_derived&, test_base&>);

static_assert(dango::is_destructible<test_derived>);
static_assert(dango::is_noexcept_destructible<test_derived>);
static_assert(!dango::is_trivial_destructible<test_derived>);
static_assert(dango::is_virtual_destructible<test_derived>);

static_assert(!dango::is_convertible<dango::mutex, dango::mutex const>);
static_assert(dango::is_convertible_ret<dango::mutex, dango::mutex const>);
static_assert(!dango::is_noexcept_convertible<dango::mutex, dango::mutex const>);
static_assert(dango::is_noexcept_convertible_ret<dango::mutex, dango::mutex const>);

struct int_holder
{
  int m_int = 5;
  int& m_ref = m_int;
  int&& m_rref = dango::move(m_int);
};

static_assert(dango::is_same<decltype(dango::declval<int_holder&&>().m_int), int>);
static_assert(dango::is_same<decltype((dango::declval<int_holder&&>().m_int)), int&&>);
static_assert(dango::is_same<decltype(dango::declval<int_holder&>().m_int), int>);
static_assert(dango::is_same<decltype((dango::declval<int_holder&>().m_int)), int&>);
static_assert(dango::is_same<decltype(dango::declval<int_holder const&>().m_int), int>);
static_assert(dango::is_same<decltype((dango::declval<int_holder const&>().m_int)), int const&>);

static_assert(dango::is_same<decltype(dango::declval<int_holder&&>().m_ref), int&>);
static_assert(dango::is_same<decltype((dango::declval<int_holder&&>().m_ref)), int&>);
static_assert(dango::is_same<decltype(dango::declval<int_holder&>().m_ref), int&>);
static_assert(dango::is_same<decltype((dango::declval<int_holder&>().m_ref)), int&>);
static_assert(dango::is_same<decltype(dango::declval<int_holder const&>().m_ref), int&>);
static_assert(dango::is_same<decltype((dango::declval<int_holder const&>().m_ref)), int&>);

static_assert(dango::is_same<decltype(dango::declval<int_holder&&>().m_rref), int&&>);
static_assert(dango::is_same<decltype((dango::declval<int_holder&&>().m_rref)), int&>);
static_assert(dango::is_same<decltype(dango::declval<int_holder&>().m_rref), int&&>);
static_assert(dango::is_same<decltype((dango::declval<int_holder&>().m_rref)), int&>);
static_assert(dango::is_same<decltype(dango::declval<int_holder const&>().m_rref), int&&>);
static_assert(dango::is_same<decltype((dango::declval<int_holder const&>().m_rref)), int&>);

constexpr auto func_test(int&&)noexcept->bool
{
  return true;
}

constexpr auto func_test(int const&)noexcept->bool
{
  return false;
}

static_assert(func_test(int_holder{ }.m_int));
static_assert(func_test(dango::move(int_holder{ }).m_int));
static_assert(!func_test(int_holder{ }.m_ref));
static_assert(!func_test(dango::move(int_holder{ }).m_ref));
static_assert(!func_test(int_holder{ }.m_rref));
static_assert(!func_test(dango::move(int_holder{ }).m_rref));

template
<typename tp_type>
requires(dango::is_same_ignore_cvref<tp_type, bool>)
struct
concept_test
{
  static constexpr auto const* const value = "is_same_ignore_cvref";
};

template
<typename tp_type>
requires(dango::is_same_ignore_ref<tp_type, bool>)
struct
concept_test<tp_type>
{
  static constexpr auto const* const value = "is_same_ignore_ref";
};

template
<typename tp_type>
requires(dango::is_same_ignore_cv<tp_type, bool>)
struct
concept_test<tp_type>
{
  static constexpr auto const* const value = "is_same_ignore_cv";
};

template
<typename tp_type>
requires(dango::is_same<tp_type, bool>)
struct
concept_test<tp_type>
{
  static constexpr auto const* const value = "is_same";
};

template
<typename tp_type>
requires(dango::is_arithmetic<tp_type>)
inline constexpr auto const* const c_arith_test = "is_arithmetic";

template
<typename tp_type>
requires(dango::is_arithmetic_exclude_bool<tp_type>)
inline constexpr auto const* const c_arith_test<tp_type> = "is_arithmetic_exclude_bool";

template
<typename tp_type>
requires(dango::is_integral<tp_type>)
inline constexpr auto const* const c_arith_test<tp_type> = "is_integral";

template
<typename tp_type>
requires(dango::is_integral_exclude_bool<tp_type>)
inline constexpr auto const* const c_arith_test<tp_type> = "is_integral_exclude_bool";

template
<typename tp_type>
concept substitution_test = dango::is_same<tp_type, int> || dango::enable_if<false, tp_type>{ 42 };

static_assert(substitution_test<int>);

static_assert(dango::is_callable_ret<dango::mutex, dango::mutex(int)noexcept, int const&>);
static_assert(dango::is_noexcept_callable_ret<dango::mutex, dango::mutex(int)noexcept, int const&>);
static_assert(!dango::is_noexcept_callable_ret<dango::mutex, dango::mutex(int)noexcept(false), int const&>);


auto
main
()noexcept(false)->dango::builtin::sint
{
  dango_assert(dango::str_size(dango::bchar_as_char(u8"hello")) == 5);
  dango_assert(dango::str_size(dango::char_as_bchar("hello")) == 5);

  printf("%s\n", concept_test<bool const&>::value);
  printf("%s\n", concept_test<bool&&>::value);
  printf("%s\n", concept_test<bool const volatile>::value);
  printf("%s\n", concept_test<bool>::value);

  printf("%s\n", c_arith_test<float>);
  printf("%s\n", c_arith_test<int>);
  printf("%s\n", c_arith_test<bool>);

  {
    using elem_type = double;
    constexpr auto const c_count = dango::usize(4);

    dango::aligned_storage<c_count * sizeof(elem_type), alignof(elem_type)> a_storage;

    auto const a_array = ::new (dango::placement, a_storage.get(), sizeof(elem_type), alignof(elem_type), dango::usize(c_count)) elem_type[c_count];

    dango_assert(a_array != dango::null);
  }

  dango_assert(dango::thread::self_ID() == dango::thread::self().get_ID());

  dango::thread a_thread = null;

  dango_assert(a_thread == a_thread);
  dango_assert(!(a_thread != a_thread));

  dango::allocator_tree a_tree0 = dango::null;

  {
    auto a_alloc = dango::allocator::make_local<dango::local_allocator>();

    a_tree0 = dango::allocator_tree{ a_alloc };

    a_tree0 = dango::null;

    auto const a_tree = dango::allocator_tree{ a_alloc };

    auto const a_tree2 = dango::allocator_tree(a_alloc, dango::allocator_tree{ dango::get_default_allocator(), dango::null, a_tree }, a_tree, a_tree);

    a_alloc.free(a_tree2.get_allocator().alloc(128, 16), 128, 16);

    a_tree2.get_child(0).get_allocator();

  }

  {
    dango::tuple<slong, ushort, sshort, float> a_tup{ 5, 5, 5, 5 };

    static_assert(sizeof(a_tup) == 16);

    printf("%p\n", static_cast<void const*>(&a_tup.first()));
    printf("%p\n", static_cast<void const*>(&a_tup.second()));
    printf("%p\n", static_cast<void const*>(&a_tup.third()));
    printf("%p\n", static_cast<void const*>(&a_tup.fourth()));

    //a_tup = a_tup;
  }

  {
    dango::atomic<dango::ssize*> a_atomic{ null };

    a_atomic.fetch_add(dango::ssize(5));

    dango::address_of(a_atomic);
  }

  {
    dango::atomic<dango::ssize> a_atomic{ 0 };

    a_atomic.fetch_add(dango::ssize(5));

    a_atomic.fetch_and(dango::ssize(5));

    dango::address_of(a_atomic);
  }

  return 0;
}

constexpr auto const integer = 5;

static_assert(dango::is_aligned(&integer, 4));
static_assert(dango::is_aligned(&integer, 16));

static_assert(u8"hello"[0] == u8'h');

static_assert(dango::char_as_bchar(static_cast<char const*>(null)) == null);
static_assert(dango::bchar_as_char(static_cast<dango::bchar const*>(null)) == null);
static_assert(dango::ptr_as_uint(null) == dango::ptr_as_sint(null));

constexpr auto const a_location = dango::source_location::current();

static_assert(dango::str_size(a_location.file_as_char()) == 17);
static_assert(dango::assume_aligned<8>(&a_location) == &a_location);
static_assert(dango::address_of(a_location) == &a_location);

