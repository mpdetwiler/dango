# dango
## UPDATED 24 august 2020
### this project is intended to be a C++ general purpose framework and standard library alternatve.
  - heavy WIP, currently not very useful.

### building
  - dango is intended to be mostly a header-only library however, it does compile into a small shared/dynamic library
    - mainly to avoid having to include large headers (ie windows.h) in dango headers
  - you will need either GCC 10 or clang 10 to compile (extensive use of C++20 features)
  - you will need the build tool called SCons (which requires python)
  - once you have these installed: `scons --target={linux, win32, win64, darwin}`
    - linux is the default
  - options:
    - `-c` clean
    - `--use-clang` use clang++ instead of g++ to compile
    - `--test` also compile test program/executable (testing in progress)
      - to run: `./exe_test` or `./exe_test.exe` on windows
    - `--no-debug` disable debug mode
      - affects the behavior of dango_assert and dango_unreachable. if a dango_assert could fail or a dango_unreachable
        is actually reachable in a debug-mode program, the corresponding non debug-mode program is undefined
      - will affect the presence of various bug detection mechanisms, for example whether or not a container contains
        a "mod count" for detecting use of invalid iterators
    - `--no-multicore` affects the behavior of `dango::spin_mutex` and other busy waiting (see dango::busy_wait_while(...))
      - causes blocked threads to not actually do any spinning in userspace and just yield immediately,
        because spinning in this case is pointless on a single-threaded CPU
    - `--big-cache-lines` compile with the cache line size assumed to 128 bytes instead of the default 64
      - it seems the vast majority of modern CPUs have 64 byte cache lines
      - some older pentium 4 had 128 byte lines
    - `--new-noexcept` causes `dango::operator_new` to be a noexcept function
      - all heap allocations made by dango are ultimately backed by `dango::operator_new` which just calls `::operator new(std::size_t, std::align_val_t)`
      - this controls whether or not `dango::operator_new` will propagate an exception thrown by `::operator new`
        or just catch it and terminate the program immediately (and in that case `dango::operator_new` is noexcept)
      - rationale:
        - many functions and constructors in dango (and most C++ code in general) are not noexcept solely because they might allocate dynamic
          memory. on many platforms/configurations (for example windows, or linux with overcommit enabled), `::operator new` will not
          realisitically ever throw an exception, outside of programmer error (ie trying to allocate size_t(-1) bytes). if `::operator new`
          were just noexcept on these platforms where it is never going to throw anyway, many functions and constructors could suddenly
          become noexcept. this could lead to a potential performance increase and code size reduction. that is the sole purpose of this
          build option
      - obviously code that is intended to be as portable as possible should still be written as if memory allocation can throw (you should
        still be writing exception safe code) because it might not be optimal use this option on all platforms
### current and future features (goals)
  - type trait library
    - all boolean type traits are concepts, not class templates or variable templates
      - instead of `std::is_same<T, U>::value` or `std::is_same_v<T, U>` just `dango::is_same<T, U>`
    - all metafunctions are alias templates already
      - instead of `typename std::remove_reference<T>::type` or `std::remove_reference_t<T>` just `dango::remove_ref<T>`
    - additional luxurious type traits not in the standard library
      - for example:
        - `dango::is_same_ignore_cv<T, U>` (equivalent to `dango::is_same<dango::remove_cv<T>, dango::remove_cv<U>>`)
        - `dango::is_const_or_volatile<T>`, `dango::is_const_and_volatile<T>`
        - `dango::is_referenceable<T>`
        - `dango::is_class_or_union<T>`
        - many more
  - ad-hoc unit tests with DANGO_UNIT_TEST_BEGIN(my_test){  }DANGO_UNIT_TEST_END
  - easy operator customization points
    - specialize dango::custom::operator_*<T> or write a member function dango_operator_*(...) to get various operators, for example
      - `T::dango_operator_is_null()const noexcept(...)->bool` => equatability with `null` (`nullptr`) for nullable types `T == null`, `null == T`, `T != null`, `null != T`
      - `T::dango_operator_equals(U const&)const noexcept(...)->bool` => T == U, T != U
      - `T::dango_operator_compare(U const&)const noexcept(...)->dango::compare_val_strong` (example return type) => `T > U`, `T >= U`, `T < U`, `T <= U`, `T <=> U`, `dango::compare(T, U)`
      - `T::dango_operator_swap(U&)& noexcept(...)->void` => `dango::swap(T, U)`
      - `T::dango_operator_hash()const noexcept->dango::hash_val` => `dango::hash(T)`
      - more to come
  - atomic library
    - standalone atomic functions
    - `dango::atomic<T>`
      - has no operators (dangerous) just named member functions only
      - T cannot be cv qualified (use standalone functions for volatile + atomic)
      - if the class template `dango::atomic<T>` instantiates then it is lock-free (and always so)
  - threading and synchronization library
    - Java inspired daemon and non-daemon threads with the ability to prevent the process from exiting until
      all non-daemon threads have terminated
    - sync primitives:
      - `spin_mutex`
      - `exec_once`
      - `mutex`
      - `static_mutex` (static meaning designed to have static storage duration and also should be `constinit`)
      - `cond_var` `static_cond_var` (condition variable that is bound to a particular mutex at construction time and is stuck to it
        for its entire lifetime (safer))
      - `rw_mutex` `static_rw_mutex` (reader writer lock)
      - `rec_mutex` `static_rec_mutex` (recursive mutex)
      - `monitor` `static_monitor` (a mutex and condition variable constructed together and stuck together for their entire lifetime (safest)
        also recursive, Java inspired)
    - support for waiting on timeouts that are or are not suspend-aware
    - portably wait on high resolution timeouts (1 ms resolution)
  - container library
    - `intrusive_list<T>` (doubly linked list where T must derive from `intrusive_list_elem<T>`)
    - `auto_ptr<T>`
    - `shared_ptr<T>` `weak_ptr<T>`
    - `owning_ptr<T>` `observing_ptr<T>`
    - container allocators are polymorphic by default
    - `array<T, N>` (compile time fixed sized array)
    - `array<T>` (runtime fixed sized array)
    - `array_list<T>` (dynamic sized array)
    - `priority_queue<P, T>`
    - `tree_set<K>` `priority_tree_set<P, K>`
    - `tree_map<K, T>` `priority_tree_map<P, K, T>`
    - `hash_set<K>` `priority_hash_set<P, K>`
    - `hash_map<K, T>` `priority_hash_map<P, K, T>`
    - `char_seq<C>`
    - `string` (strict utf8 only)
  - window creation and openGL/Vulkan context creation with input handling
    - planned to be implemented with GLFW
  - agents
