# Ahnaf Raihan
# cache-sim

all:	cache-sim

cache-sim:	main.o Simulator.o
	g++  main.o Simulator.o -o cache-sim

Simulator.o:	Simulator.cpp
	g++ -std=c++11 -c  Simulator.cpp 

main.o:	main.cpp
	g++ -std=c++11 -c main.cpp

clean:
	rm -f *.o cache-sim

