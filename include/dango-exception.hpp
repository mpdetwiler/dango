#ifndef DANGO_EXCEPTION_HPP_INCLUDED
#define DANGO_EXCEPTION_HPP_INCLUDED

#include "dango-assert.hpp"

namespace
dango
{
  using std_exception  = std::exception;
  using std_bad_alloc  = std::bad_alloc;
  using std_bad_typeid = std::bad_typeid;
  using std_bad_cast   = std::bad_cast;
}

namespace
dango
{
  class exception;
}

class DANGO_EXPORT
dango::
exception:
public dango::std_exception
{
public:
  using super_type = dango::std_exception;
public:
  explicit
  exception
  (dango::source_location const& a_location = dango::source_location::current())noexcept:
  super_type{ },
  m_location{ a_location }
  { }
  ~exception()noexcept override = default;
public:
  auto what()const noexcept->char const* final;
public:
  virtual auto message()const noexcept->dango::bchar const*;
public:
  auto file()const noexcept->dango::bchar const*{ return m_location.file(); }
  auto function()const noexcept->dango::bchar const*{ return m_location.function(); }
  auto line()const noexcept->dango::uint{ return m_location.line(); }
private:
  dango::source_location const m_location;
public:
  DANGO_UNMOVEABLE(exception)
};

#endif
