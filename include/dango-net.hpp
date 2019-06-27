#ifndef __DANGO_NET_HPP__
#define __DANGO_NET_HPP__

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
  static constexpr auto hton(dango::uint8 const a_arg)noexcept->dango::uint8{ return a_arg; }
  static constexpr auto hton(dango::uint16 const a_arg)noexcept->dango::uint16{ return __builtin_bswap16(a_arg); }
  static constexpr auto hton(dango::uint32 const a_arg)noexcept->dango::uint32{ return __builtin_bswap32(a_arg); }
  static constexpr auto hton(dango::uint64 const a_arg)noexcept->dango::uint64{ return __builtin_bswap64(a_arg); }

  static constexpr auto ntoh(dango::uint8 const a_arg)noexcept->dango::uint8{ return a_arg; }
  static constexpr auto ntoh(dango::uint16 const a_arg)noexcept->dango::uint16{ return __builtin_bswap16(a_arg); }
  static constexpr auto ntoh(dango::uint32 const a_arg)noexcept->dango::uint32{ return __builtin_bswap32(a_arg); }
  static constexpr auto ntoh(dango::uint64 const a_arg)noexcept->dango::uint64{ return __builtin_bswap64(a_arg); }
public:
  DANGO_UNINSTANTIABLE(byte_order_funcs)
};

template<>
struct
dango::
detail::
byte_order_funcs<dango::endian::big>
final
{
public:
  static constexpr auto hton(dango::uint8 const a_arg)noexcept->dango::uint8{ return a_arg; }
  static constexpr auto hton(dango::uint16 const a_arg)noexcept->dango::uint16{ return a_arg; }
  static constexpr auto hton(dango::uint32 const a_arg)noexcept->dango::uint32{ return a_arg; }
  static constexpr auto hton(dango::uint64 const a_arg)noexcept->dango::uint64{ return a_arg; }

  static constexpr auto ntoh(dango::uint8 const a_arg)noexcept->dango::uint8{ return a_arg; }
  static constexpr auto ntoh(dango::uint16 const a_arg)noexcept->dango::uint16{ return a_arg; }
  static constexpr auto ntoh(dango::uint32 const a_arg)noexcept->dango::uint32{ return a_arg; }
  static constexpr auto ntoh(dango::uint64 const a_arg)noexcept->dango::uint64{ return a_arg; }
public:
  DANGO_UNINSTANTIABLE(byte_order_funcs)
};

namespace
dango
{
  template
  <typename tp_arg>
  constexpr auto hton(tp_arg)noexcept = delete;

  constexpr auto hton(dango::uint8 const a_arg)noexcept->dango::uint8{ return detail::byte_order_funcs<>::hton(a_arg); }
  constexpr auto hton(dango::uint16 const a_arg)noexcept->dango::uint16{ return detail::byte_order_funcs<>::hton(a_arg); }
  constexpr auto hton(dango::uint32 const a_arg)noexcept->dango::uint32{ return detail::byte_order_funcs<>::hton(a_arg); }
  constexpr auto hton(dango::uint64 const a_arg)noexcept->dango::uint64{ return detail::byte_order_funcs<>::hton(a_arg); }

  template
  <typename tp_arg>
  constexpr auto ntoh(tp_arg)noexcept = delete;

  constexpr auto ntoh(dango::uint8 const a_arg)noexcept->dango::uint8{ return detail::byte_order_funcs<>::ntoh(a_arg); }
  constexpr auto ntoh(dango::uint16 const a_arg)noexcept->dango::uint16{ return detail::byte_order_funcs<>::ntoh(a_arg); }
  constexpr auto ntoh(dango::uint32 const a_arg)noexcept->dango::uint32{ return detail::byte_order_funcs<>::ntoh(a_arg); }
  constexpr auto ntoh(dango::uint64 const a_arg)noexcept->dango::uint64{ return detail::byte_order_funcs<>::ntoh(a_arg); }
}

#endif

