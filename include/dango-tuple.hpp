#ifndef __DANGO_TUPLE_HPP__
#define __DANGO_TUPLE_HPP__

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
}

namespace
dango
{
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
  tuple_model
  {
    DANGO_UNINSTANTIABLE(tuple_model)
  };
}

namespace
dango::detail
{
  template
  <typename tp_model, typename tp_value_type>
  struct tuple_get_type_help;

  template
  <typename tp_value_type>
  struct tuple_get_type_help<dango::tuple_model&&, tp_value_type>
  {
    using type = dango::conditional<dango::is_ref<tp_value_type>, tp_value_type, tp_value_type&&>;

    DANGO_UNINSTANTIABLE(tuple_get_type_help)
  };
  template
  <typename tp_value_type>
  struct tuple_get_type_help<dango::tuple_model const&&, tp_value_type>
  {
    using type = dango::conditional<dango::is_ref<tp_value_type>, tp_value_type, tp_value_type const&&>;

    DANGO_UNINSTANTIABLE(tuple_get_type_help)
  };
  template
  <typename tp_value_type>
  struct tuple_get_type_help<dango::tuple_model volatile&&, tp_value_type>;
  template
  <typename tp_value_type>
  struct tuple_get_type_help<dango::tuple_model const volatile&&, tp_value_type>;

  template
  <typename tp_value_type>
  struct tuple_get_type_help<dango::tuple_model, tp_value_type>
  {
    using type = typename dango::detail::tuple_get_type_help<dango::tuple_model&&, tp_value_type>::type;

    DANGO_UNINSTANTIABLE(tuple_get_type_help)
  };
  template
  <typename tp_value_type>
  struct tuple_get_type_help<dango::tuple_model const, tp_value_type>
  {
    using type = typename dango::detail::tuple_get_type_help<dango::tuple_model const&&, tp_value_type>::type;

    DANGO_UNINSTANTIABLE(tuple_get_type_help)
  };
  template
  <typename tp_value_type>
  struct tuple_get_type_help<dango::tuple_model volatile, tp_value_type>;
  template
  <typename tp_value_type>
  struct tuple_get_type_help<dango::tuple_model const volatile, tp_value_type>;

  template
  <typename tp_value_type>
  struct tuple_get_type_help<dango::tuple_model&, tp_value_type>
  {
    using type = dango::conditional<dango::is_ref<tp_value_type>, tp_value_type, tp_value_type&>;

    DANGO_UNINSTANTIABLE(tuple_get_type_help)
  };
  template
  <typename tp_value_type>
  struct tuple_get_type_help<dango::tuple_model const&, tp_value_type>
  {
    using type = dango::conditional<dango::is_ref<tp_value_type>, tp_value_type, tp_value_type const&>;

    DANGO_UNINSTANTIABLE(tuple_get_type_help)
  };
  template
  <typename tp_value_type>
  struct tuple_get_type_help<dango::tuple_model volatile&, tp_value_type>;
  template
  <typename tp_value_type>
  struct tuple_get_type_help<dango::tuple_model const volatile&, tp_value_type>;
}

namespace
dango
{
  template
  <typename tp_model, typename tp_type>
  using tuple_get_type =
    typename detail::tuple_get_type_help<tp_model, dango::tuple_value_type<tp_type>>::type;
}

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
    dango::detail::is_skip_constructible_help<dango::tuple_value_type<tp_type>>;

  template
  <typename tp_type>
  concept is_noexcept_skip_constructible =
    dango::detail::is_noexcept_skip_constructible_help<dango::tuple_value_type<tp_type>>;
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
  concept is_tuple_tag =
    dango::detail::is_value_init_tag<tp_arg> || dango::detail::is_skip_init_tag<tp_arg>;

  template
  <typename tp_value_type, typename tp_arg>
  concept tuple_is_constructible_help =
    (dango::detail::is_value_init_tag<tp_arg> && dango::is_default_constructible<tp_value_type>) ||
    (dango::detail::is_skip_init_tag<tp_arg> && dango::detail::is_skip_constructible_help<tp_value_type>) ||
    dango::is_constructible<tp_value_type, tp_arg>;

  template
  <typename tp_type, typename tp_arg>
  concept tuple_is_constructible =
    dango::detail::tuple_is_constructible_help<dango::tuple_value_type<tp_type>, tp_arg>;

  template
  <typename tp_value_type, typename tp_arg>
  concept tuple_is_noexcept_constructible_help =
    (dango::detail::is_value_init_tag<tp_arg> && dango::is_noexcept_default_constructible<tp_value_type>) ||
    (dango::detail::is_skip_init_tag<tp_arg> && dango::detail::is_noexcept_skip_constructible_help<tp_value_type>) ||
    dango::is_noexcept_constructible<tp_value_type, tp_arg>;

  template
  <typename tp_type, typename tp_arg>
  concept tuple_is_noexcept_constructible =
    dango::detail::tuple_is_noexcept_constructible_help<dango::tuple_value_type<tp_type>, tp_arg>;

  template
  <typename tp_value_type, typename tp_arg>
  concept tuple_is_convertible_help =
    (dango::detail::is_value_init_tag<tp_arg> && dango::is_default_constructible<tp_value_type>) ||
    (dango::detail::is_skip_init_tag<tp_arg> && dango::detail::is_skip_constructible_help<tp_value_type>) ||
    dango::is_convertible<tp_arg, tp_value_type>;

  template
  <typename tp_arg, typename tp_type>
  concept tuple_is_convertible =
    detail::tuple_is_convertible_help<tp_arg, dango::tuple_value_type<tp_type>>;
}

namespace
dango::detail
{
  template
  <typename tp_pack>
  class tuple_storage;

  template
  <typename tp_first>
  class tuple_storage<dango::detail::tuple_pack<tp_first>>;

  template
  <typename tp_first, typename... tp_next>
  class tuple_storage<dango::detail::tuple_pack<tp_first, tp_next...>>;
}

template
<typename tp_first>
class
dango::
detail::
tuple_storage
<dango::detail::tuple_pack<tp_first>>
{
public:
  using value_type = dango::tuple_value_type<tp_first>;
  static_assert(dango::is_same<value_type, tp_first>);
  static_assert(dango::is_noexcept_destructible<value_type>);
public:
  constexpr tuple_storage()noexcept = delete;
  constexpr
  tuple_storage()noexcept(dango::is_noexcept_default_constructible<value_type>)
  requires(dango::is_default_constructible<value_type>) = default;

  template
  <typename tp_arg>
  requires(!dango::detail::is_tuple_tag<tp_arg> && !dango::is_same_ignore_cvref<tp_arg, tuple_storage>)
  explicit constexpr
  tuple_storage
  (tp_arg&& a_arg)noexcept(dango::is_noexcept_constructible<value_type, tp_arg>):
  m_value(dango::forward<tp_arg>(a_arg))
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

  ~tuple_storage()noexcept = default;

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
class
dango::
detail::
tuple_storage
<dango::detail::tuple_pack<tp_first, tp_next...>>:
public dango::detail::tuple_storage<dango::detail::tuple_pack<tp_next...>>
{
public:
  using value_type = dango::tuple_value_type<tp_first>;
  static_assert(dango::is_same<value_type, tp_first>);
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
  requires((sizeof...(tp_args) == sizeof...(tp_next)) && !dango::detail::is_tuple_tag<tp_arg>)
  explicit constexpr
  tuple_storage
  (tp_arg&& a_arg, tp_args&&... a_args)
  noexcept(dango::is_noexcept_constructible<value_type, tp_arg> && dango::is_noexcept_brace_constructible<super_type, tp_args...>):
  super_type{ dango::forward<tp_args>(a_args)... },
  m_value(dango::forward<tp_arg>(a_arg))
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

  ~tuple_storage()noexcept = default;

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

  DANGO_UNINSTANTIABLE(make_tuple_storage_help)
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

  DANGO_UNINSTANTIABLE(make_tuple_storage_help)
};

/*** tuple ***/

namespace
dango
{
  template
  <typename... tp_types>
  concept tuple_constraint_spec =
    ( ... && dango::is_same<dango::tuple_value_type<tp_types>, tp_types>) &&
    ( ... && dango::is_noexcept_destructible<tp_types>);
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

template
<typename... tp_types>
requires(dango::tuple_constraint_spec<tp_types...>)
class
dango::
tuple
{
private:
  using storage_type =
    detail::tuple_storage<detail::tuple_pack_reverse<tp_types...>>;
public:
  static constexpr auto size()noexcept->dango::usize{ return sizeof...(tp_types); }
public:
  constexpr tuple()noexcept = delete;
  constexpr
  tuple()noexcept(( ... && dango::is_noexcept_default_constructible<tp_types>))
  requires(( ... && dango::is_default_constructible<tp_types>)) = default;

#define DANGO_TUPLE_LONG_NOEXCEPT_SPEC \
  ( ... && dango::detail::tuple_is_noexcept_constructible<tp_types, tp_args>)

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    !( ... && dango::detail::is_tuple_tag<tp_args>) &&
    ( ... && dango::detail::tuple_is_constructible<tp_types, tp_args>)
  )
  explicit(!( ... && dango::detail::tuple_is_convertible<tp_args, tp_types>))
  constexpr
  tuple(tp_args&&... a_args)
  noexcept(DANGO_TUPLE_LONG_NOEXCEPT_SPEC):
  m_storage{ dango::detail::make_tuple_storage<storage_type, DANGO_TUPLE_LONG_NOEXCEPT_SPEC>(dango::forward<tp_args>(a_args)...) }
  {

  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    ( ... && dango::detail::is_tuple_tag<tp_args>) &&
    ( ... && dango::detail::tuple_is_constructible<tp_types, tp_args>)
  )
  constexpr
  tuple(tp_args&&... a_args)
  noexcept(DANGO_TUPLE_LONG_NOEXCEPT_SPEC):
  m_storage{ dango::detail::make_tuple_storage<storage_type, DANGO_TUPLE_LONG_NOEXCEPT_SPEC>(dango::forward<tp_args>(a_args)...) }
  {

  }

#undef DANGO_TUPLE_LONG_NOEXCEPT_SPEC

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

  ~tuple()noexcept = default;

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
  construct
  (dango::index_seq<tp_indices...> const, tuple<tp_args...> const& a_tup)
  noexcept(tp_noexcept)->storage_type
  {
    return dango::detail::make_tuple_storage<storage_type, tp_noexcept>(a_tup.template get<tp_indices>()...);
  }

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename... tp_args>
  static constexpr auto
  construct
  (dango::index_seq<tp_indices...> const, tuple<tp_args...>&& a_tup)
  noexcept(tp_noexcept)->storage_type
  {
    return dango::detail::make_tuple_storage<storage_type, tp_noexcept>(dango::move(a_tup).template get<tp_indices>()...);
  }

public:

/*** converting copy-construct ***/

#define DANGO_TUPLE_LONG_NOEXCEPT_SPEC(cvref) \
  ( ... && dango::detail::tuple_is_noexcept_constructible<tp_types, dango::tuple_get_type<dango::tuple_model cvref, tp_args>>)

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    (sizeof...(tp_args) != dango::usize(1)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    !( ... && dango::detail::is_tuple_tag<tp_args>) &&
    ( ... && dango::detail::tuple_is_constructible<tp_types, dango::tuple_get_type<dango::tuple_model const&, tp_args>>)
  )
  explicit(!( ... && detail::tuple_is_convertible<dango::tuple_get_type<dango::tuple_model const&, tp_args>, tp_types>))
  constexpr
  tuple
  (dango::tuple<tp_args...> const& a_tup)
  noexcept(DANGO_TUPLE_LONG_NOEXCEPT_SPEC(const&)):
  m_storage{ construct<DANGO_TUPLE_LONG_NOEXCEPT_SPEC(const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup) }
  {

  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    (sizeof...(tp_args) == dango::usize(1)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    !( ... && dango::detail::is_tuple_tag<tp_args>) &&
    ( ... && dango::detail::tuple_is_constructible<tp_types, dango::tuple_get_type<dango::tuple_model const&, tp_args>>) &&
    !( ... && dango::is_constructible<tp_types, dango::tuple<tp_args...> const&>) &&
    !( ... && dango::is_convertible<dango::tuple<tp_args...> const&, tp_types>)
  )
  explicit(!( ... && detail::tuple_is_convertible<dango::tuple_get_type<dango::tuple_model const&, tp_args>, tp_types>))
  constexpr
  tuple
  (dango::tuple<tp_args...> const& a_tup)
  noexcept(DANGO_TUPLE_LONG_NOEXCEPT_SPEC(const&)):
  m_storage{ construct<DANGO_TUPLE_LONG_NOEXCEPT_SPEC(const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup) }
  {

  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    (sizeof...(tp_args) != dango::usize(1)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    ( ... && dango::detail::is_tuple_tag<tp_args>) &&
    ( ... && dango::detail::tuple_is_constructible<tp_types, dango::tuple_get_type<dango::tuple_model const&, tp_args>>)
  )
  constexpr
  tuple
  (dango::tuple<tp_args...> const& a_tup)
  noexcept(DANGO_TUPLE_LONG_NOEXCEPT_SPEC(const&)):
  m_storage{ construct<DANGO_TUPLE_LONG_NOEXCEPT_SPEC(const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup) }
  {

  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    (sizeof...(tp_args) == dango::usize(1)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    ( ... && dango::detail::is_tuple_tag<tp_args>) &&
    ( ... && dango::detail::tuple_is_constructible<tp_types, dango::tuple_get_type<dango::tuple_model const&, tp_args>>) &&
    !( ... && dango::is_constructible<tp_types, dango::tuple<tp_args...> const&>) &&
    !( ... && dango::is_convertible<dango::tuple<tp_args...> const&, tp_types>)
  )
  constexpr
  tuple
  (dango::tuple<tp_args...> const& a_tup)
  noexcept(DANGO_TUPLE_LONG_NOEXCEPT_SPEC(const&)):
  m_storage{ construct<DANGO_TUPLE_LONG_NOEXCEPT_SPEC(const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup) }
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
    !( ... && dango::detail::is_tuple_tag<tp_args>) &&
    ( ... && dango::detail::tuple_is_constructible<tp_types, dango::tuple_get_type<dango::tuple_model&&, tp_args>>)
  )
  explicit(!( ... && detail::tuple_is_convertible<dango::tuple_get_type<dango::tuple_model&&, tp_args>, tp_types>))
  constexpr
  tuple
  (dango::tuple<tp_args...>&& a_tup)
  noexcept(DANGO_TUPLE_LONG_NOEXCEPT_SPEC(&&)):
  m_storage{ construct<DANGO_TUPLE_LONG_NOEXCEPT_SPEC(&&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup)) }
  {

  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    (sizeof...(tp_args) == dango::usize(1)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    !( ... && dango::detail::is_tuple_tag<tp_args>) &&
    ( ... && dango::detail::tuple_is_constructible<tp_types, dango::tuple_get_type<dango::tuple_model&&, tp_args>>) &&
    !( ... && dango::is_constructible<tp_types, dango::tuple<tp_args...>&&>) &&
    !( ... && dango::is_convertible<dango::tuple<tp_args...>&&, tp_types>)
  )
  explicit(!( ... && detail::tuple_is_convertible<dango::tuple_get_type<dango::tuple_model&&, tp_args>, tp_types>))
  constexpr
  tuple
  (dango::tuple<tp_args...>&& a_tup)
  noexcept(DANGO_TUPLE_LONG_NOEXCEPT_SPEC(&&)):
  m_storage{ construct<DANGO_TUPLE_LONG_NOEXCEPT_SPEC(&&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup)) }
  {

  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    (sizeof...(tp_args) != dango::usize(1)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    ( ... && dango::detail::is_tuple_tag<tp_args>) &&
    ( ... && dango::detail::tuple_is_constructible<tp_types, dango::tuple_get_type<dango::tuple_model&&, tp_args>>)
  )
  constexpr
  tuple
  (dango::tuple<tp_args...>&& a_tup)
  noexcept(DANGO_TUPLE_LONG_NOEXCEPT_SPEC(&&)):
  m_storage{ construct<DANGO_TUPLE_LONG_NOEXCEPT_SPEC(&&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup)) }
  {

  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    (sizeof...(tp_args) == dango::usize(1)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    ( ... && dango::detail::is_tuple_tag<tp_args>) &&
    ( ... && dango::detail::tuple_is_constructible<tp_types, dango::tuple_get_type<dango::tuple_model&&, tp_args>>) &&
    !( ... && dango::is_constructible<tp_types, dango::tuple<tp_args...>&&>) &&
    !( ... && dango::is_convertible<dango::tuple<tp_args...>&&, tp_types>)
  )
  constexpr
  tuple
  (dango::tuple<tp_args...>&& a_tup)
  noexcept(DANGO_TUPLE_LONG_NOEXCEPT_SPEC(&&)):
  m_storage{ construct<DANGO_TUPLE_LONG_NOEXCEPT_SPEC(&&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup)) }
  {

  }

#undef DANGO_TUPLE_LONG_NOEXCEPT_SPEC

private:

/*** converting assign helpers ***/

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename... tp_args>
  constexpr auto
  assign
  (dango::index_seq<tp_indices...> const, dango::tuple<tp_args...> const& a_tup)
  noexcept(tp_noexcept)->tuple&
  {
    [[maybe_unused]] dango::ubyte const a_temp[] = { (static_cast<void>(get<tp_indices>() = a_tup.template get<tp_indices>()), dango::ubyte(0))... };

    return *this;
  }

  template
  <bool tp_noexcept, dango::usize... tp_indices, typename... tp_args>
  constexpr auto
  assign
  (dango::index_seq<tp_indices...> const, dango::tuple<tp_args...>&& a_tup)
  noexcept(tp_noexcept)->tuple&
  {
    [[maybe_unused]] dango::ubyte const a_temp[] = { (static_cast<void>(get<tp_indices>() = dango::move(a_tup).template get<tp_indices>()), dango::ubyte(0))... };

    return *this;
  }

public:

/*** converting assign ***/

#define DANGO_TUPLE_LONG_NOEXCEPT_SPEC(cvref) \
  ( ... && dango::is_noexcept_assignable<dango::tuple_get_type<dango::tuple_model&, tp_types>, dango::tuple_get_type<dango::tuple_model cvref, tp_args>>)

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    ( ... && !dango::is_ref<tp_types>) &&
    ( ... && dango::is_assignable<dango::tuple_get_type<dango::tuple_model&, tp_types>, dango::tuple_get_type<dango::tuple_model const&, tp_args>>)
  )
  constexpr auto
  operator =
  (dango::tuple<tp_args...> const& a_tup)&
  noexcept(DANGO_TUPLE_LONG_NOEXCEPT_SPEC(const&))->tuple&
  {
    return assign<DANGO_TUPLE_LONG_NOEXCEPT_SPEC(const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup);
  }

  template
  <typename... tp_args>
  requires
  (
    (sizeof...(tp_args) == sizeof...(tp_types)) &&
    !( ... && dango::is_same<tp_args, tp_types>) &&
    ( ... && !dango::is_ref<tp_types>) &&
    ( ... && dango::is_assignable<dango::tuple_get_type<dango::tuple_model&, tp_types>, dango::tuple_get_type<dango::tuple_model&&, tp_args>>)
  )
  constexpr auto
  operator =
  (dango::tuple<tp_args...>&& a_tup)&
  noexcept(DANGO_TUPLE_LONG_NOEXCEPT_SPEC(&&))->tuple&
  {
    return assign<DANGO_TUPLE_LONG_NOEXCEPT_SPEC(&&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::move(a_tup));
  }

#undef DANGO_TUPLE_LONG_NOEXCEPT_SPEC

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

#define DANGO_TUPLE_LONG_NOEXCEPT_SPEC(cvref) \
  dango::is_noexcept_callable<tp_func, dango::tuple_get_type<dango::tuple_model cvref, tp_types>...>

  template
  <typename tp_func>
  requires(dango::is_callable<tp_func, dango::tuple_get_type<dango::tuple_model&, tp_types>...>)
  constexpr auto
  operator ->*
  (tp_func&& a_func)&
  noexcept(DANGO_TUPLE_LONG_NOEXCEPT_SPEC(&))->decltype(auto)
  {
    return static_cast<tuple&>(*this).call<DANGO_TUPLE_LONG_NOEXCEPT_SPEC(&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func));
  }

  template
  <typename tp_func>
  requires(dango::is_callable<tp_func, dango::tuple_get_type<dango::tuple_model&&, tp_types>...>)
  constexpr auto
  operator ->*
  (tp_func&& a_func)&&
  noexcept(DANGO_TUPLE_LONG_NOEXCEPT_SPEC(&&))->decltype(auto)
  {
    return static_cast<tuple&&>(*this).call<DANGO_TUPLE_LONG_NOEXCEPT_SPEC(&&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func));
  }

  template
  <typename tp_func>
  requires(dango::is_callable<tp_func, dango::tuple_get_type<dango::tuple_model const&, tp_types>...>)
  constexpr auto
  operator ->*
  (tp_func&& a_func)const&
  noexcept(DANGO_TUPLE_LONG_NOEXCEPT_SPEC(const&))->decltype(auto)
  {
    return static_cast<tuple const&>(*this).call<DANGO_TUPLE_LONG_NOEXCEPT_SPEC(const&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func));
  }

  template
  <typename tp_func>
  requires(dango::is_callable<tp_func, dango::tuple_get_type<dango::tuple_model const&&, tp_types>...>)
  constexpr auto
  operator ->*
  (tp_func&& a_func)const&&
  noexcept(DANGO_TUPLE_LONG_NOEXCEPT_SPEC(const&&))->decltype(auto)
  {
    return static_cast<tuple const&&>(*this).call<DANGO_TUPLE_LONG_NOEXCEPT_SPEC(const&&)>(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func));
  }

#undef DANGO_TUPLE_LONG_NOEXCEPT_SPEC

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
  size
  ()noexcept->dango::usize
  {
    return dango::usize(0);
  }
public:
  DANGO_EMPTY_TYPE(tuple, false)
public:
  template
  <typename tp_func>
  requires(dango::is_callable<tp_func>)
  constexpr auto
  operator ->*
  (tp_func&& a_func)const
  noexcept(dango::is_noexcept_callable<tp_func>)->decltype(auto)
  {
    return dango::forward<tp_func>(a_func)();
  }
};

/*** structured bindings ***/

namespace
dango::detail
{
  template
  <typename... tp_types>
  struct tuple_size_help;

  template
  <dango::usize tp_index, typename tp_first, typename... tp_next>
  struct tuple_element_help;

  template
  <typename tp_first, typename... tp_next>
  struct
  tuple_element_help<dango::usize(0), tp_first, tp_next...>;
}

template
<typename... tp_types>
struct
dango::
detail::
tuple_size_help
{
  static inline constexpr dango::usize const value = sizeof...(tp_types);

  DANGO_UNINSTANTIABLE(tuple_size_help)
};

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

  DANGO_UNINSTANTIABLE(tuple_element_help)
};

template
<typename tp_first, typename... tp_next>
struct
dango::
detail::
tuple_element_help<dango::usize(0), tp_first, tp_next...>
final
{
  using type = dango::tuple_value_type<tp_first>;

  DANGO_UNINSTANTIABLE(tuple_element_help)
};

namespace
std
{
  template
  <typename tp_type>
  struct tuple_size;

  template
  <typename... tp_types>
  struct
  tuple_size<dango::tuple<tp_types...>>
  final:
  dango::detail::tuple_size_help<tp_types...>
  {
    DANGO_UNINSTANTIABLE(tuple_size)
  };

  template
  <typename... tp_types>
  struct
  tuple_size<dango::tuple<tp_types...> const>
  final:
  dango::detail::tuple_size_help<tp_types...>
  {
    DANGO_UNINSTANTIABLE(tuple_size)
  };

  template
  <typename... tp_types>
  struct
  tuple_size<dango::tuple<tp_types...> volatile>
  final:
  dango::detail::tuple_size_help<tp_types...>
  {
    DANGO_UNINSTANTIABLE(tuple_size)
  };

  template
  <typename... tp_types>
  struct
  tuple_size<dango::tuple<tp_types...> const volatile>
  final:
  dango::detail::tuple_size_help<tp_types...>
  {
    DANGO_UNINSTANTIABLE(tuple_size)
  };

  template
  <dango::usize tp_index, typename tp_type>
  struct tuple_element;

  template
  <dango::usize tp_index, typename... tp_types>
  struct
  tuple_element
  <tp_index, dango::tuple<tp_types...>>
  final
  {
    using type =
      typename dango::detail::tuple_element_help<tp_index, tp_types...>::type;

    DANGO_UNINSTANTIABLE(tuple_element)
  };

  template
  <dango::usize tp_index, typename... tp_types>
  struct
  tuple_element<tp_index, dango::tuple<tp_types...> const>
  final
  {
    using type =
      typename dango::detail::tuple_element_help<tp_index, tp_types...>::type const;

    DANGO_UNINSTANTIABLE(tuple_element)
  };

  template
  <dango::usize tp_index, typename... tp_types>
  struct
  tuple_element<tp_index, dango::tuple<tp_types...> volatile>
  final
  {
    using type =
      typename dango::detail::tuple_element_help<tp_index, tp_types...>::type volatile;

    DANGO_UNINSTANTIABLE(tuple_element)
  };

  template
  <dango::usize tp_index, typename... tp_types>
  struct
  tuple_element<tp_index, dango::tuple<tp_types...> const volatile>
  final
  {
    using type =
      typename dango::detail::tuple_element_help<tp_index, tp_types...>::type const volatile;

    DANGO_UNINSTANTIABLE(tuple_element)
  };

  template
  <dango::usize tp_index>
  struct
  tuple_element<tp_index, dango::tuple<>>;
  template
  <dango::usize tp_index>
  struct
  tuple_element<tp_index, dango::tuple<> const>;
  template
  <dango::usize tp_index>
  struct
  tuple_element<tp_index, dango::tuple<> volatile>;
  template
  <dango::usize tp_index>
  struct
  tuple_element<tp_index, dango::tuple<> const volatile>;
}

#endif

