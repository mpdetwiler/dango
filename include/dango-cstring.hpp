#ifndef __DANGO_CSTRING_HPP__
#define __DANGO_CSTRING_HPP__

namespace
dango::detail
{
  template
  <typename tp_char>
  constexpr bool const is_cstring_char =
    dango::is_nonbool_integral<tp_char>;
}

namespace
dango
{
  template
  <typename tp_char, dango::enable_if<detail::is_cstring_char<tp_char>> = dango::enable_val>
  constexpr auto
  str_size
  (tp_char const* a_str)noexcept->dango::usize
  {
    dango_assert(a_str != nullptr);

    auto a_size = dango::usize(0);

    while((*(a_str++)) != tp_char(0))
    {
      ++a_size;
    }

    return a_size;
  }

  template
  <typename tp_char, dango::enable_if<detail::is_cstring_char<tp_char>> = dango::enable_val>
  constexpr auto
  str_copy
  (
    tp_char* a_dst,
    tp_char const* a_src,
    dango::usize a_cap
  )
  noexcept->dango::usize
  {
    dango_assert(a_dst != nullptr || a_cap == dango::usize(0));
    dango_assert(a_src != nullptr);

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
  <typename tp_char, dango::enable_if<detail::is_cstring_char<tp_char>> = dango::enable_val>
  constexpr auto
  str_term_copy
  (
    tp_char* a_dst,
    tp_char const* a_src,
    dango::usize a_cap
  )
  noexcept->dango::usize
  {
    dango_assert(a_dst != nullptr);
    dango_assert(a_src != nullptr);
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

#endif
