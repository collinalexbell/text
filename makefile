all: main.cpp buffer.h
	g++ main.cpp -lncurses -o text -g

test_cpp: test.cpp

test: test_cpp buffer.h bufferTest.cpp
	g++ -c bufferTest.cpp
	g++ test.o bufferTest.o -o test
	./test
