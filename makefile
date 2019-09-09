all: main.cpp buffer.h commands cursor
	g++ main.cpp commands.o cursor.o -lncurses -o text -g

commands: commands.cpp commands.h
	g++ -c commands.cpp

cursor: cursor.cpp cursor.h
	g++ -c cursor.cpp

test_cpp: test.cpp

test: test_cpp buffer.h bufferTest.cpp
	g++ -c bufferTest.cpp
	g++ test.o bufferTest.o -o test
	./test
