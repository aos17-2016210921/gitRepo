#include<iostream>
#include"mpi.h"

using namespace std;

class parallel{
	int argc;
	char** argv;
public:
	parallel(int Argc,char* Argv[]){
		argc=Argc;
		argv=Argv;
	}
	void run(){
		int rank,size;
		MPI_Init(&argc,&argv);
		MPI_Comm_rank(MPI_COMM_WORLD,&rank);
		cout<<"myrank"<<rank<<endl;
		MPI_Finalize();
	}
	parallel();
};

int main(int argc,char* argv[]){
	parallel para(argc,argv);
	para.run();
	return 0;
}
