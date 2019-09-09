SRC = src
I = include

all: $(SRC)/main.cpp $(I)/buffer.h commands cursor
	g++ $(SRC)/main.cpp commands.o cursor.o -lncurses -o text -g -I$(I)

commands: $(SRC)/commands.cpp $(I)/commands.h
	g++  -c $(SRC)/commands.cpp -I$(I)

cursor: $(SRC)/cursor.cpp $(I)/cursor.h
	g++ -c $(SRC)/cursor.cpp -I$(I)

test_cpp: $(SRC)/test.cpp

test: $(I)/test_cpp buffer.h $(SRC)/bufferTest.cpp
	g++ -c $(SRC)/bufferTest.cpp -I$(I)
	g++ test.o bufferTest.o -o test
	./test

clean:
	rm *.o
	rm test
