#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <regex>
#include <sstream>      // std::stringstream

#include "buffer.h"

using namespace std;

int Buffer::findBeginningOfLine(){
    string line = *cursorY;
    smatch match;
    regex r("\\S");
    regex_search(line, match, r);
    if(match.empty()) return 0;
    return match.position();
}

int Buffer::findEndOfLine(){
    string line = *cursorY;
    smatch match;
    regex r(".*\\S");
    regex_search(line, match, r);
    if(match.empty()) return 0;
    int start = match.position();
    return start + match[0].length();
}

void Buffer::find_character_forward(char c){
  int pos = cursorY->find(c, cursorX+1); 
  if(pos >= 0) {
    cursorX = pos;
  }
}

void Buffer::find_character_backward(char c){
  string reversed(*cursorY);
  std::reverse(reversed.begin(), reversed.end());
  int reversed_pos = reversed.find(c, reversed.length() - cursorX); 
  int pos = reversed.length() - reversed_pos - 1;
  // reversed pos will contain the sentinel
  if(reversed_pos >= 0) {
    cursorX = pos;
  }
}

Buffer::Buffer(string contents){
    mode = NORMAL;
    initContents(contents);
}

Buffer::Buffer(char* fname){
    mode = NORMAL;
    filename = fname;
    ifstream in(fname);
    string raw = string(std::istreambuf_iterator<char>(in),
            std::istreambuf_iterator<char>());
    initContents(raw);
}

void Buffer::delete_at_cursor(){
    copy_buffer.contents = string(1, (*cursorY)[cursorX]);
    copy_buffer.hasNewline = false;
    if(cursorY != contents.end() && cursorX < cursorY->size()){
        cursorY->erase(cursorX, 1);
        contentsChangedB = true;
    }
}

void Buffer::delete_line(){
  copy_buffer.contents = *cursorY;
  copy_buffer.hasNewline = true;
  cursorY = contents.erase(cursorY);
  contentsChangedB = true;
}

void Buffer::delete_lines(int n){
  copy_buffer.hasNewline = true;
  std::stringstream ss;
  for(int i=0; i<n; i++){
    ss << *cursorY << "\n";
    cursorY = contents.erase(cursorY);
  }
  copy_buffer.contents = ss.str();
  contentsChangedB = true;
}

void Buffer::yank_line(){
  copy_buffer.contents = *cursorY;
  copy_buffer.hasNewline = true;  
}

void Buffer::joinLineAtCursor(){
    if(distance(cursorY, contents.end())>1){
        auto it = cursorY;
        it++;
        cursorY->append(*it);
        contents.erase(it);
        contentsChangedB = true;
    }
}

void Buffer::initContents(string raw){
    string lineBuffer;
    stringstream ss(raw);
    while (getline(ss, lineBuffer)){
        contents.push_back(string(lineBuffer));
    }
    if(contents.size() == 0) contents.push_back("");
    cursorY = contents.begin();

}

void Buffer::insertLineAboveCursor(){
    contents.insert(cursorY, "");
    contentsChangedB = true;
}

void Buffer::insertLineAfterCursor(){
    string line = cursorY->substr(cursorX);
    cursorY->erase(cursorX);
    auto it = cursorY;
    it++;
    contents.insert(it, line);
    contentsChangedB = true;
}

void Buffer::insertAtCursor(char ch){
    bool lineExists = cursorY != contents.end();
    //colExists uses <= because cursor could insert at end.
    bool colExists  = cursorX <= cursorY->size();
    if(lineExists && colExists){
        cursorY->insert(cursorX, 1, ch);
    }
    contentsChangedB = true;
}

void Buffer::replace_at_cursor(char ch){
  if(cursorY->length() > 0) {
    (*cursorY)[cursorX] = ch;
  }
}

void Buffer::resetXCursorToCacheIfNeeded(){
    if(cursorXReset > 0){
        cursorX = cursorXReset;
        cursorXReset = -1;
    }
}

void Buffer::moveXCursorToLineEndAndCacheIfNeeded(){
    //move left if line not long enough
    if(cursorX >= cursorY->size()){
        cursorXReset = cursorX;
        cursorX = cursorY->size()-1;
        if(cursorX < 0) cursorX = 0;
    }
}

int Buffer::cursorXBound(){
    int bound = cursorY->size();
    if(mode == NORMAL)
        //Should be able to insert at line.size(), but not scroll to it.
        bound--;
    return bound;
}

Direction Buffer::orderRelativeToCursor(list<string>::iterator target){
    if(cursorY == target) return NULLDIR;
    Direction direction = UP;
    for(auto it = contents.begin(); it != contents.end(); it++){
        if(it == target){
            return direction;
        }
        if(it == cursorY){
            direction = DOWN;
        }
    }
    return NULLDIR;
}

void Buffer::moveCursor(Direction d, int amount){
    auto begin = contents.begin();
    auto end = --contents.end();
    switch(d){
        case UP:
            for(int i=0; i<amount; i++){
                if(begin == cursorY)
                    throw "can not move cursor up";
                begin++;
            }
            for(int i=0; i<amount; i++)
                cursorY--;
            resetXCursorToCacheIfNeeded();
            moveXCursorToLineEndAndCacheIfNeeded();
            lastYMove = UP;
            break;
        case DOWN:
            for(int i=0; i<amount; i++){
                if(end == cursorY)
                    throw "can not move cursor down";
                end--;
            }
            for(int i=0; i<amount; i++)
                cursorY++;
            resetXCursorToCacheIfNeeded();
            moveXCursorToLineEndAndCacheIfNeeded();
            lastYMove = DOWN;
            break;
        case LEFT:
            if(cursorX - amount < 0)
                throw "can not move cursor left";
            cursorX -= amount;
            cursorXReset = -1; //sentinel
            break;
        case RIGHT:
            if(cursorX + amount > cursorXBound())
                throw "can not move cursor right";
            cursorX += amount;
            cursorXReset = -1; //sentinel
            break;
        case BEGINNING_OF_LINE:
            cursorX = findBeginningOfLine();
            cursorXReset = -1; //sentinel
            break;
        case END_OF_LINE:
            cursorX = findEndOfLine();
            cursorXReset = -1; //sentinel
            break;
        default:
            break;
    }
}

void Buffer::moveCursor(int row){
    if(row >= 0 && row < contents.size()){
        auto it = contents.begin();
        for(int i=0; i<row; i++)
            it++;
        lastYMove = orderRelativeToCursor(it);
        cursorY = it;

        resetXCursorToCacheIfNeeded();
        moveXCursorToLineEndAndCacheIfNeeded();
        contentsChangedB = true;
    }
}

void Buffer::save(){
    if(filename.size() > 0){
        ofstream out(filename);
        for(auto line = contents.begin(); line != contents.end(); line++){
            out.write(line->c_str(), line->size());
            if(line!=--contents.end()){
                out.write("\n", 1);
            }
        }
    }
}

void Buffer::paste_after(){
  std::stringstream ss(copy_buffer.contents);
  if(copy_buffer.hasNewline) {
    string line;
    auto it = cursorY;
    while(std::getline(ss, line, '\n')){
      contents.insert(++it, line);
      cursorY++;
    }
  } else {
    cursorY->insert(++cursorX, copy_buffer.contents);
  }
  contentsChangedB = true;
}

void Buffer::paste_before(){
  std::stringstream ss(copy_buffer.contents);
  if(copy_buffer.hasNewline) {
    string line;
    auto it = cursorY;
    while(std::getline(ss,line, '\n')){
      contents.insert(it, line);
      cursorY--;
    }
  } 
  contentsChangedB = true;
}
