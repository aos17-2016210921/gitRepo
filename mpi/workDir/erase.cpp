#include<iostream>
#include<string>

using namespace std;

int main(){
	string s;
	cin>>s;
	s=s.substr(1,s.length()-2);
//	s.erase(s.length()-2);
	cout<<s<<endl;
}
