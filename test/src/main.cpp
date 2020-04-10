#include "dango.hpp"
#include "dango-global-test.hpp"

#include <cstdio>
#include <string>
#include <memory>

static_assert(dango::is_nullable<dango::null_tag>);
static_assert(dango::is_nullable<dango::thread>);

struct has_const{ int const m_int = 5; };

static_assert(!dango::is_copy_constructible<dango::auto_ptr<int> const>);
static_assert(!dango::is_move_constructible<dango::auto_ptr<int> const>);
static_assert(!dango::is_copy_constructible<dango::tuple<dango::auto_ptr<int>> const>);
static_assert(!dango::is_move_constructible<dango::tuple<dango::auto_ptr<int>> const>);

static_assert(!dango::is_copy_constructible<dango::auto_ptr<int>>);
static_assert(dango::is_move_constructible<dango::auto_ptr<int>>);
static_assert(!dango::is_copy_constructible<dango::tuple<dango::auto_ptr<int>>>);
static_assert(dango::is_move_constructible<dango::tuple<dango::auto_ptr<int>>>);

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

static_assert(dango::is_callable_ret<dango::mutex, dango::mutex(int)noexcept, int const&>);
static_assert(dango::is_noexcept_callable_ret<dango::mutex, dango::mutex(int)noexcept, int const&>);
static_assert(!dango::is_noexcept_callable_ret<dango::mutex, dango::mutex(int)noexcept(false), int const&>);

struct printer
{
  printer()noexcept{ fprintf(stderr, "printer::printer()\n"); }
  ~printer()noexcept{ fprintf(stderr, "printer::~printer()\n"); }
  void print()noexcept{ fprintf(stderr, "printer::print()\n"); }
};

auto
main
()noexcept(false)->dango::builtin::sint
{
  auto a_timeout = dango::make_timeout_rel_hr(1'000);

  for(auto a_i = uint(0); a_i < uint(10); ++a_i)
  {
    auto const a_tc = dango::tick_count_suspend_bias();

    fprintf(stderr, "tick_count()=%llu\n", dango::builtin::ulonglong(a_tc.first()));
    fprintf(stderr, "tick_count_suspend_aware()=%llu\n", dango::builtin::ulonglong(a_tc.first() + a_tc.second()));
    fprintf(stderr, "suspend_bias()=%llu\n\n", dango::builtin::ulonglong(a_tc.second()));

    dango::thread::sleep(a_timeout);

    a_timeout.add(1'000);
  }


  /*dango_assert(dango::str_size(dango::bchar_as_char(u8"hello")) == 5);
  dango_assert(dango::str_size(dango::char_as_bchar("hello")) == 5);

  fprintf(stderr, "%s\n", concept_test<bool const&>::value);
  fprintf(stderr, "%s\n", concept_test<bool&&>::value);
  fprintf(stderr, "%s\n", concept_test<bool const volatile>::value);
  fprintf(stderr, "%s\n", concept_test<bool>::value);

  fprintf(stderr, "%s\n", c_arith_test<float>);
  fprintf(stderr, "%s\n", c_arith_test<int>);
  fprintf(stderr, "%s\n", c_arith_test<bool>);

  {
    using elem_type = double;
    constexpr auto const c_count = dango::usize(4);

    dango::aligned_storage<c_count * sizeof(elem_type), alignof(elem_type)> a_storage;

    auto const a_array = ::new (dango::placement, a_storage.get(), sizeof(elem_type), alignof(elem_type), dango::usize(c_count)) elem_type[c_count];

    dango_assert(a_array != dango::null);
  }

  dango_assert(dango::thread::self_ID() == dango::thread::self().get_ID());

  dango::thread a_thread = dango::thread::self();

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
    static_assert(dango::is_trivial_default_constructible<decltype(a_tup)>);
    static_assert(dango::is_trivial_copy_constructible<decltype(a_tup)>);
    static_assert(dango::is_trivial_move_constructible<decltype(a_tup)>);
    static_assert(dango::is_trivial_copy_assignable<decltype(a_tup)>);
    static_assert(dango::is_trivial_move_assignable<decltype(a_tup)>);
    static_assert(dango::is_trivial_destructible<decltype(a_tup)>);
    static_assert(dango::is_trivial<decltype(a_tup)>);
    static_assert(!dango::is_standard_layout<decltype(a_tup)>);

    fprintf(stderr, "%p\n", static_cast<void const*>(&a_tup.first()));
    fprintf(stderr, "%p\n", static_cast<void const*>(&a_tup.second()));
    fprintf(stderr, "%p\n", static_cast<void const*>(&dango::tuple_get<dango::usize(2)>(a_tup)));
    fprintf(stderr, "%p\n", static_cast<void const*>(&a_tup.fourth()));

    static_assert(dango::is_same<decltype(a_tup.first()), slong&>);
    static_assert(dango::is_same<decltype(dango::move(a_tup).first()), slong&&>);

    a_tup = dango::move(a_tup);
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

  dango::thread::create([]()noexcept->void{ static thread_local printer t_printer{ }; t_printer.print(); });

  fprintf(stderr, "main joining\n");

  dango::thread::main_join();*/

  dango_access_global(dango::detail::s_cond_var_registry_thread, a_registry_thread)
  {

  }

  dango_access_global(test::s_global_printer, a_global_printer)
  {
    a_global_printer->print();
  }


  fprintf(stderr, "main exiting\n");

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

