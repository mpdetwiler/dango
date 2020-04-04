#ifndef DANGO_AUTO_PTR_HPP_INCLUDED
#define DANGO_AUTO_PTR_HPP_INCLUDED

namespace
dango
{
  template
  <typename tp_type>
  concept auto_ptr_constraint_spec =
    dango::is_void<tp_type> || dango::is_object_exclude_array<tp_type>;
}

namespace
dango::detail
{
  template
  <typename tp_deleter, typename tp_type>
  concept is_auto_ptr_deleter_object_help =
    dango::is_object_exclude_array<tp_type> &&
    requires(tp_type const volatile* const& a_ptr)
    {
      { tp_deleter::del(a_ptr) }noexcept->dango::is_same<void>;
    };

  template
  <typename tp_deleter, typename tp_type>
  concept is_auto_ptr_deleter_void_help =
    dango::is_void<tp_type> &&
    requires(tp_type const volatile* const& a_ptr, dango::usize const& a_size, dango::usize const& a_align)
    {
      { tp_deleter::del(a_ptr, a_size, a_align) }noexcept->dango::is_same<void>;
    };
}

namespace
dango
{
  template
  <typename tp_deleter, typename tp_type>
  concept is_auto_ptr_deleter =
    dango::is_class<tp_deleter> && !dango::is_const<tp_deleter> && !dango::is_volatile<tp_deleter> &&
    dango::auto_ptr_constraint_spec<tp_type> && !dango::is_const<tp_type> && !dango::is_volatile<tp_type> &&
    (dango::detail::is_auto_ptr_deleter_void_help<tp_deleter, tp_type> || dango::detail::is_auto_ptr_deleter_object_help<tp_deleter, tp_type>);
}

namespace
dango
{
  template
  <typename tp_config>
  concept is_auto_ptr_config =
    dango::is_class<tp_config> && !dango::is_const<tp_config> && !dango::is_volatile<tp_config> &&
    requires
    {
      typename tp_config::type;
      typename tp_config::deleter;
      requires(dango::auto_ptr_constraint_spec<typename tp_config::type>);
      requires(!dango::is_const<typename tp_config::type> && !dango::is_volatile<typename tp_config::type>);
      requires(dango::is_auto_ptr_deleter<typename tp_config::deleter, typename tp_config::type>);
      { tp_config::enable_deep_const }noexcept->dango::is_same<bool const>;
      { tp_config::enable_move_construct }noexcept->dango::is_same<bool const>;
      { tp_config::enable_move_assign }noexcept->dango::is_same<bool const>;
      { tp_config::enable_implicit_conversion }noexcept->dango::is_same<bool const>;
      { tp_config::conversion_requires_same_deleter }noexcept->dango::is_same<bool const>;
      { tp_config::differing_deleter_requires_virtual_destruct }noexcept->dango::is_same<bool const>;
      requires(dango::logical_implication(tp_config::enable_move_assign, tp_config::enable_move_construct));
    };
}

namespace
dango
{
  template
  <typename tp_type>
  struct auto_ptr_default_config;

  template
  <dango::is_object_exclude_array tp_type>
  requires(!dango::is_const<tp_type> && !dango::is_volatile<tp_type>)
  struct
  auto_ptr_default_config<tp_type>
  {
    using type = tp_type;

    struct
    deleter
    {
      static void del(type const volatile* const a_ptr)noexcept{ delete a_ptr; }

      DANGO_UNINSTANTIABLE(deleter)
    };

    static inline constexpr bool const enable_deep_const = false;
    static inline constexpr bool const enable_move_construct = true;
    static inline constexpr bool const enable_move_assign = true;
    static inline constexpr bool const enable_implicit_conversion = true;
    static inline constexpr bool const conversion_requires_same_deleter = false;
    static inline constexpr bool const differing_deleter_requires_virtual_destruct = true;

    DANGO_UNINSTANTIABLE(auto_ptr_default_config)
  };

  template
  <dango::is_void tp_type>
  requires(!dango::is_const<tp_type> && !dango::is_volatile<tp_type>)
  struct
  auto_ptr_default_config<tp_type>
  {
    using type = tp_type;

    struct
    deleter
    {
      static void
      del(type const volatile* const a_ptr, dango::usize const a_size, dango::usize const a_align)noexcept
      {
        dango::operator_delete(a_ptr, a_size, a_align);
      }

      DANGO_UNINSTANTIABLE(deleter)
    };

    static inline constexpr bool const enable_deep_const = false;
    static inline constexpr bool const enable_move_construct = true;
    static inline constexpr bool const enable_move_assign = true;
    static inline constexpr bool const enable_implicit_conversion = true;
    static inline constexpr bool const conversion_requires_same_deleter = true;
    static inline constexpr bool const differing_deleter_requires_virtual_destruct = false;

    DANGO_UNINSTANTIABLE(auto_ptr_default_config)
  };

  template
  <template<typename> typename tp_config, typename tp_type>
  using auto_ptr_config_argument = tp_config<dango::remove_cv<tp_type>>;

  template
  <typename tp_type>
  using auto_ptr_config_default_argument =
    dango::auto_ptr_config_argument<dango::auto_ptr_default_config, tp_type>;
}

static_assert(dango::is_auto_ptr_config<dango::auto_ptr_config_default_argument<void const>>);
static_assert(dango::is_auto_ptr_config<dango::auto_ptr_config_default_argument<bool const>>);

/*** auto_ptr ***/

namespace
dango
{
  template
  <dango::auto_ptr_constraint_spec tp_type, dango::is_auto_ptr_config tp_config = auto_ptr_config_default_argument<tp_type>>
  class auto_ptr;

  template
  <dango::is_void tp_type, dango::is_auto_ptr_config tp_config>
  class auto_ptr<tp_type, tp_config>;
}

template
<dango::auto_ptr_constraint_spec tp_type, dango::is_auto_ptr_config tp_config>
class
dango::
auto_ptr
final
{
public:
  using value_type = tp_type*;
  using value_const_type = tp_type const*;
  using ref_type = tp_type&;
  using ref_const_type = tp_type const&;
  using config = tp_config;
  using deleter = typename config::deleter;
private:
  template
  <typename tp_arg, typename tp_arg_config>
  static constexpr auto
  conversion_help()noexcept->bool
  {
    if(dango::is_same<typename tp_arg_config::type, typename config::type>){ return true; }
    if(dango::is_same<typename tp_arg_config::deleter, deleter>){ return true; }
    if(config::conversion_requires_same_deleter){ return false; }
    if(!config::differing_deleter_requires_virtual_destruct){ return true; }
    return dango::is_derived_from<tp_arg, tp_type> && dango::is_virtual_destructible<tp_type>;
  }
public:
  constexpr auto_ptr()noexcept = delete;
  constexpr auto_ptr(auto_ptr const&)noexcept = delete;
  constexpr auto_ptr(auto_ptr&&)noexcept = delete;
  constexpr auto operator = (auto_ptr const&)& noexcept = delete;
  constexpr auto operator = (auto_ptr&&)& noexcept = delete;

  ~auto_ptr()noexcept
  {
    auto const a_ptr = get();

    if(a_ptr)
    {
      deleter::del(a_ptr);
    }
  }

  constexpr auto_ptr(dango::null_tag const)noexcept:m_ptr{ dango::null }{ }

  constexpr explicit(!config::enable_implicit_conversion)
  auto_ptr(value_type const a_ptr)noexcept:m_ptr{ a_ptr }{ }

  constexpr auto_ptr(auto_ptr&& a_ptr)noexcept
  requires(config::enable_move_construct):m_ptr{ a_ptr.release() }{ }

  template
  <typename tp_arg, typename tp_arg_config>
  requires
  (
    config::enable_move_construct &&
    !(dango::is_same<tp_arg, tp_type> && dango::is_same<tp_arg_config, config>) &&
    dango::is_brace_constructible<value_type, tp_arg* const&> &&
    conversion_help<tp_arg, tp_arg_config>()
  )
  constexpr explicit(!config::enable_implicit_conversion)
  auto_ptr(dango::auto_ptr<tp_arg, tp_arg_config>&& a_ptr)noexcept:m_ptr{ a_ptr.release() }{ }

  constexpr auto operator = (dango::null_tag const)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::null };
    dango::swap(m_ptr, a_temp.m_ptr);
    return *this;
  }

  constexpr auto operator = (auto_ptr&& a_ptr)& noexcept->auto_ptr&
  requires(config::enable_move_assign)
  {
    auto_ptr a_temp{ dango::move(a_ptr) };
    dango::swap(m_ptr, a_temp.m_ptr);
    return *this;
  }

  template
  <typename tp_arg, typename tp_arg_config>
  requires
  (
    config::enable_move_assign &&
    !(dango::is_same<tp_arg, tp_type> && dango::is_same<tp_arg_config, config>) &&
    dango::is_assignable<value_type&, tp_arg* const&> &&
    dango::is_brace_constructible<auto_ptr, dango::auto_ptr<tp_arg, tp_arg_config>&&>
  )
  constexpr auto operator = (dango::auto_ptr<tp_arg, tp_arg_config>&& a_ptr)& noexcept->auto_ptr&
  {
    auto_ptr a_temp{ dango::move(a_ptr) };
    dango::swap(m_ptr, a_temp.m_ptr);
    return *this;
  }

  constexpr auto
  get()noexcept->value_type
  requires(config::enable_deep_const){ return m_ptr; }
  constexpr auto
  get()const noexcept->value_const_type
  requires(config::enable_deep_const){ return m_ptr; }
  constexpr auto
  get()const noexcept->value_type
  requires(!config::enable_deep_const){ return m_ptr; }

  constexpr auto
  release()noexcept->value_type
  {
    auto const a_ptr = get();
    m_ptr = dango::null;
    return a_ptr;
  }

  constexpr auto
  dango_operator_is_null()const noexcept->bool{ return get() == dango::null; }

  constexpr auto
  dango_operator_equals(dango::null_tag)const noexcept = delete;

  template
  <typename tp_arg>
  requires(dango::is_equatable<value_type, tp_arg*>)
  constexpr auto
  dango_operator_equals(tp_arg* const a_ptr)const noexcept->bool
  {
    return get() == a_ptr;
  }

  template
  <typename tp_arg, typename tp_arg_config>
  requires(dango::is_equatable<value_type, tp_arg*>)
  constexpr auto
  dango_operator_equals(dango::auto_ptr<tp_arg, tp_arg_config> const& a_ptr)const noexcept->bool
  {
    return get() == a_ptr.get();
  }

  constexpr auto
  operator -> ()noexcept->value_type
  requires(config::enable_deep_const){ return get(); }
  constexpr auto
  operator -> ()const noexcept->value_const_type
  requires(config::enable_deep_const){ return get(); }
  constexpr auto
  operator -> ()const noexcept->value_type
  requires(!config::enable_deep_const){ return get(); }

  constexpr auto
  operator * ()noexcept->ref_type
  requires(config::enable_deep_const){ return *get(); }
  constexpr auto
  operator * ()const noexcept->ref_const_type
  requires(config::enable_deep_const){ return *get(); }
  constexpr auto
  operator * ()const noexcept->ref_type
  requires(!config::enable_deep_const){ return *get(); }

  explicit operator bool()const noexcept{ return !dango_operator_is_null(); }
private:
  value_type m_ptr;
};

static_assert(dango::is_nullable<dango::auto_ptr<double>>);
static_assert(dango::is_nullable<dango::auto_ptr<double const>>);
static_assert(dango::is_equatable<dango::auto_ptr<double>, double const*>);
static_assert(dango::is_equatable<dango::auto_ptr<double const>, double*>);
static_assert(dango::is_equatable<dango::auto_ptr<double>, dango::auto_ptr<double const>>);
static_assert(dango::is_equatable<dango::auto_ptr<double const>, dango::auto_ptr<double>>);

template
<dango::is_void tp_type, dango::is_auto_ptr_config tp_config>
class
dango::
auto_ptr<tp_type, tp_config>
final
{

};



#endif // DANGO_AUTO_PTR_HPP_INCLUDED

