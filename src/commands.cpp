#include "buffer.h"
#include "commands.h"
#include "cursor.h"
#include <ncurses.h>

using namespace std;
void normalModeInput(Buffer &b, char ch);
void insertModeInput(Buffer &b, char ch);

bool handle_commands(Buffer &b){
     char ch = getch();
     if(ch == ':'){
         Command cmd = ex_command_mode();
         if(cmd == QUIT){
             return true;
         }
     }

     if(b.mode == NORMAL) normalModeInput(b, ch);
     else if(b.mode == INSERT) insertModeInput(b, ch);

     return false;
}

Command ex_command_mode(){ int row, col;
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
    return UNKNOWN;
}

void normalModeInput(Buffer &b, char ch, string state){
    state += ch;
    try{
        if(state == "k") moveCursor(b, UP);
        if(state == "j") moveCursor(b, DOWN);
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
        if(state == "g") normalModeInput(b, getch(), state);
        if(state == "gg") moveCursor(b, 0);
    } catch(const char* e){
        beep();
    }
}

void normalModeInput(Buffer &b, char ch){
    normalModeInput(b, ch, "");
}

void insertModeInput(Buffer &b, char ch){
    cursorLine();
    switch(ch){
        case 27:
            b.mode = NORMAL;
            if (b.cursorX > 0) b.moveCursor(LEFT);
            cursorBlock();
            break;
        default:
            b.insertAtCursor(ch);
            b.moveCursor(RIGHT);
    }
}
