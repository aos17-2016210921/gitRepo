#!/bin/bash

mpicc -c -std=c99 main.c
mpicc -o main main.o
