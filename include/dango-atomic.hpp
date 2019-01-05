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
  static constexpr auto is_lock_free()noexcept->bool;
public:
  explicit constexpr atomic(tp_type)noexcept;

  ~atomic()noexcept = default;

  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto load()const noexcept->tp_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  void store(tp_type)noexcept;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto exchange(tp_type)noexcept->tp_type;

  template
  <
    dango::mem_order tp_success = dango::mem_order::seq_cst,
    dango::mem_order tp_failure = dango::mem_order::seq_cst
  >
  auto compare_exchange(tp_type&, tp_type)noexcept->bool;

  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto fetch_add(tp_type)noexcept->tp_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto fetch_sub(tp_type)noexcept->tp_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto fetch_and(tp_type)noexcept->tp_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto fetch_or(tp_type)noexcept->tp_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto fetch_xor(tp_type)noexcept->tp_type;

  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto add_fetch(tp_type)noexcept->tp_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto sub_fetch(tp_type)noexcept->tp_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto and_fetch(tp_type)noexcept->tp_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto or_fetch(tp_type)noexcept->tp_type;
  template
  <dango::mem_order tp_order = dango::mem_order::seq_cst>
  auto xor_fetch(tp_type)noexcept->tp_type;
private:
  tp_type m_data;
public:
  DANGO_DELETE_DEFAULT_CTOR(atomic)
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
  auto const a_temp = tp_type(0);

  return __atomic_always_lock_free(sizeof(tp_type), &a_temp);
}

template
<typename tp_type>
constexpr
dango::
atomic<tp_type, DANGO_ATOMIC_INT_ENABLE_SPEC(tp_type)>::
atomic
(tp_type const a_data)noexcept:
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
()const noexcept->tp_type
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
auto
dango::
atomic<tp_type, DANGO_ATOMIC_INT_ENABLE_SPEC(tp_type)>::
store
(tp_type const a_data)noexcept->void
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
(tp_type const a_data)noexcept->tp_type
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
(tp_type& a_expected, tp_type const a_data)noexcept->bool
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

#define DANGO_ATOMIC_DEFINE_FETCH_OP(oper)                                  \
template                                                                    \
<typename tp_type>                                                          \
template                                                                    \
<dango::mem_order tp_order>                                                 \
auto                                                                        \
dango::                                                                     \
atomic<tp_type, DANGO_ATOMIC_INT_ENABLE_SPEC(tp_type)>::                    \
fetch_##oper                                                                \
(tp_type const a_data)noexcept->tp_type                                     \
{                                                                           \
  static_assert(detail::is_valid_mem_order(tp_order));                      \
                                                                            \
  return __atomic_fetch_##oper(&m_data, a_data, dango::s_int(tp_order));    \
}

DANGO_ATOMIC_DEFINE_FETCH_OP(add)
DANGO_ATOMIC_DEFINE_FETCH_OP(sub)
DANGO_ATOMIC_DEFINE_FETCH_OP(and)
DANGO_ATOMIC_DEFINE_FETCH_OP(or)
DANGO_ATOMIC_DEFINE_FETCH_OP(xor)

#undef DANGO_ATOMIC_DEFINE_FETCH_OP

#define DANGO_ATOMIC_DEFINE_OP_FETCH(oper)                                  \
template                                                                    \
<typename tp_type>                                                          \
template                                                                    \
<dango::mem_order tp_order>                                                 \
auto                                                                        \
dango::                                                                     \
atomic<tp_type, DANGO_ATOMIC_INT_ENABLE_SPEC(tp_type)>::                    \
oper##_fetch                                                                \
(tp_type const a_data)noexcept->tp_type                                     \
{                                                                           \
  static_assert(detail::is_valid_mem_order(tp_order));                      \
                                                                            \
  return __atomic_##oper##_fetch(&m_data, a_data, dango::s_int(tp_order));  \
}

DANGO_ATOMIC_DEFINE_OP_FETCH(add)
DANGO_ATOMIC_DEFINE_OP_FETCH(sub)
DANGO_ATOMIC_DEFINE_OP_FETCH(and)
DANGO_ATOMIC_DEFINE_OP_FETCH(or)
DANGO_ATOMIC_DEFINE_OP_FETCH(xor)

#undef DANGO_ATOMIC_DEFINE_OP_FETCH

#undef DANGO_ATOMIC_INT_ENABLE_SPEC

/*************************************************************/

template
<typename tp_type>
class
dango::
atomic<tp_type*, dango::enable_tag>
final
{

};

/*************************************************************/

template<>
class
dango::
atomic<bool, dango::enable_tag>
final
{

};

#endif

