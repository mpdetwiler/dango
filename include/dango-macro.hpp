#ifndef __DANGO_MACRO_HPP__
#define __DANGO_MACRO_HPP__ 1

#define DANGO_UNCOPYABLE(name) \
  constexpr name(name const&)noexcept = delete; \
  constexpr auto operator = (name const&)& noexcept->name& = delete;

#define DANGO_IMMOBILE(name) \
  DANGO_UNCOPYABLE(name) \
  constexpr name(name&&)noexcept = delete; \
  constexpr auto operator = (name&&)& noexcept->name& = delete;

#define DANGO_DELETE_DEFAULT(name) \
  constexpr name()noexcept = delete;

#define DANGO_UNINSTANTIABLE(name) \
  DANGO_IMMOBILE(name) \
  DANGO_DELETE_DEFAULT(name) \
  ~name()noexcept = delete;

#define DANGO_UNASSIGNABLE(name) \
  constexpr auto operator = (name const&)& noexcept->name& = delete; \
  constexpr auto operator = (name&&)& noexcept->name& = delete;

#define DANGO_TAG_TYPE(name) \
  constexpr name()noexcept = default; \
  constexpr name(name const&)noexcept = default; \
  constexpr name(name&&)noexcept = default; \
  ~name()noexcept = default; \
  constexpr auto operator = (name const&)& noexcept->name& = default; \
  constexpr auto operator = (name&&)& noexcept->name& = default;

#define DANGO_TOKEN_CONCAT_IMPL(x, y) x##y
#define DANGO_TOKEN_CONCAT(x, y) DANGO_TOKEN_CONCAT_IMPL(x, y)
#define DANGO_APPEND_LINE(x) DANGO_TOKEN_CONCAT(x, __LINE__)

#endif

