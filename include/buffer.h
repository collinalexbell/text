#ifndef BUFFER_H
#define BUFFER_H
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>      // std::stringstream

using namespace std;


enum BufferMode {INSERT, NORMAL, COMMAND};
enum Direction {UP, DOWN, LEFT, RIGHT};

struct Buffer{
    int cursorXReset = 0;
    vector<string> contents;
    bool blitContents = true;
    int cursorY = 0;
    int cursorX = 0;
    BufferMode mode = NORMAL;

    Buffer(string contents){
        initContents(contents);
    }

    Buffer(char* fname){
        ifstream in(fname);
        string raw = string(std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>());
        initContents(raw);
    }

    void initContents(string raw){
        string lineBuffer;
        stringstream ss(raw);
        while (getline(ss, lineBuffer)){
            contents.push_back(string(lineBuffer));
        }
    }

    void insertAtCursor(char ch){
        bool lineExists = cursorY < contents.size();
        //colExists uses <= because cursor could insert at end.
        bool colExists  = cursorX <= contents[cursorY].size();
        if(lineExists && colExists){
            contents[cursorY].insert(cursorX, 1, ch);
        }
        blitContents = true;
    }

    string toString(int offset, int numRows){
        string rv = string();
        if(numRows+offset > contents.size()){
            numRows = contents.size()-offset;
        }
        for(int i=offset; i<numRows+offset; i++){
            if(i>offset && i<contents.size()){
                rv.append("\n");
            }
            rv.append(contents[i]);
        }
        return rv;
    }

    string toString(){
        return toString(0, contents.size());
    }

    void resetXCursorToCacheIfNeeded(){
        if(cursorXReset > 0){
            cursorX = cursorXReset;
            cursorXReset = -1;
        }
    }

    void moveXCursorToLineEndAndCacheIfNeeded(){
        //move left if line not long enough
        if(cursorX >= contents[cursorY].size()){
            cursorXReset = cursorX;
            cursorX = contents[cursorY].size()-1;
            if(cursorX < 0) cursorX = 0;
        }
    }

    void moveCursor(Direction d, int amount=1){
        switch(d){
            case UP:
                if(cursorY - amount < 0)
                    throw "can not move cursor up";
                cursorY -= amount;
                resetXCursorToCacheIfNeeded();
                moveXCursorToLineEndAndCacheIfNeeded();
                break;
            case DOWN:
                if(cursorY + amount >= contents.size())
                    throw "can not move cursor dwn";
                cursorY += amount;
                resetXCursorToCacheIfNeeded();
                moveXCursorToLineEndAndCacheIfNeeded();
                break;
            case LEFT:
                if(cursorX - amount < 0)
                    throw "can not move cursor left";
                cursorX -= amount;
                cursorXReset = -1; //sentinel
                break;
            case RIGHT:
                if(cursorX + amount >= contents[cursorY].size())
                    throw "can not move cursor right";
                cursorX += amount;
                cursorXReset = -1; //sentinel
                break;
            default:
                break;
        }
    }

    void moveCursor(int row){
        if(row >= 0 && row < contents.size()){
            cursorY = row;
        }
    }
};

#endif
