#ifndef __DANGO_TUPLE_HPP__
#define __DANGO_TUPLE_HPP__ 1

#include "dango-traits.hpp"
#include "dango-util.hpp"

namespace
dango::detail
{
  struct
  tuple_normal_test_tag
  final
  {
    DANGO_TAG_TYPE(tuple_normal_test_tag)
  };

  inline constexpr detail::tuple_normal_test_tag const tuple_normal_test{ };

  struct
  tuple_noexcept_test_tag
  final
  {
    DANGO_TAG_TYPE(tuple_noexcept_test_tag)
  };

  inline constexpr detail::tuple_noexcept_test_tag const tuple_noexcept_test{ };
}

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
}

namespace
dango::detail
{
  template
  <typename tp_type, typename tp_value_type = detail::tuple_value_type<tp_type>>
  constexpr bool const is_skip_constructible_help =
    dango::is_trivial_default_constructible<tp_value_type> ?
    !is_const<tp_value_type> :
    dango::is_default_constructible<tp_value_type>;

  template
  <typename tp_type, typename tp_value_type = detail::tuple_value_type<tp_type>>
  constexpr bool const is_noexcept_skip_constructible_help =
    dango::is_trivial_default_constructible<tp_value_type> ?
    !is_const<tp_value_type> :
    dango::is_noexcept_default_constructible<tp_value_type>;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_skip_constructible = detail::is_skip_constructible_help<tp_type>;

  template
  <typename tp_type>
  constexpr bool const is_noexcept_skip_constructible = detail::is_noexcept_skip_constructible_help<tp_type>;
}

namespace
dango::detail
{
  template
  <typename tp_arg, typename tp_tag = dango::remove_cvref<tp_arg>>
  constexpr bool const is_tuple_tag_help =
    dango::is_same<tp_tag, dango::value_init_tag> ||
    dango::is_same<tp_tag, dango::skip_init_tag>;

  template
  <typename tp_arg>
  constexpr bool const is_tuple_tag = detail::is_tuple_tag_help<tp_arg>;

  template
  <
    typename tp_type,
    typename tp_arg,
    typename tp_value_type = detail::tuple_value_type<tp_type>,
    typename tp_tag = dango::remove_cvref<tp_arg>
  >
  constexpr bool const tuple_is_constructible_help =
    (dango::is_same<tp_tag, dango::value_init_tag> && dango::is_default_constructible<tp_value_type>) ||
    (dango::is_same<tp_tag, dango::skip_init_tag> && detail::is_skip_constructible_help<tp_type, tp_value_type>) ||
    dango::is_constructible<tp_value_type, tp_arg>;

  template
  <typename tp_type, typename tp_arg>
  constexpr bool const tuple_is_constructible = detail::tuple_is_constructible_help<tp_type, tp_arg>;

  template
  <
    typename tp_type,
    typename tp_arg,
    typename tp_value_type = detail::tuple_value_type<tp_type>,
    typename tp_tag = dango::remove_cvref<tp_arg>
  >
  constexpr bool const tuple_is_noexcept_constructible_help =
    (dango::is_same<tp_tag, dango::value_init_tag> && dango::is_noexcept_default_constructible<tp_value_type>) ||
    (dango::is_same<tp_tag, dango::skip_init_tag> && detail::is_noexcept_skip_constructible_help<tp_type, tp_value_type>) ||
    dango::is_noexcept_constructible<tp_value_type, tp_arg>;

  template
  <typename tp_type, typename tp_arg>
  constexpr bool const tuple_is_noexcept_constructible = detail::tuple_is_noexcept_constructible_help<tp_type, tp_arg>;

  template
  <
    typename tp_arg,
    typename tp_type,
    typename tp_tag = dango::remove_cvref<tp_arg>,
    typename tp_value_type = detail::tuple_value_type<tp_type>
  >
  constexpr bool const tuple_is_convertible_help =
    (dango::is_same<tp_tag, dango::value_init_tag> && dango::is_default_constructible<tp_value_type>) ||
    (dango::is_same<tp_tag, dango::skip_init_tag> && detail::is_skip_constructible_help<tp_type, tp_value_type>) ||
    dango::is_convertible<tp_arg, tp_value_type>;

  template
  <typename tp_arg, typename tp_type>
  constexpr bool const tuple_is_convertible = detail::tuple_is_convertible_help<tp_arg, tp_type>;
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
  <typename tp_storage_type, typename... tp_types>
  struct make_tuple_storage_help;

  template
  <typename tp_storage_type>
  struct make_tuple_storage_help<tp_storage_type>;

  template
  <typename tp_storage_type, typename tp_first, typename... tp_next>
  struct make_tuple_storage_help<tp_storage_type, tp_first, tp_next...>;

  template
  <
    typename tp_storage_type,
    typename... tp_args,
    typename tp_make = dango::detail::make_tuple_storage_help<tp_storage_type, tp_args&&...>,
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
<typename tp_storage_type>
struct
dango::
detail::
make_tuple_storage_help<tp_storage_type>
final
{
  using ret_type = tp_storage_type;

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
<typename tp_storage_type, typename tp_first, typename... tp_next>
struct
dango::
detail::
make_tuple_storage_help<tp_storage_type, tp_first, tp_next...>
final
{
  static_assert((dango::is_ref<tp_first> && ... && dango::is_ref<tp_next>));

  using make_type = detail::make_tuple_storage_help<tp_storage_type, tp_next...>;

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

#include <cstdio>

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
  template
  <typename... tp_tp_types>
  friend class dango::tuple;

  static_assert
  (
    (... && dango::is_noexcept_destructible<detail::tuple_value_type<tp_types>>),
    "dango::tuple only supports noexcept-destructible types"
  );
private:
  using storage_type =
    detail::tuple_storage<detail::tuple_pack_reverse<detail::tuple_value_type<tp_types>...>>;
  static_assert(dango::is_noexcept_destructible<storage_type>);
public:
  static constexpr auto
  size
  ()noexcept->dango::usize
  {
    return sizeof...(tp_types);
  }
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
  noexcept(detail::make_tuple_storage<storage_type>(dango::declval<tp_args>()...))

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !(... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, tp_args>) &&
      (... && detail::tuple_is_convertible<tp_args, tp_types>)
    > = dango::enable_val
  >
  constexpr
  tuple(tp_args&&... a_args)
  noexcept(DANGO_TUPLE_NOEXCEPT_SPEC):
  m_storage(detail::make_tuple_storage<storage_type>(dango::forward<tp_args>(a_args)...))
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
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !(... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, tp_args>) &&
      !(... && detail::tuple_is_convertible<tp_args, tp_types>)
    > = dango::enable_val
  >
  explicit constexpr
  tuple(tp_args&&... a_args)
  noexcept(DANGO_TUPLE_NOEXCEPT_SPEC):
  m_storage(detail::make_tuple_storage<storage_type>(dango::forward<tp_args>(a_args)...))
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
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      (... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, tp_args>)
    > = dango::enable_val
  >
  constexpr
  tuple(tp_args&&... a_args)
  noexcept(DANGO_TUPLE_NOEXCEPT_SPEC):
  m_storage(detail::make_tuple_storage<storage_type>(dango::forward<tp_args>(a_args)...))
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
    dango::enable_if<dango::is_lesser(tp_index, sizeof...(tp_types))> = dango::enable_val
  >
  constexpr auto
  get()noexcept->decltype(auto)
  {
    return m_storage.template get<sizeof...(tp_types) - dango::usize(1) - tp_index>();
  }

  template
  <
    dango::usize tp_index,
    dango::enable_if<dango::is_lesser(tp_index, sizeof...(tp_types))> = dango::enable_val
  >
  constexpr auto
  get()const noexcept->decltype(auto)
  {
    return m_storage.template get<sizeof...(tp_types) - dango::usize(1) - tp_index>();
  }

  constexpr auto
  first
  ()noexcept->decltype(auto)
  {
    return get<dango::usize(0)>();
  }

  template
  <
    dango::usize tp_tuple_size = sizeof...(tp_types),
    dango::enable_if<dango::is_gequal(tp_tuple_size, dango::usize(2))> = dango::enable_val
  >
  constexpr auto
  second
  ()noexcept->decltype(auto)
  {
    return get<dango::usize(1)>();
  }

  template
  <
    dango::usize tp_tuple_size = sizeof...(tp_types),
    dango::enable_if<dango::is_gequal(tp_tuple_size, dango::usize(3))> = dango::enable_val
  >
  constexpr auto
  third
  ()noexcept->decltype(auto)
  {
    return get<dango::usize(2)>();
  }

private:

/*** converting constructor helpers ***/

  template
  <
    bool tp_explicit,
    dango::usize... tp_indices,
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...> const&,
    dango::enable_if
    <
      !tp_explicit &&
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_indices)) &&
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !(... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, decltype(dango::declval<tp_tuple_arg>().template get<tp_indices>())>) &&
      (... && detail::tuple_is_convertible<decltype(dango::declval<tp_tuple_arg>().template get<tp_indices>()), tp_types>)
    > = dango::enable_val
  >
  static constexpr auto
  construct
  (dango::index_seq<tp_indices...> const, tuple<tp_args...> const& a_tup)
  noexcept
  (
    (... && detail::tuple_is_noexcept_constructible<tp_types, decltype(dango::declval<tp_tuple_arg>().template get<tp_indices>())>)
  )->storage_type
  {
    return storage_type{ a_tup.m_storage.template get<tp_indices>()... };
  }

  template
  <
    bool tp_explicit,
    dango::usize... tp_indices,
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...> const&,
    dango::enable_if
    <
      tp_explicit &&
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_indices)) &&
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !(... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, decltype(dango::declval<tp_tuple_arg>().template get<tp_indices>())>) &&
      !(... && detail::tuple_is_convertible<decltype(dango::declval<tp_tuple_arg>().template get<tp_indices>()), tp_types>)
    > = dango::enable_val
  >
  static constexpr auto
  construct
  (dango::index_seq<tp_indices...> const, tuple<tp_args...> const& a_tup)
  noexcept
  (
    (... && detail::tuple_is_noexcept_constructible<tp_types, decltype(dango::declval<tp_tuple_arg>().template get<tp_indices>())>)
  )->storage_type
  {
    return storage_type{ a_tup.m_storage.template get<tp_indices>()... };
  }

  template
  <
    bool tp_explicit,
    dango::usize... tp_indices,
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...> const&,
    dango::enable_if
    <
      !tp_explicit &&
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_indices)) &&
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      (... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, decltype(dango::declval<tp_tuple_arg>().template get<tp_indices>())>)
    > = dango::enable_val
  >
  static constexpr auto
  construct
  (dango::index_seq<tp_indices...> const, tuple<tp_args...> const& a_tup)
  noexcept
  (
    (... && detail::tuple_is_noexcept_constructible<tp_types, decltype(dango::declval<tp_tuple_arg>().template get<tp_indices>())>)
  )->storage_type
  {
    return storage_type{ a_tup.m_storage.template get<tp_indices>()... };
  }

  template
  <
    bool tp_explicit,
    dango::usize... tp_indices,
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...>&&,
    dango::enable_if
    <
      !tp_explicit &&
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_indices)) &&
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !(... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, decltype(dango::move(dango::declval<tp_tuple_arg>().template get<tp_indices>()))>) &&
      (... && detail::tuple_is_convertible<decltype(dango::move(dango::declval<tp_tuple_arg>().template get<tp_indices>())), tp_types>)
    > = dango::enable_val
  >
  static constexpr auto
  construct
  (dango::index_seq<tp_indices...> const, tuple<tp_args...>&& a_tup)
  noexcept
  (
    (... && detail::tuple_is_noexcept_constructible<tp_types, decltype(dango::move(dango::declval<tp_tuple_arg>().template get<tp_indices>()))>)
  )->storage_type
  {
    return storage_type{ dango::move(a_tup.m_storage.template get<tp_indices>())... };
  }

  template
  <
    bool tp_explicit,
    dango::usize... tp_indices,
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...>&&,
    dango::enable_if
    <
      tp_explicit &&
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_indices)) &&
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !(... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, decltype(dango::move(dango::declval<tp_tuple_arg>().template get<tp_indices>()))>) &&
      !(... && detail::tuple_is_convertible<decltype(dango::move(dango::declval<tp_tuple_arg>().template get<tp_indices>())), tp_types>)
    > = dango::enable_val
  >
  static constexpr auto
  construct
  (dango::index_seq<tp_indices...> const, tuple<tp_args...>&& a_tup)
  noexcept
  (
    (... && detail::tuple_is_noexcept_constructible<tp_types, decltype(dango::move(dango::declval<tp_tuple_arg>().template get<tp_indices>()))>)
  )->storage_type
  {
    return storage_type{ dango::move(a_tup.m_storage.template get<tp_indices>())... };
  }

  template
  <
    bool tp_explicit,
    dango::usize... tp_indices,
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...>&&,
    dango::enable_if
    <
      !tp_explicit &&
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_indices)) &&
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      (... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, decltype(dango::move(dango::declval<tp_tuple_arg>().template get<tp_indices>()))>)
    > = dango::enable_val
  >
  static constexpr auto
  construct
  (dango::index_seq<tp_indices...> const, tuple<tp_args...>&& a_tup)
  noexcept
  (
    (... && detail::tuple_is_noexcept_constructible<tp_types, decltype(dango::move(dango::declval<tp_tuple_arg>().template get<tp_indices>()))>)
  )->storage_type
  {
    return storage_type{ dango::move(a_tup.m_storage.template get<tp_indices>())... };
  }

/*** converting construct test ***/

  template
  <
    bool tp_explicit,
    dango::usize tp_tuple_size,
    typename tp_tuple_arg,
    typename tp_enabled =
      decltype(construct<tp_explicit>(dango::make_index_seq<tp_tuple_size>{ }, dango::declval<tp_tuple_arg>()))
  >
  static constexpr auto
  construct_test
  (detail::tuple_normal_test_tag const)noexcept->bool
  {
    return true;
  }

  template
  <
    bool tp_explicit,
    dango::usize tp_tuple_size,
    typename tp_tuple_arg,
    typename tp_enabled =
      decltype(construct<tp_explicit>(dango::make_index_seq<tp_tuple_size>{ }, dango::declval<tp_tuple_arg>()))
  >
  static constexpr auto
  construct_test
  (detail::tuple_noexcept_test_tag const)noexcept->bool
  {
    return noexcept(construct<tp_explicit>(dango::make_index_seq<tp_tuple_size>{ }, dango::declval<tp_tuple_arg>()));
  }

  template
  <
    bool tp_explicit,
    dango::usize tp_tuple_size,
    typename tp_tuple_arg,
    typename tp_tag
  >
  static constexpr auto
  construct_test
  (tp_tag const)noexcept->bool
  {
    return false;
  }

public:

/*** converting-copy ***/

  template
  <
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...> const&,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      !dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      construct_test<false, sizeof...(tp_types), tp_tuple_arg>(detail::tuple_normal_test)
    > = dango::enable_val
  >
  constexpr
  tuple
  (tuple<tp_args...> const& a_tup)
  noexcept(construct_test<false, sizeof...(tp_types), tp_tuple_arg>(detail::tuple_noexcept_test)):
  m_storage(construct<false>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup))
  {

  }

  template
  <
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...> const&,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      (... && !detail::tuple_is_constructible<tp_types, tp_tuple_arg>) &&
      (... && !detail::tuple_is_convertible<tp_tuple_arg, tp_types>) &&
      construct_test<false, sizeof...(tp_types), tp_tuple_arg>(detail::tuple_normal_test)
    > = dango::enable_val
  >
  constexpr
  tuple
  (tuple<tp_args...> const& a_tup)
  noexcept(construct_test<false, sizeof...(tp_types), tp_tuple_arg>(detail::tuple_noexcept_test)):
  m_storage(construct<false>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup))
  {

  }

  template
  <
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...> const&,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      !dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      construct_test<true, sizeof...(tp_types), tp_tuple_arg>(detail::tuple_normal_test)
    > = dango::enable_val
  >
  explicit constexpr
  tuple
  (tuple<tp_args...> const& a_tup)
  noexcept(construct_test<true, sizeof...(tp_types), tp_tuple_arg>(detail::tuple_noexcept_test)):
  m_storage(construct<true>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup))
  {

  }

  template
  <
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...> const&,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      (... && !detail::tuple_is_constructible<tp_types, tp_tuple_arg>) &&
      (... && !detail::tuple_is_convertible<tp_tuple_arg, tp_types>) &&
      construct_test<true, sizeof...(tp_types), tp_tuple_arg>(detail::tuple_normal_test)
    > = dango::enable_val
  >
  explicit constexpr
  tuple
  (tuple<tp_args...> const& a_tup)
  noexcept(construct_test<true, sizeof...(tp_types), tp_tuple_arg>(detail::tuple_noexcept_test)):
  m_storage(construct<true>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup))
  {

  }

/*** converting-move ***/

  template
  <
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...>&&,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      !dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      construct_test<false, sizeof...(tp_types), tp_tuple_arg>(detail::tuple_normal_test)
    > = dango::enable_val
  >
  constexpr
  tuple
  (tuple<tp_args...>&& a_tup)
  noexcept(construct_test<false, sizeof...(tp_types), tp_tuple_arg>(detail::tuple_noexcept_test)):
  m_storage(construct<false>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_tuple_arg>(a_tup)))
  {

  }

  template
  <
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...>&&,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      (... && !detail::tuple_is_constructible<tp_types, tp_tuple_arg>) &&
      (... && !detail::tuple_is_convertible<tp_tuple_arg, tp_types>) &&
      construct_test<false, sizeof...(tp_types), tp_tuple_arg>(detail::tuple_normal_test)
    > = dango::enable_val
  >
  constexpr
  tuple
  (tuple<tp_args...>&& a_tup)
  noexcept(construct_test<false, sizeof...(tp_types), tp_tuple_arg>(detail::tuple_noexcept_test)):
  m_storage(construct<false>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_tuple_arg>(a_tup)))
  {

  }

  template
  <
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...>&&,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      !dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      construct_test<true, sizeof...(tp_types), tp_tuple_arg>(detail::tuple_normal_test)
    > = dango::enable_val
  >
  explicit constexpr
  tuple
  (tuple<tp_args...>&& a_tup)
  noexcept(construct_test<true, sizeof...(tp_types), tp_tuple_arg>(detail::tuple_noexcept_test)):
  m_storage(construct<true>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_tuple_arg>(a_tup)))
  {

  }

  template
  <
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...>&&,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      (... && !detail::tuple_is_constructible<tp_types, tp_tuple_arg>) &&
      (... && !detail::tuple_is_convertible<tp_tuple_arg, tp_types>) &&
      construct_test<true, sizeof...(tp_types), tp_tuple_arg>(detail::tuple_normal_test)
    > = dango::enable_val
  >
  explicit constexpr
  tuple
  (tuple<tp_args...>&& a_tup)
  noexcept(construct_test<true, sizeof...(tp_types), tp_tuple_arg>(detail::tuple_noexcept_test)):
  m_storage(construct<true>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_tuple_arg>(a_tup)))
  {

  }

private:

/*** private assign helpers ***/

  template
  <
    dango::usize tp_index,
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...> const&,
    dango::enable_if<!dango::is_equal(tp_index, dango::usize(0))> = dango::enable_val
  >
  constexpr auto
  copy_assign
  (tuple<tp_args...> const& a_tup)
  noexcept
  (
    dango::is_noexcept_assignable
    <
      decltype(dango::declval<tuple&>().m_storage.template get<tp_index>()),
      decltype(dango::declval<tp_tuple_arg>().m_storage.template get<tp_index>())
    > &&
    noexcept(dango::declval<tuple&>().template copy_assign<tp_index - dango::usize(1)>(dango::declval<tp_tuple_arg>()))
  )->tuple&
  {
    m_storage.template get<tp_index>() = a_tup.m_storage.template get<tp_index>();

    return copy_assign<tp_index - dango::usize(1)>(a_tup);
  }

  template
  <
    dango::usize tp_index,
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...> const&,
    dango::enable_if<dango::is_equal(tp_index, dango::usize(0))> = dango::enable_val
  >
  constexpr auto
  copy_assign
  (tuple<tp_args...> const& a_tup)
  noexcept
  (
    dango::is_noexcept_assignable
    <
      decltype(dango::declval<tuple&>().m_storage.template get<tp_index>()),
      decltype(dango::declval<tp_tuple_arg>().m_storage.template get<tp_index>())
    >
  )->tuple&
  {
    m_storage.template get<tp_index>() = a_tup.m_storage.template get<tp_index>();

    return *this;
  }

  template
  <
    dango::usize tp_index,
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...>&&,
    dango::enable_if<!dango::is_equal(tp_index, dango::usize(0))> = dango::enable_val
  >
  constexpr auto
  move_assign
  (tuple<tp_args...>&& a_tup)
  noexcept
  (
    dango::is_noexcept_assignable
    <
      decltype(dango::declval<tuple&>().m_storage.template get<tp_index>()),
      decltype(dango::move(dango::declval<tp_tuple_arg>().m_storage.template get<tp_index>()))
    > &&
    noexcept(dango::declval<tuple&>().template move_assign<tp_index - dango::usize(1)>(dango::declval<tp_tuple_arg>()))
  )->tuple&
  {
    m_storage.template get<tp_index>() = dango::move(a_tup.m_storage.template get<tp_index>());

    return move_assign<tp_index - dango::usize(1)>(dango::forward<tp_tuple_arg>(a_tup));
  }

  template
  <
    dango::usize tp_index,
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...>&&,
    dango::enable_if<dango::is_equal(tp_index, dango::usize(0))> = dango::enable_val
  >
  constexpr auto
  move_assign
  (tuple<tp_args...>&& a_tup)
  noexcept
  (
    dango::is_noexcept_assignable
    <
      decltype(dango::declval<tuple&>().m_storage.template get<tp_index>()),
      decltype(dango::move(dango::declval<tp_tuple_arg>().m_storage.template get<tp_index>()))
    >
  )->tuple&
  {
    m_storage.template get<tp_index>() = dango::move(a_tup.m_storage.template get<tp_index>());

    return *this;
  }

/*** private converting assign ***/

  template
  <
    dango::usize... tp_indices,
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...> const&,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_indices)) &&
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      (... && !dango::is_ref<detail::tuple_value_type<tp_types>>) &&
      (
        ... &&
        dango::is_assignable
        <
          decltype(dango::declval<tuple&>().template get<tp_indices>()),
          decltype(dango::declval<tp_tuple_arg>().template get<tp_indices>())
        >
      )
    > = dango::enable_val
  >
  constexpr auto
  assign
  (dango::index_seq<tp_indices...> const, tuple<tp_args...> const& a_tup)
  noexcept
  (
    (
      ... &&
      dango::is_noexcept_assignable
      <
        decltype(dango::declval<tuple&>().template get<tp_indices>()),
        decltype(dango::declval<tp_tuple_arg>().template get<tp_indices>())
      >
    )
  )->tuple&
  {
    return copy_assign<sizeof...(tp_types) - dango::usize(1)>(a_tup);
  }

  template
  <
    dango::usize... tp_indices,
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...>&&,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_indices)) &&
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      (... && !dango::is_ref<detail::tuple_value_type<tp_types>>) &&
      (
        ... &&
        dango::is_assignable
        <
          decltype(dango::declval<tuple&>().template get<tp_indices>()),
          decltype(dango::move(dango::declval<tp_tuple_arg>().template get<tp_indices>()))
        >
      )
    > = dango::enable_val
  >
  constexpr auto
  assign
  (dango::index_seq<tp_indices...> const, tuple<tp_args...>&& a_tup)
  noexcept
  (
    (
      ... &&
      dango::is_noexcept_assignable
      <
        decltype(dango::declval<tuple&>().template get<tp_indices>()),
        decltype(dango::move(dango::declval<tp_tuple_arg>().template get<tp_indices>()))
      >
    )
  )->tuple&
  {
    return move_assign<sizeof...(tp_types) - dango::usize(1)>(dango::forward<tp_tuple_arg>(a_tup));
  }

/*** converting assign test ***/

  template
  <
    dango::usize tp_tuple_size,
    typename tp_tuple_arg,
    typename tp_enabled =
      decltype(dango::declval<tuple&>().assign(dango::make_index_seq<tp_tuple_size>{ }, dango::declval<tp_tuple_arg>()))
  >
  static constexpr auto
  assign_test
  (detail::tuple_normal_test_tag const)noexcept->bool
  {
    return true;
  }

  template
  <
    dango::usize tp_tuple_size,
    typename tp_tuple_arg,
    typename tp_enabled =
      decltype(dango::declval<tuple&>().assign(dango::make_index_seq<tp_tuple_size>{ }, dango::declval<tp_tuple_arg>()))
  >
  static constexpr auto
  assign_test
  (detail::tuple_noexcept_test_tag const)noexcept->bool
  {
    return noexcept(dango::declval<tuple&>().assign(dango::make_index_seq<tp_tuple_size>{ }, dango::declval<tp_tuple_arg>()));
  }

  template
  <
    dango::usize tp_tuple_size,
    typename tp_tuple_arg,
    typename tp_tag
  >
  static constexpr auto
  assign_test
  (tp_tag const)noexcept->bool
  {
    return false;
  }

public:

/*** converting assign ***/

  template
  <
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...> const&,
    dango::enable_if
    <
      assign_test<sizeof...(tp_types), tp_tuple_arg>(detail::tuple_normal_test)
    > = dango::enable_val
  >
  constexpr auto
  operator =
  (tuple<tp_args...> const& a_tup)&
  noexcept(assign_test<sizeof...(tp_types), tp_tuple_arg>(detail::tuple_noexcept_test))->tuple&
  {
    return assign(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup);
  }

  template
  <
    typename... tp_args,
    typename tp_tuple_arg = tuple<tp_args...>&&,
    dango::enable_if
    <
      assign_test<sizeof...(tp_types), tp_tuple_arg>(detail::tuple_normal_test)
    > = dango::enable_val
  >
  constexpr auto
  operator =
  (tuple<tp_args...>&& a_tup)&
  noexcept(assign_test<sizeof...(tp_types), tp_tuple_arg>(detail::tuple_noexcept_test))->tuple&
  {
    return assign(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_tuple_arg>(a_tup));
  }

private:

/*** operator ->* helper ***/

  template
  <
    dango::usize... tp_indices,
    typename tp_func,
    typename tp_tuple_arg = tuple&,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_indices)) &&
      dango::is_callable
      <
        dango::remove_ref<tp_func>&,
        decltype(dango::declval<tp_tuple_arg>().template get<tp_indices>())...
      >
    > = dango::enable_val
  >
  constexpr auto
  call
  (dango::index_seq<tp_indices...> const, tp_func&& a_func)
  noexcept
  (
    dango::is_noexcept_callable
    <
      dango::remove_ref<tp_func>&,
      decltype(dango::declval<tp_tuple_arg>().template get<tp_indices>())...
    >
  )->decltype(auto)
  {
    return a_func(get<tp_indices>()...);
  }

  template
  <
    dango::usize... tp_indices,
    typename tp_func,
    typename tp_tuple_arg = tuple const&,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_indices)) &&
      dango::is_callable
      <
        dango::remove_ref<tp_func>&,
        decltype(dango::declval<tp_tuple_arg>().template get<tp_indices>())...
      >
    > = dango::enable_val
  >
  constexpr auto
  call
  (dango::index_seq<tp_indices...> const, tp_func&& a_func)const
  noexcept
  (
    dango::is_noexcept_callable
    <
      dango::remove_ref<tp_func>&,
      decltype(dango::declval<tp_tuple_arg>().template get<tp_indices>())...
    >
  )->decltype(auto)
  {
    return a_func(get<tp_indices>()...);
  }

/*** call test ***/

template
<
  dango::usize tp_tuple_size,
  typename tp_tuple_arg,
  typename tp_func,
  typename tp_enabled =
    decltype(dango::declval<tp_tuple_arg>().call(dango::make_index_seq<tp_tuple_size>{ }, dango::declval<tp_func>()))
>
static constexpr auto
call_test
(detail::tuple_normal_test_tag const)noexcept->bool
{
  return true;
}

template
<
  dango::usize tp_tuple_size,
  typename tp_tuple_arg,
  typename tp_func,
  typename tp_enabled =
    decltype(dango::declval<tp_tuple_arg>().call(dango::make_index_seq<tp_tuple_size>{ }, dango::declval<tp_func>()))
>
static constexpr auto
call_test
(detail::tuple_noexcept_test_tag const)noexcept->bool
{
  return noexcept(dango::declval<tp_tuple_arg>().call(dango::make_index_seq<tp_tuple_size>{ }, dango::declval<tp_func>()));
}

template
<
  dango::usize tp_tuple_size,
  typename tp_tuple_arg,
  typename tp_func,
  typename tp_tag
>
static constexpr auto
call_test
(tp_tag const)noexcept->bool
{
  return false;
}

public:

/*** operator ->* ***/

  template
  <
    typename tp_func,
    typename tp_tuple_arg = tuple&,
    dango::enable_if
    <
      call_test<sizeof...(tp_types), tp_tuple_arg, tp_func>(detail::tuple_normal_test)
    > = dango::enable_val
  >
  constexpr auto
  operator ->*
  (tp_func&& a_func)
  noexcept(call_test<sizeof...(tp_types), tp_tuple_arg, tp_func>(detail::tuple_noexcept_test))->decltype(auto)
  {
    return call(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func));
  }

  template
  <
    typename tp_func,
    typename tp_tuple_arg = tuple const&,
    dango::enable_if
    <
      call_test<sizeof...(tp_types), tp_tuple_arg, tp_func>(detail::tuple_normal_test)
    > = dango::enable_val
  >
  constexpr auto
  operator ->*
  (tp_func&& a_func)const
  noexcept(call_test<sizeof...(tp_types), tp_tuple_arg, tp_func>(detail::tuple_noexcept_test))->decltype(auto)
  {
    return call(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func));
  }

private:
  storage_type m_storage;
};

/*** empty tuple ***/

template<>
class
dango::
tuple<>
final
{
public:
  static constexpr auto
  size
  ()noexcept->dango::usize
  {
    return dango::usize(0);
  }
public:
  DANGO_TAG_TYPE(tuple)
public:
  template
  <
    typename tp_func,
    dango::enable_if
    <
      dango::is_callable<dango::remove_ref<tp_func>&>
    > = dango::enable_val
  >
  constexpr auto
  operator ->*
  (tp_func&& a_func)const
  noexcept(dango::is_noexcept_callable<dango::remove_ref<tp_func>&>)->decltype(auto)
  {
    return a_func();
  }
};

#endif

