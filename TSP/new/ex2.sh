#!/bin/bash

for (( i=1; i<=5; i=i+1))
do
	./main 0.1 3 0 >>out0
	./main 0.1 3 1 >>out1
done
