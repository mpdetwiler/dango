#ifndef DANGO_OPERATOR_HPP_INCLUDED
#define DANGO_OPERATOR_HPP_INCLUDED

/*** is_nullable ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_null_equatable =
    dango::is_equatable<tp_type, dango::null_tag const&> && dango::is_equatable<tp_type, dango::null_tag&&> &&
    dango::is_equatable<dango::null_tag const&, tp_type> && dango::is_equatable<dango::null_tag&&, tp_type>;

  template
  <typename tp_type>
  concept is_noexcept_null_equatable =
    dango::is_null_equatable<tp_type> &&
    dango::is_noexcept_equatable<tp_type, dango::null_tag const&> && dango::is_noexcept_equatable<tp_type, dango::null_tag&&> &&
    dango::is_noexcept_equatable<dango::null_tag const&, tp_type> && dango::is_noexcept_equatable<dango::null_tag&&, tp_type>;

  template
  <typename tp_type>
  concept is_nullable =
    dango::is_object_exclude_array<tp_type> &&
    dango::is_null_equatable<dango::remove_cv<tp_type> const&> && dango::is_null_equatable<dango::remove_cv<tp_type>&&> &&
    dango::is_constructible<dango::remove_cv<tp_type>, dango::null_tag const&> && dango::is_constructible<dango::remove_cv<tp_type>, dango::null_tag&&> &&
    dango::is_convertible<dango::null_tag const&, dango::remove_cv<tp_type>> && dango::is_convertible<dango::null_tag&&, dango::remove_cv<tp_type>> &&
    dango::is_assignable<dango::remove_cv<tp_type>&, dango::null_tag const&> && dango::is_assignable<dango::remove_cv<tp_type>&, dango::null_tag&&>;
}

/*** enum min max ***/

namespace
dango
{
  template
  <dango::is_enum tp_arg>
  requires(dango::is_comparable<tp_arg const&, tp_arg const&>)
  constexpr auto
  min
  (tp_arg const a_arg)noexcept->tp_arg
  {
    return a_arg;
  }

  template
  <dango::is_enum tp_arg>
  requires(dango::is_comparable<tp_arg const&, tp_arg const&>)
  constexpr auto
  min
  (tp_arg const a_arg1, tp_arg const a_arg2)noexcept(dango::is_noexcept_comparable<tp_arg const&, tp_arg const&>)->tp_arg
  {
    return a_arg1 < a_arg2 ? a_arg1 : a_arg2;
  }

  template
  <dango::is_enum tp_arg, typename... tp_args>
  requires
  ((sizeof...(tp_args) >= dango::usize(2)) && (dango::is_comparable<tp_arg const&, tp_arg const&> && ... && dango::is_same<tp_args, tp_arg>))
  constexpr auto
  min
  (tp_arg const a_arg, tp_args... a_args)noexcept(dango::is_noexcept_comparable<tp_arg const&, tp_arg const&>)->tp_arg
  {
    return dango::min(a_arg, dango::min(a_args...));
  }
}

namespace
dango
{
  template
  <dango::is_enum tp_arg>
  requires(dango::is_comparable<tp_arg const&, tp_arg const&>)
  constexpr auto
  max
  (tp_arg const a_arg)noexcept->tp_arg
  {
    return a_arg;
  }

  template
  <dango::is_enum tp_arg>
  requires(dango::is_comparable<tp_arg const&, tp_arg const&>)
  constexpr auto
  max
  (tp_arg const a_arg1, tp_arg const a_arg2)noexcept(dango::is_noexcept_comparable<tp_arg const&, tp_arg const&>)->tp_arg
  {
    return a_arg1 > a_arg2 ? a_arg1 : a_arg2;
  }

  template
  <dango::is_enum tp_arg, typename... tp_args>
  requires
  ((sizeof...(tp_args) >= dango::usize(2)) && (dango::is_comparable<tp_arg const&, tp_arg const&> && ... && dango::is_same<tp_args, tp_arg>))
  constexpr auto
  max
  (tp_arg const a_arg, tp_args... a_args)noexcept(dango::is_noexcept_comparable<tp_arg const&, tp_arg const&>)->tp_arg
  {
    return dango::max(a_arg, dango::max(a_args...));
  }
}

/*** generic min max ***/

namespace
dango::detail
{
  template
  <typename... tp_args>
  struct min_max_help;
}

namespace
dango
{
  template
  <typename tp_arg>
  requires
  (
    dango::is_class_or_union<dango::remove_ref<tp_arg>> &&
    dango::is_comparable<tp_arg, tp_arg> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg>
  )
  constexpr auto
  min
  (tp_arg&& a_arg)
  noexcept(dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg>)->dango::remove_cvref<tp_arg>
  {
    return dango::remove_cvref<tp_arg>{ dango::forward<tp_arg>(a_arg) };
  }

  template
  <typename tp_arg, typename tp_same>
  requires
  (
    dango::is_class_or_union<dango::remove_ref<tp_arg>> && dango::is_same_ignore_cvref<tp_arg, tp_same> &&
    dango::is_comparable<tp_arg, tp_same> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg>, tp_same>
  )
  constexpr auto
  min
  (tp_arg&& a_arg1, tp_same&& a_arg2)
  noexcept
  (
    dango::is_noexcept_comparable<tp_arg, tp_same> &&
    dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg> &&
    dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg>, tp_same>
  )->dango::remove_cvref<tp_arg>
  {
    using ret_type = dango::remove_cvref<tp_arg>;

    if(dango::forward<tp_arg>(a_arg1) < dango::forward<tp_same>(a_arg2))
    {
      return ret_type{ dango::forward<tp_arg>(a_arg1) };
    }
    else
    {
      return ret_type{ dango::forward<tp_same>(a_arg2) };
    }
  }

  template
  <typename tp_arg, typename... tp_args>
  requires
  (
    (sizeof...(tp_args) >= dango::usize(2)) &&
    (dango::is_class_or_union<dango::remove_ref<tp_arg>> && ... && dango::is_same_ignore_cvref<tp_arg, tp_args>) &&
    requires
    {
      { dango::detail::min_max_help<tp_args&&...>::min(dango::declval<tp_args>()...) }->dango::is_same<dango::remove_cvref<tp_arg>>;
      { dango::min(dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }->dango::is_same<dango::remove_cvref<tp_arg>>;
    }
  )
  constexpr auto
  min
  (tp_arg&& a_arg, tp_args&&... a_args)
  noexcept
  (
    requires
    {
      { dango::detail::min_max_help<tp_args&&...>::min(dango::declval<tp_args>()...) }noexcept;
      { dango::min(dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }noexcept;
    }
  )->dango::remove_cvref<tp_arg>
  {
    using ret_type = dango::remove_cvref<tp_arg>;

    return ret_type{ dango::min(dango::forward<tp_arg>(a_arg), dango::detail::min_max_help<tp_args&&...>::min(dango::forward<tp_args>(a_args)...)) };
  }
}



namespace
dango
{
  template
  <typename tp_arg>
  requires
  (
    dango::is_class_or_union<dango::remove_ref<tp_arg>> &&
    dango::is_comparable<tp_arg, tp_arg> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg>
  )
  constexpr auto
  max
  (tp_arg&& a_arg)
  noexcept(dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg>)->dango::remove_cvref<tp_arg>
  {
    return dango::remove_cvref<tp_arg>{ dango::forward<tp_arg>(a_arg) };
  }

  template
  <typename tp_arg, typename tp_same>
  requires
  (
    dango::is_class_or_union<dango::remove_ref<tp_arg>> && dango::is_same_ignore_cvref<tp_arg, tp_same> &&
    dango::is_comparable<tp_arg, tp_same> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg> &&
    dango::is_brace_constructible<dango::remove_cvref<tp_arg>, tp_same>
  )
  constexpr auto
  max
  (tp_arg&& a_arg1, tp_same&& a_arg2)
  noexcept
  (
    dango::is_noexcept_comparable<tp_arg, tp_same> &&
    dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg>, tp_arg> &&
    dango::is_noexcept_brace_constructible<dango::remove_cvref<tp_arg>, tp_same>
  )->dango::remove_cvref<tp_arg>
  {
    using ret_type = dango::remove_cvref<tp_arg>;

    if(dango::forward<tp_arg>(a_arg1) > dango::forward<tp_same>(a_arg2))
    {
      return ret_type{ dango::forward<tp_arg>(a_arg1) };
    }
    else
    {
      return ret_type{ dango::forward<tp_same>(a_arg2) };
    }
  }

  template
  <typename tp_arg, typename... tp_args>
  requires
  (
    (sizeof...(tp_args) >= dango::usize(2)) &&
    (dango::is_class_or_union<dango::remove_ref<tp_arg>> && ... && dango::is_same_ignore_cvref<tp_arg, tp_args>) &&
    requires
    {
      { dango::detail::min_max_help<tp_args&&...>::max(dango::declval<tp_args>()...) }->dango::is_same<dango::remove_cvref<tp_arg>>;
      { dango::max(dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }->dango::is_same<dango::remove_cvref<tp_arg>>;
    }
  )
  constexpr auto
  max
  (tp_arg&& a_arg, tp_args&&... a_args)
  noexcept
  (
    requires
    {
      { dango::detail::min_max_help<tp_args&&...>::max(dango::declval<tp_args>()...) }noexcept;
      { dango::max(dango::declval<tp_arg>(), dango::declval<dango::remove_cvref<tp_arg>>()) }noexcept;
    }
  )->dango::remove_cvref<tp_arg>
  {
    using ret_type = dango::remove_cvref<tp_arg>;

    return ret_type{ dango::max(dango::forward<tp_arg>(a_arg), dango::detail::min_max_help<tp_args&&...>::max(dango::forward<tp_args>(a_args)...)) };
  }
}

template
<typename... tp_args>
struct
dango::
detail::
min_max_help
final
{
  static constexpr auto
  min(tp_args... a_args)noexcept(requires{ { dango::min(dango::declval<tp_args>()...) }noexcept; })->decltype(auto)
  requires(requires{ { dango::min(dango::declval<tp_args>()...) }; })
  {
    return dango::min(dango::forward<tp_args>(a_args)...);
  }

  static constexpr auto
  max(tp_args... a_args)noexcept(requires{ { dango::max(dango::declval<tp_args>()...) }noexcept; })->decltype(auto)
  requires(requires{ { dango::max(dango::declval<tp_args>()...) }; })
  {
    return dango::max(dango::forward<tp_args>(a_args)...);
  }

  DANGO_UNINSTANTIABLE(min_max_help)
};

#endif // DANGO_OPERATOR_HPP_INCLUDED

