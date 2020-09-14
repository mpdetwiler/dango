#include "dango-exception.hpp"

auto
dango::
exception::
what()const noexcept->char const*
{
  return dango::bchar_as_char(message());
}

auto
dango::
exception::
message()const noexcept->dango::bchar const*
{
  return u8"dango::exception";
}
