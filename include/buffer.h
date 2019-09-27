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

struct Buffer{
    string filename;
    int cursorXReset = 0;
    list<string> contents;
    bool contentsChangedB = true;
    bool contentsChanged(){
        bool rv = contentsChangedB;
        contentsChangedB = false;
        return rv;
    };
    int cursorX = 0;
    list<string>::iterator cursorY;
    Direction lastYMove = UP;
    BufferMode mode = NORMAL;

    Buffer(string contents);
    Buffer(char* fname);
    int cursorXBound();
    void deleteAtCursor();
    int findBeginningOfLine();
    int findEndOfLine();
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
    void save();
};

#endif
