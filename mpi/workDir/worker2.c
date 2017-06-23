#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#include"mpi.h"
#define false 0
#define true 1
void map(int* info);
void reduce(int* info);
double randm(){
	srand(time(0));
	double ret=(rand()%374239)*1.0/374239;
	return ret;
}
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
	int flag=true;
	int signal=1;
	while(flag){
		MPI_Status sta;
		stage=-1;
		MPI_Send(&info,1,MPI_INT,0,0,parent);
		MPI_Recv(&stage,1,MPI_INT,0,0,parent,&sta);
		//printf("state:%d\n",state);
		switch(stage){
			case 0:
				break;
			case 1:
				signal=1;
				omp_set_num_threads(2);
				#pragma omp parallel sections
				{
					#pragma omp section
					{
						info=1;
						while(signal){
							MPI_Status sta;
							MPI_Send(&info,1,MPI_INT,0,0,parent);
							MPI_Recv(&stage,1,MPI_INT,0,0,parent,&sta);
							if(stage==1&&info==2){
								signal=0;
							}
						}
					}
					#pragma omp section
					{
						map(&info);
						info=2;
					}
				}
				break;
			case 2:
				signal=1;
				omp_set_num_threads(2);
				#pragma omp parallel sections
				{
					#pragma omp section
					{
						info=1;
						while(signal){
							MPI_Status sta;
							MPI_Send(&info,1,MPI_INT,0,0,parent);
							MPI_Recv(&stage,1,MPI_INT,0,0,parent,&sta);
							if(stage==1&&info==3){
								signal=0;
							} 
							if(stage==3){
								flag=false;
								signal=0;
							//	printf("trigger");
							}
						}
					}
					#pragma omp section
					{
						reduce(&info);
						info=3;
					}
				}
				break;
			case 3:
				flag=false;
			//	printf("triggered!\n");
				break;
			default:
				info=1;
				break;
		}
		//randomly dead or alive
		if(randm()>0.7){
			printf("%d is dead\n",rank);
			exit(1);
		}
		printf("still alive%d\n",rank);
	}//end while
	printf("ret child\n");
	MPI_Finalize();
	printf("ret\n");
	return 0;
}
void map(int* info){
	printf("map begin!\n");
	usleep(300000);
	printf("map end!\n");
	*info=1;
}
void reduce(int* info){
	printf("reduce begin!\n");
	usleep(300000);
	printf("reduce end!\n");
	*info=1;
}
