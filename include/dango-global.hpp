#ifndef __DANGO_GLOBAL_HPP__
#define __DANGO_GLOBAL_HPP__ 1

#include "dango-concurrent.hpp"

#include <cstdio>

#define DANGO_DEFINE_GLOBAL(name, type, ...) \
class \
name##_global \
final \
{ \
public: \
  class incrementer; \
private: \
  using value_type = type; \
  class storage; \
  static storage s_storage; \
private: \
  DANGO_UNINSTANTIABLE(name##_global) \
}; \
 \
class \
name##_global:: \
storage \
final \
{ \
public: \
  constexpr storage()noexcept; \
  ~storage()noexcept = default; \
  void increment()noexcept; \
  void decrement()noexcept; \
private: \
  dango::aligned_storage<sizeof(value_type), alignof(value_type)> m_storage; \
  dango::exec_once m_init; \
  dango::atomic<dango::uint32> m_ref_count; \
private: \
  DANGO_IMMOBILE(storage) \
}; \
constexpr \
name##_global:: \
storage:: \
storage \
()noexcept: \
m_storage{ }, \
m_init{ }, \
m_ref_count{ dango::uint32(0) }{ } \
class \
name##_global:: \
incrementer \
final \
{ \
public: \
  incrementer()noexcept; \
  ~incrementer()noexcept; \
public: \
  DANGO_IMMOBILE(incrementer) \
}; \
inline \
name##_global:: \
incrementer:: \
incrementer \
()noexcept \
{ \
  s_storage.increment(); \
} \
inline \
name##_global:: \
incrementer:: \
~incrementer \
()noexcept \
{ \
  s_storage.decrement(); \
} \
inline void \
name##_global:: \
storage:: \
increment \
()noexcept \
{ \
  m_init.exec \
  ( \
    [this]()noexcept->void \
    { \
      ::new (dango::placement, m_storage.get()) value_type{ __VA_ARGS__ }; \
    } \
  ); \
  m_ref_count.add_fetch(dango::uint32(1)); \
  printf("incremented\n"); \
} \
inline void \
name##_global:: \
storage:: \
decrement \
()noexcept \
{ \
  auto const a_count = m_ref_count.sub_fetch(dango::uint32(1)); \
  printf("decremented\n"); \
  if(a_count != dango::uint32(0)) \
  { \
    return; \
  } \
  auto const a_ptr = static_cast<value_type*>(m_storage.get()); \
  a_ptr->~value_type(); \
} \
inline name##_global::storage \
name##_global:: \
s_storage{ }; \
static name##_global::incrementer const s_##name##_incrementer{ };

struct
printer
{
  printer(dango::uint32 const a_x)noexcept
  {
    printf("printer constructor %u\n", a_x);
  }

  ~printer()noexcept
  {
    printf("printer destructor\n");
  }
};

DANGO_DEFINE_GLOBAL(global_printer, printer, 7u)

#endif

