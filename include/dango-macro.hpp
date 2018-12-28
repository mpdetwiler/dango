#ifndef __DANGO_MACRO_HPP__
#define __DANGO_MACRO_HPP__ 1

#define DANGO_UNCOPYABLE(name)                                              \
  constexpr name(name const&)noexcept = delete;                             \
  constexpr auto operator = (name const&)noexcept->name& = delete;

#define DANGO_IMMOBILE(name)                                                \
  DANGO_UNCOPYABLE(name)                                                    \
  constexpr name(name&&)noexcept = delete;                                  \
  constexpr auto operator = (name&&)noexcept->name& = delete;

#define DANGO_UNINSTANTIABLE(name)                                          \
  DANGO_IMMOBILE(name)                                                      \
  constexpr name()noexcept = delete;                                        \
  ~name()noexcept = delete;

#endif

