#include<iostream>
#include<fstream>
#include<string>
using namespace std;

int main(int argc,char* argv[]){
	string fileName;
	fileName=argv[1];
	ifstream file;
	ofstream input;
	file.open(argv[1],ios::out);
	input.open("output1",ios::in|ios::trunc);
	if(file.is_open())
	{
		if(input.is_open()){
			do{
				string s;
				file>>s;
				if(file.eof()){
				}else{
					if(s[0]<'A'||s[0]>'z'){
					//	cout<<s[0]<<endl;	
					}else{
						if(s[s.length()-1]<'A'||s[s.length()-1]>'z'){
							s.erase(s.length()-1);
						}
						s="{"+s+","+"1}";
						input<<s<<endl;
					}
				}
			}while(!file.eof());
			input.close();
		}
		file.close();
	}
	/*wg*/


	return 0;
}
