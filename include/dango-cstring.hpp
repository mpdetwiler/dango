#ifndef DANGO_CSTRING_HPP_INCLUDED
#define DANGO_CSTRING_HPP_INCLUDED

#include "dango-assert.hpp"

namespace
dango
{
  template
  <typename tp_char>
  concept is_cstring_char =
    dango::is_integral_exclude_bool<tp_char>;
}

namespace
dango
{
  template
  <dango::is_cstring_char tp_char>
  constexpr auto
  str_size
  (tp_char const* a_str)noexcept->dango::usize
  {
    dango_assert(a_str != dango::null);

    auto a_size = dango::usize(0);

    while((*(a_str++)) != tp_char(0))
    {
      ++a_size;
    }

    return a_size;
  }

  template
  <dango::is_cstring_char tp_char>
  constexpr auto
  str_copy
  (
    tp_char* a_dst,
    tp_char const* a_src,
    dango::usize a_cap
  )
  noexcept->dango::usize
  {
    dango_assert(a_dst != dango::null || a_cap == dango::usize(0));
    dango_assert(a_src != dango::null);

    auto a_size = dango::usize(0);

    while(a_cap != dango::usize(0))
    {
      auto const a_r = (*a_src);

      if(a_r == tp_char(0))
      {
        break;
      }

      (*a_dst) = a_r;

      ++a_src;
      ++a_dst;
      --a_cap;

      ++a_size;
    }

    while((*(a_src++)) != tp_char(0))
    {
      ++a_size;
    }

    return a_size;
  }

  template
  <dango::is_cstring_char tp_char>
  constexpr auto
  str_term_copy
  (
    tp_char* a_dst,
    tp_char const* a_src,
    dango::usize a_cap
  )
  noexcept->dango::usize
  {
    dango_assert(a_dst != dango::null);
    dango_assert(a_src != dango::null);
    dango_assert(a_cap != dango::usize(0));

    auto a_size = dango::usize(0);

    do
    {
      auto const a_r = (*a_src);

      if(a_r == tp_char(0))
      {
        break;
      }

      (*a_dst) = a_r;

      ++a_src;

      ++a_size;

      if(--a_cap == dango::usize(0))
      {
        break;
      }

      ++a_dst;
    }
    while(true);

    (*a_dst) = tp_char(0);

    while((*(a_src++)) != tp_char(0))
    {
      ++a_size;
    }

    return a_size;
  }
}

#endif // DANGO_CSTRING_HPP_INCLUDED
