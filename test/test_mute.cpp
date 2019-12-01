// test_mute.cpp

#include "mute/mute.h"

__MUTE_TEST( "Scenario: ", "something stringy", "[aaa,bbb]" ) {
    int v = 123;
    __MUTE_CHECK( v );
    __MUTE_CHECK_THAT( v, mute::gt( 124 ) );
}

__MUTE_TEST( "Scenario: ", "something else stringy", "[aaa,bbb]" ) {
    __MUTE_SECTION( "when ", "something" ) {
        __MUTE_SECTION( "then ", "something" ) {
        }
        __MUTE_SECTION( "then ", "something else" ) {
        }
    }

    __MUTE_SECTION( "when ", "something else" ) {
    }
}
