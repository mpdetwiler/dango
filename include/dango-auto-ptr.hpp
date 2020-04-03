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
  <typename tp_config, typename tp_type>
  concept is_auto_ptr_config =
    dango::is_class<tp_config> && !dango::is_const<tp_config> && !dango::is_volatile<tp_config> &&
    dango::auto_ptr_constraint_spec<tp_type> && !dango::is_const<tp_type> && !dango::is_volatile<tp_type> &&
    !dango::is_const<tp_type> && !dango::is_volatile<tp_type> &&
    requires
    {
      typename tp_config::deleter;
      requires(dango::is_auto_ptr_deleter<typename tp_config::deleter, tp_type>);
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
    struct
    deleter
    {
      static void del(tp_type const volatile* const a_ptr)noexcept{ delete a_ptr; }

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
    struct
    deleter
    {
      static void
      del(tp_type const volatile* const a_ptr, dango::usize const a_size, dango::usize const a_align)noexcept
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
  <typename tp_type>
  using auto_ptr_config_default_argument =
    dango::auto_ptr_default_config<dango::remove_cv<tp_type>>;
}

/*** auto_ptr ***/

namespace
dango
{
  template
  <dango::auto_ptr_constraint_spec tp_type, typename tp_config = auto_ptr_config_default_argument<tp_type>>
  requires(dango::is_auto_ptr_config<tp_config, dango::remove_cv<tp_type>>)
  class auto_ptr;

  template
  <dango::is_void tp_type, typename tp_config>
  requires(dango::is_auto_ptr_config<tp_config, dango::remove_cv<tp_type>>)
  class auto_ptr<tp_type, tp_config>;
}

template
<dango::auto_ptr_constraint_spec tp_type, typename tp_config>
requires(dango::is_auto_ptr_config<tp_config, dango::remove_cv<tp_type>>)
class
dango::
auto_ptr
final
{
public:
  using value_type = tp_type*;
  using config = tp_config;
  using deleter = typename config::deleter;
public:
  constexpr auto_ptr()noexcept = delete;
  constexpr auto_ptr(auto_ptr const&)noexcept = delete;
  constexpr auto_ptr(auto_ptr&&)noexcept = delete;
  constexpr auto operator = (auto_ptr const&)& noexcept = delete;
  constexpr auto operator = (auto_ptr&&)& noexcept = delete;

  ~auto_ptr()noexcept{ if(m_ptr){ deleter::del(m_ptr); } }

  constexpr auto_ptr(dango::null_tag const)noexcept:m_ptr{ dango::null }{ }

  constexpr explicit(!config::enable_implicit_conversion)
  auto_ptr(value_type const a_ptr)noexcept:m_ptr{ a_ptr }{ }

  constexpr auto_ptr(auto_ptr&& a_ptr)noexcept
  requires(config::enable_move_construct):m_ptr{ a_ptr.m_ptr }{ a_ptr.m_ptr = dango::null; }

  template
  <typename tp_arg, typename tp_arg_config>
  requires
  (
    config::enable_move_construct &&
    !(dango::is_same<tp_arg, tp_type> && dango::is_same<tp_arg_config, config>) &&
    dango::is_brace_constructible<value_type, tp_arg* const&> &&
    dango::logical_implication(config::conversion_requires_same_deleter, dango::is_same<typename tp_arg_config::deleter, deleter>) &&
    (
      dango::is_same_ignore_cv<tp_arg, tp_type> ||
      dango::logical_implication
      (
        config::differing_deleter_requires_virtual_destruct,
        dango::is_derived_from<tp_arg, tp_type> &&
        dango::is_virtual_destructible<tp_type>
      )
    )
  )
  constexpr explicit(!config::enable_implicit_conversion)
  auto_ptr(auto_ptr<tp_arg, tp_arg_config>&& a_ptr)noexcept:m_ptr{ a_ptr.m_ptr }{ a_ptr.m_ptr = dango::null; }

  constexpr auto operator = (dango::null_tag const)& noexcept->auto_ptr&
  { auto_ptr a_temp{ dango::null }; dango::swap(m_ptr, a_temp.m_ptr); return *this; }

  constexpr auto operator = (auto_ptr&& a_ptr)& noexcept->auto_ptr&
  requires(config::enable_move_assign)
  { auto_ptr a_temp{ dango::move(a_ptr) }; dango::swap(m_ptr, a_temp.m_ptr); return *this; }

  template
  <typename tp_arg, typename tp_arg_config>
  requires
  (
    config::enable_move_assign &&
    !(dango::is_same<tp_arg, tp_type> && dango::is_same<tp_arg_config, config>) &&
    dango::is_assignable<value_type&, tp_arg* const&> &&
    dango::is_brace_constructible<auto_ptr, dango::auto_ptr<tp_arg, tp_arg_config>&&>
  )
  constexpr auto operator = (auto_ptr<tp_arg, tp_arg_config>&& a_ptr)& noexcept->auto_ptr&
  { auto_ptr a_temp{ dango::move(a_ptr) }; dango::swap(m_ptr, a_temp.m_ptr); return *this; }

private:
  value_type m_ptr;
};

template
<dango::is_void tp_type, typename tp_config>
requires(dango::is_auto_ptr_config<tp_config, dango::remove_cv<tp_type>>)
class
dango::
auto_ptr<tp_type, tp_config>
final
{

};



#endif // DANGO_AUTO_PTR_HPP_INCLUDED

