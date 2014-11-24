all: logger

logger: logger.o
	 g++  -std=c++0x -o logger logger.o

logger.o: logger.cpp
	 g++ -std=c++0x -include logger.h -c logger.cpp

     
clean:
	 rm logger.o logger
