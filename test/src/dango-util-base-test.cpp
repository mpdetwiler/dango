#include "dango-util-base.hpp"

static_assert(dango::is_convertible<dango::compare_val_strong, std::strong_ordering>);
static_assert(dango::is_convertible<dango::compare_val_strong, std::weak_ordering>);
static_assert(dango::is_convertible<dango::compare_val_strong, std::partial_ordering>);
static_assert(dango::is_convertible<dango::compare_val_strong, dango::compare_val_strong>);
static_assert(dango::is_convertible<dango::compare_val_strong, dango::compare_val_weak>);
static_assert(dango::is_convertible<dango::compare_val_strong, dango::compare_val_partial>);

static_assert(!dango::is_convertible<dango::compare_val_weak, std::strong_ordering>);
static_assert(dango::is_convertible<dango::compare_val_weak, std::weak_ordering>);
static_assert(dango::is_convertible<dango::compare_val_weak, std::partial_ordering>);
static_assert(!dango::is_convertible<dango::compare_val_weak, dango::compare_val_strong>);
static_assert(dango::is_convertible<dango::compare_val_weak, dango::compare_val_weak>);
static_assert(dango::is_convertible<dango::compare_val_weak, dango::compare_val_partial>);

static_assert(!dango::is_convertible<dango::compare_val_partial, std::strong_ordering>);
static_assert(!dango::is_convertible<dango::compare_val_partial, std::weak_ordering>);
static_assert(dango::is_convertible<dango::compare_val_partial, std::partial_ordering>);
static_assert(!dango::is_convertible<dango::compare_val_partial, dango::compare_val_strong>);
static_assert(!dango::is_convertible<dango::compare_val_partial, dango::compare_val_weak>);
static_assert(dango::is_convertible<dango::compare_val_partial, dango::compare_val_partial>);

static_assert(dango::is_convertible<std::strong_ordering, dango::compare_val_strong>);
static_assert(dango::is_convertible<std::strong_ordering, dango::compare_val_weak>);
static_assert(dango::is_convertible<std::strong_ordering, dango::compare_val_partial>);

static_assert(!dango::is_convertible<std::weak_ordering, dango::compare_val_strong>);
static_assert(dango::is_convertible<std::weak_ordering, dango::compare_val_weak>);
static_assert(dango::is_convertible<std::weak_ordering, dango::compare_val_partial>);

static_assert(!dango::is_convertible<std::partial_ordering, dango::compare_val_strong>);
static_assert(!dango::is_convertible<std::partial_ordering, dango::compare_val_weak>);
static_assert(dango::is_convertible<std::partial_ordering, dango::compare_val_partial>);


static_assert(dango::equals("hello", "hello"));
static_assert(!dango::equals("hello", "hi"));
static_assert(dango::compare("hello", "hello") == 0);
static_assert(dango::compare("hello", "hi") < 0);
static_assert(dango::compare("hey", "hello") > 0);
static_assert(dango::min(16.0f) == 16.0f);
static_assert(dango::max(16.0f) == 16.0f);
static_assert(dango::min(16.0, 32.0) == 16.0);
static_assert(dango::max(16.0, 32.0) == 32.0);
static_assert(dango::min(5, 4, 3, 2, 1, 1, 1, 2, 3, 4, 5) == 1);
static_assert(dango::max(1, 2, 3, 4, 5, 5, 5, 4, 3, 2, 1) == 5);
