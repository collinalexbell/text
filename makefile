SRC = src
I = src
G++ = g++ -std=c++11
PWD := $(shell pwd)


all: $(SRC)/main.cpp window.o commands.o cursor.o buffer.o ncurses_interface.o
	$(G++) $(SRC)/main.cpp buffer.o commands.o cursor.o window.o ncurses_interface.o -lncurses -ltinfo -o text -g -I$(I)

buffer.o: $(SRC)/buffer.cpp $(I)/buffer.h
	$(G++) -g -c $(SRC)/buffer.cpp -I$(I)

commands.o: $(SRC)/commands.cpp $(I)/commands.h $(I)/buffer.h $(I)/cursor.h $(I)/interface.h
	$(G++) -g -c $(SRC)/commands.cpp -I$(I)

cursor.o: $(SRC)/cursor.cpp $(I)/cursor.h
	$(G++) -g -c $(SRC)/cursor.cpp -I$(I)

ncurses_interface.o: $(SRC)/ncurses_interface.cpp $(I)/interface.h
	$(G++) -g -c $(SRC)/ncurses_interface.cpp -I$(I)

window.o: $(SRC)/window.cpp $(I)/window.h
	$(G++) -g -c $(SRC)/window.cpp -I$(I)

test.o: $(SRC)/test.cpp
	$(G++) -g -c $(SRC)/test.cpp -I$(I)

test: test.o $(SRC)/bufferTest.cpp buffer.o $(SRC)/commandsTest.cpp commands.o cursor.o ncurses_interface.o
	$(G++) -g -c $(SRC)/bufferTest.cpp -I$(I)
	$(G++) -g -c $(SRC)/commandsTest.cpp -I$(I)
	$(G++) test.o bufferTest.o commandsTest.o commands.o cursor.o buffer.o ncurses_interface.o -ltinfo -lncurses -o test 
	./test

install: text
	ln -s $(PWD)/text /home/$(USER)/.bin/text 

clean:
	rm *.o
