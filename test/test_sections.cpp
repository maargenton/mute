// test_section.cpp

#include "mute/mute.h"

SCENARIO( "first scenario", "" ) {
    GIVEN( "something" ) {
        WHEN( "doing something" ) {
            THEN( "something happens" ) {
            }
            THEN( "something else happens" ) {
            }
        }
    }
}

TEST_CASE( "first test", "" ) {
    SECTION( "something" ) {
        SECTION( "doing something" ) {
            SECTION( "something happens" ) {
            }
            SECTION( "something else happens" ) {
            }
        }
    }
}
