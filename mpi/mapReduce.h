#ifndef _MAPREDUCE_H_
#define _MAPREDUCE_H_
#include<fstream>
#include<string>
typedef struct KV{
	std::string key;
	std::string value;
}KV;
void stoch(std::string,char *ch);
std::string itostr(int i);
std::string Map(std::string fileName);
std::string Reduce(std::string fineName,int reduceId);
int KeyHash(std::string s,int *p);
void parse(std::string s,KV &kv);
#endif
