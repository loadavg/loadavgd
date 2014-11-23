all: parser

parser: parser.o
	 g++  -std=c++0x -o parser parser.o

parser.o: parser.cpp
	 g++ -std=c++0x -c parser.cpp

     
clean:
	 rm parser.o parser
