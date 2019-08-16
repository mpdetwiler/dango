#ifndef __DANGO_MEM_HPP__
#define __DANGO_MEM_HPP__

/*** placement new ***/

namespace
dango
{
  struct
  placement_tag
  final
  {
    DANGO_TAG_TYPE(placement_tag)
  };

  inline constexpr dango::placement_tag const placement{ };
}

[[nodiscard]] constexpr auto
operator new
(dango::usize, dango::placement_tag, void*)noexcept->void*;

[[nodiscard]] constexpr auto
operator new[]
(dango::usize, dango::placement_tag, void*)noexcept->void*;

constexpr void
operator delete
(void*, dango::placement_tag, void*)noexcept;

constexpr void
operator delete[]
(void*, dango::placement_tag, void*)noexcept;

constexpr auto
operator new
(
  dango::usize const,
  dango::placement_tag const,
  void* const a_addr
)
noexcept->void*
{
  return a_addr;
}

constexpr auto
operator new[]
(
  dango::usize const,
  dango::placement_tag const,
  void* const a_addr
)
noexcept->void*
{
  return a_addr;
}

constexpr void
operator delete
(
  void* const,
  dango::placement_tag const,
  void* const
)
noexcept
{

}

constexpr void
operator delete[]
(
  void* const,
  dango::placement_tag const,
  void* const
)
noexcept
{

}

/*** destructor ***/

namespace
dango
{
  template
  <typename tp_type>
  auto
  destructor
  (tp_type const volatile*)
  noexcept(dango::is_noexcept_destructible<tp_type>)->
  dango::enable_if<dango::is_destructible<tp_type>, void>;
}

template
<typename tp_type>
auto
dango::
destructor
(tp_type const volatile* const a_ptr)
noexcept(dango::is_noexcept_destructible<tp_type>)->
dango::enable_if<dango::is_destructible<tp_type>, void>
{
  dango_assert(a_ptr != nullptr);

  a_ptr->~tp_type();
}

/*** dango_new_noexcept ***/

namespace
dango
{
#ifndef DANGO_NEW_NOEXCEPT
  inline constexpr bool const c_operator_new_noexcept = false;
#else
  inline constexpr bool const c_operator_new_noexcept = true;
#endif
}

namespace
dango::detail
{
  constexpr auto true_bool()noexcept->bool;

  template
  <typename tp_type>
  constexpr auto
  true_bool
  (tp_type&&)noexcept->
  dango::enable_if<dango::is_constructible<bool, tp_type>, bool>;
}

constexpr auto
dango::
detail::
true_bool
()noexcept->bool
{
  return true;
}

template
<typename tp_type>
constexpr auto
dango::
detail::
true_bool
(tp_type&& a_arg)noexcept->
dango::enable_if<dango::is_constructible<bool, tp_type>, bool>
{
  return bool(dango::forward<tp_type>(a_arg));
}

#define dango_new_noexcept(...) \
noexcept(dango::c_operator_new_noexcept && dango::detail::true_bool(__VA_ARGS__))

/*** launder ***/

namespace
dango
{
  template
  <typename tp_type>
  [[nodiscard]] constexpr auto
  launder
  (tp_type*)noexcept->
  dango::enable_if<dango::is_object<tp_type>, tp_type*>;
}

template
<typename tp_type>
constexpr auto
dango::
launder
(tp_type* const a_arg)noexcept->
dango::enable_if<dango::is_object<tp_type>, tp_type*>
{
  return __builtin_launder(a_arg);
}

/*** address_of ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr auto
  address_of
  (tp_type&)noexcept->
  dango::enable_if<dango::is_object<tp_type>, tp_type*>;
}

template
<typename tp_type>
constexpr auto
dango::
address_of
(tp_type& a_arg)noexcept->
dango::enable_if<dango::is_object<tp_type>, tp_type*>
{
  using cast_type = dango::preserve_cv<tp_type, dango::byte>;

  auto& a_ref = reinterpret_cast<cast_type&>(a_arg);

  return reinterpret_cast<tp_type*>(&a_ref);
}

/*** is_aligned ***/

namespace
dango
{
  constexpr auto is_aligned(void const volatile*, dango::usize)noexcept->bool;
}

constexpr auto
dango::
is_aligned
(void const volatile* const a_ptr, dango::usize const a_align)noexcept->bool
{
  dango_assert(dango::is_pow_two(a_align));

  auto const a_int = reinterpret_cast<dango::uintptr>(a_ptr);

  return (a_int % dango::uintptr(a_align)) == dango::uintptr(0);
}

/*** assume_aligned ***/

namespace
dango
{
  template
  <dango::usize tp_align, typename tp_type>
  [[nodiscard]] constexpr auto
  assume_aligned
  (tp_type*)noexcept->
  dango::enable_if<dango::is_pow_two(tp_align) && !dango::is_func<tp_type>, tp_type*>;
}

template
<dango::usize tp_align, typename tp_type>
[[nodiscard]] constexpr auto
dango::
assume_aligned
(tp_type* const a_ptr)noexcept->
dango::enable_if<dango::is_pow_two(tp_align) && !dango::is_func<tp_type>, tp_type*>
{
  dango_assert(dango::is_aligned(a_ptr, tp_align));

  auto const a_result = __builtin_assume_aligned(a_ptr, tp_align);

  return static_cast<tp_type*>(a_result);
}

/*** operator_new operator_delete ***/

namespace
dango
{
  [[nodiscard]] auto
  operator_new
  (dango::usize, dango::usize)dango_new_noexcept(true)->void*;

  void operator_delete(void const volatile*, dango::usize, dango::usize)noexcept;
}

/*** auto_ptr ***/

namespace
dango::detail
{
  template
  <
    typename tp_destroyer,
    typename tp_type,
    bool tp_size,
    bool tp_align,
    typename tp_enabled = void
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
    dango::void_type
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
    dango::void_type
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
    dango::void_type
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
    dango::void_type
    <
      dango::enable_if<dango::is_same<decltype(tp_destroyer::destroy(dango::declval<tp_type* const&>(), dango::declval<dango::usize const&>(), dango::declval<dango::usize const&>())), void>>,
      dango::enable_if<noexcept(tp_destroyer::destroy(dango::declval<tp_type* const&>(), dango::declval<dango::usize const&>(), dango::declval<dango::usize const&>()))>
    >
  > = true;

  template
  <typename tp_config, typename tp_type, typename tp_enabled = void>
  constexpr bool const is_valid_auto_ptr_config = false;

  template
  <typename tp_config, typename tp_type>
  constexpr bool const
  is_valid_auto_ptr_config
  <
    tp_config,
    tp_type,
    dango::void_type
    <
      dango::enable_if<dango::is_class<tp_config> && !dango::is_const<tp_config> && !dango::is_volatile<tp_config>>,
      dango::enable_if<dango::is_void<tp_type> || dango::is_scalar<tp_type> || dango::is_class<tp_type>>,
      dango::enable_if<dango::is_class<typename tp_config::destroyer> && !dango::is_const<typename tp_config::destroyer> && !dango::is_volatile<typename tp_config::destroyer>>,
      dango::enable_if<dango::is_same<decltype(tp_config::deep_const), bool const>>,
      dango::enable_if<dango::is_same<decltype(tp_config::move_constructor), bool const>>,
      dango::enable_if<dango::is_same<decltype(tp_config::move_assignment), bool const>>,
      dango::enable_if<dango::is_same<decltype(tp_config::explicit_conversion), bool const>>,
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

namespace
dango::detail
{
  template
  <bool tp_size, bool tp_align>
  struct
  auto_ptr_size_align
  {
  protected:
    DANGO_TAG_TYPE(auto_ptr_size_align)
  };

  template<>
  struct
  auto_ptr_size_align<false, true>
  {
  private:
    using value_type = dango::usize;
  protected:
    constexpr auto_ptr_size_align()noexcept:m_align{ value_type(0) }{ }
    constexpr
    auto_ptr_size_align
    (value_type const a_align)noexcept:
    m_align{ a_align }
    { }
    constexpr auto_ptr_size_align(auto_ptr_size_align&& a_arg)noexcept:m_align{ a_arg.m_align }{ a_arg.m_align = value_type(0); }
    ~auto_ptr_size_align()noexcept = default;
    constexpr auto get_align()noexcept->value_type&{ return m_align; }
    constexpr auto get_align()const noexcept->value_type const&{ return m_align; }
  private:
    value_type m_align;
  protected:
    DANGO_UNCOPYABLE(auto_ptr_size_align)
  };

  template<>
  struct
  auto_ptr_size_align<true, false>
  {
  private:
    using value_type = dango::usize;
  protected:
    constexpr auto_ptr_size_align()noexcept:m_size{ value_type(0) }{ }
    constexpr
    auto_ptr_size_align
    (value_type const a_size)noexcept:
    m_size{ a_size }
    { }
    constexpr auto_ptr_size_align(auto_ptr_size_align&& a_arg)noexcept:m_size{ a_arg.m_size }{ a_arg.m_size = value_type(0); }
    ~auto_ptr_size_align()noexcept = default;
    constexpr auto get_size()noexcept->value_type&{ return m_size; }
    constexpr auto get_size()const noexcept->value_type const&{ return m_size; }
  private:
    value_type m_size;
  protected:
    DANGO_UNCOPYABLE(auto_ptr_size_align)
  };

  template<>
  struct
  auto_ptr_size_align<true, true>
  {
  private:
    using value_type = dango::usize;
  protected:
    constexpr
    auto_ptr_size_align()noexcept:
    m_size{ value_type(0) },
    m_align{ value_type(0) }
    { }
    constexpr
    auto_ptr_size_align
    (value_type const a_size, value_type const a_align)noexcept:
    m_size{ a_size },
    m_align{ a_align }
    { }
    constexpr
    auto_ptr_size_align(auto_ptr_size_align&& a_arg)noexcept:
    m_size{ a_arg.m_size },
    m_align{ a_arg.m_align }
    {
      a_arg.m_size = value_type(0);
      a_arg.m_align = value_type(0);
    }
    ~auto_ptr_size_align()noexcept = default;
    constexpr auto get_size()noexcept->value_type&{ return m_size; }
    constexpr auto get_size()const noexcept->value_type const&{ return m_size; }
    constexpr auto get_align()noexcept->value_type&{ return m_align; }
    constexpr auto get_align()const noexcept->value_type const&{ return m_align; }
  private:
    value_type m_size;
    value_type m_align;
  protected:
    DANGO_UNCOPYABLE(auto_ptr_size_align)
  };
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
final:
private dango::detail::auto_ptr_size_align<tp_config::store_size, tp_config::store_align>
{
  template
  <typename, typename, typename>
  friend class dango::auto_ptr;
private:
  using super_type = dango::detail::auto_ptr_size_align<tp_config::store_size, tp_config::store_align>;
  using value_type = tp_type*;
  using const_type = dango::remove_const<tp_type> const*;
public:
  using config = tp_config;
public:
  constexpr
  auto_ptr
  (dango::null_tag const)noexcept:
  super_type{ },
  m_ptr{ nullptr }
  { }

  template
  <typename tp_tp_config = config, dango::enable_if<!tp_tp_config::store_size && !tp_tp_config::store_align> = dango::enable_val>
  constexpr explicit auto_ptr(dango::nullptr_tag)noexcept = delete;
  template
  <typename tp_tp_config = config, dango::enable_if<tp_tp_config::store_size && !tp_tp_config::store_align> = dango::enable_val>
  constexpr explicit auto_ptr(dango::nullptr_tag, dango::usize)noexcept = delete;
  template
  <typename tp_tp_config = config, dango::enable_if<!tp_tp_config::store_size && tp_tp_config::store_align> = dango::enable_val>
  constexpr explicit auto_ptr(dango::nullptr_tag, dango::usize)noexcept = delete;
  template
  <typename tp_tp_config = config, dango::enable_if<tp_tp_config::store_size && tp_tp_config::store_align> = dango::enable_val>
  constexpr explicit auto_ptr(dango::nullptr_tag, dango::usize, dango::usize)noexcept = delete;

  template
  <
    typename tp_arg,
    dango::enable_if
    <
      (!config::store_size && !config::store_align) &&
      dango::logical_equivalence(dango::is_void<tp_arg>, dango::is_void<tp_type>) &&
      dango::is_constructible<value_type, tp_arg*>
    > = dango::enable_val
  >
  constexpr explicit
  auto_ptr
  (tp_arg* const a_ptr)noexcept:
  super_type{ },
  m_ptr{ a_ptr }
  { }

  template
  <
    typename tp_arg,
    dango::enable_if
    <
      (config::store_size && !config::store_align) &&
      dango::logical_equivalence(dango::is_void<tp_arg>, dango::is_void<tp_type>) &&
      dango::is_constructible<value_type, tp_arg*>
    > = dango::enable_val
  >
  constexpr explicit
  auto_ptr
  (tp_arg* const a_ptr, dango::usize const a_size)noexcept:
  super_type{ a_size },
  m_ptr{ a_ptr }
  { }

  template
  <
    typename tp_arg,
    dango::enable_if
    <
      (!config::store_size && config::store_align) &&
      dango::logical_equivalence(dango::is_void<tp_arg>, dango::is_void<tp_type>) &&
      dango::is_constructible<value_type, tp_arg*>
    > = dango::enable_val
  >
  constexpr explicit
  auto_ptr
  (tp_arg* const a_ptr, dango::usize const a_align)noexcept:
  super_type{ a_align },
  m_ptr{ a_ptr }
  {
    dango_assert(dango::is_pow_two(a_align));
    dango_assert(a_align >= config::alignment);
  }

  template
  <
    typename tp_arg,
    dango::enable_if
    <
      (config::store_size && config::store_align) &&
      dango::logical_equivalence(dango::is_void<tp_arg>, dango::is_void<tp_type>) &&
      dango::is_constructible<value_type, tp_arg*>
    > = dango::enable_val
  >
  constexpr explicit
  auto_ptr
  (tp_arg* const a_ptr, dango::usize const a_size, dango::usize const a_align)noexcept:
  super_type{ a_size, a_align },
  m_ptr{ a_ptr }
  {
    dango_assert(dango::is_pow_two(a_align));
    dango_assert(a_align >= config::alignment);
  }

  template
  <typename tp_tp_config = config, dango::enable_if<tp_tp_config::move_constructor> = dango::enable_val>
  constexpr
  auto_ptr
  (auto_ptr&& a_ptr)noexcept:
  super_type{ static_cast<super_type&&>(a_ptr) },
  m_ptr{ a_ptr.m_ptr }
  {
    a_ptr.m_ptr = nullptr;
  }

  template
  <typename tp_tp_config = config, dango::enable_if<!tp_tp_config::move_constructor> = dango::enable_val>
  constexpr auto_ptr(auto_ptr&& a_ptr)noexcept = delete;

  template
  <
    typename tp_arg,
    typename tp_arg_config,
    dango::enable_if
    <
      (!dango::is_same<tp_arg, tp_type> || !dango::is_same<tp_arg_config, config>) &&
      config::move_constructor &&
      dango::is_same<typename tp_arg_config::destroyer, typename config::destroyer> &&
      dango::is_gequal(tp_arg_config::alignment, config::alignment) &&
      dango::logical_equivalence(tp_arg_config::store_size, config::store_size) &&
      dango::logical_equivalence(tp_arg_config::store_align, config::store_align) &&
      dango::logical_equivalence(dango::is_void<tp_arg>, dango::is_void<tp_type>) &&
      dango::is_constructible<value_type, tp_arg*>
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
      dango::is_same<typename tp_arg_config::destroyer, typename config::destroyer> &&
      dango::is_gequal(tp_arg_config::alignment, config::alignment) &&
      dango::logical_equivalence(tp_arg_config::store_size, config::store_size) &&
      dango::logical_equivalence(tp_arg_config::store_align, config::store_align) &&
      dango::logical_equivalence(dango::is_void<tp_arg>, dango::is_void<tp_type>) &&
      dango::is_constructible<value_type, tp_arg*>
      > = dango::enable_val
  >
  constexpr
  auto_ptr
  (auto_ptr<tp_arg, tp_arg_config>&& a_ptr)noexcept:
  super_type{ static_cast<super_type&&>(a_ptr) },
  m_ptr{ a_ptr.m_ptr }
  {
    a_ptr.m_ptr = nullptr;
  }

  ~auto_ptr
  ()noexcept
  {
    if(!m_ptr)
    {
      return;
    }

    if constexpr(config::store_align)
    {
      if constexpr(config::destroy_size && config::destroy_align)
      {
        config::destroyer::destroy(m_ptr, super_type::get_size(), super_type::get_align());
      }

      if constexpr(!config::destroy_size && config::destroy_align)
      {
        config::destroyer::destroy(m_ptr, super_type::get_align());
      }
    }
    else
    {
      if constexpr(config::destroy_size && config::destroy_align)
      {
        config::destroyer::destroy(m_ptr, super_type::get_size(), config::alignment);
      }

      if constexpr(!config::destroy_size && config::destroy_align)
      {
        config::destroyer::destroy(m_ptr, config::alignment);
      }
    }

    if constexpr(config::destroy_size && !config::destroy_align)
    {
      config::destroyer::destroy(m_ptr, super_type::get_size());
    }

    if constexpr(!config::destroy_size && !config::destroy_align)
    {
      config::destroyer::destroy(m_ptr);
    }
  }

  auto
  operator =
  (dango::null_tag const)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::null };

    if constexpr(config::store_size)
    {
      dango::swap(super_type::get_size(), a_temp.super_type::get_size());
    }

    if constexpr(config::store_align)
    {
      dango::swap(super_type::get_align(), a_temp.super_type::get_align());
    }

    dango::swap(m_ptr, a_temp.m_ptr);

    return *this;
  }

  template
  <typename tp_tp_config = config, dango::enable_if<tp_tp_config::move_assignment> = dango::enable_val>
  auto
  operator =
  (auto_ptr&& a_ptr)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    if constexpr(config::store_size)
    {
      dango::swap(super_type::get_size(), a_temp.super_type::get_size());
    }

    if constexpr(config::store_align)
    {
      dango::swap(super_type::get_align(), a_temp.super_type::get_align());
    }

    dango::swap(m_ptr, a_temp.m_ptr);

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
      dango::is_same<typename tp_arg_config::destroyer, typename config::destroyer> &&
      dango::is_gequal(tp_arg_config::alignment, config::alignment) &&
      dango::logical_equivalence(tp_arg_config::store_size, config::store_size) &&
      dango::logical_equivalence(tp_arg_config::store_align, config::store_align) &&
      dango::logical_equivalence(dango::is_void<tp_arg>, dango::is_void<tp_type>) &&
      dango::is_constructible<value_type, tp_arg*>
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
      dango::is_same<typename tp_arg_config::destroyer, typename config::destroyer> &&
      dango::is_gequal(tp_arg_config::alignment, config::alignment) &&
      dango::logical_equivalence(tp_arg_config::store_size, config::store_size) &&
      dango::logical_equivalence(tp_arg_config::store_align, config::store_align) &&
      dango::logical_equivalence(dango::is_void<tp_arg>, dango::is_void<tp_type>) &&
      dango::is_constructible<value_type, tp_arg*>
    > = dango::enable_val
  >
  auto
  operator =
  (auto_ptr<tp_arg, tp_arg_config>&& a_ptr)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::move(a_ptr) };

    if constexpr(config::store_size)
    {
      dango::swap(super_type::get_size(), a_temp.super_type::get_size());
    }

    if constexpr(config::store_align)
    {
      dango::swap(super_type::get_align(), a_temp.super_type::get_align());
    }

    dango::swap(m_ptr, a_temp.m_ptr);

    return *this;
  }

  constexpr auto dango_operator_is_null()const noexcept->bool{ return m_ptr == nullptr; }

  template
  <typename tp_tp_config = config>
  constexpr auto get()const noexcept->dango::enable_if<!tp_tp_config::deep_const, value_type>{ return dango::assume_aligned<config::alignment>(m_ptr); }
  template
  <typename tp_tp_config = config>
  constexpr auto get()noexcept->dango::enable_if<tp_tp_config::deep_const, value_type>{ return dango::assume_aligned<config::alignment>(m_ptr); }
  template
  <typename tp_tp_config = config>
  constexpr auto get()const noexcept->dango::enable_if<tp_tp_config::deep_const, const_type>{ return dango::assume_aligned<config::alignment>(m_ptr); }

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
  constexpr auto size()const noexcept->dango::usize{ return super_type::get_size(); }

  template
  <typename tp_tp_config = config, dango::enable_if<tp_tp_config::store_align> = dango::enable_val>
  constexpr auto align()const noexcept->dango::usize{ return super_type::get_align(); }
  template
  <typename tp_tp_config = config, dango::enable_if<!tp_tp_config::store_align> = dango::enable_val>
  constexpr auto align()const noexcept->dango::usize{ return config::alignment; }

  constexpr auto
  dismiss
  ()noexcept->value_type
  {
    auto const a_ptr = get();

    m_ptr = nullptr;

    if constexpr(config::store_size)
    {
      super_type::get_size() = dango::usize(0);
    }

    if constexpr(config::store_align)
    {
      super_type::get_align() = dango::usize(0);
    }

    return a_ptr;
  }

  constexpr explicit operator bool()const noexcept{ return m_ptr != nullptr; }

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
  value_type m_ptr;
public:
  DANGO_DELETE_DEFAULT(auto_ptr)
  DANGO_UNCOPYABLE(auto_ptr)
};

/*** auto_new ***/

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
  auto_new(dango::usize const a_size)dango_new_noexcept(true)->
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
  auto_new(dango::usize const a_size, dango::usize const a_align)dango_new_noexcept(true)->
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

/*** mem_copy ***/

namespace
dango
{
  constexpr auto mem_copy(void*, void const*, dango::usize)noexcept->void*;
}

constexpr auto
dango::
mem_copy
(
  void* const a_dest,
  void const* const a_source,
  dango::usize const a_count
)
noexcept->void*
{
  dango_assert(a_dest != nullptr);
  dango_assert(a_source != nullptr);

  return __builtin_memcpy(a_dest, a_source, a_count);
}

/*** aligned_storage ***/

namespace
dango::detail
{
  template
  <dango::usize tp_size, dango::usize tp_align>
  using aligned_storage_enable_spec =
    dango::enable_if<!dango::is_equal(tp_size, dango::usize(0)) && dango::is_pow_two(tp_align)>;
}

namespace
dango
{
  template
  <
    dango::usize tp_size,
    dango::usize tp_align = alignof(dango::max_align_type),
    typename tp_enabled = dango::enable_tag
  >
  class aligned_storage;

  template
  <dango::usize tp_size, dango::usize tp_align>
  class
  aligned_storage
  <tp_size, tp_align, dango::detail::aligned_storage_enable_spec<tp_size, tp_align>>;
}

template
<dango::usize tp_size, dango::usize tp_align>
class
dango::
aligned_storage
<tp_size, tp_align, dango::detail::aligned_storage_enable_spec<tp_size, tp_align>>
{
public:
  static constexpr dango::usize const c_size = dango::next_multiple(tp_size, tp_align);
  static constexpr dango::usize const c_align = tp_align;
public:
  constexpr aligned_storage()noexcept = default;

  ~aligned_storage()noexcept = default;

  constexpr auto get()noexcept->void*;
  constexpr auto get()const noexcept->void const*;
private:
  alignas(c_align) dango::byte m_storage[c_size];
public:
  DANGO_IMMOBILE(aligned_storage)
};

template
<dango::usize tp_size, dango::usize tp_align>
constexpr auto
dango::
aligned_storage
<tp_size, tp_align, dango::detail::aligned_storage_enable_spec<tp_size, tp_align>>::
get
()noexcept->void*
{
  return m_storage;
}

template
<dango::usize tp_size, dango::usize tp_align>
constexpr auto
dango::
aligned_storage
<tp_size, tp_align, dango::detail::aligned_storage_enable_spec<tp_size, tp_align>>::
get
()const noexcept->void const*
{
  return m_storage;
}

/*** array_destroy array_copy array_move array_relocate array_shift ***/

namespace
dango
{
  template
  <typename tp_type>
  constexpr auto
  array_destroy
  (tp_type const volatile*, dango::usize)noexcept->
  dango::enable_if<dango::is_noexcept_destructible<tp_type>, void>;

  template
  <typename tp_type>
  constexpr auto
  array_copy
  (void* dango_restrict, tp_type const*, dango::usize)
  noexcept(dango::is_noexcept_copy_constructible<tp_type>)->
  dango::enable_if
  <
    !dango::is_volatile<tp_type> &&
    dango::is_copy_constructible<tp_type> &&
    dango::is_noexcept_destructible<tp_type>,
    tp_type*
  >;

  template
  <typename tp_type>
  constexpr auto
  array_move
  (void* dango_restrict, tp_type*, dango::usize)
  noexcept->
  dango::enable_if
  <
    !dango::is_const<tp_type> &&
    !dango::is_volatile<tp_type> &&
    dango::is_noexcept_move_constructible<tp_type>,
    tp_type*
  >;

  template
  <typename tp_type>
  constexpr auto
  array_relocate
  (void* dango_restrict, tp_type*, dango::usize)
  noexcept->
  dango::enable_if
  <
    !dango::is_const<tp_type> &&
    !dango::is_volatile<tp_type> &&
    dango::is_noexcept_move_constructible<tp_type> &&
    dango::is_noexcept_destructible<tp_type>,
    tp_type*
  >;

  template
  <typename tp_type>
  constexpr auto
  array_shift
  (dango::ssize, tp_type*, dango::usize)
  noexcept->
  dango::enable_if
  <
    !dango::is_const<tp_type> &&
    !dango::is_volatile<tp_type> &&
    dango::is_noexcept_move_constructible<tp_type> &&
    dango::is_noexcept_destructible<tp_type>,
    tp_type*
  >;
}

template
<typename tp_type>
constexpr auto
dango::
array_destroy
(tp_type const volatile* const a_array, dango::usize const a_count)noexcept->
dango::enable_if<dango::is_noexcept_destructible<tp_type>, void>
{
  dango_assert(a_array != nullptr);

  if(a_count == dango::usize(0))
  {
    return;
  }

  auto const a_end = a_array + a_count;
  auto a_cur = a_array;

  while(a_cur != a_end)
  {
    dango::destructor(a_cur++);
  }
}

template
<typename tp_type>
constexpr auto
dango::
array_copy
(void* const dango_restrict a_dest, tp_type const* const a_array, dango::usize const a_count)
noexcept(dango::is_noexcept_copy_constructible<tp_type>)->
dango::enable_if
<
  !dango::is_volatile<tp_type> &&
  dango::is_copy_constructible<tp_type> &&
  dango::is_noexcept_destructible<tp_type>,
  tp_type*
>
{
  dango_assert(a_dest != nullptr);
  dango_assert(a_array != nullptr);

  if(a_count == dango::usize(0))
  {
    return nullptr;
  }

  auto const a_end = a_array + a_count;
  auto a_src = a_array;
  auto const a_result = dango::assume_aligned<alignof(tp_type)>(static_cast<tp_type*>(a_dest));
  auto a_dst = a_result;

  auto a_guard =
  dango::make_guard
  (
    [a_result, &a_dst]()noexcept->void
    {
      dango::array_destroy(a_result, dango::usize(a_dst - a_result));
    }
  );

  do
  {
    ::new (dango::placement, a_dst) tp_type{ *a_src };

    dango::destructor(a_src);

    ++a_src;
    ++a_dst;
  }
  while(a_src != a_end);

  a_guard.dismiss();

  return a_result;
}

template
<typename tp_type>
constexpr auto
dango::
array_move
(void* const dango_restrict a_dest, tp_type* const a_array, dango::usize const a_count)
noexcept->
dango::enable_if
<
  !dango::is_const<tp_type> &&
  !dango::is_volatile<tp_type> &&
  dango::is_noexcept_move_constructible<tp_type>,
  tp_type*
>
{
  dango_assert(a_dest != nullptr);
  dango_assert(a_array != nullptr);

  if(a_count == dango::usize(0))
  {
    return nullptr;
  }

  tp_type const* const a_end = a_array + a_count;
  auto a_src = a_array;
  auto const a_result = dango::assume_aligned<alignof(tp_type)>(static_cast<tp_type*>(a_dest));
  auto a_dst = a_result;

  do
  {
    ::new (dango::placement, a_dst) tp_type{ dango::move(*a_src) };

    ++a_src;
    ++a_dst;
  }
  while(a_src != a_end);

  return a_result;
}

template
<typename tp_type>
constexpr auto
dango::
array_relocate
(void* const dango_restrict a_dest, tp_type* const a_array, dango::usize const a_count)
noexcept->
dango::enable_if
<
  !dango::is_const<tp_type> &&
  !dango::is_volatile<tp_type> &&
  dango::is_noexcept_move_constructible<tp_type> &&
  dango::is_noexcept_destructible<tp_type>,
  tp_type*
>
{
  dango_assert(a_dest != nullptr);
  dango_assert(a_array != nullptr);

  if(a_count == dango::usize(0))
  {
    return nullptr;
  }

  tp_type const* const a_end = a_array + a_count;
  auto a_src = a_array;
  auto const a_result = dango::assume_aligned<alignof(tp_type)>(static_cast<tp_type*>(a_dest));
  auto a_dst = a_result;

  do
  {
    ::new (dango::placement, a_dst) tp_type{ dango::move(*a_src) };

    dango::destructor(a_src);

    ++a_src;
    ++a_dst;
  }
  while(a_src != a_end);

  return a_result;
}

template
<typename tp_type>
constexpr auto
dango::
array_shift
(dango::ssize const a_shift, tp_type* const a_array, dango::usize const a_count)
noexcept->
dango::enable_if
<
  !dango::is_const<tp_type> &&
  !dango::is_volatile<tp_type> &&
  dango::is_noexcept_move_constructible<tp_type> &&
  dango::is_noexcept_destructible<tp_type>,
  tp_type*
>
{
  dango_assert(a_array != nullptr);

  if(a_count == dango::usize(0))
  {
    return nullptr;
  }

  auto const a_result = a_array + a_shift;

  if(a_shift < dango::ssize(0))
  {
    tp_type const* const a_end = a_array + a_count;
    auto a_src = a_array;
    auto a_dst = a_result;

    do
    {
      ::new (dango::placement, a_dst) tp_type{ dango::move(*a_src) };

      dango::destructor(a_src);

      ++a_src;
      ++a_dst;
    }
    while(a_src != a_end);
  }

  if(a_shift > dango::ssize(0))
  {
    tp_type const* const a_end = a_array;
    auto a_src = a_array + a_count;
    auto a_dst = a_result + a_count;

    do
    {
      --a_src;
      --a_dst;

      ::new (dango::placement, a_dst) tp_type{ dango::move(*a_src) };

      dango::destructor(a_src);
    }
    while(a_src != a_end);
  }

  return a_result;
}



#endif

