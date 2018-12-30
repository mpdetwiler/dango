#ifndef __DANGO_TRAITS_HPP__
#define __DANGO_TRAITS_HPP__ 1

#include "dango-def.hpp"

namespace
dango::detail
{
  template
  <bool tp_cond, typename tp_type>
  struct enable_if_class;

  template
  <typename tp_type>
  struct enable_if_class<true, tp_type>;
}

namespace
dango
{
  template
  <bool tp_cond, typename tp_type = void>
  using enable_if =
    typename dango::detail::enable_if_class<tp_cond, tp_type>::type;
}

template
<bool tp_cond, typename tp_type>
struct
dango::
detail::
enable_if_class
{

};

template
<typename tp_type>
struct
dango::
detail::
enable_if_class<true, tp_type>
{
  using type = tp_type;
};

#endif

