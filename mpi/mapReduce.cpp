#include<iostream>
#include<sstream>
#include<fstream>
#include<unistd.h>
#include<sys/time.h>
#include"mapReduce.h"
using namespace std;
KV::~KV(){
}
KV::KV(){
}
void Map(string fileName,int reduceNum){
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
	usleep(200000);
}

void Reduce(){
	usleep(300000);
}

