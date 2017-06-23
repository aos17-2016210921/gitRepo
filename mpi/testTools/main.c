/* this is a program to test my idea which try to implement a easy gabage allocate*/
#include<stdio.h>
#include<stdlib.h>
#define MAX 10
typedef struct mallocSet{
	int* intMalloc[MAX];
	double* doubleMalloc[MAX];
	int intLen,doubleLen;
}mallocSet;
int registInt(mallocSet ms,int* p){
	if(ms.intLen==MAX){
		return 1;		
	}else{
		ms.intMalloc[ms.intLen]=p;
		ms.intLen++;
		return 0;
	}
}
int registDouble(mallocSet ms,double* p){
	if(ms.doubleLen==MAX){
		return 1;
	}else{
		ms.doubleMalloc[ms.doubleLen]=p;
		ms.doubleLen++;
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
	return 0;
}
void init(mallocSet* ms){
	ms->intLen=0;
	ms->doubleLen=0;
}
int main(int argc,char* argv[]){
	printf("haha");
	mallocSet ms;
	init(&ms);
//	ms.intLen=0;
//	ms.doubleLen=0;
	printf("1\n");
	for(int i=0;i!=5;i++){
		int *p;
		p=malloc(sizeof(int)*(2));
		//printf("len:%d %d\n",ms.intLen,ms.doubleLen);
		registInt(ms,p);
	}
	printf("hahah");
	for(int i=0;i!=3;i++){
		double *p;
		p=malloc(sizeof(double)*(i+3));
		registDouble(ms,p);
	}
	freeMalloc(ms);
	return 0;
}
