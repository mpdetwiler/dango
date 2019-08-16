#include "dango.hpp"

#include <cstdio>
#include <string>
#include <memory>

static_assert(dango::is_nullable<dango::null_tag>);
static_assert(dango::is_nullable<dango::thread>);

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
};

auto
main
()noexcept(false)->dango::s_int
{
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

  static_assert(sizeof(dango::auto_ptr<void const>) == 24);

  dango::auto_ptr<double> a_double{ new double{ 1.0 } };

  auto a_d2 = dango::auto_ptr<double const>{ dango::move(a_double) };

  double const& a_d = *a_d2;

  dango_assert(a_d == 1.0);

  auto a_void = dango::auto_new(128, 64);

  dango_assert(a_void.align() == 64);

  auto a_void2 = dango::auto_new(128);

  return 0;
}

