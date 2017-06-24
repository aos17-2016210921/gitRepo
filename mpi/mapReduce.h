#ifndef _MAPREDUCE_H_
#define _MAPREDUCE_H_
#include<fstream>
#include<string>
typedef struct KV{
	string key;
	string value;
}KV;
void Map(std::string fileName,int reduceNum);
void Reduce();

#endif
