#ifndef INTERFACE_H
#define INTERFACE_H

class Interface {
  public:
    virtual char getChar() = 0;
};

class NcursesInterface : public Interface {
  public:
    NcursesInterface(){};
    char getChar();
};

#endif
