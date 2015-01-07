all: parser

parser: parser.o utility.o main.o
	 g++  -std=c++0x -o parser parser.o utility.o main.o

parser.o: parser.cpp
	 g++ -std=c++0x -include parser.h -c parser.cpp

utility.o: utility.cpp
	 g++ -std=c++0x -include parser.h -c utility.cpp

main.o: main.cpp
	 g++ -std=c++0x -include parser.h -c main.cpp

clean:
	 rm parser.o utility.o main.o parser 
