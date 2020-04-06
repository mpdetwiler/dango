#ifndef DANGO_MACRO_HPP_INCLUDED
#define DANGO_MACRO_HPP_INCLUDED

#ifdef DANGO_BUILDING_DANGO
#define DANGO_EXPORT [[gnu::dllexport]]
#define DANGO_EXPORT_ONLY DANGO_EXPORT
#else
#define DANGO_EXPORT [[gnu::dllimport]]
#define DANGO_EXPORT_ONLY
#endif

#define DANGO_UNCOPYABLE(name) \
constexpr name(name const&)noexcept = delete; \
constexpr auto operator = (name const&)& noexcept->name& = delete;

#define DANGO_IMMOBILE(name) \
DANGO_UNCOPYABLE(name) \
constexpr name(name&&)noexcept = delete; \
constexpr auto operator = (name&&)& noexcept->name& = delete;

#define DANGO_DELETE_DEFAULT(name) \
explicit constexpr name()noexcept = delete;

#define DANGO_UNINSTANTIABLE(name) \
DANGO_IMMOBILE(name) \
DANGO_DELETE_DEFAULT(name) \
~name()noexcept = delete;

#define DANGO_UNASSIGNABLE(name) \
constexpr auto operator = (name const&)& noexcept->name& = delete; \
constexpr auto operator = (name&&)& noexcept->name& = delete;

#define DANGO_EMPTY_TYPE(name, ...) \
explicit(bool(__VA_ARGS__)) constexpr name()noexcept = default; \
constexpr name(name const&)noexcept = default; \
constexpr name(name&&)noexcept = default; \
~name()noexcept = default; \
constexpr auto operator = (name const&)& noexcept->name& = default; \
constexpr auto operator = (name&&)& noexcept->name& = default;

#define DANGO_TAG_TYPE(name) DANGO_EMPTY_TYPE(name, true)

#define DANGO_TOKEN_CONCAT_IMPL(x, y) x##y
#define DANGO_TOKEN_CONCAT(x, y) DANGO_TOKEN_CONCAT_IMPL(x, y)
#define DANGO_APPEND_LINE(x) DANGO_TOKEN_CONCAT(x, __LINE__)

/*** DANGO_PARENTH_TYPE ***/

#define DANGO_PARENTH_TYPE(...) dango::parenth_type<void(__VA_ARGS__)noexcept>

namespace
dango::detail
{
  template
  <typename tp_type>
  struct parenth_type_help;

  template
  <typename tp_ret, typename tp_type, bool tp_noexcept>
  struct parenth_type_help<tp_ret(tp_type)noexcept(tp_noexcept)>;
}

namespace
dango
{
  template
  <typename tp_type>
  using parenth_type = typename dango::detail::parenth_type_help<tp_type>::type;
}

template
<typename tp_ret, typename tp_type, bool tp_noexcept>
struct
dango::
detail::
parenth_type_help<tp_ret(tp_type)noexcept(tp_noexcept)>
final
{
  using type = tp_type;

  DANGO_UNINSTANTIABLE(parenth_type_help)
};

/*** DANGO_MAGIC_CONST_FOLD ***/

#define DANGO_MAGIC_CONST_FOLD(...) (__builtin_constant_p((__VA_ARGS__))?(__VA_ARGS__):(__VA_ARGS__))

#endif // DANGO_MACRO_HPP_INCLUDED

