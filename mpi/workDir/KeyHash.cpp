#include<iostream>
#include<string>
#include<cmath>
int primer[50];
using namespace std;
int KeyHash(string key){
	int sum=0;
	for(int i=0;i!=key.length();i++){
		sum+=(key[i]-'A')*primer[i];
	}
	return sum;
}

void priGen(int *primer){
	int j=0;
	int i;
	for(i=1;i!=1000;i++){
		int flag=0;
		int leave=0;
		for(int t=2;t<=(int)(sqrt(i));t++){
			//cout<<c%t<<endl;
			if(i%t==0){
				flag=1;
				break;
			}
		}
		if(flag==0){
			primer[j]=i;
			j++;
			if(j==50){
				leave=1;	
			}
		}
		if(leave==1){
			break;
		}
	}
	//cout<<i<<endl;

}

int main(){
	priGen(primer);
	string s("destino");
	string s2("lavitb");
	cout<<KeyHash(s)<<endl;
	cout<<KeyHash(s2)<<endl;
}
