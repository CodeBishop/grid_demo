thecommand: main.o watcher.o
	ls

main: SharedGrid.hpp
	g++ -o main main.cpp -lrt -pthread

watcher: SharedGrid.hpp
	g++ -o watcher watcher.cpp -lrt -pthread