#include "dango-mutex.hpp"
#include "dango-concurrent-private.hpp"

/*** platform specific ***/

#ifdef DANGO_PLATFORM_LINUX_OR_APPLE

void
dango::
detail::
mutex_control::
initialize
()noexcept
{
  dango::detail::pthread_mutex_init(storage());
}

void
dango::
detail::
mutex_control::
destroy
()noexcept
{
  dango::detail::pthread_mutex_destroy(storage());
}

auto
dango::
detail::
mutex_control::
acquire
()noexcept->mutex_control*
{
  dango_assert_noassume(m_init.has_executed());

  dango::detail::pthread_mutex_lock(storage());

  return this;
}

auto
dango::
detail::
mutex_control::
try_acquire
()noexcept->mutex_control*
{
  dango_assert_noassume(m_init.has_executed());

  if(dango::detail::pthread_mutex_try_lock(storage()))
  {
    return this;
  }

  return dango::null;
}

void
dango::
detail::
mutex_control::
release
()noexcept
{
  dango_assert_noassume(m_init.has_executed());

  dango::detail::pthread_mutex_unlock(storage());
}

#endif // DANGO_PLATFORM_LINUX_OR_APPLE

#ifdef DANGO_PLATFORM_WINDOWS

void
dango::
detail::
mutex_control::
initialize
()noexcept
{
  dango::detail::srw_lock_init(storage());
}

void
dango::
detail::
mutex_control::
destroy
()noexcept
{
  dango::detail::srw_lock_destroy(storage());
}

auto
dango::
detail::
mutex_control::
acquire
()noexcept->mutex_control*
{
  dango_assert_noassume(m_init.has_executed());

  dango::detail::srw_lock_acquire(storage());

  return this;
}

auto
dango::
detail::
mutex_control::
try_acquire
()noexcept->mutex_control*
{
  dango_assert_noassume(m_init.has_executed());

  if(dango::detail::srw_lock_try_acquire(storage()))
  {
    return this;
  }

  return dango::null;
}

void
dango::
detail::
mutex_control::
release
()noexcept
{
  dango_assert_noassume(m_init.has_executed());

  dango::detail::srw_lock_release(storage());
}

#endif // DANGO_PLATFORM_WINDOWS
