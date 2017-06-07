#include<stdio.h>
#include<stdlib.h>
#include"mpi.h"
#define false 0
#define true 1
int main(int argc,char* argv[]){
	printf("start successfully\n");
	MPI_Init(&argc,&argv);
	MPI_Comm parent;
	int rank;
	MPI_Comm_get_parent(&parent);
	MPI_Comm_rank(parent,&rank);
	printf("rank:%d\n",rank);
	int t=10;
	int info=1;
	int stage=-1;
	bool flag=true;
	while(flag){
		MPI_Status sta;
		state=-1;
		MPI_Send(&info,1,MPI_INT,0,0,parent);
		MPI_Recv(&stage,1,MPI_INT,0,0,parent,&sta);
		//printf("state:%d\n",state);
		switch(stage){
			case 0:
				flag=false;
				break;
			case 1:
				map(&info);
				break;
			case 2:
				reduce(&info);
				break;
			default:
				info=1;
				break;
		}
	}
	MPI_Finalize();
	printf("ret\n");
	return 0;
}
void map(int &info){
	info=2;
}
void reduce(int &info){
	info=3
}
