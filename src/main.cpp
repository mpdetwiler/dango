#include "dango-def.hpp"
#include "dango-int.hpp"
#include "dango-macro.hpp"
#include "dango-traits.hpp"
#include "dango-util.hpp"
#include "dango-assert.hpp"
#include "dango-atomic.hpp"
#include "dango-mem.hpp"

void func()noexcept;

template
<
  typename... tp_bool,
  dango::enable_if<(true && ... && dango::is_same<dango::remove_cv<tp_bool>, bool>)> = dango::enable_val
>
constexpr auto
conjunction
(tp_bool const... args)noexcept->bool
{
  return (true && ... && args);
}

static_assert(conjunction(true, true, true, true));

#include <cstdio>

struct ptr_holder
{
private:
  int* const m_ptr;
public:
  constexpr ptr_holder(int* const a_ptr)noexcept:m_ptr{ a_ptr }{ }
  constexpr operator int*()const noexcept{ return m_ptr; }
};

void conversion(ptr_holder)noexcept;

auto
main
()noexcept->dango::s_int
{
  printf("%lli\n", dango::s_cent(dango::integer::MIN_VAL<sint8>));
  printf("%lli\n", dango::s_cent(dango::integer::MIN_VAL<sint16>));
  printf("%lli\n", dango::s_cent(dango::integer::MIN_VAL<sint32>));
  printf("%lli\n", dango::s_cent(dango::integer::MIN_VAL<sint64>));

  printf
  (
    "%s %s %u\n",
    dango::source_location::current().file(),
    dango::source_location::current().function(),
    dango::source_location::current().line()
  );

  func();

  dango::atomic<int> a_test{ 5 };

  a_test.load<dango::mem_order::acquire>();
  a_test.sub_fetch(4);

  int a_x[2];

  dango::atomic<int const*> a_ax{ &a_x[0] };

  a_ax.fetch_add<dango::mem_order::acq_rel>(1);

  dango_assert(a_ax.load() == &a_x[1]);

  dango::atomic<void(*)()noexcept> a_afunc{ &func };

  dango::set_assert_log_handler(dango::detail::default_assert_log_handler);

  dango_assert(true);

  dango::param_ptr<void, true> a_param = a_x;

  dango::param_ptr<void const, true> a_param2 = a_param;

  dango::mem_copy(a_x, a_x, sizeof(a_x));

  dango::param_ptr<void()noexcept(true)> a_param_func = func;

  dango::param_ptr<void()noexcept(false)> a_param_func2 = a_param_func;

  a_param_func2.get();

  ptr_holder a_holder = nullptr;

  dango::param_ptr<int> a_param_h = a_holder;

  dango_assert(!a_param_h);

  return 0;
}



template class dango::atomic<int(*)(int)noexcept>;

template class dango::atomic<bool>;
template auto dango::atomic<bool>::load<dango::mem_order::seq_cst>()const noexcept->bool;
template auto dango::atomic<bool>::store<dango::mem_order::seq_cst>(bool)noexcept->void;

static_assert(dango::atomic<bool>::is_lock_free());

