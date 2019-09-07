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
    vector<string> contents;
    bool blitContents = true;
    int cursorY = 0;
    int cursorX = 0;
    BufferMode mode = NORMAL;
    Buffer(char* fname){
        ifstream in(fname);
        string raw = string(std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>());
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

    string toString(){
        string rv = string();
        for(int i=0; i<contents.size(); i++){
            if(i>0 && i<contents.size()-1){
                rv.append("\n");
            }
            rv.append(contents[i]);
        }
        return rv;
    }

    void moveCursor(Direction d){
        switch(d){
            case UP:
                if(cursorY>0){
                    cursorY--;
                }
                break;
            case DOWN:
                cursorY++;
                break;
            case LEFT:
                if(cursorX>0) cursorX--;
                break;
            case RIGHT:
                cursorX++;
            default:
                break;
        }
    }
};

#endif
