#include<iostream>
#include<fstream>
using namespace std;
int main(){
	ofstream* out;
	out=new ofstream [4];
	for(int i=0;i!=4;i++){
		out[i].open("test",ios::out);
		if(out[i].is_open()){
			out[i]<<"haha"<<i<<endl;
			out[i].close();
			cout<<i<<endl;
		}
	}
	return 0;
}
