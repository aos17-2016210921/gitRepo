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
	intercomm=malloc(sizeof(MPI_Comm)*1);
	for(int i=0;i!=1;i++)
		MPI_Comm_spawn(worker,MPI_ARGV_NULL,total,
				MPI_INFO_NULL,0,MPI_COMM_SELF,&intercomm[i],
				MPI_ERRCODES_IGNORE);
	MPI_Request req[2];
	MPI_Status sta[2];
	int alive[2];
	int t=10;
	int state=0;
	while(t--){
		for(int i=0;i!=2;i++){
			alive[i]=-1;
		}
		MPI_Request req[2];
		MPI_Status sta[2];
		int index[2];
		for(int i=0;i!=2;i++){
				MPI_Irecv(&alive[i],1,MPI_INT,i,0,intercomm[0],&req[i]);
		}
		MPI_Testall(2,req,index,sta);
		usleep(300000);
		state=1;
		for(int i=0;i!=2;i++){
		//	if(alive[i]==1){
			MPI_Send(&state,1,MPI_INT,i,0,intercomm[0]);
		//	}else{
		//		printf("bad\n");	
		//	}
		}
		usleep(300000);
	/*heartbeat*/
		printf("%d %d\n",alive[0],alive[1]);
	}
	MPI_Finalize();
	return 0;
}
