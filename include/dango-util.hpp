#ifndef DANGO_UTIL_HPP_INCLUDED
#define DANGO_UTIL_HPP_INCLUDED

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
  m_dismissed{ false }
  {

  }

  ~scope_guard()noexcept;

  void dismiss()noexcept;
private:
  func_type m_func;
  bool m_dismissed;
public:
  DANGO_DELETE_DEFAULT(scope_guard)
  DANGO_UNMOVEABLE(scope_guard)
};

template
<typename tp_func>
dango::
detail::
scope_guard<tp_func>::
~scope_guard
()noexcept
{
  static_assert(noexcept(m_func()));
  static_assert(dango::is_noexcept_destructible<func_type>);

  if(m_dismissed)
  {
    return;
  }

  m_func();
}

template
<typename tp_func>
void
dango::
detail::
scope_guard<tp_func>::
dismiss
()noexcept
{
  m_dismissed = true;
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

  ~finally_guard()noexcept;
private:
  func_type m_func;
public:
  DANGO_DELETE_DEFAULT(finally_guard)
  DANGO_UNMOVEABLE(finally_guard)
};

template
<typename tp_func>
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
  requires(!dango::is_const<tp_type> && !dango::is_volatile<tp_type>)
  class member_function;

  template
  <typename tp_type>
  using member_func =
    dango::member_function<dango::remove_cv<tp_type>>;

  template
  <dango::is_member_func_ptr tp_arg>
  constexpr auto
  make_member_func
  (tp_arg const a_arg)noexcept->dango::member_func<tp_arg>
  {
    return dango::member_func<tp_arg>{ a_arg };
  }
}

template
<dango::is_member_func_ptr tp_type>
requires(!dango::is_const<tp_type> && !dango::is_volatile<tp_type>)
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
  ~member_function()noexcept = default;
  constexpr auto operator = (member_function const&)&noexcept->member_function& = default;
  constexpr auto operator = (member_function&&)&noexcept->member_function& = default;

  template
  <typename tp_class, typename... tp_args>
  requires requires{ (dango::declval<tp_class>().*dango::declval<value_type const&>())(dango::declval<tp_args>()...); }
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
  requires requires{ (dango::declval<tp_class* const&>()->*dango::declval<value_type const&>())(dango::declval<tp_args>()...); }
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
  using type = dango::member_func<tp_type>;

  DANGO_UNCONSTRUCTIBLE(member_func_type_help)
};

#endif // DANGO_UTIL_HPP_INCLUDED

