#include<stdio.h>
void vans(char *c2,char *c1){
	int i=0;
	while(c1[i]!='\0'){
		c2[i]=c1[i];
		i++;
	}
	c2[i]='\0';
}
int main(){
	char c1[100]="asfasd";
	char c2[2][100];
	int i=0;
	vans(c2[1],c1);
	i=0;
	while(c2[1][i]!='\0'){
		printf("%c",c2[1][i]);
		i++;
	}


}
