SRC = src
BUILD = build
I = src
G++ = g++ -std=c++11
PWD := $(shell pwd)


all: $(SRC)/main.cpp $(BUILD)/window.o $(BUILD)/InputProcessor.o $(BUILD)/cursor.o $(BUILD)/buffer.o $(BUILD)/ncurses_interface.o
	$(G++) $(SRC)/main.cpp $(BUILD)/buffer.o $(BUILD)/InputProcessor.o $(BUILD)/cursor.o $(BUILD)/window.o $(BUILD)/ncurses_interface.o -lncurses -ltinfo -o text -g -I$(I)

$(BUILD)/buffer.o: $(SRC)/buffer.cpp $(I)/buffer.h
	$(G++) -g -c $(SRC)/buffer.cpp -I$(I) -o $(BUILD)/buffer.o

$(BUILD)/InputProcessor.o: $(SRC)/InputProcessor.cpp $(I)/InputProcessor.h $(I)/buffer.h $(I)/cursor.h $(I)/interface.h
	$(G++) -g -c $(SRC)/InputProcessor.cpp -I$(I) -o $(BUILD)/InputProcessor.o

$(BUILD)/cursor.o: $(SRC)/cursor.cpp $(I)/cursor.h
	$(G++) -g -c $(SRC)/cursor.cpp -I$(I) -o $(BUILD)/cursor.o

$(BUILD)/ncurses_interface.o: $(SRC)/ncurses_interface.cpp $(I)/interface.h
	$(G++) -g -c $(SRC)/ncurses_interface.cpp -I$(I) -o $(BUILD)/ncurses_interface.o

$(BUILD)/window.o: $(SRC)/window.cpp $(I)/window.h
	$(G++) -g -c $(SRC)/window.cpp -I$(I) -o $(BUILD)/window.o

$(BUILD)/test.o: $(SRC)/test.cpp
	$(G++) -g -c $(SRC)/test.cpp -I$(I) -o $(BUILD)/test.o

test: $(BUILD)/test.o $(SRC)/bufferTest.cpp $(BUILD)/buffer.o $(SRC)/InputProcessorTest.cpp $(BUILD)/InputProcessor.o $(BUILD)/cursor.o $(BUILD)/ncurses_interface.o
	$(G++) -g -c $(SRC)/bufferTest.cpp -I$(I) -o $(BUILD)/bufferTest.o
	$(G++) -g -c $(SRC)/InputProcessorTest.cpp -I$(I) -o $(BUILD)/InputProcessorTest.o
	$(G++) $(BUILD)/test.o $(BUILD)/bufferTest.o $(BUILD)/InputProcessorTest.o $(BUILD)/InputProcessor.o $(BUILD)/cursor.o $(BUILD)/buffer.o $(BUILD)/ncurses_interface.o -ltinfo -lncurses -o test 
	./test

install: text
	ln -s $(PWD)/text /home/$(USER)/.bin/text 

clean:
	rm *.o
