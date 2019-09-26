#include "window.h"
#include "buffer.h"
#include <ncurses.h>
#include <stdio.h>
#include <cmath>
#include <iterator>

int Window::computeScroll(Buffer &b){
    /* returns:
     *    0 if no scroll needed since last render,
     *    -1 if scroll up or first render
     *    1  if scroll down
     */
    vector<string>::iterator cursor;
    cursor = b.contents.begin() + b.cursorY;
    if(cursor < last.start)
        return -1;
    if(cursor > last.end)
        return 1;
    return 0;
}

void scrollUp(Buffer &b, BufferBlit &rv, int width, int height){
    int rowsRemaining = height;
    vector<string>::iterator it;
    it = rv.start = b.contents.begin() + b.cursorY;
    while(rowsRemaining > 0 && it != b.contents.end()){
        int n = it->size();
        int rowsUsed = (n+width-1)/width;
        if (rowsUsed == 0) rowsUsed = 1;
        rowsRemaining -= rowsUsed;
        if(rowsRemaining > 0){
                rv.end = it;
                it++;
        }
    }
}

void scrollDown(Buffer &b, BufferBlit &rv, int width, int height){
    int rowsRemaining = height;
    vector<string>::reverse_iterator it;
    rv.end = b.contents.begin() + b.cursorY;
    it = vector<string>::reverse_iterator(rv.end);
    while(rowsRemaining > 0 && it != b.contents.rend()){
        int n = it->size();
        int rowsUsed = (n+width-1)/width;
        if (rowsUsed == 0) rowsUsed = 1;
        rowsRemaining -= rowsUsed;
        if(rowsRemaining > 0){
            rv.start = it.base();
            it++;
        }
    }
}

void Window::computeBufferSegment(int scroll, Buffer &b, BufferBlit &rv){
    if(scroll < 0)
        scrollUp(b, rv, width, height);
    else
        scrollDown(b, rv, width, height);
}

BufferBlit Window::computeBlit(Buffer &b){
    int scroll;
    if(!last.buf)
        scroll = -1;
    else
        scroll = computeScroll(b);
    BufferBlit rv;

    if(scroll == 0){
        rv.start = last.start;
        rv.end = last.end;
    } else {
        computeBufferSegment(scroll, b, rv);
    }

    rv.cursorX = 0;
    rv.cursorY = 0;
    vector<string>::iterator it;
    for(it = rv.start;
            it < b.contents.begin() + b.cursorY; it++){
        rv.cursorY += (it->size() / width) + 1;
    }
    rv.cursorX = b.cursorX % width;
    rv.cursorY += b.cursorX / width;


    if(scroll != 0 || b.contentsChanged())
        rv.shouldBlit = true;
    else
        rv.shouldBlit = false;

    bool cursorXChanged = last.buf && rv.cursorX != last.cursorX;
    bool cursorYChanged = last.buf && rv.cursorY != last.cursorY;
    if(rv.shouldBlit || cursorXChanged || cursorYChanged)
        rv.moveCursor = true;
    else
        rv.moveCursor = false;


    rv.buf = &b;
    last = rv;
    return rv;
}

void Window::blit(BufferBlit b){
    clear();
    vector<string>::iterator it;
    for(it = b.start; it <= b.end; it++){
        addstr(it->c_str());
        addstr("\n");
    }
}

void Window::moveCursor(BufferBlit b){
    move(b.cursorY, b.cursorX);
}

void Window::display(Buffer &b){
    BufferBlit bb = computeBlit(b);
    if(bb.shouldBlit)
        blit(bb);
    if(bb.moveCursor)
        moveCursor(bb);
}
