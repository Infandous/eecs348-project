# Makefile

CXX = g++
CXXFLAGS = -std=c++11 -g

all: calculator

calculator: main.o
	$(CXX) $(CXXFLAGS) -o calculator main.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

clean:
	rm -f calculator main.o