#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>      // std::stringstream
#include <unistd.h>

#include <ncurses.h>

#include "window.h"
#include "buffer.h"
#include "InputProcessor.h"
#include "cursor.h"

using namespace std;

char* getFileToOpen(int argc, char** argv){
    if (argc >= 2){
        return argv[1];
    }

    printf("no file to open\n");
    exit(1);
}

int main(int argc, char** argv){
    Interface *interface = new NcursesInterface();
    InputProcessor inputProcessor = InputProcessor(interface);
    char* fname = getFileToOpen(argc, argv);
    Buffer buffer = Buffer(fname);
    initscr();			/* Start curses mode 		  */
    Window window = Window();
    noecho();

    bool quit = false;

    do{
        window.display(buffer);
        quit = inputProcessor.handle_commands(buffer);
    } while(!quit);
    endwin();			/* End curses mode		  */
    cursorBlock();
    delete interface;
    return 0;
}
