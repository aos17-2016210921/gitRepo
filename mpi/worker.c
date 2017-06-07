#include<stdio.h>
#include<stdlib.h>
#include"mpi.h"
int main(int argc,char* argv[]){
	printf("start successfully\n");
	MPI_Init(&argc,&argv);
	
	MPI_Finalize();
	printf("ret\n");
	return 0;
}
