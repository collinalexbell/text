SRC = src
I = include


all: $(SRC)/main.cpp window commands cursor buffer
	g++ $(SRC)/main.cpp buffer.o commands.o cursor.o window.o -lncurses -o text -g -I$(I)

buffer: $(SRC)/buffer.cpp $(I)/buffer.h
	g++ -g -c $(SRC)/buffer.cpp -I$(I)

commands: $(SRC)/commands.cpp $(I)/commands.h
	g++ -g -c $(SRC)/commands.cpp -I$(I)

cursor: $(SRC)/cursor.cpp $(I)/cursor.h
	g++ -g -c $(SRC)/cursor.cpp -I$(I)

window: $(SRC)/window.cpp $(I)/window.h
	g++ -g -c $(SRC)/window.cpp -I$(I)

test_cpp: $(SRC)/test.cpp
	g++ -g -c $(SRC)/test.cpp -I$(I)

test: test_cpp $(SRC)/bufferTest.cpp buffer
	g++ -g -c $(SRC)/bufferTest.cpp -I$(I)
	g++ test.o bufferTest.o buffer.o -o test
	./test

clean:
	rm *.o
