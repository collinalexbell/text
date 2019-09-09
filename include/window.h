#ifndef WINDOW_H
#define WINDOW_H

#include <ncurses.h>
#include <stdio.h>

struct Window {
    int height, width;
    int rowOffset;

    Window(){
        rowOffset = 0;
        getmaxyx(stdscr, height, width);
    }

    bool moveCursor(int cursorY){
        if(cursorY >= rowOffset+height){
            rowOffset = cursorY-(height-1);
            return true;
        }

        if(cursorY >= 0 && cursorY < rowOffset){
            rowOffset = cursorY;
            return true;
        }
        return false;
    }
};

#endif
