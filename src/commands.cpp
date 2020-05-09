#include "buffer.h"
#include "commands.h"
#include "cursor.h"
#include <ncurses.h>

using namespace std;
bool normalModeInput(Buffer &b, char ch);
void insertModeInput(Buffer &b, char ch);

bool handle_commands(Buffer &b){
     char ch = getch();

     if(b.mode == NORMAL) {
       bool quit = normalModeInput(b, ch);
       if(quit) {
         return true;
       }
     }
     else if(b.mode == INSERT) insertModeInput(b, ch);

     return false;
}

Command ex_command_mode(Buffer &b){ int row, col;
    getmaxyx(stdscr, row, col);
    chtype last_line[col+1];

    row = row-1;
    mvinchstr(row, 0, last_line);
    clrtoeol();

    mvaddch(row, 0, ':');
    refresh();

    string cmd;
    int cmd_i=0;

    char c;

    while(cmd_i<20 && c != 10){
        c = getch();
        addch(c);
        refresh();
        if(c != 10) cmd+=c;
        cmd_i++;

    }

    mvaddchstr(row, 0, last_line);
    refresh();
    if(cmd == "q"){
        return QUIT;
    }
    if(cmd == "wq"){
        b.save();
        return QUIT;
    }
    if(cmd == "w"){
        b.save();
    }
    return UNKNOWN;
}

bool normalModeInput(Buffer &b, char ch, string state){
    state += ch;
    try{
        if(state == "O"){
            b.insertLineAboveCursor();
            b.moveCursor(UP);
            b.moveCursor(BEGINNING_OF_LINE);
            b.mode = INSERT;
            cursorLine();
        }
        if(state == "o"){
            b.moveCursor(END_OF_LINE);
            b.insertLineAfterCursor();
            b.mode = INSERT;
            cursorLine();
            b.moveCursor(DOWN);
        }
        if(state == "k") moveCursor(b, UP);
        if(state == "j") moveCursor(b, DOWN);
        if(state == "J") b.joinLineAtCursor();
        if(state == "h") moveCursor(b, LEFT);
        if(state == "l") moveCursor(b, RIGHT);
        if(state == "G") moveCursor(b, b.contents.size()-1);
        if(state == "c") cursorUnderscore();
        if(state == "i") {
            b.mode = INSERT;
            cursorLine();
        }
        if(state == "I") {
            b.mode = INSERT;
            b.moveCursor(BEGINNING_OF_LINE);
            cursorLine();
        }
        if(state == "a") {
            b.mode = INSERT;
            b.moveCursor(RIGHT);
            cursorLine();
        }
        if(state == "A"){
            b.mode = INSERT;
            b.moveCursor(END_OF_LINE);
            cursorLine();
        }
        if(state == "g") normalModeInput(b, getch(), state);
        if(state == "gg") moveCursor(b, 0);
        if(state == ":"){
          Command cmd = ex_command_mode(b);
          if(cmd == QUIT){
            return true;
          }
        }
    } catch(const char* e){
        beep();
    }
    return false;
}

bool normalModeInput(Buffer &b, char ch){
    return normalModeInput(b, ch, "");
}

void insertModeInput(Buffer &b, char ch){
    cursorLine();
    switch(ch){
        //Backspace
        case 8:
        case 127:
            if(b.cursorX == 0){
                b.moveCursor(UP);
                b.moveCursor(END_OF_LINE);
                b.joinLineAtCursor();
            } else {
                b.moveCursor(LEFT);
                b.deleteAtCursor();
            }
            break;
        //Escape
        case 27:
            b.mode = NORMAL;
            if (b.cursorX > 0) b.moveCursor(LEFT);
            cursorBlock();
            break;
        //Enter
        case 10:
        case 13:
            b.insertLineAfterCursor();
            b.moveCursor(DOWN);
            b.moveCursor(BEGINNING_OF_LINE);
            break;
        default:
            b.insertAtCursor(ch);
            b.moveCursor(RIGHT);
    }
}
