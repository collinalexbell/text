#include "InputProcessor.h"
#include "catch_amalgamated.hpp"
#include "buffer.h"
#include "interface.h"
#include <stdio.h>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

using namespace std;

class MockBuffer: public Buffer {
  public:
    MockBuffer(string empty = "readme\n"): Buffer(empty){}
    void save() override {}
};

class TestingInterface: public Interface {
  char *in;
  public:
    int y, x;
    TestingInterface(char *in): in(in) {
      this->y = -1;
    }
    char getChar() {      if(*in != '\0'){
        char rv = *in;
        in++;
        return rv;
      } else {
        return 0;
      }
    }
    int move(int y, int x) {
      this->y = y;
      this->x = x;
    }
    void moveAndWriteString (int y, int x, char* str) {
      this->y = y;
      this->x = x;
    }
    void moveAndReadString (int y, int x, char* str) {
      this->y = y;
      this->x = x;
    }

    void getCursorPosition (int &y, int &x) {
      y = this->y;
      x = this->x;
    }
};

TEST_CASE("ex_command_mode quit", "[command]"){
  Buffer *b = new MockBuffer();

  char *in = (char*)malloc(3);
  strcpy(in, "q\n");
  Interface *interface = new TestingInterface(in);
  InputProcessor inputProcessor(interface);

  Command cmd = inputProcessor.ex_command_mode(*b);
  REQUIRE(cmd == QUIT);
  delete b;
}

TEST_CASE("command mode <del>"){
  Buffer *b = new MockBuffer();
  
  char *in = (char*)malloc(3);
  strcpy(in, "a\x7Fq\n");
  Interface *interface = new TestingInterface(in);
  InputProcessor inputProcessor(interface);
  
  Command cmd = inputProcessor.ex_command_mode(*b);
  REQUIRE(cmd == QUIT);
  delete b;  
}


TEST_CASE("command moves cursor of interface back to buffer cursor when on second line"){
  Buffer *b = new MockBuffer("readme\nreadme\nreadme\n");
  char *in = (char*)malloc(2);
  strcpy(in, "1\n");
  TestingInterface *interface = new TestingInterface(in);
  InputProcessor inputProcessor(interface);
  interface->move(1,0);

  Command cmd = inputProcessor.ex_command_mode(*b);
  REQUIRE(interface->y == 1);
  delete b;
}

TEST_CASE("'x' should delete the character the cursor is on") {
  string contents = "asdf\n";
  Buffer b = Buffer(contents);

  char *in = (char*)malloc(1);
  TestingInterface *interface = new TestingInterface(in);
  InputProcessor inputProcessor(interface);

  // look in InputProcessor.cpp to understand what I need here
  inputProcessor.normalModeInput(b, 'l', "");
  inputProcessor.normalModeInput(b, 'x', "");

  // look in buffer.h to understand what I need here
  string new_contents = b.contents.front();
  REQUIRE(new_contents.compare(string("adf")) == 0);
}

Buffer make_buffer_and_handle_commands(string bufferContents, const char* inputs, int num_commands = 1) {
  Buffer b = Buffer(bufferContents);
  char *in = (char*)malloc(100);
  strcpy(in, inputs);
  TestingInterface *interface = new TestingInterface(in);
  InputProcessor inputProcessor(interface);
  for(int i=0; i<num_commands; i++){
    inputProcessor.handle_commands(b);
  }
  return b;
}

TEST_CASE("'f' should find matching next character in line"){
  string contents = "asdf\n";
  SECTION("character exists"){
    Buffer b = make_buffer_and_handle_commands(contents, "fd");
    REQUIRE(b.cursorX == 2);
  }
  SECTION("character doesn't exist"){
    Buffer b = make_buffer_and_handle_commands(contents, "fg");
    REQUIRE(b.cursorX == 0);
  }
}

TEST_CASE("'F' should find matching previous character in line"){
  string contents = "asdf\n";
  SECTION("character exists"){
    Buffer b = make_buffer_and_handle_commands(contents, "llFa", 3);
    REQUIRE(b.cursorX == 0);
  }
  SECTION("character doesn't exist"){
    Buffer b = make_buffer_and_handle_commands(contents, "llFz", 3);
    REQUIRE(b.cursorX == 2);
  }
}

TEST_CASE("'p' should paste"){
  SECTION("paste after 'dd'"){
    string contents = "asdf\nqwerty\n";
    Buffer b = make_buffer_and_handle_commands(contents, "ddp", 2);
    REQUIRE(*b.cursorY == string("asdf"));
    REQUIRE(b.contents.front() == string("qwerty"));
    b.contents.pop_front();
    REQUIRE(b.contents.front() == string("asdf"));

  }
  SECTION("paste after 'x'"){
    string contents = "asdf\n";
    Buffer b = make_buffer_and_handle_commands(contents, "xp", 2);
    REQUIRE(b.contents.front() == string("sadf"));
    REQUIRE(b.cursorX == 1);
  }

  SECTION("paste before 'dd'"){
    string contents = "asdf\nqwerty\n";
    Buffer b = make_buffer_and_handle_commands(contents, "jddP", 3);
    REQUIRE(*b.cursorY == string("qwerty"));
    REQUIRE(b.contents.front() == string("asdf"));
    b.contents.pop_front();
    REQUIRE(b.contents.front() == string("qwerty"));
  }
  SECTION("paste after 'yy'"){
    string contents = "asdf\n";
    Buffer b = make_buffer_and_handle_commands(contents, "yyP", 2);
    REQUIRE(*b.cursorY == string("asdf"));
    REQUIRE(b.contents.size() == 2);
    REQUIRE(b.contents.front() == string("asdf"));
    b.contents.pop_front();
    REQUIRE(b.contents.front() == string("asdf"));    
  }
}

TEST_CASE("d<x> should delete"){
  SECTION("d4d"){
    string contents = "abc\ndef\nghi\nlmn\nopq\nrst\nuvw\n";
    Buffer b = make_buffer_and_handle_commands(contents, "jd4d",2);
    REQUIRE(*b.cursorY == string("rst"));
    REQUIRE(b.contents.size() == 3);
    REQUIRE(b.contents.front() == string("abc"));
  }
}

TEST_CASE("'r' should replace"){
  string contents = "a\n";
  Buffer b = make_buffer_and_handle_commands(contents, "rs", 1);
  REQUIRE(b.cursorY->at(b.cursorX) == 's');
}