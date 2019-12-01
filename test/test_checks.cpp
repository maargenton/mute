// test_checks.cpp

#include "mute/mute.h"

SCENARIO( "All checks are executed regarles of success", "" ) {
    CHECK( false );
    CHECK( false );
    GIVEN( "something" ) {
        CHECK( false );
        CHECK( false );
        WHEN( "doing something" ) {
            CHECK( false );
            CHECK( false );
            THEN( "something happens" ) {
                CHECK( false );
                CHECK( false );
            }
            THEN( "something else happens" ) {
                CHECK( false );
                CHECK( false );
            }
        }
    }
}
