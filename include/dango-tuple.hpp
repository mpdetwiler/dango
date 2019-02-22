#ifndef __DANGO_TUPLE_HPP__
#define __DANGO_TUPLE_HPP__ 1

#include "dango-traits.hpp"

namespace
dango::detail
{
  template
  <typename... tp_types>
  struct
  tuple_pack
  final
  {
    DANGO_UNINSTANTIABLE(tuple_pack)
  };

  template
  <typename tp_type, typename tp_pack>
  struct tuple_pack_prepend_help;

  template
  <typename tp_type, typename... tp_pack>
  struct tuple_pack_prepend_help<tp_type, dango::detail::tuple_pack<tp_pack...>>;

  template
  <typename tp_type, typename tp_pack>
  using tuple_pack_prepend = typename detail::tuple_pack_prepend_help<tp_type, tp_pack>::type;
}

template
<typename tp_type, typename... tp_pack>
struct
dango::
detail::
tuple_pack_prepend_help<tp_type, dango::detail::tuple_pack<tp_pack...>>
final
{
  using type = dango::detail::tuple_pack<tp_type, tp_pack...>;

  DANGO_UNINSTANTIABLE(tuple_pack_prepend_help)
};

namespace
dango::detail
{
  template
  <typename tp_pack, typename tp_first, typename... tp_next>
  struct tuple_pack_reverse_help;

  template
  <typename tp_pack, typename tp_first>
  struct tuple_pack_reverse_help<tp_pack, tp_first>;

  template
  <typename... tp_types>
  using tuple_pack_reverse = typename detail::tuple_pack_reverse_help<detail::tuple_pack<>, tp_types...>::type;
}

template
<typename tp_pack, typename tp_first, typename... tp_next>
struct
dango::
detail::
tuple_pack_reverse_help
final
{
  using type = typename detail::tuple_pack_reverse_help<detail::tuple_pack_prepend<tp_first, tp_pack>, tp_next...>::type;

  DANGO_UNINSTANTIABLE(tuple_pack_reverse_help)
};

template
<typename tp_pack, typename tp_first>
struct
dango::
detail::
tuple_pack_reverse_help<tp_pack, tp_first>
final
{
  using type = detail::tuple_pack_prepend<tp_first, tp_pack>;

  DANGO_UNINSTANTIABLE(tuple_pack_reverse_help)
};

namespace
dango::detail
{
  template
  <typename tp_type>
  struct tuple_value_type_help;
  template
  <typename tp_type>
  struct tuple_value_type_help<tp_type[]>;
  template
  <typename tp_type, dango::usize tp_size>
  struct tuple_value_type_help<tp_type[tp_size]>;
  template
  <typename tp_ret, typename... tp_args, bool tp_noexcept>
  struct tuple_value_type_help<tp_ret(tp_args...)noexcept(tp_noexcept)>;

  template
  <typename tp_type>
  using tuple_value_type = typename detail::tuple_value_type_help<tp_type>::type;

  template
  <typename tp_type>
  struct tuple_storage_type_help;
  template
  <typename tp_type>
  struct tuple_storage_type_help<tp_type&&>;

  template
  <typename tp_type>
  using tuple_storage_type = typename detail::tuple_storage_type_help<tp_type>::type;
}

template
<typename tp_type>
struct
dango::
detail::
tuple_value_type_help
final
{
  using type = tp_type;

  DANGO_UNINSTANTIABLE(tuple_value_type_help)
};

template
<typename tp_type>
struct
dango::
detail::
tuple_value_type_help<tp_type[]>
final
{
  using type = tp_type*;

  DANGO_UNINSTANTIABLE(tuple_value_type_help)
};

template
<typename tp_type, dango::usize tp_size>
struct
dango::
detail::
tuple_value_type_help<tp_type[tp_size]>
final
{
  using type = tp_type*;

  DANGO_UNINSTANTIABLE(tuple_value_type_help)
};

template
<typename tp_ret, typename... tp_args, bool tp_noexcept>
struct
dango::
detail::
tuple_value_type_help<tp_ret(tp_args...)noexcept(tp_noexcept)>
final
{
  using type = tp_ret(*)(tp_args...)noexcept(tp_noexcept);

  DANGO_UNINSTANTIABLE(tuple_value_type_help)
};

template
<typename tp_type>
struct
dango::
detail::
tuple_storage_type_help
final
{
  using type = detail::tuple_value_type<tp_type>;

  DANGO_UNINSTANTIABLE(tuple_storage_type_help)
};

template
<typename tp_type>
struct
dango::
detail::
tuple_storage_type_help<tp_type&&>
final
{
  using type = tp_type&;

  DANGO_UNINSTANTIABLE(tuple_storage_type_help)
};

namespace
dango::detail
{
  template
  <typename tp_pack>
  class tuple_storage;

  template
  <typename tp_first>
  class tuple_storage<detail::tuple_pack<tp_first>>;

  template
  <typename tp_first, typename... tp_next>
  class tuple_storage<detail::tuple_pack<tp_first, tp_next...>>;
}

template
<typename tp_first>
class
dango::
detail::
tuple_storage<dango::detail::tuple_pack<tp_first>>
{
private:
  using storage_type = detail::tuple_storage_type<tp_first>;
public:
  using value_type = detail::tuple_value_type<tp_first>;
public:
  template
  <typename tp_arg>
  constexpr
  tuple_storage
  (tp_arg&& a_arg)noexcept:
  m_value{ dango::forward<tp_arg>(a_arg) }
  {

  }
private:
  storage_type m_value;
public:
  DANGO_DELETE_DEFAULT(tuple_storage)
};

template
<typename tp_first, typename... tp_next>
class
dango::
detail::
tuple_storage<dango::detail::tuple_pack<tp_first, tp_next...>>:
public dango::detail::tuple_storage<dango::detail::tuple_pack<tp_next...>>
{
private:
  using super_type = dango::detail::tuple_storage<dango::detail::tuple_pack<tp_next...>>;
  using storage_type = detail::tuple_storage_type<tp_first>;
public:
  using value_type = detail::tuple_value_type<tp_first>;
public:
  template
  <typename tp_arg, typename... tp_args>
  constexpr
  tuple_storage
  (tp_arg&& a_arg, tp_args&&... a_args)noexcept:
  super_type{ dango::forward<tp_args>(a_args)... },
  m_value{ dango::forward<tp_arg>(a_arg) }
  {

  }
private:
  storage_type m_value;
public:
  DANGO_DELETE_DEFAULT(tuple_storage)
};

namespace
dango::detail
{
  template
  <typename tp_pack, typename... tp_types>
  struct make_tuple_storage_help;

  template
  <typename tp_pack>
  struct make_tuple_storage_help<tp_pack>;

  template
  <typename tp_pack, typename tp_first, typename... tp_next>
  struct make_tuple_storage_help<tp_pack, tp_first, tp_next...>;

  template
  <typename tp_pack, typename... tp_args>
  constexpr auto
  make_tuple_storage
  (tp_args&&... a_args)noexcept->decltype(auto)
  {
    return detail::make_tuple_storage_help<tp_pack, tp_args&&...>::make(dango::forward<tp_args>(a_args)...);
  }
}

template
<typename tp_pack>
struct
dango::
detail::
make_tuple_storage_help<tp_pack>
final
{
  template
  <typename... tp_args>
  static constexpr auto
  make
  (tp_args&&... a_args)noexcept->decltype(auto)
  {
    return detail::tuple_storage<tp_pack>{ dango::forward<tp_args>(a_args)... };
  }

  DANGO_UNINSTANTIABLE(make_tuple_storage_help)
};

template
<typename tp_pack, typename tp_first, typename... tp_next>
struct
dango::
detail::
make_tuple_storage_help<tp_pack, tp_first, tp_next...>
final
{
  template
  <typename... tp_args>
  static constexpr auto
  make
  (tp_first a_first, tp_next... a_next, tp_args&&... a_args)noexcept->decltype(auto)
  {
    return
      detail::make_tuple_storage_help<tp_pack, tp_next...>::make
      (
        dango::forward<tp_next>(a_next)...,
        dango::forward<tp_first>(a_first),
        dango::forward<tp_args>(a_args)...
      );
  }

  DANGO_UNINSTANTIABLE(make_tuple_storage_help)
};

using tuple_test = dango::detail::tuple_storage<dango::detail::tuple_pack_reverse<float, int, double>>;

static_assert(dango::is_same<tuple_test::value_type, double>);
static_assert(sizeof(tuple_test) == 16);
static_assert(alignof(tuple_test) == 8);

inline constexpr float const c_float_test = 1.4f;

inline constexpr auto const c_tuple_test =
  dango::detail::make_tuple_storage<dango::detail::tuple_pack_reverse<float, int, double>>(c_float_test, 42, 3.14);

static_assert(sizeof(c_tuple_test) == 16);

#endif

