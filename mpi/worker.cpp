#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<sys/time.h>
#include<unistd.h>
#include<random>
#include<string>
#include<cstdlib>
#include<sstream>
#include<omp.h>
#include<map>
#include"mpi.h"
#include"mapReduce.h"
#define false 0
#define true 1
using namespace std;
void doMap(int& info,string fileName,int mapID,int reduceNum);
void doReduce(int& info,int mapNum,int reduceId);
void priGen(int *p,int len);
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
	int initData[5];
	MPI_Status sta;
	MPI_Recv(&initData,5,MPI_INT,0,0,parent,&sta);
	stage=initData[0];
	int mapID=initData[1];
	int mapNum=initData[2];
	int reduceId=initData[3];
	int reduceNum=initData[4];
	int finished=false;
	if(stage<0){
		finished=true;
		stage=0;
		printf("after map fail restart!\n");
	}
	int t=10;
	int info=1;
	cout<<"reduceId:"<<reduceId<<endl;
	int flag=true;
	int signal=1;
	printf("begin worker\n");
	int timeuse=0;
	string fileName;
	string mapS;
	stringstream ss;
	ss<<mapID;
	ss>>mapS;
	fileName="map"+mapS;
	cout<<fileName<<endl;
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
										doReduce(info,mapNum,reduceId);
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
void doMap(int& info,string fileName,int mapID,int reduceNum){//generate relative file to feed reduce
	int primer[50];
	priGen(primer,50);
	printf("mapID:%d!\n",mapID);
	KV pair;
//	usleep(300000);
	ifstream file;
	char fileN[100];
	for(int i=0;i!=fileName.length();i++){
		fileN[i]=fileName[i];
	}
	fileN[fileName.length()]='\0';
	file.open(fileN,ios::in);
	for(int i=0;i!=reduceNum;i++){
		ofstream output;
		string fileOut(fileName);
		string s;
		stringstream ss;
		ss<<i;
		ss>>s;
		fileOut+="out"+s;
		char fileT[100];
		for(int i=0;i!=fileOut.length();i++){
			fileT[i]=fileOut[i];
		}
		fileT[fileOut.length()]='\0';
		output.open(fileT,ios::out|ios::trunc);
		if(output.is_open()){
			output.close();
		}
	}
	if(file.is_open()){
		file.close();
	}
	string KeyFile=Map(fileName);
	char kFCh[100];
	stoch(KeyFile,kFCh);
	ifstream fileK;
	fileK.open(kFCh,ios::in);
	ofstream *out;
	out=new ofstream [reduceNum];
	for(int i=0;i!=reduceNum;i++){
		string fileOut(fileName);
		string s;
		stringstream ss;
		ss<<i;
		ss>>s;
		fileOut+="out"+s;
		char fileT[100];
		stoch(fileOut,fileT);
		out[i].open(fileT,ios::out);
	}
	while(!fileK.eof()){
		string s;
		getline(fileK,s);
		if(fileK.eof()){
			break;
		}
		int kpos=s.find_first_of("key",0);
		int vpos=s.find_first_of(",",0)+1;
		int len=vpos-1-(kpos+4);
		string key;
		key+=s.substr(kpos+4,len);
		int index=KeyHash(key,primer)%reduceNum;
		out[index]<<s<<endl;
	}
	for(int i=0;i!=reduceNum;i++){
		out[i].close();
	}
	fileK.close();
	remove(kFCh);
	info=1;
}
void doReduce(int& info,int mapNum,int reduceId){ //pull the file and feed to reduce
	ofstream outfile;
	string output("reduce");
	output+=itostr(reduceId);
	char outTmp[100];
	stoch(output,outTmp);
	outfile.open(outTmp,ios::out|ios::trunc);
	if(outfile.is_open()){
		outfile.close();
	}
	for(int i=0;i!=mapNum;i++){
		string fileIn;
		fileIn="map";
		fileIn+=itostr(i)+"out"+itostr(reduceId);
		char fileTmp[100];
		stoch(fileIn,fileTmp);
		ifstream file;
		file.open(fileTmp,ios::in);
		map<string,string> m;
		if(file.is_open()){
			while(!file.eof()){
				string s;
				getline(file,s);
				if(file.eof()){
					break;
				}
				KV kv;
				parse(s,kv);
				m[kv.key]+=kv.value+",";
			}
			file.close();
		}
		ofstream outfile;
		outfile.open(outTmp,ios::out);
		if(outfile.is_open()){
			for(map<string,string>::iterator it=m.begin();it!=m.end();it++){
				string s("{key:");
				s+=it->first+",value:{"+it->second;
				s.erase(s.length()-1);
				s+="}}";
				outfile<<s<<endl;
			}
			outfile.close();
		}
		remove(fileTmp);

	}
	Reduce(output,reduceId);
	cout<<"endReduce"<<endl;
}

void priGen(int *p,int len){
	int j=0;
	int i;
	for(i=1;i!=1000;i++){
		int flag=0;
		int leave=0;
		for(int t=2;t<=(int)(sqrt(i));t++){
			if(i%t==0){
				flag=1;
				break;
			}
		}
		if(flag==0){
			p[j]=i;
			j++;
			if(j==len){
				leave=1;
			}
		}
		if(leave==1){
			break;
		}
	}
}
