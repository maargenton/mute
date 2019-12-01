# Mute : Micro Unit Testing Engine for C++

## Overview

Mute is a compact single header test framework designed to enable testability of
embedded code on embedded platforms. Its interface is similar to Catch2
BDD-style interface, but using test predicates rather than expression
decomposition macros. The framework goes through all the tests defined in a
binary, and all the distinct section branches defined in those tests. It outputs
its progress verbosely and relies on an external test runner to interpret the
results. It does not support or use exceptions, and does not perform any heap
memory allocation.

### Versions

- **v0.1.0**: Initial version, fully usable. Needs more predicates, output parser
  and report generator

## Usage

Mute is composed of a single header file, `mute/mute.h`. To use it, make sure it
is in the include search path of your build system and include it in the `.cpp`
file containing your tests, right after the header of the module under test.

```cpp
#include "feature_under_test.h"
#include "mute/mute.h"
```

Depending on the situation, each test compilation unit can be linked into
individual binaries, or all of them can be linked together into a single binary.
In the first case, a default `main()` function can be included using the GCC
`-include` option

```
gcc -o test -include mute/mute_runner_stdout.h test.cpp
./test
```

When building a single binary from multiple test compilation units, you will
need to provide one additional compilation unit containing the main function,
typically:

```cpp
// main_test.cpp
#include "mute/mute_runner_stdout.h"
```

When building the tests for less common platform, you might need a custom test
runner with an alternate output interface. In that case, you can make your own,
suing `mute/mute_runner_stdout.h` as a reference.


## Writing tests

Mute exposes both a BDD-style and a more traditional style interface. Within a
test compilation unit, tests are defined within `SCENARIO()` or `TEST_CASE()`.
Each scenario is composed of different sections (`SECTION()`, `GIVEN()`,
`WHEN()`, `THEN()`), consecutive or nested. Each scenario or section is named
with a plain quoted string, generally explaining the purpose and details of the
test performed.

Test statements are expressed within `CHECK(<expr>)` or `REQUIRE(<expr>)` macros
that evaluate the expression as a boolean and expect it to be `true`. The
`REQUIRE()` form prevents any further testing upon failure. In addition
`CHECK_THAT(<value>, <predicate>)` and `REQUIRE_THAT(...)` provide a predicate
based interface tht can provide a lot more details about the value in case of
failure. Using the predicate form is recommended whenever possible.

> **NOTE**: Due to implementation details and C++ syntax limitation, `REQUIRE()`
> and `REQUIRE_THAT()` can only be used within a section, and not at the root of
> a scenario or test case.

```cpp
// test.cpp

#include "mute/mute.h"

SCENARIO( "first scenario", "" ) {
    using namespace mute;
    int v = 123;

    WHEN( "doing something" ) {
        THEN( "something happens" ) {
            REQUIRE_THAT( v, eq(123) );
        }
        THEN( "something else happens" ) {
            REQUIRE_THAT( v, lt(125) );
        }
    }
}
```

## Predicates

Mute provide built-in predicates to test numeric values:
- `eq( <value> )`: equals
- `ne( <value> )` : not equal
- `lt( <value> )` : less than
- `le( <value> )` : less or equal
- `gt( <value> )` : greater than
- `ge( <value> )` : greater or equal


## TODO

- [ ] predicates for C strings (equal, contains, starts_with, ends_with)
- [ ] value display for C strings
- [ ] predicates for float comparison (almost_equal)
- [ ] value display for float values
- [ ] output parser and report generator
