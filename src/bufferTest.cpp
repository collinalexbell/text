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

TEST_CASE("delete_at_cursor() ", "[buffer]") {
  string contents = "foo\n";
  Buffer b(contents);

  b.moveCursor(RIGHT);
  REQUIRE_NOTHROW(
      [&b](){if(b.cursorX != 1){throw "givens fail to hold";}}()
  );
  b.delete_at_cursor();
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

  SECTION("finds the character") {
    string contents = "asdf";
    Buffer b(contents);
    b.moveCursor(RIGHT);
    b.find_character_forward('d');
    REQUIRE(b.cursorX == 2);
  }

  SECTION("finds the next character if currently on the character") {
    string contents = "aa";
    Buffer b(contents);
    b.find_character_forward('a');
    REQUIRE(b.cursorX == 1);
  }
}

TEST_CASE("find_character_backward()") {
  SECTION("finds the character") {
    string contents = "asdf";
    Buffer b(contents);
    b.moveCursor(RIGHT);
    b.moveCursor(RIGHT);
    b.find_character_backward('a');
    REQUIRE(b.cursorX == 0);
  }

  SECTION("finds the next character if currently on the character") {
    string contents = "aa";
    Buffer b(contents);
    b.moveCursor(RIGHT);
    b.find_character_backward('a');
    REQUIRE(b.cursorX == 0);
  }

  SECTION("doesn't break when cursorX is 0"){
    string contents = "a";
    Buffer b(contents);
    b.find_character_backward('a');
    REQUIRE(b.cursorX == 0);
  }
}

TEST_CASE("paste()") {
  SECTION("paste_after() following a delete_line()"){
    string contents = "asdf\nqwerty\n";
    Buffer b(contents);
    b.delete_line();
    b.paste_after();

    REQUIRE(*b.cursorY == string("asdf"));
    REQUIRE(b.contents.front() == string("qwerty"));
    b.contents.pop_front();
    REQUIRE(b.contents.front() == string("asdf")); 
  }
  SECTION("paste_after() following a delete_at_cursor()"){
    string contents = "asdf\n";
    Buffer b(contents);
    b.delete_at_cursor();
    b.paste_after();

    REQUIRE(b.contents.front() == string("sadf")); 
    REQUIRE(b.cursorX == 1);
  }
}
