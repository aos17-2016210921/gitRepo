#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;
string itostr(int i){
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
int main(){
	ifstream file;
	file.open("test",ios::in);
	if(file.is_open()){
		file.seekg(0,ios::end);
		int m=file.tellg();
		cout<<m<<endl;
		ifstream file2[3];
		int offset[3];
		for(int i=0;i!=3;i++){
			file2[i].open("test",ios::in);
			file2[i].seekg(i*m/3,ios::cur);
			if(i!=0){
				string s;
				file2[i]>>s;
			}
			offset[i]=file2[i].tellg();
	
		}
		for(int i=0;i!=3;i++){
			
			int m2=0;
			ofstream out;
			string outname("out");
			outname+=itostr(i);
			//cout<<outname<<endl;
			char outch[100];
			stoch(outname,outch);
			cout<<outch<<endl;
			out.open(outch,ios::out|ios::trunc);
			int m3;
			if(i==2){
				m3=m;
			}else{
				m3=offset[i+1];
			}
			cout<<m3<<" "<<i<<endl;
			while(m2<m3&&m2>=0){
				string s;
				file2[i]>>s;
				out<<s<<endl;
				m2=file2[i].tellg();
			}
			file2[i].close();
			out.close();
		}
		file.close();
		remove("test");
	}
	return 0;
}
