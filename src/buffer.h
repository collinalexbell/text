#ifndef BUFFER_H
#define BUFFER_H

#include <list>
#include <string>

using namespace std;

enum BufferMode {INSERT, NORMAL, COMMAND};
enum Direction {
    UP, DOWN, LEFT, RIGHT,
    BEGINNING_OF_LINE, END_OF_LINE,
    NULLDIR};

struct CopyBuffer {
  bool isLine;
  string contents;
};

class Buffer{
  public:
    string filename;
    bool contentsChangedB = true;
    int cursorXReset = 0;
    bool contentsChanged(){
        bool rv = contentsChangedB;
        contentsChangedB = false;
        return rv;
    };
    int cursorX = 0;
    list<string> contents;
    list<string>::iterator cursorY;
    struct CopyBuffer copy_buffer{false, ""};
    Direction lastYMove = UP;
    BufferMode mode = NORMAL;

    Buffer(string contents);
    Buffer(char* fname);
    int cursorXBound();
    void delete_at_cursor();
    void delete_line();
    void paste_after();

    void find_character_forward(char c);


    //--------------------
    int findBeginningOfLine();
    /* returns the x-index
     * that is the index of
     * the 1st non-whitespace
     * character in the line
     *
     * eg. Vim's 'I' keybind
     */
    //--------------------


    //--------------------
    int findEndOfLine();
    /* returns the x-index 
     * that is 1+index of 
     * the last character
     *
     * eg. Vim's 'A' keybind
     */
    //--------------------


    void initContents(string raw);
    void insertAtCursor(char ch);
    void insertLineAboveCursor();
    void insertLineAfterCursor();
    void joinLineAtCursor();
    Direction orderRelativeToCursor(list<string>::iterator it);
    void resetXCursorToCacheIfNeeded();
    void moveXCursorToLineEndAndCacheIfNeeded();
    void moveCursor(Direction d, int amount=1);
    void moveCursor(int row);
    virtual void save();
};

#endif
