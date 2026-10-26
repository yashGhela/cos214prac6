CXX := g++
CXXFLAGS := -g -std=c++17 -pthread

valgrind:
	make clean
	make demo
	valgrind --track-origins=yes --leak-check=full 

gdb:
	make clean
	make demo
	gdb ./demo

clean:
	rm -f *.o

run: demo
	./demo