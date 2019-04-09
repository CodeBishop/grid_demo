#!/usr/bin/env bash
g++ -o main main.cpp -lrt -pthread
g++ -o watcher watcher.cpp -lrt -pthread
