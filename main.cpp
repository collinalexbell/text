#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>      // std::stringstream
#include <unistd.h>

#include <ncurses.h>

#include "buffer.h"

using namespace std;

enum Command {QUIT, UNKNOWN};

void display(Buffer &b){
    if(b.blitContents){
        clear();
        addstr(b.toString().c_str());
        b.blitContents = false;
    }
    move(b.cursorY, b.cursorX);
	refresh();
}

char* getFileToOpen(int argc, char** argv){
    if (argc >= 2){
        return argv[1];
    }

    printf("no file to open\n");
    exit(1);
}

Command ex_command_mode(){
    int row, col;
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

void normalModeInput(Buffer &b, char ch){
    try{
        switch(ch){
            case 'k':
                b.moveCursor(UP);
                break;
            case 'j':
                b.moveCursor(DOWN);
                break;
            case 'h':
                b.moveCursor(LEFT);
                break;
            case 'l':
                b.moveCursor(RIGHT);
                break;
            case 'c':
                cursorUnderscore();
                break;
            case 'i':
                b.mode = INSERT;
                cursorLine();
            default:
                break;
        }
    } catch(const char* e){
        beep();
    }
}

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

int main(int argc, char** argv){
    char* fname = getFileToOpen(argc, argv);
    Buffer buffer = Buffer(fname);
    initscr();			/* Start curses mode 		  */
    noecho();

    bool quit = false;

    do{
        display(buffer);
        quit = handle_commands(buffer);
    } while(!quit);
	endwin();			/* End curses mode		  */
    cursorBlock();

	return 0;
}
