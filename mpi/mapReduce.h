#ifndef _MAPREDUCE_H_
#define _MAPREDUCE_H_
#include<fstream>
#include<string>
class KV{
	std::string key;
	std::string value;
public:
	~KV();
	KV();
	KV(std::string k,std::string v){
		key=k;
		value=v;
	}

};

void Map(std::string fileName,int reduceNum);
void Reduce();

#endif
