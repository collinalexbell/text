#ifndef WINDOW_H
#define WINDOW_H

#include "buffer.h"
#include <ncurses.h>
#include <stdio.h>
#include <list>
#include <string>

using namespace std;

struct BufferBlit {
    Buffer *buf = NULL;
    int cursorX, cursorY;
    list<string>::iterator start, end;
    bool moveCursor, shouldBlit;
};

enum Color {white=1, green=2};

struct Window {

    int height, width;
    int nRowsBlitted;

    BufferBlit last;

    Window(){
        getmaxyx(stdscr, height, width);
        initColor();
    }
    void initColor();
    void changeColor(Color color);
    void blit(BufferBlit b);
    int computeScroll(Buffer &b);
    void computeBufferSegment(int scroll, Buffer &b, BufferBlit &rv);
    BufferBlit computeBlit(Buffer &b);
    void display(Buffer &b);
    bool isInFrame(Buffer &b);
    void moveCursor(BufferBlit b);
};

#endif