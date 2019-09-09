#ifndef CURSOR_H
#define CURSOR_H
#include "buffer.h"

void cursorUnderscore();
void cursorLine();
void cursorBlock();
void moveCursor(Buffer &b, Direction d);
void moveCursor(Buffer &b, int row);

#endif
