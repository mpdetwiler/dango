#include "dango.hpp"

void
func
()noexcept
{
  dango::exec_once a_exec;

  a_exec.exec([]()noexcept->void{ });
}
