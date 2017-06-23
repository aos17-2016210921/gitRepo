#!/bin/bash

if [ $1 -eq 0 ]; then
	echo main
	mpicxx -c -std=c++11 main.cpp
	mpicxx -o main main.o
else
	echo worker
#	mpicxx -c -std=c++11 worker.cpp
	mpicxx -o  worker worker.cpp mapReduce.o  -fopenmp -std=c++11
fi
