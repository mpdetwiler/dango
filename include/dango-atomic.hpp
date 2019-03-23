#ifndef __DANGO_ATOMIC_HPP__
#define __DANGO_ATOMIC_HPP__

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
dango::detail
{
  template
  <typename tp_type>
  constexpr auto
  is_atomic_help
  ()noexcept->bool
  {
    using temp_type = dango::remove_cv<tp_type>;

    temp_type a_temp{ };

    return __atomic_always_lock_free(sizeof(a_temp), &a_temp);
  }
}

namespace
dango
{
  template
  <typename tp_type, typename tp_enabled = dango::enable_tag>
  constexpr bool const is_atomic = false;

  template
  <typename tp_type>
  constexpr bool const
  is_atomic
  <tp_type, dango::enable_if<dango::is_scalar<tp_type> && !dango::is_null_ptr<tp_type>>> =
    detail::is_atomic_help<tp_type>();
}

static_assert(dango::is_atomic<bool>);
static_assert(dango::is_atomic<char>);
static_assert(dango::is_atomic<dango::wchar>);
static_assert(dango::is_atomic<dango::dchar>);
static_assert(dango::is_atomic<wchar_t>);
static_assert(dango::is_atomic<dango::uint8>);
static_assert(dango::is_atomic<dango::sint8>);
static_assert(dango::is_atomic<dango::uint16>);
static_assert(dango::is_atomic<dango::sint16>);
static_assert(dango::is_atomic<dango::uint32>);
static_assert(dango::is_atomic<dango::sint32>);
static_assert(dango::is_atomic<dango::usize>);
static_assert(dango::is_atomic<dango::ssize>);
static_assert(dango::is_atomic<dango::uintptr>);
static_assert(dango::is_atomic<dango::sintptr>);
static_assert(dango::is_atomic<void*>);
static_assert(dango::is_atomic<void(*)()noexcept>);

namespace
dango
{
  template
  <typename tp_type, typename tp_enabled = dango::enable_tag>
  class atomic;
}

/*************************************************************/

#define DANGO_ATOMIC_ENABLE_SPEC(param) \
dango::enable_if \
< \
  dango::is_arithmetic<dango::underlying_type<param>> && \
  !dango::is_const<param> && \
  !dango::is_volatile<param> && \
  dango::is_atomic<param> \
>

namespace
dango
{
  template
  <typename tp_type>
  class atomic<tp_type, DANGO_ATOMIC_ENABLE_SPEC(tp_type)>;
}

template
<typename tp_type>
class
dango::
atomic<tp_type, DANGO_ATOMIC_ENABLE_SPEC(tp_type)>
final
{
public:
  using value_type = tp_type;
public:
  explicit constexpr atomic(value_type)noexcept;
  ~atomic()noexcept = default;
  constexpr auto address()noexcept->value_type*;
  constexpr auto address()const noexcept->value_type const*;

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
  <
    dango::mem_order tp_order = dango::mem_order::seq_cst,
    typename tp_value_type = value_type,
    typename tp_ret =
      decltype(dango::declval<tp_value_type>() + dango::declval<tp_value_type>())
  >
  auto fetch_add(value_type)noexcept->
  dango::enable_if
  <
    dango::is_int<dango::underlying_type<tp_value_type>> &&
    dango::is_convertible<tp_ret, value_type>,
    value_type
  >;

  template
  <
    dango::mem_order tp_order = dango::mem_order::seq_cst,
    typename tp_value_type = value_type,
    typename tp_ret =
      decltype(dango::declval<tp_value_type>() - dango::declval<tp_value_type>())
  >
  auto fetch_sub(value_type)noexcept->
  dango::enable_if
  <
    dango::is_int<dango::underlying_type<tp_value_type>> &&
    dango::is_convertible<tp_ret, value_type>,
    value_type
  >;

  template
  <
    dango::mem_order tp_order = dango::mem_order::seq_cst,
    typename tp_value_type = value_type,
    typename tp_ret =
      decltype(dango::declval<tp_value_type>() & dango::declval<tp_value_type>())
  >
  auto fetch_and(value_type)noexcept->
  dango::enable_if
  <
    dango::is_int<dango::underlying_type<tp_value_type>> &&
    dango::is_convertible<tp_ret, value_type>,
    value_type
  >;

  template
  <
    dango::mem_order tp_order = dango::mem_order::seq_cst,
    typename tp_value_type = value_type,
    typename tp_ret =
      decltype(dango::declval<tp_value_type>() | dango::declval<tp_value_type>())
  >
  auto fetch_or(value_type)noexcept->
  dango::enable_if
  <
    dango::is_int<dango::underlying_type<tp_value_type>> &&
    dango::is_convertible<tp_ret, value_type>,
    value_type
  >;

  template
  <
    dango::mem_order tp_order = dango::mem_order::seq_cst,
    typename tp_value_type = value_type,
    typename tp_ret =
      decltype(dango::declval<tp_value_type>() ^ dango::declval<tp_value_type>())
  >
  auto fetch_xor(value_type)noexcept->
  dango::enable_if
  <
    dango::is_int<dango::underlying_type<tp_value_type>> &&
    dango::is_convertible<tp_ret, value_type>,
    value_type
  >;

  template
  <
    dango::mem_order tp_order = dango::mem_order::seq_cst,
    typename tp_value_type = value_type,
    typename tp_ret =
      decltype(dango::declval<tp_value_type>() + dango::declval<tp_value_type>())
  >
  auto add_fetch(value_type)noexcept->
  dango::enable_if
  <
    dango::is_int<dango::underlying_type<tp_value_type>> &&
    dango::is_convertible<tp_ret, value_type>,
    value_type
  >;

  template
  <
    dango::mem_order tp_order = dango::mem_order::seq_cst,
    typename tp_value_type = value_type,
    typename tp_ret =
      decltype(dango::declval<tp_value_type>() - dango::declval<tp_value_type>())
  >
  auto sub_fetch(value_type)noexcept->
  dango::enable_if
  <
    dango::is_int<dango::underlying_type<tp_value_type>> &&
    dango::is_convertible<tp_ret, value_type>,
    value_type
  >;

  template
  <
    dango::mem_order tp_order = dango::mem_order::seq_cst,
    typename tp_value_type = value_type,
    typename tp_ret =
      decltype(dango::declval<tp_value_type>() & dango::declval<tp_value_type>())
  >
  auto and_fetch(value_type)noexcept->
  dango::enable_if
  <
    dango::is_int<dango::underlying_type<tp_value_type>> &&
    dango::is_convertible<tp_ret, value_type>,
    value_type
  >;

  template
  <
    dango::mem_order tp_order = dango::mem_order::seq_cst,
    typename tp_value_type = value_type,
    typename tp_ret =
      decltype(dango::declval<tp_value_type>() | dango::declval<tp_value_type>())
  >
  auto or_fetch(value_type)noexcept->
  dango::enable_if
  <
    dango::is_int<dango::underlying_type<tp_value_type>> &&
    dango::is_convertible<tp_ret, value_type>,
    value_type
  >;

  template
  <
    dango::mem_order tp_order = dango::mem_order::seq_cst,
    typename tp_value_type = value_type,
    typename tp_ret =
      decltype(dango::declval<tp_value_type>() ^ dango::declval<tp_value_type>())
  >
  auto xor_fetch(value_type)noexcept->
  dango::enable_if
  <
    dango::is_int<dango::underlying_type<tp_value_type>> &&
    dango::is_convertible<tp_ret, value_type>,
    value_type
  >;
private:
  value_type m_data;
public:
  DANGO_DELETE_DEFAULT(atomic)
  DANGO_IMMOBILE(atomic)
};

template
<typename tp_type>
constexpr
dango::
atomic<tp_type, DANGO_ATOMIC_ENABLE_SPEC(tp_type)>::
atomic
(value_type const a_data)noexcept:
m_data{ a_data }
{

}

template
<typename tp_type>
constexpr auto
dango::
atomic<tp_type, DANGO_ATOMIC_ENABLE_SPEC(tp_type)>::
address
()noexcept->value_type*
{
  return &m_data;
}

template
<typename tp_type>
constexpr auto
dango::
atomic<tp_type, DANGO_ATOMIC_ENABLE_SPEC(tp_type)>::
address
()const noexcept->value_type const*
{
  return &m_data;
}

template
<typename tp_type>
template
<dango::mem_order tp_order>
auto
dango::
atomic<tp_type, DANGO_ATOMIC_ENABLE_SPEC(tp_type)>::
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
atomic<tp_type, DANGO_ATOMIC_ENABLE_SPEC(tp_type)>::
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
atomic<tp_type, DANGO_ATOMIC_ENABLE_SPEC(tp_type)>::
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
atomic<tp_type, DANGO_ATOMIC_ENABLE_SPEC(tp_type)>::
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

#define DANGO_ATOMIC_DEFINE_FETCH(name) \
template \
<typename tp_type> \
template \
< \
  dango::mem_order tp_order, \
  typename tp_value_type, \
  typename tp_ret \
> \
auto \
dango:: \
atomic<tp_type, DANGO_ATOMIC_ENABLE_SPEC(tp_type)>:: \
name \
(value_type const a_data) \
noexcept-> \
dango::enable_if \
< \
  dango::is_int<dango::underlying_type<tp_value_type>> && \
  dango::is_convertible<tp_ret, value_type>, \
  value_type \
> \
{ \
  static_assert(detail::is_valid_mem_order(tp_order)); \
\
  return __atomic_##name(&m_data, a_data, dango::s_int(tp_order)); \
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

#undef DANGO_ATOMIC_ENABLE_SPEC

/*************************************************************/

#define DANGO_ATOMIC_PTR_ENABLE_SPEC(param) \
dango::enable_if \
< \
  (dango::is_ptr<param> || dango::is_member_ptr<param>) && \
  !dango::is_const<param> && \
  !dango::is_volatile<param> && \
  dango::is_atomic<param> \
>

namespace
dango
{
  template
  <typename tp_type>
  class atomic<tp_type, DANGO_ATOMIC_PTR_ENABLE_SPEC(tp_type)>;
}

template
<typename tp_type>
class
dango::
atomic<tp_type, DANGO_ATOMIC_PTR_ENABLE_SPEC(tp_type)>
final
{
public:
  using value_type = tp_type;
public:
  explicit constexpr atomic(value_type)noexcept;
  ~atomic()noexcept = default;
  constexpr auto address()noexcept->value_type*;
  constexpr auto address()const noexcept->value_type const*;

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
  <
    dango::mem_order tp_order = dango::mem_order::seq_cst,
    typename tp_value_type = value_type,
    typename tp_pointed = dango::remove_ptr<value_type>
  >
  auto fetch_add(dango::ssize)noexcept->
  dango::enable_if
  <
    !dango::is_member_ptr<tp_value_type> &&
    !dango::is_func<tp_pointed>,
    value_type
  >;

  template
  <
    dango::mem_order tp_order = dango::mem_order::seq_cst,
    typename tp_value_type = value_type,
    typename tp_pointed = dango::remove_ptr<value_type>
  >
  auto fetch_sub(dango::ssize)noexcept->
  dango::enable_if
  <
    !dango::is_member_ptr<tp_value_type> &&
    !dango::is_func<tp_pointed>,
    value_type
  >;

  template
  <
    dango::mem_order tp_order = dango::mem_order::seq_cst,
    typename tp_value_type = value_type,
    typename tp_pointed = dango::remove_ptr<value_type>
  >
  auto add_fetch(dango::ssize)noexcept->
  dango::enable_if
  <
    !dango::is_member_ptr<tp_value_type> &&
    !dango::is_func<tp_pointed>,
    value_type
  >;

  template
  <
    dango::mem_order tp_order = dango::mem_order::seq_cst,
    typename tp_value_type = value_type,
    typename tp_pointed = dango::remove_ptr<value_type>
  >
  auto sub_fetch(dango::ssize)noexcept->
  dango::enable_if
  <
    !dango::is_member_ptr<tp_value_type> &&
    !dango::is_func<tp_pointed>,
    value_type
  >;
private:
  value_type m_data;
public:
  DANGO_DELETE_DEFAULT(atomic)
  DANGO_IMMOBILE(atomic)
};

template
<typename tp_type>
constexpr
dango::
atomic<tp_type, DANGO_ATOMIC_PTR_ENABLE_SPEC(tp_type)>::
atomic
(value_type const a_data)noexcept:
m_data{ a_data }
{

}

template
<typename tp_type>
constexpr auto
dango::
atomic<tp_type, DANGO_ATOMIC_PTR_ENABLE_SPEC(tp_type)>::
address
()noexcept->value_type*
{
  return &m_data;
}

template
<typename tp_type>
constexpr auto
dango::
atomic<tp_type, DANGO_ATOMIC_PTR_ENABLE_SPEC(tp_type)>::
address
()const noexcept->value_type const*
{
  return &m_data;
}

template
<typename tp_type>
template
<dango::mem_order tp_order>
auto
dango::
atomic<tp_type, DANGO_ATOMIC_PTR_ENABLE_SPEC(tp_type)>::
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
atomic<tp_type, DANGO_ATOMIC_PTR_ENABLE_SPEC(tp_type)>::
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
atomic<tp_type, DANGO_ATOMIC_PTR_ENABLE_SPEC(tp_type)>::
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
atomic<tp_type, DANGO_ATOMIC_PTR_ENABLE_SPEC(tp_type)>::
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

#define DANGO_ATOMIC_DEFINE_FETCH(name) \
template \
<typename tp_type> \
template \
< \
  dango::mem_order tp_order, \
  typename tp_value_type, \
  typename tp_pointed \
> \
auto \
dango:: \
atomic<tp_type, DANGO_ATOMIC_PTR_ENABLE_SPEC(tp_type)>:: \
name \
(dango::ssize const a_arg)noexcept-> \
dango::enable_if \
< \
  !dango::is_member_ptr<tp_value_type> && \
  !dango::is_func<tp_pointed>, \
  value_type\
> \
{ \
  static_assert(detail::is_valid_mem_order(tp_order)); \
\
  value_type const a_result = \
  __atomic_##name \
  ( \
    &m_data, \
    a_arg * dango::ssize(sizeof(value_type)), \
    dango::s_int(tp_order) \
  ); \
\
  return a_result; \
}

DANGO_ATOMIC_DEFINE_FETCH(fetch_add)
DANGO_ATOMIC_DEFINE_FETCH(fetch_sub)

DANGO_ATOMIC_DEFINE_FETCH(add_fetch)
DANGO_ATOMIC_DEFINE_FETCH(sub_fetch)

#undef DANGO_ATOMIC_DEFINE_FETCH

#undef DANGO_ATOMIC_PTR_ENABLE_SPEC

#endif

