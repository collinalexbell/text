#include <iostream>

#include "cursor.h"
#include "buffer.h"

void cursorUnderscore(){
    printf("\x1b[\x34 q");
    std::cout.flush();
}

void cursorBlock(){
    printf("\x1b[\x32 q");
    std::cout.flush();
}

void cursorLine(){
    printf("\x1b[\x36 q");
    std::cout.flush();
}

void moveCursor(Buffer &b, Direction d){
    b.moveCursor(d);
}

void moveCursor(Buffer &b, int row){
    b.moveCursor(row);
}
