SRC = src
I = include


all: $(SRC)/main.cpp $(I)/window.h commands cursor buffer
	g++ $(SRC)/main.cpp buffer.o commands.o cursor.o -lncurses -o text -g -I$(I)

buffer: $(SRC)/buffer.cpp $(I)/buffer.h
	g++ -c $(SRC)/buffer.cpp -I$(I)

commands: $(SRC)/commands.cpp $(I)/commands.h
	g++  -c $(SRC)/commands.cpp -I$(I)

cursor: $(SRC)/cursor.cpp $(I)/cursor.h
	g++ -c $(SRC)/cursor.cpp -I$(I)

test_cpp: $(SRC)/test.cpp
	g++ -c $(SRC)/test.cpp -I$(I)

test: test_cpp $(I)/buffer.h $(SRC)/bufferTest.cpp
	g++ -c $(SRC)/bufferTest.cpp -I$(I)
	g++ test.o bufferTest.o -o test
	./test

clean:
	rm *.o
