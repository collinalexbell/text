#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <ncurses.h>

using namespace std;

struct Buffer{
    string contents;
    Buffer(char* fname){
        ifstream in(fname);
        contents = string(std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>());
    }
};

enum Command {QUIT, UNKNOWN};

void display(Buffer &b){
    clear();
    addstr(b.contents.c_str());
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

bool handle_commands(Buffer b){
     char ch = getch();
     if(ch == ':'){
         Command cmd = ex_command_mode();
         if(cmd == QUIT){
             return true;
         }
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


