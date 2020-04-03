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
      { tp_config::conversion_requires_same_destroyer }noexcept->dango::is_same<bool const>;
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
    static inline constexpr bool const conversion_requires_same_destroyer = false;

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
    static inline constexpr bool const conversion_requires_same_destroyer = true;

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



#endif // DANGO_AUTO_PTR_HPP_INCLUDED

