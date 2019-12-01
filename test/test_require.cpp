// test_checks.cpp

#include "mute/mute.h"

SCENARIO( "Require prevents any further testing", "" ) {
    CHECK( false );
    CHECK( false );

    GIVEN( "a section after failed require" ) {
        THEN( "the section is not executed nor visible" ) {
            REQUIRE( false );
            REQUIRE( false );
        }
    }
}

SCENARIO( "Require is section allows further section", "" ) {
    using namespace mute;
    int v = 123;

    GIVEN( "a first section before any failed require" ) {
        THEN( "the section is executed" ) {
            REQUIRE_THAT( v, lt( 123 ) );
            REQUIRE( false );
        }
    }
    GIVEN( "a second section after a section containing s failed require" ) {
        THEN( "the section is executed" ) {
            REQUIRE_THAT( v, gt( 123 ) );
            REQUIRE( false );
        }
    }
}
