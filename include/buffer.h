#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <string>

using namespace std;


enum BufferMode {INSERT, NORMAL, COMMAND};
enum Direction {
    UP, DOWN, LEFT, RIGHT,
    BEGINNING_OF_LINE, END_OF_LINE};

struct Buffer{
    int cursorXReset = 0;
    vector<string> contents;
    bool contentsChangedB = true;
    bool contentsChanged(){
        bool rv = contentsChangedB;
        contentsChangedB = false;
        return rv;
    };
    int cursorY = 0;
    int cursorX = 0;
    BufferMode mode = NORMAL;

    Buffer(string contents);
    Buffer(char* fname);
    int cursorXBound(int lineNo);
    void deleteAtCursor();
    int findBeginningOfLine(int lineNo);
    int findEndOfLine(int lineNo);
    void initContents(string raw);
    void insertAtCursor(char ch);
    void insertLineAboveCursor();
    void insertLineAfterCursor();
    void joinLineAtCursor();
    string toString(int offset, int numRows);
    string toString();
    void resetXCursorToCacheIfNeeded();
    void moveXCursorToLineEndAndCacheIfNeeded();
    void moveCursor(Direction d, int amount=1);
    void moveCursor(int row);
};

#endif
