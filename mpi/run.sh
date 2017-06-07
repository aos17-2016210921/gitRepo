#!/bin/bash

if [ $1 -eq 0 ]; then
	echo main
	mpicc -c -std=c99 main.c
	mpicc -o main main.o
else
	echo worker
	mpicc -c -std=c99 worker.c
	mpicc -o worker worker.o
fi
