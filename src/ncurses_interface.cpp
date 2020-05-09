#include "interface.h"
#include <ncurses.h>

char NcursesInterface::getChar(){
  return getch();
}
