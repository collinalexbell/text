SRC = src
BUILD = build
I = src
G++ = g++ -std=c++11
PWD := $(shell pwd)


all: $(SRC)/main.cpp window.o InputProcessor.o cursor.o buffer.o ncurses_interface.o
	$(G++) $(SRC)/main.cpp $(BUILD)/buffer.o $(BUILD)/InputProcessor.o $(BUILD)/cursor.o $(BUILD)/window.o $(BUILD)/ncurses_interface.o -lncurses -ltinfo -o text -g -I$(I)

buffer.o: $(SRC)/buffer.cpp $(I)/buffer.h
	$(G++) -g -c $(SRC)/buffer.cpp -I$(I) -o $(BUILD)/buffer.o

InputProcessor.o: $(SRC)/InputProcessor.cpp $(I)/InputProcessor.h $(I)/buffer.h $(I)/cursor.h $(I)/interface.h
	$(G++) -g -c $(SRC)/InputProcessor.cpp -I$(I) -o $(BUILD)/InputProcessor.o

cursor.o: $(SRC)/cursor.cpp $(I)/cursor.h
	$(G++) -g -c $(SRC)/cursor.cpp -I$(I) -o $(BUILD)/cursor.o

ncurses_interface.o: $(SRC)/ncurses_interface.cpp $(I)/interface.h
	$(G++) -g -c $(SRC)/ncurses_interface.cpp -I$(I) -o $(BUILD)/ncurses_interface.o

window.o: $(SRC)/window.cpp $(I)/window.h
	$(G++) -g -c $(SRC)/window.cpp -I$(I) -o $(BUILD)/window.o

test.o: $(SRC)/test.cpp
	$(G++) -g -c $(SRC)/test.cpp -I$(I) -o $(BUILD)/test.o

test: test.o $(SRC)/bufferTest.cpp buffer.o $(SRC)/InputProcessorTest.cpp InputProcessor.o cursor.o ncurses_interface.o
	$(G++) -g -c $(SRC)/bufferTest.cpp -I$(I) -o $(BUILD)/bufferTest.o
	$(G++) -g -c $(SRC)/InputProcessorTest.cpp -I$(I) -o $(BUILD)/InputProcessorTest.o
	$(G++) $(BUILD)/test.o $(BUILD)/bufferTest.o $(BUILD)/InputProcessorTest.o $(BUILD)/InputProcessor.o $(BUILD)/cursor.o $(BUILD)/buffer.o $(BUILD)/ncurses_interface.o -ltinfo -lncurses -o test 
	./test

install: text
	ln -s $(PWD)/text /home/$(USER)/.bin/text 

clean:
	rm *.o
