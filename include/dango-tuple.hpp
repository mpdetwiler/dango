#ifndef __DANGO_TUPLE_HPP__
#define __DANGO_TUPLE_HPP__ 1

#include "dango-traits.hpp"
#include "dango-util.hpp"

namespace
dango::detail
{
  template
  <typename... tp_types>
  struct
  tuple_pack
  final
  {
    static constexpr auto
    size()noexcept->dango::usize
    {
      return sizeof...(tp_types);
    }

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
  using type = tp_type* const;

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
  using type = tp_type(&)[tp_size];

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
  using type = tp_ret(&)(tp_args...)noexcept(tp_noexcept);

  DANGO_UNINSTANTIABLE(tuple_value_type_help)
};

namespace
dango
{
  struct
  value_init_tag
  final
  {
    DANGO_TAG_TYPE(value_init_tag)
  };

  inline constexpr dango::value_init_tag const value_init{ };

  struct
  skip_init_tag
  final
  {
    DANGO_TAG_TYPE(skip_init_tag)
  };

  inline constexpr dango::skip_init_tag const skip_init{ };

  template
  <typename tp_type, typename tp_value_type = detail::tuple_value_type<tp_type>>
  constexpr bool const is_skip_constructible =
    dango::is_trivial_default_constructible<tp_value_type> ?
    !is_const<tp_value_type> :
    dango::is_default_constructible<tp_value_type>;

  template
  <typename tp_type, typename tp_value_type = detail::tuple_value_type<tp_type>>
  constexpr bool const is_noexcept_skip_constructible =
    dango::is_trivial_default_constructible<tp_value_type> ?
    !is_const<tp_value_type> :
    dango::is_noexcept_default_constructible<tp_value_type>;
}

namespace
dango::detail
{
  template
  <typename tp_arg, typename tp_tag = dango::remove_cvref<tp_arg>>
  constexpr bool const is_tuple_tag =
    dango::is_same<tp_tag, dango::value_init_tag> ||
    dango::is_same<tp_tag, dango::skip_init_tag>;

  template
  <
    typename tp_type,
    typename tp_arg,
    typename tp_value_type = detail::tuple_value_type<tp_type>,
    typename tp_tag = dango::remove_cvref<tp_arg>
  >
  constexpr bool const tuple_is_constructible =
    (dango::is_same<tp_tag, dango::value_init_tag> && dango::is_default_constructible<tp_value_type>) ||
    (dango::is_same<tp_tag, dango::skip_init_tag> && dango::is_skip_constructible<tp_type, tp_value_type>) ||
    dango::is_constructible<tp_value_type, tp_arg>;

  template
  <
    typename tp_type,
    typename tp_arg,
    typename tp_value_type = detail::tuple_value_type<tp_type>,
    typename tp_tag = dango::remove_cvref<tp_arg>
  >
  constexpr bool const tuple_is_noexcept_constructible =
    (dango::is_same<tp_tag, dango::value_init_tag> && dango::is_noexcept_default_constructible<tp_value_type>) ||
    (dango::is_same<tp_tag, dango::skip_init_tag> && dango::is_noexcept_skip_constructible<tp_type, tp_value_type>) ||
    dango::is_noexcept_constructible<tp_value_type, tp_arg>;

  template
  <
    typename tp_arg,
    typename tp_type,
    typename tp_tag = dango::remove_cvref<tp_arg>,
    typename tp_value_type = detail::tuple_value_type<tp_type>
  >
  constexpr bool const tuple_is_convertible =
    (dango::is_same<tp_tag, dango::value_init_tag> && dango::is_default_constructible<tp_value_type>) ||
    (dango::is_same<tp_tag, dango::skip_init_tag> && dango::is_skip_constructible<tp_type, tp_value_type>) ||
    dango::is_convertible<tp_arg, tp_value_type>;
}

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
  static_assert(dango::is_same<value_type, tp_first>);
  static_assert(dango::is_noexcept_destructible<value_type>);
public:
  template
  <
    typename tp_value_type = value_type,
    dango::enable_if<dango::is_default_constructible<tp_value_type>> = dango::enable_val
  >
  constexpr
  tuple_storage()
  noexcept(dango::is_noexcept_default_constructible<value_type>):
  m_value{ }
  {

  }

  template
  <
    typename tp_arg,
    dango::enable_if
    <
      !detail::is_tuple_tag<tp_arg> &&
      dango::is_constructible<value_type, tp_arg>
    > = dango::enable_val
  >
  explicit constexpr
  tuple_storage
  (tp_arg&& a_arg)
  noexcept(dango::is_noexcept_constructible<value_type, tp_arg>):
  m_value(dango::forward<tp_arg>(a_arg))
  {

  }

  template
  <
    typename tp_value_type = value_type,
    dango::enable_if<dango::is_default_constructible<tp_value_type>> = dango::enable_val
  >
  explicit constexpr
  tuple_storage
  (dango::value_init_tag const)
  noexcept(dango::is_noexcept_default_constructible<value_type>):
  m_value{ }
  {

  }

  template
  <
    typename tp_value_type = value_type,
    dango::enable_if<dango::is_skip_constructible<tp_value_type>> = dango::enable_val
  >
  explicit constexpr
  tuple_storage
  (dango::skip_init_tag const)
  noexcept(dango::is_noexcept_skip_constructible<value_type>)
  {

  }

  constexpr
  tuple_storage
  (tuple_storage const&)
  noexcept(dango::is_noexcept_copy_constructible<value_type>) = default;

  constexpr
  tuple_storage
  (tuple_storage&&)
  noexcept(dango::is_noexcept_move_constructible<value_type>) = default;

  ~tuple_storage()noexcept = default;

  constexpr auto
  operator = (tuple_storage const&)&
  noexcept(dango::is_noexcept_copy_assignable<value_type>)->tuple_storage& = default;

  constexpr auto
  operator = (tuple_storage&&)&
  noexcept(dango::is_noexcept_move_assignable<value_type>)->tuple_storage& = default;

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
};

template
<typename tp_first, typename... tp_next>
class
dango::
detail::
tuple_storage<dango::detail::tuple_pack<tp_first, tp_next...>>:
public dango::detail::tuple_storage<dango::detail::tuple_pack<tp_next...>>
{
public:
  using value_type = detail::tuple_value_type<tp_first>;
  static_assert(dango::is_same<value_type, tp_first>);
  static_assert(dango::is_noexcept_destructible<value_type>);
private:
  using super_type = dango::detail::tuple_storage<dango::detail::tuple_pack<tp_next...>>;
public:
  template
  <
    typename tp_value_type = value_type,
    dango::enable_if
    <
      dango::is_default_constructible<tp_value_type> &&
      dango::is_default_constructible<super_type>
    > = dango::enable_val
  >
  constexpr
  tuple_storage()
  noexcept
  (
    dango::is_noexcept_default_constructible<value_type> &&
    dango::is_noexcept_default_constructible<super_type>
  ):
  super_type{ },
  m_value{ }
  {

  }

  template
  <
    typename tp_arg,
    typename... tp_args,
    dango::enable_if
    <
      !detail::is_tuple_tag<tp_arg> &&
      dango::is_constructible<value_type, tp_arg> &&
      dango::is_constructible<super_type, tp_args...>
    > = dango::enable_val
  >
  explicit constexpr
  tuple_storage
  (tp_arg&& a_arg, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_constructible<value_type, tp_arg> &&
    dango::is_noexcept_constructible<super_type, tp_args...>
  ):
  super_type(dango::forward<tp_args>(a_args)...),
  m_value(dango::forward<tp_arg>(a_arg))
  {

  }

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_default_constructible<value_type> &&
      dango::is_constructible<super_type, tp_args...>
    > = dango::enable_val
  >
  explicit constexpr
  tuple_storage
  (dango::value_init_tag const, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_default_constructible<value_type> &&
    dango::is_noexcept_constructible<super_type, tp_args...>
  ):
  super_type(dango::forward<tp_args>(a_args)...),
  m_value{ }
  {

  }

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_skip_constructible<value_type> &&
      dango::is_constructible<super_type, tp_args...>
    > = dango::enable_val
  >
  explicit constexpr
  tuple_storage
  (dango::skip_init_tag const, tp_args&&... a_args)
  noexcept
  (
    dango::is_noexcept_skip_constructible<value_type> &&
    dango::is_noexcept_constructible<super_type, tp_args...>
  ):
  super_type(dango::forward<tp_args>(a_args)...)
  {

  }

  constexpr
  tuple_storage
  (tuple_storage const&)
  noexcept
  (
    dango::is_noexcept_copy_constructible<value_type> &&
    dango::is_noexcept_copy_constructible<super_type>
  ) = default;

  constexpr
  tuple_storage
  (tuple_storage&&)
  noexcept
  (
    dango::is_noexcept_move_constructible<value_type> &&
    dango::is_noexcept_move_constructible<super_type>
  ) = default;

  ~tuple_storage()noexcept = default;

  constexpr auto
  operator = (tuple_storage const&)&
  noexcept
  (
    dango::is_noexcept_copy_assignable<value_type> &&
    dango::is_noexcept_copy_assignable<super_type>
  )->tuple_storage& = default;

  constexpr auto
  operator = (tuple_storage&&)&
  noexcept
  (
    dango::is_noexcept_move_assignable<value_type> &&
    dango::is_noexcept_move_assignable<super_type>
  )->tuple_storage& = default;

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
    typename tp_make = dango::detail::make_tuple_storage_help<tp_pack, tp_args&&...>,
    typename tp_ret = decltype(tp_make::make(dango::declval<tp_args>()...))
  >
  constexpr auto
  make_tuple_storage
  (tp_args&&... a_args)
  noexcept(noexcept(tp_make::make(dango::declval<tp_args>()...)))->tp_ret
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
  <
    typename... tp_args,
    dango::enable_if<dango::is_constructible<ret_type, tp_args...>> = dango::enable_val
  >
  static constexpr auto
  make
  (tp_args&&... a_args)
  noexcept(dango::is_noexcept_constructible<ret_type, tp_args...>)->ret_type
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

#define DANGO_MAKE_TUPLE_SPEC \
  make_type::make(dango::declval<tp_next>()..., dango::declval<tp_first>(), dango::declval<tp_args>()...)


  template
  <
    typename... tp_args,
    typename tp_ret =
      decltype(DANGO_MAKE_TUPLE_SPEC)
  >
  static constexpr auto
  make
  (tp_first a_first, tp_next... a_next, tp_args&&... a_args)
  noexcept(noexcept(DANGO_MAKE_TUPLE_SPEC))->tp_ret
  {
    return make_type::make(dango::forward<tp_next>(a_next)..., dango::forward<tp_first>(a_first), dango::forward<tp_args>(a_args)...);
  }

#undef DANGO_MAKE_TUPLE_SPEC

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
  static_assert
  (
    (... && dango::is_noexcept_destructible<detail::tuple_value_type<tp_types>>),
    "dango::tuple only supports noexcept-destructible types"
  );
private:
  using pack_type = detail::tuple_pack_reverse<detail::tuple_value_type<tp_types>...>;
  using storage_type = detail::tuple_storage<pack_type>;
  static_assert(dango::is_noexcept_destructible<storage_type>);
public:
  template
  <
    typename tp_storage_type = storage_type,
    dango::enable_if<dango::is_default_constructible<tp_storage_type>> = dango::enable_val
  >
  constexpr
  tuple()
  noexcept(dango::is_noexcept_default_constructible<storage_type>):
  m_storage{ }
  {

  }

#define DANGO_TUPLE_NOEXCEPT_SPEC \
  (... && detail::tuple_is_noexcept_constructible<tp_types, tp_args>)

#define DANGO_TUPLE_MAKE_SPEC \
  noexcept(detail::make_tuple_storage<pack_type>(dango::declval<tp_args>()...))

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_args), sizeof...(tp_types)) &&
      !(... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, tp_args>) &&
      (... && detail::tuple_is_convertible<tp_args, tp_types>)
    > = dango::enable_val
  >
  constexpr
  tuple(tp_args&&... a_args)
  noexcept(DANGO_TUPLE_NOEXCEPT_SPEC):
  m_storage(detail::make_tuple_storage<pack_type>(dango::forward<tp_args>(a_args)...))
  {
#ifndef DANGO_NO_DEBUG
    static_assert(dango::logical_equivalence(DANGO_TUPLE_NOEXCEPT_SPEC, DANGO_TUPLE_MAKE_SPEC));
#endif
  }

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_args), sizeof...(tp_types)) &&
      !(... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, tp_args>) &&
      !(... && detail::tuple_is_convertible<tp_args, tp_types>)
    > = dango::enable_val
  >
  explicit constexpr
  tuple(tp_args&&... a_args)
  noexcept(DANGO_TUPLE_NOEXCEPT_SPEC):
  m_storage(detail::make_tuple_storage<pack_type>(dango::forward<tp_args>(a_args)...))
  {
#ifndef DANGO_NO_DEBUG
    static_assert(dango::logical_equivalence(DANGO_TUPLE_NOEXCEPT_SPEC, DANGO_TUPLE_MAKE_SPEC));
#endif
  }

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_args), sizeof...(tp_types)) &&
      (... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, tp_args>)
    > = dango::enable_val
  >
  constexpr
  tuple(tp_args&&... a_args)
  noexcept(DANGO_TUPLE_NOEXCEPT_SPEC):
  m_storage(detail::make_tuple_storage<pack_type>(dango::forward<tp_args>(a_args)...))
  {
#ifndef DANGO_NO_DEBUG
    static_assert(dango::logical_equivalence(DANGO_TUPLE_NOEXCEPT_SPEC, DANGO_TUPLE_MAKE_SPEC));
#endif
  }

#undef DANGO_TUPLE_MAKE_SPEC

#undef DANGO_TUPLE_NOEXCEPT_SPEC

  constexpr
  tuple(tuple const&)noexcept(dango::is_noexcept_copy_constructible<storage_type>) = default;

  constexpr
  tuple(tuple&&)noexcept(dango::is_noexcept_move_constructible<storage_type>) = default;

  ~tuple()noexcept = default;

  constexpr auto
  operator = (tuple const&)&
  noexcept(dango::is_noexcept_copy_assignable<storage_type>)->tuple& = default;

  constexpr auto
  operator = (tuple&&)&
  noexcept(dango::is_noexcept_move_assignable<storage_type>)->tuple& = default;

  template
  <
    dango::usize tp_index,
    dango::enable_if<dango::is_lesser(tp_index, pack_type::size())> = dango::enable_val
  >
  constexpr auto
  get()noexcept->decltype(auto)
  {
    return m_storage.template get<pack_type::size() - dango::usize(1) - tp_index>();
  }

  template
  <
    dango::usize tp_index,
    dango::enable_if<dango::is_lesser(tp_index, pack_type::size())> = dango::enable_val
  >
  constexpr auto
  get()const noexcept->decltype(auto)
  {
    return m_storage.template get<pack_type::size() - dango::usize(1) - tp_index>();
  }
public:
  template
  <
    typename tp_arg,
    typename tp_elem = decltype(dango::declval<tuple<tp_arg> const&>().template get<0>()),
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      !detail::tuple_is_constructible<tp_types..., tuple<tp_arg> const&> &&
      !detail::tuple_is_convertible<tuple<tp_arg> const&, tp_types...> &&
      !dango::is_same<tp_types..., tp_arg> &&
      detail::tuple_is_constructible<tp_types..., tp_elem> &&
      detail::tuple_is_convertible<tp_elem, tp_types...>
    > = dango::enable_val
  >
  constexpr
  tuple(tuple<tp_arg> const& a_tup)
  noexcept(detail::tuple_is_noexcept_constructible<tp_types..., tp_elem>):
  m_storage(dango::forward<tp_elem>(a_tup.template get<0>()))
  {

  }

  template
  <
    typename tp_arg,
    typename tp_elem = decltype(dango::declval<tuple<tp_arg> const&>().template get<0>()),
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      !detail::tuple_is_constructible<tp_types..., tuple<tp_arg> const&> &&
      !detail::tuple_is_convertible<tuple<tp_arg> const&, tp_types...> &&
      !dango::is_same<tp_types..., tp_arg> &&
      detail::tuple_is_constructible<tp_types..., tp_elem> &&
      !detail::tuple_is_convertible<tp_elem, tp_types...>
    > = dango::enable_val
  >
  explicit constexpr
  tuple(tuple<tp_arg> const& a_tup)
  noexcept(detail::tuple_is_noexcept_constructible<tp_types..., tp_elem>):
  m_storage(dango::forward<tp_elem>(a_tup.template get<0>()))
  {

  }
private:
  storage_type m_storage;
};

inline constexpr dango::tuple<int, float, double, char const[4]> const c_tup{ dango::value_init, dango::value_init, 3, "hey"};

static_assert(c_tup.get<3>()[0] == 'h');
static_assert(c_tup.get<3>()[1] == 'e');
static_assert(c_tup.get<3>()[2] == 'y');
static_assert(c_tup.get<3>()[3] == '\0');

#endif

