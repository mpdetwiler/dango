#ifndef DANGO_TUPLE_HPP_INCLUDED
#define DANGO_TUPLE_HPP_INCLUDED

#include "dango-util.hpp"
#include "dango-hash.hpp"

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
  concept is_tuple =
    dango::detail::is_tuple_help<dango::remove_cv<tp_type>>;

  template
  <typename tp_type>
  concept is_tuple_ignore_ref =
    dango::is_tuple<tp_type> || dango::is_tuple<dango::remove_ref<tp_type>>;
}

/*** tuple_at_type ***/

namespace
dango
{
  template
  <typename tp_type>
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
  <typename tp_model>
  struct
  tuple_at_type_help
  final
  {
    DANGO_UNCONSTRUCTIBLE(tuple_at_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_type>
  struct
  tuple_at_type_help<dango::tuple_model<tp_type>&&>
  final
  {
    using type = dango::conditional<dango::is_ref<tp_type>, tp_type, tp_type&&>;

    DANGO_UNCONSTRUCTIBLE(tuple_at_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_type>
  struct
  tuple_at_type_help<dango::tuple_model<tp_type> const&&>
  final
  {
    using type = dango::conditional<dango::is_ref<tp_type>, tp_type, tp_type const&&>;

    DANGO_UNCONSTRUCTIBLE(tuple_at_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_type>
  struct
  tuple_at_type_help<dango::tuple_model<tp_type> volatile&&>
  final
  {
    DANGO_UNCONSTRUCTIBLE(tuple_at_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_type>
  struct
  tuple_at_type_help<dango::tuple_model<tp_type> const volatile&&>
  final
  {
    DANGO_UNCONSTRUCTIBLE(tuple_at_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_type>
  struct
  tuple_at_type_help<dango::tuple_model<tp_type>>
  final
  {
    using type = typename dango::detail::tuple_at_type_help<dango::tuple_model<tp_type>&&>::type;

    DANGO_UNCONSTRUCTIBLE(tuple_at_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_type>
  struct
  tuple_at_type_help<dango::tuple_model<tp_type> const>
  final
  {
    using type = typename dango::detail::tuple_at_type_help<dango::tuple_model<tp_type> const&&>::type;

    DANGO_UNCONSTRUCTIBLE(tuple_at_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_type>
  struct
  tuple_at_type_help<dango::tuple_model<tp_type> volatile>
  final
  {
    DANGO_UNCONSTRUCTIBLE(tuple_at_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_type>
  struct
  tuple_at_type_help<dango::tuple_model<tp_type> const volatile>
  final
  {
    DANGO_UNCONSTRUCTIBLE(tuple_at_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_type>
  struct
  tuple_at_type_help<dango::tuple_model<tp_type>&>
  final
  {
    using type = dango::conditional<dango::is_ref<tp_type>, tp_type, tp_type&>;

    DANGO_UNCONSTRUCTIBLE(tuple_at_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_type>
  struct
  tuple_at_type_help<dango::tuple_model<tp_type> const&>
  final
  {
    using type = dango::conditional<dango::is_ref<tp_type>, tp_type, tp_type const&>;

    DANGO_UNCONSTRUCTIBLE(tuple_at_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_type>
  struct
  tuple_at_type_help<dango::tuple_model<tp_type> volatile&>
  final
  {
    DANGO_UNCONSTRUCTIBLE(tuple_at_type_help)
  };

  template
  <dango::tuple_constraint_spec tp_type>
  struct
  tuple_at_type_help<dango::tuple_model<tp_type> const volatile&>
  final
  {
    DANGO_UNCONSTRUCTIBLE(tuple_at_type_help)
  };
}

namespace
dango
{
  template
  <typename tp_model>
  requires(requires{ typename dango::detail::tuple_at_type_help<tp_model>::type; })
  using tuple_at_type =
    typename dango::detail::tuple_at_type_help<tp_model>::type;
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
  tuple_storage()
  noexcept(dango::is_noexcept_default_constructible<value_type>)
  requires(dango::is_default_constructible<value_type>) = default;

  template
  <typename tp_arg>
  requires(!dango::is_same_ignore_cvref<tp_arg, tuple_storage>)
  explicit constexpr
  tuple_storage
  (tp_arg&& a_arg)
  noexcept(dango::is_noexcept_brace_constructible<value_type, tp_arg>):
  m_value{ dango::forward<tp_arg>(a_arg) }
  { }

  constexpr tuple_storage(tuple_storage const&)noexcept = delete;
  constexpr
  tuple_storage
  (tuple_storage const&)
  noexcept(dango::is_noexcept_copy_constructible<value_type>)
  requires(dango::is_copy_constructible<value_type>) = default;

  constexpr tuple_storage(tuple_storage&&)noexcept = delete;
  constexpr
  tuple_storage
  (tuple_storage&&)
  noexcept(dango::is_noexcept_move_constructible<value_type>)
  requires(dango::is_move_constructible<value_type>) = default;

  constexpr ~tuple_storage()noexcept = default;

  constexpr auto operator = (tuple_storage const&)& noexcept = delete;
  constexpr auto
  operator =
  (tuple_storage const&)&
  noexcept(dango::is_noexcept_copy_assignable<value_type>)->tuple_storage&
  requires(dango::is_copy_assignable<value_type>) = default;

  constexpr auto operator = (tuple_storage&&)& noexcept = delete;
  constexpr auto
  operator =
  (tuple_storage&&)&
  noexcept(dango::is_noexcept_move_assignable<value_type>)->tuple_storage&
  requires(dango::is_move_assignable<value_type>) = default;

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && dango::is_ref<value_type>)
  constexpr auto
  at()const noexcept->value_type
  {
    return dango::forward<value_type>(m_value);
  }

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && !dango::is_ref<value_type>)
  constexpr auto
  at()& noexcept->value_type&
  {
    return m_value;
  }

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && !dango::is_ref<value_type>)
  constexpr auto
  at()&& noexcept->value_type&&
  {
    return dango::move(m_value);
  }

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && !dango::is_ref<value_type>)
  constexpr auto
  at()const& noexcept->value_type const&
  {
    return m_value;
  }

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && !dango::is_ref<value_type>)
  constexpr auto
  at()const&& noexcept->value_type const&&
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
  noexcept(dango::is_noexcept_default_constructible<value_type> &&  dango::is_noexcept_default_constructible<super_type>)
  requires(dango::is_default_constructible<value_type> &&  dango::is_default_constructible<super_type>) = default;

  template
  <typename tp_arg, typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_next))
  explicit constexpr
  tuple_storage
  (tp_arg&& a_arg, tp_args&&... a_args)
  noexcept(dango::is_noexcept_brace_constructible<value_type, tp_arg> && dango::is_noexcept_brace_constructible<super_type, tp_args...>):
  super_type{ dango::forward<tp_args>(a_args)... },
  m_value{ dango::forward<tp_arg>(a_arg) }
  { }

  constexpr tuple_storage(tuple_storage const&)noexcept = delete;
  constexpr
  tuple_storage
  (tuple_storage const&)
  noexcept(dango::is_noexcept_copy_constructible<value_type> && dango::is_noexcept_copy_constructible<super_type>)
  requires(dango::is_copy_constructible<value_type> && dango::is_copy_constructible<super_type>) = default;

  constexpr tuple_storage(tuple_storage&&)noexcept = delete;
  constexpr
  tuple_storage
  (tuple_storage&&)
  noexcept(dango::is_noexcept_move_constructible<value_type> && dango::is_noexcept_move_constructible<super_type>)
  requires(dango::is_move_constructible<value_type> && dango::is_move_constructible<super_type>) = default;

  constexpr ~tuple_storage()noexcept = default;

  constexpr auto operator = (tuple_storage const&)& noexcept = delete;
  constexpr auto
  operator =
  (tuple_storage const&)&
  noexcept(dango::is_noexcept_copy_assignable<value_type> && dango::is_noexcept_copy_assignable<super_type>)->tuple_storage&
  requires(dango::is_copy_assignable<value_type> && dango::is_copy_assignable<super_type>) = default;

  constexpr auto operator = (tuple_storage&&)& noexcept = delete;
  constexpr auto
  operator =
  (tuple_storage&&)&
  noexcept(dango::is_noexcept_move_assignable<value_type> && dango::is_noexcept_move_assignable<super_type>)->tuple_storage&
  requires(dango::is_move_assignable<value_type> && dango::is_move_assignable<super_type>) = default;

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && dango::is_ref<value_type>)
  constexpr auto
  at()const noexcept->value_type
  {
    return dango::forward<value_type>(m_value);
  }

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && !dango::is_ref<value_type>)
  constexpr auto
  at()& noexcept->value_type&
  {
    return m_value;
  }

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && !dango::is_ref<value_type>)
  constexpr auto
  at()&& noexcept->value_type&&
  {
    return dango::move(m_value);
  }

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && !dango::is_ref<value_type>)
  constexpr auto
  at()const& noexcept->value_type const&
  {
    return m_value;
  }

  template
  <dango::usize tp_index>
  requires((tp_index == dango::usize(0)) && !dango::is_ref<value_type>)
  constexpr auto
  at()const&& noexcept->value_type const&&
  {
    return dango::move(m_value);
  }

  template
  <dango::usize tp_index>
  requires(tp_index != dango::usize(0))
  constexpr auto
  at()& noexcept->decltype(auto)
  {
    return static_cast<super_type&>(*this).template at<tp_index - dango::usize(1)>();
  }

  template
  <dango::usize tp_index>
  requires(tp_index != dango::usize(0))
  constexpr auto
  at()&& noexcept->decltype(auto)
  {
    return static_cast<super_type&&>(*this).template at<tp_index - dango::usize(1)>();
  }

  template
  <dango::usize tp_index>
  requires(tp_index != dango::usize(0))
  constexpr auto
  at()const& noexcept->decltype(auto)
  {
    return static_cast<super_type const&>(*this).template at<tp_index - dango::usize(1)>();
  }

  template
  <dango::usize tp_index>
  requires(tp_index != dango::usize(0))
  constexpr auto
  at()const&& noexcept->decltype(auto)
  {
    return static_cast<super_type const&&>(*this).template at<tp_index - dango::usize(1)>();
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

  template
  <bool tp_noexcept>
  struct
  private_construct_tag
  final
  {
    DANGO_TAG_TYPE(private_construct_tag)
  };
private:
  template
  <bool tp_noexcept, typename... tp_args>
  explicit constexpr
  tuple
  (private_construct_tag<tp_noexcept> const, tp_args&&... a_args)noexcept(tp_noexcept):
  m_storage{ dango::detail::make_tuple_storage<storage_type, tp_noexcept>(dango::forward<tp_args>(a_args)...) }
  { }
private:
  using construct_help_prio = dango::priority_tag<dango::uint(2)>;

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_types))
  static constexpr auto
  construct_help
  (dango::priority_tag<dango::uint(2)> const, tp_args&&... a_args)
  noexcept(( ... && dango::is_noexcept_brace_constructible<tp_types, tp_args>))->tuple
  requires(( ... && dango::is_brace_constructible<tp_types, tp_args>))
  {
    constexpr bool const c_noexcept =
      ( ... && dango::is_noexcept_brace_constructible<tp_types, tp_args>);

    return tuple{ private_construct_tag<c_noexcept>{ }, dango::forward<tp_args>(a_args)... };
  }

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_types))
  static constexpr auto
  construct_help
  (dango::priority_tag<dango::uint(1)> const, dango::tuple<tp_args...> const& a_tup)
  noexcept(( ... && dango::is_noexcept_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args> const&>>))->tuple
  requires(( ... && dango::is_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args> const&>>))
  {
    constexpr bool const c_noexcept =
      ( ... && dango::is_noexcept_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args> const&>>);

    constexpr auto const c_make =
      []<dango::usize... tp_indices>
      (dango::index_seq<tp_indices...> const, dango::tuple<tp_args...> const& a_arg)constexpr
      noexcept(c_noexcept)->tuple
      {
        return tuple{ private_construct_tag<c_noexcept>{ }, a_arg.template at<tp_indices>()... };
      };

    return c_make(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup);
  }

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_types))
  static constexpr auto
  construct_help
  (dango::priority_tag<dango::uint(1)> const, dango::tuple<tp_args...>&& a_tup)
  noexcept(( ... && dango::is_noexcept_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args>&&>>))->tuple
  requires(( ... && dango::is_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args>&&>>))
  {
    constexpr bool const c_noexcept =
      ( ... && dango::is_noexcept_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args>&&>>);

    constexpr auto const c_make =
      []<dango::usize... tp_indices>
      (dango::index_seq<tp_indices...> const, dango::tuple<tp_args...>&& a_arg)constexpr
      noexcept(c_noexcept)->tuple
      {
        return tuple{ private_construct_tag<c_noexcept>{ }, dango::move(a_arg).template at<tp_indices>()... };
      };

    return c_make(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup));
  }

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_types))
  static constexpr auto
  construct_explicit_help
  (dango::priority_tag<dango::uint(2)> const, dango::type_tag<tp_args> const...)noexcept->bool
  requires(( ... && dango::is_brace_constructible<tp_types, tp_args>))
  {
    return ( ... || !dango::is_convertible_arg<tp_args, tp_types>);
  }

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_types))
  static constexpr auto
  construct_explicit_help
  (dango::priority_tag<dango::uint(1)> const, dango::type_tag<dango::tuple<tp_args...>&> const)noexcept->bool
  requires(( ... && dango::is_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args> const&>>))
  {
    return ( ... || !dango::is_convertible_arg<dango::tuple_at_type<dango::tuple_model<tp_args> const&>, tp_types>);
  }

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_types))
  static constexpr auto
  construct_explicit_help
  (dango::priority_tag<dango::uint(1)> const, dango::type_tag<dango::tuple<tp_args...> const&> const)noexcept->bool
  requires(( ... && dango::is_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args> const&>>))
  {
    return ( ... || !dango::is_convertible_arg<dango::tuple_at_type<dango::tuple_model<tp_args> const&>, tp_types>);
  }

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_types))
  static constexpr auto
  construct_explicit_help
  (dango::priority_tag<dango::uint(1)> const, dango::type_tag<dango::tuple<tp_args...>&&> const)noexcept->bool
  requires(( ... && dango::is_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args>&&>>))
  {
    return ( ... || !dango::is_convertible_arg<dango::tuple_at_type<dango::tuple_model<tp_args>&&>, tp_types>);
  }

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_types))
  static constexpr auto
  construct_explicit_help
  (dango::priority_tag<dango::uint(1)> const, dango::type_tag<dango::tuple<tp_args...> const&&> const)noexcept->bool
  requires(( ... && dango::is_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args> const&>>))
  {
    return ( ... || !dango::is_convertible_arg<dango::tuple_at_type<dango::tuple_model<tp_args> const&>, tp_types>);
  }

  template
  <typename... tp_args>
  static constexpr auto
  construct_explicit_help
  (dango::priority_tag<dango::uint(0)> const, tp_args const...)noexcept->bool
  {
    return false;
  }
public:
  template
  <typename... tp_args>
  requires(sizeof...(tp_args) != dango::usize(0))
  explicit(construct_explicit_help(construct_help_prio{ }, dango::type_tag<tp_args&&>{ }...))
  constexpr
  tuple
  (tp_args&&... a_args)
  noexcept(requires{ { construct_help(construct_help_prio{ }, dango::declval<tp_args>()...) }noexcept; })
  requires(requires{ { construct_help(construct_help_prio{ }, dango::declval<tp_args>()...) }; }):
  tuple{ construct_help(construct_help_prio{ }, dango::forward<tp_args>(a_args)...) }
  { }

  constexpr tuple()noexcept = delete;
  constexpr
  tuple()
  noexcept(( ... && dango::is_noexcept_default_constructible<tp_types>))
  requires(( ... && dango::is_default_constructible<tp_types>)) = default;

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
  at()& noexcept->decltype(auto)
  {
    return m_storage.template at<sizeof...(tp_types) - dango::usize(1) - tp_index>();
  }

  template
  <dango::usize tp_index>
  requires(tp_index < sizeof...(tp_types))
  constexpr auto
  at()&& noexcept->decltype(auto)
  {
    return dango::move(m_storage).template at<sizeof...(tp_types) - dango::usize(1) - tp_index>();
  }

  template
  <dango::usize tp_index>
  requires(tp_index < sizeof...(tp_types))
  constexpr auto
  at()const& noexcept->decltype(auto)
  {
    return m_storage.template at<sizeof...(tp_types) - dango::usize(1) - tp_index>();
  }

  template
  <dango::usize tp_index>
  requires(tp_index < sizeof...(tp_types))
  constexpr auto
  at()const&& noexcept->decltype(auto)
  {
    return dango::move(m_storage).template at<sizeof...(tp_types) - dango::usize(1) - tp_index>();
  }

#define DANGO_TUPLE_DEFINE_NAMED_GET(index, name) \
  constexpr auto name()& noexcept->decltype(auto) \
  requires(dango::usize(index) < sizeof...(tp_types)) \
  { return static_cast<tuple&>(*this).at<dango::usize(index)>(); } \
  constexpr auto name()&& noexcept->decltype(auto) \
  requires(dango::usize(index) < sizeof...(tp_types)) \
  { return static_cast<tuple&&>(*this).at<dango::usize(index)>(); } \
  constexpr auto name()const& noexcept->decltype(auto) \
  requires(dango::usize(index) < sizeof...(tp_types)) \
  { return static_cast<tuple const&>(*this).at<dango::usize(index)>(); } \
  constexpr auto name()const&& noexcept->decltype(auto) \
  requires(dango::usize(index) < sizeof...(tp_types)) \
  { return static_cast<tuple const&&>(*this).at<dango::usize(index)>(); }

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

public:

/*** converting assign ***/

#define DANGO_TUPLE_DEFINE_ASSIGN(lcvref, rcvref, ret) \
  template \
  <typename... tp_args> \
  requires \
  ( \
    (sizeof...(tp_args) == sizeof...(tp_types)) && \
    !( ... && dango::is_same<tp_args, tp_types>) && \
    ( ... && !dango::is_ref<tp_types>) && \
    ( ... && dango::is_assignable<dango::tuple_at_type<dango::tuple_model<tp_types> lcvref>, dango::tuple_at_type<dango::tuple_model<tp_args> rcvref>>) \
  ) \
  constexpr auto \
  operator = \
  (dango::tuple<tp_args...> rcvref a_tup)lcvref \
  noexcept(( ... && dango::is_noexcept_assignable<dango::tuple_at_type<dango::tuple_model<tp_types> lcvref>, dango::tuple_at_type<dango::tuple_model<tp_args> rcvref>>))->ret \
  { \
    constexpr auto const c_assign = \
      []<dango::usize... tp_indices> \
      (dango::index_seq<tp_indices...> const, tuple lcvref a_this, dango::tuple<tp_args...> rcvref a_arg)constexpr \
      noexcept(( ... && dango::is_noexcept_assignable<dango::tuple_at_type<dango::tuple_model<tp_types> lcvref>, dango::tuple_at_type<dango::tuple_model<tp_args> rcvref>>))->void \
      { \
        ( ... , void(static_cast<tuple lcvref>(a_this).at<tp_indices>() = static_cast<dango::tuple<tp_args...> rcvref>(a_arg).template at<tp_indices>())); \
      }; \
    c_assign(dango::make_index_seq<sizeof...(tp_types)>{ }, static_cast<tuple lcvref>(*this), static_cast<dango::tuple<tp_args...> rcvref>(a_tup)); \
    return static_cast<tuple lcvref>(*this); \
  }

  DANGO_TUPLE_DEFINE_ASSIGN(&, const&, tuple&)
  DANGO_TUPLE_DEFINE_ASSIGN(&, &&, tuple&)

#undef DANGO_TUPLE_DEFINE_ASSIGN

public:

/*** operator ->* ***/

#define DANGO_TUPLE_DEFINE_ARROW_STAR(lcvref) \
  template \
  <typename tp_func> \
  requires(dango::is_callable<tp_func, dango::tuple_at_type<dango::tuple_model<tp_types> lcvref>...>) \
  constexpr auto \
  operator ->* \
  (tp_func&& a_func)lcvref \
  noexcept(dango::is_noexcept_callable<tp_func, dango::tuple_at_type<dango::tuple_model<tp_types> lcvref>...>)->decltype(auto) \
  { \
    constexpr auto const c_call = \
      []<dango::usize... tp_indices> \
      (dango::index_seq<tp_indices...> const, tp_func&& a_fn, tuple lcvref a_this)constexpr \
      noexcept(dango::is_noexcept_callable<tp_func, dango::tuple_at_type<dango::tuple_model<tp_types> lcvref>...>)->decltype(auto) \
      { \
        return dango::forward<tp_func>(a_fn)(static_cast<tuple lcvref>(a_this).at<tp_indices>()...); \
      }; \
    return c_call(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func), static_cast<tuple lcvref>(*this)); \
  }

  DANGO_TUPLE_DEFINE_ARROW_STAR(&)
  DANGO_TUPLE_DEFINE_ARROW_STAR(const&)
  DANGO_TUPLE_DEFINE_ARROW_STAR(&&)
  DANGO_TUPLE_DEFINE_ARROW_STAR(const&&)

#undef DANGO_TUPLE_DEFINE_ARROW_STAR

public:

/*** dango_operator_swap ***/

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    ( ... && !dango::is_ref<tp_types>) &&
    ( ... && !dango::is_ref<tp_args>) &&
    ( ... && dango::is_swappable<dango::tuple_at_type<dango::tuple_model<tp_types>&>, dango::tuple_at_type<dango::tuple_model<tp_args>&>>)
  )
  constexpr void
  dango_operator_swap
  (dango::tuple<tp_args...>& a_tup)&
  noexcept(( ... && dango::is_noexcept_swappable<dango::tuple_at_type<dango::tuple_model<tp_types>&>, dango::tuple_at_type<dango::tuple_model<tp_args>&>>))
  {
    constexpr auto const c_swap =
      []<dango::usize... tp_indices>
      (dango::index_seq<tp_indices...> const, tuple& a_this, dango::tuple<tp_args...>& a_arg)constexpr
      noexcept(( ... && dango::is_noexcept_swappable<dango::tuple_at_type<dango::tuple_model<tp_types>&>, dango::tuple_at_type<dango::tuple_model<tp_args>&>>))->void
      {
        ( ... , void(dango::swap(a_this.at<tp_indices>(), a_arg.template at<tp_indices>())));
      };

    c_swap(dango::make_index_seq<sizeof...(tp_types)>{ }, *this, a_tup);
  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    ( ... && !dango::is_ref<tp_types>) &&
    ( ... && dango::is_ref<tp_args>) &&
    ( ... && dango::is_swappable<dango::tuple_at_type<dango::tuple_model<tp_types>&>, tp_args>)
  )
  constexpr void
  dango_operator_swap
  (dango::tuple<tp_args...> const& a_tup)&
  noexcept(( ... && dango::is_noexcept_swappable<dango::tuple_at_type<dango::tuple_model<tp_types>&>, tp_args>))
  {
    constexpr auto const c_swap =
      []<dango::usize... tp_indices>
      (dango::index_seq<tp_indices...> const, tuple& a_this, dango::tuple<tp_args...> const& a_arg)constexpr
      noexcept(( ... && dango::is_noexcept_swappable<dango::tuple_at_type<dango::tuple_model<tp_types>&>, tp_args>))->void
      {
        ( ... , void(dango::swap(a_this.at<tp_indices>(), a_arg.template at<tp_indices>())));
      };

    c_swap(dango::make_index_seq<sizeof...(tp_types)>{ }, *this, a_tup);
  }

public:
  constexpr auto size()const noexcept->dango::usize{ return sizeof...(tp_types); }
  constexpr auto is_empty()const noexcept->bool{ return false; }
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

  template
  <bool tp_noexcept>
  struct
  private_construct_tag
  final
  {
    DANGO_TAG_TYPE(private_construct_tag)
  };
private:
  template
  <bool tp_noexcept, typename... tp_args>
  explicit constexpr
  tuple
  (private_construct_tag<tp_noexcept> const, tp_args&&... a_args)noexcept(tp_noexcept):
  m_storage{ dango::detail::make_tuple_storage<storage_type, tp_noexcept>(dango::forward<tp_args>(a_args)...) }
  { }
private:
  using construct_help_prio = dango::priority_tag<dango::uint(2)>;

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_types))
  static constexpr auto
  construct_help
  (dango::priority_tag<dango::uint(2)> const, tp_args&&... a_args)
  noexcept(( ... && dango::is_noexcept_brace_constructible<tp_types, tp_args>))->tuple
  requires(( ... && dango::is_brace_constructible<tp_types, tp_args>))
  {
    constexpr bool const c_noexcept =
      ( ... && dango::is_noexcept_brace_constructible<tp_types, tp_args>);

    return tuple{ private_construct_tag<c_noexcept>{ }, dango::forward<tp_args>(a_args)... };
  }

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_types))
  static constexpr auto
  construct_help
  (dango::priority_tag<dango::uint(1)> const, dango::tuple<tp_args...> const& a_tup)
  noexcept(( ... && dango::is_noexcept_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args> const&>>))->tuple
  requires(( ... && dango::is_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args> const&>>))
  {
    constexpr bool const c_noexcept =
      ( ... && dango::is_noexcept_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args> const&>>);

    constexpr auto const c_make =
      []<dango::usize... tp_indices>
      (dango::index_seq<tp_indices...> const, dango::tuple<tp_args...> const& a_arg)constexpr
      noexcept(c_noexcept)->tuple
      {
        return tuple{ private_construct_tag<c_noexcept>{ }, a_arg.template at<tp_indices>()... };
      };

    return c_make(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup);
  }

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_types))
  static constexpr auto
  construct_help
  (dango::priority_tag<dango::uint(1)> const, dango::tuple<tp_args...>&& a_tup)
  noexcept(( ... && dango::is_noexcept_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args>&&>>))->tuple
  requires(( ... && dango::is_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args>&&>>))
  {
    constexpr bool const c_noexcept =
      ( ... && dango::is_noexcept_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args>&&>>);

    constexpr auto const c_make =
      []<dango::usize... tp_indices>
      (dango::index_seq<tp_indices...> const, dango::tuple<tp_args...>&& a_arg)constexpr
      noexcept(c_noexcept)->tuple
      {
        return tuple{ private_construct_tag<c_noexcept>{ }, dango::move(a_arg).template at<tp_indices>()... };
      };

    return c_make(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup));
  }

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_types))
  static constexpr auto
  construct_explicit_help
  (dango::priority_tag<dango::uint(2)> const, dango::type_tag<tp_args> const...)noexcept->bool
  requires(( ... && dango::is_brace_constructible<tp_types, tp_args>))
  {
    return ( ... || !dango::is_convertible_arg<tp_args, tp_types>);
  }

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_types))
  static constexpr auto
  construct_explicit_help
  (dango::priority_tag<dango::uint(1)> const, dango::type_tag<dango::tuple<tp_args...>&> const)noexcept->bool
  requires(( ... && dango::is_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args> const&>>))
  {
    return ( ... || !dango::is_convertible_arg<dango::tuple_at_type<dango::tuple_model<tp_args> const&>, tp_types>);
  }

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_types))
  static constexpr auto
  construct_explicit_help
  (dango::priority_tag<dango::uint(1)> const, dango::type_tag<dango::tuple<tp_args...> const&> const)noexcept->bool
  requires(( ... && dango::is_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args> const&>>))
  {
    return ( ... || !dango::is_convertible_arg<dango::tuple_at_type<dango::tuple_model<tp_args> const&>, tp_types>);
  }

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_types))
  static constexpr auto
  construct_explicit_help
  (dango::priority_tag<dango::uint(1)> const, dango::type_tag<dango::tuple<tp_args...>&&> const)noexcept->bool
  requires(( ... && dango::is_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args>&&>>))
  {
    return ( ... || !dango::is_convertible_arg<dango::tuple_at_type<dango::tuple_model<tp_args>&&>, tp_types>);
  }

  template
  <typename... tp_args>
  requires(sizeof...(tp_args) == sizeof...(tp_types))
  static constexpr auto
  construct_explicit_help
  (dango::priority_tag<dango::uint(1)> const, dango::type_tag<dango::tuple<tp_args...> const&&> const)noexcept->bool
  requires(( ... && dango::is_brace_constructible<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args> const&>>))
  {
    return ( ... || !dango::is_convertible_arg<dango::tuple_at_type<dango::tuple_model<tp_args> const&>, tp_types>);
  }

  template
  <typename... tp_args>
  static constexpr auto
  construct_explicit_help
  (dango::priority_tag<dango::uint(0)> const, tp_args const...)noexcept->bool
  {
    return false;
  }
public:
  template
  <typename... tp_args>
  requires(sizeof...(tp_args) != dango::usize(0))
  explicit(construct_explicit_help(construct_help_prio{ }, dango::type_tag<tp_args&&>{ }...))
  constexpr
  tuple
  (tp_args&&... a_args)
  noexcept(requires{ { construct_help(construct_help_prio{ }, dango::declval<tp_args>()...) }noexcept; })
  requires(requires{ { construct_help(construct_help_prio{ }, dango::declval<tp_args>()...) }; }):
  tuple{ construct_help(construct_help_prio{ }, dango::forward<tp_args>(a_args)...) }
  { }

  constexpr tuple()noexcept = delete;
  constexpr tuple(tuple const&)noexcept = default;
  constexpr tuple(tuple&&)noexcept = default;
  constexpr ~tuple()noexcept = default;

  template
  <dango::usize tp_index>
  requires(tp_index < sizeof...(tp_types))
  constexpr auto
  at()const noexcept->decltype(auto)
  {
    return m_storage.template at<sizeof...(tp_types) - dango::usize(1) - tp_index>();
  }

#define DANGO_TUPLE_DEFINE_NAMED_GET(index, name) \
  constexpr auto name()const noexcept->decltype(auto) \
  requires(dango::usize(index) < sizeof...(tp_types)) \
  { return at<dango::usize(index)>(); }

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

public:

/*** assign ***/

#define DANGO_TUPLE_DEFINE_ASSIGN(lcvref, rcvref, ret) \
  constexpr auto operator = (tuple rcvref)lcvref noexcept = delete; \
  constexpr auto \
  operator = \
  (tuple rcvref a_tup)lcvref \
  noexcept(( ... && dango::is_noexcept_assignable<tp_types, tp_types>))->ret \
  requires(( ... && dango::is_assignable<tp_types, tp_types>)) \
  { \
    constexpr auto const c_assign = \
    []<dango::usize... tp_indices> \
    (dango::index_seq<tp_indices...> const, tuple lcvref a_this, tuple rcvref a_arg)constexpr \
    noexcept(( ... && dango::is_noexcept_assignable<tp_types, tp_types>))->void \
    { \
      ( ... , void(static_cast<tuple lcvref>(a_this).at<tp_indices>() = static_cast<tuple rcvref>(a_arg).at<tp_indices>())); \
    }; \
    c_assign(dango::make_index_seq<sizeof...(tp_types)>{ }, static_cast<tuple lcvref>(*this), static_cast<tuple rcvref>(a_tup)); \
    return static_cast<tuple lcvref>(*this); \
  }

  DANGO_TUPLE_DEFINE_ASSIGN(&,       const&, tuple&)
  DANGO_TUPLE_DEFINE_ASSIGN(const&,  const&, tuple const&)
  DANGO_TUPLE_DEFINE_ASSIGN(&&,      const&, tuple)
  DANGO_TUPLE_DEFINE_ASSIGN(const&&, const&, tuple)

  DANGO_TUPLE_DEFINE_ASSIGN(&,       &&, tuple&)
  DANGO_TUPLE_DEFINE_ASSIGN(const&,  &&, tuple const&)
  DANGO_TUPLE_DEFINE_ASSIGN(&&,      &&, tuple)
  DANGO_TUPLE_DEFINE_ASSIGN(const&&, &&, tuple)

#undef DANGO_TUPLE_DEFINE_ASSIGN

/*** converting assign ***/

#define DANGO_TUPLE_DEFINE_ASSIGN(lcvref, rcvref, ret) \
  template \
  <typename... tp_args> \
  requires \
  ( \
    (sizeof...(tp_args) == sizeof...(tp_types)) && \
    !( ... && dango::is_same<tp_args, tp_types>) && \
    ( ... && dango::is_assignable<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args> rcvref>>) \
  ) \
  constexpr auto \
  operator = \
  (dango::tuple<tp_args...> rcvref a_tup)lcvref \
  noexcept(( ... && dango::is_noexcept_assignable<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args> rcvref>>))->ret \
  { \
    constexpr auto const c_assign = \
    []<dango::usize... tp_indices> \
    (dango::index_seq<tp_indices...> const, tuple lcvref a_this, dango::tuple<tp_args...> rcvref a_arg)constexpr \
    noexcept(( ... && dango::is_noexcept_assignable<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args> rcvref>>))->void \
    { \
      ( ... , void(static_cast<tuple lcvref>(a_this).at<tp_indices>() = static_cast<dango::tuple<tp_args...> rcvref>(a_arg).template at<tp_indices>())); \
    }; \
    c_assign(dango::make_index_seq<sizeof...(tp_types)>{ }, static_cast<tuple lcvref>(*this), static_cast<dango::tuple<tp_args...> rcvref>(a_tup)); \
    return static_cast<tuple lcvref>(*this); \
  }

  DANGO_TUPLE_DEFINE_ASSIGN(&,       const&, tuple&)
  DANGO_TUPLE_DEFINE_ASSIGN(const&,  const&, tuple const&)
  DANGO_TUPLE_DEFINE_ASSIGN(&&,      const&, tuple)
  DANGO_TUPLE_DEFINE_ASSIGN(const&&, const&, tuple)

  DANGO_TUPLE_DEFINE_ASSIGN(&,       &&, tuple&)
  DANGO_TUPLE_DEFINE_ASSIGN(const&,  &&, tuple const&)
  DANGO_TUPLE_DEFINE_ASSIGN(&&,      &&, tuple)
  DANGO_TUPLE_DEFINE_ASSIGN(const&&, &&, tuple)

#undef DANGO_TUPLE_DEFINE_ASSIGN

public:

/*** operator ->* ***/

  template
  <typename tp_func>
  requires(dango::is_callable<tp_func, tp_types...>)
  constexpr auto
  operator ->*
  (tp_func&& a_func)const
  noexcept(dango::is_noexcept_callable<tp_func, tp_types...>)->decltype(auto)
  {
    constexpr auto const c_call =
      []<dango::usize... tp_indices>
      (dango::index_seq<tp_indices...> const, tp_func&& a_fn, tuple const& a_this)constexpr
      noexcept(dango::is_noexcept_callable<tp_func, tp_types...>)->decltype(auto)
      {
        return dango::forward<tp_func>(a_fn)(a_this.at<tp_indices>()...);
      };

    return c_call(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func), *this);
  }

public:

/*** dango_operator_swap ***/

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    ( ... && dango::is_ref<tp_args>) &&
    ( ... && dango::is_swappable<tp_types, tp_args>)
  )
  constexpr void
  dango_operator_swap
  (dango::tuple<tp_args...> const& a_tup)const
  noexcept(( ... && dango::is_noexcept_swappable<tp_types, tp_args>))
  {
    constexpr auto const c_swap =
      []<dango::usize... tp_indices>
      (dango::index_seq<tp_indices...> const, tuple const& a_this, dango::tuple<tp_args...> const& a_arg)constexpr
      noexcept(( ... && dango::is_noexcept_swappable<tp_types, tp_args>))->void
      {
        ( ... , void(dango::swap(a_this.at<tp_indices>(), a_arg.template at<tp_indices>())));
      };

    c_swap(dango::make_index_seq<sizeof...(tp_types)>{ }, *this, a_tup);
  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    ( ... && !dango::is_ref<tp_args>) &&
    ( ... && dango::is_swappable<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args>&>>)
  )
  constexpr void
  dango_operator_swap
  (dango::tuple<tp_args...>& a_tup)const
  noexcept(( ... && dango::is_noexcept_swappable<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args>&>>))
  {
    constexpr auto const c_swap =
      []<dango::usize... tp_indices>
      (dango::index_seq<tp_indices...> const, tuple const& a_this, dango::tuple<tp_args...>& a_arg)constexpr
      noexcept(( ... && dango::is_noexcept_swappable<tp_types, dango::tuple_at_type<dango::tuple_model<tp_args>&>>))->void
      {
        ( ... , void(dango::swap(a_this.at<tp_indices>(), a_arg.template at<tp_indices>())));
      };

    c_swap(dango::make_index_seq<sizeof...(tp_types)>{ }, *this, a_tup);
  }

public:
  constexpr auto size()const noexcept->dango::usize{ return sizeof...(tp_types); }
  constexpr auto is_empty()const noexcept->bool{ return false; }
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
  constexpr auto size()const noexcept->dango::usize{ return dango::usize(0); }
  constexpr auto is_empty()const noexcept->bool{ return true; }
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

  constexpr void dango_operator_swap(dango::tuple<> const&)const noexcept{ }
};

/*** deduction guide and make ***/

namespace
dango
{
  template
  <typename... tp_args>
  explicit(!( ... && dango::is_convertible_arg<tp_args, dango::emplacer_return_type_arg<tp_args>>))
  tuple(tp_args&&...)->tuple<dango::emplacer_return_type_arg<tp_args>...>;

  template
  <typename... tp_args>
  requires
  (
    ( ... && dango::tuple_constraint_spec<dango::emplacer_return_type_arg<tp_args>>) &&
    ( ... && dango::is_brace_constructible<dango::emplacer_return_type_arg<tp_args>, tp_args>)
  )
  constexpr auto
  make_tuple
  (tp_args&&... a_args)
  noexcept(( ... && dango::is_noexcept_brace_constructible<dango::emplacer_return_type_arg<tp_args>, tp_args>))->auto
  {
    return dango::tuple<dango::emplacer_return_type_arg<tp_args>...>{ dango::forward<tp_args>(a_args)... };
  }

  template
  <typename tp_elem, dango::usize tp_size>
  requires(dango::tuple_constraint_spec<dango::decay<tp_elem>> && dango::is_brace_constructible<dango::decay<tp_elem>, tp_elem&>)
  constexpr auto
  make_tuple_from_array
  (tp_elem(& a_arg)[tp_size])
  noexcept(dango::is_noexcept_brace_constructible<dango::decay<tp_elem>, tp_elem&>)->auto
  {
    constexpr auto const c_make =
      []<dango::usize... tp_indices>
      (dango::index_seq<tp_indices...> const, tp_elem* const a_array)constexpr
      noexcept(dango::is_noexcept_brace_constructible<dango::decay<tp_elem>, tp_elem&>)->auto
      {
        return dango::make_tuple(a_array[tp_indices]...);
      };

    return c_make(dango::make_index_seq<tp_size>{ }, a_arg);
  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == dango::usize(2)) &&
    ( ... && dango::tuple_constraint_spec<dango::emplacer_return_type_arg<tp_args>>) &&
    ( ... && dango::is_brace_constructible<dango::emplacer_return_type_arg<tp_args>, tp_args>)
  )
  constexpr auto
  make_pair
  (tp_args&&... a_args)
  noexcept(( ... && dango::is_noexcept_brace_constructible<dango::emplacer_return_type_arg<tp_args>, tp_args>))->auto
  {
    return dango::tuple<dango::emplacer_return_type_arg<tp_args>...>{ dango::forward<tp_args>(a_args)... };
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

/*** operator_tuple ***/

namespace
dango::detail
{
  template
  <dango::usize tp_index, typename... tp_types>
  struct tuple_element_help;

  template
  <dango::usize tp_index, typename tp_first, typename... tp_next>
  struct
  tuple_element_help<tp_index, tp_first, tp_next...>;

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
tuple_element_help<tp_index, tp_first, tp_next...>
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
dango::custom
{
  template
  <typename... tp_types>
  struct operator_tuple<dango::tuple<tp_types...>>;
}

template
<typename... tp_types>
struct
dango::
custom::
operator_tuple<dango::tuple<tp_types...>>
final
{
  using tuple_type = dango::tuple<tp_types...>;

  static inline constexpr auto const size = sizeof...(tp_types);

  template
  <dango::usize tp_index, dango::is_same_ignore_cv<tuple_type> tp_tuple>
  requires(tp_index < size)
  using elem =
    dango::copy_cv<tp_tuple, typename dango::detail::tuple_element_help<tp_index, tp_types...>::type>;

  template
  <dango::usize tp_index, dango::is_same_ignore_cvref<tuple_type> tp_tuple>
  requires(tp_index < size)
  static constexpr auto
  get
  (tp_tuple&& a_tup)noexcept->decltype(auto)
  {
    return dango::forward<tp_tuple>(a_tup).template at<tp_index>();
  }

  DANGO_UNCONSTRUCTIBLE(operator_tuple)
};

/*** equals, compare, and hash for tuples ***/

namespace
dango::custom
{
  template
  <typename... tp_types>
  requires(( ... && dango::is_equatable<dango::tuple_at_type<dango::tuple_model<tp_types> const&>, dango::tuple_at_type<dango::tuple_model<tp_types> const&>>))
  struct
  operator_equals<dango::tuple<tp_types...>, dango::tuple<tp_types...>>;

  template
  <typename... tp_lhs, typename... tp_rhs>
  struct
  operator_equals<dango::tuple<tp_lhs...>, dango::tuple<tp_rhs...>>;

  template
  <typename... tp_lhs, typename... tp_rhs>
  struct
  operator_compare<dango::tuple<tp_lhs...>, dango::tuple<tp_rhs...>>;

  template
  <typename... tp_types>
  requires(( ... && dango::is_hashable<dango::tuple_at_type<dango::tuple_model<tp_types> const&>>))
  struct
  operator_hash<dango::tuple<tp_types...>>;
}

template
<typename... tp_types>
requires(( ... && dango::is_equatable<dango::tuple_at_type<dango::tuple_model<tp_types> const&>, dango::tuple_at_type<dango::tuple_model<tp_types> const&>>))
struct
dango::
custom::
operator_equals<dango::tuple<tp_types...>, dango::tuple<tp_types...>>
final
{
private:
  using tuple_type = dango::tuple<tp_types...>;

  static inline constexpr bool const c_noexcept =
    ( ... && dango::is_noexcept_equatable<dango::tuple_at_type<dango::tuple_model<tp_types> const&>, dango::tuple_at_type<dango::tuple_model<tp_types> const&>>);

  template
  <dango::usize... tp_indices>
  static constexpr auto
  equals_help
  (dango::index_seq<tp_indices...> const, tuple_type const& a_lhs, tuple_type const& a_rhs)noexcept(c_noexcept)->bool
  {
    return ( ... && dango::equals(dango::tuple_get<tp_indices>(a_lhs), dango::tuple_get<tp_indices>(a_rhs)));
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
struct
dango::
custom::
operator_compare<dango::tuple<tp_lhs...>, dango::tuple<tp_rhs...>>
final
{
private:
  using tuple_lhs = dango::tuple<tp_lhs...>;
  using tuple_rhs = dango::tuple<tp_rhs...>;

  static inline constexpr auto const c_size = dango::min(sizeof...(tp_lhs), sizeof...(tp_rhs));

  template
  <dango::usize... tp_indices>
  requires(requires{ { ( ... , void(dango::compare(dango::tuple_get<tp_indices>(dango::declval<tuple_lhs const&>()), dango::tuple_get<tp_indices>(dango::declval<tuple_rhs const&>())))) }; })
  static constexpr auto
  compare_help
  (dango::index_seq<tp_indices...> const, tuple_lhs const& a_lhs, tuple_rhs const& a_rhs)
  noexcept(requires{ { ( ... , void(dango::compare(dango::tuple_get<tp_indices>(dango::declval<tuple_lhs const&>()), dango::tuple_get<tp_indices>(dango::declval<tuple_rhs const&>())))) }noexcept; })->auto
  {
    using ret_type =
      dango::comparison::common_type<decltype(dango::compare(dango::tuple_get<tp_indices>(a_lhs), dango::tuple_get<tp_indices>(a_rhs)))...>;

    ret_type a_ret{ dango::ssize(0) };

    [[maybe_unused]] bool const a_temp =
      ( ... && (a_ret = ret_type{ dango::compare(dango::tuple_get<tp_indices>(a_lhs), dango::tuple_get<tp_indices>(a_rhs)) }).is_eq());

    return a_ret;
  }
public:
  static constexpr auto
  compare
  (tuple_lhs const& a_lhs, tuple_rhs const& a_rhs)
  noexcept(requires{ { compare_help(dango::make_index_seq<c_size>{ }, a_lhs, a_rhs) }noexcept; })->auto
  requires(requires{ { compare_help(dango::make_index_seq<c_size>{ }, a_lhs, a_rhs) }; })
  {
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

template
<typename... tp_types>
requires(( ... && dango::is_hashable<dango::tuple_at_type<dango::tuple_model<tp_types> const&>>))
struct
dango::
custom::
operator_hash<dango::tuple<tp_types...>>
final
{
private:
  using tuple_type = dango::tuple<tp_types...>;

  static inline constexpr bool const c_noexcept =
    ( ... && dango::has_noexcept_hash<dango::tuple_at_type<dango::tuple_model<tp_types> const&>>);
public:
  static constexpr auto
  hash
  (tuple_type const& a_tup)noexcept(c_noexcept)->dango::hash_val
  {
    return a_tup->*dango::multi_hash;
  }

  DANGO_UNCONSTRUCTIBLE(operator_hash)
};

#endif // DANGO_TUPLE_HPP_INCLUDED
