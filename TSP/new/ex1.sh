#!/bin/bash

for (( i=0; i<=4; i=i+1))
do
	./main 0.2 3 0 >>output
	./main 0.2 3 1 >>output1
done
 

