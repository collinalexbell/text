#include "commands.h"
#include "catch.hpp"
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
    char getChar() {
      if(*in != '\0'){
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

  Command cmd = ex_command_mode(*b, interface);
  REQUIRE(cmd == QUIT);
  delete b;
}

TEST_CASE("command moves cursor of interface back to buffer cursor when on second line"){
  Buffer *b = new MockBuffer("readme\nreadme\nreadme\n");
  char *in = (char*)malloc(2);
  strcpy(in, "1\n");
  TestingInterface *interface = new TestingInterface(in);
  interface->move(1,0);

  Command cmd = ex_command_mode(*b, interface);
  REQUIRE(interface->y == 1);
  delete b;
}

TEST_CASE("'x' should delete the character the cursor is on") {
  string contents = "asdf\n";
  Buffer b = Buffer(contents);

  // look in commands.cpp to understand what I need here
  normalModeInput(b, 'l', "");
  normalModeInput(b, 'x', "");

  // look in buffer.h to understand what I need here
  string new_contents = b.contents.front();
  REQUIRE(new_contents.compare(string("adf")) == 0);
}
