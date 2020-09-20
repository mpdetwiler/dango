#ifndef DANGO_GLOBAL_HPP_INCLUDED
#define DANGO_GLOBAL_HPP_INCLUDED

#include "dango-concurrent.hpp"
#include "dango-mem.hpp"

/*** globals ***/

namespace
dango::detail
{
  template
  <typename tp_type>
  concept global_storage_constraint_spec =
    dango::is_object_exclude_array<tp_type> && dango::is_noexcept_destructible<tp_type>;

  template
  <typename tp_type>
  using global_storage_construct_ref = dango::remove_cv<tp_type>(&)()noexcept;

  template
  <dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
  class global_storage;

  template
  <typename tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
  using global_storage_ref = dango::detail::global_storage<tp_type, tp_construct>&;
}

template
<dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
class alignas(dango::cache_align_type)
dango::
detail::
global_storage
final
{
public:
  template
  <dango::detail::global_storage_ref<tp_type, tp_construct> tp_storage>
  class strong_incrementer;

  template
  <dango::detail::global_storage_ref<tp_type, tp_construct> tp_storage>
  class weak_incrementer;
public:
  explicit constexpr global_storage()noexcept;
  constexpr ~global_storage()noexcept = default;
private:
  auto get()const noexcept->tp_type*;
  void increment()noexcept;
  auto try_increment()noexcept->bool;
  void decrement()noexcept;
private:
  tp_type* m_ptr;
  dango::aligned_union<tp_type, dango::cache_align_type> m_storage;
  dango::exec_once m_init;
  DANGO_CACHE_LINE_START
  dango::spin_mutex m_lock;
  dango::usize m_ref_count;
private:
  DANGO_UNMOVEABLE(global_storage)
};

/*** strong_incrementer ***/

template
<dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
template
<dango::detail::global_storage_ref<tp_type, tp_construct> tp_storage>
class
dango::
detail::
global_storage<tp_type, tp_construct>::
strong_incrementer
final
{
public:
  explicit strong_incrementer()noexcept{ tp_storage.increment(); }
  ~strong_incrementer()noexcept{ tp_storage.decrement(); }
public:
  DANGO_UNMOVEABLE(strong_incrementer)
};

/*** weak_incrementer ***/

template
<dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
template
<dango::detail::global_storage_ref<tp_type, tp_construct> tp_storage>
class
dango::
detail::
global_storage<tp_type, tp_construct>::
weak_incrementer
final
{
public:
  explicit
  weak_incrementer(DANGO_SRC_LOC_ARG(a_loc))noexcept
  {
    if(tp_storage.try_increment())
    {
      return;
    }

    dango_unreachable_terminate_msg_loc(u8"attempt to access already-destroyed global", a_loc);
  }

  ~weak_incrementer()noexcept{ tp_storage.decrement(); }
public:
  auto get()const noexcept->tp_type*{ return tp_storage.get(); }
  auto begin()const noexcept->tp_type*{ return get(); }
  auto end()const noexcept->void const volatile*{ return get() + dango::usize(1); }
  auto operator -> ()const noexcept->tp_type*{ return get(); }
  auto operator * ()const noexcept->tp_type&{ return *get(); }
public:
  DANGO_UNMOVEABLE(weak_incrementer)
};

/*** global_storage ***/

template
<dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
constexpr
dango::
detail::
global_storage<tp_type, tp_construct>::
global_storage
()noexcept:
m_ptr{ dango::null },
m_storage{ },
m_init{ },
m_lock{ },
m_ref_count{ dango::usize(0) }
{

}

template
<dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
auto
dango::
detail::
global_storage<tp_type, tp_construct>::
get
()const noexcept->tp_type*
{
  return m_ptr;
}

template
<dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
void
dango::
detail::
global_storage<tp_type, tp_construct>::
increment
()noexcept
{
  bool const a_init =
  m_init.exec
  (
    [this]()noexcept->void
    {
      m_ptr = dango_placement_new(m_storage.get(), tp_type, { tp_construct() });

      m_ref_count = dango::usize(1);
    }
  );

  if(a_init)
  {
    return;
  }

  dango_crit(m_lock)
  {
    if(m_ref_count != dango::usize(0))
    {
      ++m_ref_count;

      return;
    }
  }

  dango_unreachable_terminate_msg(u8"attempt to initialize already-destroyed global");
}

template
<dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
auto
dango::
detail::
global_storage<tp_type, tp_construct>::
try_increment
()noexcept->bool
{
  dango_crit(m_lock)
  {
    if(m_ref_count == dango::usize(0))
    {
      return false;
    }

    ++m_ref_count;
  }

  return true;
}

template
<dango::detail::global_storage_constraint_spec tp_type, dango::detail::global_storage_construct_ref<tp_type> tp_construct>
void
dango::
detail::
global_storage<tp_type, tp_construct>::
decrement
()noexcept
{
  dango_crit(m_lock)
  {
    dango_assert(m_ref_count != dango::usize(0));

    --m_ref_count;

    if(m_ref_count != dango::usize(0))
    {
      return;
    }
  }

  auto const a_ptr = get();

  m_ptr = dango::null;

  dango::destructor_as<dango::remove_cv<tp_type>>(a_ptr);
}

/*** inline global test ***/

#ifndef DANGO_NO_DEBUG
namespace
dango::detail::test
{
  DANGO_DEFINE_GLOBAL_INLINE(dango::ulong, inline_global_address_test, { 10 })

  DANGO_EXPORT auto inline_global_address_extern()noexcept->void const*;

#ifndef DANGO_BUILDING_LIB
  inline auto
  inline_global_address_inline()noexcept->void const*
  {
    return dango::addressof(*dango::detail::test::inline_global_address_test());
  }
#endif
}
#endif

#endif // DANGO_GLOBAL_HPP_INCLUDED
