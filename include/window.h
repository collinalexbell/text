#ifndef WINDOW_H
#define WINDOW_H

#include "buffer.h"
#include <ncurses.h>
#include <stdio.h>

using namespace std;

struct BufferBlit {
    Buffer *buf = NULL;
    int cursorX, cursorY;
    vector<string>::iterator start, end;
    bool moveCursor, shouldBlit;
};

struct Window {

    int height, width;
    int nRowsBlitted;

    BufferBlit last;

    Window(){
        getmaxyx(stdscr, height, width);
    }
    void blit(BufferBlit b);
    int computeScroll(Buffer &b);
    void computeBufferSegment(int scroll, Buffer &b, BufferBlit &rv);
    BufferBlit computeBlit(Buffer &b);
    void display(Buffer &b);
    void moveCursor(BufferBlit b);
};

#endif
