#!/bin/bash

g++ -c -std=c++11 main.cpp
g++ -o main main.o

./main $1 $2 $3
