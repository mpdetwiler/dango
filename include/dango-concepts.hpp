#ifndef DANGO_CONCEPTS_HPP_INCLUDED
#define DANGO_CONCEPTS_HPP_INCLUDED

#include "dango-util-base.hpp"

/*** is_nullable ***/

namespace
dango
{
  template
  <typename tp_type>
  concept is_nullable =
    dango::is_object_exclude_array<tp_type> &&
    dango::is_noexcept_null_equatable<dango::remove_cv<tp_type> const&> && dango::is_noexcept_null_equatable<dango::remove_cv<tp_type>&&> &&
    dango::is_noexcept_constructible<dango::remove_cv<tp_type>, dango::null_tag const&> && dango::is_noexcept_constructible<dango::remove_cv<tp_type>, dango::null_tag&&> &&
    dango::is_noexcept_convertible<dango::null_tag const&, dango::remove_cv<tp_type>> && dango::is_noexcept_convertible<dango::null_tag&&, dango::remove_cv<tp_type>> &&
    dango::is_noexcept_assignable<dango::remove_cv<tp_type>&, dango::null_tag const&> && dango::is_noexcept_assignable<dango::remove_cv<tp_type>&, dango::null_tag&&>;
}

#endif
