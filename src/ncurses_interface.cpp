#include "interface.h"
#include <ncurses.h>
#include <stdio.h>
#include <string.h>

char NcursesInterface::getChar(){
  return getch();
}

int NcursesInterface::move(int y, int x) {
  return ::move(y, x);
}

void NcursesInterface::moveAndReadString(int y, int x, char* str){
  int str_len = strlen(str);
  chtype ncurses_str[str_len+1];
  mvinchstr(y, x, ncurses_str);
  for(int i=0; i<str_len+1; i++) {
    str[i] = ncurses_str[i];
  }
}
void NcursesInterface::moveAndWriteString(int y, int x, char* str){
  mvaddstr(y, x, str);
}
void NcursesInterface::getCursorPosition(int &y, int &x){
  getyx(stdscr, y, x);
}
