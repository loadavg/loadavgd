all: parser

parser: parser.o utility.o main.o
	 g++  -std=c++0x `pkg-config --libs gtk+-3.0` -o parser parser.o utility.o main.o

parser.o: parser.cpp
	 g++ -std=c++0x `pkg-config --cflags gtk+-3.0` -include parser.h -c parser.cpp

utility.o: utility.cpp
	 g++ -std=c++0x `pkg-config --cflags gtk+-3.0` -include parser.h -c utility.cpp

main.o: main.cpp
	 g++ -std=c++0x `pkg-config --cflags gtk+-3.0` -include parser.h -c main.cpp

clean:
	 rm parser.o utility.o main.o parser 
