#ifndef COMMANDS_H
#define COMMANDS_H

#include "buffer.h"

enum Command {QUIT, UNKNOWN};

Command ex_command_mode();
bool handle_commands(Buffer &b);
void insertModeInput(Buffer &b, char ch);
void normalModeInput(Buffer &b, char ch);

#endif
