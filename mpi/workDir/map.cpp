#include<iostream>
#include<map>
#include<string>
using namespace std;
int main(){
	map<string, string> m;
	int t;
	while((cin>>t)&&t!=0){
		string key;
		string value;
		cout<<"input key value"<<endl;
		cin>>key>>value;
		m[key]+=value;
	}
	for(map<string,string>::iterator it=m.begin();it!=m.end();it++){
		cout<<it->first<<" "<<it->second<<endl;
	}
	return 0;
}
