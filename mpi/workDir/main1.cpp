#include<iostream>
#include<fstream>
#include<string>
using namespace std;

/*
	use this to test parser the key value pair
*/
typedef struct KV{
	string key;
	string value;
}KV;
void printKV(KV kv){
	cout<<kv.key<<" , "<<kv.value<<endl;
}
void parse(string s,KV &kv){
	int kpos=s.find_first_of("key",0);
	int vpos=s.find_first_of(",",0)+1;
	int len=vpos-1-(kpos+4);
	kv.key+=s.substr(kpos+4,len);
	int len2=s.length()-1-(vpos+6);
	kv.value+=s.substr(vpos+6,len2);
}
int main(int argc,char *argv[]){
	string fileName=argv[1];
	char fileN[100];
	for(int i=0;i!=fileName.length();i++){
		fileN[i]=fileName[i];
	}
	fileN[fileName.length()]='\0';
	ifstream file;
	file.open(fileN,ios::in);
	do{
		string s;
		getline(file,s);
		if(file.eof()){
			break;
		}
		KV kv;
		parse(s,kv);
		printKV(kv);

	}while(!file.eof());
	return 0;
}
