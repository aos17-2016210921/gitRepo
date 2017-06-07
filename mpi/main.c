#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"mpi.h"
#define false 0
#define true 1
#define MAX_PROC 12

/*if all node return phase OK,then return a plus to another phase*/
int for_all_node(int* signal,int length,int check){
	bool flag=true;
	for(int i=0;i!=length;i++){
		if(signal[i]!=check){
			flag=false;
		}
	}
	if(flag==true){
		return 1;
	}else{
		return 0;
	}
}
int main(int argc,char* argv[]){
	int rank,size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	char worker[100]="./worker";
	int total=atoi(argv[1]);
	int procNum=total/size;
	if(total%size>rank){
		procNum++;
	}
	printf("total:%d,%d,%d\n",total,procNum,size);
	MPI_Comm* intercomm;
	intercomm=malloc(sizeof(MPI_Comm)*1);
	for(int i=0;i!=1;i++)
		MPI_Comm_spawn(worker,MPI_ARGV_NULL,procNum,
				MPI_INFO_NULL,0,MPI_COMM_SELF,&intercomm[i],
				MPI_ERRCODES_IGNORE);
	int* alive;
	alive=malloc(sizeof(int)*procNum);
	int t=10;
	int state=0;
	int flag=true;
	while(flag){
		for(int i=0;i!=procNum;i++){
			alive[i]=-1;
		}
		MPI_Request req[MAX_PROC];
		MPI_Status sta[MAX_PROC];
		int index[MAX_PROC];
		for(int i=0;i!=procNum;i++){
				MPI_Irecv(&alive[i],1,MPI_INT,i,0,intercomm[0],&req[i]);
		}
		MPI_Testall(procNum,req,index,sta);
		usleep(50000);
		flag=false;
		state+=for_all_node(alive,procNum,state);//?
		MPI_Request reqs[MAX_PROC];
		MPI_Status stas[MAX_PROC];
		for(int i=0;i!=procNum;i++){
				MPI_Isend(&state,1,MPI_INT,i,0,intercomm[0],&reqs[i]);
		}
		int index2[MAX_PROC];
		MPI_Testall(procNum,reqs,index2,stas);
		usleep(50000);
	/*heartbeat*/
		printf("rank:%d alive:%d %d\n",rank,alive[0],alive[1]);
	}
	MPI_Finalize();
	return 0;
}
