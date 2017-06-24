#include<iostream>
#include<string>
using namespace std;
void stoch(string s,char *ch){
	for(int i=0;i!=s.length();i++){
		ch[i]=s[i];
	}
	ch[s.length()]='\0';
}

int main(){
	string s("ahahha");
	char a[100];
	stoch(s,a);
	cout<<a<<endl;
	return 0;
}

