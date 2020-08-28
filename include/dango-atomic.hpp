#ifndef DANGO_ATOMIC_HPP_INCLUDED
#define DANGO_ATOMIC_HPP_INCLUDED

namespace
dango
{
  enum class
  memory_order:
  dango::integer::s_int
  {
    relaxed = __ATOMIC_RELAXED,
    acquire = __ATOMIC_ACQUIRE,
    release = __ATOMIC_RELEASE,
    acq_rel = __ATOMIC_ACQ_REL,
    seq_cst = __ATOMIC_SEQ_CST
  };

  namespace
  mem_order
  {
    inline constexpr auto const relaxed = dango::memory_order::relaxed;
    inline constexpr auto const acquire = dango::memory_order::acquire;
    inline constexpr auto const release = dango::memory_order::release;
    inline constexpr auto const acq_rel = dango::memory_order::acq_rel;
    inline constexpr auto const seq_cst = dango::memory_order::seq_cst;
  }
}

namespace
dango::detail
{
  constexpr auto is_valid_mem_order(dango::memory_order)noexcept->bool;
  constexpr auto mem_order_strict_level(dango::memory_order)noexcept->dango::uint;
}

constexpr auto
dango::
detail::
is_valid_mem_order
(dango::memory_order const a_order)noexcept->bool
{
  switch(a_order)
  {
    default: return false;
    case dango::memory_order::relaxed: break;
    case dango::memory_order::acquire: break;
    case dango::memory_order::release: break;
    case dango::memory_order::acq_rel: break;
    case dango::memory_order::seq_cst: break;
  }

  return true;
}

constexpr auto
dango::
detail::
mem_order_strict_level
(dango::memory_order const a_order)noexcept->dango::uint
{
  switch(a_order)
  {
    default: return dango::uint(-1);
    case dango::memory_order::relaxed: return dango::uint(0);
    case dango::memory_order::acquire: break;
    case dango::memory_order::release: break;
    case dango::memory_order::acq_rel: break;
    case dango::memory_order::seq_cst: return dango::uint(2);
  }

  return dango::uint(1);
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
    static_assert(!dango::is_const<tp_type> && !dango::is_volatile<tp_type>);

    tp_type a_temp{ };

    return __atomic_always_lock_free(sizeof(a_temp), &a_temp);
  }
}

namespace
dango
{
  template
  <typename tp_type>
  concept is_atomic =
    dango::is_scalar<tp_type> && dango::detail::is_atomic_help<dango::remove_cv<tp_type>>();
}

static_assert(dango::is_atomic<bool>);
static_assert(dango::is_atomic<char>);
static_assert(dango::is_atomic<wchar_t>);
static_assert(dango::is_atomic<dango::bchar>);
static_assert(dango::is_atomic<dango::wchar>);
static_assert(dango::is_atomic<dango::dchar>);
static_assert(dango::is_atomic<dango::ubyte>);
static_assert(dango::is_atomic<dango::sbyte>);
static_assert(dango::is_atomic<dango::ushort>);
static_assert(dango::is_atomic<dango::sshort>);
static_assert(dango::is_atomic<dango::uint>);
static_assert(dango::is_atomic<dango::sint>);
static_assert(dango::is_atomic<dango::usize>);
static_assert(dango::is_atomic<dango::ssize>);
static_assert(dango::is_atomic<dango::uptr>);
static_assert(dango::is_atomic<dango::sptr>);
static_assert(dango::is_atomic<float>);
static_assert(dango::is_atomic<void*>);
static_assert(dango::is_atomic<void(*)()noexcept>);

/*** standalone functions ***/

namespace
dango
{
  template
  <dango::memory_order tp_order = dango::memory_order::seq_cst, dango::is_atomic tp_type>
  auto
  atomic_load(tp_type const* const a_addr)noexcept->dango::remove_volatile<tp_type>
  {
    using dango::integer::s_int;

    static_assert(detail::is_valid_mem_order(tp_order));

    static_assert
    (
      tp_order == dango::memory_order::relaxed ||
      tp_order == dango::memory_order::acquire ||
      tp_order == dango::memory_order::seq_cst
    );

    dango::remove_volatile<tp_type> a_ret;

    __atomic_load(a_addr, &a_ret, s_int(tp_order));

    return a_ret;
  }

  template
  <dango::memory_order tp_order = dango::memory_order::seq_cst, dango::is_atomic tp_type, typename tp_arg>
  requires
  (
    !dango::is_const<tp_type> &&
    dango::is_brace_constructible<dango::remove_volatile<tp_type>, tp_arg> &&
    dango::is_convertible<tp_arg, dango::remove_volatile<tp_type>>
  )
  void
  atomic_store
  (tp_type* const a_addr, tp_arg&& a_arg)
  noexcept(dango::is_noexcept_brace_constructible<dango::remove_volatile<tp_type>, tp_arg>)
  {
    using dango::integer::s_int;

    static_assert(detail::is_valid_mem_order(tp_order));

    static_assert
    (
      tp_order == dango::memory_order::relaxed ||
      tp_order == dango::memory_order::release ||
      tp_order == dango::memory_order::seq_cst
    );

    dango::remove_volatile<tp_type> a_new_value{ dango::forward<tp_arg>(a_arg) };

    __atomic_store(a_addr, &a_new_value, s_int(tp_order));
  }

  template
  <dango::memory_order tp_order = dango::memory_order::seq_cst, dango::is_atomic tp_type, typename tp_arg>
  requires
  (
    !dango::is_const<tp_type> &&
    dango::is_brace_constructible<dango::remove_volatile<tp_type>, tp_arg> &&
    dango::is_convertible<tp_arg, dango::remove_volatile<tp_type>>
  )
  auto
  atomic_exchange(tp_type* const a_addr, tp_arg&& a_arg)
  noexcept(dango::is_noexcept_brace_constructible<dango::remove_volatile<tp_type>, tp_arg>)->dango::remove_volatile<tp_type>
  {
    using dango::integer::s_int;

    static_assert(detail::is_valid_mem_order(tp_order));

    using type = dango::remove_volatile<tp_type>;

    type a_new_value{ dango::forward<tp_arg>(a_arg) };

    type a_ret;

    __atomic_exchange(a_addr, &a_new_value, &a_ret, s_int(tp_order));

    return a_ret;
  }

  template
  <
    dango::memory_order tp_success = dango::memory_order::seq_cst,
    dango::memory_order tp_failure = dango::memory_order::seq_cst,
    bool tp_weak = false,
    dango::is_atomic tp_type,
    typename tp_expected_type,
    typename tp_arg
  >
  requires
  (
    !dango::is_const<tp_type> && !is_const<tp_expected_type> &&
    dango::is_same<dango::remove_volatile<tp_type>, dango::remove_volatile<tp_expected_type>> &&
    dango::is_brace_constructible<dango::remove_volatile<tp_type>, tp_arg> &&
    dango::is_convertible<tp_arg, dango::remove_volatile<tp_type>>
  )
  auto
  atomic_compare_exchange
  (tp_type* const a_addr, tp_expected_type* const a_expected, tp_arg&& a_arg)
  noexcept(dango::is_noexcept_brace_constructible<dango::remove_volatile<tp_type>, tp_arg>)->bool
  {
    using dango::integer::s_int;

    static_assert(detail::is_valid_mem_order(tp_success));
    static_assert(detail::is_valid_mem_order(tp_failure));

    static_assert
    (
      tp_failure == dango::memory_order::relaxed ||
      tp_failure == dango::memory_order::acquire ||
      tp_failure == dango::memory_order::seq_cst
    );

    static_assert(dango::detail::mem_order_strict_level(tp_failure) <= dango::detail::mem_order_strict_level(tp_success));

    dango::remove_volatile<tp_type> a_new_value{ dango::forward<tp_arg>(a_arg) };

    bool const a_result =
    __atomic_compare_exchange
    (
      a_addr,
      a_expected,
      &a_new_value,
      tp_weak,
      s_int(tp_success),
      s_int(tp_failure)
    );

    return a_result;
  }
}

#define DANGO_ATOMIC_DEFINE_FETCH(name) \
template \
<dango::memory_order tp_order = dango::memory_order::seq_cst, dango::is_atomic tp_type, typename tp_arg> \
requires \
( \
  !dango::is_const<tp_type> && dango::is_int<tp_type> && \
  dango::is_brace_constructible<dango::remove_volatile<tp_type>, tp_arg> && \
  dango::is_convertible<tp_arg, dango::remove_volatile<tp_type>> \
) \
auto \
atomic_##name \
(tp_type* const a_addr, tp_arg&& a_arg) \
noexcept(dango::is_noexcept_brace_constructible<dango::remove_volatile<tp_type>, tp_arg>)->dango::remove_volatile<tp_type> \
{ \
  static_assert(detail::is_valid_mem_order(tp_order)); \
  using type = dango::remove_volatile<tp_type>; \
  return __atomic_##name(a_addr, type{ dango::forward<tp_arg>(a_arg) }, dango::integer::s_int(tp_order)); \
}

namespace
dango
{
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
}

#undef DANGO_ATOMIC_DEFINE_FETCH

#define DANGO_ATOMIC_DEFINE_FETCH(name) \
template \
<dango::memory_order tp_order = dango::memory_order::seq_cst, dango::is_atomic tp_type> \
requires(!dango::is_const<tp_type> && dango::is_object_ptr<tp_type>) \
auto \
atomic_##name \
(tp_type* const a_addr, dango::ssize const a_arg)noexcept->dango::remove_volatile<tp_type> \
{ \
  static_assert(detail::is_valid_mem_order(tp_order)); \
  auto const a_val = a_arg * dango::ssize(sizeof(dango::remove_ptr<tp_type>)); \
  return __atomic_##name(a_addr, a_val, dango::integer::s_int(tp_order)); \
}

namespace
dango
{
  DANGO_ATOMIC_DEFINE_FETCH(fetch_add)
  DANGO_ATOMIC_DEFINE_FETCH(fetch_sub)

  DANGO_ATOMIC_DEFINE_FETCH(add_fetch)
  DANGO_ATOMIC_DEFINE_FETCH(sub_fetch)
}

#undef DANGO_ATOMIC_DEFINE_FETCH

/*** atomic ***/

namespace
dango
{
  template
  <typename tp_type>
  concept atomic_constraint_spec =
    !dango::is_const_or_volatile<tp_type> && dango::is_atomic<tp_type>;

  template
  <dango::atomic_constraint_spec tp_type>
  class atomic;
}

template
<dango::atomic_constraint_spec tp_type>
class
dango::
atomic
final
{
public:
  using value_type = tp_type;
public:
  explicit constexpr atomic(value_type)noexcept;
  constexpr ~atomic()noexcept = default;
  template
  <dango::memory_order tp_order = dango::memory_order::seq_cst>
  auto load()const noexcept->value_type;
  template
  <dango::memory_order tp_order = dango::memory_order::seq_cst>
  void store(value_type)noexcept;
  template
  <dango::memory_order tp_order = dango::memory_order::seq_cst>
  auto exchange(value_type)noexcept->value_type;
  template
  <dango::memory_order tp_success = dango::memory_order::seq_cst, dango::memory_order tp_failure = dango::memory_order::seq_cst, bool tp_weak = false>
  auto compare_exchange(value_type&, value_type)noexcept->bool;

#define DANGO_ATOMIC_DEFINE_FETCH(name) \
template \
<dango::memory_order tp_order = dango::memory_order::seq_cst> \
requires(dango::is_int<value_type>) \
auto name(value_type const a_data)noexcept->value_type \
{ \
  return dango::atomic_##name<tp_order>(&m_data, a_data); \
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

#define DANGO_ATOMIC_DEFINE_FETCH(name) \
template \
<dango::memory_order tp_order = dango::memory_order::seq_cst> \
requires(dango::is_object_ptr<value_type>) \
auto name(dango::ssize const a_data)noexcept->value_type \
{ \
  return dango::atomic_##name<tp_order>(&m_data, a_data); \
}

  DANGO_ATOMIC_DEFINE_FETCH(fetch_add)
  DANGO_ATOMIC_DEFINE_FETCH(fetch_sub)

  DANGO_ATOMIC_DEFINE_FETCH(add_fetch)
  DANGO_ATOMIC_DEFINE_FETCH(sub_fetch)

#undef DANGO_ATOMIC_DEFINE_FETCH

private:
  value_type m_data;
public:
  DANGO_DELETE_DEFAULT(atomic)
  DANGO_UNMOVEABLE(atomic)
};

template
<dango::atomic_constraint_spec tp_type>
constexpr
dango::
atomic<tp_type>::
atomic
(value_type const a_data)noexcept:
m_data{ a_data }
{

}

template
<dango::atomic_constraint_spec tp_type>
template
<dango::memory_order tp_order>
auto
dango::
atomic<tp_type>::
load
()const noexcept->value_type
{
  return dango::atomic_load<tp_order>(&m_data);
}

template
<dango::atomic_constraint_spec tp_type>
template
<dango::memory_order tp_order>
void
dango::
atomic<tp_type>::
store
(value_type const a_data)noexcept
{
  dango::atomic_store<tp_order>(&m_data, a_data);
}

template
<dango::atomic_constraint_spec tp_type>
template
<dango::memory_order tp_order>
auto
dango::
atomic<tp_type>::
exchange
(value_type const a_data)noexcept->value_type
{
  return dango::atomic_exchange<tp_order>(&m_data, a_data);
}

template
<dango::atomic_constraint_spec tp_type>
template
<dango::memory_order tp_success, dango::memory_order tp_failure, bool tp_weak>
auto
dango::
atomic<tp_type>::
compare_exchange
(value_type& a_expected, value_type const a_data)noexcept->bool
{
  return dango::atomic_compare_exchange<tp_success, tp_failure, tp_weak>(&m_data, &a_expected, a_data);
}

#endif // DANGO_ATOMIC_HPP_INCLUDED
