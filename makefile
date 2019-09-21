SRC = src
I = include
G++ = g++ -std=c++11


all: $(SRC)/main.cpp window commands cursor buffer
	$(G++) $(SRC)/main.cpp buffer.o commands.o cursor.o window.o -lncurses -o text -g -I$(I)

buffer: $(SRC)/buffer.cpp $(I)/buffer.h
	$(G++) -g -c $(SRC)/buffer.cpp -I$(I)

commands: $(SRC)/commands.cpp $(I)/commands.h
	$(G++) -g -c $(SRC)/commands.cpp -I$(I)

cursor: $(SRC)/cursor.cpp $(I)/cursor.h
	$(G++) -g -c $(SRC)/cursor.cpp -I$(I)

window: $(SRC)/window.cpp $(I)/window.h
	$(G++) -g -c $(SRC)/window.cpp -I$(I)

test_cpp: $(SRC)/test.cpp
	$(G++) -g -c $(SRC)/test.cpp -I$(I)

test: test_cpp $(SRC)/bufferTest.cpp buffer
	$(G++) -g -c $(SRC)/bufferTest.cpp -I$(I)
	$(G++) test.o bufferTest.o buffer.o -o test
	./test

clean:
	rm *.o
