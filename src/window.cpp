#include "window.h"
#include "buffer.h"
#include <ncurses.h>
#include <stdio.h>
#include <cmath>

int Window::computeScroll(Buffer &b){
    /* returns:
     *    0 if no scroll needed since last render,
     *    -1 if scroll up or first render
     *    1  if scroll down
     */
    if(b.cursorY < last.start)
        return -1;
    if(b.cursorY > last.end)
        return 1;
    return 0;
}

void Window::computeBufferSegment(int scroll, Buffer &b, BufferBlit &rv){
    int rowsRemaining = height;
    int i;
    if(scroll < 0)
        i = rv.start = b.cursorY;
    else
        i = rv.end = b.cursorY;

    while(rowsRemaining > 0 && i>=0 && i<b.contents.size()){
        int n = b.contents[i].size();
        int rowsUsed = (n+width-1)/width;
        if (rowsUsed == 0) rowsUsed = 1;
        rowsRemaining -= rowsUsed;
        if(rowsRemaining > 0){
            if(scroll < 0){
                rv.end = i;
                i++;
            }else{
                rv.start = i;
                i--;
            }
        }
    }
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
    for(int i = rv.start; i < b.cursorY; i++){
        rv.cursorY += (b.contents[i].size() / width) + 1;
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
    for(int i = b.start; i <= b.end; i++){
        addstr(b.buf->contents[i].c_str());
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
