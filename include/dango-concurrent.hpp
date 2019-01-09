#ifndef __DANGO_CONCURRENT_HPP__
#define __DANGO_CONCURRENT_HPP__ 1

#include "dango-macro.hpp"
#include "dango-traits.hpp"
#include "dango-atomic.hpp"

/*** exec_once ***/

namespace
dango
{
  class exec_once;
}

class
dango::
exec_once
final
{
private:
  enum class
  state:
  dango::uint32
  {
    EXECUTED, EXECUTING, INITIAL
  };
public:
  constexpr exec_once()noexcept;

  ~exec_once()noexcept = default;

  template
  <typename tp_func, typename... tp_args>
  auto exec
  (tp_func&&, tp_args&&...)
  noexcept(dango::is_noexcept_callable_ret<tp_func, void, tp_args...>)->
  dango::enable_if<dango::is_callable_ret<tp_func, void, tp_args...>, bool>;

  template
  <typename tp_func, typename... tp_args>
  auto exec
  (tp_func&&, tp_args&&...)
  noexcept(dango::is_noexcept_callable_ret<tp_func, bool, tp_args...>)->
  dango::enable_if<dango::is_callable_ret<tp_func, bool, tp_args...>, bool>;

  auto has_executed()const noexcept->bool;
  void reset()noexcept;
private:
  bool try_acquire()noexcept;
  void release(bool)noexcept;
private:
  dango::atomic<state> m_state;
public:
  DANGO_IMMOBILE(exec_once)
};

constexpr
dango::
exec_once::
exec_once
()noexcept:
m_state{ state::INITIAL }
{

}

inline auto
dango::
exec_once::
try_acquire
()noexcept->bool
{
  constexpr auto const acquire = dango::mem_order::acquire;

  if(m_state.load<acquire>() == state::EXECUTED)
  {
    return false;
  }

  do
  {
    state a_expected = state::INITIAL;

    if(m_state.compare_exchange<acquire, acquire>(a_expected, state::EXECUTING))
    {
      break;
    }

    if(a_expected == state::EXECUTED)
    {
      return false;
    }
  }
  while(true);

  return true;
}

inline void
dango::
exec_once::
release
(bool const a_success)noexcept
{
  constexpr auto const release = dango::mem_order::release;

  m_state.store<release>(a_success ? state::EXECUTED : state::INITIAL);
}

template
<typename tp_func, typename... tp_args>
auto
dango::
exec_once::
exec
(tp_func&& a_func, tp_args&&... a_args)
noexcept(dango::is_noexcept_callable_ret<tp_func, void, tp_args...>)->
dango::enable_if<dango::is_callable_ret<tp_func, void, tp_args...>, bool>
{
  if(!try_acquire())
  {
    return false;
  }

  if constexpr(dango::is_noexcept_callable_ret<tp_func, void, tp_args...>)
  {
    a_func(dango::forward<tp_args>(a_args)...);
  }
  else
  {
    try
    {
      a_func(dango::forward<tp_args>(a_args)...);
    }
    catch(...)
    {
      release(false);

      throw;
    }
  }

  release(true);

  return true;
}

template
<typename tp_func, typename... tp_args>
auto
dango::
exec_once::
exec
(tp_func&& a_func, tp_args&&... a_args)
noexcept(dango::is_noexcept_callable_ret<tp_func, bool, tp_args...>)->
dango::enable_if<dango::is_callable_ret<tp_func, bool, tp_args...>, bool>
{
  if(!try_acquire())
  {
    return false;
  }

  bool a_success;

  if constexpr(dango::is_noexcept_callable_ret<tp_func, bool, tp_args...>)
  {
    a_success = a_func(dango::forward<tp_args>(a_args)...);
  }
  else
  {
    try
    {
      a_success = a_func(dango::forward<tp_args>(a_args)...);
    }
    catch(...)
    {
      release(false);

      throw;
    }
  }

  release(a_success);

  return a_success;
}

inline auto
dango::
exec_once::
has_executed
()const noexcept->bool
{
  return m_state.load<dango::mem_order::acquire>() == state::EXECUTED;
}

inline void
dango::
exec_once::
reset
()noexcept
{
  constexpr auto const acquire = dango::mem_order::acquire;
  constexpr auto const release = dango::mem_order::release;

  if(m_state.load<acquire>() == state::INITIAL)
  {
    return;
  }

  do
  {
    state a_expected = state::EXECUTED;

    if(m_state.compare_exchange<release, acquire>(a_expected, state::INITIAL))
    {
      break;
    }

    if(a_expected == state::INITIAL)
    {
      break;
    }
  }
  while(true);
}

#endif

