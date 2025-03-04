CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

all: main

main: main.cpp Expression.cpp
	$(CXX) $(CXXFLAGS) -o main main.cpp Expression.cpp

clean:
	rm -f main