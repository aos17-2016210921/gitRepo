#!/bin/bash

for (( i=0; i<=5; i=i+1))
do
	./main 0.05 3 0 >>$1
	./main 0.05 3 1 >>$2
done
