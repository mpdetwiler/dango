#ifndef DANGO_TUPLE_HPP_INCLUDED
#define DANGO_TUPLE_HPP_INCLUDED

#include "dango-util.hpp"

/*** helpers ***/

namespace
dango::detail
{
  template
  <typename... tp_types>
  struct
  tuple_pack
  final
  {
    DANGO_TAG_TYPE(tuple_pack)
  };
}

/*** append prepend ***/

namespace
dango::detail
{
  template
  <typename tp_type, typename tp_pack>
  struct tuple_pack_prepend_help;
  template
  <typename tp_type, typename... tp_pack>
  struct tuple_pack_prepend_help<tp_type, dango::detail::tuple_pack<tp_pack...>>;

  template
  <typename tp_type, typename tp_pack>
  struct tuple_pack_append_help;
  template
  <typename tp_type, typename... tp_pack>
  struct tuple_pack_append_help<tp_type, dango::detail::tuple_pack<tp_pack...>>;

  template
  <typename tp_type, typename tp_pack>
  using tuple_pack_prepend = typename dango::detail::tuple_pack_prepend_help<tp_type, tp_pack>::type;

  template
  <typename tp_type, typename tp_pack>
  using tuple_pack_append = typename dango::detail::tuple_pack_append_help<tp_type, tp_pack>::type;
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

  DANGO_UNCONSTRUCTIBLE(tuple_pack_prepend_help)
};

template
<typename tp_type, typename... tp_pack>
struct
dango::
detail::
tuple_pack_append_help<tp_type, dango::detail::tuple_pack<tp_pack...>>
final
{
  using type = dango::detail::tuple_pack<tp_pack..., tp_type>;

  DANGO_UNCONSTRUCTIBLE(tuple_pack_append_help)
};

/*** reverse ***/

namespace
dango::detail
{
  template
  <typename tp_pack, typename... tp_next>
  struct tuple_pack_reverse_help;

  template
  <typename... tp_pack, typename tp_first, typename... tp_next>
  struct tuple_pack_reverse_help<dango::detail::tuple_pack<tp_pack...>, tp_first, tp_next...>;
  template
  <typename... tp_pack>
  struct tuple_pack_reverse_help<dango::detail::tuple_pack<tp_pack...>>;

  template
  <typename... tp_types>
  using tuple_pack_reverse =
    typename dango::detail::tuple_pack_reverse_help<dango::detail::tuple_pack<>, tp_types...>::type;
}

template
<typename... tp_pack, typename tp_first, typename... tp_next>
struct
dango::
detail::
tuple_pack_reverse_help
<dango::detail::tuple_pack<tp_pack...>, tp_first, tp_next...>
final
{
  using pack_type = dango::detail::tuple_pack<tp_pack...>;

  using type =
    typename dango::detail::tuple_pack_reverse_help<dango::detail::tuple_pack_prepend<tp_first, pack_type>, tp_next...>::type;

  DANGO_UNCONSTRUCTIBLE(tuple_pack_reverse_help)
};

template
<typename... tp_pack>
struct
dango::
detail::
tuple_pack_reverse_help
<dango::detail::tuple_pack<tp_pack...>>
final
{
  using pack_type = dango::detail::tuple_pack<tp_pack...>;

  using type = pack_type;

  DANGO_UNCONSTRUCTIBLE(tuple_pack_reverse_help)
};

/*** clamp ***/

namespace
dango::detail
{
  template
  <typename tp_pack, dango::usize tp_size, typename... tp_types>
  struct tuple_pack_clamp_help;

  template
  <typename... tp_pack, dango::usize tp_size, typename tp_first, typename... tp_next>
  struct
  tuple_pack_clamp_help
  <dango::detail::tuple_pack<tp_pack...>, tp_size, tp_first, tp_next...>;
  template
  <typename... tp_pack, typename tp_first, typename... tp_next>
  struct
  tuple_pack_clamp_help
  <dango::detail::tuple_pack<tp_pack...>, dango::usize(0), tp_first, tp_next...>;
  template
  <typename... tp_pack>
  struct
  tuple_pack_clamp_help
  <dango::detail::tuple_pack<tp_pack...>, dango::usize(0)>;

  template
  <dango::usize tp_size, typename... tp_types>
  requires(tp_size <= sizeof...(tp_types))
  using tuple_pack_clamp =
    typename dango::detail::tuple_pack_clamp_help<dango::detail::tuple_pack<>, tp_size, tp_types...>::type;
}

template
<typename... tp_pack, dango::usize tp_size, typename tp_first, typename... tp_next>
struct
dango::
detail::
tuple_pack_clamp_help
<dango::detail::tuple_pack<tp_pack...>, tp_size, tp_first, tp_next...>
final
{
  using pack_type = dango::detail::tuple_pack<tp_pack...>;

  using type =
    typename dango::detail::tuple_pack_clamp_help<dango::detail::tuple_pack_append<tp_first, pack_type>, tp_size - dango::usize(1), tp_next...>::type;

  DANGO_UNCONSTRUCTIBLE(tuple_pack_clamp_help)
};

template
<typename... tp_pack, typename tp_first, typename... tp_next>
struct
dango::
detail::
tuple_pack_clamp_help
<dango::detail::tuple_pack<tp_pack...>, dango::usize(0), tp_first, tp_next...>
final
{
  using pack_type = dango::detail::tuple_pack<tp_pack...>;

  using type = pack_type;

  DANGO_UNCONSTRUCTIBLE(tuple_pack_clamp_help)
};

template
<typename... tp_pack>
struct
dango::
detail::
tuple_pack_clamp_help
<dango::detail::tuple_pack<tp_pack...>, dango::usize(0)>
final
{
  using pack_type = dango::detail::tuple_pack<tp_pack...>;

  using type = pack_type;

  DANGO_UNCONSTRUCTIBLE(tuple_pack_clamp_help)
};

/*** tuple_is_comparable ***/

namespace
dango::detail
{
  template
  <typename tp_pack1, typename tp_pack2>
  struct tuple_is_comparable_help;
  template
  <typename... tp_pack1, typename... tp_pack2>
  struct
  tuple_is_comparable_help
  <dango::detail::tuple_pack<tp_pack1...>, dango::detail::tuple_pack<tp_pack2...>>;

  template
  <typename tp_pack1, typename tp_pack2>
  concept tuple_is_comparable =
    dango::detail::tuple_is_comparable_help<tp_pack1, tp_pack2>::value;

  template
  <typename tp_pack1, typename tp_pack2>
  concept tuple_is_noexcept_comparable =
    dango::detail::tuple_is_comparable<tp_pack1, tp_pack2> &&
    dango::detail::tuple_is_comparable_help<tp_pack1, tp_pack2>::value_noexcept;
}

template
<typename... tp_pack1, typename... tp_pack2>
struct
dango::
detail::
tuple_is_comparable_help
<dango::detail::tuple_pack<tp_pack1...>, dango::detail::tuple_pack<tp_pack2...>>
final
{
private:
  static inline constexpr dango::usize const c_size = dango::min(sizeof...(tp_pack1), sizeof...(tp_pack2));

  using pack_clamped1 = dango::detail::tuple_pack_clamp<c_size, tp_pack1...>;
  using pack_clamped2 = dango::detail::tuple_pack_clamp<c_size, tp_pack2...>;

  template
  <typename... tp_clamped1, typename... tp_clamped2>
  static constexpr auto
  check
  (dango::detail::tuple_pack<tp_clamped1...> const, dango::detail::tuple_pack<tp_clamped2...> const)noexcept->bool
  {
    return ( ... && dango::is_comparable<tp_clamped1, tp_clamped2>);
  }

  template
  <typename... tp_clamped1, typename... tp_clamped2>
  static constexpr auto
  check_noexcept
  (dango::detail::tuple_pack<tp_clamped1...> const, dango::detail::tuple_pack<tp_clamped2...> const)noexcept->bool
  {
    return ( ... && dango::is_noexcept_comparable<tp_clamped1, tp_clamped2>);
  }
public:
  static inline constexpr bool const value = check(pack_clamped1{ }, pack_clamped2{ });
  static inline constexpr bool const value_noexcept = check_noexcept(pack_clamped1{ }, pack_clamped2{ });
public:
  DANGO_UNCONSTRUCTIBLE(tuple_is_comparable_help)
};

/*** tuple constraint ***/

namespace
dango
{
  template
  <typename... tp_types>
  concept tuple_constraint_spec =
    ( ... && (dango::is_ref<tp_types> || dango::is_object_exclude_array<tp_types>)) &&
    ( ... && dango::is_noexcept_destructible<tp_types>);

  template
  <typename... tp_types>
  concept tuple_constraint_spec_all_ref =
    dango::tuple_constraint_spec<tp_types...> && ( ... && dango::is_ref<tp_types>);
}

namespace
dango
{
  template
  <typename... tp_types>
  requires(dango::tuple_constraint_spec<tp_types...>)
  class tuple;

  template<>
  class tuple<>;

  template
  <typename tp_first, typename tp_second>
  using pair = dango::tuple<tp_first, tp_second>;
}

/*** is_tuple ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  inline constexpr bool const is_tuple_help = false;

  template
  <typename... tp_types>
  inline constexpr bool const is_tuple_help<dango::tuple<tp_types...>> = true;
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_tuple = dango::detail::is_tuple_help<dango::remove_cv<tp_type>>;
}

/*** tuple_get_type ***/

namespace
dango
{
  struct
  tuple_model
  {
    DANGO_UNCONSTRUCTIBLE(tuple_model)
  };
}

namespace
dango::detail
{
  template
  <typename tp_model, dango::tuple_constraint_spec tp_value_type>
  struct tuple_get_type_help;

  template
  <dango::tuple_constraint_spec tp_value_type>
  struct
  tuple_get_type_help<dango::tuple_model&&, tp_value_type>
  final
  {
    using type = dango::conditional<dango::is_ref<tp_value_type>, tp_value_type, tp_value_type&&>;

    DANGO_UNCONSTRUCTIBLE(tuple_get_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_value_type>
  struct
  tuple_get_type_help<dango::tuple_model const&&, tp_value_type>
  final
  {
    using type = dango::conditional<dango::is_ref<tp_value_type>, tp_value_type, tp_value_type const&&>;

    DANGO_UNCONSTRUCTIBLE(tuple_get_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_value_type>
  struct tuple_get_type_help<dango::tuple_model volatile&&, tp_value_type>;
  template
  <dango::tuple_constraint_spec tp_value_type>
  struct tuple_get_type_help<dango::tuple_model const volatile&&, tp_value_type>;

  template
  <dango::tuple_constraint_spec tp_value_type>
  struct
  tuple_get_type_help<dango::tuple_model, tp_value_type>
  final
  {
    using type = typename dango::detail::tuple_get_type_help<dango::tuple_model&&, tp_value_type>::type;

    DANGO_UNCONSTRUCTIBLE(tuple_get_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_value_type>
  struct
  tuple_get_type_help<dango::tuple_model const, tp_value_type>
  final
  {
    using type = typename dango::detail::tuple_get_type_help<dango::tuple_model const&&, tp_value_type>::type;

    DANGO_UNCONSTRUCTIBLE(tuple_get_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_value_type>
  struct tuple_get_type_help<dango::tuple_model volatile, tp_value_type>;
  template
  <dango::tuple_constraint_spec tp_value_type>
  struct tuple_get_type_help<dango::tuple_model const volatile, tp_value_type>;

  template
  <dango::tuple_constraint_spec tp_value_type>
  struct
  tuple_get_type_help<dango::tuple_model&, tp_value_type>
  final
  {
    using type = dango::conditional<dango::is_ref<tp_value_type>, tp_value_type, tp_value_type&>;

    DANGO_UNCONSTRUCTIBLE(tuple_get_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_value_type>
  struct
  tuple_get_type_help<dango::tuple_model const&, tp_value_type>
  final
  {
    using type = dango::conditional<dango::is_ref<tp_value_type>, tp_value_type, tp_value_type const&>;

    DANGO_UNCONSTRUCTIBLE(tuple_get_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_value_type>
  struct tuple_get_type_help<dango::tuple_model volatile&, tp_value_type>;
  template
  <dango::tuple_constraint_spec tp_value_type>
  struct tuple_get_type_help<dango::tuple_model const volatile&, tp_value_type>;
}

namespace
dango
{
  template
  <typename tp_model, typename tp_type>
  using tuple_get_type =
    typename dango::detail::tuple_get_type_help<tp_model, tp_type>::type;
}

/*** init tags ***/

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

/*** tuple_is_constructible tuple_is_convertible ***/

namespace
dango::detail
{
  template
  <typename tp_value_type>
  concept is_skip_constructible_help =
    (dango::is_trivial_default_constructible<tp_value_type> && !dango::is_const<tp_value_type>) ||
    (!dango::is_trivial_default_constructible<tp_value_type> && dango::is_default_constructible<tp_value_type>);

  template
  <typename tp_value_type>
  concept is_noexcept_skip_constructible_help =
    (dango::is_trivial_default_constructible<tp_value_type> && !dango::is_const<tp_value_type>) ||
    (!dango::is_trivial_default_constructible<tp_value_type> && dango::is_noexcept_default_constructible<tp_value_type>);
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_skip_constructible =
    dango::tuple_constraint_spec<tp_type> && dango::detail::is_skip_constructible_help<tp_type>;

  template
  <typename tp_type>
  concept is_noexcept_skip_constructible =
    dango::is_skip_constructible<tp_type> && dango::detail::is_noexcept_skip_constructible_help<tp_type>;
}

namespace
dango::detail
{
  template
  <typename tp_arg>
  concept is_value_init_tag = dango::is_same_ignore_cvref<tp_arg, dango::value_init_tag>;

  template
  <typename tp_arg>
  concept is_skip_init_tag = dango::is_same_ignore_cvref<tp_arg, dango::skip_init_tag>;

  template
  <typename tp_arg>
  concept is_init_tag =
    dango::detail::is_value_init_tag<tp_arg> || dango::detail::is_skip_init_tag<tp_arg>;

  template
  <typename tp_value_type, typename tp_arg>
  concept tuple_is_constructible_help =
    (dango::detail::is_value_init_tag<tp_arg> && dango::is_default_constructible<tp_value_type>) ||
    (dango::detail::is_skip_init_tag<tp_arg> && dango::detail::is_skip_constructible_help<tp_value_type>) ||
    dango::is_brace_constructible<tp_value_type, tp_arg>;

  template
  <typename tp_value_type, typename tp_arg>
  concept tuple_is_noexcept_constructible_help =
    (dango::detail::is_value_init_tag<tp_arg> && dango::is_noexcept_default_constructible<tp_value_type>) ||
    (dango::detail::is_skip_init_tag<tp_arg> && dango::detail::is_noexcept_skip_constructible_help<tp_value_type>) ||
    dango::is_noexcept_brace_constructible<tp_value_type, tp_arg>;

  template
  <typename tp_arg, typename tp_value_type>
  concept tuple_is_convertible_help =
    !dango::detail::is_value_init_tag<tp_arg> && !dango::detail::is_skip_init_tag<tp_arg> &&
    dango::is_convertible_arg<tp_arg, tp_value_type>;

  template
  <typename tp_type, typename tp_arg>
  concept tuple_is_constructible =
    dango::tuple_constraint_spec<tp_type> && dango::detail::tuple_is_constructible_help<tp_type, tp_arg>;

  template
  <typename tp_type, typename tp_arg>
  concept tuple_is_noexcept_constructible =
    dango::detail::tuple_is_constructible<tp_type, tp_arg> && dango::detail::tuple_is_noexcept_constructible_help<tp_type, tp_arg>;

  template
  <typename tp_arg, typename tp_type>
  concept tuple_is_convertible =
    dango::tuple_constraint_spec<tp_type> && dango::detail::tuple_is_convertible_help<tp_arg, tp_type>;
}

/*** tuple_storage ***/

namespace
dango::detail
{
  template
  <typename tp_pack>
  struct tuple_storage;

  template
  <typename tp_first>
  struct tuple_storage<dango::detail::tuple_pack<tp_first>>;

  template
  <typename tp_first, typename... tp_next>
  struct tuple_storage<dango::detail::tuple_pack<tp_first, tp_next...>>;
}

template
<typename tp_first>
struct
dango::
detail::
tuple_storage
<dango::detail::tuple_pack<tp_first>>
{
public:
  using value_type = tp_first;
  static_assert(dango::is_noexcept_destructible<value_type>);
public:
  constexpr tuple_storage()noexcept = delete;
  constexpr
  tuple_storage()noexcept(dango::is_noexcept_default_constructible<value_type>)
  requires(dango::is_default_constructible<value_type>) = default;

  template
  <typename tp_arg>
  requires(!dango::detail::is_init_tag<tp_arg> && !dango::is_same_ignore_cvref<tp_arg, tuple_storage>)
  explicit constexpr
  tuple_storage
  (tp_arg&& a_arg)noexcept(dango::is_noexcept_brace_constructible<value_type, tp_arg>):
  m_value{ dango::forward<tp_arg>(a_arg) }
  {

  }

  explicit constexpr
  tuple_storage
  (dango::value_init_tag const)noexcept(dango::is_noexcept_default_constructible<value_type>):
  m_value{ }
  {

  }

  explicit constexpr
  tuple_storage
  (dango::skip_init_tag const)noexcept(dango::is_noexcept_default_constructible<value_type>)
  {

  }

  constexpr tuple_storage(tuple_storage const&)noexcept = delete;
  constexpr
  tuple_storage
  (tuple_storage const&)noexcept(dango::is_noexcept_copy_constructible<value_type>)
  requires(dango::is_copy_constructible<value_type>) = default;

  constexpr tuple_storage(tuple_storage&&)noexcept = delete;
  constexpr
  tuple_storage
  (tuple_storage&&)noexcept(dango::is_noexcept_move_constructible<value_type>)
  requires(dango::is_move_constructible<value_type>) = default;

  constexpr ~tuple_storage()noexcept = default;

  constexpr auto operator = (tuple_storage const&)& noexcept = delete;
  constexpr auto
  operator =
  (tuple_storage const&)& noexcept(dango::is_noexcept_copy_assignable<value_type>)->tuple_storage&
  requires(dango::is_copy_assignable<value_type>) = default;

  constexpr auto operator = (tuple_storage&&)& noexcept = delete;
  constexpr auto
  operator =
  (tuple_storage&&)& noexcept(dango::is_noexcept_move_assignable<value_type>)->tuple_storage&
  requires(dango::is_move_assignable<value_type>) = default;

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && dango::is_ref<value_type>)
  constexpr auto
  get()const noexcept->value_type
  {
    return dango::forward<value_type>(m_value);
  }

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && !dango::is_ref<value_type>)
  constexpr auto
  get()& noexcept->value_type&
  {
    return m_value;
  }

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && !dango::is_ref<value_type>)
  constexpr auto
  get()&& noexcept->value_type&&
  {
    return dango::move(m_value);
  }

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && !dango::is_ref<value_type>)
  constexpr auto
  get()const& noexcept->value_type const&
  {
    return m_value;
  }

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && !dango::is_ref<value_type>)
  constexpr auto
  get()const&& noexcept->value_type const&&
  {
    return dango::move(m_value);
  }
private:
  value_type m_value;
};

template
<typename tp_first, typename... tp_next>
struct
dango::
detail::
tuple_storage
<dango::detail::tuple_pack<tp_first, tp_next...>>:
public dango::detail::tuple_storage<dango::detail::tuple_pack<tp_next...>>
{
public:
  using value_type = tp_first;
  static_assert(dango::is_noexcept_destructible<value_type>);
private:
  using super_type = dango::detail::tuple_storage<dango::detail::tuple_pack<tp_next...>>;
public:
  constexpr tuple_storage()noexcept = delete;
  constexpr
  tuple_storage()
  noexcept((dango::is_noexcept_default_constructible<value_type> && ... && dango::is_noexcept_default_constructible<tp_next>))
  requires((dango::is_default_constructible<value_type> && ... && dango::is_default_constructible<tp_next>)) = default;

  template
  <typename tp_arg, typename... tp_args>
  requires((sizeof...(tp_args) == sizeof...(tp_next)) && !dango::detail::is_init_tag<tp_arg>)
  explicit constexpr
  tuple_storage
  (tp_arg&& a_arg, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<value_type, tp_arg> && dango::is_noexcept_brace_constructible<super_type, tp_args...>):
  super_type{ dango::forward<tp_args>(a_args)... },
  m_value{ dango::forward<tp_arg>(a_arg) }
  {

  }

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_next))
  explicit constexpr
  tuple_storage
  (dango::value_init_tag const, tp_args&&... a_args)
  noexcept(dango::is_noexcept_default_constructible<value_type> && dango::is_noexcept_brace_constructible<super_type, tp_args...>):
  super_type{ dango::forward<tp_args>(a_args)... },
  m_value{ }
  {

  }

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_next))
  explicit constexpr
  tuple_storage
  (dango::skip_init_tag const, tp_args&&... a_args)
  noexcept(dango::is_noexcept_default_constructible<value_type> && dango::is_noexcept_brace_constructible<super_type, tp_args...>):
  super_type{ dango::forward<tp_args>(a_args)... }
  {

  }

  constexpr tuple_storage(tuple_storage const&)noexcept = delete;
  constexpr
  tuple_storage
  (tuple_storage const&)
  noexcept((dango::is_noexcept_copy_constructible<value_type> && ... && dango::is_noexcept_copy_constructible<tp_next>))
  requires((dango::is_copy_constructible<value_type> && ... && dango::is_copy_constructible<tp_next>)) = default;

  constexpr tuple_storage(tuple_storage&&)noexcept = delete;
  constexpr
  tuple_storage
  (tuple_storage&&)
  noexcept((dango::is_noexcept_move_constructible<value_type> && ... && dango::is_noexcept_move_constructible<tp_next>))
  requires((dango::is_move_constructible<value_type> && ... && dango::is_move_constructible<tp_next>)) = default;

  constexpr ~tuple_storage()noexcept = default;

  constexpr auto operator = (tuple_storage const&)& noexcept = delete;
  constexpr auto
  operator =
  (tuple_storage const&)&
  noexcept((dango::is_noexcept_copy_assignable<value_type> && ... && dango::is_noexcept_copy_assignable<tp_next>))->tuple_storage&
  requires((dango::is_copy_assignable<value_type> && ... && dango::is_copy_assignable<tp_next>)) = default;

  constexpr auto operator = (tuple_storage&&)& noexcept = delete;
  constexpr auto
  operator =
  (tuple_storage&&)&
  noexcept((dango::is_noexcept_move_assignable<value_type> && ... && dango::is_noexcept_move_assignable<tp_next>))->tuple_storage&
  requires((dango::is_move_assignable<value_type> && ... && dango::is_move_assignable<tp_next>)) = default;

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && dango::is_ref<value_type>)
  constexpr auto
  get()const noexcept->value_type
  {
    return dango::forward<value_type>(m_value);
  }

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && !dango::is_ref<value_type>)
  constexpr auto
  get()& noexcept->value_type&
  {
    return m_value;
  }

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && !dango::is_ref<value_type>)
  constexpr auto
  get()&& noexcept->value_type&&
  {
    return dango::move(m_value);
  }

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && !dango::is_ref<value_type>)
  constexpr auto
  get()const& noexcept->value_type const&
  {
    return m_value;
  }

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && !dango::is_ref<value_type>)
  constexpr auto
  get()const&& noexcept->value_type const&&
  {
    return dango::move(m_value);
  }

  template
  <dango::usize tp_index>
  requires(tp_index != dango::usize(0))
  constexpr auto
  get()& noexcept->decltype(auto)
  {
    return static_cast<super_type&>(*this).template get<tp_index - dango::usize(1)>();
  }

  template
  <dango::usize tp_index>
  requires(tp_index != dango::usize(0))
  constexpr auto
  get()&& noexcept->decltype(auto)
  {
    return static_cast<super_type&&>(*this).template get<tp_index - dango::usize(1)>();
  }

  template
  <dango::usize tp_index>
  requires(tp_index != dango::usize(0))
  constexpr auto
  get()const& noexcept->decltype(auto)
  {
    return static_cast<super_type const&>(*this).template get<tp_index - dango::usize(1)>();
  }

  template
  <dango::usize tp_index>
  requires(tp_index != dango::usize(0))
  constexpr auto
  get()const&& noexcept->decltype(auto)
  {
    return static_cast<super_type const&&>(*this).template get<tp_index - dango::usize(1)>();
  }
private:
  value_type m_value;
};

/*** make_tuple_storage ***/

namespace
dango::detail
{
  template
  <typename tp_storage_type, bool tp_noexcept, typename... tp_types>
  struct make_tuple_storage_help;

  template
  <typename tp_storage_type, bool tp_noexcept>
  struct make_tuple_storage_help<tp_storage_type, tp_noexcept>;

  template
  <typename tp_storage_type, bool tp_noexcept, typename tp_first, typename... tp_next>
  struct make_tuple_storage_help<tp_storage_type, tp_noexcept, tp_first, tp_next...>;

  template
  <typename tp_storage_type, bool tp_noexcept, typename... tp_args>
  constexpr auto
  make_tuple_storage
  (tp_args&&... a_args)noexcept(tp_noexcept)->tp_storage_type
  {
    using make_type = dango::detail::make_tuple_storage_help<tp_storage_type, tp_noexcept, tp_args&&...>;

    return make_type::make(dango::forward<tp_args>(a_args)...);
  }
}

template
<typename tp_storage_type, bool tp_noexcept>
struct
dango::
detail::
make_tuple_storage_help<tp_storage_type, tp_noexcept>
final
{
  template
  <typename... tp_args>
  static constexpr auto
  make(tp_args&&... a_args)noexcept(tp_noexcept)->tp_storage_type
  {
    return tp_storage_type{ dango::forward<tp_args>(a_args)... };
  }

  DANGO_UNCONSTRUCTIBLE(make_tuple_storage_help)
};

template
<typename tp_storage_type, bool tp_noexcept, typename tp_first, typename... tp_next>
struct
dango::
detail::
make_tuple_storage_help<tp_storage_type, tp_noexcept, tp_first, tp_next...>
final
{
  static_assert((dango::is_ref<tp_first> && ... && dango::is_ref<tp_next>));

  using make_type = dango::detail::make_tuple_storage_help<tp_storage_type, tp_noexcept, tp_next...>;

  template
  <typename... tp_args>
  static constexpr auto
  make(tp_first a_first, tp_next... a_next, tp_args&&... a_args)noexcept(tp_noexcept)->tp_storage_type
  {
    return make_type::make(dango::forward<tp_next>(a_next)..., dango::forward<tp_first>(a_first), dango::forward<tp_args>(a_args)...);
  }

  DANGO_UNCONSTRUCTIBLE(make_tuple_storage_help)
};

/*** tuple ***/

template
<typename... tp_types>
requires(dango::tuple_constraint_spec<tp_types...>)
class
dango::
tuple
{
private:
  using storage_type =
    dango::detail::tuple_storage<dango::detail::tuple_pack_reverse<tp_types...>>;
public:
  static constexpr auto size()noexcept->dango::usize{ return sizeof...(tp_types); }
public:
  constexpr tuple()noexcept = delete;
  constexpr
  tuple()noexcept(( ... && dango::is_noexcept_default_constructible<tp_types>))
  requires(( ... && dango::is_default_constructible<tp_types>)) = default;

#define DANGO_TUPLE_SPEC(noex) \
  ( ... && dango::detail::tuple_is##noex##constructible<tp_types, tp_args>)

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    DANGO_TUPLE_SPEC(_)
  )
  explicit(!( ... && dango::detail::tuple_is_convertible<tp_args, tp_types>))
  constexpr
  tuple(tp_args&&... a_args)
  noexcept(DANGO_TUPLE_SPEC(_noexcept_)):
  m_storage{ dango::detail::make_tuple_storage<storage_type, DANGO_TUPLE_SPEC(_noexcept_)>(dango::forward<tp_args>(a_args)...) }
  {

  }

#undef DANGO_TUPLE_SPEC

  constexpr tuple(tuple const&)noexcept = delete;
  constexpr
  tuple(tuple const&)
  noexcept(( ... && dango::is_noexcept_copy_constructible<tp_types>))
  requires(( ... && dango::is_copy_constructible<tp_types>)) = default;

  constexpr tuple(tuple&&)noexcept = delete;
  constexpr
  tuple(tuple&&)
  noexcept(( ... && dango::is_noexcept_move_constructible<tp_types>))
  requires(( ... && dango::is_move_constructible<tp_types>)) = default;

  constexpr ~tuple()noexcept = default;

  constexpr auto operator = (tuple const&)& noexcept = delete;
  constexpr auto
  operator =
  (tuple const&)&
  noexcept(( ... && dango::is_noexcept_copy_assignable<tp_types>))->tuple&
  requires(( ... && dango::is_copy_assignable<tp_types>)) = default;

  constexpr auto operator = (tuple&&)& noexcept = delete;
  constexpr auto
  operator =
  (tuple&&)&
  noexcept(( ... && dango::is_noexcept_move_assignable<tp_types>))->tuple&
  requires(( ... && dango::is_move_assignable<tp_types>)) = default;

  template
  <dango::usize tp_index>
  requires(tp_index < sizeof...(tp_types))
  constexpr auto
  get()& noexcept->decltype(auto)
  {
    return m_storage.template get<sizeof...(tp_types) - dango::usize(1) - tp_index>();
  }

  template
  <dango::usize tp_index>
  requires(tp_index < sizeof...(tp_types))
  constexpr auto
  get()&& noexcept->decltype(auto)
  {
    return dango::move(m_storage).template get<sizeof...(tp_types) - dango::usize(1) - tp_index>();
  }

  template
  <dango::usize tp_index>
  requires(tp_index < sizeof...(tp_types))
  constexpr auto
  get()const& noexcept->decltype(auto)
  {
    return m_storage.template get<sizeof...(tp_types) - dango::usize(1) - tp_index>();
  }

  template
  <dango::usize tp_index>
  requires(tp_index < sizeof...(tp_types))
  constexpr auto
  get()const&& noexcept->decltype(auto)
  {
    return dango::move(m_storage).template get<sizeof...(tp_types) - dango::usize(1) - tp_index>();
  }

#define DANGO_TUPLE_DEFINE_NAMED_GET(index, name) \
  constexpr auto name()& noexcept->decltype(auto) \
  requires(dango::usize(index) < sizeof...(tp_types)) \
  { return static_cast<tuple&>(*this).get<dango::usize(index)>(); } \
  constexpr auto name()&& noexcept->decltype(auto) \
  requires(dango::usize(index) < sizeof...(tp_types)) \
  { return static_cast<tuple&&>(*this).get<dango::usize(index)>(); } \
  constexpr auto name()const& noexcept->decltype(auto) \
  requires(dango::usize(index) < sizeof...(tp_types)) \
  { return static_cast<tuple const&>(*this).get<dango::usize(index)>(); } \
  constexpr auto name()const&& noexcept->decltype(auto) \
  requires(dango::usize(index) < sizeof...(tp_types)) \
  { return static_cast<tuple const&&>(*this).get<dango::usize(index)>(); }

  DANGO_TUPLE_DEFINE_NAMED_GET(0, first)
  DANGO_TUPLE_DEFINE_NAMED_GET(1, second)
  DANGO_TUPLE_DEFINE_NAMED_GET(2, third)
  DANGO_TUPLE_DEFINE_NAMED_GET(3, fourth)
  DANGO_TUPLE_DEFINE_NAMED_GET(4, fifth)
  DANGO_TUPLE_DEFINE_NAMED_GET(5, sixth)
  DANGO_TUPLE_DEFINE_NAMED_GET(6, seventh)
  DANGO_TUPLE_DEFINE_NAMED_GET(7, eighth)
  DANGO_TUPLE_DEFINE_NAMED_GET(8, ninth)
  DANGO_TUPLE_DEFINE_NAMED_GET(9, tenth)

#undef DANGO_TUPLE_DEFINE_NAMED_GET

private:

/*** converting constructor helpers ***/

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename... tp_args>
  static constexpr auto
  construct_help
  (dango::index_seq<tp_indices...> const, tuple<tp_args...> const& a_tup)
  noexcept(tp_noexcept)->storage_type
  {
    return dango::detail::make_tuple_storage<storage_type, tp_noexcept>(a_tup.template get<tp_indices>()...);
  }

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename... tp_args>
  static constexpr auto
  construct_help
  (dango::index_seq<tp_indices...> const, tuple<tp_args...>&& a_tup)
  noexcept(tp_noexcept)->storage_type
  {
    return dango::detail::make_tuple_storage<storage_type, tp_noexcept>(dango::move(a_tup).template get<tp_indices>()...);
  }

public:

/*** converting copy-construct ***/

#define DANGO_TUPLE_SPEC(noex, cvref) \
  ( ... && dango::detail::tuple_is##noex##constructible<tp_types, dango::tuple_get_type<dango::tuple_model cvref, tp_args>>)

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    (sizeof...(tp_args) != dango::usize(1)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    DANGO_TUPLE_SPEC(_, const&)
  )
  explicit(!( ... && dango::detail::tuple_is_convertible<dango::tuple_get_type<dango::tuple_model const&, tp_args>, tp_types>))
  constexpr
  tuple
  (dango::tuple<tp_args...> const& a_tup)
  noexcept(DANGO_TUPLE_SPEC(_noexcept_, const&)):
  m_storage{ construct_help<DANGO_TUPLE_SPEC(_noexcept_, const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup) }
  {

  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    (sizeof...(tp_args) == dango::usize(1)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    DANGO_TUPLE_SPEC(_, const&) &&
    !( ... && dango::is_brace_constructible<tp_types, dango::tuple<tp_args...> const&>) &&
    !( ... && dango::is_convertible_arg<dango::tuple<tp_args...> const&, tp_types>)
  )
  explicit(!( ... && dango::detail::tuple_is_convertible<dango::tuple_get_type<dango::tuple_model const&, tp_args>, tp_types>))
  constexpr
  tuple
  (dango::tuple<tp_args...> const& a_tup)
  noexcept(DANGO_TUPLE_SPEC(_noexcept_, const&)):
  m_storage{ construct_help<DANGO_TUPLE_SPEC(_noexcept_, const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup) }
  {

  }

/*** converting move-construct ***/

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    (sizeof...(tp_args) != dango::usize(1)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    DANGO_TUPLE_SPEC(_, &&)
  )
  explicit(!( ... && dango::detail::tuple_is_convertible<dango::tuple_get_type<dango::tuple_model&&, tp_args>, tp_types>))
  constexpr
  tuple
  (dango::tuple<tp_args...>&& a_tup)
  noexcept(DANGO_TUPLE_SPEC(_noexcept_, &&)):
  m_storage{ construct_help<DANGO_TUPLE_SPEC(_noexcept_, &&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup)) }
  {

  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    (sizeof...(tp_args) == dango::usize(1)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    DANGO_TUPLE_SPEC(_, &&)  &&
    !( ... && dango::is_brace_constructible<tp_types, dango::tuple<tp_args...>&&>) &&
    !( ... && dango::is_convertible_arg<dango::tuple<tp_args...>&&, tp_types>)
  )
  explicit(!( ... && dango::detail::tuple_is_convertible<dango::tuple_get_type<dango::tuple_model&&, tp_args>, tp_types>))
  constexpr
  tuple
  (dango::tuple<tp_args...>&& a_tup)
  noexcept(DANGO_TUPLE_SPEC(_noexcept_, &&)):
  m_storage{ construct_help<DANGO_TUPLE_SPEC(_noexcept_, &&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup)) }
  {

  }

#undef DANGO_TUPLE_SPEC

private:

/*** converting assign helpers ***/

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename... tp_args>
  constexpr auto
  assign_help
  (dango::index_seq<tp_indices...> const, dango::tuple<tp_args...> const& a_tup)& noexcept(tp_noexcept)->tuple&
  {
    ( ... , void(get<tp_indices>() = a_tup.template get<tp_indices>()));

    return *this;
  }

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename... tp_args>
  constexpr auto
  assign_help
  (dango::index_seq<tp_indices...> const, dango::tuple<tp_args...>&& a_tup)& noexcept(tp_noexcept)->tuple&
  {
    ( ... , void(get<tp_indices>() = dango::move(a_tup).template get<tp_indices>()));

    return *this;
  }

public:

/*** converting assign ***/

#define DANGO_TUPLE_SPEC(noex, lcvref, rcvref) \
  ( ... && dango::is##noex##assignable<dango::tuple_get_type<dango::tuple_model lcvref, tp_types>, dango::tuple_get_type<dango::tuple_model rcvref, tp_args>>)

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    ( ... && !dango::is_ref<tp_types>) &&
    DANGO_TUPLE_SPEC(_, &, const&)
  )
  constexpr auto
  operator =
  (dango::tuple<tp_args...> const& a_tup)& noexcept(DANGO_TUPLE_SPEC(_noexcept_, &, const&))->tuple&
  {
    return assign_help<DANGO_TUPLE_SPEC(_noexcept_, &, const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup);
  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    ( ... && !dango::is_ref<tp_types>) &&
    DANGO_TUPLE_SPEC(_, &, &&)
  )
  constexpr auto
  operator =
  (dango::tuple<tp_args...>&& a_tup)& noexcept(DANGO_TUPLE_SPEC(_noexcept_, &, &&))->tuple&
  {
    return assign_help<DANGO_TUPLE_SPEC(_noexcept_, &, &&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup));
  }

#undef DANGO_TUPLE_SPEC

private:

/*** operator ->* helpers ***/

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename tp_func>
  constexpr auto
  call
  (dango::index_seq<tp_indices...> const, tp_func&& a_func)&
  noexcept(tp_noexcept)->decltype(auto)
  {
    return dango::forward<tp_func>(a_func)(static_cast<tuple&>(*this).get<tp_indices>()...);
  }

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename tp_func>
  constexpr auto
  call
  (dango::index_seq<tp_indices...> const, tp_func&& a_func)&&
  noexcept(tp_noexcept)->decltype(auto)
  {
    return dango::forward<tp_func>(a_func)(static_cast<tuple&&>(*this).get<tp_indices>()...);
  }

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename tp_func>
  constexpr auto
  call
  (dango::index_seq<tp_indices...> const, tp_func&& a_func)const&
  noexcept(tp_noexcept)->decltype(auto)
  {
    return dango::forward<tp_func>(a_func)(static_cast<tuple const&>(*this).get<tp_indices>()...);
  }

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename tp_func>
  constexpr auto
  call
  (dango::index_seq<tp_indices...> const, tp_func&& a_func)const&&
  noexcept(tp_noexcept)->decltype(auto)
  {
    return dango::forward<tp_func>(a_func)(static_cast<tuple const&&>(*this).get<tp_indices>()...);
  }

public:

/*** operator ->* ***/

#define DANGO_TUPLE_SPEC(noex, cvref) \
  dango::is##noex##callable<tp_func, dango::tuple_get_type<dango::tuple_model cvref, tp_types>...>

  template
  <typename tp_func>
  requires(DANGO_TUPLE_SPEC(_, &))
  constexpr auto
  operator ->*
  (tp_func&& a_func)&
  noexcept(DANGO_TUPLE_SPEC(_noexcept_, &))->decltype(auto)
  {
    return static_cast<tuple&>(*this).call<DANGO_TUPLE_SPEC(_noexcept_, &)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func));
  }

  template
  <typename tp_func>
  requires(DANGO_TUPLE_SPEC(_, &&))
  constexpr auto
  operator ->*
  (tp_func&& a_func)&&
  noexcept(DANGO_TUPLE_SPEC(_noexcept_, &&))->decltype(auto)
  {
    return static_cast<tuple&&>(*this).call<DANGO_TUPLE_SPEC(_noexcept_, &&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func));
  }

  template
  <typename tp_func>
  requires(DANGO_TUPLE_SPEC(_, const&))
  constexpr auto
  operator ->*
  (tp_func&& a_func)const&
  noexcept(DANGO_TUPLE_SPEC(_noexcept_, const&))->decltype(auto)
  {
    return static_cast<tuple const&>(*this).call<DANGO_TUPLE_SPEC(_noexcept_, const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func));
  }

  template
  <typename tp_func>
  requires(DANGO_TUPLE_SPEC(_, const&&))
  constexpr auto
  operator ->*
  (tp_func&& a_func)const&&
  noexcept(DANGO_TUPLE_SPEC(_noexcept_, const&&))->decltype(auto)
  {
    return static_cast<tuple const&&>(*this).call<DANGO_TUPLE_SPEC(_noexcept_, const&&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func));
  }

#undef DANGO_TUPLE_SPEC

private:

/*** dango_operator_swap ***/

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename... tp_args>
  constexpr void
  swap_help
  (dango::index_seq<tp_indices...> const, dango::tuple<tp_args...>& a_tup)& noexcept(tp_noexcept)
  {
    ( ... , (dango::swap(get<tp_indices>(), a_tup.template get<tp_indices>())));
  }

public:

#define DANGO_TUPLE_SPEC(noex, lcvref, rcvref) \
  ( ... && dango::is##noex##swappable<dango::tuple_get_type<dango::tuple_model lcvref, tp_types>, dango::tuple_get_type<dango::tuple_model rcvref, tp_args>>)

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    ( ... && !dango::is_ref<tp_types>) &&
    ( ... && !dango::is_ref<tp_args>) &&
    DANGO_TUPLE_SPEC(_, &, &)
  )
  constexpr void
  dango_operator_swap
  (dango::tuple<tp_args...>& a_tup)& noexcept(DANGO_TUPLE_SPEC(_noexcept_, &, &))
  {
    swap_help<DANGO_TUPLE_SPEC(_noexcept_, &, &)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup);
  }

#undef DANGO_TUPLE_SPEC

private:
  storage_type m_storage;
};

/*** tuple of all references ***/

template
<typename... tp_types>
requires(dango::tuple_constraint_spec_all_ref<tp_types...>)
class
dango::
tuple<tp_types...>
{
private:
  using storage_type =
    dango::detail::tuple_storage<dango::detail::tuple_pack_reverse<tp_types...>>;
public:
  static constexpr auto size()noexcept->dango::usize{ return sizeof...(tp_types); }
public:
  constexpr tuple()noexcept = delete;

#define DANGO_TUPLE_SPEC(noex) \
  ( ... && dango::is##noex##brace_constructible<tp_types, tp_args>)

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    DANGO_TUPLE_SPEC(_)
  )
  explicit(!( ... && dango::is_convertible_arg<tp_args, tp_types>))
  constexpr
  tuple(tp_args&&... a_args)
  noexcept(DANGO_TUPLE_SPEC(_noexcept_)):
  m_storage{ dango::detail::make_tuple_storage<storage_type, DANGO_TUPLE_SPEC(_noexcept_)>(dango::forward<tp_args>(a_args)...) }
  {

  }

#undef DANGO_TUPLE_SPEC

  constexpr tuple(tuple const&)noexcept = default;

  constexpr tuple(tuple&&)noexcept = default;

  constexpr ~tuple()noexcept = default;

  template
  <dango::usize tp_index>
  requires(tp_index < sizeof...(tp_types))
  constexpr auto
  get()& noexcept->decltype(auto)
  {
    return m_storage.template get<sizeof...(tp_types) - dango::usize(1) - tp_index>();
  }

  template
  <dango::usize tp_index>
  requires(tp_index < sizeof...(tp_types))
  constexpr auto
  get()&& noexcept->decltype(auto)
  {
    return dango::move(m_storage).template get<sizeof...(tp_types) - dango::usize(1) - tp_index>();
  }

  template
  <dango::usize tp_index>
  requires(tp_index < sizeof...(tp_types))
  constexpr auto
  get()const& noexcept->decltype(auto)
  {
    return m_storage.template get<sizeof...(tp_types) - dango::usize(1) - tp_index>();
  }

  template
  <dango::usize tp_index>
  requires(tp_index < sizeof...(tp_types))
  constexpr auto
  get()const&& noexcept->decltype(auto)
  {
    return dango::move(m_storage).template get<sizeof...(tp_types) - dango::usize(1) - tp_index>();
  }

#define DANGO_TUPLE_DEFINE_NAMED_GET(index, name) \
  constexpr auto name()& noexcept->decltype(auto) \
  requires(dango::usize(index) < sizeof...(tp_types)) \
  { return static_cast<tuple&>(*this).get<dango::usize(index)>(); } \
  constexpr auto name()&& noexcept->decltype(auto) \
  requires(dango::usize(index) < sizeof...(tp_types)) \
  { return static_cast<tuple&&>(*this).get<dango::usize(index)>(); } \
  constexpr auto name()const& noexcept->decltype(auto) \
  requires(dango::usize(index) < sizeof...(tp_types)) \
  { return static_cast<tuple const&>(*this).get<dango::usize(index)>(); } \
  constexpr auto name()const&& noexcept->decltype(auto) \
  requires(dango::usize(index) < sizeof...(tp_types)) \
  { return static_cast<tuple const&&>(*this).get<dango::usize(index)>(); }

  DANGO_TUPLE_DEFINE_NAMED_GET(0, first)
  DANGO_TUPLE_DEFINE_NAMED_GET(1, second)
  DANGO_TUPLE_DEFINE_NAMED_GET(2, third)
  DANGO_TUPLE_DEFINE_NAMED_GET(3, fourth)
  DANGO_TUPLE_DEFINE_NAMED_GET(4, fifth)
  DANGO_TUPLE_DEFINE_NAMED_GET(5, sixth)
  DANGO_TUPLE_DEFINE_NAMED_GET(6, seventh)
  DANGO_TUPLE_DEFINE_NAMED_GET(7, eighth)
  DANGO_TUPLE_DEFINE_NAMED_GET(8, ninth)
  DANGO_TUPLE_DEFINE_NAMED_GET(9, tenth)

#undef DANGO_TUPLE_DEFINE_NAMED_GET

private:

/*** converting constructor helpers ***/

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename... tp_args>
  static constexpr auto
  construct_help
  (dango::index_seq<tp_indices...> const, tuple<tp_args...> const& a_tup)
  noexcept(tp_noexcept)->storage_type
  {
    return dango::detail::make_tuple_storage<storage_type, tp_noexcept>(a_tup.template get<tp_indices>()...);
  }

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename... tp_args>
  static constexpr auto
  construct_help
  (dango::index_seq<tp_indices...> const, tuple<tp_args...>&& a_tup)
  noexcept(tp_noexcept)->storage_type
  {
    return dango::detail::make_tuple_storage<storage_type, tp_noexcept>(dango::move(a_tup).template get<tp_indices>()...);
  }

public:

/*** converting copy-construct ***/

#define DANGO_TUPLE_SPEC(noex, cvref) \
  ( ... && dango::is##noex##brace_constructible<tp_types, dango::tuple_get_type<dango::tuple_model cvref, tp_args>>)

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    (sizeof...(tp_args) != dango::usize(1)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    DANGO_TUPLE_SPEC(_, const&)
  )
  explicit(!( ... && dango::is_convertible_arg<dango::tuple_get_type<dango::tuple_model const&, tp_args>, tp_types>))
  constexpr
  tuple
  (dango::tuple<tp_args...> const& a_tup)
  noexcept(DANGO_TUPLE_SPEC(_noexcept_, const&)):
  m_storage{ construct_help<DANGO_TUPLE_SPEC(_noexcept_, const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup) }
  {

  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    (sizeof...(tp_args) == dango::usize(1)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    DANGO_TUPLE_SPEC(_, const&) &&
    !( ... && dango::is_brace_constructible<tp_types, dango::tuple<tp_args...> const&>) &&
    !( ... && dango::is_convertible_arg<dango::tuple<tp_args...> const&, tp_types>)
  )
  explicit(!( ... && dango::is_convertible_arg<dango::tuple_get_type<dango::tuple_model const&, tp_args>, tp_types>))
  constexpr
  tuple
  (dango::tuple<tp_args...> const& a_tup)
  noexcept(DANGO_TUPLE_SPEC(_noexcept_, const&)):
  m_storage{ construct_help<DANGO_TUPLE_SPEC(_noexcept_, const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup) }
  {

  }

/*** converting move-construct ***/

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    (sizeof...(tp_args) != dango::usize(1)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    DANGO_TUPLE_SPEC(_, &&)
  )
  explicit(!( ... && dango::is_convertible_arg<dango::tuple_get_type<dango::tuple_model&&, tp_args>, tp_types>))
  constexpr
  tuple
  (dango::tuple<tp_args...>&& a_tup)
  noexcept(DANGO_TUPLE_SPEC(_noexcept_, &&)):
  m_storage{ construct_help<DANGO_TUPLE_SPEC(_noexcept_, &&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup)) }
  {

  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    (sizeof...(tp_args) == dango::usize(1)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    DANGO_TUPLE_SPEC(_, &&) &&
    !( ... && dango::is_brace_constructible<tp_types, dango::tuple<tp_args...>&&>) &&
    !( ... && dango::is_convertible_arg<dango::tuple<tp_args...>&&, tp_types>)
  )
  explicit(!( ... && dango::is_convertible_arg<dango::tuple_get_type<dango::tuple_model&&, tp_args>, tp_types>))
  constexpr
  tuple
  (dango::tuple<tp_args...>&& a_tup)
  noexcept(DANGO_TUPLE_SPEC(_noexcept_, &&)):
  m_storage{ construct_help<DANGO_TUPLE_SPEC(_noexcept_, &&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup)) }
  {

  }

#undef DANGO_TUPLE_SPEC

private:

/*** assign helpers ***/

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename... tp_args>
  constexpr auto
  assign_help
  (dango::index_seq<tp_indices...> const, dango::tuple<tp_args...> const& a_tup)& noexcept(tp_noexcept)->tuple&
  {
    ( ... , void(get<tp_indices>() = a_tup.template get<tp_indices>()));

    return *this;
  }

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename... tp_args>
  constexpr auto
  assign_help
  (dango::index_seq<tp_indices...> const, dango::tuple<tp_args...> const& a_tup)const& noexcept(tp_noexcept)->tuple const&
  {
    ( ... , void(get<tp_indices>() = a_tup.template get<tp_indices>()));

    return *this;
  }

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename... tp_args>
  constexpr auto
  assign_help
  (dango::index_seq<tp_indices...> const, dango::tuple<tp_args...>&& a_tup)& noexcept(tp_noexcept)->tuple&
  {
    ( ... , void(get<tp_indices>() = dango::move(a_tup).template get<tp_indices>()));

    return *this;
  }

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename... tp_args>
  constexpr auto
  assign_help
  (dango::index_seq<tp_indices...> const, dango::tuple<tp_args...>&& a_tup)const& noexcept(tp_noexcept)->tuple const&
  {
    ( ... , void(get<tp_indices>() = dango::move(a_tup).template get<tp_indices>()));

    return *this;
  }

public:

/*** assign ***/

#define DANGO_TUPLE_SPEC(noex, lcvref, rcvref) \
  ( ... && dango::is##noex##assignable<dango::tuple_get_type<dango::tuple_model lcvref, tp_types>, dango::tuple_get_type<dango::tuple_model rcvref, tp_args>>)

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    DANGO_TUPLE_SPEC(_, &, const&)
  )
  constexpr auto
  operator =
  (dango::tuple<tp_args...> const& a_tup)& noexcept(DANGO_TUPLE_SPEC(_noexcept_, &, const&))->tuple&
  {
    return assign_help<DANGO_TUPLE_SPEC(_noexcept_, &, const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup);
  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    DANGO_TUPLE_SPEC(_, const&, const&)
  )
  constexpr auto
  operator =
  (dango::tuple<tp_args...> const& a_tup)const& noexcept(DANGO_TUPLE_SPEC(_noexcept_, const&, const&))->tuple const&
  {
    return assign_help<DANGO_TUPLE_SPEC(_noexcept_, const&, const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup);
  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    DANGO_TUPLE_SPEC(_, &, const&)
  )
  constexpr auto
  operator =
  (dango::tuple<tp_args...> const& a_tup)&& noexcept(DANGO_TUPLE_SPEC(_noexcept_, &, const&))->tuple
  {
    return assign_help<DANGO_TUPLE_SPEC(_noexcept_, &, const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup);
  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    DANGO_TUPLE_SPEC(_, const&, const&)
  )
  constexpr auto
  operator =
  (dango::tuple<tp_args...> const& a_tup)const&& noexcept(DANGO_TUPLE_SPEC(_noexcept_, const&, const&))->tuple
  {
    return assign_help<DANGO_TUPLE_SPEC(_noexcept_, const&, const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup);
  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    DANGO_TUPLE_SPEC(_, &, &&)
  )
  constexpr auto
  operator =
  (dango::tuple<tp_args...>&& a_tup)& noexcept(DANGO_TUPLE_SPEC(_noexcept_, &, &&))->tuple&
  {
    return assign_help<DANGO_TUPLE_SPEC(_noexcept_, &, &&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup));
  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    DANGO_TUPLE_SPEC(_, const&, &&)
  )
  constexpr auto
  operator =
  (dango::tuple<tp_args...>&& a_tup)const& noexcept(DANGO_TUPLE_SPEC(_noexcept_, const&, &&))->tuple const&
  {
    return assign_help<DANGO_TUPLE_SPEC(_noexcept_, const&, &&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup));
  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    DANGO_TUPLE_SPEC(_, &, &&)
  )
  constexpr auto
  operator =
  (dango::tuple<tp_args...>&& a_tup)&& noexcept(DANGO_TUPLE_SPEC(_noexcept_, &, &&))->tuple
  {
    return assign_help<DANGO_TUPLE_SPEC(_noexcept_, &, &&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup));
  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    DANGO_TUPLE_SPEC(_, const&, &&)
  )
  constexpr auto
  operator =
  (dango::tuple<tp_args...>&& a_tup)const&& noexcept(DANGO_TUPLE_SPEC(_noexcept_, const&, &&))->tuple
  {
    return assign_help<DANGO_TUPLE_SPEC(_noexcept_, const&, &&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup));
  }

#undef DANGO_TUPLE_SPEC

/*** special member assign ***/

  constexpr auto operator = (tuple const&)& noexcept = delete;
  constexpr auto
  operator =
  (tuple const& a_tup)&
  noexcept(( ... && dango::is_noexcept_assignable<tp_types, tp_types>))->tuple&
  requires(( ... && dango::is_assignable<tp_types, tp_types>))
  {
    return assign_help<( ... && dango::is_noexcept_assignable<tp_types, tp_types>)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup);
  }

  constexpr auto operator = (tuple const&)const& noexcept = delete;
  constexpr auto
  operator =
  (tuple const& a_tup)const&
  noexcept(( ... && dango::is_noexcept_assignable<tp_types, tp_types>))->tuple const&
  requires(( ... && dango::is_assignable<tp_types, tp_types>))
  {
    return assign_help<( ... && dango::is_noexcept_assignable<tp_types, tp_types>)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup);
  }

  constexpr auto operator = (tuple const&)&& noexcept = delete;
  constexpr auto
  operator =
  (tuple const& a_tup)&&
  noexcept(( ... && dango::is_noexcept_assignable<tp_types, tp_types>))->tuple
  requires(( ... && dango::is_assignable<tp_types, tp_types>))
  {
    return assign_help<( ... && dango::is_noexcept_assignable<tp_types, tp_types>)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup);
  }

  constexpr auto operator = (tuple const&)const&& noexcept = delete;
  constexpr auto
  operator =
  (tuple const& a_tup)const&&
  noexcept(( ... && dango::is_noexcept_assignable<tp_types, tp_types>))->tuple
  requires(( ... && dango::is_assignable<tp_types, tp_types>))
  {
    return assign_help<( ... && dango::is_noexcept_assignable<tp_types, tp_types>)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup);
  }

  constexpr auto operator = (tuple&&)& noexcept = delete;
  constexpr auto
  operator =
  (tuple&& a_tup)&
  noexcept(( ... && dango::is_noexcept_assignable<tp_types, tp_types>))->tuple&
  requires(( ... && dango::is_assignable<tp_types, tp_types>))
  {
    return assign_help<( ... && dango::is_noexcept_assignable<tp_types, tp_types>)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup));
  }

  constexpr auto operator = (tuple&&)const& noexcept = delete;
  constexpr auto
  operator =
  (tuple&& a_tup)const&
  noexcept(( ... && dango::is_noexcept_assignable<tp_types, tp_types>))->tuple const&
  requires(( ... && dango::is_assignable<tp_types, tp_types>))
  {
    return assign_help<( ... && dango::is_noexcept_assignable<tp_types, tp_types>)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup));
  }

  constexpr auto operator = (tuple&&)&& noexcept = delete;
  constexpr auto
  operator =
  (tuple&& a_tup)&&
  noexcept(( ... && dango::is_noexcept_assignable<tp_types, tp_types>))->tuple
  requires(( ... && dango::is_assignable<tp_types, tp_types>))
  {
    return assign_help<( ... && dango::is_noexcept_assignable<tp_types, tp_types>)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup));
  }

  constexpr auto operator = (tuple&&)const&& noexcept = delete;
  constexpr auto
  operator =
  (tuple&& a_tup)const&&
  noexcept(( ... && dango::is_noexcept_assignable<tp_types, tp_types>))->tuple
  requires(( ... && dango::is_assignable<tp_types, tp_types>))
  {
    return assign_help<( ... && dango::is_noexcept_assignable<tp_types, tp_types>)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup));
  }

private:

/*** operator ->* helpers ***/

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename tp_func>
  constexpr auto
  call
  (dango::index_seq<tp_indices...> const, tp_func&& a_func)&
  noexcept(tp_noexcept)->decltype(auto)
  {
    return dango::forward<tp_func>(a_func)(static_cast<tuple&>(*this).get<tp_indices>()...);
  }

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename tp_func>
  constexpr auto
  call
  (dango::index_seq<tp_indices...> const, tp_func&& a_func)&&
  noexcept(tp_noexcept)->decltype(auto)
  {
    return dango::forward<tp_func>(a_func)(static_cast<tuple&&>(*this).get<tp_indices>()...);
  }

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename tp_func>
  constexpr auto
  call
  (dango::index_seq<tp_indices...> const, tp_func&& a_func)const&
  noexcept(tp_noexcept)->decltype(auto)
  {
    return dango::forward<tp_func>(a_func)(static_cast<tuple const&>(*this).get<tp_indices>()...);
  }

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename tp_func>
  constexpr auto
  call
  (dango::index_seq<tp_indices...> const, tp_func&& a_func)const&&
  noexcept(tp_noexcept)->decltype(auto)
  {
    return dango::forward<tp_func>(a_func)(static_cast<tuple const&&>(*this).get<tp_indices>()...);
  }

public:

/*** operator ->* ***/

#define DANGO_TUPLE_SPEC(noex, cvref) \
  dango::is##noex##callable<tp_func, dango::tuple_get_type<dango::tuple_model cvref, tp_types>...>

  template
  <typename tp_func>
  requires(DANGO_TUPLE_SPEC(_, &))
  constexpr auto
  operator ->*
  (tp_func&& a_func)&
  noexcept(DANGO_TUPLE_SPEC(_noexcept_, &))->decltype(auto)
  {
    return static_cast<tuple&>(*this).call<DANGO_TUPLE_SPEC(_noexcept_, &)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func));
  }

  template
  <typename tp_func>
  requires(DANGO_TUPLE_SPEC(_, &&))
  constexpr auto
  operator ->*
  (tp_func&& a_func)&&
  noexcept(DANGO_TUPLE_SPEC(_noexcept_, &&))->decltype(auto)
  {
    return static_cast<tuple&&>(*this).call<DANGO_TUPLE_SPEC(_noexcept_, &&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func));
  }

  template
  <typename tp_func>
  requires(DANGO_TUPLE_SPEC(_, const&))
  constexpr auto
  operator ->*
  (tp_func&& a_func)const&
  noexcept(DANGO_TUPLE_SPEC(_noexcept_, const&))->decltype(auto)
  {
    return static_cast<tuple const&>(*this).call<DANGO_TUPLE_SPEC(_noexcept_, const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func));
  }

  template
  <typename tp_func>
  requires(DANGO_TUPLE_SPEC(_, const&&))
  constexpr auto
  operator ->*
  (tp_func&& a_func)const&&
  noexcept(DANGO_TUPLE_SPEC(_noexcept_, const&&))->decltype(auto)
  {
    return static_cast<tuple const&&>(*this).call<DANGO_TUPLE_SPEC(_noexcept_, const&&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func));
  }

#undef DANGO_TUPLE_SPEC

private:

/*** dango_operator_swap ***/

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename... tp_args>
  constexpr void
  swap_help
  (dango::index_seq<tp_indices...> const, dango::tuple<tp_args...> const& a_tup)const& noexcept(tp_noexcept)
  {
    ( ... , (dango::swap(get<tp_indices>(), a_tup.template get<tp_indices>())));
  }

public:

#define DANGO_TUPLE_SPEC(noex, lcvref, rcvref) \
  ( ... && dango::is##noex##swappable<dango::tuple_get_type<dango::tuple_model lcvref, tp_types>, dango::tuple_get_type<dango::tuple_model rcvref, tp_args>>)

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    ( ... && dango::is_ref<tp_args>) &&
    DANGO_TUPLE_SPEC(_, const&, const&)
  )
  constexpr void
  dango_operator_swap
  (dango::tuple<tp_args...> const& a_tup)const& noexcept(DANGO_TUPLE_SPEC(_noexcept_, const&, const&))
  {
    swap_help<DANGO_TUPLE_SPEC(_noexcept_, const&, const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup);
  }

#undef DANGO_TUPLE_SPEC

private:
  storage_type m_storage;
};

/*** empty tuple ***/

template<>
class
dango::
tuple<>
{
public:
  static constexpr auto
  size()noexcept->dango::usize
  {
    return dango::usize(0);
  }
public:
  DANGO_ALL_DEFAULT_CONSTEXPR_NOEXCEPT(tuple, false)
public:
  template
  <typename tp_func>
  requires(dango::is_callable<tp_func>)
  constexpr auto
  operator ->*
  (tp_func&& a_func)const noexcept(dango::is_noexcept_callable<tp_func>)->decltype(auto)
  {
    return dango::forward<tp_func>(a_func)();
  }

  constexpr void dango_operator_swap(dango::tuple<> const&)const& noexcept{ }
};

/*** deduction guide and make ***/

namespace
dango
{
  template
  <typename... tp_args>
  explicit(!( ... && dango::is_convertible_arg<tp_args, dango::emplacer_return_type_decay<tp_args>>))
  tuple(tp_args&&...)->tuple<dango::emplacer_return_type_decay<tp_args>...>;

  template
  <typename... tp_args>
  requires(( ... && dango::is_brace_constructible<dango::emplacer_return_type_decay<tp_args>, tp_args>))
  constexpr auto
  make_tuple
  (tp_args&&... a_args)
  noexcept(( ... && dango::is_noexcept_brace_constructible<dango::emplacer_return_type_decay<tp_args>, tp_args>))->auto
  {
    return dango::tuple<dango::emplacer_return_type_decay<tp_args>...>{ dango::forward<tp_args>(a_args)... };
  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == dango::usize(2)) &&
    ( ... && dango::is_brace_constructible<dango::emplacer_return_type_decay<tp_args>, tp_args>)
  )
  constexpr auto
  make_pair
  (tp_args&&... a_args)
  noexcept(( ... && dango::is_noexcept_brace_constructible<dango::emplacer_return_type_decay<tp_args>, tp_args>))->auto
  {
    return dango::tuple<dango::emplacer_return_type_decay<tp_args>...>{ dango::forward<tp_args>(a_args)... };
  }

  template
  <typename... tp_args>
  requires(( ... && dango::is_lvalue_ref<tp_args>))
  constexpr auto
  tie_as_tuple
  (tp_args&&... a_args)noexcept->auto
  {
    return dango::tuple<tp_args...>{ a_args... };
  }

  template
  <typename... tp_args>
  constexpr auto
  ctie_as_tuple
  (tp_args&&... a_args)noexcept->auto
  {
    return dango::tuple<dango::remove_ref<tp_args> const&...>{ a_args... };
  }

  template
  <typename... tp_args>
  constexpr auto
  forward_as_tuple
  (tp_args&&... a_args)noexcept->auto
  {
    return dango::tuple<tp_args&&...>{ dango::forward<tp_args>(a_args)... };
  }
}

/*** get for tuple ***/

namespace
dango::custom
{
  template
  <typename... tp_types>
  struct operator_get<dango::tuple<tp_types...>>;
}

template
<typename... tp_types>
struct
dango::
custom::
operator_get<dango::tuple<tp_types...>>
final
{
  using tuple_type = dango::tuple<tp_types...>;

  template
  <dango::usize tp_index, dango::is_same_ignore_cvref<tuple_type> tp_tuple>
  requires(tp_index < sizeof...(tp_types))
  static constexpr auto
  get
  (tp_tuple&& a_tup)noexcept->decltype(auto)
  {
    return dango::forward<tp_tuple>(a_tup).template get<tp_index>();
  }

  DANGO_UNCONSTRUCTIBLE(operator_get)
};

/*** equals and compare for tuples ***/

namespace
dango::custom
{
  template
  <typename... tp_types>
  requires(( ... && dango::is_equatable<dango::tuple_get_type<dango::tuple_model const&, tp_types>, dango::tuple_get_type<dango::tuple_model const&, tp_types>>))
  struct
  operator_equals<dango::tuple<tp_types...>, dango::tuple<tp_types...>>;

  template
  <typename... tp_lhs, typename... tp_rhs>
  struct
  operator_equals<dango::tuple<tp_lhs...>, dango::tuple<tp_rhs...>>;

  template
  <typename... tp_lhs, typename... tp_rhs>
  requires
  (
    dango::detail::tuple_is_comparable
    <
      dango::detail::tuple_pack<dango::tuple_get_type<dango::tuple_model const&, tp_lhs>...>,
      dango::detail::tuple_pack<dango::tuple_get_type<dango::tuple_model const&, tp_rhs>...>
    >
  )
  struct
  operator_compare<dango::tuple<tp_lhs...>, dango::tuple<tp_rhs...>>;
}

template
<typename... tp_types>
requires(( ... && dango::is_equatable<dango::tuple_get_type<dango::tuple_model const&, tp_types>, dango::tuple_get_type<dango::tuple_model const&, tp_types>>))
struct
dango::
custom::
operator_equals<dango::tuple<tp_types...>, dango::tuple<tp_types...>>
final
{
private:
  using tuple_type = dango::tuple<tp_types...>;

  static inline constexpr bool const c_noexcept =
    ( ... && dango::is_noexcept_equatable<dango::tuple_get_type<dango::tuple_model const&, tp_types>, dango::tuple_get_type<dango::tuple_model const&, tp_types>>);

  template
  <dango::usize... tp_indices>
  static constexpr auto
  equals_help
  (dango::index_seq<tp_indices...> const, tuple_type const& a_lhs, tuple_type const& a_rhs)noexcept(c_noexcept)->bool
  {
    return ( ... && dango::equals(dango::get<tp_indices>(a_lhs), dango::get<tp_indices>(a_rhs)));
  }
public:
  static constexpr auto
  equals
  (tuple_type const& a_lhs, tuple_type const& a_rhs)noexcept(c_noexcept)->bool
  {
    return equals_help(dango::make_index_seq<sizeof...(tp_types)>{ }, a_lhs, a_rhs);
  }

  DANGO_UNCONSTRUCTIBLE(operator_equals)
};

template
<typename... tp_lhs, typename... tp_rhs>
struct
dango::
custom::
operator_equals<dango::tuple<tp_lhs...>, dango::tuple<tp_rhs...>>
final
{
  static constexpr auto
  equals
  (dango::tuple<tp_lhs...> const&, dango::tuple<tp_rhs...> const&)noexcept->bool
  {
    return false;
  }

  DANGO_UNCONSTRUCTIBLE(operator_equals)
};

template
<typename... tp_lhs, typename... tp_rhs>
requires
(
  dango::detail::tuple_is_comparable
  <
    dango::detail::tuple_pack<dango::tuple_get_type<dango::tuple_model const&, tp_lhs>...>,
    dango::detail::tuple_pack<dango::tuple_get_type<dango::tuple_model const&, tp_rhs>...>
  >
)
struct
dango::
custom::
operator_compare<dango::tuple<tp_lhs...>, dango::tuple<tp_rhs...>>
final
{
private:
  using tuple_lhs = dango::tuple<tp_lhs...>;
  using tuple_rhs = dango::tuple<tp_rhs...>;

  static inline constexpr bool const c_noexcept =
    dango::detail::tuple_is_noexcept_comparable
    <
      dango::detail::tuple_pack<dango::tuple_get_type<dango::tuple_model const&, tp_lhs>...>,
      dango::detail::tuple_pack<dango::tuple_get_type<dango::tuple_model const&, tp_rhs>...>
    >;

  template
  <dango::usize... tp_indices>
  static constexpr auto
  compare_help
  (dango::index_seq<tp_indices...> const, tuple_lhs const& a_lhs, tuple_rhs const& a_rhs)noexcept(c_noexcept)->auto
  {
    using ret_type =
      dango::comparison::common_type<decltype(dango::compare(dango::get<tp_indices>(a_lhs), dango::get<tp_indices>(a_rhs)))...>;

    ret_type a_ret{ 0 };

    [[maybe_unused]] bool const a_temp =
      ( ... && (a_ret = ret_type{ dango::compare(dango::get<tp_indices>(a_lhs), dango::get<tp_indices>(a_rhs)) }).is_eq());

    return a_ret;
  }
public:
  static constexpr auto
  compare
  (tuple_lhs const& a_lhs, tuple_rhs const& a_rhs)noexcept(c_noexcept)->auto
  {
    constexpr auto const c_size = dango::min(sizeof...(tp_lhs), sizeof...(tp_rhs));

    auto a_ret = compare_help(dango::make_index_seq<c_size>{ }, a_lhs, a_rhs);

    using ret_type = decltype(a_ret);

    if(a_ret.is_eq())
    {
      a_ret = ret_type{ dango::compare(sizeof...(tp_lhs), sizeof...(tp_rhs)) };
    }

    return a_ret;
  }

  DANGO_UNCONSTRUCTIBLE(operator_compare)
};

/*** structured bindings ***/

namespace
dango::detail
{
  template
  <dango::usize tp_index, typename tp_first, typename... tp_next>
  struct tuple_element_help;

  template
  <typename tp_first, typename... tp_next>
  struct
  tuple_element_help<dango::usize(0), tp_first, tp_next...>;
}

template
<dango::usize tp_index, typename tp_first, typename... tp_next>
struct
dango::
detail::
tuple_element_help
final
{
  using type =
    typename dango::detail::tuple_element_help<tp_index - dango::usize(1), tp_next...>::type;

  DANGO_UNCONSTRUCTIBLE(tuple_element_help)
};

template
<typename tp_first, typename... tp_next>
struct
dango::
detail::
tuple_element_help<dango::usize(0), tp_first, tp_next...>
final
{
  using type = tp_first;

  DANGO_UNCONSTRUCTIBLE(tuple_element_help)
};

namespace
std
{
  template
  <typename... tp_types>
  struct
  tuple_size<dango::tuple<tp_types...>>
  {
    static inline constexpr dango::usize const value = sizeof...(tp_types);

    DANGO_UNCONSTRUCTIBLE(tuple_size)
  };

  template
  <dango::usize tp_index, typename... tp_types>
  struct
  tuple_element
  <tp_index, dango::tuple<tp_types...>>
  {
    using type =
      typename dango::detail::tuple_element_help<tp_index, tp_types...>::type;

    DANGO_UNCONSTRUCTIBLE(tuple_element)
  };

  template
  <dango::usize tp_index>
  struct
  tuple_element<tp_index, dango::tuple<>>
  {
    DANGO_UNCONSTRUCTIBLE(tuple_element)
  };
}

#endif // DANGO_TUPLE_HPP_INCLUDED
