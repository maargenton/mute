//=============================================================================
//
// MODULE      : mute/mute.h
// PROJECT     : Micro Unit Testing Engine for embedded C++
// DESCRIPTION : Mute is a compact single header file test framework designed
//               to provide testability of embedded code on embedded platforms.
//               Its interface is similar to Catch2 BDD-style interface, but
//               using test predicates rather than expression decomposition
//               macros. The framework goes through all the tests defined in a
//               binary, and all the distinct section branches defined in those
//               tests. It outputs its progress verbosely and relies on an
//               external test runner to interpret the results. It does not
//               support or use exceptions, and does not perform any heap
//               memory allocation.
//
// Copyright (c) 2013-2019, Marc-Antoine Argenton.  All rights reserved.
//=============================================================================
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// - Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//============================================================================

#pragma once
#include <inttypes.h>
#include <limits>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MUTE_VERSION "v0.1.0"

#define MUTE_PP_CAT( x, y ) __MUTE_PP_CAT( x, y )
#define __MUTE_PP_CAT( x, y ) x##y

#define MUTE_PP_STR( x ) __MUTE_PP_STR( x )
#define __MUTE_PP_STR( x ) #x

#ifdef __cplusplus

// =============================================================================
// Definition of output_t interface and writer() helpers
// =============================================================================

namespace mute {

struct output_t {
    virtual void write( const char* p, size_t n ) = 0;
};

// writer encapsulate an output_t with a collection of common formating
// functions.
template <typename output_t>
struct writer_t {
    output_t& out;
    writer_t( output_t& out ) : out( out ) {
    }

    void write( const char* b, size_t l ) {
        out.write( b, l );
    }
    void write_cstr( const char* str ) {
        int l = strlen( str );
        out.write( str, l );
    }

    void write_int( int v ) {
        char   buf[16];
        size_t length = snprintf( buf, sizeof( buf ), "%d", v );
        if ( length < sizeof( buf ) ) {
            write( buf, length );
        }
    }

    void write_newline() {
        out.write( "\n", 1 );
    }

    void write_prefix( const char* filename, int lineno ) {
        write_cstr( filename );
        out.write( ":", 1 );
        write_int( lineno );
        out.write( ": ", 2 );
    }

    void enter( const char* filename, int lineno, const char* prefix, const char* name ) {
        write_prefix( filename, lineno );
        write( "enter: ", 7 );
        write_cstr( prefix );
        write_cstr( name );
        write_newline();
    }

    void leave( const char* filename, int lineno, const char* prefix, const char* name ) {
        write_prefix( filename, lineno );
        write( "leave: ", 7 );
        write_cstr( prefix );
        write_cstr( name );
        write_newline();
    }

    void report_prefix( const char* filename, int lineno, bool success ) {
        write_prefix( filename, lineno );
        if ( success ) {
            write( "passed: ", 8 );
        } else {
            write( "failed: expected ", 17 );
        }
    }
};

template <typename output_t>
writer_t<output_t> writer( output_t& out ) {
    return writer_t<output_t>( out );
}

} // namespace mute

// =============================================================================
// Definition of write_description() methods for all basic types, used by
// predicates and assertion function to display values of various kinds.
// =============================================================================

namespace mute {

// ---------------------------------------------------------------------------
// write_description_xxx() implementation for integers; actual overloads of
// write_description() functions need to be defined on primitive type (not
// stdint types) to avoid ambiguous type resolution.

template <typename output_t>
void write_description_signed( output_t& out, int32_t v ) {
    char buf[64];
    int  l = 0;
    if ( v >= int32_t( 0x20 ) && v < int32_t( 0x7f ) ) {
        l = snprintf(
            buf, sizeof( buf ), "%" PRId32 " (0x%02" PRIx32 ",'%c')", v,
            uint32_t( uint8_t( v ) ), (char)v );

    } else if ( v >= std::numeric_limits<int8_t>::min() && v <= std::numeric_limits<int8_t>::max() ) {
        l = snprintf(
            buf, sizeof( buf ), "%" PRId32 " (0x%02" PRIx32 ")", v,
            uint32_t( uint8_t( v ) ) );

    } else if ( v >= std::numeric_limits<int16_t>::min() && v <= std::numeric_limits<int16_t>::max() ) {
        l = snprintf(
            buf, sizeof( buf ), "%" PRId32 " (0x%04" PRIx32 ")", v,
            uint32_t( uint16_t( v ) ) );

    } else {
        l = snprintf( buf, sizeof( buf ), "%" PRId32 " (0x%08" PRIx32 ")", v, uint32_t( v ) );
    }

    out.write( buf, l );
}

template <typename output_t>
void write_description_signed64( output_t& out, int64_t v ) {
    char buf[64];
    int  l = 0;
    if ( v >= int32_t( 0x20 ) && v < int32_t( 0x7f ) ) {
        l = snprintf(
            buf, sizeof( buf ), "%" PRId32 " (0x%02" PRIx32 ",'%c')",
            int32_t( v ), uint32_t( uint8_t( v ) ), (char)v );

    } else if ( v >= std::numeric_limits<int8_t>::min() && v <= std::numeric_limits<int8_t>::max() ) {
        l = snprintf(
            buf, sizeof( buf ), "%" PRId32 " (0x%02" PRIx32 ")", int32_t( v ),
            uint32_t( uint8_t( v ) ) );

    } else if ( v >= std::numeric_limits<int16_t>::min() && v <= std::numeric_limits<int16_t>::max() ) {
        l = snprintf(
            buf, sizeof( buf ), "%" PRId32 " (0x%04" PRIx32 ")", int32_t( v ),
            uint32_t( uint16_t( v ) ) );

    } else if ( v >= std::numeric_limits<int32_t>::min() && v <= std::numeric_limits<int32_t>::max() ) {
        l = snprintf(
            buf, sizeof( buf ), "%" PRId32 " (0x%08" PRIx32 ")", int32_t( v ),
            uint32_t( v ) );

    } else {
        l = snprintf(
            buf, sizeof( buf ), "%" PRId64 " (0x%016" PRIx64 ")", int64_t( v ),
            uint64_t( v ) );
    }

    out.write( buf, l );
}

template <typename output_t>
void write_description_unsigned( output_t& out, uint32_t v ) {
    char buf[64];
    int  l = 0;
    if ( v >= uint32_t( 0x20 ) && v < uint32_t( 0x7f ) ) {
        l = snprintf(
            buf, sizeof( buf ), "%" PRId32 " (0x%02" PRIx32 ",'%c')", v, v, (char)v );

    } else if ( v <= std::numeric_limits<uint8_t>::max() ) {
        l = snprintf( buf, sizeof( buf ), "%" PRId32 " (0x%02" PRIx32 ")", v, v );

    } else if ( v <= std::numeric_limits<uint16_t>::max() ) {
        l = snprintf( buf, sizeof( buf ), "%" PRId32 " (0x%04" PRIx32 ")", v, v );

    } else {
        l = snprintf( buf, sizeof( buf ), "%" PRId32 " (0x%08" PRIx32 ")", v, v );
    }

    out.write( buf, l );
}

template <typename output_t>
void write_description_unsigned64( output_t& out, uint64_t v ) {
    char buf[64];
    int  l = 0;
    if ( v >= uint32_t( 0x20 ) && v < uint32_t( 0x7f ) ) {
        l = snprintf(
            buf, sizeof( buf ), "%" PRId32 " (0x%02" PRIx32 ",'%c')",
            uint32_t( v ), uint32_t( v ), (char)v );

    } else if ( v <= std::numeric_limits<uint8_t>::max() ) {
        l = snprintf(
            buf, sizeof( buf ), "%" PRId32 " (0x%02" PRIx32 ")", uint32_t( v ),
            uint32_t( v ) );

    } else if ( v <= std::numeric_limits<uint16_t>::max() ) {
        l = snprintf(
            buf, sizeof( buf ), "%" PRId32 " (0x%04" PRIx32 ")", uint32_t( v ),
            uint32_t( v ) );

    } else if ( v <= std::numeric_limits<uint32_t>::max() ) {
        l = snprintf(
            buf, sizeof( buf ), "%" PRId32 " (0x%08" PRIx32 ")", uint32_t( v ),
            uint32_t( v ) );

    } else {
        l = snprintf(
            buf, sizeof( buf ), "%" PRId64 " (0x%016" PRIx64 ")", uint64_t( v ),
            uint64_t( v ) );
    }

    out.write( buf, l );
}

template <typename output_t>
void write_description_ptr( output_t& out, uintptr_t v ) {
    char buf[64];
    int  l = 0;

    if ( sizeof( uintptr_t ) == 4 ) {
        l = snprintf( buf, sizeof( buf ), "[0x%08" PRIxPTR "]", v );
    } else if ( sizeof( uintptr_t ) == 8 ) {
        l = snprintf( buf, sizeof( buf ), "[0x%016" PRIxPTR "]", v );
    }

    out.write( buf, l );
}

template <typename output_t>
void write_description( output_t& out, signed char v ) {
    write_description_signed( out, int32_t( v ) );
}

template <typename output_t>
void write_description( output_t& out, unsigned char v ) {
    write_description_unsigned( out, uint32_t( v ) );
}

template <typename output_t>
void write_description( output_t& out, signed short int v ) {
    write_description_signed( out, int32_t( v ) );
}

template <typename output_t>
void write_description( output_t& out, unsigned short int v ) {
    write_description_unsigned( out, uint32_t( v ) );
}

template <typename output_t>
void write_description( output_t& out, signed int v ) {
    if ( sizeof( v ) > 4 )
        write_description_signed64( out, int64_t( v ) );
    else
        write_description_signed( out, int32_t( v ) );
}

template <typename output_t>
void write_description( output_t& out, unsigned int v ) {
    if ( sizeof( v ) > 4 )
        write_description_signed64( out, uint64_t( v ) );
    else
        write_description_signed( out, uint32_t( v ) );
}

template <typename output_t>
void write_description( output_t& out, signed long int v ) {
    if ( sizeof( v ) > 4 )
        write_description_signed64( out, int64_t( v ) );
    else
        write_description_signed( out, int32_t( v ) );
}

template <typename output_t>
void write_description( output_t& out, unsigned long int v ) {
    if ( sizeof( v ) > 4 )
        write_description_signed64( out, uint64_t( v ) );
    else
        write_description_signed( out, uint32_t( v ) );
}

template <typename output_t>
void write_description( output_t& out, signed long long int v ) {
    if ( sizeof( v ) > 4 )
        write_description_signed64( out, int64_t( v ) );
    else
        write_description_signed( out, int32_t( v ) );
}

template <typename output_t>
void write_description( output_t& out, unsigned long long int v ) {
    if ( sizeof( v ) > 4 )
        write_description_signed64( out, uint64_t( v ) );
    else
        write_description_signed( out, uint32_t( v ) );
}

template <typename output_t>
void write_description( output_t& out, const void* v ) {
    write_description_ptr( out, uintptr_t( v ) );
}

template <typename output_t, typename T>
void write_description( output_t& out, T* v ) {
    write_description_ptr( out, uintptr_t( v ) );
}

template <typename output_t, typename T>
void write_description( output_t& out, const T* v ) {
    write_description_ptr( out, uintptr_t( v ) );
}

} // namespace mute

// =============================================================================
// Definition of main mute test framework classes, including test_env_t,
// section_t, test_t and test_registry_t
// =============================================================================

namespace mute {

struct mute_t {};

// test_env_t encapsulates the context in which tests are run, including
// the output to report to, the sections being visited, and the abort status
// for the current test.
struct test_env_t {
    test_env_t( output_t& output ) : output( output ) {
    }
    output_t& output;

    bool enter_section() {
        _index[_depth]++;
        _count[_depth]++;
        bool enter = ( _index[_depth] == _count[_depth] );
        _depth++;
        return enter;
    }

    void leave_section() {
        _depth--;
        _index[_depth]--;
    }

    int depth() {
        return _depth;
    }

    void reset() {
        _tracking = false;
        for ( int i = 0; i < max_depth; i++ ) {
            _count[i] = 0;
            _index[i] = 0;
        }
    }

    bool repeat() {
        if ( !_tracking ) {
            reset();
            _tracking = true;
            return true;
        }

        bool done = true;
        for ( int i = max_depth - 1; i >= 0; i-- ) {
            if ( _index[i] < _count[i] - 1 ) {
                _index[i]++;
                done = false;
                for ( int j = i + 1; j < max_depth; j++ ) {
                    _index[j] = 0;
                }
                break;
            }
        }
        for ( int i = 0; i < max_depth; i++ ) {
            _count[i] = 0;
        }
        if ( !done ) {
            return true;
        }

        _tracking = false;
        return false;
    }

    static const int max_depth = 16;

private:
    bool _tracking         = false;
    int  _depth            = 0;
    int  _index[max_depth] = { 0 };
    int  _count[max_depth] = { 0 };
};

// section_t represent an exclusive branch within a test case
struct section_t {
    section_t( test_env_t& __test_env, const char* filename, int lineno, const char* prefix, const char* name )
        : _test_env( __test_env ),
          _filename( filename ),
          _lineno( lineno ),
          _prefix( prefix ),
          _name( name ) {
        //
        _enter = _test_env.enter_section();
        if ( _enter ) {
            writer( _test_env.output ).enter( _filename, _lineno, _prefix, _name );
        }
    }

    ~section_t() {
        if ( _enter ) {
            writer( _test_env.output ).leave( _filename, _lineno, _prefix, _name );
        }
        _test_env.leave_section();
    }

    operator bool() {
        if ( !_done ) {
            _done = true;
            return _enter;
        }
        return false;
    }

private:
    test_env_t& _test_env;
    const char* _filename;
    int         _lineno;
    const char* _prefix;
    const char* _name;

    bool _enter = false;
    bool _done  = false;
};

// test_t is the base class for the registrar of each registered
// test, and provides both a list-node interface and a virtual interface to
// access the test information and run the test.
struct test_t {
    virtual const char* type() const                 = 0;
    virtual const char* name() const                 = 0;
    virtual const char* filename() const             = 0;
    virtual const char* flags() const                = 0;
    virtual int         lineno() const               = 0;
    virtual void        run( test_env_t& env ) const = 0;

    test_t* _next = nullptr;
    test_t* next() {
        return _next;
    }
};

// list_t<node_t> is a STL-like container for singly linked list of nodes
// with embedded next pointer.
template <typename node_t>
struct list_t {
    struct iterator {
        explicit iterator( node_t* elt = nullptr ) : _current( elt ) {
        }

        iterator& operator++() {
            _current = _current->next();
            return *this;
        }
        iterator operator++( int ) {
            iterator it = *this;
            _current    = _current->next();
            return it;
        }
        bool operator==( const iterator& rhs ) {
            return _current == rhs._current;
        }
        bool operator!=( const iterator& rhs ) {
            return _current != rhs._current;
        }
        node_t& operator*() const {
            return *_current;
        }
        node_t* operator->() const {
            return _current;
        }

    private:
        node_t* _current;
    };

    explicit list_t( node_t* elt ) : _head( elt ) {
    }
    iterator begin() {
        return iterator( _head );
    }
    iterator end() {
        return iterator( nullptr );
    }

private:
    node_t* _head;
};

// test_registry_t maintains the master list of registered tests. Is is
// implemeneted as a dummy template type to ensure correct sharing of
// class static variables.
template <typename T>
struct test_registry_tt {
    typedef list_t<test_t> test_list_t;

    static test_registry_tt& instance() {
        static test_registry_tt _instance;
        return _instance;
    }

    void register_test( test_t* test ) {
        if ( _last_test )
            _last_test->_next = test;
        else
            _first_test = test;

        test->_next = nullptr;
        _last_test  = test;
    }

    test_list_t test_list() const {
        return test_list_t( _first_test );
    }

private:
    test_registry_tt() : _first_test( nullptr ), _last_test( nullptr ) {
    }
    ~test_registry_tt() {
    }

    test_t* _first_test;
    test_t* _last_test;
};
typedef test_registry_tt<mute_t> test_registry_t;

// test_tt<tc_t> is a template subclass of test_t that implements the test_t
// interface for a specific test class. Actual test classes need to expose
// their interface through static methods.
template <typename tc_t>
struct test_tt : test_t {
    virtual const char* type() const {
        return tc_t::type();
    }
    virtual const char* name() const {
        return tc_t::name();
    }
    virtual const char* flags() const {
        return tc_t::flags();
    }
    virtual const char* filename() const {
        return tc_t::filename();
    }
    virtual int lineno() const {
        return tc_t::lineno();
    }
    virtual void run( test_env_t& env ) const {
        tc_t::run( env );
    }
};

template <typename tc_t>
const test_t* register_test() {
    static test_tt<tc_t> registrar;
    mute::test_registry_t::instance().register_test( &registrar );
    return &registrar;
}

// check_that checks a value against a predicate and reports the result to
// the test framework
template <typename value_t, typename predicate_t>
bool check_that( test_env_t& env, const char* filename, int line, const char* expr, value_t value, predicate_t pred ) {
    bool success = pred.eval( value );
    writer( env.output ).report_prefix( filename, line, success );
    pred.describe( env.output, expr );
    writer( env.output ).write_newline();
    if ( !success ) {
        pred.write_details( env.output, value );
    }
    return success;
}

// check checks a value as a boolean expression and reports the result to
// the test framework
template <typename value_t>
bool check( test_env_t& env, const char* filename, int line, const char* expr, value_t value ) {
    bool success = !!( value );
    writer( env.output ).report_prefix( filename, line, success );
    writer( env.output ).write_cstr( expr );
    writer( env.output ).write( " == true\n", 9 );

    return success;
}

// run_all_tests runs all registered tests, using the provided output to
// to print out progress and diagnostic
static inline void run_all_tests( output_t& output ) {
    auto tests = mute::test_registry_t::instance().test_list();
    for ( auto it = tests.begin(); it != tests.end(); it++ ) {
        const mute::test_t& test = *it;
        mute::test_env_t    env( output );

        while ( env.repeat() ) {
            writer( output ).enter(
                test.filename(), test.lineno(), test.type(), test.name() );

            test.run( env );

            writer( output ).leave(
                test.filename(), test.lineno(), test.type(), test.name() );
            writer( output ).write_newline();
        }
    }
}

}; // namespace mute

// =============================================================================
// Definition of main framework macros, leveraging all the underlying machinery
// and exposing it through a fluid BDD-style interface.
// =============================================================================

#define __MUTE_TEST( __type, __name, __flags )                                 \
    ___MUTE_TEST( MUTE_PP_CAT( test, __LINE__ ), __type, __name, __flags )

#define ___MUTE_TEST( tc, __type, __name, __flags )                            \
    namespace {                                                                \
    struct tc {                                                                \
        static const char* type() {                                            \
            return __type;                                                     \
        }                                                                      \
        static const char* name() {                                            \
            return __name;                                                     \
        }                                                                      \
        static const char* flags() {                                           \
            return __flags;                                                    \
        }                                                                      \
        static const char* filename() {                                        \
            return __FILE__;                                                   \
        }                                                                      \
        static int lineno() {                                                  \
            return __LINE__;                                                   \
        }                                                                      \
        static void                run( mute::test_env_t& env );               \
        static const mute::test_t* registrar;                                  \
    };                                                                         \
    const mute::test_t* tc::registrar = mute::register_test<tc>();             \
    }                                                                          \
    void tc::run( mute::test_env_t& __test_env )

#define __MUTE_SECTION( __type, __name )                                       \
    for ( mute::section_t section( __test_env, __FILE__, __LINE__, __type, __name ); section; )

#define __MUTE_CHECK( __expr )                                                 \
    mute::check( __test_env, __FILE__, __LINE__, MUTE_PP_STR( __expr ), ( __expr ) )

#define __MUTE_CHECK_THAT( __expr, __predicate )                               \
    mute::check_that( __test_env, __FILE__, __LINE__, MUTE_PP_STR( __expr ), ( __expr ), __predicate )

#define SCENARIO( __name, __flags ) __MUTE_TEST( "Scenario: ", __name, __flags )
#define GIVEN( __name ) __MUTE_SECTION( "given ", __name )
#define WHEN( __name ) __MUTE_SECTION( "when ", __name )
#define THEN( __name ) __MUTE_SECTION( "then ", __name )

#define TEST_CASE( __name, __flags ) __MUTE_TEST( "", __name, __flags )
#define SECTION( __name ) __MUTE_SECTION( "", __name )

#define CHECK( __expr ) __MUTE_CHECK( __expr )
#define CHECK_THAT( __expr, __predicate )                                      \
    __MUTE_CHECK_THAT( __expr, __predicate )

#define REQUIRE( __expr )                                                      \
    if ( !__MUTE_CHECK( __expr ) ) {                                           \
        continue;                                                              \
    }

#define REQUIRE_THAT( __expr, __predicate )                                    \
    if ( !__MUTE_CHECK_THAT( __expr, __predicate ) ) {                         \
        continue;                                                              \
    }

// =============================================================================
// Definition of common test predicates for numeric types
// =============================================================================

#define MUTE_DEFINE_NUMERIC_COMPARISON_MATCHER( fct, op )                      \
    namespace mute {                                                           \
    template <typename __ref_t>                                                \
    struct fct##_t {                                                           \
    public:                                                                    \
        typedef __ref_t        ref_t;                                          \
        typedef fct##_t<ref_t> matcher_t;                                      \
        ref_t                  ref;                                            \
                                                                               \
        fct##_t( ref_t ref ) : ref( ref ) {                                    \
        }                                                                      \
                                                                               \
        template <typename value_t>                                            \
        bool eval( value_t value ) const {                                     \
            return (ref_t)value op ref;                                        \
        }                                                                      \
                                                                               \
        template <typename output_t>                                           \
        void describe( output_t& out, const char* expr ) const {               \
            writer( out ).write_cstr( expr );                                  \
            writer( out ).write_cstr( " " MUTE_PP_STR( op ) " " );             \
            write_description( out, ref );                                     \
        }                                                                      \
                                                                               \
        template <typename output_t, typename value_t>                         \
        void write_details( output_t& out, value_t value ) const {             \
            writer( out ).write( "    actual value: ", 18 );                   \
            write_description( out, value );                                   \
            writer( out ).write_newline();                                     \
        }                                                                      \
    };                                                                         \
                                                                               \
    template <typename ref_t>                                                  \
    fct##_t<ref_t> fct( ref_t expected ) {                                     \
        return fct##_t<ref_t>( expected );                                     \
    }                                                                          \
    }

MUTE_DEFINE_NUMERIC_COMPARISON_MATCHER( eq, == )
MUTE_DEFINE_NUMERIC_COMPARISON_MATCHER( ne, != )
MUTE_DEFINE_NUMERIC_COMPARISON_MATCHER( lt, < )
MUTE_DEFINE_NUMERIC_COMPARISON_MATCHER( gt, > )
MUTE_DEFINE_NUMERIC_COMPARISON_MATCHER( le, <= )
MUTE_DEFINE_NUMERIC_COMPARISON_MATCHER( ge, >= )

#endif // __cplusplus
