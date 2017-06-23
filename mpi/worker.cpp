#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<unistd.h>
#include<random>
#include<cstdlib>
#include<omp.h>
#include"mpi.h"
#include"mapReduce.h"
#define false 0
#define true 1
using namespace std;
void doMap(int& info,string fileName,int mapID,int reduceNum);
void doReduce(int& info);
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
	int initData[3];
	MPI_Status sta;
	MPI_Recv(&initData,3,MPI_INT,0,0,parent,&sta);
	stage=initData[0];
	int reduceNum=initData[2];
	int mapID=initData[1];
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
	string fileName;
	fileName="map"+mapID;
	while(flag){
				switch(stage){
					case 0:
					//MPI_Status stas;
						//printf("send\n");
						gettimeofday(&t_start,NULL);
						//cout<<"send"<<endl;
						MPI_Send(&info,1,MPI_INT,0,0,parent);
						//cout<<"send end"<<endl;
						MPI_Recv(&stage,1,MPI_INT,0,0,parent,&sta);
						//cout<<"recv"<<endl;
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
									doMap(info,fileName,mapID,reduceNum);
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
									doReduce(info);
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
		if(rd>1.99){
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
void doMap(int& info,string file,int mapID,int reduceNum){//generate relative file to feed reduce
	printf("mapID:%d!\n",mapID);
	KV pair;
//	usleep(300000);
	pair=Map(file,reduceNum);
	info=1;
}
void doReduce(int& info){ //pull the file and feed to reduce
	printf("reduce begin!\n");
	KV pair;
	usleep(300000);
//	pair=Reduce();
	printf("reduce end!\n");
	info=1;
}

int keyHash(string key){
	int sum=0;
	//sum+=key[i]-'0';
	if(sum<0){
		sum=-sum;
	}
	return sum;
}
