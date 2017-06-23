#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<omp.h>
#include"mpi.h"
#define false 0
#define true 1
#define MAX_PROC 12
#define MAX_MEM 20
typedef struct mallocSet{
	int* intMalloc[MAX_MEM];
	double* doubleMalloc[MAX_MEM];
	MPI_Request* mpireqMalloc[MAX_MEM];
	MPI_Status* mpistaMalloc[MAX_MEM];
	int intLen,doubleLen,mpireqLen,mpistaLen;
}mallocSet;
int initMalloc(mallocSet* ms){
	ms->intLen=0;
	ms->doubleLen=0;
	ms->mpireqLen=0;
	ms->mpistaLen=0;
}
int registInt(mallocSet ms,int *p){
	if(ms.intLen==MAX_MEM-1){
		return 1;
	}else{
		ms.intMalloc[ms.intLen]=p;
		ms.intLen++;
		return 0;
	}
}
int registDouble(mallocSet ms,double *p){
	if(ms.doubleLen==MAX_MEM-1){
		return 1;
	}else{
		ms.doubleMalloc[ms.doubleLen]=p;
		ms.doubleLen++;
		return 0;
	}
}
int registMpireq(mallocSet ms,MPI_Request* p){
	if(ms.mpireqLen==MAX_MEM-1){
		return 1;
	}else{
		ms.mpireqMalloc[ms.mpireqLen]=p;
		ms.mpireqLen++;
		return 0;
	}
}
int registMpista(mallocSet ms,MPI_Status* p){
	if(ms.mpistaLen==MAX_MEM-1){
		return 1;
	}else{
		ms.mpistaMalloc[ms.mpistaLen]=p;
		ms.mpistaLen++;
		return 0;
	}
}
int freeMalloc(mallocSet ms){
	for(int i=0;i!=ms.intLen;i++){
		free(ms.intMalloc[i]);
	}
	ms.intLen=0;
	for(int i=0;i!=ms.doubleLen;i++){
		free(ms.doubleMalloc[i]);
	}
	ms.doubleLen=0;
	for(int  i=0;i!=ms.mpireqLen;i++){
		free(ms.mpireqMalloc[i]);
	}
	ms.mpireqLen=0;
	for(int i=0;i!=ms.mpistaLen;i++){
		free(ms.mpistaMalloc[i]);
	}
	ms.mpistaLen=0;
	return 0;
}
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
	mallocSet ms;
	initMalloc(&ms);
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
	int state=0;
	intercomm=malloc(sizeof(MPI_Comm)*1);
	MPI_Request* request;
	MPI_Status* status;
	int* Index;
	request=malloc(sizeof(MPI_Request)*procNum);
	registMpireq(ms,request);
	status=malloc(sizeof(MPI_Status)*procNum);
	registMpista(ms,status);
	Index=malloc(sizeof(int)*procNum);
	registInt(ms,Index);
	#pragma omp parallel for
	for(int i=0;i!=procNum;i++){
		MPI_Comm_spawn(worker,MPI_ARGV_NULL,1,
				MPI_INFO_NULL,0,MPI_COMM_SELF,&intercomm[i],
				MPI_ERRCODES_IGNORE);
		MPI_Isend(&state,1,MPI_INT,0,0,intercomm[i],&request[i]);
	}
	MPI_Testall(procNum,request,Index,status);
	freeMalloc(ms);
	MPI_Barrier(MPI_COMM_WORLD);
	int* alive;
	int* mapSt;
	int* reduceSt;
	int* restart;
	int restartNum=0;

	mapSt=malloc(sizeof(int)*procNum);//allocate pointers and regist to manage structure then init
	registInt(ms,mapSt);
	reduceSt=malloc(sizeof(int)*procNum);
	registInt(ms,reduceSt);
	alive=malloc(sizeof(int)*procNum);
	registInt(ms,alive);
	restart=malloc(sizeof(int)*procNum);
	registInt(ms,restart);
	initVec(mapSt,procNum,0);
	initVec(reduceSt,procNum,0);
	initVec(restart,procNum,0);
	int mapNum=procNum;
	int reduceNum=procNum;

	int t=10;
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
		MPI_Waitall(procNum,req,sta);
		usleep(30000);
		for(int i=0;i!=procNum;i++){
			if(alive[i]==-1){
				printf("spawn\n");
				MPI_Comm_spawn(worker,MPI_ARGV_NULL,1,
						MPI_INFO_NULL,0,MPI_COMM_SELF,&intercomm[i],
						MPI_ERRCODES_IGNORE);
				restart[restartNum]=i;
				restartNum++;			
			}
		}
		MPI_Request reqRes[MAX_PROC];
		MPI_Status staRes[MAX_PROC];
		int indexRes[MAX_PROC];
		/*
			for restart we need to figure out whether is is in-map fail or after-map fail(as well as to reduce),
			we use a rather simple implementation, i.e.:
				state -1 represent after-map fail
				state -2 represent after-reduce fail
		*/
		for(int i=0;i!=restartNum;i++){
			int pos=restart[i];
			int sendState;
			if(state==1){
				if(mapSt[i]==0){
					sendState=1;
				}else{
					sendState=-1;
				}
			}
			if(state==2){
				if(reduceSt[i]==0){
					sendState=2;
				}else{
					sendState=-2;
				}
			}
			MPI_Isend(&sendState,1,MPI_INT,0,0,intercomm[pos],&reqRes[i]); 
		}							          
		MPI_Testall(restartNum,reqRes,indexRes,staRes);		          
		restartNum=0;
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
				printf("halo\n");
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
				int sendState;
				sendState=0;
				if(state==1&&mapSt[i]==0){
					sendState=1;
				}
				if(state==2&&reduceSt[i]==0){
					sendState=2;
				//	printf("echo\n");
				}
				if(state==3){
					//printf("sendTo: %d\n",i);
					sendState=3;
				}
				MPI_Isend(&sendState,1,MPI_INT,0,0,intercomm[i],&reqs[i]);
		}
		int index2[MAX_PROC];
		MPI_Testall(procNum,reqs,index2,stas);
		usleep(50000);
		//printf("haha\n");
		if(state==3){
			printf("end!\n");
		}
	/*heartbeat*/
		//printf("rank:%d alive:%d %d\n",rank,alive[0],alive[1]);
	}
	freeMalloc(ms);
//	printf("main ret\n");
	MPI_Finalize();
	return 0;
}
