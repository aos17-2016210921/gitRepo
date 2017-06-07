#include<stdio.h>
#include<stdlib.h>
#include"mpi.h"
int main(int argc,char* argv[]){
	int rank,size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	char worker[100]="./worker";
	int total=atoi(argv[1]);
	printf("total:%d\n",total);
	MPI_Comm* intercomm;
	intercomm=malloc(sizeof(MPI_Comm)*2);
	for(int i=0;i!=2;i++)
		MPI_Comm_spawn(worker,MPI_ARGV_NULL,total,
				MPI_INFO_NULL,0,MPI_COMM_SELF,&intercomm[i],
				MPI_ERRCODES_IGNORE);
	/*MPI_Request req[2];
	MPI_Status sta[2];
	int info;
	for(int i=0;i!=2;i++){
		MPI_Irecv(&info,1,MPI_INT,)
		MPI_Irecv()
	}
	MPI_Waitall();*/
	/*heartbeat*/

	MPI_Finalize();
	return 0;
}
