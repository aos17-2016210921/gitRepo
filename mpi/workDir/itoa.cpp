#include<iostream>
#include<sstream>
#include<string>
using namespace std;

int main(){
	int inter;
	cin>>inter;
	stringstream ss;
	ss<<inter;
	string s;
	ss>>s;
	cout<<s<<endl;
	return 0;
}
