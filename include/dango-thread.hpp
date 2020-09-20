#ifndef DANGO_THREAD_HPP_INCLUDED
#define DANGO_THREAD_HPP_INCLUDED

#include "dango-concurrent.hpp"
#include "dango-exception.hpp"
#include "dango-global.hpp"
#include "dango-hash.hpp"

/*** thread ***/

namespace
dango
{
  class thread;

  enum class
  thread_id:
  dango::uptr
  {
    none = dango::ptr_as_uint(dango::null)
  };
}

class
dango::
thread
final
{
private:
  using thread_start_func = void(*)(void*)noexcept(false);
  struct construct_tag;
  class control_block;
  class registry;
  class notifier;

  template
  <typename tp_func, typename... tp_args>
  class runnable;

  template
  <typename tp_func>
  class runnable<tp_func>;
private:
  static auto control_block_new(bool)noexcept->control_block*;
  DANGO_EXPORT static void control_block_inc(control_block&)noexcept;
  DANGO_EXPORT static void control_block_dec(control_block&)noexcept;
  DANGO_EXPORT static auto thread_self_init(bool)noexcept->dango::thread const*;
  DANGO_EXPORT static void start_thread(thread_start_func, void*)noexcept(false);

  template
  <typename tp_func>
  static void thread_start_address(void*)noexcept(false);

  static registry s_registry;
public:
  static void yield()noexcept;
  static auto self()noexcept->thread const&;
  DANGO_EXPORT static auto self_id()noexcept->dango::thread_id;
  DANGO_EXPORT static void main_join()noexcept;
  [[nodiscard]] static auto main_join_finally()noexcept;

  template
  <typename tp_func, typename... tp_args>
  requires
  (
    (dango::is_noexcept_destructible<dango::decay<tp_func>> && ... && dango::is_noexcept_destructible<dango::decay<tp_args>>) &&
    (dango::in_exception_safe_order<tp_func, tp_args...> || (dango::is_copy_constructible<dango::decay<tp_func>> && ... && dango::is_copy_constructible<dango::decay<tp_args>>)) &&
    dango::is_callable_ret<void, dango::decay<tp_func>&, dango::tuple_get_type<dango::tuple_model&, dango::decay<tp_args>>...>
  )
  static auto
  create(bool, tp_func&&, tp_args&&...)noexcept(false)->dango::thread;

  template
  <typename tp_func, typename... tp_args>
  requires(requires{ { dango::thread::create(false, dango::declval<tp_func>(), dango::declval<tp_args>()...) }->dango::is_same<dango::thread>; })
  static auto
  create(tp_func&& a_thread_func, tp_args&&... a_thread_args)noexcept(false)->dango::thread
  {
    return dango::thread::create(false, dango::forward<tp_func>(a_thread_func), dango::forward<tp_args>(a_thread_args)...);
  }

  template
  <typename tp_func, typename... tp_args>
  requires(requires{ { dango::thread::create(true, dango::declval<tp_func>(), dango::declval<tp_args>()...) }->dango::is_same<dango::thread>; })
  static auto
  create_daemon(tp_func&& a_thread_func, tp_args&&... a_thread_args)noexcept(false)->dango::thread
  {
    return dango::thread::create(true, dango::forward<tp_func>(a_thread_func), dango::forward<tp_args>(a_thread_args)...);
  }

  DANGO_EXPORT static void sleep(dango::timeout const&)noexcept;
  static void sleep_rel(dango::tick_count_type)noexcept;
  static void sleep_rel(dango::tick_count_type, dango::timeout_flag)noexcept;
private:
  explicit thread(construct_tag, bool)noexcept;
  explicit thread(control_block*)noexcept;
public:
  explicit constexpr thread()noexcept;
  constexpr thread(dango::null_tag)noexcept;
  constexpr thread(thread const&)noexcept;
  constexpr thread(thread&&)noexcept;
  constexpr ~thread()noexcept;
  DANGO_DEFINE_NULL_SWAP_ASSIGN(thread, constexpr, true)
  DANGO_DEFINE_COPY_SWAP_ASSIGN(thread, constexpr, true)
  DANGO_DEFINE_MOVE_SWAP_ASSIGN(thread, constexpr, true)
  DANGO_DEFINE_NULLABLE_OPERATOR_BOOL(constexpr, true)
  constexpr auto dango_operator_is_null()const noexcept->bool{ return dango::is_null(m_control); }
  constexpr auto dango_operator_equals(thread const& a_arg)const noexcept->bool{ return dango::equals(m_control, a_arg.m_control); }
  constexpr auto dango_operator_compare(thread const& a_arg)const noexcept->auto{ return dango::compare(m_control, a_arg.m_control); }
  auto dango_operator_hash()const noexcept->dango::hash_val{ return dango::hash(m_control); }
  constexpr void dango_operator_swap(thread& a_arg)& noexcept{ dango::swap(m_control, a_arg.m_control); }
  DANGO_EXPORT auto is_alive()const noexcept->bool;
  DANGO_EXPORT void join()const noexcept;
  DANGO_EXPORT void join(dango::timeout const&)const noexcept;
  DANGO_EXPORT auto is_daemon()const noexcept->bool;
  DANGO_EXPORT auto get_id()const noexcept->dango::thread_id;
private:
  control_block* m_control;
};

/*** runnable ***/

template
<typename tp_func, typename... tp_args>
class alignas(dango::cache_align_type)
dango::
thread::
runnable
final
{
public:
  DANGO_DEFINE_CLASS_OPERATOR_NEW_DELETE(runnable)
public:
  template
  <typename tp_tp_func, typename... tp_tp_args>
  explicit constexpr
  runnable
  (tp_tp_func&& a_func, tp_tp_args&&... a_args)noexcept(false):
  m_func{ dango::forward<tp_tp_func>(a_func) },
  m_args{ dango::forward<tp_tp_args>(a_args)... }
  {

  }

  constexpr ~runnable()noexcept = default;

  constexpr void
  run()noexcept(false)
  {
    m_args->*m_func;
  }
private:
  tp_func m_func;
  dango::tuple<tp_args...> m_args;
public:
  DANGO_DELETE_DEFAULT(runnable)
  DANGO_UNMOVEABLE(runnable)
};

template
<typename tp_func>
class alignas(dango::cache_align_type)
dango::
thread::
runnable<tp_func>
final
{
public:
  DANGO_DEFINE_CLASS_OPERATOR_NEW_DELETE(runnable)
public:
  template
  <typename tp_tp_func>
  explicit constexpr
  runnable
  (tp_tp_func&& a_func)noexcept(false):
  m_func{ dango::forward<tp_tp_func>(a_func) }
  {

  }

  constexpr ~runnable()noexcept = default;

  constexpr void
  run()noexcept(false)
  {
    m_func();
  }
private:
  tp_func m_func;
public:
  DANGO_DELETE_DEFAULT(runnable)
  DANGO_UNMOVEABLE(runnable)
};

/*** thread ***/

inline void
dango::
thread::
yield
()noexcept
{
  dango::thread_yield_soft();
}

inline auto
dango::
thread::
self
()noexcept->thread const&
{
  auto const a_self = thread_self_init(true);

  dango_assert_nonnull(a_self);

  return *a_self;
}

inline auto
dango::
thread::
main_join_finally
()noexcept->auto
{
  return dango::make_finally(dango::thread::main_join);
}

inline void
dango::
thread::
sleep_rel
(dango::tick_count_type const a_rel)noexcept
{
  auto const a_timeout = dango::timeout::make_rel(a_rel);

  thread::sleep(a_timeout);
}

inline void
dango::
thread::
sleep_rel
(dango::tick_count_type const a_rel, dango::timeout_flag const a_flags)noexcept
{
  auto const a_timeout = dango::timeout::make_rel(a_rel, a_flags);

  thread::sleep(a_timeout);
}

constexpr
dango::
thread::
thread
()noexcept:
thread{ dango::null }
{

}

constexpr
dango::
thread::
thread
(dango::null_tag const)noexcept:
m_control{ dango::null }
{

}

constexpr
dango::
thread::
thread
(thread const& a_arg)noexcept:
m_control{ a_arg.m_control }
{
  if(m_control)
  {
    control_block_inc(*m_control);
  }
}

constexpr
dango::
thread::
thread
(thread&& a_arg)noexcept:
m_control{ dango::exchange(a_arg.m_control, dango::null) }
{

}

constexpr
dango::
thread::
~thread
()noexcept
{
  if(m_control)
  {
    control_block_dec(*m_control);
  }
}

template
<typename tp_func, typename... tp_args>
requires
(
  (dango::is_noexcept_destructible<dango::decay<tp_func>> && ... && dango::is_noexcept_destructible<dango::decay<tp_args>>) &&
  (dango::in_exception_safe_order<tp_func, tp_args...> || (dango::is_copy_constructible<dango::decay<tp_func>> && ... && dango::is_copy_constructible<dango::decay<tp_args>>)) &&
  dango::is_callable_ret<void, dango::decay<tp_func>&, dango::tuple_get_type<dango::tuple_model&, dango::decay<tp_args>>...>
)
auto
dango::
thread::
create
(bool const a_daemon, tp_func&& a_thread_func, tp_args&&... a_thread_args)noexcept(false)->dango::thread
{
  using dango::mem_order::acquire;
  using dango::mem_order::release;

  using run_type = runnable<dango::decay<tp_func>, dango::decay<tp_args>...>;

  auto const a_runnable =
  [](auto& a_func, auto&... a_args)noexcept(false)->run_type*
  {
    if constexpr(dango::in_exception_safe_order<tp_func, tp_args...>)
    {
      return new run_type{ dango::forward_if_noexcept<tp_func>(a_func), dango::forward_if_noexcept<tp_args>(a_args)... };
    }
    else
    {
      return new run_type{ dango::as_const(a_func), dango::as_const(a_args)... };
    }
  }(a_thread_func, a_thread_args...);

  auto a_guard =
  dango::make_guard
  (
    [a_runnable]()noexcept->void
    {
      delete a_runnable;
    }
  );

  DANGO_CACHE_LINE_START
  dango::thread a_ret{ dango::null };
  DANGO_CACHE_LINE_START
  dango::atomic<bool> a_starting{ true };

  auto a_func =
  [
    a_daemon,
    a_runnable,
    &a_ret,
    &a_starting
  ]
  ()noexcept(false)->void
  {
    a_ret = dango::thread{ *thread_self_init(a_daemon) };

    a_starting.store<release>(false);

    auto const a_fin =
    dango::make_finally
    (
      [a_runnable]()noexcept->void
      {
        delete a_runnable;
      }
    );

    a_runnable->run();
  };

  start_thread(&thread_start_address<decltype(a_func)>, &a_func);

  dango::busy_wait_while
  ([&a_starting]()noexcept->bool{ return a_starting.load<acquire>(); }, dango::uint(128));

  a_guard.dismiss();

  return a_ret;
}

template
<typename tp_func>
void
dango::
thread::
thread_start_address
(void* const a_data)noexcept(false)
{
  static_assert(!dango::is_ref<tp_func>);
  static_assert(!dango::is_const<tp_func> && !dango::is_volatile<tp_func>);
  static_assert(dango::is_noexcept_move_constructible<tp_func>);
  static_assert(dango::is_callable_ret<void, tp_func const&>);
  static_assert(dango::is_noexcept_destructible<tp_func>);

  auto const a_func_ptr = static_cast<tp_func*>(a_data);

  tp_func const a_func = dango::move(*a_func_ptr);

  a_func();
}

/*** bad_thread ***/

namespace
dango
{
  class bad_thread;
}

class DANGO_EXPORT
dango::
bad_thread
final:
public dango::exception
{
public:
  using super_type = dango::exception;
public:
  explicit
  bad_thread
  (dango::source_location const& a_location = dango::source_location::current())noexcept:
  super_type{ a_location }
  { }

  ~bad_thread()noexcept override = default;
public:
  auto message()const noexcept->dango::bchar const* override;
public:
  DANGO_UNMOVEABLE(bad_thread)
};

/*** init thread self of main thread ***/

#ifndef DANGO_BUILDING_LIB
namespace
dango::detail
{
  [[maybe_unused]] inline bool const
  s_main_thread_init_bool =
    (void(dango::thread::self()), false);
}
#endif

/*** cond_var_registry_thread ***/

namespace
dango::detail
{
  class cond_var_registry_thread;
}

class
dango::
detail::
cond_var_registry_thread
final
{
private:
  DANGO_EXPORT static auto start_thread()noexcept->dango::thread;
  DANGO_EXPORT static void stop_thread(dango::thread const&)noexcept;
public:
  explicit
  cond_var_registry_thread()noexcept:
  m_thread{ start_thread() }
  { }
  ~cond_var_registry_thread()noexcept
  {
    stop_thread(m_thread);
  }
private:
  dango::thread const m_thread;
public:
  DANGO_UNMOVEABLE(cond_var_registry_thread)
};

namespace
dango::detail
{
  DANGO_DECLARE_GLOBAL_EXTERN(dango::detail::cond_var_registry_thread const, s_cond_var_registry_thread)
}

/*** windows_timer_res_daemon ***/

#ifdef DANGO_PLATFORM_WINDOWS

namespace
dango::detail
{
  class windows_timer_res_daemon;
}

class
dango::
detail::
windows_timer_res_daemon
final
{
private:
  DANGO_EXPORT static auto start_thread()noexcept->dango::thread;
  DANGO_EXPORT static void stop_thread(dango::thread const&)noexcept;
public:
  explicit
  windows_timer_res_daemon()noexcept:
  m_thread{ start_thread() }
  { }
  ~windows_timer_res_daemon()noexcept
  {
    stop_thread(m_thread);
  }
private:
  dango::thread const m_thread;
public:
  DANGO_UNMOVEABLE(windows_timer_res_daemon)
};

namespace
dango::detail
{
  DANGO_DECLARE_GLOBAL_EXTERN(dango::detail::windows_timer_res_daemon const, s_windows_timer_res_daemon)
}

#endif // DANGO_PLATFORM_WINDOWS

#endif // DANGO_THREAD_HPP_INCLUDED
