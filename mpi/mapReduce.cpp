#include<iostream>
#include<sstream>
#include<fstream>
#include<unistd.h>
#include<sys/time.h>
#include<map>
#include"mapReduce.h"
using namespace std;
string itostr(int i){//untest
	stringstream ss;
	ss<<i;
	string s;
	ss>>s;
	return s;
}
void stoch(string s,char *ch){
	for(int i=0;i!=s.length();i++){
		ch[i]=s[i];
	}
	ch[s.length()]='\0';
}
string Map(string fileName){
	ifstream file;
	ofstream output;
	char fileIn[100];
	char fileOut[100];
	stoch(fileName,fileIn);
	string fileOutN(fileName);
	fileOutN+="Medi";
	stoch(fileOutN,fileOut);
	file.open(fileIn,ios::in);
	output.open(fileOut,ios::out|ios::trunc);
	if(file.is_open()&&output.is_open()){
		do{
			string s;
			file>>s;
			//cout<<s<<endl;
			if(file.eof()){
				break;
			}else{
				if(s[0]<'A'|s[0]>'z'){
				}else{
					if(s[s.length()-1]<'A'||s[s.length()-1]>'z')
						s.erase(s.length()-1);
					string outStr;
					outStr="{";
					outStr+="key:"+s+","+"value:1"+"}";
					output<<outStr<<endl;
				}
			}
		}while(!file.eof());
		file.close();
		output.close();
	}
	return fileOutN;
}

string Reduce(string fileName,int reduceId){
	char fileTmp[100];
	char outTmp[100];
	string outfile="reduceOut1"+itostr(reduceId);
	stoch(fileName,fileTmp);
	stoch(outfile,outTmp);
	ifstream file;
	ofstream output;
	file.open(fileTmp,ios::in);
	output.open(outTmp,ios::out|ios::trunc);
	if(file.is_open()&&output.is_open()){
		while(!file.eof()){
			string s;
			getline(file,s);
			if(file.eof()){
				break;
			}
			KV kv;
			parse(s,kv);
			//cout<<"begin here"<<kv.value.length()<<endl;
			kv.value=kv.value.substr(1,kv.value.length()-2);
			//cout<<"not bad"<<kv.value.length()<<endl;
			
			long sum=0;
			for(int i=0;i<kv.value.length();i=i+2){
				sum+=kv.value[i]-'0';
				if(kv.value[i]<'0'){
					cout<<kv.value[i]<<endl;
				}
			}
			string s1;
			s1+=kv.key+":"+itostr(sum);
			output<<s1<<endl;
			
		}
		cout<<"close file"<<endl;
		output.close();
		file.close();
	}
	return outfile;
}
void parse(string s,KV &kv){
	int kpos=s.find_first_of("key",0);
	int vpos=s.find_first_of(",",0)+1;
	int len=vpos-1-(kpos+4);
	kv.key+=s.substr(kpos+4,len);
	int len2=s.length()-1-(vpos+6);
	kv.value+=s.substr(vpos+6,len2);
}
int KeyHash(string s,int *p){
	int sum=0;
	for(int i=0;i!=s.length();i++){
		sum+=(s[i]-'A')*p[i];
	}
	if(sum<0){
		sum=-sum;
	}
	return sum;
}
