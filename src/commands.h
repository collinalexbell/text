#ifndef COMMANDS_H
#define COMMANDS_H

#include "buffer.h"
#include "interface.h"

enum Command {QUIT, UNKNOWN};

Command ex_command_mode(Buffer &b, Interface *interface);
bool handle_commands(Buffer &b);
bool normalModeInput(Buffer &b, char ch, string state);

#endif
