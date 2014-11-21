CCFLAGS = -g -pedantic -Wall
CC = g++
NAME = tcbvrp_solver

all: tcbvrp_solver

tcbvrp_solver: main.o Algorithm.o Graph.o Node.o
	$(CC) $(CCFLAGS) -o $(NAME) $^
	
main.o: src/main.cpp
	$(CC) $(CCFLAGS) -c $<

Algorithm.o: src/Algorithm.cpp src/Algorithm.h
	$(CC) $(CCFLAGS) -c $<

Graph.o: src/Graph.cpp src/Graph.h src/Node.h
	$(CC) $(CCFLAGS) -c $<

Node.o: src/Node.cpp src/Node.h
	$(CC) $(CCFLAGS) -c $<
	
clean:
	rm -rf *.o src/*.gch $(NAME)