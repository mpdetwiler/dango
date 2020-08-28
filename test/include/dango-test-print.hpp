#ifndef DANGO_TEST_PRINT_HPP_INCLUDED
#define DANGO_TEST_PRINT_HPP_INCLUDED

#include <cstdio>

#define test_print(...) std::fprintf(stderr, __VA_ARGS__)

#endif
