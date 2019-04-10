thecommand: main watcher
	ls

main: SharedGrid.hpp
	g++ -o bin/main main.cpp -lrt -pthread

watcher: SharedGrid.hpp
	g++ -o bin/watcher watcher.cpp -lrt -pthread
