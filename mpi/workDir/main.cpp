#include<iostream>
#include<fstream>
#include<string>
#include"mpi.h"
using namespace std;

int main(int argc,char* argv[]){
	int rank,size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);



	MPI_Finalize();
	return 0;
}
