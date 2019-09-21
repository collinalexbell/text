#ifndef COMMANDS_H
#define COMMANDS_H

#include "buffer.h"

enum Command {QUIT, UNKNOWN};

Command ex_command_mode(Buffer &b);
bool handle_commands(Buffer &b);

#endif
