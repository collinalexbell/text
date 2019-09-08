#include "buffer.h"
#include "catch.hpp"

TEST_CASE("moveCursor doesn't allow movement out of contents bounds",
        "[buffer]"){
    string contents = "foo bar\nbaz bin\n\n";
    Buffer b(contents);

    //up
    b.moveCursor(DOWN);
    REQUIRE_NOTHROW(b.moveCursor(UP));
    REQUIRE_THROWS(b.moveCursor(UP));

    //left
    b.moveCursor(RIGHT);
    REQUIRE_NOTHROW(b.moveCursor(LEFT));
    REQUIRE_THROWS(b.moveCursor(LEFT));

    //down
    b.moveCursor(RIGHT); // setup for last REQUIRE in this section
    REQUIRE_NOTHROW(b.moveCursor(DOWN, 2));
    REQUIRE_THROWS(b.moveCursor(DOWN));
    //check that moving from row with 7 chars to row with 0 chars:
    //  sets cursorX to 0
    REQUIRE(b.cursorX == 0);

    //right
    b.moveCursor(UP, 2); //setup (0,2) now
    b.moveCursor(LEFT); //setup (0,0) now
    REQUIRE_NOTHROW(b.moveCursor(RIGHT, 6));
    REQUIRE_THROWS(b.moveCursor(RIGHT));


}

