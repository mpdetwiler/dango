#include "dango-int.hpp"
#include "dango-mem.hpp"
#include "dango-concurrent.hpp"

void
func
()noexcept
{
  dango::exec_once a_exec;

  a_exec.exec([]()noexcept->void{ });
}
