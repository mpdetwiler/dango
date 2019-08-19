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
dango::detail
{
  template
  <typename tp_value_type, bool tp_const>
  struct tuple_get_type_help;
  template
  <typename tp_value_type, bool tp_const>
  struct tuple_get_type_help<tp_value_type&, tp_const>;
  template
  <typename tp_value_type, bool tp_const>
  struct tuple_get_type_help<tp_value_type&&, tp_const>;
  template
  <typename tp_value_type, bool tp_const>
  struct tuple_get_type_help<tp_value_type const, tp_const>;
}

namespace
dango
{
  template
  <typename tp_type, bool tp_const = false>
  using tuple_get_type =
    typename detail::tuple_get_type_help<dango::tuple_value_type<tp_type>, tp_const>::type;
}

template
<typename tp_value_type, bool tp_const>
struct
dango::
detail::
tuple_get_type_help
final
{
  using type = dango::conditional<tp_const, tp_value_type const, tp_value_type>&;

  DANGO_UNINSTANTIABLE(tuple_get_type_help)
};

template
<typename tp_value_type, bool tp_const>
struct
dango::
detail::
tuple_get_type_help<tp_value_type&, tp_const>
final
{
  using type = tp_value_type&;

  DANGO_UNINSTANTIABLE(tuple_get_type_help)
};

template
<typename tp_value_type, bool tp_const>
struct
dango::
detail::
tuple_get_type_help<tp_value_type&&, tp_const>
final
{
  using type = tp_value_type&&;

  DANGO_UNINSTANTIABLE(tuple_get_type_help)
};

template
<typename tp_value_type, bool tp_const>
struct
dango::
detail::
tuple_get_type_help<tp_value_type const, tp_const>
final
{
  using type = tp_value_type const&;

  DANGO_UNINSTANTIABLE(tuple_get_type_help)
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

  struct
  empty_elem
  final
  {
    constexpr empty_elem()noexcept = default;
    constexpr empty_elem(empty_elem const&)noexcept{ }
    constexpr empty_elem(empty_elem const&&)noexcept{ }
    ~empty_elem()noexcept = default;
    constexpr auto operator = (empty_elem const&)const& noexcept->empty_elem const&{ return *this; }
    constexpr auto operator = (empty_elem const&&)const& noexcept->empty_elem const&{ return *this; }

    template
    <typename tp_arg, dango::enable_if<!dango::is_same<empty_elem, dango::remove_cvref<tp_arg>>> = dango::enable_val>
    constexpr explicit empty_elem(tp_arg&&)noexcept{ }
  };
}

namespace
dango::detail
{
  inline empty_elem empty_elem_instance{ };
}

namespace
dango::detail
{
  template
  <typename tp_value_type>
  constexpr bool const is_skip_constructible_help =
    dango::is_trivial_default_constructible<tp_value_type> ?
    !is_const<tp_value_type> :
    dango::is_default_constructible<tp_value_type>;

  template
  <typename tp_value_type>
  constexpr bool const is_noexcept_skip_constructible_help =
    dango::is_trivial_default_constructible<tp_value_type> ?
    !is_const<tp_value_type> :
    dango::is_noexcept_default_constructible<tp_value_type>;

  template
  <typename tp_type>
  constexpr bool const is_empty_elem =
    dango::is_same<dango::empty_elem, dango::remove_cv<tp_type>>;
}

namespace
dango
{
  template
  <typename tp_type>
  constexpr bool const is_skip_constructible =
    detail::is_skip_constructible_help<dango::tuple_value_type<tp_type>>;

  template
  <typename tp_type>
  constexpr bool const is_noexcept_skip_constructible =
    detail::is_noexcept_skip_constructible_help<dango::tuple_value_type<tp_type>>;
}

namespace
dango::detail
{
  template
  <typename tp_tag>
  constexpr bool const is_tuple_tag_help =
    dango::is_same<tp_tag, dango::value_init_tag> ||
    dango::is_same<tp_tag, dango::skip_init_tag>;

  template
  <typename tp_arg>
  constexpr bool const is_tuple_tag = detail::is_tuple_tag_help<dango::remove_cvref<tp_arg>>;

  template
  <
    typename tp_value_type,
    typename tp_arg,
    typename tp_tag = dango::remove_cvref<tp_arg>
  >
  constexpr bool const tuple_is_constructible_help =
    (dango::is_same<tp_tag, dango::value_init_tag> && dango::is_default_constructible<tp_value_type>) ||
    (dango::is_same<tp_tag, dango::skip_init_tag> && detail::is_skip_constructible_help<tp_value_type>) ||
    dango::is_constructible<tp_value_type, tp_arg>;

  template
  <typename tp_type, typename tp_arg>
  constexpr bool const tuple_is_constructible =
    detail::tuple_is_constructible_help<dango::tuple_value_type<tp_type>, tp_arg>;

  template
  <
    typename tp_value_type,
    typename tp_arg,
    typename tp_tag = dango::remove_cvref<tp_arg>
  >
  constexpr bool const tuple_is_noexcept_constructible_help =
    (dango::is_same<tp_tag, dango::value_init_tag> && dango::is_noexcept_default_constructible<tp_value_type>) ||
    (dango::is_same<tp_tag, dango::skip_init_tag> && detail::is_noexcept_skip_constructible_help<tp_value_type>) ||
    dango::is_noexcept_constructible<tp_value_type, tp_arg>;

  template
  <typename tp_type, typename tp_arg>
  constexpr bool const tuple_is_noexcept_constructible =
    detail::tuple_is_noexcept_constructible_help<dango::tuple_value_type<tp_type>, tp_arg>;

  template
  <
    typename tp_arg,
    typename tp_value_type,
    typename tp_tag = dango::remove_cvref<tp_arg>
  >
  constexpr bool const tuple_is_convertible_help =
    (dango::is_same<tp_tag, dango::value_init_tag> && dango::is_default_constructible<tp_value_type>) ||
    (dango::is_same<tp_tag, dango::skip_init_tag> && detail::is_skip_constructible_help<tp_value_type>) ||
    dango::is_convertible<tp_arg, tp_value_type>;

  template
  <typename tp_arg, typename tp_type>
  constexpr bool const tuple_is_convertible =
    detail::tuple_is_convertible_help<tp_arg, dango::tuple_value_type<tp_type>>;
}

namespace
dango::detail
{
  template
  <typename tp_pack, typename tp_enabled = dango::enable_tag>
  class tuple_storage;

  template
  <typename tp_first>
  class tuple_storage<detail::tuple_pack<tp_first>, dango::enable_if<detail::is_empty_elem<tp_first>>>;

  template
  <typename tp_first>
  class tuple_storage<detail::tuple_pack<tp_first>, dango::enable_if<!detail::is_empty_elem<tp_first>>>;

  template
  <typename tp_first, typename... tp_next>
  class tuple_storage<detail::tuple_pack<tp_first, tp_next...>, dango::enable_if<detail::is_empty_elem<tp_first>>>;

  template
  <typename tp_first, typename... tp_next>
  class tuple_storage<detail::tuple_pack<tp_first, tp_next...>, dango::enable_if<!detail::is_empty_elem<tp_first>>>;
}

/*** empty ***/

template
<typename tp_first>
class
dango::
detail::
tuple_storage
<dango::detail::tuple_pack<tp_first>, dango::enable_if<dango::detail::is_empty_elem<tp_first>>>
{
public:
  using value_type = dango::tuple_value_type<tp_first>;
  static_assert(dango::is_same<value_type, tp_first>);
  static_assert(dango::is_noexcept_destructible<value_type>);
  static_assert(!dango::is_ref<value_type>);
public:
  explicit constexpr tuple_storage()noexcept = default;

  template
  <
    typename tp_arg,
    dango::enable_if
    <
      !detail::is_tuple_tag<tp_arg> &&
      !dango::is_same<dango::remove_cvref<tp_arg>, tuple_storage>
    > = dango::enable_val
  >
  explicit constexpr tuple_storage(tp_arg&&)noexcept{ }

  explicit constexpr
  tuple_storage
  (dango::value_init_tag const)noexcept
  {

  }

  explicit constexpr
  tuple_storage
  (dango::skip_init_tag const)noexcept
  {

  }

  explicit constexpr
  tuple_storage
  (tuple_storage const&)noexcept = default;

  explicit constexpr
  tuple_storage
  (tuple_storage&&)noexcept = default;

  ~tuple_storage()noexcept = default;

  constexpr auto
  operator =
  (tuple_storage const&)& noexcept->tuple_storage& = default;

  constexpr auto
  operator =
  (tuple_storage&&)& noexcept->tuple_storage& = default;

  template
  <
    dango::usize tp_index,
    dango::enable_if
    <
      dango::is_equal(tp_index, dango::usize(0)) &&
      !dango::is_const<value_type>
    > = dango::enable_val
  >
  constexpr auto
  get()noexcept->value_type&
  {
    return detail::empty_elem_instance;
  }

  template
  <
    dango::usize tp_index,
    dango::enable_if
    <
      dango::is_equal(tp_index, dango::usize(0)) &&
      !dango::is_const<value_type>
    > = dango::enable_val
  >
  constexpr auto
  get()const noexcept->value_type const&
  {
    return detail::empty_elem_instance;
  }

  template
  <
    dango::usize tp_index,
    dango::enable_if
    <
      dango::is_equal(tp_index, dango::usize(0)) &&
      dango::is_const<value_type>
    > = dango::enable_val
  >
  constexpr auto
  get()const noexcept->value_type&
  {
    return detail::empty_elem_instance;
  }
};

/*** not empty ***/

template
<typename tp_first>
class
dango::
detail::
tuple_storage
<dango::detail::tuple_pack<tp_first>, dango::enable_if<!dango::detail::is_empty_elem<tp_first>>>
{
public:
  using value_type = dango::tuple_value_type<tp_first>;
  static_assert(dango::is_same<value_type, tp_first>);
  static_assert(dango::is_noexcept_destructible<value_type>);
public:
  explicit constexpr
  tuple_storage
  ()noexcept(false):
  m_value{ }
  {

  }

  template
  <
    typename tp_arg,
    dango::enable_if
    <
      !detail::is_tuple_tag<tp_arg> &&
      !dango::is_same<dango::remove_cvref<tp_arg>, tuple_storage>
    > = dango::enable_val
  >
  explicit constexpr
  tuple_storage
  (tp_arg&& a_arg)noexcept(false):
  m_value(dango::forward<tp_arg>(a_arg))
  {

  }

  explicit constexpr
  tuple_storage
  (dango::value_init_tag const)noexcept(false):
  m_value{ }
  {

  }

  explicit constexpr
  tuple_storage
  (dango::skip_init_tag const)
  noexcept(false)
  {

  }

  explicit constexpr
  tuple_storage
  (tuple_storage const&)
  noexcept(dango::is_noexcept_copy_constructible<value_type>) = default;

  explicit constexpr
  tuple_storage
  (tuple_storage&&)
  noexcept(dango::is_noexcept_move_constructible<value_type>) = default;

  ~tuple_storage()noexcept = default;

  constexpr auto
  operator =
  (tuple_storage const&)&
  noexcept(dango::is_noexcept_copy_assignable<value_type>)->tuple_storage& = default;

  constexpr auto
  operator =
  (tuple_storage&&)&
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

/*** empty ***/

template
<typename tp_first, typename... tp_next>
class
dango::
detail::
tuple_storage
<dango::detail::tuple_pack<tp_first, tp_next...>, dango::enable_if<dango::detail::is_empty_elem<tp_first>>>:
public dango::detail::tuple_storage<dango::detail::tuple_pack<tp_next...>>
{
public:
  using value_type = dango::tuple_value_type<tp_first>;
  static_assert(dango::is_same<value_type, tp_first>);
  static_assert(dango::is_noexcept_destructible<value_type>);
  static_assert(!dango::is_ref<value_type>);
private:
  using super_type = dango::detail::tuple_storage<dango::detail::tuple_pack<tp_next...>>;
public:
  explicit constexpr
  tuple_storage
  ()noexcept(false):
  super_type{ }
  {

  }

  template
  <
    typename tp_arg,
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_next), sizeof...(tp_args)) &&
      !detail::is_tuple_tag<tp_arg>
    > = dango::enable_val
  >
  explicit constexpr
  tuple_storage
  (tp_arg&&, tp_args&&... a_args)noexcept(false):
  super_type(dango::forward<tp_args>(a_args)...)
  {

  }

  template
  <typename... tp_args, dango::enable_if<dango::is_equal(sizeof...(tp_next), sizeof...(tp_args))> = dango::enable_val>
  explicit constexpr
  tuple_storage
  (dango::value_init_tag const, tp_args&&... a_args)noexcept(false):
  super_type(dango::forward<tp_args>(a_args)...)
  {

  }

  template
  <typename... tp_args, dango::enable_if<dango::is_equal(sizeof...(tp_next), sizeof...(tp_args))> = dango::enable_val>
  explicit constexpr
  tuple_storage
  (dango::skip_init_tag const, tp_args&&... a_args)noexcept(false):
  super_type(dango::forward<tp_args>(a_args)...)
  {

  }

  explicit constexpr
  tuple_storage
  (tuple_storage const&)
  noexcept((... && dango::is_noexcept_copy_constructible<tp_next>)) = default;

  explicit constexpr
  tuple_storage
  (tuple_storage&&)
  noexcept((... && dango::is_noexcept_move_constructible<tp_next>)) = default;

  ~tuple_storage()noexcept = default;

  constexpr auto
  operator =
  (tuple_storage const&)&
  noexcept((... && dango::is_noexcept_copy_assignable<tp_next>))->tuple_storage& = default;

  constexpr auto
  operator =
  (tuple_storage&&)&
  noexcept((... && dango::is_noexcept_move_assignable<tp_next>))->tuple_storage& = default;

  template
  <
    dango::usize tp_index,
    dango::enable_if
    <
      dango::is_equal(tp_index, dango::usize(0)) &&
      !dango::is_const<value_type>
    > = dango::enable_val
  >
  constexpr auto
  get()noexcept->value_type&
  {
    return dango::detail::empty_elem_instance;
  }

  template
  <
    dango::usize tp_index,
    dango::enable_if
    <
      dango::is_equal(tp_index, dango::usize(0)) &&
      !dango::is_const<value_type>
    > = dango::enable_val
  >
  constexpr auto
  get()const noexcept->value_type const&
  {
    return dango::detail::empty_elem_instance;
  }

  template
  <
    dango::usize tp_index,
    dango::enable_if
    <
      dango::is_equal(tp_index, dango::usize(0)) &&
      dango::is_const<value_type>
    > = dango::enable_val
  >
  constexpr auto
  get()const noexcept->value_type&
  {
    return dango::detail::empty_elem_instance;
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
};

/*** not empty ***/

template
<typename tp_first, typename... tp_next>
class
dango::
detail::
tuple_storage
<dango::detail::tuple_pack<tp_first, tp_next...>, dango::enable_if<!dango::detail::is_empty_elem<tp_first>>>:
public dango::detail::tuple_storage<dango::detail::tuple_pack<tp_next...>>
{
public:
  using value_type = dango::tuple_value_type<tp_first>;
  static_assert(dango::is_same<value_type, tp_first>);
  static_assert(dango::is_noexcept_destructible<value_type>);
private:
  using super_type = dango::detail::tuple_storage<dango::detail::tuple_pack<tp_next...>>;
public:
  explicit constexpr
  tuple_storage
  ()noexcept(false):
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
      dango::is_equal(sizeof...(tp_next), sizeof...(tp_args)) &&
      !detail::is_tuple_tag<tp_arg>
    > = dango::enable_val
  >
  explicit constexpr
  tuple_storage
  (tp_arg&& a_arg, tp_args&&... a_args)noexcept(false):
  super_type(dango::forward<tp_args>(a_args)...),
  m_value(dango::forward<tp_arg>(a_arg))
  {

  }

  template
  <typename... tp_args, dango::enable_if<dango::is_equal(sizeof...(tp_next), sizeof...(tp_args))> = dango::enable_val>
  explicit constexpr
  tuple_storage
  (dango::value_init_tag const, tp_args&&... a_args)noexcept(false):
  super_type(dango::forward<tp_args>(a_args)...),
  m_value{ }
  {

  }

  template
  <typename... tp_args, dango::enable_if<dango::is_equal(sizeof...(tp_next), sizeof...(tp_args))> = dango::enable_val>
  explicit constexpr
  tuple_storage
  (dango::skip_init_tag const, tp_args&&... a_args)noexcept(false):
  super_type(dango::forward<tp_args>(a_args)...)
  {

  }

  explicit constexpr
  tuple_storage
  (tuple_storage const&)
  noexcept((dango::is_noexcept_copy_constructible<value_type> && ... && dango::is_noexcept_copy_constructible<tp_next>)) = default;

  explicit constexpr
  tuple_storage
  (tuple_storage&&)
  noexcept((dango::is_noexcept_move_constructible<value_type> && ... && dango::is_noexcept_move_constructible<tp_next>)) = default;

  ~tuple_storage()noexcept = default;

  constexpr auto
  operator =
  (tuple_storage const&)&
  noexcept((dango::is_noexcept_copy_assignable<value_type> && ... && dango::is_noexcept_copy_assignable<tp_next>))->tuple_storage& = default;

  constexpr auto
  operator =
  (tuple_storage&&)&
  noexcept((dango::is_noexcept_move_assignable<value_type> && ... && dango::is_noexcept_move_assignable<tp_next>))->tuple_storage& = default;

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

/*** make_tuple_storage ***/

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
    typename tp_make = dango::detail::make_tuple_storage_help<tp_storage_type, tp_args&&...>
  >
  constexpr auto
  make_tuple_storage
  (tp_args&&... a_args)
  noexcept(false)->tp_storage_type
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
  template
  <typename... tp_args>
  static constexpr auto
  make
  (tp_args&&... a_args)
  noexcept(false)->tp_storage_type
  {
    return tp_storage_type{ dango::forward<tp_args>(a_args)... };
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

  template
  <typename... tp_args>
  static constexpr auto
  make
  (tp_first a_first, tp_next... a_next, tp_args&&... a_args)
  noexcept(false)->tp_storage_type
  {
    return make_type::make(dango::forward<tp_next>(a_next)..., dango::forward<tp_first>(a_first), dango::forward<tp_args>(a_args)...);
  }

  DANGO_UNINSTANTIABLE(make_tuple_storage_help)
};

/*** tuple ***/

namespace
dango::detail
{
  template
  <typename... tp_types>
  using tuple_enable_spec =
    dango::enable_if
    <
      !dango::is_equal(sizeof...(tp_types), dango::usize(0)) &&
      (... && dango::is_noexcept_destructible<dango::tuple_value_type<tp_types>>),
      dango::enable_tag
    >;
}

namespace
dango
{
  template
  <typename tp_enabled, typename... tp_types>
  class tuple_impl;

  template
  <typename... tp_types>
  class tuple_impl<dango::detail::tuple_enable_spec<tp_types...>, tp_types...>;

  template<>
  class tuple_impl<dango::enable_tag>;

  template
  <typename... tp_types>
  using tuple = dango::tuple_impl<dango::enable_tag, tp_types...>;

  template
  <typename tp_first, typename tp_second>
  using pair = dango::tuple<tp_first, tp_second>;
}

template
<typename... tp_types>
class
dango::
tuple_impl
<dango::detail::tuple_enable_spec<tp_types...>, tp_types...>
{
private:
  using storage_type =
    detail::tuple_storage<detail::tuple_pack_reverse<dango::tuple_value_type<tp_types>...>>;

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
    bool tp_enabled = (... && dango::is_default_constructible<dango::tuple_value_type<tp_types>>),
    dango::enable_if<tp_enabled> = dango::enable_val
  >
  constexpr
  tuple_impl()
  noexcept((... && dango::is_noexcept_default_constructible<dango::tuple_value_type<tp_types>>)):
  m_storage{ }
  {

  }

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
  tuple_impl(tp_args&&... a_args)
  noexcept((... && detail::tuple_is_noexcept_constructible<tp_types, tp_args>)):
  m_storage(detail::make_tuple_storage<storage_type>(dango::forward<tp_args>(a_args)...))
  {

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
  tuple_impl(tp_args&&... a_args)
  noexcept((... && detail::tuple_is_noexcept_constructible<tp_types, tp_args>)):
  m_storage(detail::make_tuple_storage<storage_type>(dango::forward<tp_args>(a_args)...))
  {

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
  tuple_impl(tp_args&&... a_args)
  noexcept((... && detail::tuple_is_noexcept_constructible<tp_types, tp_args>)):
  m_storage(detail::make_tuple_storage<storage_type>(dango::forward<tp_args>(a_args)...))
  {

  }

  constexpr
  tuple_impl(tuple_impl const& a_tup)
  noexcept((... && dango::is_noexcept_copy_constructible<dango::tuple_value_type<tp_types>>)) = default;

  constexpr
  tuple_impl(tuple_impl&& a_tup)
  noexcept((... && dango::is_noexcept_move_constructible<dango::tuple_value_type<tp_types>>)) = default;

  ~tuple_impl()noexcept = default;

  constexpr auto
  operator =
  (tuple_impl const& a_tup)&
  noexcept((... && dango::is_noexcept_copy_assignable<dango::tuple_value_type<tp_types>>))->tuple_impl& = default;

  constexpr auto
  operator =
  (tuple_impl&& a_tup)&
  noexcept((... && dango::is_noexcept_move_assignable<dango::tuple_value_type<tp_types>>))->tuple_impl& = default;

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

  constexpr auto
  first
  ()const noexcept->decltype(auto)
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
    dango::enable_if<dango::is_gequal(tp_tuple_size, dango::usize(2))> = dango::enable_val
  >
  constexpr auto
  second
  ()const noexcept->decltype(auto)
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

  template
  <
    dango::usize tp_tuple_size = sizeof...(tp_types),
    dango::enable_if<dango::is_gequal(tp_tuple_size, dango::usize(3))> = dango::enable_val
  >
  constexpr auto
  third
  ()const noexcept->decltype(auto)
  {
    return get<dango::usize(2)>();
  }

private:

/*** converting constructor helpers ***/

  template
  <dango::usize... tp_indices, typename... tp_args>
  static constexpr auto
  construct
  (dango::index_seq<tp_indices...> const, tuple_impl<tp_args...> const& a_tup)
  noexcept(false)->storage_type
  {
    return detail::make_tuple_storage<storage_type>(a_tup.template get<tp_indices>()...);
  }

  template
  <dango::usize... tp_indices, typename... tp_args>
  static constexpr auto
  construct
  (dango::index_seq<tp_indices...> const, tuple_impl<tp_args...>&& a_tup)
  noexcept(false)->storage_type
  {
    return detail::make_tuple_storage<storage_type>(dango::forward<dango::tuple_value_type<tp_args>>(a_tup.template get<tp_indices>())...);
  }

public:

/*** converting-copy ***/

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      !(... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, dango::tuple_get_type<tp_args, true>>) &&
      (... && detail::tuple_is_convertible<dango::tuple_get_type<tp_args, true>, tp_types>)
    > = dango::enable_val
  >
  constexpr
  tuple_impl
  (tuple<tp_args...> const& a_tup)
  noexcept((... && detail::tuple_is_noexcept_constructible<tp_types, dango::tuple_get_type<tp_args, true>>)):
  m_storage(construct(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup))
  {

  }

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      !(... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, dango::tuple_get_type<tp_args, true>>) &&
      (... && detail::tuple_is_convertible<dango::tuple_get_type<tp_args, true>, tp_types>) &&
      !(... && dango::is_constructible<dango::tuple_value_type<tp_types>, tuple<tp_args...> const&>) &&
      !(... && dango::is_convertible<tuple<tp_args...> const&, dango::tuple_value_type<tp_types>>)
    > = dango::enable_val
  >
  constexpr
  tuple_impl
  (tuple<tp_args...> const& a_tup)
  noexcept((... && detail::tuple_is_noexcept_constructible<tp_types, dango::tuple_get_type<tp_args, true>>)):
  m_storage(construct(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup))
  {

  }

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      (... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, dango::tuple_get_type<tp_args, true>>)
    > = dango::enable_val
  >
  constexpr
  tuple_impl
  (tuple<tp_args...> const& a_tup)
  noexcept((... && detail::tuple_is_noexcept_constructible<tp_types, dango::tuple_get_type<tp_args, true>>)):
  m_storage(construct(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup))
  {

  }

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      (... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, dango::tuple_get_type<tp_args, true>>) &&
      !(... && dango::is_constructible<dango::tuple_value_type<tp_types>, tuple<tp_args...> const&>) &&
      !(... && dango::is_convertible<tuple<tp_args...> const&, dango::tuple_value_type<tp_types>>)
    > = dango::enable_val
  >
  constexpr
  tuple_impl
  (tuple<tp_args...> const& a_tup)
  noexcept((... && detail::tuple_is_noexcept_constructible<tp_types, dango::tuple_get_type<tp_args, true>>)):
  m_storage(construct(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup))
  {

  }

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      !(... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, dango::tuple_get_type<tp_args, true>>) &&
      !(... && detail::tuple_is_convertible<dango::tuple_get_type<tp_args, true>, tp_types>)
    > = dango::enable_val
  >
  explicit constexpr
  tuple_impl
  (tuple<tp_args...> const& a_tup)
  noexcept((... && detail::tuple_is_noexcept_constructible<tp_types, dango::tuple_get_type<tp_args, true>>)):
  m_storage(construct(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup))
  {

  }

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      !(... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, dango::tuple_get_type<tp_args, true>>) &&
      !(... && detail::tuple_is_convertible<dango::tuple_get_type<tp_args, true>, tp_types>) &&
      !(... && dango::is_constructible<dango::tuple_value_type<tp_types>, tuple<tp_args...> const&>) &&
      !(... && dango::is_convertible<tuple<tp_args...> const&, dango::tuple_value_type<tp_types>>)
    > = dango::enable_val
  >
  explicit constexpr
  tuple_impl
  (tuple<tp_args...> const& a_tup)
  noexcept((... && detail::tuple_is_noexcept_constructible<tp_types, dango::tuple_get_type<tp_args, true>>)):
  m_storage(construct(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup))
  {

  }

/*** converting-move ***/

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      !(... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, dango::tuple_value_type<tp_args>>) &&
      (... && detail::tuple_is_convertible<dango::tuple_value_type<tp_args>, tp_types>)
    > = dango::enable_val
  >
  constexpr
  tuple_impl
  (tuple<tp_args...>&& a_tup)
  noexcept((... && detail::tuple_is_noexcept_constructible<tp_types, dango::tuple_value_type<tp_args>>)):
  m_storage(construct(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tuple<tp_args...>&&>(a_tup)))
  {

  }

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      !(... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, dango::tuple_value_type<tp_args>>) &&
      (... && detail::tuple_is_convertible<dango::tuple_value_type<tp_args>, tp_types>) &&
      !(... && dango::is_constructible<dango::tuple_value_type<tp_types>, tuple<tp_args...>&&>) &&
      !(... && dango::is_convertible<tuple<tp_args...>&&, dango::tuple_value_type<tp_types>>)
    > = dango::enable_val
  >
  constexpr
  tuple_impl
  (tuple<tp_args...>&& a_tup)
  noexcept((... && detail::tuple_is_noexcept_constructible<tp_types, dango::tuple_value_type<tp_args>>)):
  m_storage(construct(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tuple<tp_args...>&&>(a_tup)))
  {

  }

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      (... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, dango::tuple_value_type<tp_args>>)
    > = dango::enable_val
  >
  constexpr
  tuple_impl
  (tuple<tp_args...>&& a_tup)
  noexcept((... && detail::tuple_is_noexcept_constructible<tp_types, dango::tuple_value_type<tp_args>>)):
  m_storage(construct(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tuple<tp_args...>&&>(a_tup)))
  {

  }

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      (... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, dango::tuple_value_type<tp_args>>) &&
      !(... && dango::is_constructible<dango::tuple_value_type<tp_types>, tuple<tp_args...>&&>) &&
      !(... && dango::is_convertible<tuple<tp_args...>&&, dango::tuple_value_type<tp_types>>)
    > = dango::enable_val
  >
  constexpr
  tuple_impl
  (tuple<tp_args...>&& a_tup)
  noexcept((... && detail::tuple_is_noexcept_constructible<tp_types, dango::tuple_value_type<tp_args>>)):
  m_storage(construct(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tuple<tp_args...>&&>(a_tup)))
  {

  }

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      !(... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, dango::tuple_value_type<tp_args>>) &&
      !(... && detail::tuple_is_convertible<dango::tuple_value_type<tp_args>, tp_types>)
    > = dango::enable_val
  >
  explicit constexpr
  tuple_impl
  (tuple<tp_args...>&& a_tup)
  noexcept((... && detail::tuple_is_noexcept_constructible<tp_types, dango::tuple_value_type<tp_args>>)):
  m_storage(construct(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tuple<tp_args...>&&>(a_tup)))
  {

  }

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      dango::is_equal(sizeof...(tp_types), dango::usize(1)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      !(... && detail::is_tuple_tag<tp_args>) &&
      (... && detail::tuple_is_constructible<tp_types, dango::tuple_value_type<tp_args>>) &&
      !(... && detail::tuple_is_convertible<dango::tuple_value_type<tp_args>, tp_types>) &&
      !(... && dango::is_constructible<dango::tuple_value_type<tp_types>, tuple<tp_args...>&&>) &&
      !(... && dango::is_convertible<tuple<tp_args...>&&, dango::tuple_value_type<tp_types>>)
    > = dango::enable_val
  >
  explicit constexpr
  tuple_impl
  (tuple<tp_args...>&& a_tup)
  noexcept((... && detail::tuple_is_noexcept_constructible<tp_types, dango::tuple_value_type<tp_args>>)):
  m_storage(construct(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tuple<tp_args...>&&>(a_tup)))
  {

  }

private:

/*** private converting assign ***/

  template
  <dango::usize... tp_indices, typename... tp_args>
  constexpr auto
  assign
  (dango::index_seq<tp_indices...> const, tuple<tp_args...> const& a_tup)
  noexcept(false)->tuple_impl&
  {
    [[maybe_unused]] dango::s_int const a_temp[] = { (static_cast<void>(get<tp_indices>() = a_tup.template get<tp_indices>()), 0)... };

    return *this;
  }

  template
  <dango::usize... tp_indices, typename... tp_args>
  constexpr auto
  assign
  (dango::index_seq<tp_indices...> const, tuple<tp_args...>&& a_tup)
  noexcept(false)->tuple_impl&
  {
    [[maybe_unused]] dango::s_int const a_temp[] = { (static_cast<void>(get<tp_indices>() = dango::move(a_tup.template get<tp_indices>())), 0)... };

    return *this;
  }

public:

/*** converting assign ***/

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      (... && !dango::is_ref<dango::tuple_value_type<tp_types>>) &&
      (... && dango::is_assignable<dango::tuple_get_type<tp_types, false>, dango::tuple_get_type<tp_args, true>>)
    > = dango::enable_val
  >
  constexpr auto
  operator =
  (tuple<tp_args...> const& a_tup)&
  noexcept
  ((... && dango::is_noexcept_assignable<dango::tuple_get_type<tp_types, false>, dango::tuple_get_type<tp_args, true>>))->tuple_impl&
  {
    return assign(dango::make_index_seq<sizeof...(tp_types)>{ }, a_tup);
  }

  template
  <
    typename... tp_args,
    dango::enable_if
    <
      dango::is_equal(sizeof...(tp_types), sizeof...(tp_args)) &&
      !(... && dango::is_same<tp_types, tp_args>) &&
      (... && !dango::is_ref<dango::tuple_value_type<tp_types>>) &&
      (... && dango::is_assignable<dango::tuple_get_type<tp_types, false>, dango::remove_ref<dango::tuple_get_type<tp_args, false>>&&>)
    > = dango::enable_val
  >
  constexpr auto
  operator =
  (tuple<tp_args...>&& a_tup)&
  noexcept
  ((... && dango::is_noexcept_assignable<dango::tuple_get_type<tp_types, false>, dango::remove_ref<dango::tuple_get_type<tp_args, false>>&&>))->tuple_impl&
  {
    return assign(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tuple<tp_args...>&&>(a_tup));
  }

private:

/*** call ***/

  template
  <dango::usize... tp_indices, typename tp_func>
  constexpr auto
  call
  (dango::index_seq<tp_indices...> const, tp_func&& a_func)
  noexcept(false)->decltype(auto)
  {
    return a_func(get<tp_indices>()...);
  }

  template
  <dango::usize... tp_indices, typename tp_func>
  constexpr auto
  call
  (dango::index_seq<tp_indices...> const, tp_func&& a_func)const
  noexcept(false)->decltype(auto)
  {
    return a_func(get<tp_indices>()...);
  }

public:

/*** operator ->* ***/

  template
  <
    typename tp_func,
    dango::enable_if
    <
      dango::is_callable<dango::remove_ref<tp_func>&, dango::tuple_get_type<tp_types, false>...>
    > = dango::enable_val
  >
  constexpr auto
  operator ->*
  (tp_func&& a_func)
  noexcept(dango::is_noexcept_callable<dango::remove_ref<tp_func>&, dango::tuple_get_type<tp_types, false>...>)->decltype(auto)
  {
    return call(dango::make_index_seq<sizeof...(tp_types)>{ }, dango::forward<tp_func>(a_func));
  }

  template
  <
    typename tp_func,
    dango::enable_if
    <
      dango::is_callable<dango::remove_ref<tp_func>&, dango::tuple_get_type<tp_types, true>...>
    > = dango::enable_val
  >
  constexpr auto
  operator ->*
  (tp_func&& a_func)const
  noexcept(dango::is_noexcept_callable<dango::remove_ref<tp_func>&, dango::tuple_get_type<tp_types, true>...>)->decltype(auto)
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
tuple_impl<dango::enable_tag>
{
public:
  static constexpr auto
  size
  ()noexcept->dango::usize
  {
    return dango::usize(0);
  }
public:
  DANGO_TAG_TYPE(tuple_impl)
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

/*** optional_elem ***/

namespace
dango
{
  template
  <bool tp_enabled, typename tp_elem_type>
  using optional_elem = dango::conditional<tp_enabled, tp_elem_type, dango::empty_elem>;
}

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
  static dango::usize const value;

  DANGO_UNINSTANTIABLE(tuple_size_help)
};

template
<typename... tp_types>
constexpr dango::usize const
dango::
detail::
tuple_size_help<tp_types...>::
value = sizeof...(tp_types);

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
  class tuple_size;

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
  class tuple_element;

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

