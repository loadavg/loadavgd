all: INIReader

INIReader: INIReader.o INIReaderTest.o
	 g++  -std=c++0x -o INIReader ../ini.o INIReader.o INIReaderTest.o

INIReader.o: INIReader.cpp
	 g++ -std=c++0x -c INIReader.cpp

INIReaderTest.o: INIReaderTest.cpp
	 g++ -std=c++0x -c INIReaderTest.cpp

clean:
	 rm INIReader.o INIReaderTest.o INIReader



#Manually

#g++ -c INIReader.cpp 
#g++ -c INIReaderTest.cpp 

#g++ -o INIReader INIReader.o INIReaderTest.o ../ini.o


