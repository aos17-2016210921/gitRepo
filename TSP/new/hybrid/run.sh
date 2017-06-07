#!/bin/bash

g++ -c -std=c++11 $1.cpp
g++ -o $1 $1.o
