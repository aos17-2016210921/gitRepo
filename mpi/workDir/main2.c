#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"mpi.h"
#define false 0
#define true 1
#define MAX_PROC 12
int pk=0;
void pork(){
	pk++;
	printf("%d\n",pk);
}
/*if all node return phase OK,then return a plus to another phase*/
void initVec(int* vec,int length,int v){
	for(int i=0;i!=length;i++){
		vec[i]=v;
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
	for(int i=0;i!=procNum;i++)
		MPI_Comm_spawn(worker,MPI_ARGV_NULL,1,
				MPI_INFO_NULL,0,MPI_COMM_SELF,&intercomm[i],
				MPI_ERRCODES_IGNORE);
	
	int* alive;
	int* mapSt;
	int* reduceSt;
	mapSt=malloc(sizeof(int)*procNum);
	reduceSt=malloc(sizeof(int)*procNum);
	alive=malloc(sizeof(int)*procNum);
	initVec(mapSt,procNum,0);
	initVec(reduceSt,procNum,0);
	int mapNum=procNum;
	int reduceNum=procNum;

	int t=10;
	int state=0;
	int flag=true;
	int beats=0;
	while(flag){
		for(int i=0;i!=procNum;i++){
			alive[i]=-1;
		}
		MPI_Request req[MAX_PROC];
		MPI_Status sta[MAX_PROC];
		int index[MAX_PROC];
		for(int i=0;i!=procNum;i++){
				MPI_Irecv(&alive[i],1,MPI_INT,0,0,intercomm[i],&req[i]);
		}
		MPI_Testall(procNum,req,index,sta);
		usleep(50000);
		switch(state){
			case 0:
				beats++;
				if(beats==5){//heartbeat
					state=1;
				}
				break;
			case 1:
				for(int i=0;i!=procNum;i++){// we haven't consider the error of recv the alive info
					if(alive[i]==2&&mapSt[i]==0){
						mapSt[i]=1;
						mapNum--;	
					}
				}
				if(mapNum==0){
					state=2;
				}
				break;
			case 2:
				for(int i=0;i!=procNum;i++){
					if(alive[i]==3&&reduceSt[i]==0){
						reduceSt[i]=1;
						reduceNum--;
					}
				}
				if(reduceNum==0){
					state=3;
					flag=false;
				}
				break;
			case 3:
				state=3;// may need to deal with fault child process, present no such support
				flag=false;
				break;
			default :
			break;
		}
		//update and check wether to change state		
		MPI_Request reqs[MAX_PROC];
		MPI_Status stas[MAX_PROC];
		for(int i=0;i!=procNum;i++){
				MPI_Isend(&state,1,MPI_INT,0,0,intercomm[i],&reqs[i]);
		}
		int index2[MAX_PROC];
		MPI_Testall(procNum,reqs,index2,stas);
		usleep(50000);
		if(state==3){
			printf("end!\n");
		}
	/*heartbeat*/
		//printf("rank:%d alive:%d %d\n",rank,alive[0],alive[1]);
	}
//	printf("main ret\n");
	MPI_Finalize();
	return 0;
}
