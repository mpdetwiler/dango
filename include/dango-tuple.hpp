#ifndef __DANGO_TUPLE_HPP__
#define __DANGO_TUPLE_HPP__ 1

#include "dango-traits.hpp"
#include "dango-util.hpp"
#include "dango-mem.hpp"

namespace
dango::detail
{
  template
  <typename... tp_types>
  struct
  tuple_pack
  final
  {
    static inline constexpr dango::usize const c_size = sizeof...(tp_types);

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
public:
  using value_type = detail::tuple_value_type<tp_first>;
public:
  template
  <
    typename tp_arg,
    dango::enable_if<dango::is_constructible<value_type, tp_arg>> = dango::enable_val
  >
  explicit constexpr
  tuple_storage
  (tp_arg&& a_arg)
  noexcept(dango::is_noexcept_placement_constructible<value_type, tp_arg>):
  m_value(dango::forward<tp_arg>(a_arg))
  {

  }

  tuple_storage
  (tuple_storage const&)
  noexcept(dango::is_noexcept_copy_constructible<value_type>) = default;

  tuple_storage
  (tuple_storage&&)
  noexcept(dango::is_noexcept_move_constructible<value_type>) = default;

  ~tuple_storage()noexcept(dango::is_noexcept_destructible<value_type>) = default;

  template
  <
    dango::usize tp_index,
    dango::enable_if
    <
      dango::is_equal(tp_index, dango::usize(0)) &&
      dango::is_ref<value_type>
    > = dango::enable_val
  >
  constexpr auto
  get()const noexcept->value_type
  {
    return dango::forward<value_type>(m_value);
  }

  template
  <
    dango::usize tp_index,
    dango::enable_if
    <
      dango::is_equal(tp_index, dango::usize(0)) &&
      !dango::is_ref<value_type> &&
      !dango::is_const<value_type>
    > = dango::enable_val
  >
  constexpr auto
  get()noexcept->value_type&
  {
    return m_value;
  }

  template
  <
    dango::usize tp_index,
    dango::enable_if
    <
      dango::is_equal(tp_index, dango::usize(0)) &&
      !dango::is_ref<value_type> &&
      !dango::is_const<value_type>
    > = dango::enable_val
  >
  constexpr auto
  get()const noexcept->value_type const&
  {
    return m_value;
  }

  template
  <
    dango::usize tp_index,
    dango::enable_if
    <
      dango::is_equal(tp_index, dango::usize(0)) &&
      !dango::is_ref<value_type> &&
      dango::is_const<value_type>
    > = dango::enable_val
  >
  constexpr auto
  get()const noexcept->value_type&
  {
    return m_value;
  }
private:
  value_type m_value;
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
public:
  using value_type = detail::tuple_value_type<tp_first>;
public:
  template
  <
    typename tp_arg,
    typename... tp_args,
    dango::enable_if
    <
      dango::is_constructible<value_type, tp_arg> &&
      dango::is_constructible<super_type, tp_args...>
    > = dango::enable_val
  >
  explicit constexpr
  tuple_storage
  (tp_arg&& a_arg, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_placement_constructible<value_type, tp_arg> &&
    dango::is_noexcept_placement_constructible<super_type, tp_args...>
  ):
  super_type(dango::forward<tp_args>(a_args)...),
  m_value(dango::forward<tp_arg>(a_arg))
  {

  }

  tuple_storage
  (tuple_storage const&)
  noexcept
  (
    dango::is_noexcept_copy_constructible<value_type> &&
    dango::is_noexcept_copy_constructible<super_type>
  ) = default;

  tuple_storage
  (tuple_storage&&)
  noexcept
  (
    dango::is_noexcept_move_constructible<value_type> &&
    dango::is_noexcept_move_constructible<super_type>
  ) = default;

  ~tuple_storage()noexcept(dango::is_noexcept_destructible<value_type>) = default;

  template
  <
    dango::usize tp_index,
    dango::enable_if
    <
      dango::is_equal(tp_index, dango::usize(0)) &&
      dango::is_ref<value_type>
    > = dango::enable_val
  >
  constexpr auto
  get()const noexcept->value_type
  {
    return dango::forward<value_type>(m_value);
  }

  template
  <
    dango::usize tp_index,
    dango::enable_if
    <
      dango::is_equal(tp_index, dango::usize(0)) &&
      !dango::is_ref<value_type> &&
      !dango::is_const<value_type>
    > = dango::enable_val
  >
  constexpr auto
  get()noexcept->value_type&
  {
    return m_value;
  }

  template
  <
    dango::usize tp_index,
    dango::enable_if
    <
      dango::is_equal(tp_index, dango::usize(0)) &&
      !dango::is_ref<value_type> &&
      !dango::is_const<value_type>
    > = dango::enable_val
  >
  constexpr auto
  get()const noexcept->value_type const&
  {
    return m_value;
  }

  template
  <
    dango::usize tp_index,
    dango::enable_if
    <
      dango::is_equal(tp_index, dango::usize(0)) &&
      !dango::is_ref<value_type> &&
      dango::is_const<value_type>
    > = dango::enable_val
  >
  constexpr auto
  get()const noexcept->value_type&
  {
    return m_value;
  }

  template
  <
    dango::usize tp_index,
    dango::enable_if<!dango::is_equal(tp_index, dango::usize(0))> = dango::enable_val
  >
  constexpr auto
  get()noexcept->decltype(auto)
  {
    return super_type::template get<tp_index - dango::usize(1)>();
  }

  template
  <
    dango::usize tp_index,
    dango::enable_if<!dango::is_equal(tp_index, dango::usize(0))> = dango::enable_val
  >
  constexpr auto
  get()const noexcept->decltype(auto)
  {
    return super_type::template get<tp_index - dango::usize(1)>();
  }
private:
  value_type m_value;
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
  <
    typename tp_pack,
    typename... tp_args,
    typename tp_make = dango::detail::make_tuple_storage_help<tp_pack, tp_args&&...>
  >
  constexpr auto
  make_tuple_storage
  (tp_args&&... a_args)
  noexcept(noexcept(tp_make::make(dango::declval<tp_args>()...)))->auto
  {
    return tp_make::make(dango::forward<tp_args>(a_args)...);
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
  using ret_type = dango::detail::tuple_storage<tp_pack>;

  template
  <typename... tp_args>
  static constexpr auto
  make
  (tp_args&&... a_args)
  noexcept(dango::is_noexcept_placement_constructible<ret_type, tp_args...>)->ret_type
  {
    return ret_type{ dango::forward<tp_args>(a_args)... };
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
  static_assert((dango::is_ref<tp_first> && ... && dango::is_ref<tp_next>));

  using make_type = detail::make_tuple_storage_help<tp_pack, tp_next...>;

  template
  <typename... tp_args>
  static constexpr auto
  make
  (tp_first a_first, tp_next... a_next, tp_args&&... a_args)
  noexcept
  (
    noexcept
    (
      ::new (dango::placement, nullptr) auto{ make_type::make(dango::declval<tp_next>()..., dango::declval<tp_first>(), dango::declval<tp_args>()...) }
    )
  )->auto
  {
    return make_type::make(dango::forward<tp_next>(a_next)..., dango::forward<tp_first>(a_first), dango::forward<tp_args>(a_args)...);
  }

  DANGO_UNINSTANTIABLE(make_tuple_storage_help)
};

/*** tuple ***/

namespace
dango
{
  template
  <typename... tp_types>
  class tuple;

  template<>
  class tuple<>;
}

template
<typename... tp_types>
class
dango::
tuple
final
{
private:
  using pack_type = detail::tuple_pack_reverse<tp_types...>;
  using storage_type = detail::tuple_storage<pack_type>;
public:
  template
  <
    typename... tp_args,
    typename tp_enabled = decltype(detail::make_tuple_storage<pack_type>(dango::declval<tp_args>()...)),
    dango::enable_if<dango::is_equal(sizeof...(tp_args), pack_type::c_size)> = dango::enable_val
  >
  constexpr
  tuple(tp_args&&... a_args)
  noexcept(noexcept(detail::make_tuple_storage<pack_type>(dango::declval<tp_args>()...))):
  m_storage(detail::make_tuple_storage<pack_type>(dango::forward<tp_args>(a_args)...))
  {

  }

  template
  <
    dango::usize tp_index,
    dango::enable_if<dango::is_lesser(tp_index, pack_type::c_size)> = dango::enable_val
  >
  constexpr auto
  get()noexcept->decltype(auto)
  {
    return m_storage.template get<pack_type::c_size - dango::usize(1) - tp_index>();
  }

  template
  <
    dango::usize tp_index,
    dango::enable_if<dango::is_lesser(tp_index, pack_type::c_size)> = dango::enable_val
  >
  constexpr auto
  get()const noexcept->decltype(auto)
  {
    return m_storage.template get<pack_type::c_size - dango::usize(1) - tp_index>();
  }
private:
  storage_type m_storage;
};


inline constexpr dango::tuple<int, float, double> const c_tup{ 1, 2, 3 };

#endif

