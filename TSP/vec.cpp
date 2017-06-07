#include<iostream>
#include<vector>
using namespace std;
int main(){
	vector<int*> vec;

	for(int i=0;i!=10;i++){
		int* tmp;
		tmp=new int[10];
		vec.push_back(tmp);
	}
	for(vector<int*>::iterator it=vec.begin();it!=vec.end();it++){
		delete [] *it;
	}
	vec.clear();
	cout<<vec.size();

	return 0;
}
