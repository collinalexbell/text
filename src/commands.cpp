#include "buffer.h"
#include "commands.h"
#include "cursor.h"
#include <ncurses.h>

using namespace std;

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

void normalModeInput(Buffer &b, char ch){
    try{
        switch(ch){
            case 'k':
                moveCursor(b, UP);
                break;
            case 'j':
                moveCursor(b, DOWN);
                break;
            case 'h':
                moveCursor(b, LEFT);
                break;
            case 'l':
                moveCursor(b, RIGHT);
                break;
            case 'G':
                moveCursor(b, b.contents.size()-1);
                break;
            case 'c':
                cursorUnderscore();
                break;
            case 'i':
                b.mode = INSERT;
                cursorLine();
                break;
            default:
                break;
        }
    } catch(const char* e){
        beep();
    }
}

void insertModeInput(Buffer &b, char ch){
    cursorLine();
    switch(ch){
        case 27:
            b.mode = NORMAL;
            cursorBlock();
            break;
        default:
            b.insertAtCursor(ch);
    }
}
