#ifndef INTERFACE_H
#define INTERFACE_H

class Interface {
  public:
    virtual char getChar() = 0;
    virtual int move(int y, int x) = 0;
    virtual void moveAndReadString(int y, int x, char* str) = 0;
    virtual void moveAndWriteString(int y, int x, char* str) = 0;
    virtual void getCursorPosition(int &y, int &x) = 0;

};

class NcursesInterface : public Interface {
  public:
    NcursesInterface(){};
    char getChar();
    int move(int y, int x);
    void moveAndReadString(int y, int x, char* str);
    void moveAndWriteString(int y, int x, char* str);
    void getCursorPosition(int &y, int &x);
};

#endif
