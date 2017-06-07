#!/bin/bash

for (( i=0; i<=2; i=i+1))
do
	./main 0.02 3 0 >>$1
	./main 0.02 3 1 >>$2
done
