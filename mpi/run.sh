#!/bin/bash
g++ -c mapReduce.h mapReduce.cpp
if [ $1 -eq 0 ]; then
	echo main
	mpicxx -o  main main.cpp mapReduce.o -std=c++11
else
	echo worker
#	mpicxx -c -std=c++11 worker.cpp
	mpicxx -o  worker worker.cpp mapReduce.o  -fopenmp -std=c++11
fi

rm *.o *.h.gch
