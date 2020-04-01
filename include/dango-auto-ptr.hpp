#ifndef __DANGO_AUTO_PTR_HPP__
#define __DANGO_AUTO_PTR_HPP__

/*** auto_ptr ***/

/*

namespace
dango::detail
{
  template
  <
    typename tp_destroyer,
    typename tp_type,
    bool tp_size,
    bool tp_align,
    typename tp_enabled = dango::enable_tag
  >
  constexpr bool const is_valid_auto_ptr_destroyer = false;

  template
  <typename tp_destroyer, typename tp_type>
  constexpr bool const
  is_valid_auto_ptr_destroyer
  <
    tp_destroyer,
    tp_type,
    false,
    false,
    dango::expr_check
    <
      dango::enable_if<dango::is_same<decltype(tp_destroyer::destroy(dango::declval<tp_type* const&>())), void>>,
      dango::enable_if<noexcept(tp_destroyer::destroy(dango::declval<tp_type* const&>()))>
    >
  > = true;

  template
  <typename tp_destroyer, typename tp_type>
  constexpr bool const
  is_valid_auto_ptr_destroyer
  <
    tp_destroyer,
    tp_type,
    false,
    true,
    dango::expr_check
    <
      dango::enable_if<dango::is_same<decltype(tp_destroyer::destroy(dango::declval<tp_type* const&>(), dango::declval<dango::usize const&>())), void>>,
      dango::enable_if<noexcept(tp_destroyer::destroy(dango::declval<tp_type* const&>(), dango::declval<dango::usize const&>()))>
    >
  > = true;

  template
  <typename tp_destroyer, typename tp_type>
  constexpr bool const
  is_valid_auto_ptr_destroyer
  <
    tp_destroyer,
    tp_type,
    true,
    false,
    dango::expr_check
    <
      dango::enable_if<dango::is_same<decltype(tp_destroyer::destroy(dango::declval<tp_type* const&>(), dango::declval<dango::usize const&>())), void>>,
      dango::enable_if<noexcept(tp_destroyer::destroy(dango::declval<tp_type* const&>(), dango::declval<dango::usize const&>()))>
    >
  > = true;

  template
  <typename tp_destroyer, typename tp_type>
  constexpr bool const
  is_valid_auto_ptr_destroyer
  <
    tp_destroyer,
    tp_type,
    true,
    true,
    dango::expr_check
    <
      dango::enable_if<dango::is_same<decltype(tp_destroyer::destroy(dango::declval<tp_type* const&>(), dango::declval<dango::usize const&>(), dango::declval<dango::usize const&>())), void>>,
      dango::enable_if<noexcept(tp_destroyer::destroy(dango::declval<tp_type* const&>(), dango::declval<dango::usize const&>(), dango::declval<dango::usize const&>()))>
    >
  > = true;

  template
  <typename tp_config, typename tp_type, typename tp_enabled = dango::enable_tag>
  constexpr bool const is_valid_auto_ptr_config = false;

  template
  <typename tp_config, typename tp_type>
  constexpr bool const
  is_valid_auto_ptr_config
  <
    tp_config,
    tp_type,
    dango::expr_check
    <
      dango::enable_if<dango::is_class<tp_config> && !dango::is_const<tp_config> && !dango::is_volatile<tp_config>>,
      dango::enable_if<dango::is_void<tp_type> || dango::is_scalar<tp_type> || dango::is_class<tp_type>>,
      dango::enable_if<dango::is_class<typename tp_config::destroyer> && !dango::is_const<typename tp_config::destroyer> && !dango::is_volatile<typename tp_config::destroyer>>,
      dango::enable_if<dango::is_same<decltype(tp_config::deep_const), bool const>>,
      dango::enable_if<dango::is_same<decltype(tp_config::move_constructor), bool const>>,
      dango::enable_if<dango::is_same<decltype(tp_config::move_assignment), bool const>>,
      dango::enable_if<dango::is_same<decltype(tp_config::explicit_conversion), bool const>>,
      dango::enable_if<dango::is_same<decltype(tp_config::require_same_destroyer), bool const>>,
      dango::enable_if<dango::is_same<decltype(tp_config::store_size), bool const>>,
      dango::enable_if<dango::is_same<decltype(tp_config::store_align), bool const>>,
      dango::enable_if<dango::is_same<decltype(tp_config::destroy_size), bool const>>,
      dango::enable_if<dango::is_same<decltype(tp_config::destroy_align), bool const>>,
      dango::enable_if<dango::is_same<decltype(tp_config::alignment), dango::usize const>>,
      dango::enable_if<dango::logical_implication(tp_config::move_assignment, tp_config::move_constructor)>,
      dango::enable_if<dango::logical_implication(tp_config::destroy_size, tp_config::store_size)>,
      dango::enable_if<dango::is_pow_two(tp_config::alignment)>,
      dango::enable_if<dango::detail::is_valid_auto_ptr_destroyer<typename tp_config::destroyer, tp_type, tp_config::destroy_size, tp_config::destroy_align>>
    >
  > = true;
}

namespace
dango
{
  template
  <typename tp_type, typename tp_enabled = dango::enable_tag>
  struct auto_ptr_default_destroyer;

  template
  <typename tp_type>
  struct
  auto_ptr_default_destroyer
  <tp_type, dango::enable_if<!dango::is_const<tp_type> && !dango::is_volatile<tp_type> && dango::is_void<tp_type>>>
  final
  {
    static void
    destroy
    (tp_type const volatile* const a_ptr, dango::usize const a_size, dango::usize const a_align)noexcept
    {
      dango::operator_delete(a_ptr, a_size, a_align);
    }

    DANGO_UNINSTANTIABLE(auto_ptr_default_destroyer)
  };

  template
  <typename tp_type>
  struct
  auto_ptr_default_destroyer
  <tp_type, dango::enable_if<!dango::is_const<tp_type> && !dango::is_volatile<tp_type> && (dango::is_scalar<tp_type> || dango::is_class<tp_type>)>>
  final
  {
    static void
    destroy
    (tp_type const volatile* const a_ptr)noexcept
    {
      delete a_ptr;
    }

    DANGO_UNINSTANTIABLE(auto_ptr_default_destroyer)
  };
}

namespace
dango
{
  template
  <
    typename tp_type,
    dango::usize tp_alignment = dango::alignof_with_void<tp_type, dango::usize(1)>,
    typename tp_destroyer = dango::auto_ptr_default_destroyer<dango::remove_cv<tp_type>>,
    typename tp_enabled = dango::enable_tag
  >
  struct auto_ptr_default_config;

  template
  <typename tp_type, dango::usize tp_alignment, typename tp_destroyer>
  struct
  auto_ptr_default_config
  <tp_type, tp_alignment, tp_destroyer, dango::enable_if<dango::is_void<tp_type> && dango::is_pow_two(tp_alignment)>>;

  template
  <typename tp_type, dango::usize tp_alignment, typename tp_destroyer>
  struct
  auto_ptr_default_config
  <tp_type, tp_alignment, tp_destroyer, dango::enable_if<(dango::is_scalar<tp_type> || dango::is_class<tp_type>) && dango::is_pow_two(tp_alignment)>>;
}

template
<typename tp_type, dango::usize tp_alignment, typename tp_destroyer>
struct
dango::
auto_ptr_default_config
<tp_type, tp_alignment, tp_destroyer, dango::enable_if<dango::is_void<tp_type> && dango::is_pow_two(tp_alignment)>>
{
  using destroyer = tp_destroyer;

  static constexpr bool const deep_const = false;
  static constexpr bool const move_constructor = true;
  static constexpr bool const move_assignment = true;
  static constexpr bool const explicit_conversion = false;
  static constexpr bool const require_same_destroyer = true;
  static constexpr bool const store_size = true;
  static constexpr bool const store_align = true;
  static constexpr bool const destroy_size = true;
  static constexpr bool const destroy_align = true;
  static constexpr dango::usize const alignment = tp_alignment;

  DANGO_UNINSTANTIABLE(auto_ptr_default_config)
};

static_assert(dango::detail::is_valid_auto_ptr_config<dango::auto_ptr_default_config<void>, void>);

template
<typename tp_type, dango::usize tp_alignment, typename tp_destroyer>
struct
dango::
auto_ptr_default_config
<tp_type, tp_alignment, tp_destroyer, dango::enable_if<(dango::is_scalar<tp_type> || dango::is_class<tp_type>) && dango::is_pow_two(tp_alignment)>>
{
  using destroyer = tp_destroyer;

  static constexpr bool const deep_const = false;
  static constexpr bool const move_constructor = true;
  static constexpr bool const move_assignment = true;
  static constexpr bool const explicit_conversion = false;
  static constexpr bool const require_same_destroyer = !dango::is_polymorphic<tp_type>;
  static constexpr bool const store_size = false;
  static constexpr bool const store_align = false;
  static constexpr bool const destroy_size = false;
  static constexpr bool const destroy_align = false;
  static constexpr dango::usize const alignment = tp_alignment;

  DANGO_UNINSTANTIABLE(auto_ptr_default_config)
};

static_assert(dango::detail::is_valid_auto_ptr_config<dango::auto_ptr_default_config<int>, int>);

namespace
dango
{
  template
  <typename tp_type, typename tp_config = dango::auto_ptr_default_config<tp_type>, typename tp_enabled = dango::enable_tag>
  class auto_ptr;

  template
  <typename tp_type, typename tp_config>
  class
  auto_ptr
  <
    tp_type,
    tp_config,
    dango::enable_if
    <
      (dango::is_void<tp_type> || dango::is_scalar<tp_type> || dango::is_class<tp_type>) &&
      dango::detail::is_valid_auto_ptr_config<tp_config, tp_type>
    >
  >;
}

template
<typename tp_type, typename tp_config>
class
dango::
auto_ptr
<
  tp_type,
  tp_config,
  dango::enable_if
  <
    (dango::is_void<tp_type> || dango::is_scalar<tp_type> || dango::is_class<tp_type>) &&
    dango::detail::is_valid_auto_ptr_config<tp_config, tp_type>
  >
>
final
{
  template
  <typename, typename, typename>
  friend class dango::auto_ptr;
private:
  using value_type = tp_type*;
  using const_type = dango::remove_const<tp_type> const*;
  using size_type = dango::usize;
  using tuple_type =
  dango::tuple
  <
    value_type,
    dango::optional_elem<tp_config::store_size, size_type>,
    dango::optional_elem<tp_config::store_align, size_type>
  >;
public:
  using config = tp_config;
private:
  constexpr auto get_ptr()noexcept->value_type&{ return m_data.first(); }
  constexpr auto get_ptr()const noexcept->value_type const&{ return m_data.first(); }

  template
  <typename tp_tp_config = config>
  constexpr auto get_size()noexcept->dango::enable_if<tp_tp_config::store_size, size_type&>{ return m_data.second(); }
  template
  <typename tp_tp_config = config>
  constexpr auto get_size()const noexcept->dango::enable_if<tp_tp_config::store_size, size_type const&>{ return m_data.second(); }

  template
  <typename tp_tp_config = config>
  constexpr auto get_align()noexcept->dango::enable_if<tp_tp_config::store_align, size_type&>{ return m_data.third(); }
  template
  <typename tp_tp_config = config>
  constexpr auto get_align()const noexcept->dango::enable_if<tp_tp_config::store_align, size_type const&>{ return m_data.third(); }
public:
  constexpr
  auto_ptr
  (dango::null_tag const)noexcept:
  m_data{ dango::null, size_type(0), size_type(0) }
  { }

  template
  <typename tp_tp_config = config, dango::enable_if<tp_tp_config::store_size && !tp_tp_config::store_align> = dango::enable_val>
  constexpr explicit auto_ptr(dango::null_tag, size_type)noexcept = delete;
  template
  <typename tp_tp_config = config, dango::enable_if<!tp_tp_config::store_size && tp_tp_config::store_align> = dango::enable_val>
  constexpr explicit auto_ptr(dango::null_tag, size_type)noexcept = delete;
  template
  <typename tp_tp_config = config, dango::enable_if<tp_tp_config::store_size && tp_tp_config::store_align> = dango::enable_val>
  constexpr explicit auto_ptr(dango::null_tag, size_type, size_type)noexcept = delete;

  template
  <
    typename tp_arg,
    dango::enable_if
    <
      (!config::store_size && !config::store_align) &&
      dango::logical_equivalence(dango::is_void<tp_arg>, dango::is_void<tp_type>) &&
      dango::is_constructible<value_type, tp_arg* const&>
    > = dango::enable_val
  >
  constexpr explicit
  auto_ptr
  (tp_arg* const a_ptr)noexcept:
  m_data{ a_ptr, dango::skip_init, dango::skip_init }
  {
    dango_assert(a_ptr != dango::null);
    dango_assert(dango::is_aligned(a_ptr, config::alignment));
  }

  template
  <
    typename tp_arg,
    dango::enable_if
    <
      (config::store_size && !config::store_align) &&
      dango::logical_equivalence(dango::is_void<tp_arg>, dango::is_void<tp_type>) &&
      dango::is_constructible<value_type, tp_arg* const&>
    > = dango::enable_val
  >
  constexpr explicit
  auto_ptr
  (tp_arg* const a_ptr, size_type const a_size)noexcept:
  m_data{ a_ptr, a_size, dango::skip_init }
  {
    dango_assert(a_ptr != dango::null);
    dango_assert(a_size != size_type(0));
    dango_assert(dango::is_aligned(a_ptr, config::alignment));
  }

  template
  <
    typename tp_arg,
    dango::enable_if
    <
      (!config::store_size && config::store_align) &&
      dango::logical_equivalence(dango::is_void<tp_arg>, dango::is_void<tp_type>) &&
      dango::is_constructible<value_type, tp_arg* const&>
    > = dango::enable_val
  >
  constexpr explicit
  auto_ptr
  (tp_arg* const a_ptr, size_type const a_align)noexcept:
  m_data{ a_ptr, dango::skip_init, a_align }
  {
    dango_assert(a_ptr != dango::null);
    dango_assert(dango::is_pow_two(a_align));
    dango_assert(a_align >= config::alignment);
    dango_assert(dango::is_aligned(a_ptr, a_align));
  }

  template
  <
    typename tp_arg,
    dango::enable_if
    <
      (config::store_size && config::store_align) &&
      dango::logical_equivalence(dango::is_void<tp_arg>, dango::is_void<tp_type>) &&
      dango::is_constructible<value_type, tp_arg* const&>
    > = dango::enable_val
  >
  constexpr explicit
  auto_ptr
  (tp_arg* const a_ptr, size_type const a_size, size_type const a_align)noexcept:
  m_data{ a_ptr, a_size, a_align }
  {
    dango_assert(a_ptr != dango::null);
    dango_assert(a_size != size_type(0));
    dango_assert(dango::is_pow_two(a_align));
    dango_assert(a_align >= config::alignment);
    dango_assert(dango::is_aligned(a_ptr, a_align));
  }

  template
  <typename tp_tp_config = config, dango::enable_if<tp_tp_config::move_constructor> = dango::enable_val>
  constexpr
  auto_ptr
  (auto_ptr&& a_ptr)noexcept:
  m_data{ a_ptr.m_data }
  {
    a_ptr.get_ptr() = dango::null;

    if constexpr(config::store_size)
    {
      a_ptr.get_size() = size_type(0);
    }

    if constexpr(config::store_align)
    {
      a_ptr.get_align() = size_type(0);
    }
  }

  template
  <typename tp_tp_config = config, dango::enable_if<!tp_tp_config::move_constructor> = dango::enable_val>
  constexpr auto_ptr(auto_ptr&&)noexcept = delete;

  template
  <
    typename tp_arg,
    typename tp_arg_config,
    dango::enable_if
    <
      (!dango::is_same<tp_arg, tp_type> || !dango::is_same<tp_arg_config, config>) &&
      config::move_constructor &&
      dango::logical_equivalence(tp_arg_config::require_same_destroyer, config::require_same_destroyer) &&
      dango::logical_implication(config::require_same_destroyer, dango::is_same<typename tp_arg_config::destroyer, typename config::destroyer>) &&
      dango::is_gequal(tp_arg_config::alignment, config::alignment) &&
      dango::logical_equivalence(tp_arg_config::store_size, config::store_size) &&
      dango::logical_equivalence(tp_arg_config::store_align, config::store_align) &&
      dango::logical_equivalence(dango::is_void<tp_arg>, dango::is_void<tp_type>) &&
      dango::is_constructible<value_type, tp_arg* const&>
    > = dango::enable_val
  >
  constexpr auto_ptr(auto_ptr<tp_arg, tp_arg_config> const&)noexcept = delete;

  template
  <
    typename tp_arg,
    typename tp_arg_config,
    dango::enable_if
    <
      (!dango::is_same<tp_arg, tp_type> || !dango::is_same<tp_arg_config, config>) &&
      config::move_constructor &&
      dango::logical_equivalence(tp_arg_config::require_same_destroyer, config::require_same_destroyer) &&
      dango::logical_implication(config::require_same_destroyer, dango::is_same<typename tp_arg_config::destroyer, typename config::destroyer>) &&
      dango::is_gequal(tp_arg_config::alignment, config::alignment) &&
      dango::logical_equivalence(tp_arg_config::store_size, config::store_size) &&
      dango::logical_equivalence(tp_arg_config::store_align, config::store_align) &&
      dango::logical_equivalence(dango::is_void<tp_arg>, dango::is_void<tp_type>) &&
      dango::is_constructible<value_type, tp_arg* const&>
      > = dango::enable_val
  >
  constexpr
  auto_ptr
  (auto_ptr<tp_arg, tp_arg_config>&& a_ptr)noexcept:
  m_data{ a_ptr.m_data }
  {
    a_ptr.get_ptr() = dango::null;

    if constexpr(config::store_size)
    {
      a_ptr.get_size() = size_type(0);
    }

    if constexpr(config::store_align)
    {
      a_ptr.get_align() = size_type(0);
    }
  }

  ~auto_ptr
  ()noexcept
  {
    if(!get_ptr())
    {
      return;
    }

    if constexpr(config::store_align)
    {
      if constexpr(config::destroy_size && config::destroy_align)
      {
        config::destroyer::destroy(get_ptr(), get_size(), get_align());
      }

      if constexpr(!config::destroy_size && config::destroy_align)
      {
        config::destroyer::destroy(get_ptr(), get_align());
      }
    }
    else
    {
      if constexpr(config::destroy_size && config::destroy_align)
      {
        config::destroyer::destroy(get_ptr(), get_size(), config::alignment);
      }

      if constexpr(!config::destroy_size && config::destroy_align)
      {
        config::destroyer::destroy(get_ptr(), config::alignment);
      }
    }

    if constexpr(config::destroy_size && !config::destroy_align)
    {
      config::destroyer::destroy(get_ptr(), get_size());
    }

    if constexpr(!config::destroy_size && !config::destroy_align)
    {
      config::destroyer::destroy(get_ptr());
    }
  }

  auto
  operator =
  (dango::null_tag const)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::null };

    dango::swap(get_ptr(), a_temp.get_ptr());

    if constexpr(config::store_size)
    {
      dango::swap(get_size(), a_temp.get_size());
    }

    if constexpr(config::store_align)
    {
      dango::swap(get_align(), a_temp.get_align());
    }

    return *this;
  }

  template
  <typename tp_tp_config = config, dango::enable_if<tp_tp_config::move_assignment> = dango::enable_val>
  auto
  operator =
  (auto_ptr&& a_ptr)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(get_ptr(), a_temp.get_ptr());

    if constexpr(config::store_size)
    {
      dango::swap(get_size(), a_temp.get_size());
    }

    if constexpr(config::store_align)
    {
      dango::swap(get_align(), a_temp.get_align());
    }

    return *this;
  }

  template
  <typename tp_tp_config = config, dango::enable_if<!tp_tp_config::move_assignment> = dango::enable_val>
  constexpr auto operator = (auto_ptr&& a_ptr)& noexcept->auto_ptr& = delete;

  template
  <
    typename tp_arg,
    typename tp_arg_config,
    dango::enable_if
    <
      (!dango::is_same<tp_arg, tp_type> || !dango::is_same<tp_arg_config, config>) &&
      config::move_assignment &&
      dango::logical_equivalence(tp_arg_config::require_same_destroyer, config::require_same_destroyer) &&
      dango::logical_implication(config::require_same_destroyer, dango::is_same<typename tp_arg_config::destroyer, typename config::destroyer>) &&
      dango::is_gequal(tp_arg_config::alignment, config::alignment) &&
      dango::logical_equivalence(tp_arg_config::store_size, config::store_size) &&
      dango::logical_equivalence(tp_arg_config::store_align, config::store_align) &&
      dango::logical_equivalence(dango::is_void<tp_arg>, dango::is_void<tp_type>) &&
      dango::is_constructible<value_type, tp_arg* const&>
    > = dango::enable_val
  >
  auto operator = (auto_ptr<tp_arg, tp_arg_config> const& a_ptr)& noexcept->auto_ptr& = delete;

  template
  <
    typename tp_arg,
    typename tp_arg_config,
    dango::enable_if
    <
      (!dango::is_same<tp_arg, tp_type> || !dango::is_same<tp_arg_config, config>) &&
      config::move_assignment &&
      dango::logical_equivalence(tp_arg_config::require_same_destroyer, config::require_same_destroyer) &&
      dango::logical_implication(config::require_same_destroyer, dango::is_same<typename tp_arg_config::destroyer, typename config::destroyer>) &&
      dango::is_gequal(tp_arg_config::alignment, config::alignment) &&
      dango::logical_equivalence(tp_arg_config::store_size, config::store_size) &&
      dango::logical_equivalence(tp_arg_config::store_align, config::store_align) &&
      dango::logical_equivalence(dango::is_void<tp_arg>, dango::is_void<tp_type>) &&
      dango::is_constructible<value_type, tp_arg* const&>
    > = dango::enable_val
  >
  auto
  operator =
  (auto_ptr<tp_arg, tp_arg_config>&& a_ptr)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    dango::swap(get_ptr(), a_temp.get_ptr());

    if constexpr(config::store_size)
    {
      dango::swap(get_size(), a_temp.get_size());
    }

    if constexpr(config::store_align)
    {
      dango::swap(get_align(), a_temp.get_align());
    }

    return *this;
  }

  constexpr auto dango_operator_is_null()const noexcept->bool{ return get_ptr() == dango::null; }

  template
  <typename tp_tp_config = config>
  constexpr auto get()const noexcept->dango::enable_if<!tp_tp_config::deep_const, value_type>{ return dango::assume_aligned<config::alignment>(get_ptr()); }
  template
  <typename tp_tp_config = config>
  constexpr auto get()noexcept->dango::enable_if<tp_tp_config::deep_const, value_type>{ return dango::assume_aligned<config::alignment>(get_ptr()); }
  template
  <typename tp_tp_config = config>
  constexpr auto get()const noexcept->dango::enable_if<tp_tp_config::deep_const, const_type>{ return dango::assume_aligned<config::alignment>(get_ptr()); }

  template
  <typename tp_tp_config = config, dango::enable_if<!tp_tp_config::deep_const && !tp_tp_config::explicit_conversion> = dango::enable_val>
  constexpr operator value_type()const noexcept{ return get(); }
  template
  <typename tp_tp_config = config, dango::enable_if<tp_tp_config::deep_const && !tp_tp_config::explicit_conversion> = dango::enable_val>
  constexpr operator value_type()noexcept{ return get(); }
  template
  <typename tp_tp_config = config, dango::enable_if<tp_tp_config::deep_const && !tp_tp_config::explicit_conversion> = dango::enable_val>
  constexpr operator const_type()const noexcept{ return get(); }

  template
  <typename tp_tp_config = config, dango::enable_if<!tp_tp_config::deep_const && tp_tp_config::explicit_conversion> = dango::enable_val>
  constexpr explicit operator value_type()const noexcept{ return get(); }
  template
  <typename tp_tp_config = config, dango::enable_if<tp_tp_config::deep_const && tp_tp_config::explicit_conversion> = dango::enable_val>
  constexpr explicit operator value_type()noexcept{ return get(); }
  template
  <typename tp_tp_config = config, dango::enable_if<tp_tp_config::deep_const && tp_tp_config::explicit_conversion> = dango::enable_val>
  constexpr explicit operator const_type()const noexcept{ return get(); }

  template
  <typename tp_tp_config = config, dango::enable_if<tp_tp_config::store_size> = dango::enable_val>
  constexpr auto size()const noexcept->size_type{ return get_size(); }

  template
  <typename tp_tp_config = config, dango::enable_if<tp_tp_config::store_align> = dango::enable_val>
  constexpr auto align()const noexcept->size_type{ return get_align(); }
  template
  <typename tp_tp_config = config, dango::enable_if<!tp_tp_config::store_align> = dango::enable_val>
  constexpr auto align()const noexcept->size_type{ return config::alignment; }

  constexpr auto
  dismiss
  ()noexcept->value_type
  {
    auto const a_ptr = get();

    get_ptr() = dango::null;

    if constexpr(config::store_size)
    {
      get_size() = size_type(0);
    }

    if constexpr(config::store_align)
    {
      get_align() = size_type(0);
    }

    return a_ptr;
  }

  constexpr explicit operator bool()const noexcept{ return get_ptr() != dango::null; }

  template
  <typename tp_tp_config = config>
  constexpr auto operator -> ()const noexcept->dango::enable_if<!dango::is_void<tp_type> && !tp_tp_config::deep_const, value_type>{ return get(); }
  template
  <typename tp_tp_config = config>
  constexpr auto operator -> ()noexcept->dango::enable_if<!dango::is_void<tp_type> && tp_tp_config::deep_const, value_type>{ return get(); }
  template
  <typename tp_tp_config = config>
  constexpr auto operator -> ()const noexcept->dango::enable_if<!dango::is_void<tp_type> && tp_tp_config::deep_const, const_type>{ return get(); }

  template
  <typename tp_tp_type = tp_type>
  constexpr auto operator * ()const noexcept->dango::enable_if<!dango::is_void<tp_tp_type> && !config::deep_const, tp_tp_type&>{ return *get(); }
  template
  <typename tp_tp_type = tp_type>
  constexpr auto operator * ()noexcept->dango::enable_if<!dango::is_void<tp_tp_type> && config::deep_const, tp_tp_type&>{ return *get(); }
  template
  <typename tp_tp_type = tp_type>
  constexpr auto operator * ()const noexcept->dango::enable_if<!dango::is_void<tp_tp_type> && config::deep_const, dango::remove_const<tp_tp_type> const&>{ return *get(); }
private:
  tuple_type m_data;
public:
  DANGO_DELETE_DEFAULT(auto_ptr)
  DANGO_UNCOPYABLE(auto_ptr)
};

*/

/*** auto_new ***/

/*

namespace
dango::detail
{
  template
  <dango::usize tp_align, bool tp_store_align>
  struct
  auto_new_config
  final:
  public dango::auto_ptr_default_config<void, tp_align>
  {
    static constexpr bool const store_align = tp_store_align;

    DANGO_UNINSTANTIABLE(auto_new_config)
  };
}

namespace
dango
{
  template
  <dango::usize tp_align = alignof(dango::max_align_type)>
  auto
  auto_new(dango::usize const a_size)dango_new_noexcept->
  dango::enable_if
  <dango::is_pow_two(tp_align), dango::auto_ptr<void, dango::detail::auto_new_config<tp_align, false>>>
  {
    using return_type = dango::auto_ptr<void, dango::detail::auto_new_config<tp_align, false>>;

    auto const a_ptr = dango::operator_new(a_size, tp_align);

    return return_type{ a_ptr, a_size };
  }

  template
  <dango::usize tp_align = alignof(dango::max_align_type)>
  auto
  auto_new(dango::usize const a_size, dango::usize const a_align)dango_new_noexcept->
  dango::enable_if
  <dango::is_pow_two(tp_align), dango::auto_ptr<void, dango::detail::auto_new_config<tp_align, true>>>
  {
    using return_type = dango::auto_ptr<void, dango::detail::auto_new_config<tp_align, true>>;

    dango_assert(dango::is_pow_two(a_align));
    dango_assert(a_align >= tp_align);

    auto const a_ptr = dango::operator_new(a_size, a_align);

    return return_type{ a_ptr, a_size, a_align };
  }
}

*/

#endif

