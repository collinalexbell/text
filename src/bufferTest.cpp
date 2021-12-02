#include "buffer.h"
#include "catch.hpp"

/*
 int Buffer::findBeginningOfLine(){
    string line = *cursorY;
    smatch match;
    regex r("\\S");
    regex_search(line, match, r);
    if(match.empty()) return 0;
    return match.position();
}
 */

TEST_CASE("findBeginningOfLine() ", "[buffer]") {
  string contents = "foo\nbar\nbaz\n";
  Buffer b(contents);

  b.moveCursor(RIGHT);
  b.moveCursor(RIGHT);
  REQUIRE_NOTHROW(
      [&b](){if(b.cursorX != 2){throw "fail";}}()
  );
  b.findBeginningOfLine();
}

TEST_CASE("moveCursor(UP) doesn't allow movement out of contents bounds",
        "[buffer]"){
    string contents = "foo bar\nbaz bin\n\n";
    Buffer b(contents);

    b.moveCursor(DOWN);
    REQUIRE_NOTHROW(b.moveCursor(UP));
    REQUIRE_THROWS(b.moveCursor(UP));
}

TEST_CASE("moveCursor(LEFT) doesn't allow movement out of contents bounds",
        "[buffer]"){

    string contents = "foo bar\nbaz bin\n\n";
    Buffer b(contents);

    b.moveCursor(RIGHT);
    REQUIRE_NOTHROW(b.moveCursor(LEFT));
    REQUIRE_THROWS(b.moveCursor(LEFT));
}

TEST_CASE("moveCursor(DOWN) doesn't allow movement out of contents bounds",
        "[buffer]"){

    string contents = "foo bar\nbaz bin\n\n";
    Buffer b(contents);

    b.moveCursor(RIGHT); // setup for last REQUIRE in this section
    REQUIRE_NOTHROW(b.moveCursor(DOWN, 2));
    REQUIRE_THROWS(b.moveCursor(DOWN));
    //check that moving from row with 7 chars to row with 0 chars:
    //  sets cursorX to 0
    REQUIRE(b.cursorX == 0);
}

TEST_CASE("moveCursor(RIGHT) doesn't allow movement out of contents bounds",
        "[buffer]"){

    string contents = "foo bar\nbaz bin\n\n";
    Buffer b(contents);

    REQUIRE_NOTHROW(b.moveCursor(RIGHT, 6));
    REQUIRE_THROWS(b.moveCursor(RIGHT));
}

