#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <ncurses.h>

using namespace std;

struct Buffer{
    string contents;
    bool blitContents = true;
    int cursorY = 0;
    int cursorX = 0;
    Buffer(char* fname){
        ifstream in(fname);
        contents = string(std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>());
    }
};

enum Command {QUIT, UNKNOWN};

void display(Buffer &b){
    if(b.blitContents){
        clear();
        addstr(b.contents.c_str());
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

enum Direction {UP, DOWN, LEFT, RIGHT};

void moveCursor(Buffer &buf, Direction d){
    switch(d){
        case UP:
            if(buf.cursorY>0){
                buf.cursorY--;
            }
            break;
        case DOWN:
            buf.cursorY++;
            break;
        case LEFT:
            if(buf.cursorX>0) buf.cursorX--;
            break;
        case RIGHT:
            buf.cursorX++;
        default:
            break;
    }
    refresh();
}

bool handle_commands(Buffer &b){
     char ch = getch();
     if(ch == ':'){
         Command cmd = ex_command_mode();
         if(cmd == QUIT){
             return true;
         }
     }
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
         default:
             break;
     }

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

	return 0;
}
