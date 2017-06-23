#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<omp.h>
#include<random>
#include"mpi.h"
#define false 0
#define true 1
void map(int* info,int rank);
void reduce(int* info);
double randm(){
	default_random_engine generator(random_device{}());
	uniform_real_distribution<double> fun(0.0,1.0);
	return fun(generator);
}
int main(int argc,char* argv[]){
//	printf("start successfully\n");
	MPI_Init(&argc,&argv);
	MPI_Comm parent;
	int rank;
	MPI_Comm_get_parent(&parent);
	MPI_Comm_rank(parent,&rank);
	struct timeval t_end,t_start;
//	printf("rank:%d\n",rank);
	int stage=-1;
	MPI_Status sta;
	MPI_Recv(&stage,1,MPI_INT,0,0,parent,&sta);
	int finished=false;
	if(stage<0){
		finished=true;
		stage=0;
		printf("after map fail restart!\n");
	}
	int t=10;
	int info=1;
	int flag=true;
	int signal=1;
	printf("begin worker\n");
	int timeuse=0;
	while(flag){
		//printf("state:%d\n",state);
				switch(stage){
					case 0:
					//MPI_Status stas;
						//printf("send\n");
						gettimeofday(&t_start,NULL);
						MPI_Send(&info,1,MPI_INT,0,0,parent);
						MPI_Recv(&stage,1,MPI_INT,0,0,parent,&sta);
						gettimeofday(&t_end,NULL);
						timeuse=(t_end.tv_sec-t_start.tv_sec)*1000000+t_end.tv_usec-t_start.tv_usec;
						//printf("cost %d us\n",timeuse);
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
									if(stage==2&&info==2){
										signal=0;
										printf("end stage and rank:%d\n",rank);
									}
									if(stage==0&&info==2){
										signal=0;
									}
								}
							}
							#pragma omp section
							{
								if(!finished){
									map(&info,rank);
								}
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
										if(stage==0&&info==3){//
											signal=0;
											printf("reduce stage 0 get\n");
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
									if(!finished){
									reduce(&info);
									}
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
					}//switch
		//randomly dead or alive
		double rd=randm();
		if(randm()>0.7){
			printf("rd:%lf\n",rd);
			MPI_Finalize();
			printf("%d is dead\n",rank);
			return 0;
		}
		//printf("still alive%d\n",rank);
	}//end while
	printf("ret child\n");
	MPI_Finalize();
	printf("ret\n");
	return 0;
}
void map(int* info,int rank){
	printf("rank:%d,map begin!\n",rank);
	usleep(300000);
	printf("rank:%d,map end!\n",rank);
	*info=1;
}
void reduce(int* info){
	printf("reduce begin!\n");
	usleep(300000);
	printf("reduce end!\n");
	*info=1;
}
