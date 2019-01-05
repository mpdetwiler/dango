#ifndef __DANGO_ATOMIC_HPP__
#define __DANGO_ATOMIC_HPP__

#include "dango-traits.hpp"

namespace
dango
{
  enum class
  mem_order:
  dango::s_int
  {
    relaxed = __ATOMIC_RELAXED,
    acquire = __ATOMIC_ACQUIRE,
    release = __ATOMIC_RELEASE,
    acq_rel = __ATOMIC_ACQ_REL,
    seq_cst = __ATOMIC_SEQ_CST
  };
}

namespace
dango::detail
{
  constexpr auto is_valid_mem_order(dango::mem_order)noexcept->bool;
}

constexpr auto
dango::
detail::
is_valid_mem_order
(dango::mem_order const a_order)noexcept->bool
{
  switch(a_order)
  {
    default: return false;
    case dango::mem_order::relaxed: break;
    case dango::mem_order::acquire: break;
    case dango::mem_order::release: break;
    case dango::mem_order::acq_rel: break;
    case dango::mem_order::seq_cst: break;
  }

  return true;
}

namespace
dango
{
  template
  <typename tp_type, typename tp_enabled = dango::enable_tag>
  class atomic;

  template
  <typename tp_type>
  class atomic<tp_type*, dango::enable_tag>;

  template<>
  class atomic<bool, dango::enable_tag>;
}

/*************************************************************/

#define DANGO_ATOMIC_INT_ENABLE_SPEC(param)   \
dango::enable_if                              \
<                                             \
  dango::is_int<param> &&                     \
  !dango::is_const<param> &&                  \
  !dango::is_volatile<param>,                 \
  dango::enable_tag                           \
>

namespace
dango
{
  template
  <typename tp_type>
  class atomic<tp_type, DANGO_ATOMIC_INT_ENABLE_SPEC(tp_type)>;
}

template
<typename tp_type>
class
dango::
atomic<tp_type, DANGO_ATOMIC_INT_ENABLE_SPEC(tp_type)>
final
{
public:
  using value_type = tp_type;

  static constexpr auto is_lock_free()noexcept->bool;
public:
  explicit constexpr atomic(value_type)noexcept;

  ~atomic()noexcept = default;

  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto load()const noexcept->value_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  void store(value_type)noexcept;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto exchange(value_type)noexcept->value_type;

  template
  <
    dango::mem_order tp_success = dango::mem_order::seq_cst,
    dango::mem_order tp_failure = dango::mem_order::seq_cst
  >
  auto compare_exchange(value_type&, value_type)noexcept->bool;

  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto fetch_add(value_type)noexcept->value_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto fetch_sub(value_type)noexcept->value_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto fetch_and(value_type)noexcept->value_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto fetch_or(value_type)noexcept->value_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto fetch_xor(value_type)noexcept->value_type;

  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto add_fetch(value_type)noexcept->value_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto sub_fetch(value_type)noexcept->value_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto and_fetch(value_type)noexcept->value_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto or_fetch(value_type)noexcept->value_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto xor_fetch(value_type)noexcept->value_type;
private:
  value_type m_data;
public:
  DANGO_DELETE_DEFAULT(atomic)
  DANGO_IMMOBILE(atomic)
};

template
<typename tp_type>
constexpr auto
dango::
atomic<tp_type, DANGO_ATOMIC_INT_ENABLE_SPEC(tp_type)>::
is_lock_free
()noexcept->bool
{
  auto a_temp = value_type(0);

  return __atomic_always_lock_free(sizeof(value_type), &a_temp);
}

template
<typename tp_type>
constexpr
dango::
atomic<tp_type, DANGO_ATOMIC_INT_ENABLE_SPEC(tp_type)>::
atomic
(value_type const a_data)noexcept:
m_data{ a_data }
{

}

template
<typename tp_type>
template
<dango::mem_order tp_order>
auto
dango::
atomic<tp_type, DANGO_ATOMIC_INT_ENABLE_SPEC(tp_type)>::
load
()const noexcept->value_type
{
  static_assert(detail::is_valid_mem_order(tp_order));

  static_assert
  (
    tp_order == dango::mem_order::relaxed ||
    tp_order == dango::mem_order::acquire ||
    tp_order == dango::mem_order::seq_cst
  );

  return __atomic_load_n(&m_data, dango::s_int(tp_order));
}

template
<typename tp_type>
template
<dango::mem_order tp_order>
void
dango::
atomic<tp_type, DANGO_ATOMIC_INT_ENABLE_SPEC(tp_type)>::
store
(value_type const a_data)noexcept
{
  static_assert(detail::is_valid_mem_order(tp_order));

  static_assert
  (
    tp_order == dango::mem_order::relaxed ||
    tp_order == dango::mem_order::release ||
    tp_order == dango::mem_order::seq_cst
  );

  __atomic_store_n(&m_data, a_data, dango::s_int(tp_order));
}

template
<typename tp_type>
template
<dango::mem_order tp_order>
auto
dango::
atomic<tp_type, DANGO_ATOMIC_INT_ENABLE_SPEC(tp_type)>::
exchange
(value_type const a_data)noexcept->value_type
{
  static_assert(detail::is_valid_mem_order(tp_order));

  return __atomic_exchange_n(&m_data, a_data, dango::s_int(tp_order));
}

template
<typename tp_type>
template
<
  dango::mem_order tp_success,
  dango::mem_order tp_failure
>
auto
dango::
atomic<tp_type, DANGO_ATOMIC_INT_ENABLE_SPEC(tp_type)>::
compare_exchange
(value_type& a_expected, value_type const a_data)noexcept->bool
{
  static_assert(detail::is_valid_mem_order(tp_success));
  static_assert(detail::is_valid_mem_order(tp_failure));

  static_assert
  (
    tp_failure == dango::mem_order::relaxed ||
    tp_failure == dango::mem_order::acquire ||
    tp_failure == dango::mem_order::seq_cst
  );

  bool const a_result =
  __atomic_compare_exchange_n
  (
    &m_data,
    &a_expected,
    a_data,
    false,
    dango::s_int(tp_success),
    dango::s_int(tp_failure)
  );

  return a_result;
}

#define DANGO_ATOMIC_DEFINE_FETCH(name)                                     \
template                                                                    \
<typename tp_type>                                                          \
template                                                                    \
<dango::mem_order tp_order>                                                 \
auto                                                                        \
dango::                                                                     \
atomic<tp_type, DANGO_ATOMIC_INT_ENABLE_SPEC(tp_type)>::                    \
name                                                                        \
(value_type const a_data)noexcept->value_type                               \
{                                                                           \
  static_assert(detail::is_valid_mem_order(tp_order));                      \
                                                                            \
  return __atomic_##name(&m_data, a_data, dango::s_int(tp_order));          \
}

DANGO_ATOMIC_DEFINE_FETCH(fetch_add)
DANGO_ATOMIC_DEFINE_FETCH(fetch_sub)
DANGO_ATOMIC_DEFINE_FETCH(fetch_and)
DANGO_ATOMIC_DEFINE_FETCH(fetch_or)
DANGO_ATOMIC_DEFINE_FETCH(fetch_xor)

DANGO_ATOMIC_DEFINE_FETCH(add_fetch)
DANGO_ATOMIC_DEFINE_FETCH(sub_fetch)
DANGO_ATOMIC_DEFINE_FETCH(and_fetch)
DANGO_ATOMIC_DEFINE_FETCH(or_fetch)
DANGO_ATOMIC_DEFINE_FETCH(xor_fetch)

#undef DANGO_ATOMIC_DEFINE_FETCH

#undef DANGO_ATOMIC_INT_ENABLE_SPEC

/*************************************************************/

template
<typename tp_type>
class
dango::
atomic<tp_type*, dango::enable_tag>
final
{
public:
  using value_type = tp_type*;

  static constexpr auto is_lock_free()noexcept->bool;
public:
  explicit constexpr atomic(value_type)noexcept;

  ~atomic()noexcept = default;

  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto load()const noexcept->value_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  void store(value_type)noexcept;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto exchange(value_type)noexcept->value_type;

  template
  <
    dango::mem_order tp_success = dango::mem_order::seq_cst,
    dango::mem_order tp_failure = dango::mem_order::seq_cst
  >
  auto compare_exchange(value_type&, value_type)noexcept->bool;

  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst, typename tp_pointed = tp_type>
  auto fetch_add(dango::ssize)noexcept->
  dango::enable_if<!dango::is_func<tp_pointed>, value_type>;

  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst, typename tp_pointed = tp_type>
  auto fetch_sub(dango::ssize)noexcept->
  dango::enable_if<!dango::is_func<tp_pointed>, value_type>;

  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst, typename tp_pointed = tp_type>
  auto add_fetch(dango::ssize)noexcept->
  dango::enable_if<!dango::is_func<tp_pointed>, value_type>;

  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst, typename tp_pointed = tp_type>
  auto sub_fetch(dango::ssize)noexcept->
  dango::enable_if<!dango::is_func<tp_pointed>, value_type>;
private:
  value_type m_data;
public:
  DANGO_DELETE_DEFAULT(atomic)
  DANGO_IMMOBILE(atomic)
};

template
<typename tp_type>
constexpr auto
dango::
atomic<tp_type*, dango::enable_tag>::
is_lock_free
()noexcept->bool
{
  auto a_temp = value_type(nullptr);

  return __atomic_always_lock_free(sizeof(value_type), &a_temp);
}

template
<typename tp_type>
constexpr
dango::
atomic<tp_type*, dango::enable_tag>::
atomic
(value_type const a_data)noexcept:
m_data{ a_data }
{

}

template
<typename tp_type>
template
<dango::mem_order tp_order>
auto
dango::
atomic<tp_type*, dango::enable_tag>::
load
()const noexcept->value_type
{
  static_assert(detail::is_valid_mem_order(tp_order));

  static_assert
  (
    tp_order == dango::mem_order::relaxed ||
    tp_order == dango::mem_order::acquire ||
    tp_order == dango::mem_order::seq_cst
  );

  return __atomic_load_n(&m_data, dango::s_int(tp_order));
}

template
<typename tp_type>
template
<dango::mem_order tp_order>
void
dango::
atomic<tp_type*, dango::enable_tag>::
store
(value_type const a_data)noexcept
{
  static_assert(detail::is_valid_mem_order(tp_order));

  static_assert
  (
    tp_order == dango::mem_order::relaxed ||
    tp_order == dango::mem_order::release ||
    tp_order == dango::mem_order::seq_cst
  );

  __atomic_store_n(&m_data, a_data, dango::s_int(tp_order));
}

template
<typename tp_type>
template
<dango::mem_order tp_order>
auto
dango::
atomic<tp_type*, dango::enable_tag>::
exchange
(value_type const a_data)noexcept->value_type
{
  static_assert(detail::is_valid_mem_order(tp_order));

  return __atomic_exchange_n(&m_data, a_data, dango::s_int(tp_order));
}

template
<typename tp_type>
template
<
  dango::mem_order tp_success,
  dango::mem_order tp_failure
>
auto
dango::
atomic<tp_type*, dango::enable_tag>::
compare_exchange
(value_type& a_expected, value_type const a_data)noexcept->bool
{
  static_assert(detail::is_valid_mem_order(tp_success));
  static_assert(detail::is_valid_mem_order(tp_failure));

  static_assert
  (
    tp_failure == dango::mem_order::relaxed ||
    tp_failure == dango::mem_order::acquire ||
    tp_failure == dango::mem_order::seq_cst
  );

  bool const a_result =
  __atomic_compare_exchange_n
  (
    &m_data,
    &a_expected,
    a_data,
    false,
    dango::s_int(tp_success),
    dango::s_int(tp_failure)
  );

  return a_result;
}

#define DANGO_ATOMIC_DEFINE_FETCH(name)                                     \
template                                                                    \
<typename tp_type>                                                          \
template                                                                    \
<dango::mem_order tp_order, typename tp_pointed>                            \
auto                                                                        \
dango::                                                                     \
atomic<tp_type*, dango::enable_tag>::                                       \
name                                                                        \
(dango::ssize const a_data)noexcept->                                       \
dango::enable_if<!dango::is_func<tp_pointed>, value_type>                   \
{                                                                           \
  static_assert(detail::is_valid_mem_order(tp_order));                      \
                                                                            \
  value_type const a_result =                                               \
  __atomic_##name                                                           \
  (                                                                         \
    &m_data,                                                                \
    a_data * dango::ssize(sizeof(tp_type)),                                 \
    dango::s_int(tp_order)                                                  \
  );                                                                        \
                                                                            \
  return a_result;                                                          \
}

DANGO_ATOMIC_DEFINE_FETCH(fetch_add)
DANGO_ATOMIC_DEFINE_FETCH(fetch_sub)

DANGO_ATOMIC_DEFINE_FETCH(add_fetch)
DANGO_ATOMIC_DEFINE_FETCH(sub_fetch)

#undef DANGO_ATOMIC_DEFINE_FETCH

/*************************************************************/

template<>
class
dango::
atomic<bool, dango::enable_tag>
final
{
public:
  using value_type = bool;

  static constexpr auto is_lock_free()noexcept->bool;
public:
  explicit constexpr atomic(value_type)noexcept;

  ~atomic()noexcept = default;

  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto load()const noexcept->value_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  void store(value_type)noexcept;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto exchange(value_type)noexcept->value_type;

  template
  <
    dango::mem_order tp_success = dango::mem_order::seq_cst,
    dango::mem_order tp_failure = dango::mem_order::seq_cst
  >
  auto compare_exchange(value_type&, value_type)noexcept->bool;
private:
  value_type m_data;
public:
  DANGO_DELETE_DEFAULT(atomic)
  DANGO_IMMOBILE(atomic)
};

constexpr auto
dango::
atomic<bool, dango::enable_tag>::
is_lock_free
()noexcept->bool
{
  auto a_temp = value_type(false);

  return __atomic_always_lock_free(sizeof(value_type), &a_temp);
}

constexpr
dango::
atomic<bool, dango::enable_tag>::
atomic
(value_type const a_data)noexcept:
m_data{ a_data }
{

}

template
<dango::mem_order tp_order>
auto
dango::
atomic<bool, dango::enable_tag>::
load
()const noexcept->value_type
{
  static_assert(detail::is_valid_mem_order(tp_order));

  static_assert
  (
    tp_order == dango::mem_order::relaxed ||
    tp_order == dango::mem_order::acquire ||
    tp_order == dango::mem_order::seq_cst
  );

  return __atomic_load_n(&m_data, dango::s_int(tp_order));
}

template
<dango::mem_order tp_order>
void
dango::
atomic<bool, dango::enable_tag>::
store
(value_type const a_data)noexcept
{
  static_assert(detail::is_valid_mem_order(tp_order));

  static_assert
  (
    tp_order == dango::mem_order::relaxed ||
    tp_order == dango::mem_order::release ||
    tp_order == dango::mem_order::seq_cst
  );

  __atomic_store_n(&m_data, a_data, dango::s_int(tp_order));
}

template
<dango::mem_order tp_order>
auto
dango::
atomic<bool, dango::enable_tag>::
exchange
(value_type const a_data)noexcept->value_type
{
  static_assert(detail::is_valid_mem_order(tp_order));

  return __atomic_exchange_n(&m_data, a_data, dango::s_int(tp_order));
}

template
<
  dango::mem_order tp_success,
  dango::mem_order tp_failure
>
auto
dango::
atomic<bool, dango::enable_tag>::
compare_exchange
(value_type& a_expected, value_type const a_data)noexcept->bool
{
  static_assert(detail::is_valid_mem_order(tp_success));
  static_assert(detail::is_valid_mem_order(tp_failure));

  static_assert
  (
    tp_failure == dango::mem_order::relaxed ||
    tp_failure == dango::mem_order::acquire ||
    tp_failure == dango::mem_order::seq_cst
  );

  bool const a_result =
  __atomic_compare_exchange_n
  (
    &m_data,
    &a_expected,
    a_data,
    false,
    dango::s_int(tp_success),
    dango::s_int(tp_failure)
  );

  return a_result;
}

#endif

