#ifndef COMMANDS_H
#define COMMANDS_H

#include "buffer.h"
#include "interface.h"

enum Command {QUIT, UNKNOWN};
class InputProcessor{
  private:
    // GuiFacade
    Interface *interface;

  public:
    InputProcessor(Interface* interface);
    Command ex_command_mode(Buffer &b);
    bool handle_commands(Buffer &b);
    bool normalModeInput(Buffer &b, char ch);
    bool normalModeInput(Buffer &b, char ch, string state);
    void insertModeInput(Buffer &b, char ch);
};
#endif
