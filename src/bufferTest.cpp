#include "buffer.h"
#include "catch.hpp"

TEST_CASE("findBeginningOfLine() ", "[buffer]") {
  string contents = "foo\n  bar\nbaz\n";
  Buffer b(contents);

  b.moveCursor(RIGHT);
  b.moveCursor(RIGHT);
  REQUIRE_NOTHROW(
      [&b](){if(b.cursorX != 2){throw "fail";}}()
  );
  int beginning = b.findBeginningOfLine();
  REQUIRE(beginning == 0); 

  b.moveCursor(DOWN);
  b.moveCursor(RIGHT);
  b.moveCursor(RIGHT);
  REQUIRE_NOTHROW(
      [&b](){if(b.cursorX != 4 && *b.cursorY == "  bar"){throw "givens fail to hold";}}()
  );
  beginning = b.findBeginningOfLine();
  // 'b' of bar. think of it as "I" command in vim
  REQUIRE(beginning == 2);
}

TEST_CASE("findEndOfLine() ", "[buffer]") {
  string contents = "foo\n";
  Buffer b(contents);

  int end = b.findEndOfLine();
  REQUIRE(end == 3); 
}

/*
 *void Buffer::deleteAtCursor(){
    if(cursorY != contents.end() && cursorX < cursorY->size()){
        cursorY->erase(cursorX, 1);
        contentsChangedB = true;
    }
}
*/
TEST_CASE("deleteAtCursor() ", "[buffer]") {
  string contents = "foo\n";
  Buffer b(contents);

  b.moveCursor(RIGHT);
  REQUIRE_NOTHROW(
      [&b](){if(b.cursorX != 1){throw "givens fail to hold";}}()
  );
  b.deleteAtCursor();
  REQUIRE(b.cursorY->compare("fo") == 0);
  REQUIRE(b.contentsChanged()); 
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

TEST_CASE("find_character_forward()") {
  string contents = "asdf";
  Buffer b(contents);
  b.moveCursor(RIGHT);
  b.find_character_forward('d');
  REQUIRE(b.cursorX == 2);
}

