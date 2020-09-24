#ifndef DANGO_FIXED_ARRAY_HPP_INCLUDED
#define DANGO_FIXED_ARRAY_HPP_INCLUDED

#include "dango-allocator.hpp"
#include "dango-util.hpp"

namespace
dango::detail
{
  template
  <typename tp_header, dango::usize tp_align = alignof(tp_header)>
  class flex_array;
}

#endif
