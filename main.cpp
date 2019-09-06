#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

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

char* getFileToOpen(int argc, char** argv){
    if (argc >= 2){
        return argv[1];
    }

    printf("no file to open\n");
    exit(1);
}

int main(int argc, char** argv){
    char* fname = getFileToOpen(argc, argv);
    Buffer buffer = Buffer(fname);

    initscr();			/* Start curses mode 		  */
    addstr(buffer.contents.c_str());

	refresh();			/* Print it on to the real screen */
	getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */

	return 0;
}


