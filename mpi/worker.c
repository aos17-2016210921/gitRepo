#include<stdio.h>
#include<stdlib.h>
#include"mpi.h"
int main(int argc,char* argv[]){
	printf("start successfully\n");
	MPI_Init(&argc,&argv);
	MPI_Comm parent;
	int rank;
	MPI_Comm_get_parent(&parent);
	MPI_Comm_rank(parent,&rank);
	printf("rank:%d\n",rank);
	int t=10;
	int info=0;
	int state=-1;
	while(t--){
		MPI_Status sta;
		state=-1;
		info=1;
		MPI_Send(&info,1,MPI_INT,0,0,parent);
		MPI_Recv(&state,1,MPI_INT,0,0,parent,&sta);
		printf("state:%d\n",state);
	}
	MPI_Finalize();
	printf("ret\n");
	return 0;
}
