.PHONY:run clean
run:a.out
ClientConnectsHandler.o:ClientConnectsHandler.h ClientConnectsHandler.cpp
	g++ -c ClientConnectsHandler.cpp -o ClientConnectsHandler.o -lpthread
ClientRequestAnalyzer.o:ClientRequestAnalyzer.h ClientRequestAnalyzer.cpp
	g++ -c ClientRequestAnalyzer.cpp -o ClientRequestAnalyzer.o
ClientSocketHandler.o:ClientSocketHandler.h ClientConnectsHandler.cpp
	g++ -c ClientSocketHandler.cpp -o ClientSocketHandler.o
FileHandler.o:FileHandler.h FileHandler.cpp
	g++ -c FileHandler.cpp -o FileHandler.o
FileScanner.o:FileScanner.h FileScanner.cpp
	g++ -c FileScanner.cpp -o FileScanner.o
Server.o:Server.h Server.cpp
	g++ -c Server.cpp -o Server.o
Tools.o:Tools.h Tools.cpp
	g++ -c Tools.cpp -o Tools.o
main.o:main.cpp ClientConnectsHandler.h ClientRequestAnalyzer.h ClientSocketHandler.h FileHandler.h FileScanner.h \
Server.h Tools.h
	g++ -c main.cpp -o main.o
Defines.o:Defines.cpp
	g++ -c Defines.cpp -o Defines.o
a.out:main.o ClientConnectsHandler.o ClientRequestAnalyzer.o ClientSocketHandler.o FileHandler.o FileScanner.o Server.o Tools.o Defines.o
	g++ -o a.out main.o ClientConnectsHandler.o ClientRequestAnalyzer.o \
	ClientSocketHandler.o FileHandler.o FileScanner.o Server.o Tools.o Defines.o -lpthread
clean:
	rm -f *.o a.out