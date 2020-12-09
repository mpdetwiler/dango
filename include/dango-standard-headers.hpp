#ifndef DANGO_STANDARD_HEADERS_HPP_INCLUDED
#define DANGO_STANDARD_HEADERS_HPP_INCLUDED

#include <cstddef>
#include <climits>
#include <cstdint>
#include <compare>
#include <exception>
#include <new>
#include <typeinfo>

#ifdef __GLIBCXX__
namespace
std
{
  template
  <typename tp_type>
  struct tuple_size;

  template
  <std::size_t tp_index, typename tp_type>
  struct tuple_element;
}
#endif

#ifdef _LIBCPP_VERSION
#include <utility>
#endif

#endif // DANGO_STANDARD_HEADERS_HPP_INCLUDED
