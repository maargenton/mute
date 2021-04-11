// test_checks.cpp

#include "mute/mute.h"

SCENARIO( "`REQUIRE` prevents any further testing", "" ) {
    CHECK( false );
    CHECK( false );

    GIVEN( "a section after failed `REQUIRE`" ) {
        THEN( "the section is not executed nor visible" ) {
            REQUIRE( false );
            REQUIRE( false );
        }
    }
}

SCENARIO( "`REQUIRE` statement in section allows further section to run", "" ) {
    using namespace mute;
    int v = 123;

    GIVEN( "a first section before any failed `REQUIRE`" ) {
        THEN(
            "the section is executed, and the first failed `REQUIRE` is "
            "reported" ) {
            REQUIRE_THAT( v, lt( 123 ) );
            REQUIRE( false );
        }
    }
    GIVEN( "a second section after a section containing s failed require" ) {
        THEN(
            "the section is executed, and the first failed `REQUIRE` is "
            "reported" ) {
            REQUIRE_THAT( v, gt( 123 ) );
            REQUIRE( false );
        }
    }
}
