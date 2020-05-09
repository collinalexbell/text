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
    MockBuffer(char* empty = "readme"): Buffer(empty){}
    void save() override {}
};

class TestingInterface: public Interface {
  char *in;
  public:
    TestingInterface(char *in): in(in) {}
    char getChar() {
      if(*in != '\0'){
        char rv = *in;
        in++;
        return rv;
      } else {
        return 0;
      }
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

