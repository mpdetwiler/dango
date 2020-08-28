#ifndef DANGO_NET_HPP_INCLUDED
#define DANGO_NET_HPP_INCLUDED

#include "dango-traits.hpp"

namespace
dango::detail
{
  template
  <dango::endian = dango::endian::native>
  struct byte_order_funcs;

  template<>
  struct byte_order_funcs<dango::endian::little>;

  template<>
  struct byte_order_funcs<dango::endian::big>;
}

template<>
struct
dango::
detail::
byte_order_funcs<dango::endian::little>
final
{
public:
  static constexpr auto hton(dango::ubyte const a_arg)noexcept->dango::ubyte{ return a_arg; }
  static constexpr auto hton(dango::ushort const a_arg)noexcept->dango::ushort{ return __builtin_bswap16(a_arg); }
  static constexpr auto hton(dango::uint const a_arg)noexcept->dango::uint{ return __builtin_bswap32(a_arg); }
  static constexpr auto hton(dango::ulong const a_arg)noexcept->dango::ulong{ return __builtin_bswap64(a_arg); }

  static constexpr auto ntoh(dango::ubyte const a_arg)noexcept->dango::ubyte{ return a_arg; }
  static constexpr auto ntoh(dango::ushort const a_arg)noexcept->dango::ushort{ return __builtin_bswap16(a_arg); }
  static constexpr auto ntoh(dango::uint const a_arg)noexcept->dango::uint{ return __builtin_bswap32(a_arg); }
  static constexpr auto ntoh(dango::ulong const a_arg)noexcept->dango::ulong{ return __builtin_bswap64(a_arg); }
public:
  DANGO_UNCONSTRUCTIBLE(byte_order_funcs)
};

template<>
struct
dango::
detail::
byte_order_funcs<dango::endian::big>
final
{
public:
  static constexpr auto hton(dango::ubyte const a_arg)noexcept->dango::ubyte{ return a_arg; }
  static constexpr auto hton(dango::ushort const a_arg)noexcept->dango::ushort{ return a_arg; }
  static constexpr auto hton(dango::uint const a_arg)noexcept->dango::uint{ return a_arg; }
  static constexpr auto hton(dango::ulong const a_arg)noexcept->dango::ulong{ return a_arg; }

  static constexpr auto ntoh(dango::ubyte const a_arg)noexcept->dango::ubyte{ return a_arg; }
  static constexpr auto ntoh(dango::ushort const a_arg)noexcept->dango::ushort{ return a_arg; }
  static constexpr auto ntoh(dango::uint const a_arg)noexcept->dango::uint{ return a_arg; }
  static constexpr auto ntoh(dango::ulong const a_arg)noexcept->dango::ulong{ return a_arg; }
public:
  DANGO_UNCONSTRUCTIBLE(byte_order_funcs)
};

namespace
dango
{
  template
  <typename tp_arg>
  constexpr auto hton(tp_arg)noexcept = delete;

  constexpr auto hton(dango::ubyte const a_arg)noexcept->dango::ubyte{ return detail::byte_order_funcs<>::hton(a_arg); }
  constexpr auto hton(dango::ushort const a_arg)noexcept->dango::ushort{ return detail::byte_order_funcs<>::hton(a_arg); }
  constexpr auto hton(dango::uint const a_arg)noexcept->dango::uint{ return detail::byte_order_funcs<>::hton(a_arg); }
  constexpr auto hton(dango::ulong const a_arg)noexcept->dango::ulong{ return detail::byte_order_funcs<>::hton(a_arg); }

  template
  <typename tp_arg>
  constexpr auto ntoh(tp_arg)noexcept = delete;

  constexpr auto ntoh(dango::ubyte const a_arg)noexcept->dango::ubyte{ return detail::byte_order_funcs<>::ntoh(a_arg); }
  constexpr auto ntoh(dango::ushort const a_arg)noexcept->dango::ushort{ return detail::byte_order_funcs<>::ntoh(a_arg); }
  constexpr auto ntoh(dango::uint const a_arg)noexcept->dango::uint{ return detail::byte_order_funcs<>::ntoh(a_arg); }
  constexpr auto ntoh(dango::ulong const a_arg)noexcept->dango::ulong{ return detail::byte_order_funcs<>::ntoh(a_arg); }
}

#endif // DANGO_NET_HPP_INCLUDED
