#include "dango.hpp"

#include <cstdio>
#include <string>
#include <memory>

static_assert(dango::is_nullable<dango::null_tag>);
static_assert(dango::is_nullable<dango::thread>);

auto
main
()noexcept(false)->dango::s_int
{
  dango::allocator_tree a_tree0 = dango::null;

  {
    auto a_alloc = dango::allocator::make<dango::allocator_base>();

    a_tree0 = dango::allocator_tree{ a_alloc };

    a_tree0 = dango::null;

    auto const a_tree = dango::allocator_tree{ a_alloc };

    auto const a_tree2 = dango::allocator_tree(a_alloc, dango::allocator_tree{ a_alloc, dango::null, a_tree }, a_tree, a_tree);

    a_alloc.free(a_tree2.get_allocator().alloc(128, 16), 128, 16);

    a_tree2.get_child(0).get_allocator();

  }



  return 0;
}

