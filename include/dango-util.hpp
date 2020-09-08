#ifndef DANGO_UTIL_HPP_INCLUDED
#define DANGO_UTIL_HPP_INCLUDED

#include "dango-util-base.hpp"
#include "dango-assert.hpp"

/*** make_guard ***/

namespace
dango::detail
{
  template
  <typename tp_func>
  class scope_guard;
}

template
<typename tp_func>
class
dango::
detail::
scope_guard
{
private:
  using func_type = dango::decay<tp_func>;
  static_assert(dango::is_same<func_type, tp_func>);
public:
  template
  <typename tp_func_type>
  requires(dango::is_brace_constructible<func_type, tp_func_type>)
  explicit constexpr
  scope_guard
  (tp_func_type&& a_func)noexcept(dango::is_noexcept_brace_constructible<func_type, tp_func_type>):
  m_func{ dango::forward<tp_func_type>(a_func) },
  m_active{ true }
  {

  }

  constexpr ~scope_guard()noexcept;

  constexpr void dismiss()noexcept;
private:
  func_type m_func;
  bool m_active;
public:
  DANGO_DELETE_DEFAULT(scope_guard)
  DANGO_UNMOVEABLE(scope_guard)
};

template
<typename tp_func>
constexpr
dango::
detail::
scope_guard<tp_func>::
~scope_guard
()noexcept
{
  static_assert(noexcept(m_func()));
  static_assert(dango::is_noexcept_destructible<func_type>);

  if(m_active)
  {
    m_func();
  }
}

template
<typename tp_func>
constexpr void
dango::
detail::
scope_guard<tp_func>::
dismiss
()noexcept
{
  m_active = false;
}

namespace
dango
{
  template
  <typename tp_func>
  requires
  (
    dango::is_brace_constructible<dango::detail::scope_guard<dango::decay<tp_func>>, tp_func> &&
    dango::is_noexcept_callable_ret<void, dango::decay<tp_func>&> &&
    dango::is_noexcept_destructible<dango::decay<tp_func>>
  )
  [[nodiscard]] auto
  make_guard
  (tp_func&& a_func)
  noexcept(dango::is_noexcept_brace_constructible<dango::detail::scope_guard<dango::decay<tp_func>>, tp_func>)->auto
  {
    using return_type = dango::detail::scope_guard<dango::decay<tp_func>>;

    return return_type{ dango::forward<tp_func>(a_func) };
  }
}

/*** make_finally ***/

namespace
dango::detail
{
  template
  <typename tp_func>
  class finally_guard;
}

template
<typename tp_func>
class
dango::
detail::
finally_guard
{
private:
  using func_type = dango::decay<tp_func>;
  static_assert(dango::is_same<func_type, tp_func>);
public:
  template
  <typename tp_func_type>
  requires(dango::is_brace_constructible<func_type, tp_func_type>)
  explicit constexpr
  finally_guard
  (tp_func_type&& a_func)noexcept(dango::is_noexcept_brace_constructible<func_type, tp_func_type>):
  m_func{ dango::forward<tp_func_type>(a_func) }
  {

  }

  constexpr ~finally_guard()noexcept;
private:
  func_type m_func;
public:
  DANGO_DELETE_DEFAULT(finally_guard)
  DANGO_UNMOVEABLE(finally_guard)
};

template
<typename tp_func>
constexpr
dango::
detail::
finally_guard<tp_func>::
~finally_guard
()noexcept
{
  static_assert(noexcept(m_func()));
  static_assert(dango::is_noexcept_destructible<func_type>);

  m_func();
}

namespace
dango
{
  template
  <typename tp_func>
  requires
  (
    dango::is_brace_constructible<dango::detail::finally_guard<dango::decay<tp_func>>, tp_func> &&
    dango::is_noexcept_callable_ret<void, dango::decay<tp_func>&> &&
    dango::is_noexcept_destructible<dango::decay<tp_func>>
  )
  [[nodiscard]] auto
  make_finally
  (tp_func&& a_func)
  noexcept(dango::is_noexcept_brace_constructible<dango::detail::finally_guard<dango::decay<tp_func>>, tp_func>)->auto
  {
    using return_type = dango::detail::finally_guard<dango::decay<tp_func>>;

    return return_type{ dango::forward<tp_func>(a_func) };
  }
}

/*** member_func ***/

namespace
dango
{
  template
  <dango::is_member_func_ptr tp_type>
  requires(!dango::is_const_or_volatile<tp_type>)
  class member_function;

  template
  <dango::is_member_ptr tp_arg>
  member_function(tp_arg)->member_function<tp_arg>;

  template
  <dango::is_member_func_ptr tp_arg>
  constexpr auto
  make_member_func
  (tp_arg const a_arg)noexcept->dango::member_function<tp_arg>
  {
    return dango::member_function<tp_arg>{ a_arg };
  }
}

template
<dango::is_member_func_ptr tp_type>
requires(!dango::is_const_or_volatile<tp_type>)
class
dango::
member_function
final
{
private:
  using value_type = tp_type;
public:
  constexpr member_function(dango::null_tag)noexcept = delete;
  explicit constexpr member_function(value_type const a_method)noexcept:m_method{ a_method }{ dango_assert(m_method != dango::null); }
  constexpr member_function(member_function const&)noexcept = default;
  constexpr member_function(member_function&&)noexcept = default;
  constexpr ~member_function()noexcept = default;
  constexpr auto operator = (member_function const&)& noexcept->member_function& = default;
  constexpr auto operator = (member_function&&)& noexcept->member_function& = default;

  template
  <typename tp_class, typename... tp_args>
  requires requires{ { (dango::declval<tp_class>().*dango::declval<value_type const&>())(dango::declval<tp_args>()...) }; }
  constexpr auto
  operator ()
  (tp_class&& a_class, tp_args&&... a_args)const
  noexcept(requires{ { (dango::declval<tp_class>().*dango::declval<value_type const&>())(dango::declval<tp_args>()...) }noexcept; })->
  decltype(auto)
  {
    return (dango::forward<tp_class>(a_class).*m_method)(dango::forward<tp_args>(a_args)...);
  }

  template
  <typename tp_class, typename... tp_args>
  requires requires{ { (dango::declval<tp_class* const&>()->*dango::declval<value_type const&>())(dango::declval<tp_args>()...) }; }
  constexpr auto
  operator ()
  (tp_class* const a_class, tp_args&&... a_args)const
  noexcept(requires{ { (dango::declval<tp_class* const&>()->*dango::declval<value_type const&>())(dango::declval<tp_args>()...) }noexcept; })->
  decltype(auto)
  {
    return (a_class->*m_method)(dango::forward<tp_args>(a_args)...);
  }
private:
  value_type m_method;
public:
  DANGO_DELETE_DEFAULT(member_function)
};

/*** member_func_type ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  struct member_func_type_help;

  template
  <dango::is_member_func_ptr tp_type>
  struct
  member_func_type_help<tp_type>;
}

namespace
dango
{
  template
  <typename tp_type>
  using member_func_type =
    typename dango::detail::member_func_type_help<tp_type>::type;
}

template
<typename tp_type>
struct
dango::
detail::
member_func_type_help
final
{
  using type = tp_type;

  DANGO_UNCONSTRUCTIBLE(member_func_type_help)
};

template
<dango::is_member_func_ptr tp_type>
struct
dango::
detail::
member_func_type_help<tp_type>
final
{
  using type = dango::member_function<dango::remove_cv<tp_type>>;

  DANGO_UNCONSTRUCTIBLE(member_func_type_help)
};

/*** emplacer ***/

namespace
dango::detail
{
  template
  <dango::is_object_exclude_array tp_ret, dango::is_object_exclude_array tp_func>
  requires(!dango::is_const_or_volatile<tp_ret> && !dango::is_const_or_volatile<tp_func>)
  class emplacer;
}

template
<dango::is_object_exclude_array tp_ret, dango::is_object_exclude_array tp_func>
requires(!dango::is_const_or_volatile<tp_ret> && !dango::is_const_or_volatile<tp_func>)
class
dango::
detail::
emplacer
final
{
public:
  using return_type = tp_ret;
  using func_type = tp_func;
public:
  template
  <typename tp_arg>
  requires(dango::is_brace_constructible<func_type, tp_arg>)
  explicit constexpr
  emplacer
  (tp_arg&& a_arg)
  noexcept(dango::is_noexcept_brace_constructible<func_type, tp_arg>):
  m_func{ dango::forward<tp_arg>(a_arg) }
  { }
  constexpr ~emplacer()noexcept = default;
public:
  constexpr
  operator return_type
  ()const noexcept(dango::is_noexcept_callable_ret<return_type, func_type&>)
  requires(dango::is_callable_ret<return_type, func_type&>)
  {
    return m_func();
  }

  constexpr auto
  operator ()
  ()const noexcept(dango::is_noexcept_callable_ret<return_type, func_type&>)->return_type
  requires(dango::is_callable_ret<return_type, func_type&>)
  {
    return m_func();
  }
private:
  mutable func_type m_func;
public:
  DANGO_DELETE_DEFAULT(emplacer)
  DANGO_UNMOVEABLE(emplacer)
};

namespace
dango
{
  template
  <dango::is_object_exclude_array tp_ret, typename tp_func>
  requires
  (
    !dango::is_const_or_volatile<tp_ret> &&
    dango::is_noexcept_destructible<dango::decay<tp_func>> &&
    dango::is_callable_ret<tp_ret, dango::decay<tp_func>&> &&
    dango::is_brace_constructible<dango::detail::emplacer<tp_ret, dango::decay<tp_func>>, tp_func>
  )
  constexpr auto
  make_emplacer
  (tp_func&& a_func)
  noexcept(dango::is_noexcept_brace_constructible<dango::detail::emplacer<tp_ret, dango::decay<tp_func>>, tp_func>)->auto
  {
    using emplacer_type = dango::detail::emplacer<tp_ret, dango::decay<tp_func>>;

    return emplacer_type{ dango::forward<tp_func>(a_func) };
  }

  template
  <dango::is_object_exclude_array tp_ret, typename... tp_args>
  requires
  (
    !dango::is_const_or_volatile<tp_ret> &&
    ( ... && dango::is_lvalue_ref<tp_args>) &&
    dango::is_brace_constructible<tp_ret, tp_args...>
  )
  constexpr auto
  tie_as_emplacer
  (tp_args&&... a_args)noexcept->auto
  {
    using return_type = tp_ret;

    constexpr bool const c_noexcept =
      dango::is_noexcept_brace_constructible<return_type, tp_args...>;

    return
      dango::make_emplacer<return_type>
      (
        [&a_args...]()constexpr noexcept(c_noexcept)->return_type
        {
          return return_type{ a_args... };
        }
      );
  }

  template
  <dango::is_object_exclude_array tp_ret, typename... tp_args>
  requires
  (
    !dango::is_const_or_volatile<tp_ret> &&
    dango::is_brace_constructible<tp_ret, dango::remove_ref<tp_args> const&...>
  )
  constexpr auto
  ctie_as_emplacer
  (tp_args&&... a_args)noexcept->auto
  {
    using return_type = tp_ret;

    constexpr bool const c_noexcept =
      dango::is_noexcept_brace_constructible<return_type, dango::remove_ref<tp_args> const&...>;

    return
      dango::make_emplacer<return_type>
      (
        [&a_args...]()constexpr noexcept(c_noexcept)->return_type
        {
          return return_type{ dango::as_const(a_args)... };
        }
      );
  }

  template
  <dango::is_object_exclude_array tp_ret, typename... tp_args>
  requires
  (
    !dango::is_const_or_volatile<tp_ret> &&
    dango::is_brace_constructible<tp_ret, tp_args...>
  )
  constexpr auto
  forward_as_emplacer
  (tp_args&&... a_args)noexcept->auto
  {
    using return_type = tp_ret;

    constexpr bool const c_noexcept =
      dango::is_noexcept_brace_constructible<return_type, tp_args...>;

    return
      dango::make_emplacer<return_type>
      (
        [&a_args...]()constexpr noexcept(c_noexcept)->return_type
        {
          return return_type{ dango::forward<tp_args>(a_args)... };
        }
      );
  }
}

namespace
dango::detail
{
  template
  <typename tp_type>
  inline constexpr bool const is_emplacer_help = false;

  template
  <typename tp_type, typename tp_func>
  inline constexpr bool const is_emplacer_help<dango::detail::emplacer<tp_type, tp_func>> = true;
}

namespace
dango
{
  template
  <typename tp_emplacer>
  concept is_emplacer =
    dango::detail::is_emplacer_help<dango::remove_cv<tp_emplacer>>;

  template
  <typename tp_emplacer>
  concept is_noexcept_emplacer =
    dango::is_emplacer<tp_emplacer> &&
    dango::is_noexcept_callable_ret<typename tp_emplacer::return_type, typename tp_emplacer::func_type&>;

  template
  <typename tp_emplacer, typename tp_type>
  concept is_emplacer_for =
    dango::is_emplacer<tp_emplacer> &&
    dango::is_same_ignore_cv<tp_type, typename tp_emplacer::return_type>;

  template
  <typename tp_emplacer, typename tp_type>
  concept is_noexcept_emplacer_for =
    dango::is_noexcept_emplacer<tp_emplacer> &&
    dango::is_same_ignore_cv<tp_type, typename tp_emplacer::return_type>;
}

namespace
dango::detail
{
  template
  <typename tp_emplacer, typename tp_default>
  struct
  emplacer_return_type_help
  final
  {
    using type = tp_default;

    DANGO_UNCONSTRUCTIBLE(emplacer_return_type_help)
  };

  template
  <dango::is_emplacer tp_emplacer, typename tp_default>
  struct
  emplacer_return_type_help<tp_emplacer, tp_default>
  final
  {
    using type = typename tp_emplacer::return_type;

    DANGO_UNCONSTRUCTIBLE(emplacer_return_type_help)
  };
}

namespace
dango
{
  template
  <typename tp_emplacer, typename tp_default = tp_emplacer>
  using emplacer_return_type =
   typename dango::detail::emplacer_return_type_help<tp_emplacer, tp_default>::type;

  template
  <typename tp_type>
  using emplacer_return_type_decay =
    dango::emplacer_return_type<dango::decay<tp_type>>;
}

#endif // DANGO_UTIL_HPP_INCLUDED
